// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ash/capture_mode/capture_mode_controller.h"
#include "ash/capture_mode/capture_mode_test_util.h"
#include "ash/capture_mode/capture_mode_util.h"
#include "ash/constants/ash_features.h"
#include "ash/system/message_center/ash_notification_expand_button.h"
#include "ash/system/message_center/ash_notification_view.h"
#include "ash/system/message_center/message_popup_animation_waiter.h"
#include "ash/system/notification_center/notification_center_test_api.h"
#include "ash/system/unified/unified_system_tray.h"
#include "ash/test/ash_test_base.h"
#include "ash/test/ash_test_util.h"
#include "ash/test/pixel/ash_pixel_differ.h"
#include "ash/test/pixel/ash_pixel_test_init_params.h"
#include "base/strings/strcat.h"
#include "base/strings/utf_string_conversions.h"
#include "base/test/scoped_feature_list.h"
#include "chromeos/constants/chromeos_features.h"
#include "ui/base/models/image_model.h"
#include "ui/message_center/views/message_popup_view.h"
#include "ui/message_center/views/message_view.h"
#include "ui/message_center/views/notification_control_buttons_view.h"

namespace ash {

namespace {

constexpr char kScreenCaptureNotificationId[] = "capture_mode_notification";

constexpr char kShortTitleString[] = "Short Title";
constexpr char kMediumTitleString[] = "Test Notification's Multiline Title";
constexpr char kLongTitleString[] =
    "Test Notification's Very Very Very Very Very Very Very Very Very Very "
    "Very Very Very Very Very Very Very Very Very Very Very Very Very Very "
    "Very Very Very Very Long Multiline Title";

constexpr char kShortTitleScreenshot[] = "ash_notification_short_title";
constexpr char kMediumTitleScreenshot[] =
    "ash_notification_multiline_medium_title";
constexpr char kLongTitleScreenshot[] = "ash_notification_multiline_long_title";

// The types of the primary display size.
enum class DisplayType {
  // The display size is the default one.
  kNormal,

  // The display's height is much greater than its width.
  kUltraHeight,

  // The display's width is much greater than its height.
  kUltraWidth,
};

// Returns the name of `type`.
std::string GetDisplayTypeName(DisplayType type) {
  std::string display_type_name;
  switch (type) {
    case DisplayType::kNormal:
      return "normal";
    case DisplayType::kUltraWidth:
      return "ultra_width";
    case DisplayType::kUltraHeight:
      return "ultra_height";
  }
}

}  // namespace

// Pixel tests for Chrome OS Notification views.
class AshNotificationViewPixelTestBase : public AshTestBase {
 public:
  // AshTestBase:
  absl::optional<pixel_test::InitParams> CreatePixelTestInitParams()
      const override {
    return pixel_test::InitParams();
  }

  // AshTestBase:
  void SetUp() override {
    AshTestBase::SetUp();

    // The `NotificationCenterTray` does not exist until the `QsRevamp` feature
    // is enabled.
    test_api_ = std::make_unique<NotificationCenterTestApi>(
        /*notification_center_tray=*/nullptr);
  }

  NotificationCenterTestApi* test_api() { return test_api_.get(); }

