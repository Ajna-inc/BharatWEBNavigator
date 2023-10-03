// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ash/shimless_rma/chrome_shimless_rma_delegate.h"

#include <utility>

#include "ash/constants/ash_features.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "base/run_loop.h"
#include "base/strings/stringprintf.h"
#include "base/task/sequenced_task_runner.h"
#include "base/test/bind.h"
#include "base/test/scoped_feature_list.h"
#include "base/test/test_future.h"
#include "chrome/browser/ash/shimless_rma/diagnostics_app_profile_helper.h"
#include "chrome/browser/ash/shimless_rma/diagnostics_app_profile_helper_constants.h"
#include "chrome/browser/extensions/extension_garbage_collector_factory.h"
#include "chrome/browser/extensions/extension_service_test_base.h"
#include "chrome/browser/extensions/test_extension_system.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/web_applications/web_app_command_scheduler.h"
#include "chrome/common/pref_names.h"
#include "chrome/services/qrcode_generator/public/cpp/qrcode_generator_service.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile_manager.h"
#include "chromeos/ash/components/browser_context_helper/browser_context_types.h"
#include "chromeos/constants/chromeos_features.h"
#include "components/variations/scoped_variations_ids_provider.h"
#include "content/public/test/browser_task_environment.h"
#include "content/public/test/fake_service_worker_context.h"
#include "extensions/common/constants.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace ash::shimless_rma {
namespace {

const char kTestCrxPath[] = "chrome/test/data/chromeos/3p_diagnostics/diag.crx";
const char kTestWrongIdCrxPath[] =
    "chrome/test/data/chromeos/3p_diagnostics/diag-wrong-id.crx";
// The test wrong id generated by the key signing the above crx.
const char kTestWrongExtId[] = "neacocmolncbbnnameegalgmoedgpfpk";
// The IWA installation is not tested in unit test. So we don't need a real
// IWA.
const char kFakeIwaPath[] = "fake_iwa_path.swbn";
}  // namespace

// Test-fake implementation of QRImageGenerator; the real implementation
// can't be used in these tests because it may require spawning a service
// process.
void GenerateFakeQRCode(
    qrcode_generator::mojom::GenerateQRCodeRequestPtr request,
    qrcode_generator::QRImageGenerator::ResponseCallback callback) {
  qrcode_generator::mojom::GenerateQRCodeResponsePtr response =
      qrcode_generator::mojom::GenerateQRCodeResponse::New();
  response->error_code = qrcode_generator::mojom::QRCodeGeneratorError::NONE;
  response->bitmap.allocN32Pixels(16, 16);

  std::move(callback).Run(std::move(response));
}

class ChromeShimlessRmaDelegateTest : public testing::Test {
 public:
  ChromeShimlessRmaDelegateTest()
      : chrome_shimless_rma_delegate_(ChromeShimlessRmaDelegate(nullptr)),
        task_environment_(content::BrowserTaskEnvironment::REAL_IO_THREAD) {}
  ~ChromeShimlessRmaDelegateTest() override = default;

  void SetUp() override {
    chrome_shimless_rma_delegate_.SetQRCodeServiceForTesting(
        base::BindRepeating(&GenerateFakeQRCode));
  }

  void TearDown() override {}

 protected:
  ChromeShimlessRmaDelegate chrome_shimless_rma_delegate_;

 private:
  content::BrowserTaskEnvironment task_environment_;
};

// Validates a QrCode Bitmap is correctly converted to a string.
TEST_F(ChromeShimlessRmaDelegateTest, GenerateQrCode) {
  base::RunLoop run_loop;
  chrome_shimless_rma_delegate_.GenerateQrCode(
      "www.sample-url.com",
      base::BindLambdaForTesting([&](const std::string& qr_code_image) {
        EXPECT_FALSE(qr_code_image.empty());
      }));
  run_loop.RunUntilIdle();
}

class FakeServiceWorkerContext : public content::FakeServiceWorkerContext {
 public:
  FakeServiceWorkerContext() = default;
  FakeServiceWorkerContext(const FakeServiceWorkerContext&) = delete;
  ~FakeServiceWorkerContext() override = default;

