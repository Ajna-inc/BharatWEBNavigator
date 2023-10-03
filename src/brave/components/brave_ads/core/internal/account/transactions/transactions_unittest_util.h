/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_UNITTEST_UTIL_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_UNITTEST_UTIL_H_

#include "brave/components/brave_ads/core/internal/account/transactions/transaction_info.h"

namespace base {
class Time;
}  // namespace base

namespace brave_ads {

class ConfirmationType;

size_t GetTransactionCountForTesting();

void SaveTransactionsForTesting(const TransactionList& transactions);

TransactionInfo BuildTransactionForTesting(
    double value,
    const ConfirmationType& confirmation_type,
    base::Time reconciled_at,
    bool should_use_random_uuids);
TransactionInfo BuildUnreconciledTransactionForTesting(
    double value,
    const ConfirmationType& confirmation_type,
    bool should_use_random_uuids);

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_UNITTEST_UTIL_H_
