// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/* Minimal externs file provided for places in the code that
 * still use JavaScript instead of TypeScript.
 * @externs
 */

/**
 * @constructor
 * @extends {HTMLElement}
 */
function CrFingerprintProgressArcElement() {}

/**
 * @param {number} prevPercentComplete
 * @param {number} currPercentComplete
 * @param {boolean} isComplete
 */
CrFingerprintProgressArcElement.prototype.setProgress = function(
    prevPercentComplete, currPercentComplete, isComplete) {};

/**
 * @param {boolean} shouldPlay
 */
CrFingerprintProgressArcElement.prototype.setPlay = function(shouldPlay) {};
