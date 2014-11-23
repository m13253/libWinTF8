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
#include <string>
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
        if(WTF8_is_console)
            WTF8_wbuffer.resize(1024);
        else
            WTF8_buffer.reserve(1024);
        setg(nullptr, nullptr, nullptr);
    }
protected:
    std::streambuf *setbuf(char *buffer, std::streamsize size) {
        if(size == 0)
            size = 1024;
        /* only use the size parameter */
        if(WTF8_is_console) {
            WTF8_wbuffer.resize(size);
            WTF8_wbuffer.shrink_to_fit();
        } else {
            WTF8_buffer.resize(0);
            WTF8_buffer.reserve(size);
        }
        WTF8_buffer_size = size;
        return this;
    }
    int underflow() {
        if(has_last_putback) {
            if(gptr() != egptr())
                return last_putback;
            else
                setg(WTF8_buffer.data(), WTF8_buffer.data(), WTF8_buffer.data()+WTF8_buffer.size());
        }
        if(gptr() == egptr()) {
            if(read() == -1)
                return EOF;
            if(gptr() == egptr())
                return EOF;
        }
        return std::char_traits<char>::to_int_type(*gptr());
    }
    int pbackfail(int c = EOF) {
        if(has_last_putback)
            return EOF;
        else if(gptr() != eback()) {
            gptr()[-1] = char(c);
            gbump(-1);
            return c;
        } else {
            has_last_putback = true;
            last_putback = c;
            setg(&last_putback, &last_putback, &last_putback+1);
            return c;
        }
    }
private:
    ssize_t read() {
        if(WTF8_is_console) {
            DWORD wchars_read = 0;
            if(last_surrogate_pair == L'\0') {
                if(!ReadConsoleW(WTF8_handle, WTF8_wbuffer.data(), WTF8_buffer_size, &wchars_read, nullptr))
                    return -1;
            } else {
                WTF8_wbuffer[0] = last_surrogate_pair;
                if(!ReadConsoleW(WTF8_handle, WTF8_wbuffer.data()+1, WTF8_buffer_size-1, &wchars_read, nullptr))
                    return -1;
                ++wchars_read;
            }
            if(std::streamsize(wchars_read) == WTF8_buffer_size && uint16_t(WTF8_wbuffer[wchars_read-1] & 0xfc00) == 0xd800)
                last_surrogate_pair = WTF8_wbuffer[--wchars_read];
            else
                last_surrogate_pair = L'\0';
            u8string WTF8_sbuffer = u8string::from_wide(std::wstring(WTF8_wbuffer.data(), wchars_read));
            WTF8_buffer.assign(WTF8_sbuffer.cbegin(), WTF8_sbuffer.cend());
        } else {
            DWORD bytes_read = 0;
            WTF8_buffer.resize(WTF8_buffer_size);
            if(!ReadFile(WTF8_handle, WTF8_buffer.data(), WTF8_buffer.size(), &bytes_read, nullptr))
                return -1;
            WTF8_buffer.resize(bytes_read);
        }
        setg(WTF8_buffer.data(), WTF8_buffer.data(), WTF8_buffer.data()+WTF8_buffer.size());
        return WTF8_buffer.size();
    }
    HANDLE WTF8_handle;
    bool WTF8_is_console;
    std::vector<char> WTF8_buffer;
    std::vector<wchar_t> WTF8_wbuffer;
    wchar_t last_surrogate_pair = L'\0';
    bool has_last_putback = false;
    char last_putback;
    std::streamsize WTF8_buffer_size = 1024;
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
        sync();
        if(size == 0) {
            WTF8_init_buffer.resize(1024);
            WTF8_init_buffer.shrink_to_fit();
            buffer = WTF8_init_buffer.data();
            size = 1024;
        } else if(!WTF8_init_buffer.empty()) {
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
                if(uint8_t(*--last_char & 0xc0) != 0x80)
                    break;
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
                /* UTF conversion error may happen, the accurate number of chars is unknown */
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
    ConsoleInput(std::ostream *tied_stream) :
        WTF8_buffer(new ConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE))) {
        rdbuf(WTF8_buffer.get());
        if(tied_stream)
            tie(tied_stream);
    }
protected:
    ConsoleInput() {
    }
private:
    std::unique_ptr<ConsoleInputBuffer> WTF8_buffer;
};

class ConsoleOutput : public std::ostream {
public:
    ConsoleOutput(int fd, std::ostream *tied_stream = nullptr) :
        WTF8_buffer(new ConsoleOutputBuffer(GetStdHandle(fd != 2 ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE))) {
        rdbuf(WTF8_buffer.get());
        if(tied_stream)
            tie(tied_stream);
    }
protected:
    ConsoleOutput() {
    }
private:
    std::unique_ptr<ConsoleOutputBuffer> WTF8_buffer;
};

static ConsoleOutput cout_inst(1);
static ConsoleInput cin_inst(&cout_inst);
static ConsoleOutput cerr_inst(2, &cout_inst);
static ConsoleOutput clog_inst(2, &cout_inst);

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

extern "C" {

int WTF8_fgetc(std::FILE *stream) {
    return WTF8::fgetc(stream);
}

char *WTF8_fgets(char *s, int size, std::FILE *stream) {
    return WTF8::fgets(s, size, stream);
}

int WTF8_ungetc(int c, std::FILE *stream) {
    return WTF8::ungetc(c, stream);
}

int WTF8_fputc(int c, std::FILE *stream) {
    return WTF8::fputc(c, stream);
}

int WTF8_fputs(const char *s, std::FILE *stream) {
    return WTF8::fputs(s, stream);
}

int WTF8_feof(std::FILE *stream) {
    return WTF8::feof(stream);
}

int WTF8_puts(const char *s) {
    return WTF8::puts(s);
}

}
