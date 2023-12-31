/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_BROWSER_COMPONENT_UPDATER_COMPONENT_UTIL_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_BROWSER_COMPONENT_UPDATER_COMPONENT_UTIL_H_

#include <string>

#include "brave/components/brave_ads/browser/component_updater/component_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace brave_ads {

absl::optional<ComponentInfo> GetComponentInfo(const std::string& id);

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_BROWSER_COMPONENT_UPDATER_COMPONENT_UTIL_H_
