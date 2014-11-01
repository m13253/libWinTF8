/*
  Copyright (c) 2014 StarBrilliant <m13253@hotmail.com>
  All rights reserved.

  Redistribution and use in source and binary forms are permitted
  provided that the above copyright notice and this paragraph are
  duplicated in all such forms and that any documentation,
  advertising materials, and other materials related to such
  distribution and use acknowledge that the software was developed by
  StarBrilliant.
  The name of StarBrilliant may not be used to endorse or promote
  products derived from this software without specific prior written
  permission.

  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
#pragma once
#ifndef WTF8_UTFCONV_H_INCLUDED_
#define WTF8_UTFCONV_H_INCLUDED_

#ifdef __cplusplus
#include <cstddef>
#include <stdexcept>
#include <string>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
namespace WinTF8 {

class UnicodeError : public std::runtime_error {
    using std::runtime_error::runtime_error;
public:
    UnicodeError() : runtime_error("Can not convert string to Unicode") {}
};

/* Microsoft VC++ Runtime behaves differently on different versions,
   we will just reimplement them ourselves. */
std::wstring UTF8ToWide(const std::string& utf8str, bool strict = false);
std::string WideToUTF8(const std::wstring& widestr, bool strict = false);

};
#endif

#ifdef __cplusplus
extern "C" {
#endif

size_t WTF8_UTF8ToWide(wchar_t *widestr, const char *utf8str, int strict, size_t bufsize);
size_t WTF8_WideToUTF8(char *utf8str, const wchar_t *widestr, int strict, size_t bufsize);

#ifdef __cplusplus
}
#endif

#endif
