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
#ifndef WTF8_U8IOS_H_INCLUDED_
#define WTF8_U8IOS_H_INCLUDED_

#ifdef __cplusplus
#include <cstddef>
#include <ios>
#include <fstream>
#include "u8str.h"
#include "utfconv.h"

namespace WinTF8 {

template<typename CharT, typename Traits = std::char_traits<CharT> >
class basic_ifstream : public std::basic_ifstream<CharT, Traits> {
public:
    using std::basic_ifstream<CharT, Traits>::basic_ifstream;
#ifdef _WIN32
    explicit basic_ifstream(const char* filename, std::ios_base::openmode mode = std::ios_base::in) :
        std::basic_ifstream<CharT, Traits>(u8string(filename).to_wide(true), mode) {
    }
    explicit basic_ifstream(const u8string& filename, std::ios_base::openmode mode = std::ios_base::in) :
        std::basic_ifstream<CharT, Traits>(filename.to_wide(true), mode) {
    }
    void open(const char* filename, std::ios_base::openmode mode = std::ios_base::in) {
        std::basic_ifstream<CharT, Traits>::open(u8string(filename).to_wide(true), mode);
    }
    void open(const u8string& filename, std::ios_base::openmode mode = std::ios_base::in) {
        std::basic_ifstream<CharT, Traits>::open(filename.to_wide(true), mode);
    }
#else
    explicit basic_ifstream(const u8string& filename, std::ios_base::openmode mode = std::ios_base::in) :
        std::basic_ifstream<CharT, Traits>(static_cast<std::string>(filename), mode) {
    }
    void open(const u8string& filename, std::ios_base::openmode mode = std::ios_base::in) {
        std::basic_ifstream<CharT, Traits>::open(static_cast<std::string>(filename), mode);
    }
#endif
};

template<typename CharT, typename Traits = std::char_traits<CharT> >
class basic_ofstream : public std::basic_ofstream<CharT, Traits> {
public:
    using std::basic_ofstream<CharT, Traits>::basic_ofstream;
#ifdef _WIN32
    explicit basic_ofstream(const char* filename, std::ios_base::openmode mode = std::ios_base::out) :
        std::basic_ofstream<CharT, Traits>(u8string(filename).to_wide(true), mode) {
    }
    explicit basic_ofstream(const u8string& filename, std::ios_base::openmode mode = std::ios_base::out) :
        std::basic_ofstream<CharT, Traits>(filename.to_wide(true), mode) {
    }
    void open(const char* filename, std::ios_base::openmode mode = std::ios_base::out) {
        std::basic_ofstream<CharT, Traits>::open(u8string(filename).to_wide(true), mode);
    }
    void open(const u8string& filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) {
        std::basic_ofstream<CharT, Traits>::open(filename.to_wide(true), mode);
    }
#else
    explicit basic_ofstream(const u8string& filename, std::ios_base::openmode mode = std::ios_base::out) :
        std::basic_ofstream<CharT, Traits>(static_cast<std::string>(filename), mode) {
    }
    void open(const u8string& filename, std::ios_base::openmode mode = std::ios_base::out) {
        std::basic_ofstream<CharT, Traits>::open(static_cast<std::string>(filename), mode);
    }
#endif
};

template<typename CharT, typename Traits = std::char_traits<CharT> >
class basic_fstream : public std::basic_fstream<CharT, Traits> {
public:
    using std::basic_fstream<CharT, Traits>::basic_fstream;
#ifdef _WIN32
    explicit basic_fstream(const char* filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) :
        std::basic_fstream<CharT, Traits>(u8string(filename).to_wide(true), mode) {
    }
    explicit basic_fstream(const u8string& filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) :
        std::basic_fstream<CharT, Traits>(filename.to_wide(true), mode) {
    }
    void open(const char* filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) {
        std::basic_fstream<CharT, Traits>::open(u8string(filename).to_wide(true), mode);
    }
    void open(const u8string& filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) {
        std::basic_fstream<CharT, Traits>::open(filename.to_wide(true), mode);
    }
#else
    explicit basic_fstream(const u8string& filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) :
        std::basic_fstream<CharT, Traits>(static_cast<std::string>(filename), mode) {
    }
    void open(const u8string& filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) {
        std::basic_fstream<CharT, Traits>::open(static_cast<std::string>(filename), mode);
    }
#endif
};

typedef basic_ifstream<char>    ifstream;
typedef basic_ofstream<char>    ofstream;
typedef basic_fstream<char>     fstream;
typedef basic_ifstream<wchar_t> wifstream;
typedef basic_ofstream<wchar_t> wofstream;
typedef basic_fstream<wchar_t>  wfstream;

}

#endif

#endif
