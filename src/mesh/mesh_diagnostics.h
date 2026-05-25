// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 Ben
//
// Fixed-size MeshCore diagnostics ring for lightweight RX/path inspection.

#pragma once

#include <cstdint>

namespace slopos::mesh {

enum class DiagnosticEventType : uint8_t {
    RawRx,
    RawData,
    ControlData,
    PeerPath,
    FloodPath,
};

static constexpr int DIAGNOSTIC_SAMPLE_MAX = 16;
static constexpr int DIAGNOSTIC_RING_MAX = 32;

struct MeshDiagnosticEvent {
    DiagnosticEventType type;
    uint32_t timestamp;
    int rssi;
    float snr;
    uint8_t payload_len;
    uint8_t sample_len;
    uint8_t path_len;
    char peer[32];
    uint8_t sample[DIAGNOSTIC_SAMPLE_MAX];
};

void diagnostics_clear();
void diagnostics_record(DiagnosticEventType type, uint32_t timestamp, int rssi, float snr,
                        const char* peer, const uint8_t* payload, uint8_t payload_len,
                        uint8_t path_len);
int diagnostics_count();
int diagnostics_export(MeshDiagnosticEvent* out, int max);
const char* diagnostic_event_type_name(DiagnosticEventType type);

} // namespace slopos::mesh
