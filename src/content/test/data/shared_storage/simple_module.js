// Copyright 2021 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

console.log('Start executing simple_module.js')

class TestOperation {
  async run(data) {
    console.log('Start executing \'test-operation\'');
    if (data === undefined || data === null) {
      console.log(data);
    } else if (data.constructor.name === 'CryptoKey') {
      console.log(
        "CryptoKey,",
        "algorithm:",
        JSON.stringify(data.algorithm, Object.keys(data.algorithm).sort()),
        "usages:",
        JSON.stringify(data.usages, Object.keys(data.usages).sort()),
        "extractable:", data.extractable);
    } else {
      console.log(JSON.stringify(data, Object.keys(data).sort()));
    }
    console.log('Finish executing \'test-operation\'');
  }
}

class TestURLSelectionOperation {
  async run(urls, data) {
    console.log('Start executing \'test-url-selection-operation\'');
    console.log(JSON.stringify(urls));
    console.log(JSON.stringify(data, Object.keys(data).sort()));
    console.log('Finish executing \'test-url-selection-operation\'');

    if (data && data.hasOwnProperty('mockResult')) {
      return data['mockResult'];
    }

    return -1;
  }
}

class RemainingBudgetOperation {
  async run(data) {
    console.log('remaining budget: ' +
                (await sharedStorage.remainingBudget()).toString());
  }
}

register("test-operation", TestOperation);
register("test-url-selection-operation", TestURLSelectionOperation);
register("remaining-budget-operation", RemainingBudgetOperation);

console.log('Finish executing simple_module.js')
