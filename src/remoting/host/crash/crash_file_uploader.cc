// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "remoting/host/crash/crash_file_uploader.h"

#include <list>
#include <memory>
#include <string>
#include <utility>

#include "base/files/file_enumerator.h"
#include "base/files/file_util.h"
#include "base/functional/bind.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "base/task/single_thread_task_runner.h"
#include "base/task/single_thread_task_runner_thread_mode.h"
#include "base/task/task_traits.h"
#include "base/task/thread_pool.h"
#include "net/base/load_flags.h"
#include "net/base/mime_util.h"
#include "net/base/net_errors.h"
#include "net/traffic_annotation/network_traffic_annotation.h"
#include "remoting/base/breakpad_utils_linux.h"
#include "services/network/public/cpp/resource_request.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "services/network/public/cpp/simple_url_loader.h"
#include "services/network/public/mojom/url_response_head.mojom.h"

namespace remoting {

namespace {

// Post multi-part form-data names for the upload request.
constexpr char kProductNameKey[] = "prod";
constexpr char kProductVersionKey[] = "ver";
constexpr char kProcessUptimeKey[] = "ptime";
constexpr char kMinidumpFileKey[] = "upload_file_minidump";
constexpr char kMinidumpFileName[] = "dump";

constexpr char kProductNameValue[] = "Chromoting_Linux";

constexpr char kCrashReportUploadUrl[] =
    "https://clients2.google.com/cr/report";

// Used to reserve space for the non-file data in the post form to prevent
// string re-allocations which building up the multi-part post form.
constexpr int kPostFormReservationSize = 4096;

// A successful response should only contain the report_id so this is bigger
// than is needed for a typical response but it prevents a ridiculously large
// value and will ensure we don't reject the response if the format changes in
// the future.
constexpr size_t kMaxResponseSize = 1024;

constexpr net::NetworkTrafficAnnotationTag kTrafficAnnotation =
    net::DefineNetworkTrafficAnnotation("crash_file_uploader",
                                        R"(
        semantics {
          sender: "Chrome Remote Desktop"
          description:
            "Uploads crash reports generated by Chrome Remote Desktop."
          trigger:
            "The upload request is made when a minidump file is generated by "
            "the Chrome Remote Desktop host service after the user has opted "
            "into crash reporting."
          data: "Minidump file and product-specific info (e.g. version)."
          destination: GOOGLE_OWNED_SERVICE
          internal {
            contacts {
              email: "chromoting-team@google.com"
            }
          }
          user_data {
            type: ARBITRARY_DATA
          }
          last_reviewed: "2023-05-12"
        }
        policy {
          cookies_allowed: NO
          setting:
            "This request will not be sent if crash reporting is not enabled "
            "for Chrome Remote Desktop."
          policy_exception_justification:
            "Not implemented."
        })");

base::FilePath GetCrashDirectoryPath(const std::string& crash_guid) {
  return base::FilePath(kMinidumpPath).Append(crash_guid);
}

base::FilePath GetDumpFilePath(const std::string& crash_guid) {
  return GetCrashDirectoryPath(crash_guid).Append(crash_guid + ".dmp");
}

base::FilePath GetMetadataFilePath(const std::string& crash_guid) {
  return GetCrashDirectoryPath(crash_guid).Append(crash_guid + ".json");
}

bool RetrieveCrashReportDetails(const std::string& crash_guid,
                                std::string& minidump_file_contents,
                                base::Value::Dict& metadata,
                                std::string& error_reason) {
  base::FilePath minidump_file_path = GetDumpFilePath(crash_guid);
  if (!base::PathExists(minidump_file_path)) {
    error_reason = "Upload directory is missing the minidump file";
    return false;
  }

  std::string minidump_string;
  if (!base::ReadFileToString(minidump_file_path, &minidump_string)) {
    error_reason = "Failed to read dump file contents";
    return false;
  }

  base::FilePath metadata_file = GetMetadataFilePath(crash_guid);
  if (!base::PathExists(metadata_file)) {
    error_reason = "Upload directory is missing the metadata file";
    return false;
  }

  std::string metadata_file_contents;
  if (!base::ReadFileToString(metadata_file, &metadata_file_contents)) {
    error_reason = "Failed to read metadata file";
    return false;
  }

  absl::optional<base::Value::Dict> opt_metadata =
      base::JSONReader::ReadDict(metadata_file_contents);
  if (!opt_metadata.has_value()) {
    error_reason = "Failed to parse metadata file contents";
    return false;
  }

  // Ensure the metadata file has the required fields. Note that the metadata
  // file may contain additional fields (e.g. for troubleshooting or manual
  // uploading) but there are only a few which are required for the upload.
  const std::string* version =
      opt_metadata->FindString(kBreakpadHostVersionKey);
  if (version == nullptr || version->empty()) {
    error_reason = "Metadata file is missing the product version field";
    return false;
  }
  const std::string* uptime =
      opt_metadata->FindString(kBreakpadProcessUptimeKey);
  if (uptime == nullptr || uptime->empty()) {
    error_reason = "Metadata file is missing the process uptime field";
    return false;
  }

  metadata = std::move(*opt_metadata);
  minidump_file_contents = std::move(minidump_string);
  return true;
}

void DeleteDumpFileAndWriteResult(const base::FilePath& crash_file,
                                  const std::string& result) {
  if (!base::DeleteFile(crash_file)) {
    LOG(WARNING) << "Failed to delete minidump file: " << crash_file.value();
  }

  base::FilePath result_file = crash_file.DirName().Append("upload_result.txt");
  if (!base::WriteFile(result_file, result + "\r\n")) {
    LOG(WARNING) << "Failed to write upload result to: " << result_file.value();
  }
}

std::unique_ptr<network::SimpleURLLoader> CreateSimpleUrlLoader() {
  auto resource_request = std::make_unique<network::ResourceRequest>();
  resource_request->url = GURL(kCrashReportUploadUrl);
  resource_request->load_flags =
      net::LOAD_BYPASS_CACHE | net::LOAD_DISABLE_CACHE;
  resource_request->credentials_mode = network::mojom::CredentialsMode::kOmit;
  resource_request->method = net::HttpRequestHeaders::kPostMethod;

  std::unique_ptr<network::SimpleURLLoader> simple_url_loader =
      network::SimpleURLLoader::Create(std::move(resource_request),
                                       kTrafficAnnotation);
  simple_url_loader->SetTimeoutDuration(base::Seconds(60));
  simple_url_loader->SetAllowHttpErrorResults(false);
  simple_url_loader->SetRetryOptions(
      3, network::SimpleURLLoader::RETRY_ON_5XX |
             network::SimpleURLLoader::RETRY_ON_NETWORK_CHANGE);

  return simple_url_loader;
}

void GenerateMultiPartPostData(const base::Value::Dict& metadata,
                               const std::string& minidump_data,
                               std::string& post_data,
                               std::string& content_type) {
  post_data.clear();
  content_type.clear();

  post_data.reserve(minidump_data.size() + kPostFormReservationSize);

  std::string mime_boundary = net::GenerateMimeMultipartBoundary();

  // Add the product name.
  net::AddMultipartValueForUpload(kProductNameKey, kProductNameValue,
                                  mime_boundary, std::string(), &post_data);
  // Add the product version.
  const std::string* version = metadata.FindString(kBreakpadHostVersionKey);
  net::AddMultipartValueForUpload(kProductVersionKey, *version, mime_boundary,
                                  std::string(), &post_data);
  // Add the process uptime.
  const std::string* uptime = metadata.FindString(kBreakpadProcessUptimeKey);
  net::AddMultipartValueForUpload(kProcessUptimeKey, *uptime, mime_boundary,
                                  std::string(), &post_data);
  // Add the minidump file.
  net::AddMultipartValueForUploadWithFileName(
      kMinidumpFileKey, kMinidumpFileName, minidump_data, mime_boundary,
      "application/octet-stream", &post_data);
  // Add the final delimiter.
  net::AddMultipartFinalDelimiterForUpload(mime_boundary, &post_data);
  post_data.shrink_to_fit();

  content_type = "multipart/form-data; boundary=" + mime_boundary;
}

}  // namespace

class CrashFileUploader::Core {
 public:
  explicit Core(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory);
  Core(const Core&) = delete;
  Core& operator=(const Core&) = delete;
  ~Core();

