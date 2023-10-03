/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/serving/prediction/model_based/sampling/creative_ad_model_based_predictor_sampling_util.h"

#include "brave/components/brave_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"
#include "brave/components/brave_ads/core/internal/serving/prediction/model_based/creative_ad_model_based_predictor_info.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

TEST(BraveAdsCreativeAdPredictorSamplingUtilTest,
     CalculateNormalizingConstantForCreativeAdPredictors) {
  // Arrange
  CreativeAdPredictorList<CreativeNotificationAdInfo> creative_ad_predictors;

  CreativeAdPredictorInfo<CreativeNotificationAdInfo> creative_ad_predictor_1;
  creative_ad_predictor_1.score = 0.5;
  creative_ad_predictors.push_back(creative_ad_predictor_1);

  CreativeAdPredictorInfo<CreativeNotificationAdInfo> creative_ad_predictor_2;
  creative_ad_predictor_2.score = 1.0;
  creative_ad_predictors.push_back(creative_ad_predictor_2);

  // Act
  const double normalizing_constant =
      CalculateNormalizingConstantForCreativeAdPredictors(
          creative_ad_predictors);

  // Assert
  EXPECT_DOUBLE_EQ(1.5, normalizing_constant);
}

}  // namespace brave_ads
