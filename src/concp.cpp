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
#include "utils.h"
#include <iostream>

#ifdef _WIN32
extern "C" {
__declspec(dllimport) bool __stdcall SetConsoleOutputCP(unsigned int wCodePageID);
__declspec(dllimport) bool __stdcall SetConsoleCP(unsigned int wCodePageID);
}
#endif

namespace WinTF8 {

#ifdef _WIN32
static bool cp_already_set = false;
#endif

bool init_console() {
#ifdef _WIN32
    if(!cp_already_set) {
        cp_already_set = true;
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
        return true;
    }
#endif
    return false;
}

}

extern "C" {

int WTF8_init_console(void) {
    return WinTF8::init_console();
}

#ifdef __GNUC__
static int WTF8_init_console(void) __attribute__((constructor));
#elif defined(_MSC_VER)
static int WTF8_init_console_wrapper(void) {
    WinTF8::init_console();
    return 0;
}
#pragma section(".CRT$XCU", read)
__declspec(allocate(".CRT$XCU")) static int (* _array_WTF8_init_console)(void) = WTF8_init_console_wrapper;
#endif

}

#endif