  void CheckHasServiceWorker(const GURL& url,
                             const blink::StorageKey& key,
                             CheckHasServiceWorkerCallback callback) override {
    content::ServiceWorkerCapability result =
        content::ServiceWorkerCapability::SERVICE_WORKER_NO_FETCH_HANDLER;
    if (service_worker_check_retry_ > 0) {
      --service_worker_check_retry_;
      result = content::ServiceWorkerCapability::NO_SERVICE_WORKER;
    }
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE, base::BindOnce(std::move(callback), result));
  }

  void set_service_worker_check_retry(int64_t retry) {
    service_worker_check_retry_ = retry;
  }

 private:
  // The times until the check return service worker found.
  int64_t service_worker_check_retry_ = 2;
};

class FakeWebAppCommandScheduler : public web_app::WebAppCommandScheduler {
 public:
  using web_app::WebAppCommandScheduler::WebAppCommandScheduler;

  void InstallIsolatedWebApp(
      const web_app::IsolatedWebAppUrlInfo& url_info,
      const web_app::IsolatedWebAppLocation& location,
      const absl::optional<base::Version>& expected_version,
      std::unique_ptr<ScopedKeepAlive> keep_alive,
      std::unique_ptr<ScopedProfileKeepAlive> profile_keep_alive,
      web_app::WebAppCommandScheduler::InstallIsolatedWebAppCallback callback,
      const base::Location& call_location) override {
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE, base::BindOnce(std::move(callback),
                                  web_app::InstallIsolatedWebAppCommandSuccess{
                                      base::Version{}}));
  }
};

class FakeDiagnosticsAppProfileHelperDelegate
    : public DiagnosticsAppProfileHelperDelegate {
 public:
  explicit FakeDiagnosticsAppProfileHelperDelegate(Profile* profile)
      : web_app_command_scheduler_(*profile) {}
  FakeDiagnosticsAppProfileHelperDelegate(
      const DiagnosticsAppProfileHelperDelegate&) = delete;
  ~FakeDiagnosticsAppProfileHelperDelegate() override = default;

  content::ServiceWorkerContext* GetServiceWorkerContextForExtensionId(
      const extensions::ExtensionId& extension_id,
      content::BrowserContext* browser_context) override {
    return &fake_service_worker_context_;
  }

  web_app::WebAppCommandScheduler* GetWebAppCommandScheduler(
      content::BrowserContext* browser_context) override {
    return &web_app_command_scheduler_;
  }

  FakeServiceWorkerContext& fake_service_worker_context() {
    return fake_service_worker_context_;
  }

 protected:
  FakeServiceWorkerContext fake_service_worker_context_;
  FakeWebAppCommandScheduler web_app_command_scheduler_;
};

