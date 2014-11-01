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
#include <vector>
#include "utils.h"
#include "u8str.h"
#include "argv.h"

#if defined(_WIN32)
extern "C" extern int __argc;
extern "C" extern wchar_t** __wargv;
#elif defined(__APPLE__) && defined(__MACH__)
extern "C" extern int* _NSGetArgc();
extern "C" extern char*** _NSGetArgv();
#endif

namespace WinTF8 {

std::vector<u8string> get_argv(int argc_hint, char* argv_hint[]) {
#if defined(_WIN32)
    int argc = __argc;
#elif defined(__APPLE__) && defined(__MACH__)
    int argc = *_NSGetArgc();
    char **argv = *_NSGetArgv();
#else
    int argc = argc_hint;
#endif
    std::vector<u8string> result;
    result.reserve(argc);
    for(int i = 0; i < argc; ++i) {
#if defined(_WIN32)
        result.push_back(wide_to_utf8(std::wstring(__wargv[i])));
#elif defined(__APPLE__) && defined(__MACH__)
        result.push_back(argv[i]);
#else
        result.push_back(argv_hint[i]);
#endif
    }
    return result;
}

}

extern "C" {

int WTF8_get_argc(int argc_hint) {
#if defined(_WIN32)
    return __argc;
#elif defined(__APPLE__) && defined(__MACH__)
    return *_NSGetArgc();
#else
    return argc_hint;
#endif
}

char **WTF8_get_argv(int argc_hint, const char **argv_hint) {
    int argc = WTF8_get_argc(argc_hint);
#if defined(__APPLE__) && defined(__MACH__)
    char **argv = *_NSGetArgv();
#endif
    char **result = new char *[argc+1];
    for(int i = 0; i < argc; ++i) {
#if defined(_WIN32)
        result[i] = WinTF8::new_c_str(wide_to_utf8(std::wstring(__wargv[i])));
#elif defined(__APPLE__) && defined(__MACH__)
        result[i] = WinTF8::new_c_str(argv[i]);
#else
        result[i] = WinTF8::new_c_str(argv_hint[i]);
#endif
    }
    result[argc] = nullptr;
    return result;
}

char **WTF8_free_argv(int argc_got, char **argv_got) {
    if(argv_got) {
        for(int i = 0; i < argc_got; ++i)
            WinTF8::delete_c_str(argv_got[i]);
        delete[] argv_got;
    }
    return nullptr;
}

}