  void Upload(const std::string& crash_guid);

 private:
  using SimpleURLLoaderList =
      std::list<std::unique_ptr<network::SimpleURLLoader>>;
  void OnUploadComplete(SimpleURLLoaderList::iterator it,
                        std::string crash_guid,
                        std::unique_ptr<std::string> response_body);

  scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory_;
  SimpleURLLoaderList simple_url_loaders_;

  THREAD_CHECKER(thread_checker_);

  base::WeakPtrFactory<Core> weak_ptr_factory_{this};
};

CrashFileUploader::Core::Core(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory)
    : url_loader_factory_(url_loader_factory) {
  DETACH_FROM_THREAD(thread_checker_);
}

CrashFileUploader::Core::~Core() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
}

void CrashFileUploader::Core::Upload(const std::string& crash_guid) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

  base::FilePath crash_report_directory = GetCrashDirectoryPath(crash_guid);
  LOG(INFO) << "Validating crash report files in "
            << crash_report_directory.value();

  if (!base::DirectoryExists(crash_report_directory)) {
    LOG(ERROR) << "Upload directory does not exist for report: " << crash_guid;
    return;
  }

  base::Value::Dict metadata;
  std::string minidump_data;
  std::string error;
  if (!RetrieveCrashReportDetails(crash_guid, minidump_data, metadata, error)) {
    LOG(ERROR) << "Failed to retrieve crash report details: " << error;
    DeleteDumpFileAndWriteResult(GetDumpFilePath(crash_guid), error);
    return;
  }

  LOG(INFO) << "Sending crash report (" << crash_guid << ") to "
            << kCrashReportUploadUrl;

  auto simple_url_loader = CreateSimpleUrlLoader();
  auto* simple_url_loader_ptr = simple_url_loader.get();
  auto it = simple_url_loaders_.insert(simple_url_loaders_.begin(),
                                       std::move(simple_url_loader));

  std::string post_data;
  std::string content_type;
  GenerateMultiPartPostData(metadata, minidump_data, post_data, content_type);
  simple_url_loader_ptr->AttachStringForUpload(post_data, content_type);
  simple_url_loader_ptr->DownloadToString(
      url_loader_factory_.get(),
      base::BindOnce(&CrashFileUploader::Core::OnUploadComplete,
                     weak_ptr_factory_.GetWeakPtr(), std::move(it), crash_guid),
      kMaxResponseSize);
}

