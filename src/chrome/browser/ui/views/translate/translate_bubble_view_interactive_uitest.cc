// Copyright 2021 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/views/translate/translate_bubble_view.h"

#include <string>

#include "base/command_line.h"
#include "base/functional/bind.h"
#include "base/functional/callback_helpers.h"
#include "base/i18n/base_i18n_switches.h"
#include "base/strings/string_piece_forward.h"
#include "base/strings/utf_string_conversions.h"
#include "base/test/bind.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/translate/chrome_translate_client.h"
#include "chrome/browser/translate/translate_test_utils.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/views/translate/translate_bubble_controller.h"
#include "chrome/common/chrome_switches.h"
#include "chrome/grit/generated_resources.h"
#include "chrome/test/base/ui_test_utils.h"
#include "chrome/test/interaction/interactive_browser_test.h"
#include "components/translate/core/browser/translate_download_manager.h"
#include "components/translate/core/browser/translate_manager.h"
#include "components/translate/core/common/translate_switches.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/common/content_switches.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/network_connection_change_simulator.h"
#include "content/public/test/url_loader_interceptor.h"
#include "net/dns/mock_host_resolver.h"
#include "net/test/embedded_test_server/controllable_http_response.h"
#include "ui/accessibility/ax_action_data.h"
#include "ui/accessibility/ax_enums.mojom.h"
#include "ui/base/interaction/element_tracker.h"
#include "ui/base/interaction/expect_call_in_scope.h"
#include "ui/base/interaction/interaction_sequence.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/ui_base_features.h"
#include "ui/events/base_event_utils.h"
#include "ui/events/keycodes/dom/dom_code.h"
#include "ui/views/controls/button/menu_button.h"
#include "ui/views/controls/combobox/combobox.h"
#include "ui/views/controls/tabbed_pane/tabbed_pane.h"
#include "ui/views/interaction/element_tracker_views.h"
#include "ui/views/interaction/interaction_sequence_views.h"
#include "ui/views/test/button_test_api.h"

