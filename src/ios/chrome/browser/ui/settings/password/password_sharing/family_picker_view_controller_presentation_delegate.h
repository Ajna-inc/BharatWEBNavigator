// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_CHROME_BROWSER_UI_SETTINGS_PASSWORD_PASSWORD_SHARING_FAMILY_PICKER_VIEW_CONTROLLER_PRESENTATION_DELEGATE_H_
#define IOS_CHROME_BROWSER_UI_SETTINGS_PASSWORD_PASSWORD_SHARING_FAMILY_PICKER_VIEW_CONTROLLER_PRESENTATION_DELEGATE_H_

@class FamilyPickerViewController;

// Delegate for FamilyPickerViewController.
@protocol FamilyPickerViewControllerPresentationDelegate <NSObject>

// Called when the user clicks cancel button or dismisses the view by swiping.
- (void)familyPickerWasDismissed:(FamilyPickerViewController*)controller;

@end

#endif  // IOS_CHROME_BROWSER_UI_SETTINGS_PASSWORD_PASSWORD_SHARING_FAMILY_PICKER_VIEW_CONTROLLER_PRESENTATION_DELEGATE_H_
