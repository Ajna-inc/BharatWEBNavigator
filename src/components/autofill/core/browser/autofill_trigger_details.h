// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_TRIGGER_DETAILS_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_TRIGGER_DETAILS_H_

namespace autofill {

// Specifies the source that triggered autofilling a form. Differently from
// `AutofillSuggestionTriggerSource` this enum does not mean the action that
// triggered autofill (for example
// AutofillSuggestionTriggerSource::kFormControlElementClicked) but only the
// internal surface that provided the autofill experience (like `kPopup`) or
// internal mechanisms/business logic (like kSelectOptionsChanged). For example
// AutofillSuggestionTriggerSource::kFormControlElementClicked "action" can lead
// autofill to be "surfaced" from `kPopup` or `kFastCheckout`.
enum class AutofillTriggerSource {
  kNone = 0,
  // Autofill was triggered from accepting a suggestion in the Autofill popup.
  kPopup = 1,
  // Autofill was triggered from accepting a suggestion in the keyboard
  // accessory.
  kKeyboardAccessory = 2,
  // Autofill was triggered from accepting a suggestion in the touch to fill for
  // credit cards bottom sheet.
  kTouchToFillCreditCard = 3,
  // Refill was triggered from the forms seen event. This includes cases where a
  // refill was triggered right after a non-refill Autofill invocation - in this
  // case the original trigger source got lost.
  kFormsSeen = 4,
  // Refill was triggered from blink when the selected option of a <select>
  // control is changed.
  kSelectOptionsChanged = 5,
  // Refill was triggered from blink when the input element is in the autofilled
  // state and the value has been changed by JavaScript.
  kJavaScriptChangedAutofilledValue = 6,
  // Autofill was applied after unlocking a server card with the CVC. The
  // original trigger source got lost. This should not happen.
  kCreditCardCvcPopup = 7,
  // Autofill was triggered from a Fast Checkout run.
  kFastCheckout = 8,
  // Filling/preview was triggered through a suggestion generated by manual
  // fallbacks for autocomplete=unrecognized fields. Contrary to regular
  // Autofill, Autofill with this trigger source fills autocomplete=unrecognized
  // fields.
  kManualFallbackForAutocompleteUnrecognized = 9
};

// Specifies the level of filling i.e granularity. This varies depending on the
// suggestion chosen by the user. Today every use case except for address forms
// and single field fillers (like autocomplete) will be explicitly set to
// `kFillForm`, for example credit cards. Address forms can have other
// granularities in the case where the user chooses a more granular suggestion,
// like "Fill Full name" (kGroupFilling) or "Jon doe" to fill a specific field
// (`kFieldByFieldFilling`). Single field fillers such as autocomplete, Ibans
// and merchant promo codes do not use the concept of `FillingGranularity` and
// they always work on a field by field case. The `FillingGranularity` is
// ultimately used by the manager, who will use it to define which fields to
// fill and how to generate suggestions.
enum class FillingGranularity {
  kNone = 0,
  // Default autofill behaviour. Fills the form with every available information
  // in the profile.
  kFillForm = 1,
  // Triggered from fields belongings to types that are either of type name,
  // address or phone number. It will fill only field whose group match the
  // field triggering group.
  kGroupFilling = 2,
  // Triggered when the user chooses to fill a single field with the provided
  // suggestion.
  kFieldByFieldFilling = 3
};

// Holds the details about a filling operation. The `trigger_source` field
// defines what triggered the autofill experience, i.e kFastCheckout or kPopup.
// the `filling_granularity` is about which fields from a form are going to be
// filled, today it can be either the whole form (classic case), a specific
// field group (such address fields) or specific field like the given name.
struct AutofillTriggerDetails {
  AutofillTriggerSource trigger_source = AutofillTriggerSource::kNone;
  FillingGranularity filling_granularity = FillingGranularity::kNone;
};

}  // namespace autofill

#endif  // COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_TRIGGER_DETAILS_H_
