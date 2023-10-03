/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/serving/eligible_ads/exclusion_rules/exclusion_rule_feature.h"

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

TEST(BraveAdsExclusionRuleFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsExclusionRuleFeatureEnabled());
}

TEST(BraveAdsExclusionRuleFeatureTest, IsDisabled) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(kExclusionRulesFeature);

  // Act

  // Assert
  EXPECT_FALSE(IsExclusionRuleFeatureEnabled());
}

TEST(BraveAdsExclusionRuleFeatureTest, ShouldExcludeAdIfConverted) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kExclusionRulesFeature, {{"should_exclude_ad_if_converted", "false"}});

  // Act

  // Assert
  EXPECT_FALSE(kShouldExcludeAdIfConverted.Get());
}

TEST(BraveAdsExclusionRuleFeatureTest, DefaultShouldExcludeAdIfConverted) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(kShouldExcludeAdIfConverted.Get());
}

TEST(BraveAdsExclusionRuleFeatureTest,
     DefaultShouldExcludeAdIfConvertedWhenDisabled) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(kExclusionRulesFeature);

  // Act

  // Assert
  EXPECT_TRUE(kShouldExcludeAdIfConverted.Get());
}

TEST(BraveAdsExclusionRuleFeatureTest,
     GetExcludeAdIfDismissedWithinTimeWindow) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kExclusionRulesFeature,
      {{"should_exclude_ad_if_dismissed_within_time_window", "1d"}});

  // Act

  // Assert
  EXPECT_EQ(base::Days(1), kShouldExcludeAdIfDismissedWithinTimeWindow.Get());
}

TEST(BraveAdsExclusionRuleFeatureTest,
     DefaultExcludeAdIfDismissedWithinTimeWindow) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::Hours(0), kShouldExcludeAdIfDismissedWithinTimeWindow.Get());
}

TEST(BraveAdsExclusionRuleFeatureTest,
     DefaultExcludeAdIfDismissedWithinTimeWindowWhenDisabled) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(kExclusionRulesFeature);

  // Act

  // Assert
  EXPECT_EQ(base::Hours(0), kShouldExcludeAdIfDismissedWithinTimeWindow.Get());
}

TEST(BraveAdsExclusionRuleFeatureTest,
     GetExcludeAdIfTransferredWithinTimeWindow) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kExclusionRulesFeature,
      {{"should_exclude_ad_if_transferred_within_time_window", "1d"}});

  // Act

  // Assert
  EXPECT_EQ(base::Days(1), kShouldExcludeAdIfTransferredWithinTimeWindow.Get());
}

TEST(BraveAdsExclusionRuleFeatureTest,
     DefaultExcludeAdIfTransferredWithinTimeWindow) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::Hours(0),
            kShouldExcludeAdIfTransferredWithinTimeWindow.Get());
}

TEST(BraveAdsExclusionRuleFeatureTest,
     DefaultExcludeAdIfTransferredWithinTimeWindowWhenDisabled) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(kExclusionRulesFeature);

  // Act

  // Assert
  EXPECT_EQ(base::Hours(0),
            kShouldExcludeAdIfTransferredWithinTimeWindow.Get());
}

}  // namespace brave_ads
