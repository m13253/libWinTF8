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

#include <istream>
#include <ostream>
#include "termio.h"

#ifdef _WIN32
#include <streambuf>
#include <windows.h>
#else
#include <iostream>
#endif

namespace WTF8 {

#ifdef _WIN32

class ConsoleOutputBuffer : public std::streambuf {
};
class ConsoleInputBuffer : public std::streambuf {
};

#else

std::istream& cin = std::cin;
std::ostream& cout = std::cout;
std::ostream& cerr = std::cerr;
std::ostream& clog = std::clog;

#endif

}
