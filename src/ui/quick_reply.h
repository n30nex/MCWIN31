#pragma once

// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace slopos::ui {

struct QuickReplyContext {
    const char* own_name;
    const char* channel;
    uint32_t epoch;
};

inline void quick_reply_format_time(char* buf, size_t sz, uint32_t epoch)
{
    if (!buf || sz == 0) return;
    if (epoch == 0) {
        snprintf(buf, sz, "--:--");
        return;
    }
    uint32_t t = epoch % 86400;
    snprintf(buf, sz, "%02u:%02u", (unsigned)((t / 3600) % 24),
             (unsigned)((t / 60) % 60));
}

inline void quick_reply_append(char* out, size_t out_sz, size_t& pos, const char* text)
{
    if (!out || out_sz == 0 || !text) return;
    while (*text && pos + 1 < out_sz) {
        out[pos++] = *text++;
    }
    out[pos] = '\0';
}

inline void quick_reply_expand_template(const char* tpl, const QuickReplyContext& ctx,
                                        char* out, size_t out_sz)
{
    if (!out || out_sz == 0) return;
    out[0] = '\0';
    if (!tpl) return;

    char time_buf[8];
    quick_reply_format_time(time_buf, sizeof(time_buf), ctx.epoch);

    size_t pos = 0;
    for (const char* p = tpl; *p && pos + 1 < out_sz;) {
        if (strncmp(p, "{name}", 6) == 0) {
            quick_reply_append(out, out_sz, pos, ctx.own_name ? ctx.own_name : "");
            p += 6;
        } else if (strncmp(p, "{channel}", 9) == 0) {
            quick_reply_append(out, out_sz, pos, ctx.channel ? ctx.channel : "");
            p += 9;
        } else if (strncmp(p, "{time}", 6) == 0) {
            quick_reply_append(out, out_sz, pos, time_buf);
            p += 6;
        } else {
            out[pos++] = *p++;
            out[pos] = '\0';
        }
    }
}

} // namespace slopos::ui
