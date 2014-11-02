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
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <unordered_map>
#include "utils.h"
#include "u8str.h"

namespace WinTF8 {

#ifdef _WIN32
static std::unordered_map<u8string, u8string> env_cache;
#endif

char* getenv(const char* varname, bool ignore_cache = false) {
    using namespace std;
#ifdef _WIN32
    u8string varname_(varname);
    if(!ignore_cache)
        try {
            return const_cast<char*>(env_cache.at(varname_).c_str());
        } catch(std::out_of_range) {
        }
    wchar_t* result = _wgetenv(varname_.to_wide().c_str());
    if(result) {
        u8string& cache_item = env_cache[varname_];
        cache_item = u8string(result);
        return const_cast<char*>(cache_item.c_str());
    } else
        return nullptr;
#else
    (void) ignore_cache;
    return getenv(varname);
#endif
}

int putenv(const char* envstring) {
    using namespace std;
#ifdef _WIN32
    const char* equal_sign = std::strchr(envstring, '=');
    if(equal_sign) {
        env_cache.erase(u8string(envstring, equal_sign-envstring));
        return _wputenv(u8string(envstring).to_wide().c_str());
    } else {
        errno = EINVAL;
        return -1;
    }
#else
    return putenv(envstring);
#endif
}

}

extern "C" {

char *WTF8_getenv(const char *varname) {
    return WinTF8::getenv(varname);
}

char *WTF8_getenv_nocache(const char *varname) {
    return WinTF8::getenv(varname, true);
}

int WTF8_putenv(const char *envstring) {
    return WinTF8::putenv(envstring);
}

}
