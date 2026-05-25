// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 Ben

#pragma once

#include "terminal_commands.h"
#include "../mesh/mesh_wrapper.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace slopos::ui {

static constexpr int TERMINAL_DIAGNOSTICS_HISTORY_MAX = 4;

inline bool terminal_diagnostics_arg_equals(const char* arg, const char* expected)
{
    return arg && expected && std::strcmp(arg, expected) == 0;
}

inline void terminal_diagnostics_appendf(char* out, size_t out_sz, size_t& used,
                                         const char* fmt, ...)
{
    if (!out || out_sz == 0 || used >= out_sz) return;

    va_list args;
    va_start(args, fmt);
    int written = std::vsnprintf(out + used, out_sz - used, fmt, args);
    va_end(args);

    if (written <= 0) return;
    used += static_cast<size_t>(written);
    if (used >= out_sz) {
        used = out_sz - 1;
        out[used] = '\0';
    }
}

inline void terminal_diagnostics_format_sample(const mesh::MeshDiagnosticEvent& event,
                                               char* out, size_t out_sz)
{
    if (!out || out_sz == 0) return;
    if (event.sample_len == 0) {
        std::snprintf(out, out_sz, "-");
        return;
    }

    size_t used = 0;
    for (uint8_t i = 0; i < event.sample_len && used < out_sz; ++i) {
        int written = std::snprintf(out + used, out_sz - used, "%s%02X",
                                    i == 0 ? "" : " ", event.sample[i]);
        if (written <= 0) break;
        used += static_cast<size_t>(written);
        if (used >= out_sz) {
            out[out_sz - 1] = '\0';
            break;
        }
    }
}

inline void terminal_diagnostics_describe_latest(char* result, size_t result_sz,
                                                 const mesh::MeshDiagnosticEvent* events,
                                                 int n)
{
    if (n <= 0) {
        std::snprintf(result, result_sz, "Diag: no mesh events yet");
        return;
    }

    const auto& event = events[n - 1];
    char sample[mesh::DIAGNOSTIC_SAMPLE_MAX * 3 + 1];
    terminal_diagnostics_format_sample(event, sample, sizeof(sample));

    std::snprintf(result, result_sz,
                  "Diag latest (%d stored): t=%lu %s len=%u path=%u RSSI:%d SNR:%.1f peer:%s sample:%s",
                  mesh::diagnosticEventCount(),
                  static_cast<unsigned long>(event.timestamp),
                  mesh::diagnostic_event_type_name(event.type),
                  event.payload_len, event.path_len, event.rssi, event.snr,
                  event.peer[0] ? event.peer : "-", sample);
}

inline void terminal_diagnostics_describe_history(char* result, size_t result_sz,
                                                  const mesh::MeshDiagnosticEvent* events,
                                                  int n)
{
    if (n <= 0) {
        std::snprintf(result, result_sz, "Diag history: no mesh events yet");
        return;
    }

    size_t used = 0;
    terminal_diagnostics_appendf(result, result_sz, used, "Diag history (%d stored):",
                                 mesh::diagnosticEventCount());

    int start = n > TERMINAL_DIAGNOSTICS_HISTORY_MAX ? n - TERMINAL_DIAGNOSTICS_HISTORY_MAX : 0;
    for (int i = start; i < n; ++i) {
        const auto& event = events[i];
        terminal_diagnostics_appendf(result, result_sz, used,
                                     " #%d:%s l=%u p=%u r=%d s=%.1f",
                                     i + 1,
                                     mesh::diagnostic_event_type_name(event.type),
                                     event.payload_len, event.path_len,
                                     event.rssi, event.snr);
        if (event.peer[0]) {
            terminal_diagnostics_appendf(result, result_sz, used, " @%s", event.peer);
        }
    }
}

inline void terminal_describe_diagnostics(const TerminalCommandLine& parsed,
                                          char* result, size_t result_sz)
{
    if (!result || result_sz == 0) return;

    if (terminal_diagnostics_arg_equals(parsed.arg, "clear")) {
        mesh::clearDiagnostics();
        std::snprintf(result, result_sz, "Diag cleared");
        return;
    }

    mesh::MeshDiagnosticEvent events[mesh::DIAGNOSTIC_RING_MAX];
    int n = mesh::exportDiagnostics(events, mesh::DIAGNOSTIC_RING_MAX);

    if (parsed.arg[0] == '\0' ||
        terminal_diagnostics_arg_equals(parsed.arg, "latest")) {
        terminal_diagnostics_describe_latest(result, result_sz, events, n);
    } else if (terminal_diagnostics_arg_equals(parsed.arg, "list") ||
               terminal_diagnostics_arg_equals(parsed.arg, "history")) {
        terminal_diagnostics_describe_history(result, result_sz, events, n);
    } else {
        std::snprintf(result, result_sz, "Diag usage: diag [latest|list|history|clear]");
    }
}

} // namespace slopos::ui
