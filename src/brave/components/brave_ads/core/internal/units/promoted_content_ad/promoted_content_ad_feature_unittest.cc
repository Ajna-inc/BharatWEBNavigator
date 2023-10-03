/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/units/promoted_content_ad/promoted_content_ad_feature.h"

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

TEST(BraveAdsPromotedContentAdFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsPromotedContentAdFeatureEnabled());
}

TEST(BraveAdsPromotedContentAdFeatureTest, IsDisabled) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(kPromotedContentAdFeature);

  // Act

  // Assert
  EXPECT_FALSE(IsPromotedContentAdFeatureEnabled());
}

TEST(BraveAdsPromotedContentAdFeatureTest, MaximumAdsPerHour) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kPromotedContentAdFeature, {{"maximum_ads_per_hour", "42"}});

  // Act

  // Assert
  EXPECT_EQ(42, kMaximumPromotedContentAdsPerHour.Get());
}

TEST(BraveAdsPromotedContentAdFeatureTest, DefaultMaximumAdsPerHour) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(4, kMaximumPromotedContentAdsPerHour.Get());
}

TEST(BraveAdsPromotedContentAdFeatureTest,
     DefaultMaximumAdsPerHourWhenDisabled) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(kPromotedContentAdFeature);

  // Act

  // Assert
  EXPECT_EQ(4, kMaximumPromotedContentAdsPerHour.Get());
}

TEST(BraveAdsPromotedContentAdFeatureTest, MaximumAdsPerDay) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kPromotedContentAdFeature, {{"maximum_ads_per_day", "24"}});

  // Act

  // Assert
  EXPECT_EQ(24, kMaximumPromotedContentAdsPerDay.Get());
}

TEST(BraveAdsPromotedContentAdFeatureTest, DefaultMaximumAdsPerDay) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(20, kMaximumPromotedContentAdsPerDay.Get());
}

TEST(BraveAdsPromotedContentAdFeatureTest,
     DefaultMaximumAdsPerDayWhenDisabled) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(kPromotedContentAdFeature);

  // Act

  // Assert
  EXPECT_EQ(20, kMaximumPromotedContentAdsPerDay.Get());
}

}  // namespace brave_ads