 private:
  std::unique_ptr<NotificationCenterTestApi> test_api_;
};

class AshNotificationViewPixelTest
    : public AshNotificationViewPixelTestBase,
      public testing::WithParamInterface<
          std::tuple<bool /*IsQsRevampEnabled()*/, bool /*IsJellyEnabled()*/>> {
 public:
  // AshTestBase:
  void SetUp() override {
    scoped_feature_list_ = std::make_unique<base::test::ScopedFeatureList>();
    scoped_feature_list_->InitWithFeatureStates(
        {{features::kQsRevamp, /*enabled=*/IsQsRevampEnabled()},
         {chromeos::features::kJelly, /*enabled=*/IsJellyEnabled()}});
    AshNotificationViewPixelTestBase::SetUp();
  }

  bool IsQsRevampEnabled() { return std::get<0>(GetParam()); }

  bool IsJellyEnabled() { return std::get<1>(GetParam()); }

 private:
  std::unique_ptr<base::test::ScopedFeatureList> scoped_feature_list_;
};

INSTANTIATE_TEST_SUITE_P(All,
                         AshNotificationViewPixelTest,
                         testing::Combine(
                             /*QsRevamp*/ testing::Bool(),
                             /*Jelly*/ testing::Bool()));

// Tests that a notification's close button is visible when it is focused.
TEST_P(AshNotificationViewPixelTest, CloseButtonFocused) {
  // Create a notification and open the notification center bubble to view it.
  const auto id = test_api()->AddNotification();
  test_api()->ToggleBubble();

  // Verify that the close button is neither focused nor visible. Note that the
  // close button, as a `views::ImageButton`, will actually be visible in the
  // sense of `views::View::GetVisible()`, but its parent's `ui::Layer` will
  // have an opacity of zero, making it visually invisible.
  auto* notification_view = static_cast<AshNotificationView*>(
      test_api()->GetNotificationViewForId(id));
  auto* control_buttons_layer =
      notification_view->GetControlButtonsView()->layer();
  auto* close_button =
      notification_view->GetControlButtonsView()->close_button();
  EXPECT_EQ(control_buttons_layer->opacity(), 0);
  EXPECT_FALSE(close_button->HasFocus());

  // Move focus to the close button.
  close_button->GetWidget()->widget_delegate()->SetCanActivate(true);
  close_button->RequestFocus();

  // Verify, with both an assertion and a pixel test, that the close button has
  // focus and is visible.
  EXPECT_TRUE(close_button->HasFocus());
  EXPECT_EQ(control_buttons_layer->opacity(), 1);
  EXPECT_TRUE(GetPixelDiffer()->CompareUiComponentsOnPrimaryScreen(
      "close_button_focused", /*revision_number=*/0, notification_view));
}

// Regression test for http://b/267195370. Tests that a notification with no
// message has its title vertically centered in the collapsed state.
TEST_P(AshNotificationViewPixelTest, CollapsedNoMessage) {
  // Create a notification with no message, and open the notification center
  // bubble to view it.
  const std::string id = test_api()->AddCustomNotification(
      u"Notification title", u"",
      ui::ImageModel::FromImageSkia(CreateSolidColorTestImage(
          gfx::Size(/*width=*/45, /*height=*/45), SK_ColorGREEN)));
  test_api()->ToggleBubble();

  // Make sure the notification is collapsed.
  auto* notification_view = static_cast<AshNotificationView*>(
      test_api()->GetNotificationViewForId(id));
  notification_view->SetExpanded(false);
  ASSERT_FALSE(notification_view->IsExpanded());

  // Verify with a pixel test that the notification's title is vertically
  // centered.
  EXPECT_TRUE(GetPixelDiffer()->CompareUiComponentsOnPrimaryScreen(
      "collapsed_no_message", /*revision_number=*/0, notification_view));
}

class AshNotificationViewTitlePixelTest
    : public AshNotificationViewPixelTestBase,
      public testing::WithParamInterface<
          std::tuple<bool /*IsQsRevampEnabled()*/,
                     bool /*IsJellyEnabled()*/,
                     std::pair<const char* /*notification title string*/,
                               const char* /*screenshot name*/>>> {
 public:
  // AshTestBase:
  void SetUp() override {
    scoped_feature_list_ = std::make_unique<base::test::ScopedFeatureList>();
    scoped_feature_list_->InitWithFeatureStates(
        {{features::kQsRevamp, /*enabled=*/IsQsRevampEnabled()},
         {chromeos::features::kJelly, /*enabled=*/IsJellyEnabled()}});
    AshNotificationViewPixelTestBase::SetUp();
  }

  bool IsQsRevampEnabled() { return std::get<0>(GetParam()); }

  bool IsJellyEnabled() { return std::get<1>(GetParam()); }

  const std::string GetTitle() { return std::get<2>(GetParam()).first; }

  const std::string GetScreenshotName() {
    return std::get<2>(GetParam()).second;
  }

 private:
  std::unique_ptr<base::test::ScopedFeatureList> scoped_feature_list_;
};

INSTANTIATE_TEST_SUITE_P(
    All,
    AshNotificationViewTitlePixelTest,
    testing::Combine(
        /*QsRevamp*/ testing::Bool(),
        /*Jelly*/ testing::Bool(),
        testing::ValuesIn({
            std::make_pair(kShortTitleString, kShortTitleScreenshot),
            std::make_pair(kMediumTitleString, kMediumTitleScreenshot),
            std::make_pair(kLongTitleString, kLongTitleScreenshot),
        })));

// Regression test for b/251686063. Tests that a notification with a medium
// length multiline title and an icon is correctly displayed. This string would
// not be displayed properly without the workaround implemented for b/251686063.
TEST_P(AshNotificationViewTitlePixelTest, NotificationTitleTest) {
  // Create a notification with a multiline title and an icon.
  const std::string title = GetTitle();

  const std::string id = test_api()->AddCustomNotification(
      base::UTF8ToUTF16(title), u"Notification Content",
      ui::ImageModel::FromImageSkia(CreateSolidColorTestImage(
          gfx::Size(/*width=*/45, /*height=*/45), SK_ColorGREEN)));

  test_api()->ToggleBubble();

  // Make sure the notification view exists and is visible.
  message_center::MessageView* notification_view =
      test_api()->GetNotificationViewForId(id);
  ASSERT_TRUE(notification_view);
  EXPECT_TRUE(notification_view->GetVisible());

  // Compare pixels.
  const std::string screenshot_name = GetScreenshotName();
  EXPECT_TRUE(GetPixelDiffer()->CompareUiComponentsOnPrimaryScreen(
      screenshot_name, /*revision_number=*/4, notification_view));
}

class ScreenCaptureNotificationPixelTest
    : public AshNotificationViewPixelTestBase,
      public testing::WithParamInterface<
          std::tuple<bool /*IsQsRevampEnabled()*/, DisplayType>> {
 public:
  // AshNotificationViewPixelTestBase:
  void SetUp() override {
    scoped_feature_list_.InitWithFeatureStates(
        {{features::kQsRevamp, IsQsRevampEnabled()},
         {chromeos::features::kJelly, IsQsRevampEnabled()}});

    AshNotificationViewPixelTestBase::SetUp();

    // Change the display size depending on the test param.
    switch (GetDisplayType()) {
      case DisplayType::kNormal:
        break;
      case DisplayType::kUltraWidth:
        UpdateDisplay("1200x600");
        break;
      case DisplayType::kUltraHeight:
        UpdateDisplay("600x1200");
        break;
    }

    // Create windows so that the screenshot has more contents.
    window1_ = CreateAppWindow(/*bounds_in_screen=*/gfx::Rect(200, 200));
    window2_ =
        CreateAppWindow(/*bounds_in_screen=*/gfx::Rect(220, 220, 100, 100));
    DecorateWindow(window1_.get(), u"Window1", SK_ColorDKGRAY);
    DecorateWindow(window2_.get(), u"Window2", SK_ColorBLUE);
  }

  void TearDown() override {
    window2_.reset();
    window1_.reset();
    AshNotificationViewPixelTestBase::TearDown();
  }

  bool IsQsRevampEnabled() const { return std::get<0>(GetParam()); }

  const DisplayType& GetDisplayType() const { return std::get<1>(GetParam()); }

 private:
  base::test::ScopedFeatureList scoped_feature_list_;
  std::unique_ptr<aura::Window> window1_;
  std::unique_ptr<aura::Window> window2_;
};

INSTANTIATE_TEST_SUITE_P(
    DisplaySize,
    ScreenCaptureNotificationPixelTest,
    testing::Combine(/*IsQsRevampEnabled()=*/testing::Bool(),
                     testing::ValuesIn({DisplayType::kNormal,
                                        DisplayType::kUltraWidth,
                                        DisplayType::kUltraHeight})));

// Verifies the notification popup of a full screenshot.
TEST_P(ScreenCaptureNotificationPixelTest, VerifyPopup) {
  // Take a full screenshot then wait for the file path to the saved image.
  ash::CaptureModeController* controller = StartCaptureSession(
      CaptureModeSource::kFullscreen, CaptureModeType::kImage);
  controller->PerformCapture();
  const base::FilePath image_file_path = WaitForCaptureFileToBeSaved();

  // Wait until the notification popup shows.
  MessagePopupAnimationWaiter(
      GetPrimaryUnifiedSystemTray()->GetMessagePopupCollection())
      .Wait();

  // Get the notification view.
  EXPECT_TRUE(GetPixelDiffer()->CompareUiComponentsOnPrimaryScreen(
      base::StrCat({"screen_capture_popup_notification_",
                    GetDisplayTypeName(GetDisplayType())}),
      /*revision_number=*/8,
      test_api()->GetPopupViewForId(kScreenCaptureNotificationId)));
}

}  // namespace ash