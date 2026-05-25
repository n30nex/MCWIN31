#pragma once

// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstddef>
#include <cstring>

namespace slopos {

static constexpr size_t CLIPBOARD_TEXT_MAX = 192;

inline bool clipboard_trim_space(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

inline size_t clipboard_prepare_text(char* dst, size_t dst_sz, const char* src)
{
    if (!dst || dst_sz == 0) return 0;
    dst[0] = '\0';
    if (!src) return 0;

    const char* start = src;
    while (*start && clipboard_trim_space(*start)) start++;

    const char* end = src + strlen(src);
    while (end > start && clipboard_trim_space(*(end - 1))) end--;

    size_t written = 0;
    for (const char* p = start; p < end && written + 1 < dst_sz; ++p) {
        char c = *p;
        if (c == '\t' || c == '\r' || c == '\n') c = ' ';
        if ((unsigned char)c < 0x20 || c == 0x7f) continue;
        dst[written++] = c;
    }
    dst[written] = '\0';
    return written;
}

bool clipboard_load(char* out, size_t out_sz);
bool clipboard_save(const char* text);
bool clipboard_clear();

} // namespace slopos