class ChromeShimlessRmaDelegatePrepareDiagnosticsAppProfileTest
    : public extensions::ExtensionServiceTestBase {
 public:
  ChromeShimlessRmaDelegatePrepareDiagnosticsAppProfileTest()
      : extensions::ExtensionServiceTestBase(
            std::make_unique<content::BrowserTaskEnvironment>(
                base::test::TaskEnvironment::TimeSource::MOCK_TIME)) {}

  void SetUp() override {
    extensions::ExtensionServiceTestBase::SetUp();

    feature_list_.InitWithFeatures(
        {
            ash::features::kShimlessRMA3pDiagnostics,
            ash::features::kShimlessRMA3pDiagnosticsDevMode,
            chromeos::features::kIWAForTelemetryExtensionAPI,
        },
        {});
    testing_profile_manager_.SetUp();
    TestingProfile* profile = testing_profile_manager_.CreateTestingProfile(
        kShimlessRmaAppBrowserContextBaseName);

    fake_diagnostics_app_profile_helper_delegate_ =
        std::make_unique<FakeDiagnosticsAppProfileHelperDelegate>(profile);

    InitializeExtensionSystem(profile);

    chrome_shimless_rma_delegate_
        .SetDiagnosticsAppProfileHelperDelegateForTesting(
            fake_diagnostics_app_profile_helper_delegate_.get());
  }

  void InitializeExtensionSystem(Profile* profile) {
    auto extensions_install_dir =
        profile->GetPath().AppendASCII(extensions::kInstallDirectoryName);
    auto unpacked_install_dir = profile->GetPath().AppendASCII(
        extensions::kUnpackedInstallDirectoryName);

    extensions::TestExtensionSystem* system =
        static_cast<extensions::TestExtensionSystem*>(
            extensions::ExtensionSystem::Get(profile));
    auto* service = system->CreateExtensionService(
        base::CommandLine::ForCurrentProcess(), extensions_install_dir,
        unpacked_install_dir, true, true);

    // When we start up, we want to make sure there is no external provider,
    // since the ExtensionService on Windows will use the Registry as a default
    // provider and if there is something already registered there then it will
    // interfere with the tests. Those tests that need an external provider
    // will register one specifically.
    service->ClearProvidersForTesting();

    service->Init();

    // Garbage collector is typically NULL during tests, so give it a build.
    extensions::ExtensionGarbageCollectorFactory::GetInstance()
        ->SetTestingFactoryAndUse(
            profile,
            base::BindRepeating(&extensions::ExtensionGarbageCollectorFactory::
                                    BuildInstanceFor));
  }

  void TearDown() override { extensions::ExtensionServiceTestBase::TearDown(); }

  using Result = base::expected<
      ChromeShimlessRmaDelegate::PrepareDiagnosticsAppBrowserContextResult,
      std::string>;
  Result PrepareDiagnosticsAppBrowserContext(const base::FilePath& crx_path) {
    base::test::TestFuture<Result> future;
    chrome_shimless_rma_delegate_.PrepareDiagnosticsAppBrowserContext(
        crx_path, base::FilePath{kFakeIwaPath}, future.GetCallback());
    return future.Get();
  }

 protected:
  base::test::ScopedFeatureList feature_list_;
  TestingProfileManager testing_profile_manager_{
      TestingBrowserProcess::GetGlobal(), &testing_local_state_};
  variations::ScopedVariationsIdsProvider scoped_variations_ids_provider_{
      variations::VariationsIdsProvider::Mode::kUseSignedInState};
  std::unique_ptr<FakeDiagnosticsAppProfileHelperDelegate>
      fake_diagnostics_app_profile_helper_delegate_;
  ChromeShimlessRmaDelegate chrome_shimless_rma_delegate_{nullptr};
};

// Verify the whole flow of `PrepareDiagnosticsAppProfile`.
TEST_F(ChromeShimlessRmaDelegatePrepareDiagnosticsAppProfileTest, Success) {
  // Call this twice to verify that even if the profile has already been loaded
  // it still works.
  for (int i = 0; i < 2; ++i) {
    auto result = PrepareDiagnosticsAppBrowserContext(
        base::PathService::CheckedGet(base::DIR_SRC_TEST_DATA_ROOT)
            .Append(kTestCrxPath));

    EXPECT_TRUE(result.has_value());

    content::BrowserContext* context = result->context;
    EXPECT_FALSE(context->IsOffTheRecord());
    EXPECT_TRUE(Profile::FromBrowserContext(context)->GetPrefs()->GetBoolean(
        prefs::kForceEphemeralProfiles));
  }
}

// Verify that we denied extensions which is not in the ChromeOS system
// extension allowlist.
TEST_F(ChromeShimlessRmaDelegatePrepareDiagnosticsAppProfileTest,
       NotChromeOSSystemExtension) {
  auto result = PrepareDiagnosticsAppBrowserContext(
      base::PathService::CheckedGet(base::DIR_SRC_TEST_DATA_ROOT)
          .Append(kTestWrongIdCrxPath));

  EXPECT_FALSE(result.has_value());
  EXPECT_EQ(result.error(),
            base::StringPrintf(k3pDiagErrorNotChromeOSSystemExtension,
                               kTestWrongExtId));
}

// Verify the service worker polling logic break after reaching the timeout.
TEST_F(ChromeShimlessRmaDelegatePrepareDiagnosticsAppProfileTest,
       ServiceWorkerTimeout) {
  // To hit the timeout, we need to retry more than ((timeout / polling
  // interval) + 1) times.
  int64_t retry_times = k3pDiagExtensionReadyPollingTimeout.IntDiv(
                            k3pDiagExtensionReadyPollingInterval) +
                        1;
  fake_diagnostics_app_profile_helper_delegate_->fake_service_worker_context()
      .set_service_worker_check_retry(retry_times);

  auto result = PrepareDiagnosticsAppBrowserContext(
      base::PathService::CheckedGet(base::DIR_SRC_TEST_DATA_ROOT)
          .Append(kTestCrxPath));

  EXPECT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), k3pDiagErrorCannotActivateExtension);
}

}  // namespace ash::shimless_rma