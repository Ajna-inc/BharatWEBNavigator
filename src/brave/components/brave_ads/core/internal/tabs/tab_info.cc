/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/tabs/tab_info.h"

#include <tuple>

namespace brave_ads {

TabInfo::TabInfo() = default;

TabInfo::TabInfo(const TabInfo& other) = default;

TabInfo& TabInfo::operator=(const TabInfo& other) = default;

TabInfo::TabInfo(TabInfo&& other) noexcept = default;

TabInfo& TabInfo::operator=(TabInfo&& other) noexcept = default;

TabInfo::~TabInfo() = default;

bool TabInfo::operator==(const TabInfo& other) const {
  const auto tie = [](const TabInfo& tab) {
    return std::tie(tab.id, tab.redirect_chain, tab.is_playing_media);
  };

  return tie(*this) == tie(other);
}

bool TabInfo::operator!=(const TabInfo& other) const {
  return !(*this == other);
}

}  // namespace brave_ads
