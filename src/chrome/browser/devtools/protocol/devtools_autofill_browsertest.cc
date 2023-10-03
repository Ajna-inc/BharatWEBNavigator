// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/check_deref.h"
#include "base/strings/strcat.h"
#include "base/test/scoped_feature_list.h"
#include "build/chromeos_buildflags.h"
#include "chrome/browser/autofill/autofill_uitest_util.h"
#include "chrome/browser/devtools/protocol/devtools_protocol_test_support.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/autofill/content/browser/content_autofill_driver.h"
#include "components/autofill/content/browser/test_autofill_manager_injector.h"
#include "components/autofill/core/browser/autofill_test_utils.h"
#include "components/autofill/core/browser/browser_autofill_manager.h"
#include "components/autofill/core/browser/browser_autofill_manager_test_api.h"
#include "components/autofill/core/browser/field_types.h"
#include "components/autofill/core/browser/test_autofill_manager_waiter.h"
#include "components/autofill/core/common/autofill_features.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "net/dns/mock_host_resolver.h"
#include "testing/gmock/include/gmock/gmock.h"

using testing::Eq;
using testing::Not;
using testing::ResultOf;

namespace autofill {

namespace {

// Asserts that a filled field sent to devtools has `attribute` set with
// `expected_value`.
auto FilledFieldHasAttributeWithValue(const std::string& attribute,
                                      const std::string& expected_value) {
  return ResultOf(
      [&](const base::Value& filled_field) {
        const std::string* value =
            filled_field.GetDict().FindStringByDottedPath(attribute);
        return value ? *value : "";
      },
      Eq(expected_value));
}

auto FilledFieldHasAttributeWithValue16(const std::string& attribute,
                                        const std::u16string& expected_value) {
  return FilledFieldHasAttributeWithValue(attribute,
                                          base::UTF16ToASCII(expected_value));
}

std::string GetProfileInfoFromAddressField(const AutofillProfile profile,
                                           const base::Value& address_field) {
  return base::UTF16ToASCII(profile.GetRawInfo(TypeNameToFieldType(
      *address_field.GetDict().FindStringByDottedPath("name"))));
}

}  // namespace

// Adds waiting capabilities to BrowserAutofillManager.
class TestAutofillManager : public autofill::BrowserAutofillManager {
 public:
  TestAutofillManager(autofill::ContentAutofillDriver* driver,
                      autofill::AutofillClient* client)
      : BrowserAutofillManager(driver, client, "en-US") {}

  static TestAutofillManager& GetForRenderFrameHost(
      content::RenderFrameHost* rfh) {
    return static_cast<TestAutofillManager&>(
        autofill::ContentAutofillDriver::GetForRenderFrameHost(rfh)
            ->GetAutofillManager());
  }

  [[nodiscard]] testing::AssertionResult WaitForFormsSeen(
      size_t num_awaited_calls) {
    return forms_seen_.Wait(num_awaited_calls);
  }

 private:
  autofill::TestAutofillManagerWaiter forms_seen_{
      *this,
      {autofill::AutofillManagerEvent::kFormsSeen}};
};

class DevToolsAutofillTest : public DevToolsProtocolTestBase {
 public:
  void SetUpOnMainThread() override {
    host_resolver()->AddRule("*", "127.0.0.1");
  }

  content::RenderFrameHost* main_frame() {
    return web_contents()->GetPrimaryMainFrame();
  }

  TestAutofillManager& main_autofill_manager() {
    return TestAutofillManager::GetForRenderFrameHost(main_frame());
  }

  std::string EvaluateAndGetValue(const std::string& expression,
                                  const std::string& unique_context_id,
                                  const std::string& session_id) {
    base::Value::Dict params;
    params.Set("expression", expression);
    if (!unique_context_id.empty()) {
      params.Set("uniqueContextId", unique_context_id);
    }
    const base::Value::Dict* result = SendSessionCommand(
        "Runtime.evaluate", std::move(params), session_id, /*wait=*/true);
    return *result->FindStringByDottedPath("result.value");
  }

