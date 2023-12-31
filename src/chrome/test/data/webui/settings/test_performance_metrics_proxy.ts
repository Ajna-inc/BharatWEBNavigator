// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import {BatterySaverModeState, HighEfficiencyModeExceptionListAction, HighEfficiencyModeState, PerformanceMetricsProxy} from 'chrome://settings/settings.js';
import {TestBrowserProxy} from 'chrome://webui-test/test_browser_proxy.js';

export class TestPerformanceMetricsProxy extends TestBrowserProxy implements
    PerformanceMetricsProxy {
  constructor() {
    super([
      'recordBatterySaverModeChanged',
      'recordHighEfficiencyModeChanged',
      'recordExceptionListAction',
    ]);
  }

  recordBatterySaverModeChanged(state: BatterySaverModeState) {
    this.methodCalled('recordBatterySaverModeChanged', state);
  }

  recordHighEfficiencyModeChanged(state: HighEfficiencyModeState) {
    this.methodCalled('recordHighEfficiencyModeChanged', state);
  }

  recordExceptionListAction(action: HighEfficiencyModeExceptionListAction) {
    this.methodCalled('recordExceptionListAction', action);
  }
}