// Copyright 2021 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_ASH_COMPONENTS_DBUS_USERDATAAUTH_FAKE_CRYPTOHOME_MISC_CLIENT_H_
#define CHROMEOS_ASH_COMPONENTS_DBUS_USERDATAAUTH_FAKE_CRYPTOHOME_MISC_CLIENT_H_

#include "chromeos/ash/components/dbus/userdataauth/cryptohome_misc_client.h"

#include "base/component_export.h"
#include "chromeos/ash/components/dbus/cryptohome/UserDataAuth.pb.h"

namespace ash {

class COMPONENT_EXPORT(USERDATAAUTH_CLIENT) FakeCryptohomeMiscClient
    : public CryptohomeMiscClient {
 public:
  FakeCryptohomeMiscClient();
  ~FakeCryptohomeMiscClient() override;

  // Not copyable or movable.
  FakeCryptohomeMiscClient(const FakeCryptohomeMiscClient&) = delete;
  FakeCryptohomeMiscClient& operator=(const FakeCryptohomeMiscClient&) = delete;

  // Checks that a FakeCryptohomeMiscClient instance was initialized and returns
  // it.
  static FakeCryptohomeMiscClient* Get();

  // CryptohomeMiscClient override:
  void WaitForServiceToBeAvailable(
      chromeos::WaitForServiceToBeAvailableCallback callback) override;
  void GetSystemSalt(const ::user_data_auth::GetSystemSaltRequest& request,
                     GetSystemSaltCallback callback) override;
  void GetSanitizedUsername(
      const ::user_data_auth::GetSanitizedUsernameRequest& request,
      GetSanitizedUsernameCallback callback) override;
  void GetLoginStatus(const ::user_data_auth::GetLoginStatusRequest& request,
                      GetLoginStatusCallback callback) override;
  void LockToSingleUserMountUntilReboot(
      const ::user_data_auth::LockToSingleUserMountUntilRebootRequest& request,
      LockToSingleUserMountUntilRebootCallback callback) override;
  void GetRsuDeviceId(const ::user_data_auth::GetRsuDeviceIdRequest& request,
                      GetRsuDeviceIdCallback callback) override;

  absl::optional<::user_data_auth::GetSanitizedUsernameReply>
  BlockingGetSanitizedUsername(
      const ::user_data_auth::GetSanitizedUsernameRequest& request) override;

  // Various getter/setters:

  // Sets the system salt which will be returned from GetSystemSalt(). By
  // default, GetSystemSalt() returns the value generated by
  // GetStubSystemSalt().
  void set_system_salt(const std::vector<uint8_t>& system_salt) {
    system_salt_ = system_salt;
  }

  // Returns the stub system salt as raw bytes. (not as a string encoded in the
  // format used by SystemSaltGetter::ConvertRawSaltToHexString()).
  static std::vector<uint8_t> GetStubSystemSalt();

  // Set whether we say the system needs powerwash the next time CheckHealth()
  // is called.
  void set_requires_powerwash(bool requires_powerwash) {
    requires_powerwash_ = requires_powerwash;
  }

  // Was LockToSingleUserMountUntilReboot() called?
  bool is_device_locked_to_single_user() const {
    return is_device_locked_to_single_user_;
  }

  // Sets the RSU ID that we return for GetRsuDeviceId().
  void set_rsu_device_id(const std::string& rsu_device_id) {
    rsu_device_id_ = rsu_device_id;
  }

  // Sets the CryptohomeError value to return.
  void set_cryptohome_error(::user_data_auth::CryptohomeErrorCode error) {
    cryptohome_error_ = error;
  }

  // WaitForServiceToBeAvailable() related:

  // Changes the behavior of WaitForServiceToBeAvailable(). This method runs
  // pending callbacks if is_available is true.
  void SetServiceIsAvailable(bool is_available);

  // Runs pending availability callbacks reporting that the service is
  // unavailable. Expects service not to be available when called.
  void ReportServiceIsNotAvailable();

 private:
  // Helper that returns the protobuf reply.
  template <typename ReplyType>
  void ReturnProtobufMethodCallback(
      const ReplyType& reply,
      chromeos::DBusMethodCallback<ReplyType> callback);

  // The next error code to return for various functions.
  ::user_data_auth::CryptohomeErrorCode cryptohome_error_ =
      ::user_data_auth::CryptohomeErrorCode::CRYPTOHOME_ERROR_NOT_SET;

  // The system salt to return.
  std::vector<uint8_t> system_salt_{GetStubSystemSalt()};

  // Whether we say we need to powerwash the next time CheckHealth() is called.
  bool requires_powerwash_ = false;

  // Used by LockToSingleUserMountUntilReboot.
  bool is_device_locked_to_single_user_ = false;

  // Reply for GetRsuDeviceId().
  std::string rsu_device_id_;

  // WaitForServiceToBeAvailable() related fields:

  // If set, we tell callers that service is available.
  bool service_is_available_ = true;

  // If set, WaitForServiceToBeAvailable will run the callback, even if service
  // is not available (instead of adding the callback to pending callback list).
  bool service_reported_not_available_ = false;

  // The list of callbacks passed to WaitForServiceToBeAvailable when the
  // service wasn't available.
  std::vector<chromeos::WaitForServiceToBeAvailableCallback>
      pending_wait_for_service_to_be_available_callbacks_;
};

}  // namespace ash

#endif  // CHROMEOS_ASH_COMPONENTS_DBUS_USERDATAAUTH_FAKE_CRYPTOHOME_MISC_CLIENT_H_