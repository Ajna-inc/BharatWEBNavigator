/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_UNITS_PROMOTED_CONTENT_AD_PROMOTED_CONTENT_AD_FEATURE_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_UNITS_PROMOTED_CONTENT_AD_PROMOTED_CONTENT_AD_FEATURE_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace brave_ads {

BASE_DECLARE_FEATURE(kPromotedContentAdFeature);

bool IsPromotedContentAdFeatureEnabled();

constexpr base::FeatureParam<int> kMaximumPromotedContentAdsPerHour{
    &kPromotedContentAdFeature, "maximum_ads_per_hour", 4};

constexpr base::FeatureParam<int> kMaximumPromotedContentAdsPerDay{
    &kPromotedContentAdFeature, "maximum_ads_per_day", 20};

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_UNITS_PROMOTED_CONTENT_AD_PROMOTED_CONTENT_AD_FEATURE_H_