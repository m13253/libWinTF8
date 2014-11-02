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
#include <cerrno>
#include <cstdio>
#include "utils.h"
#include "u8str.h"
#include "utfconv.h"
#include "fopen.h"

namespace WinTF8 {

std::FILE* fopen(const char* path, const char* mode) {
#ifdef _WIN32
    try {
        return _wfopen(u8string(path).to_wide(true).c_str(), u8string(mode).to_wide(true).c_str());
    } catch(unicode_conversion_error) {
        errno = EINVAL;
        return nullptr;
    }
#else
    return std::fopen(path, mode);
#endif
}

std::FILE* freopen(const char* path, const char* mode, std::FILE* fp) {
#ifdef _WIN32
    try {
        return _wfreopen(u8string(path).to_wide(true).c_str(), u8string(mode).to_wide(true).c_str(), fp);
    } catch(unicode_conversion_error) {
        WinTF8::fclose(fp);
        errno = EINVAL;
        return nullptr;
    }
#else
    return std::freopen(path, mode, fp);
#endif
}

std::FILE* fclose(std::FILE *fp) {
    std::fclose(fp);
    return nullptr;
}

int remove(const char* path) {
#ifdef _WIN32
    try {
        return _wremove(u8string(path).to_wide(true).c_str());
    } catch(unicode_conversion_error) {
        errno = EINVAL;
        return -1;
    }
#else
    return std::remove(path);
#endif
}

int rename(const char* oldname, const char* newname) {
#ifdef _WIN32
    try {
        return _wrename(u8string(oldname).to_wide(true).c_str(), u8string(newname).to_wide(true).c_str());
    } catch(unicode_conversion_error) {
        errno = EINVAL;
        return -1;
    }
#else
    return std::rename(oldname, newname);
#endif
}

}

extern "C" {

std::FILE *WTF8_fopen(const char *path, const char *mode) {
    return WinTF8::fopen(path, mode);
}

std::FILE *WTF8_freopen(const char *path, const char *mode, std::FILE *fp) {
    return WinTF8::freopen(path, mode, fp);
}

std::FILE *WTF8_fclose(std::FILE *fp) {
    return WinTF8::fclose(fp);
}

int WTF8_remove(const char *path) {
    return WinTF8::remove(path);
}

int WTF8_rename(const char *oldname, const char *newname) {
    return WinTF8::rename(oldname, newname);
}

}
