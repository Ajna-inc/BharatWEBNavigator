// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/quic/network_connection.h"

#include "base/memory/raw_ptr.h"
#include "base/run_loop.h"
#include "net/base/mock_network_change_notifier.h"
#include "net/test/test_with_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace net::test {

constexpr auto CONNECTION_3G = NetworkChangeNotifier::CONNECTION_3G;
constexpr auto CONNECTION_2G = NetworkChangeNotifier::CONNECTION_2G;
constexpr auto CONNECTION_ETHERNET = NetworkChangeNotifier::CONNECTION_ETHERNET;
constexpr auto CONNECTION_WIFI = NetworkChangeNotifier::CONNECTION_WIFI;

// TestWithTaskEnvironment needed to instantiate a
// net::NetworkChangeNotifier::NetworkChangeNotifier via
// ScopedMockNetworkChangeNotifier.
class NetworkConnectionTest : public TestWithTaskEnvironment {
 protected:
  NetworkConnectionTest()
      : notifier_(scoped_notifier_.mock_network_change_notifier()) {}

  ScopedMockNetworkChangeNotifier scoped_notifier_;
  raw_ptr<MockNetworkChangeNotifier> notifier_;
};

TEST_F(NetworkConnectionTest, Connection2G) {
  notifier_->SetConnectionType(CONNECTION_2G);

  NetworkConnection network_connection;
  EXPECT_EQ(CONNECTION_2G, network_connection.connection_type());
}

TEST_F(NetworkConnectionTest, Connection3G) {
  notifier_->SetConnectionType(CONNECTION_3G);

  NetworkConnection network_connection;
  EXPECT_EQ(CONNECTION_3G, network_connection.connection_type());
}

TEST_F(NetworkConnectionTest, ConnectionEthnernet) {
  notifier_->SetConnectionType(CONNECTION_ETHERNET);

  NetworkConnection network_connection;
  EXPECT_EQ(CONNECTION_ETHERNET, network_connection.connection_type());
}

TEST_F(NetworkConnectionTest, ConnectionWifi) {
  notifier_->SetConnectionType(CONNECTION_WIFI);

  NetworkConnection network_connection;
  EXPECT_EQ(CONNECTION_WIFI, network_connection.connection_type());
}

TEST_F(NetworkConnectionTest, ConnectionChange) {
  notifier_->SetConnectionType(CONNECTION_2G);

  NetworkConnection network_connection;

  notifier_->SetConnectionType(CONNECTION_3G);
  NetworkChangeNotifier::NotifyObserversOfIPAddressChangeForTests();
  // Spin the message loop so the notification is delivered.
  base::RunLoop().RunUntilIdle();
  EXPECT_EQ(CONNECTION_3G, network_connection.connection_type());

  NetworkChangeNotifier::NotifyObserversOfConnectionTypeChangeForTests(
      CONNECTION_ETHERNET);
  // Spin the message loop so the notification is delivered.
  base::RunLoop().RunUntilIdle();
  EXPECT_EQ(CONNECTION_ETHERNET, network_connection.connection_type());

  NetworkChangeNotifier::NotifyObserversOfConnectionTypeChangeForTests(
      CONNECTION_WIFI);
  // Spin the message loop so the notification is delivered.
  base::RunLoop().RunUntilIdle();
  EXPECT_EQ(CONNECTION_WIFI, network_connection.connection_type());
}

}  // namespace net::test
