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
#ifndef WTF8_U8STR_H_INCLUDED_
#define WTF8_U8STR_H_INCLUDED_

#ifdef __cplusplus
#include <cstddef>
#include <functional>
#include <string>
#include <utility>

namespace WTF8 {

class u8string : public std::string {
public:
    using std::string::string;
    explicit u8string() : std::string() {
    }
    explicit u8string(const std::string& s) :
        std::string(s) {
    }
    explicit u8string(std::string&& s) :
        std::string(std::move(s)) {
    }
    u8string(const std::wstring& s, bool strict = false);
    u8string(const wchar_t* s, bool strict = false);
    static u8string from_wide(const std::wstring& s, bool strict = false) {
        return u8string(s, strict);
    }
    static u8string from_wide(const wchar_t* s, bool strict = false) {
        return u8string(s, strict);
    }
    std::wstring to_wide(bool strict = false) const;
    operator std::wstring() const {
        return to_wide();
    }
};

}

namespace std {

template<>
struct hash<WTF8::u8string> {
public:
    size_t operator()(const WTF8::u8string& s) const {
        return hasher(s);
    }
private:
    std::hash<std::string> hasher;
};

}

#endif

#endif
