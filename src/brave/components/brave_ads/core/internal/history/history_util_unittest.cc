/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/history/history_util.h"

#include "base/ranges/algorithm.h"
#include "brave/components/brave_ads/core/internal/common/unittest/unittest_base.h"
#include "brave/components/brave_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "brave/components/brave_ads/core/internal/creatives/notification_ads/notification_ad_builder.h"
#include "brave/components/brave_ads/core/internal/history/history_constants.h"
#include "brave/components/brave_ads/core/internal/history/history_manager.h"
#include "brave/components/brave_ads/core/public/account/confirmations/confirmation_type.h"
#include "brave/components/brave_ads/core/public/history/history_item_info.h"
#include "brave/components/brave_ads/core/public/units/notification_ad/notification_ad_info.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

namespace {

HistoryItemInfo BuildAndAddHistoryItem() {
  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);

  return AddHistory(ad, ConfirmationType::kViewed, ad.title, ad.body);
}

}  // namespace

class BraveAdsHistoryUtilTest : public UnitTestBase {};

TEST_F(BraveAdsHistoryUtilTest, AddHistory) {
  // Arrange

  // Act
  const HistoryItemInfo history_item = BuildAndAddHistoryItem();

  // Assert
  const HistoryItemList expected_history = {history_item};
  EXPECT_TRUE(base::ranges::equal(expected_history, HistoryManager::Get()));
}

TEST_F(BraveAdsHistoryUtilTest, PurgeHistoryOlderThanTimeWindow) {
  // Arrange
  BuildAndAddHistoryItem();

  AdvanceClockBy(kHistoryTimeWindow + base::Milliseconds(1));

  // Act
  const HistoryItemInfo history_item = BuildAndAddHistoryItem();

  // Assert
  const HistoryItemList expected_history = {history_item};
  EXPECT_TRUE(base::ranges::equal(expected_history, HistoryManager::Get()));
}

TEST_F(BraveAdsHistoryUtilTest, DoNotPurgeHistoryWithinTimeWindow) {
  // Arrange
  const HistoryItemInfo history_item_1 = BuildAndAddHistoryItem();

  AdvanceClockBy(kHistoryTimeWindow);

  // Act
  const HistoryItemInfo history_item_2 = BuildAndAddHistoryItem();

  // Assert
  const HistoryItemList expected_history = {history_item_2, history_item_1};
  EXPECT_TRUE(base::ranges::equal(expected_history, HistoryManager::Get()));
}

}  // namespace brave_ads