  int GetBackendNodeIdByIdAttribute(const std::string& id_attribute) {
    return GetBackendNodeIdByIdAttribute(id_attribute, "", "");
  }

  int GetBackendNodeIdByIdAttribute(const std::string& id_attribute,
                                    const std::string& unique_context_id) {
    return GetBackendNodeIdByIdAttribute(id_attribute, unique_context_id, "");
  }

  int GetBackendNodeIdByIdAttribute(const std::string& id_attribute,
                                    const std::string& unique_context_id,
                                    const std::string& session_id) {
    std::string object_id;
    {
      base::Value::Dict params;
      params.Set("expression", base::StrCat({"document.getElementById('",
                                             id_attribute, "')"}));
      if (!unique_context_id.empty()) {
        params.Set("uniqueContextId", unique_context_id);
      }
      const base::Value::Dict* result = SendSessionCommand(
          "Runtime.evaluate", std::move(params), session_id, /*wait=*/true);
      object_id = *result->FindStringByDottedPath("result.objectId");
    }

    base::Value::Dict params;
    params.Set("objectId", object_id);
    const base::Value::Dict* result = SendSessionCommand(
        "DOM.describeNode", std::move(params), session_id, /*wait=*/true);
    return *result->FindIntByDottedPath("node.backendNodeId");
  }

  base::Value::Dict GetTestCreditCard() {
    base::Value::Dict card;
    card.Set("number", "4444444444444444");
    card.Set("name", "John Smith");
    card.Set("expiryMonth", "01");
    card.Set("expiryYear", "2030");
    card.Set("cvc", "123");
    return card;
  }

  AutofillProfile CreateTestProfile() {
    AutofillProfile profile = test::GetFullProfile();
    AddTestProfile(browser()->profile(), profile);
    return profile;
  }

  FormGlobalId form_id() const { return form_id_; }

  base::Value::Dict GetFilledOutForm(const std::string& unique_context_id) {
    return GetFilledOutForm(unique_context_id, "");
  }

  base::Value::Dict GetFilledOutForm(const std::string& unique_context_id,
                                     const std::string& session_id) {
    base::Value::Dict card;
    card.Set("number",
             EvaluateAndGetValue(
                 "document.getElementById('CREDIT_CARD_NUMBER').value",
                 unique_context_id, session_id));
    card.Set("name",
             EvaluateAndGetValue(
                 "document.getElementById('CREDIT_CARD_NAME_FULL').value",
                 unique_context_id, session_id));
    card.Set("expiryMonth",
             EvaluateAndGetValue(
                 "document.getElementById('CREDIT_CARD_EXP_MONTH').value",
                 unique_context_id, session_id));
    card.Set(
        "expiryYear",
        EvaluateAndGetValue(
            "document.getElementById('CREDIT_CARD_EXP_4_DIGIT_YEAR').value",
            unique_context_id, session_id));
    // CVC is not filled out in the form.
    card.Set("cvc", "123");
    return card;
  }

