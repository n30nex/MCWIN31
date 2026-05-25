// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 Ben

#include "mesh_diagnostics.h"

#include <cstring>

namespace slopos::mesh {

namespace {

MeshDiagnosticEvent g_events[DIAGNOSTIC_RING_MAX];
int g_head = 0;
int g_count = 0;

} // namespace

void diagnostics_clear()
{
    g_head = 0;
    g_count = 0;
    std::memset(g_events, 0, sizeof(g_events));
}

void diagnostics_record(DiagnosticEventType type, uint32_t timestamp, int rssi, float snr,
                        const char* peer, const uint8_t* payload, uint8_t payload_len,
                        uint8_t path_len)
{
    MeshDiagnosticEvent& event = g_events[g_head];
    std::memset(&event, 0, sizeof(event));

    event.type = type;
    event.timestamp = timestamp;
    event.rssi = rssi;
    event.snr = snr;
    event.payload_len = payload_len;
    event.path_len = path_len;

    if (peer) {
        std::strncpy(event.peer, peer, sizeof(event.peer) - 1);
        event.peer[sizeof(event.peer) - 1] = '\0';
    }

    if (payload && payload_len > 0) {
        uint8_t sample_len = payload_len;
        if (sample_len > DIAGNOSTIC_SAMPLE_MAX) sample_len = DIAGNOSTIC_SAMPLE_MAX;
        std::memcpy(event.sample, payload, sample_len);
        event.sample_len = sample_len;
    }

    g_head = (g_head + 1) % DIAGNOSTIC_RING_MAX;
    if (g_count < DIAGNOSTIC_RING_MAX) g_count++;
}

int diagnostics_count()
{
    return g_count;
}

int diagnostics_export(MeshDiagnosticEvent* out, int max)
{
    if (!out || max <= 0 || g_count <= 0) return 0;

    int n = g_count < max ? g_count : max;
    int start = g_head - g_count;
    while (start < 0) start += DIAGNOSTIC_RING_MAX;

    for (int i = 0; i < n; i++) {
        out[i] = g_events[(start + i) % DIAGNOSTIC_RING_MAX];
    }
    return n;
}

const char* diagnostic_event_type_name(DiagnosticEventType type)
{
    switch (type) {
        case DiagnosticEventType::RawRx: return "RawRx";
        case DiagnosticEventType::RawData: return "RawData";
        case DiagnosticEventType::ControlData: return "ControlData";
        case DiagnosticEventType::PeerPath: return "PeerPath";
        case DiagnosticEventType::FloodPath: return "FloodPath";
    }
    return "Unknown";
}

} // namespace slopos::mesh
