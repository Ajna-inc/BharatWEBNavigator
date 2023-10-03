// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/views/web_apps/web_app_integration_test_driver.h"
#include "content/public/test/browser_test.h"

namespace web_app::integration_tests {
namespace {

using WebAppIntegration = WebAppIntegrationTest;

// Manual tests:

// Generated tests:

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_29NotPromotableWindowed_12NotPromotable_7NotPromotable_1NotPromotable_24) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.CreateShortcut(Site::kNotPromotable, WindowOptions::kWindowed);
  helper_.CheckAppInListWindowed(Site::kNotPromotable);
  helper_.CheckPlatformShortcutAndIcon(Site::kNotPromotable);
  helper_.LaunchFromPlatformShortcut(Site::kNotPromotable);
  helper_.CheckWindowCreated();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_31Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_1Standalone_79StandaloneStandaloneOriginal_24_26) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallOmniboxIcon(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckWindowDisplayStandalone();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_31Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_149Standalone_11Standalone_1Standalone_22One_163Standalone) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallOmniboxIcon(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.SetOpenInTabFromAppSettings(Site::kStandalone);
  helper_.CheckAppInListTabbed(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckTabCreated(Number::kOne);
  helper_.CheckAppLoadedInTab(Site::kStandalone);
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_31Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_147Standalone_11Standalone_1Standalone_22One_163Standalone) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallOmniboxIcon(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.SetOpenInTabFromAppHome(Site::kStandalone);
  helper_.CheckAppInListTabbed(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckTabCreated(Number::kOne);
  helper_.CheckAppLoadedInTab(Site::kStandalone);
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_31Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_74Standalone_72Standalone_1Standalone_24) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallOmniboxIcon(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.DeletePlatformShortcut(Site::kStandalone);
  helper_.CreateShortcutsFromList(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_31Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_32StandaloneNoShortcutBrowserWebApp_7Standalone_12Standalone_1Standalone_24) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallOmniboxIcon(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.InstallPolicyApp(Site::kStandalone, ShortcutOptions::kNoShortcut,
                           WindowOptions::kBrowser, InstallMode::kWebApp);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_31Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_32StandaloneWithShortcutBrowserWebApp_12Standalone_1Standalone_24) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallOmniboxIcon(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.InstallPolicyApp(Site::kStandalone, ShortcutOptions::kWithShortcut,
                           WindowOptions::kBrowser, InstallMode::kWebApp);
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_31Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_116StandaloneBrowser_117Standalone_1Standalone_24_94_25) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallOmniboxIcon(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.ManifestUpdateDisplay(Site::kStandalone, Display::kBrowser);
  helper_.AwaitManifestUpdate(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
  helper_.CheckTabNotCreated();
  helper_.CheckWindowDisplayMinimal();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_31Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_116StandaloneMinimalUi_117Standalone_1Standalone_24_94_25) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallOmniboxIcon(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.ManifestUpdateDisplay(Site::kStandalone, Display::kMinimalUi);
  helper_.AwaitManifestUpdate(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
  helper_.CheckTabNotCreated();
  helper_.CheckWindowDisplayMinimal();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_47Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_1Standalone_79StandaloneStandaloneOriginal_24_26) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallMenuOption(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckWindowDisplayStandalone();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_47Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_149Standalone_11Standalone_1Standalone_22One_163Standalone) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallMenuOption(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.SetOpenInTabFromAppSettings(Site::kStandalone);
  helper_.CheckAppInListTabbed(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckTabCreated(Number::kOne);
  helper_.CheckAppLoadedInTab(Site::kStandalone);
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_47Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_147Standalone_11Standalone_1Standalone_22One_163Standalone) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallMenuOption(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.SetOpenInTabFromAppHome(Site::kStandalone);
  helper_.CheckAppInListTabbed(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckTabCreated(Number::kOne);
  helper_.CheckAppLoadedInTab(Site::kStandalone);
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_47Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_74Standalone_72Standalone_1Standalone_24) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallMenuOption(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.DeletePlatformShortcut(Site::kStandalone);
  helper_.CreateShortcutsFromList(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_47Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_32StandaloneNoShortcutBrowserWebApp_7Standalone_12Standalone_1Standalone_24) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallMenuOption(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.InstallPolicyApp(Site::kStandalone, ShortcutOptions::kNoShortcut,
                           WindowOptions::kBrowser, InstallMode::kWebApp);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_47Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_32StandaloneWithShortcutBrowserWebApp_12Standalone_1Standalone_24) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallMenuOption(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.InstallPolicyApp(Site::kStandalone, ShortcutOptions::kWithShortcut,
                           WindowOptions::kBrowser, InstallMode::kWebApp);
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_47Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_116StandaloneBrowser_117Standalone_1Standalone_24_94_25) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallMenuOption(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.ManifestUpdateDisplay(Site::kStandalone, Display::kBrowser);
  helper_.AwaitManifestUpdate(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
  helper_.CheckTabNotCreated();
  helper_.CheckWindowDisplayMinimal();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_47Standalone_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_116StandaloneMinimalUi_117Standalone_1Standalone_24_94_25) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallMenuOption(InstallableSite::kStandalone);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.ManifestUpdateDisplay(Site::kStandalone, Display::kMinimalUi);
  helper_.AwaitManifestUpdate(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
  helper_.CheckTabNotCreated();
  helper_.CheckWindowDisplayMinimal();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_32StandaloneWithShortcutWindowedWebApp_79StandaloneStandaloneOriginal_12Standalone_7Standalone_116StandaloneBrowser_117Standalone_1Standalone_24_94_25) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallPolicyApp(Site::kStandalone, ShortcutOptions::kWithShortcut,
                           WindowOptions::kWindowed, InstallMode::kWebApp);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.ManifestUpdateDisplay(Site::kStandalone, Display::kBrowser);
  helper_.AwaitManifestUpdate(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
  helper_.CheckTabNotCreated();
  helper_.CheckWindowDisplayMinimal();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_32StandaloneWithShortcutWindowedWebApp_79StandaloneStandaloneOriginal_12Standalone_7Standalone_116StandaloneMinimalUi_117Standalone_1Standalone_24_94_25) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.InstallPolicyApp(Site::kStandalone, ShortcutOptions::kWithShortcut,
                           WindowOptions::kWindowed, InstallMode::kWebApp);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.ManifestUpdateDisplay(Site::kStandalone, Display::kMinimalUi);
  helper_.AwaitManifestUpdate(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
  helper_.CheckTabNotCreated();
  helper_.CheckWindowDisplayMinimal();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_29StandaloneWindowed_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_1Standalone_24_26) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.CreateShortcut(Site::kStandalone, WindowOptions::kWindowed);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
  helper_.CheckWindowDisplayStandalone();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_29StandaloneWindowed_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_149Standalone_11Standalone_1Standalone_22One_163Standalone) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.CreateShortcut(Site::kStandalone, WindowOptions::kWindowed);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.SetOpenInTabFromAppSettings(Site::kStandalone);
  helper_.CheckAppInListTabbed(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckTabCreated(Number::kOne);
  helper_.CheckAppLoadedInTab(Site::kStandalone);
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_29StandaloneWindowed_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_147Standalone_11Standalone_1Standalone_22One_163Standalone) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.CreateShortcut(Site::kStandalone, WindowOptions::kWindowed);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.SetOpenInTabFromAppHome(Site::kStandalone);
  helper_.CheckAppInListTabbed(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckTabCreated(Number::kOne);
  helper_.CheckAppLoadedInTab(Site::kStandalone);
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_29StandaloneWindowed_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_74Standalone_72Standalone_1Standalone_24) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.CreateShortcut(Site::kStandalone, WindowOptions::kWindowed);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.DeletePlatformShortcut(Site::kStandalone);
  helper_.CreateShortcutsFromList(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_29StandaloneWindowed_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_32StandaloneNoShortcutBrowserWebApp_7Standalone_12Standalone_1Standalone_24) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.CreateShortcut(Site::kStandalone, WindowOptions::kWindowed);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.InstallPolicyApp(Site::kStandalone, ShortcutOptions::kNoShortcut,
                           WindowOptions::kBrowser, InstallMode::kWebApp);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_29StandaloneWindowed_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_32StandaloneWithShortcutBrowserWebApp_12Standalone_1Standalone_24) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.CreateShortcut(Site::kStandalone, WindowOptions::kWindowed);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.InstallPolicyApp(Site::kStandalone, ShortcutOptions::kWithShortcut,
                           WindowOptions::kBrowser, InstallMode::kWebApp);
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_29StandaloneWindowed_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_116StandaloneBrowser_117Standalone_1Standalone_24_94_25) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.CreateShortcut(Site::kStandalone, WindowOptions::kWindowed);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.ManifestUpdateDisplay(Site::kStandalone, Display::kBrowser);
  helper_.AwaitManifestUpdate(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
  helper_.CheckTabNotCreated();
  helper_.CheckWindowDisplayMinimal();
}

IN_PROC_BROWSER_TEST_F(
    WebAppIntegration,
    WAI_29StandaloneWindowed_79StandaloneStandaloneOriginal_24_12Standalone_7Standalone_112StandaloneNotShown_116StandaloneMinimalUi_117Standalone_1Standalone_24_94_25) {
  // Test contents are generated by script. Please do not modify!
  // See `docs/webapps/why-is-this-test-failing.md` or
  // `docs/webapps/integration-testing-framework` for more info.
  // Sheriffs: Disabling this test is supported.
  helper_.CreateShortcut(Site::kStandalone, WindowOptions::kWindowed);
  helper_.CheckAppTitle(Site::kStandalone, Title::kStandaloneOriginal);
  helper_.CheckWindowCreated();
  helper_.CheckAppInListWindowed(Site::kStandalone);
  helper_.CheckPlatformShortcutAndIcon(Site::kStandalone);
  helper_.CheckWindowControlsOverlayToggle(Site::kStandalone,
                                           IsShown::kNotShown);
  helper_.ManifestUpdateDisplay(Site::kStandalone, Display::kMinimalUi);
  helper_.AwaitManifestUpdate(Site::kStandalone);
  helper_.LaunchFromPlatformShortcut(Site::kStandalone);
  helper_.CheckWindowCreated();
  helper_.CheckTabNotCreated();
  helper_.CheckWindowDisplayMinimal();
}

}  // namespace
}  // namespace web_app::integration_tests