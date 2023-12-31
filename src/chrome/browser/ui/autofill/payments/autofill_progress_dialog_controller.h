// Copyright 2021 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_PAYMENTS_AUTOFILL_PROGRESS_DIALOG_CONTROLLER_H_
#define CHROME_BROWSER_UI_AUTOFILL_PAYMENTS_AUTOFILL_PROGRESS_DIALOG_CONTROLLER_H_

#include <string>

namespace content {
class WebContents;
}

namespace autofill {

// Interface that exposes controller functionality to
// AutofillProgressDialogView. The interface exposes the title, description and
// the button label to the view to help show a progress dialog with a single
// button that acts as a cancel button. For example: We show a progress dialog
// when contacting the bank during unmasking a virtual card.
class AutofillProgressDialogController {
 public:
  // Callback received when the progress dialog is dismissed.
  // |is_canceled_by_user| is a boolean that is true if the user cancels the
  // progress dialog, false if the progress dialog closes automatically after a
  // confirmation message.
  virtual void OnDismissed(bool is_canceled_by_user) = 0;

  // Title and button label.
  // GetLoadingTitle() is used when card unmasking is in progress, while
  // GetConfirmationTitle() is used for the confirmation dialog that occurs upon
  // a successful card unmask.
  virtual std::u16string GetLoadingTitle() const = 0;
  virtual std::u16string GetConfirmationTitle() const = 0;
  virtual std::u16string GetCancelButtonLabel() const = 0;

  // Text displayed below the progress bar.
  virtual std::u16string GetLoadingMessage() const = 0;
  virtual std::u16string GetConfirmationMessage() const = 0;

  virtual content::WebContents* GetWebContents() = 0;

 protected:
  virtual ~AutofillProgressDialogController() = default;
};

}  // namespace autofill

#endif  // CHROME_BROWSER_UI_AUTOFILL_PAYMENTS_AUTOFILL_PROGRESS_DIALOG_CONTROLLER_H_
