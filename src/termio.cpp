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

#include <iostream>
#include <istream>
#include <ostream>
#include "utils.h"
#include "u8str.h"
#include "utfconv.h"
#include "termio.h"

#ifdef _WIN32
#include <cassert>
#include <cstddef>
#include <ios>
#include <memory>
#include <streambuf>
#include <vector>
#include <windows.h>
#endif

namespace WTF8 {

#ifdef _WIN32

class ConsoleInputBuffer : public std::streambuf {
public:
    ConsoleInputBuffer(HANDLE handle) :
        WTF8_handle(handle) {
        DWORD dummy;
        WTF8_is_console = GetConsoleMode(handle, &dummy);
    }
private:
    HANDLE WTF8_handle;
    bool WTF8_is_console;
};

class ConsoleOutputBuffer : public std::streambuf {
public:
    ConsoleOutputBuffer(HANDLE handle) :
        WTF8_handle(handle) {
        DWORD dummy;
        WTF8_is_console = GetConsoleMode(handle, &dummy);
        setp(WTF8_buffer, WTF8_buffer+WTF8_buffer_size);
    }
protected:
    std::streambuf *setbuf(char *buffer, std::streamsize size) {
        if(!WTF8_init_buffer.empty()) {
            WTF8_init_buffer.resize(0);
            WTF8_init_buffer.shrink_to_fit();
        }
        WTF8_buffer = buffer;
        WTF8_buffer_size = size;
        setp(WTF8_buffer, WTF8_buffer+WTF8_buffer_size);
        return this;
    }
    int sync() {
        ptrdiff_t size = pptr()-pbase();
        assert(size >= 0);
        if(size > 0) {
            ssize_t written = write(pbase(), size_t(size));
            if(written <= 0)
                return -1;
            else if(written < size) {
                memmove(pbase(), pbase()+written, size-written);
                setp(pbase(), epptr());
                pbump(size-written);
            } else
                setp(pbase(), epptr());
            return 0;
        } else if(size == 0)
            return 0;
        else
            return -1;
    }
    int overflow(int c) {
        if(sync() != 0)
            return EOF;
        if(c != EOF) {
            if(pptr() == epptr())
                return EOF;
            *pptr() = c;
            pbump(1);
        }
        return c;
    }
private:
    ssize_t write(const char *buf, size_t size) {
        if(WTF8_is_console) {
            const char *last_char = buf+size;
            while(last_char != buf)
                if(uint8_t(last_char[-1] & 0xc0) == 0x80)
                    --last_char;
            if(last_char == buf)
                last_char = buf+size;
            std::wstring wbuf = u8string(buf, size_t(last_char-buf)).to_wide();
            if(last_char != buf)
                try {
                    wbuf.append(u8string(last_char, size_t(size-(last_char-buf))).to_wide(true));
                } catch(unicode_conversion_error) {
                    size = size_t(last_char-buf);
                }
            DWORD wchars_written = 0;
            if(!WriteConsoleW(WTF8_handle, wbuf.data(), wbuf.size(), &wchars_written, nullptr))
                return -1;
            else if(wchars_written == wbuf.size())
                return size;
            else
                return min(u8string::from_wide(wbuf.substr(0, wchars_written)).size(), size);
        } else {
            DWORD bytes_written = 0;
            if(!WriteFile(WTF8_handle, buf, size, &bytes_written, nullptr))
                return -1;
            else
                return bytes_written;
        }
    }
    HANDLE WTF8_handle;
    bool WTF8_is_console;
    std::vector<char> WTF8_init_buffer = std::vector<char>(1024);
    char *WTF8_buffer = WTF8_init_buffer.data();
    std::streamsize WTF8_buffer_size = 1024;
};

class ConsoleInput : public std::istream {
public:
    using std::istream::istream;
    ConsoleInput() :
        WTF8_buffer(new ConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE))) {
        rdbuf(WTF8_buffer.get());
    }
private:
    std::unique_ptr<ConsoleInputBuffer> WTF8_buffer;
};

class ConsoleOutput : public std::ostream {
public:
    using std::ostream::ostream;
    ConsoleOutput(int fd = 1) :
        WTF8_buffer(new ConsoleOutputBuffer(GetStdHandle(fd != 2 ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE))) {
        rdbuf(WTF8_buffer.get());
    }
private:
    std::unique_ptr<ConsoleOutputBuffer> WTF8_buffer;
};

static ConsoleInput cin_inst;
static ConsoleOutput cout_inst(1);
static ConsoleOutput cerr_inst(2);
static ConsoleOutput clog_inst(2);

static struct ConsoleInitializer {
    ConsoleInitializer() {
        cin_inst.tie(&cout_inst);
        cerr_inst.tie(&cout_inst);
        clog_inst.tie(&cout_inst);
    }
} console_initializer;

std::istream &cin = cin_inst;
std::ostream &cout = cout_inst;
std::ostream &cerr = cerr_inst;
std::ostream &clog = clog_inst;

#else

std::istream &cin = std::cin;
std::ostream &cout = std::cout;
std::ostream &cerr = std::cerr;
std::ostream &clog = std::clog;

#endif

}
