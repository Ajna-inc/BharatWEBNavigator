// Copyright 2013 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_STRINGPRINTF_H_
#define BASE_STRINGS_STRINGPRINTF_H_

#include <stdarg.h>   // va_list

#include <string>

#include "base/base_export.h"
#include "base/compiler_specific.h"
#include "build/build_config.h"

namespace base {

// Return a C++ string given printf-like input.
[[nodiscard]] BASE_EXPORT std::string StringPrintf(const char* format, ...)
    PRINTF_FORMAT(1, 2);
#if BUILDFLAG(IS_WIN)
// Note: Unfortunately compile time checking of the format string for UTF-16
// strings is not supported by any compiler, thus these functions should be used
// carefully and sparingly. Also applies to StringAppendV below.
[[nodiscard]] BASE_EXPORT std::wstring StringPrintf(const wchar_t* format, ...)
    WPRINTF_FORMAT(1, 2);
#endif

// Return a C++ string given vprintf-like input.
[[nodiscard]] BASE_EXPORT std::string StringPrintV(const char* format,
                                                   va_list ap)
    PRINTF_FORMAT(1, 0);

// Append result to a supplied string.
BASE_EXPORT void StringAppendF(std::string* dst, const char* format, ...)
    PRINTF_FORMAT(2, 3);
#if BUILDFLAG(IS_WIN)
BASE_EXPORT void StringAppendF(std::wstring* dst, const wchar_t* format, ...)
    WPRINTF_FORMAT(2, 3);
#endif

// Lower-level routine that takes a va_list and appends to a specified
// string.  All other routines are just convenience wrappers around it.
BASE_EXPORT void StringAppendV(std::string* dst, const char* format, va_list ap)
    PRINTF_FORMAT(2, 0);
#if BUILDFLAG(IS_WIN)
BASE_EXPORT void StringAppendV(std::wstring* dst,
                               const wchar_t* format,
                               va_list ap) WPRINTF_FORMAT(2, 0);
#endif

}  // namespace base

#endif  // BASE_STRINGS_STRINGPRINTF_H_