namespace translate {

namespace {

static const char kTestValidScript[] =
    "var google = {};"
    "google.translate = (function() {"
    "  return {"
    "    TranslateService: function() {"
    "      return {"
    "        isAvailable : function() {"
    "          return true;"
    "        },"
    "        restore : function() {"
    "          return;"
    "        },"
    "        getDetectedLanguage : function() {"
    "          return \"\";"
    "        },"
    "        translatePage : function(sourceLang, targetLang,"
    "                                 onTranslateProgress) {"
    "          onTranslateProgress(100, true, false);"
    "        }"
    "      };"
    "    }"
    "  };"
    "})();"
    "cr.googleTranslate.onTranslateElementLoad();";

}  // namespace

class TranslateBubbleViewUITest
    : public InteractiveBrowserTest,
      public ::testing::WithParamInterface<std::string> {
 public:
  TranslateBubbleViewUITest() = default;
  ~TranslateBubbleViewUITest() override = default;

  void SetUp() override {
    set_open_about_blank_on_browser_launch(true);
    TranslateManager::SetIgnoreMissingKeyForTesting(true);
    ASSERT_TRUE(embedded_test_server()->InitializeAndListen());
    InteractiveBrowserTest::SetUp();
  }

  void SetUpCommandLine(base::CommandLine* command_line) override {
    command_line->AppendSwitchASCII(
        switches::kTranslateScriptURL,
        embedded_test_server()->GetURL("/mock_translate_script.js").spec());
    if (GetParam() == "RightToLeft") {
      command_line->AppendSwitchASCII(::switches::kForceUIDirection,
                                      ::switches::kForceDirectionRTL);
      command_line->AppendSwitchASCII(::switches::kForceTextDirection,
                                      ::switches::kForceDirectionRTL);
    } else if (GetParam() == "Incognito") {
      command_line->AppendSwitch(::switches::kIncognito);
    } else if (GetParam() == "Theme") {
      command_line->AppendSwitchASCII(::switches::kInstallAutogeneratedTheme,
                                      "121,0,0");
    }
    command_line->AppendSwitch(::switches::kOverrideLanguageDetection);
  }

  void SetUpOnMainThread() override {
    host_resolver()->AddRule("www.google.com", "127.0.0.1");
    embedded_test_server()->RegisterRequestHandler(base::BindRepeating(
        &TranslateBubbleViewUITest::HandleRequest, base::Unretained(this)));
    embedded_test_server()->StartAcceptingConnections();
    InteractiveBrowserTest::SetUpOnMainThread();
  }

  void TearDownOnMainThread() override {
    EXPECT_TRUE(embedded_test_server()->ShutdownAndWaitUntilComplete());
    InteractiveBrowserTest::TearDownOnMainThread();
  }

 protected:
  // Waits for the page to move to translated state `translated` and then checks
  // that the tabs are selected correctly.
  auto WaitForTranslated(bool translated) {
    MultiStep steps;
    steps.emplace_back(
        Do(base::BindOnce(&TranslateBubbleViewUITest::WaitForTranslatedImpl,
                          base::Unretained(this), translated)));
    steps.emplace_back(
        CheckViewProperty(TranslateBubbleView::kSourceLanguageTab,
                          &views::TabbedPaneTab::selected, !translated));
    steps.emplace_back(
        CheckViewProperty(TranslateBubbleView::kTargetLanguageTab,
                          &views::TabbedPaneTab::selected, translated));
    return steps;
  }

  std::unique_ptr<net::test_server::HttpResponse> HandleRequest(
      const net::test_server::HttpRequest& request) {
    if (request.GetURL().path() != "/mock_translate_script.js")
      return nullptr;

    std::unique_ptr<net::test_server::BasicHttpResponse> http_response(
        new net::test_server::BasicHttpResponse);
    http_response->set_code(net::HTTP_OK);
    http_response->set_content(kTestValidScript);
    http_response->set_content_type("text/javascript");
    return std::move(http_response);
  }

  void NavigateAndWaitForLanguageDetection(const GURL& url,
                                           const std::string& expected_lang) {
    ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
    if (GetParam() == "MultipleBubble") {
      chrome::GenerateQRCodeFromPageAction(browser());
    }

    while (expected_lang !=
           ChromeTranslateClient::FromWebContents(
               browser()->tab_strip_model()->GetActiveWebContents())
               ->GetLanguageState()
               .source_language()) {
      CreateTranslateWaiter(
          browser()->tab_strip_model()->GetActiveWebContents(),
          TranslateWaiter::WaitEvent::kLanguageDetermined)
          ->Wait();
    }
  }

  TranslateBubbleView* GetCurrentTranslateBubble() {
    return TranslateBubbleController::FromWebContents(
               browser()->tab_strip_model()->GetActiveWebContents())
        ->GetTranslateBubble();
  }

 private:
  void WaitForTranslatedImpl(bool translated = true) {
    if (ChromeTranslateClient::FromWebContents(
            browser()->tab_strip_model()->GetActiveWebContents())
            ->GetLanguageState()
            .IsPageTranslated() != translated) {
      CreateTranslateWaiter(
          browser()->tab_strip_model()->GetActiveWebContents(),
          translated ? TranslateWaiter::WaitEvent::kPageTranslated
                     : TranslateWaiter::WaitEvent::kIsPageTranslatedChanged)
          ->Wait();
    }
  }
};

// Verify that source language tab is selected and highlighted by
// default, and by selecting target language the page gets translated into
// target language and reverted to source language.
IN_PROC_BROWSER_TEST_P(TranslateBubbleViewUITest, ClickLanguageTab) {
  // P1.Opened/Navigate to non english page > Hit on Translate bubble icon.
  GURL french_url = GURL(embedded_test_server()->GetURL("/french_page.html"));
  NavigateAndWaitForLanguageDetection(french_url, "fr");

  auto* const translate_bubble = GetCurrentTranslateBubble();

  // If translate bubble changes to another context, the tests will have to be
  // modified to fix context handling, so best to put a sanity check here to
  // eliminate unexplained errors later.
  ASSERT_EQ(browser()->window()->GetElementContext(),
            views::ElementTrackerViews::GetContextForView(translate_bubble));

  RunTestSequence(
      views::InteractionSequenceViews::WithInitialView(translate_bubble),
      // V1.Verify that by default the Translate bubble’s source language
      // tab is selected and highlighted.
      WaitForTranslated(false),
      DoDefaultAction(TranslateBubbleView::kTargetLanguageTab),
      // V2.Verify that once the page is translated, the target language tab
      // will be selected.
      WaitForTranslated(true),
      // P3.To translate the page to source language again, tapping the
      // source language.
      DoDefaultAction(TranslateBubbleView::kSourceLanguageTab),
      // V3.Verify that page reverts the translation should shows in
      // original content.
      WaitForTranslated(false),
      // P4.Tap on cancel button option in the Translate bubble popup box.
      PressButton(TranslateBubbleView::kCloseButton),
      AfterHide(TranslateBubbleView::kIdentifier, base::DoNothing()));
}

// Verify the "Choose another language" option from 3 dot menu.
IN_PROC_BROWSER_TEST_P(TranslateBubbleViewUITest, ChooseAnotherLanguage) {
  // P1. Opened/Navigate to non english page.
  GURL french_url = GURL(embedded_test_server()->GetURL("/french_page.html"));
  NavigateAndWaitForLanguageDetection(french_url, "fr");

  RunTestSequence(
      views::InteractionSequenceViews::WithInitialView(
          GetCurrentTranslateBubble()),
      PressButton(TranslateBubbleView::kOptionsMenuButton),
      // P3. Click on the “Choose another language” option.
      SelectMenuItem(TranslateBubbleView::kChangeTargetLanguage),
      // V1. Verify that this dismisses the options menu and brings up a new
      // bubble with a combobox that populates a list of all available
      // languages.
      AfterHide(TranslateBubbleView::kChangeTargetLanguage, base::DoNothing()),
      // P4. Select a language from the list and select translate.
      SelectDropdownItem(TranslateBubbleView::kTargetLanguageCombobox, 0),
      PressButton(TranslateBubbleView::kTargetLanguageDoneButton),
      // V2. Verify that the language list will be dismissed, the target
      // language tab shows updated target language. Source language tab is
      // no longer highlighted and the target language tab will be
      // highlighted once translation is completed.
      AfterHide(TranslateBubbleView::kTargetLanguageCombobox,
                base::DoNothing()),
      WaitForTranslated(true),
      CheckViewProperty(
          TranslateBubbleView::kTargetLanguageTab,
          &views::TabbedPaneTab::GetTitleText,
          GetCurrentTranslateBubble()->model()->GetTargetLanguageNameAt(0)),
      // P5. Select revert.
      DoDefaultAction(TranslateBubbleView::kSourceLanguageTab),
      // V3. Verify that the page should revert to original language and source
      // language tab is selected.
      WaitForTranslated(false));
}

// Verify the "Page is not in (source language)" option from 3 dot menu.
IN_PROC_BROWSER_TEST_P(TranslateBubbleViewUITest,
                       ClickPageNotInSourceLanguage) {
  // P1. Opened/Navigate to non english page.
  GURL french_url = GURL(embedded_test_server()->GetURL("/french_page.html"));
  NavigateAndWaitForLanguageDetection(french_url, "fr");

  RunTestSequence(
      views::InteractionSequenceViews::WithInitialView(
          GetCurrentTranslateBubble()),
      // P2. Click on Translate bubble > Click on 3 dot menu.
      PressButton(TranslateBubbleView::kOptionsMenuButton),
      // P3. Click on the “Page is not in {source languages}?” option.
      SelectMenuItem(TranslateBubbleView::kChangeSourceLanguage),
      // V1. Verify that this dismisses the options menu and brings up a new
      // bubble with a combobox that populates a list of all available
      // languages.
      AfterHide(TranslateBubbleView::kChangeSourceLanguage, base::DoNothing()),
      // P4. Select a language from the list and select translate.
      // Item 0 is the detected language.
      SelectDropdownItem(TranslateBubbleView::kSourceLanguageCombobox, 1),
      PressButton(TranslateBubbleView::kSourceLanguageDoneButton),
      // V2. The language list will be dismissed, the source language tab
      // shows updated source language. Source language tab is no longer
      // highlighted and the target language tab will be highlighted once
      // the translation is completed.
      AfterHide(TranslateBubbleView::kSourceLanguageCombobox,
                base::DoNothing()),
      WaitForTranslated(true),
      CheckViewProperty(
          TranslateBubbleView::kSourceLanguageTab,
          &views::TabbedPaneTab::GetTitleText,
          GetCurrentTranslateBubble()->model()->GetSourceLanguageNameAt(1)),
      // P5. Select revert.
      // Note: The revert means revert the page to its original language,
      // but the source tab are still showing the source language we
      // selected in P4. See https://crbug.com/1222050.
      DoDefaultAction(TranslateBubbleView::kSourceLanguageTab),
      // V3. Verify that the page should revert to original language and source
      // language tab is selected.
      WaitForTranslated(false));
}

// Verify the error handling OR network interruption.
IN_PROC_BROWSER_TEST_P(TranslateBubbleViewUITest, NetworkInterruption) {
  bool offline = false;
  content::URLLoaderInterceptor interceptor(base::BindLambdaForTesting(
      [&](content::URLLoaderInterceptor::RequestParams* params) -> bool {
        if (!offline)
          return false;
        params->client->OnComplete(
            network::URLLoaderCompletionStatus(net::ERR_INTERNET_DISCONNECTED));
        return true;
      }));

  // Changing the URLLoaderFactory used by translate fetcher to the one for
  // BrowserProcess. The original one is owned by SystemNetworkContextManager
  // which cannot intercepted by content::URLLoaderInterceptor.
  TranslateDownloadManager::GetInstance()->set_url_loader_factory(
      browser()
          ->profile()
          ->GetDefaultStoragePartition()
          ->GetURLLoaderFactoryForBrowserProcess());

  // P1. Opened/Navigate to non english page > Hit on Translate bubble icon.
  GURL french_url = GURL(embedded_test_server()->GetURL("/french_page.html"));
  NavigateAndWaitForLanguageDetection(french_url, "fr");

  RunTestSequence(
      views::InteractionSequenceViews::WithInitialView(
          GetCurrentTranslateBubble()),
      // P2. Tap the target language tab.
      DoDefaultAction(TranslateBubbleView::kTargetLanguageTab),
      // V1. Wait until the translation is completed.
      WaitForTranslated(true),
      // P3. Turn off the network and clear the script cache.
      Do(base::BindLambdaForTesting([&]() {
        offline = true;
        TranslateDownloadManager::GetInstance()
            ->ClearTranslateScriptForTesting();
      })),
      // P4. Click on the source language tab.
      DoDefaultAction(TranslateBubbleView::kSourceLanguageTab),
      // V3. The page should revert to the original language.
      WaitForTranslated(false),
      // P4. Click on the target language tab again.
      DoDefaultAction(TranslateBubbleView::kTargetLanguageTab),
      // V4. Translate bubble is dismissed, An error bubble will be shown
      // with a message saying "This page could not be translated.".
      CheckViewProperty(TranslateBubbleView::kErrorMessage,
                        &views::Label::GetText,
                        l10n_util::GetStringUTF16(
                            IDS_TRANSLATE_BUBBLE_COULD_NOT_TRANSLATE_TITLE)),
      // V5. Wait for the bubble to be dismissed.
      AfterHide(TranslateBubbleView::kChangeTargetLanguage, base::DoNothing()));
}

INSTANTIATE_TEST_SUITE_P(All,
                         TranslateBubbleViewUITest,
                         ::testing::Values("Default",
                                           "RightToLeft",
                                           "Incognito",
                                           "MultipleBubble",
                                           "Theme"),
                         [](const ::testing::TestParamInfo<std::string>& inf) {
                           return inf.param;
                         });

}  // namespace translate