 private:
  test::AutofillUnitTestEnvironment autofill_test_environment_;
  base::test::ScopedFeatureList feature_list_{
      features::kAutofillTestFormWithDevtools};
  FormGlobalId form_id_ = test::MakeFormGlobalId();
  autofill::TestAutofillManagerInjector<TestAutofillManager>
      autofill_manager_injector_;
};

IN_PROC_BROWSER_TEST_F(DevToolsAutofillTest, SetAddresses) {
  embedded_test_server()->ServeFilesFromSourceDirectory(
      "chrome/test/data/autofill");
  ASSERT_TRUE(embedded_test_server()->Start());
  const GURL url(
      embedded_test_server()->GetURL("/autofill_creditcard_form.html"));
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  ASSERT_TRUE(content::WaitForLoadStop(web_contents()));
  Attach();

  EXPECT_TRUE(main_autofill_manager().WaitForFormsSeen(1));

  base::Value::Dict address_1_fields;
  address_1_fields.Set("name", "ADDRESS_HOME_LINE1");
  address_1_fields.Set("value", "Erika-mann");
  base::Value::Dict address_1;
  base::Value::List fields;
  fields.Append(std::move(address_1_fields));
  address_1.Set("fields", std::move(fields));

  base::Value::Dict address_2_fields;
  address_2_fields.Set("name", "ADDRESS_HOME_LINE2");
  address_2_fields.Set("value", "Faria lima");
  base::Value::Dict address_2;
  base::Value::List fields_2;
  fields_2.Append(std::move(address_2_fields));
  address_2.Set("fields", std::move(fields_2));

  base::Value::List test_addresses;
  test_addresses.Append(std::move(address_1));
  test_addresses.Append(std::move(address_2));

  base::Value::Dict params;
  params.Set("addresses", std::move(test_addresses));

  SendCommandSync("Autofill.setAddresses", std::move(params));

  std::vector<autofill::AutofillProfile> res =
      test_api(main_autofill_manager()).test_addresses();
  ASSERT_EQ(res.size(), 2u);
  ASSERT_EQ(res[0].GetAddress().GetRawInfo(
                autofill::ServerFieldType::ADDRESS_HOME_LINE1),
            u"Erika-mann");
  ASSERT_EQ(res[1].GetAddress().GetRawInfo(
                autofill::ServerFieldType::ADDRESS_HOME_LINE2),
            u"Faria lima");
}

IN_PROC_BROWSER_TEST_F(DevToolsAutofillTest, TriggerCreditCard) {
  embedded_test_server()->ServeFilesFromSourceDirectory(
      "chrome/test/data/autofill");
  ASSERT_TRUE(embedded_test_server()->Start());
  const GURL url(
      embedded_test_server()->GetURL("/autofill_creditcard_form.html"));
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  ASSERT_TRUE(content::WaitForLoadStop(web_contents()));
  Attach();

  EXPECT_TRUE(main_autofill_manager().WaitForFormsSeen(1));

  int backend_node_id = GetBackendNodeIdByIdAttribute("CREDIT_CARD_NUMBER");

  base::Value::Dict params;
  params.Set("fieldId", backend_node_id);
  params.Set("card", GetTestCreditCard());

  SendCommandSync("Autofill.trigger", std::move(params));
  EXPECT_EQ(*result(), base::Value::Dict());
  EXPECT_EQ(GetFilledOutForm(""), GetTestCreditCard());
}

IN_PROC_BROWSER_TEST_F(DevToolsAutofillTest, TriggerCreditCardInIframe) {
  embedded_test_server()->ServeFilesFromSourceDirectory(
      "chrome/test/data/autofill");
  ASSERT_TRUE(embedded_test_server()->Start());
  const GURL url(embedded_test_server()->GetURL(
      "/autofill_creditcard_form_in_iframe.html"));
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  ASSERT_TRUE(content::WaitForLoadStop(web_contents()));
  Attach();

  EXPECT_TRUE(main_autofill_manager().WaitForFormsSeen(1));

  std::string frame_id;
  {
    const base::Value::Dict* result = SendCommandSync("Page.getFrameTree");
    const base::Value::List* frames =
        result->FindListByDottedPath("frameTree.childFrames");
    const base::Value::Dict* frame_dict = frames->front().GetIfDict();
    frame_id = *frame_dict->FindStringByDottedPath("frame.id");
  }

  std::string unique_context_id;
  {
    base::Value::Dict command_params;
    SendCommandSync("Runtime.enable");
    base::Value::Dict params;
    for (int context_count = 1; true; context_count++) {
      params = WaitForNotification("Runtime.executionContextCreated", true);
      if (*params.FindStringByDottedPath("context.auxData.frameId") ==
          frame_id) {
        unique_context_id = *params.FindStringByDottedPath("context.uniqueId");
        break;
      }
      ASSERT_LT(context_count, 2);
    }
  }

  int backend_node_id =
      GetBackendNodeIdByIdAttribute("CREDIT_CARD_NUMBER", unique_context_id);

  {
    base::Value::Dict params;
    params.Set("fieldId", backend_node_id);
    params.Set("card", GetTestCreditCard());
    params.Set("frameId", "wrong");
    SendCommandSync("Autofill.trigger", std::move(params));
    EXPECT_EQ(*error()->FindString("message"), "Frame not found");
  }

  {
    base::Value::Dict params;
    params.Set("fieldId", backend_node_id);
    params.Set("card", GetTestCreditCard());
    params.Set("frameId", frame_id);
    SendCommandSync("Autofill.trigger", std::move(params));
    EXPECT_EQ(*result(), base::Value::Dict());
  }

  EXPECT_EQ(GetFilledOutForm(unique_context_id), GetTestCreditCard());
}

// Disabled due to nullptr deref; see https://crbug.com/1477600.
IN_PROC_BROWSER_TEST_F(DevToolsAutofillTest,
                       DISABLED_TriggerCreditCardInOOPIFIframe) {
  embedded_test_server()->ServeFilesFromSourceDirectory(
      "chrome/test/data/autofill");
  ASSERT_TRUE(embedded_test_server()->Start());
  GURL url = embedded_test_server()->GetURL(
      "a.com", "/autofill_creditcard_form_in_oopif.html");

  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  ASSERT_TRUE(content::WaitForLoadStop(web_contents()));
  Attach();

  EXPECT_TRUE(main_autofill_manager().WaitForFormsSeen(1));

  const base::Value::Dict* result = SendCommandSync("Target.getTargets");

  base::Value::Dict iframe_target;
  for (const auto& target : *result->FindList("targetInfos")) {
    if (*target.GetDict().FindString("type") == "iframe") {
      iframe_target = target.Clone().TakeDict();
      break;
    }
  }
  std::string target_id = CHECK_DEREF(iframe_target.FindString("targetId"));

  {
    base::Value::Dict params;
    params.Set("targetId", target_id);
    params.Set("flatten", true);
    result = SendCommandSync("Target.attachToTarget", std::move(params));
  }
  std::string session_id = CHECK_DEREF(result->FindString("sessionId"));

  int backend_node_id =
      GetBackendNodeIdByIdAttribute("CREDIT_CARD_NUMBER", "", session_id);

  base::Value::Dict params;
  params.Set("fieldId", backend_node_id);
  params.Set("frameId", target_id);
  params.Set("card", GetTestCreditCard());
  result = SendSessionCommand("Autofill.trigger", std::move(params), session_id,
                              /*wait=*/true);

  EXPECT_EQ(*result, base::Value::Dict());
  EXPECT_EQ(GetFilledOutForm("", session_id), GetTestCreditCard());
}

IN_PROC_BROWSER_TEST_F(DevToolsAutofillTest, AddressFormFilled) {
  Attach();
  // Create a profile to read information from and send to devtools.
  AutofillProfile profile = CreateTestProfile();
  // Create fake filled fields.
  // First field. Please note that we only use `form_field_data` to grab the
  // field value, everything else comes from `autofill_field`.
  // TODO(crbug.com/1331312): Get rid of FormFieldData.
  FormFieldData form_field_data =
      test::CreateTestFormField(/*label*/ "", "name_1", "value_1", "text");
  form_field_data.id_attribute = u"id_1";
  AutofillField autofill_field(form_field_data);
  // set `autofill_field_2` to empty to assert that we always use
  // `form_field_data_2`.
  autofill_field.value = u"";
  autofill_field.set_server_predictions(
      {test::CreateFieldPrediction(NAME_FULL)});
  autofill_field.SetHtmlType(autofill::mojom::HtmlFieldType::kName,
                             autofill::mojom::HtmlFieldMode::kShipping);
  // Second field.
  FormFieldData form_field_data_2 =
      test::CreateTestFormField(/*label*/ "", "name_2", "value_2", "text");
  form_field_data_2.id_attribute = u"id_2";
  AutofillField autofill_field_2(form_field_data_2);
  // set `autofill_field_2` to empty to assert that we always use
  // `form_field_data_2`.
  autofill_field_2.value = u"";
  autofill_field_2.set_server_predictions(
      {test::CreateFieldPrediction(NAME_FULL)});
  autofill_field_2.SetHtmlType(autofill::mojom::HtmlFieldType::kUnspecified,
                               autofill::mojom::HtmlFieldMode::kShipping);
  std::vector<const std::pair<const FormFieldData*, const AutofillField*>>
      filled_fields_by_autofill = {{{&form_field_data, &autofill_field},
                                    {&form_field_data_2, &autofill_field_2}}};

  // Enabled events and emit event about forming being filled.
  SendCommandSync("Autofill.enable");
  main_autofill_manager().NotifyObservers(
      &autofill::AutofillManager::Observer::OnFillOrPreviewDataModelForm,
      form_id(), autofill::mojom::AutofillActionPersistence::kFill,
      filled_fields_by_autofill, &profile);

  base::Value::Dict notification = WaitForNotification(
      "Autofill.addressFormFilled", /*allow_existing=*/true);
  for (const base::Value& address_line :
       *notification.FindListByDottedPath("addressUi.addressFields")) {
    for (const base::Value& address_field :
         *address_line.GetDict().FindListByDottedPath("fields")) {
      // Test that the profile address values sent to devtools match what we
      // have in `profile`.
      EXPECT_EQ(GetProfileInfoFromAddressField(profile, address_field),
                *address_field.GetDict().FindStringByDottedPath("value"));
    }
  }

  // Assert that the filled fields sent to devtools match exactly the ones
  // filled by autofill.
  const base::Value::List* filled_fields =
      notification.FindListByDottedPath("filledFields");
  ASSERT_EQ(filled_fields->size(), filled_fields_by_autofill.size());
  for (size_t i = 0; i < filled_fields->size(); ++i) {
    const base::Value& ff = (*filled_fields)[i];
    const FormFieldData* ffd = filled_fields_by_autofill[i].first;
    const AutofillField* af = filled_fields_by_autofill[i].second;

    EXPECT_THAT(ff, FilledFieldHasAttributeWithValue16("id", af->id_attribute));
    EXPECT_THAT(ff, FilledFieldHasAttributeWithValue(
                        "autofillType",
                        std::string(FieldTypeToDeveloperRepresentationString(
                            af->Type().GetStorableType()))));
    // Note: we read the value from `FormFieldData`.
    EXPECT_THAT(ff, FilledFieldHasAttributeWithValue16("value", ffd->value));
    EXPECT_THAT(ff,
                Not(FilledFieldHasAttributeWithValue16("value", af->value)));
    EXPECT_THAT(ff, FilledFieldHasAttributeWithValue("htmlType",
                                                     af->form_control_type));
    EXPECT_THAT(ff,
                FilledFieldHasAttributeWithValue16("name", af->name_attribute));
  }

  // The first filled field uses autocomplete attribute as filling strategy.
  EXPECT_EQ(*filled_fields->front().GetDict().FindStringByDottedPath(
                "fillingStrategy"),
            "autocompleteAttribute");
  // The second one used autofill internals, either local heuristics or server
  // predictions.
  EXPECT_EQ(*filled_fields->back().GetDict().FindStringByDottedPath(
                "fillingStrategy"),
            "autofillInferred");
}

}  // namespace autofill