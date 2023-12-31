// Copyright 2020 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/autofill/ios/form_util/unique_id_data_tab_helper.h"

UniqueIDDataTabHelper::~UniqueIDDataTabHelper() = default;

uint32_t UniqueIDDataTabHelper::GetNextAvailableRendererID() const {
  return next_available_renderer_id_;
}

void UniqueIDDataTabHelper::SetNextAvailableRendererID(uint32_t new_id) {
  next_available_renderer_id_ = new_id;
}

const scoped_refptr<autofill::FieldDataManager>
UniqueIDDataTabHelper::GetFieldDataManager() {
  return field_data_manager_;
}

UniqueIDDataTabHelper::UniqueIDDataTabHelper(web::WebState* web_state) {
  field_data_manager_ = base::MakeRefCounted<autofill::FieldDataManager>();
  web_state_observation_.Observe(web_state);
}

void UniqueIDDataTabHelper::WebStateDestroyed(web::WebState* web_state) {
  web_state_observation_.Reset();
}

WEB_STATE_USER_DATA_KEY_IMPL(UniqueIDDataTabHelper)
