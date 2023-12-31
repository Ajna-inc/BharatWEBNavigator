// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * Explains why a download is in DANGEROUS state.
 */
export enum DangerType {
  BLOCKED_PASSWORD_PROTECTED = 'BLOCKED_PASSWORD_PROTECTED',
  BLOCKED_TOO_LARGE = 'BLOCKED_TOO_LARGE',
  DANGEROUS_ACCOUNT_COMPROMISE = 'DANGEROUS_ACCOUNT_COMPROMISE',
  DANGEROUS_CONTENT = 'DANGEROUS_CONTENT',
  DANGEROUS_FILE = 'DANGEROUS_FILE',
  DANGEROUS_HOST = 'DANGEROUS_HOST',
  DANGEROUS_URL = 'DANGEROUS_URL',
  DEEP_SCANNED_FAILED = 'DEEP_SCANNED_FAILED',
  DEEP_SCANNED_OPENED_DANGEROUS = 'DEEP_SCANNED_OPENED_DANGEROUS',
  DEEP_SCANNED_SAFE = 'DEEP_SCANNED_SAFE',
  NOT_DANGEROUS = 'NOT_DANGEROUS',
  POTENTIALLY_UNWANTED = 'POTENTIALLY_UNWANTED',
  SENSITIVE_CONTENT_BLOCK = 'SENSITIVE_CONTENT_BLOCK',
  SENSITIVE_CONTENT_WARNING = 'SENSITIVE_CONTENT_WARNING',
  UNCOMMON_CONTENT = 'UNCOMMON_CONTENT',
}

/**
 * The states a download can be in. These correspond to states defined in
 * DownloadsDOMHandler = =CreateDownloadItemValue
 */
export enum States {
  IN_PROGRESS = 'IN_PROGRESS',
  CANCELLED = 'CANCELLED',
  COMPLETE = 'COMPLETE',
  PAUSED = 'PAUSED',
  DANGEROUS = 'DANGEROUS',
  INTERRUPTED = 'INTERRUPTED',
  INSECURE = 'INSECURE',
  ASYNC_SCANNING = 'ASYNC_SCANNING',
  PROMPT_FOR_SCANNING = 'PROMPT_FOR_SCANNING',
}
