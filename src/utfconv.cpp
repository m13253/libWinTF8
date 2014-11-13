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
#include <cstddef>
#include <cstring>
#include "utils.h"
#include "utfconv.h"

namespace WTF8 {

static bool utf8_check_continuation(const u8string &utf8str, size_t start, size_t check_length) {
    if(utf8str.size() > start + check_length) {
        while(check_length--)
            if((uint8_t(utf8str[++start]) & 0xc0) != 0x80)
                return false;
        return true;
    } else
        return false;
}

std::wstring utf8_to_wide(const u8string &utf8str, bool strict) {
    std::wstring widestr;
    size_t i = 0;
    widestr.reserve(utf8str.size()/2);
    while(i < utf8str.size()) {
        if(uint8_t(utf8str[i]) < 0x80) {
            widestr.push_back(utf8str[i]);
            ++i;
            continue;
        } else if(uint8_t(utf8str[i]) < 0xc0) {
        } else if(uint8_t(utf8str[i]) < 0xe0) {
            if(utf8_check_continuation(utf8str, i, 1)) {
                uint32_t ucs4 = uint32_t(utf8str[i] & 0x1f) << 6 | uint32_t(utf8str[i+1] & 0x3f);
                if(ucs4 >= 0x80) {
                    widestr.push_back(wchar_t(ucs4));
                    i += 2;
                    continue;
                }
            }
        } else if(uint8_t(utf8str[i]) < 0xf0) {
            if(utf8_check_continuation(utf8str, i, 2)) {
                uint32_t ucs4 = uint32_t(utf8str[i] & 0xf) << 12 | uint32_t(utf8str[i+1] & 0x3f) << 6 | (utf8str[i+2] & 0x3f);
                if(ucs4 >= 0x800 && (ucs4 & 0xf800) != 0xd800) {
                    widestr.push_back(wchar_t(ucs4));
                    i += 3;
                    continue;
                }
            }
        } else if(uint8_t(utf8str[i]) < 0xf8) {
            if(utf8_check_continuation(utf8str, i, 3)) {
                uint32_t ucs4 = uint32_t(utf8str[i] & 0x7) << 18 | uint32_t(utf8str[i+1] & 0x3f) << 12 | uint32_t(utf8str[i+2] & 0x3f) << 6 | uint32_t(utf8str[i+3] & 0x3f);
                if(ucs4 >= 0x10000 && ucs4 < 0x110000) {
                    if(sizeof (wchar_t) >= 4)
                        widestr.push_back(wchar_t(ucs4));
                    else {
                        ucs4 -= 0x10000;
                        widestr.append({
                            wchar_t(ucs4 >> 10 | 0xd800),
                            wchar_t((ucs4 & 0x3ff) | 0xdc00)
                        });
                    }
                    i += 4;
                    continue;
                }
            }
        }
        if(strict)
            throw unicode_conversion_error();
        else {
            widestr.push_back(0xfffd);
            ++i;
        }
    }
    widestr.shrink_to_fit();
    return widestr;
}

u8string wide_to_utf8(const std::wstring &widestr, bool strict) {
    u8string utf8str;
    size_t i = 0;
    utf8str.reserve(widestr.size()*2);
    while(i < widestr.size()) {
        if(uint32_t(widestr[i]) < 0x80) {
            utf8str.push_back(char(widestr[i]));
            ++i;
        } else if(uint32_t(widestr[i]) < 0x800) {
            utf8str.append({
                char(widestr[i] >> 6 | 0xc0),
                char((widestr[i] & 0x3f) | 0x80)
            });
            ++i;
        } else if(sizeof (wchar_t) >= 4) {
            if(uint32_t(widestr[i]) < 0x10000) {
                if((uint32_t(widestr[i]) & 0xf800) != 0xd800) {
                    utf8str.append({
                        char(widestr[i] >> 12 | 0xe0),
                        char(((widestr[i] >> 6) & 0x3f) | 0x80),
                        char((widestr[i] & 0x3f) | 0x80)
                    });
                    ++i;
                } else if(strict) {
                    throw unicode_conversion_error();
                } else {
                    utf8str.append("\xef\xbf\xbd", 3);
                    ++i;
                }
            } else if(uint32_t(widestr[i]) < 0x110000) {
                utf8str.append({
                    char(widestr[i] >> 18 | 0xf0),
                    char(((widestr[i] >> 12) & 0x3f) | 0x80),
                    char(((widestr[i] >> 6) & 0x3f) | 0x80),
                    char((widestr[i] & 0x3f) | 0x80)
                });
                ++i;
            } else if(strict) {
                throw unicode_conversion_error();
            } else {
                utf8str.append("\xef\xbf\xbd", 3);
                ++i;
            }
        } else {
            if((uint16_t(widestr[i]) & 0xf800) != 0xd800) {
                    utf8str.append({
                        char(widestr[i] >> 12 | 0xe0),
                        char(((widestr[i] >> 6) & 0x3f) | 0x80),
                        char((widestr[i] & 0x3f) | 0x80)
                    });
                    ++i;
            } else if(i+1 < widestr.size() && uint16_t(widestr[i] & 0xfc00) == 0xd800 && uint16_t(widestr[i+1] & 0xfc00) == 0xdc00) {
                uint32_t ucs4 = (uint32_t(widestr[i] & 0x3ff) << 10 | (widestr[i+1] & 0x3ff)) + 0x10000;
                utf8str.append({
                    char(ucs4 >> 18 | 0xf0),
                    char(((ucs4 >> 12) & 0x3f) | 0x80),
                    char(((ucs4 >> 6) & 0x3f) | 0x80),
                    char((ucs4 & 0x3f) | 0x80),
                });
                i += 2;
            } else if(strict) {
                throw unicode_conversion_error();
            } else {
                utf8str.append("\xef\xbf\xbd", 3);
                ++i;
            }
        }
    }
    utf8str.shrink_to_fit();
    return utf8str;
}

}

extern "C" {

size_t WTF8_utf8_to_wide(wchar_t *widestr, const char *utf8str, int strict, size_t bufsize) {
    try {
        std::wstring widestrpp = WTF8::utf8_to_wide(WTF8::u8string(utf8str), strict != 0);
        if(widestr && bufsize != 0) {
            std::memcpy(widestr, widestrpp.data(), WTF8::min(widestrpp.size(), bufsize-1)*sizeof (wchar_t));
            widestr[WTF8::min(widestrpp.size(), bufsize)] = L'\0';
        }
        return widestrpp.size();
    } catch(WTF8::unicode_conversion_error) {
        return WTF8_UNICODE_CONVERT_ERROR;
    }
}

size_t WTF8_wide_to_utf8(char *utf8str, const wchar_t *widestr, int strict, size_t bufsize) {
    try {
        WTF8::u8string utf8strpp = WTF8::wide_to_utf8(std::wstring(widestr), strict != 0);
        if(utf8str && bufsize != 0) {
            std::memcpy(utf8str, utf8strpp.data(), WTF8::min(utf8strpp.size(), bufsize-1)*sizeof (char));
            utf8str[WTF8::min(utf8strpp.size(), bufsize)] = '\0';
        }
        return utf8strpp.size();
    } catch(WTF8::unicode_conversion_error) {
        return WTF8_UNICODE_CONVERT_ERROR;
    }
}

}
