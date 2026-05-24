#pragma once

// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstddef>
#include <cstring>

namespace slopos::ui {

enum class TerminalCommand {
    Empty,
    Help,
    Status,
    Advert,
    Ping,
    Neighbors,
    Unknown,
};

struct TerminalCommandLine {
    TerminalCommand type;
    char arg[64];
};

inline bool terminal_is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

inline bool terminal_token_equals(const char* token, size_t len, const char* expected)
{
    return strlen(expected) == len && strncmp(token, expected, len) == 0;
}

inline void terminal_copy_arg(char* dst, size_t dst_sz, const char* src, size_t len)
{
    if (!dst || dst_sz == 0) return;
    size_t n = len < dst_sz - 1 ? len : dst_sz - 1;
    if (src && n) memcpy(dst, src, n);
    dst[n] = '\0';
}

inline TerminalCommandLine terminal_parse_command(const char* input)
{
    TerminalCommandLine out{TerminalCommand::Empty, {0}};
    if (!input) return out;

    const char* p = input;
    while (*p && terminal_is_space(*p)) p++;
    if (!*p) return out;

    const char* token = p;
    while (*p && !terminal_is_space(*p)) p++;
    size_t token_len = (size_t)(p - token);

    while (*p && terminal_is_space(*p)) p++;
    const char* arg = p;
    const char* end = input + strlen(input);
    while (end > arg && terminal_is_space(*(end - 1))) end--;
    terminal_copy_arg(out.arg, sizeof(out.arg), arg, (size_t)(end - arg));

    if (terminal_token_equals(token, token_len, "help")) {
        out.type = TerminalCommand::Help;
    } else if (terminal_token_equals(token, token_len, "status")) {
        out.type = TerminalCommand::Status;
    } else if (terminal_token_equals(token, token_len, "advert")) {
        out.type = TerminalCommand::Advert;
    } else if (terminal_token_equals(token, token_len, "ping")) {
        out.type = TerminalCommand::Ping;
    } else if (terminal_token_equals(token, token_len, "neighbors") ||
               terminal_token_equals(token, token_len, "neighbours") ||
               terminal_token_equals(token, token_len, "neighbor") ||
               terminal_token_equals(token, token_len, "neighbour")) {
        out.type = TerminalCommand::Neighbors;
    } else {
        out.type = TerminalCommand::Unknown;
        terminal_copy_arg(out.arg, sizeof(out.arg), input, strlen(input));
    }

    return out;
}

} // namespace slopos::ui