void CrashFileUploader::Core::OnUploadComplete(
    SimpleURLLoaderList::iterator it,
    std::string crash_guid,
    std::unique_ptr<std::string> response_body) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

  std::string upload_result;
  if ((*it)->NetError() == net::OK) {
    std::string report_id = (response_body ? *response_body : "empty");
    // Result file format looks like:
    // report_id: <id_from_crash_service>
    // go/crash/<id_from_crash_service>
    upload_result =
        "report_id: " + report_id + "\r\n" + "http://go/crash/" + report_id;
    // Include the crash report id and go link in the host log.
    LOG(INFO) << "Successfully uploaded: " << crash_guid << ".dmp\r\n"
              << "    report_id: " << report_id << "\r\n"
              << "    http://go/crash/" << report_id << "\r\n"
              << "    Please note that it may take a few minutes to finish "
              << "processing the report.";
  } else {
    auto response_code = (*it)->ResponseInfo()->headers->response_code();
    LOG(ERROR) << "Failed to upload crash report: " << crash_guid << ".dmp"
               << ", response_code: " << response_code;
    upload_result =
        "Upload failed, response code: " + base::NumberToString(response_code);
  }

  simple_url_loaders_.erase(it);
  DeleteDumpFileAndWriteResult(GetDumpFilePath(crash_guid), upload_result);
}

CrashFileUploader::CrashFileUploader(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  core_task_runner_ = base::ThreadPool::CreateSingleThreadTaskRunner(
      {base::MayBlock(), base::TaskPriority::BEST_EFFORT},
      base::SingleThreadTaskRunnerThreadMode::DEDICATED);
  core_ = std::make_unique<CrashFileUploader::Core>(url_loader_factory);
}

CrashFileUploader::~CrashFileUploader() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  core_task_runner_->DeleteSoon(FROM_HERE, core_.release());
}

void CrashFileUploader::Upload(const std::string& crash_guid) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  core_task_runner_->PostTask(
      FROM_HERE, base::BindOnce(&CrashFileUploader::Core::Upload,
                                base::Unretained(core_.get()), crash_guid));
}

}  // namespace remoting