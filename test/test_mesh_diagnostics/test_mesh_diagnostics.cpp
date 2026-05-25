// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 Ben
//
// This file is part of SlopOS-TDeck.
//
// SlopOS-TDeck is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SlopOS-TDeck is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SlopOS-TDeck.  If not, see <https://www.gnu.org/licenses/>.


/**
 * Unit tests for the mesh diagnostics event ring.
 * Tests: empty state, export ordering, truncation, wrap, clear, and type names.
 */
#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>

#if __has_include("mesh/mesh_diagnostics.h")
#include "mesh/mesh_diagnostics.h"
#else
namespace slopos::mesh {
enum class DiagnosticEventType : uint8_t {
    RawRx,
    RawData,
    ControlData,
    PeerPath,
    FloodPath
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
void diagnostics_record(
    DiagnosticEventType type,
    uint32_t timestamp,
    int rssi,
    float snr,
    const char* peer,
    const uint8_t* payload,
    uint8_t payload_len,
    uint8_t path_len
);
int diagnostics_count();
int diagnostics_export(MeshDiagnosticEvent* out, int max);
const char* diagnostic_event_type_name(DiagnosticEventType type);
} // namespace slopos::mesh
#endif

namespace {

using slopos::mesh::DIAGNOSTIC_RING_MAX;
using slopos::mesh::DIAGNOSTIC_SAMPLE_MAX;
using slopos::mesh::DiagnosticEventType;
using slopos::mesh::MeshDiagnosticEvent;

class MeshDiagnosticsTest : public ::testing::Test {
protected:
    void SetUp() override {
        slopos::mesh::diagnostics_clear();
    }

    void TearDown() override {
        slopos::mesh::diagnostics_clear();
    }
};

void record_event(
    DiagnosticEventType type,
    uint32_t timestamp,
    const char* peer = "node-a",
    const uint8_t* payload = nullptr,
    uint8_t payload_len = 0,
    uint8_t path_len = 0
) {
    slopos::mesh::diagnostics_record(
        type,
        timestamp,
        -90 + static_cast<int>(timestamp),
        3.25f,
        peer,
        payload,
        payload_len,
        path_len
    );
}

TEST_F(MeshDiagnosticsTest, EmptyStateExportsNoEvents) {
    MeshDiagnosticEvent out[2] = {};

    EXPECT_EQ(slopos::mesh::diagnostics_count(), 0);
    EXPECT_EQ(slopos::mesh::diagnostics_export(out, 2), 0);
    EXPECT_EQ(slopos::mesh::diagnostics_export(nullptr, 2), 0);
    EXPECT_EQ(slopos::mesh::diagnostics_export(out, 0), 0);
}

TEST_F(MeshDiagnosticsTest, RecordExportPreservesInsertionOrderAndMetadata) {
    const uint8_t first_payload[] = {0x10, 0x20, 0x30};
    const uint8_t second_payload[] = {0xaa, 0xbb};

    record_event(DiagnosticEventType::RawRx, 101, "alpha", first_payload, sizeof(first_payload), 1);
    record_event(DiagnosticEventType::ControlData, 102, "beta", second_payload, sizeof(second_payload), 2);

    MeshDiagnosticEvent out[4] = {};
    ASSERT_EQ(slopos::mesh::diagnostics_count(), 2);
    ASSERT_EQ(slopos::mesh::diagnostics_export(out, 4), 2);

    EXPECT_EQ(out[0].type, DiagnosticEventType::RawRx);
    EXPECT_EQ(out[0].timestamp, 101u);
    EXPECT_EQ(out[0].rssi, 11);
    EXPECT_FLOAT_EQ(out[0].snr, 3.25f);
    EXPECT_STREQ(out[0].peer, "alpha");
    EXPECT_EQ(out[0].payload_len, sizeof(first_payload));
    EXPECT_EQ(out[0].sample_len, sizeof(first_payload));
    EXPECT_EQ(out[0].path_len, 1u);
    EXPECT_EQ(std::memcmp(out[0].sample, first_payload, sizeof(first_payload)), 0);

    EXPECT_EQ(out[1].type, DiagnosticEventType::ControlData);
    EXPECT_EQ(out[1].timestamp, 102u);
    EXPECT_STREQ(out[1].peer, "beta");
    EXPECT_EQ(out[1].payload_len, sizeof(second_payload));
    EXPECT_EQ(out[1].sample_len, sizeof(second_payload));
    EXPECT_EQ(out[1].path_len, 2u);
    EXPECT_EQ(std::memcmp(out[1].sample, second_payload, sizeof(second_payload)), 0);
}

TEST_F(MeshDiagnosticsTest, ExportHonorsMaxWithoutDrainingRing) {
    record_event(DiagnosticEventType::RawRx, 1);
    record_event(DiagnosticEventType::RawData, 2);
    record_event(DiagnosticEventType::ControlData, 3);

    MeshDiagnosticEvent out[2] = {};
    ASSERT_EQ(slopos::mesh::diagnostics_export(out, 2), 2);

    EXPECT_EQ(out[0].timestamp, 1u);
    EXPECT_EQ(out[1].timestamp, 2u);
    EXPECT_EQ(slopos::mesh::diagnostics_count(), 3);
}

TEST_F(MeshDiagnosticsTest, SampleIsTruncatedButOriginalPayloadLengthIsPreserved) {
    uint8_t payload[DIAGNOSTIC_SAMPLE_MAX + 5] = {};
    for (uint8_t i = 0; i < sizeof(payload); ++i) {
        payload[i] = static_cast<uint8_t>(i + 1);
    }

    record_event(DiagnosticEventType::RawData, 10, "payload-node", payload, sizeof(payload), 0);

    MeshDiagnosticEvent out[1] = {};
    ASSERT_EQ(slopos::mesh::diagnostics_export(out, 1), 1);
    EXPECT_EQ(out[0].payload_len, sizeof(payload));
    EXPECT_EQ(out[0].sample_len, DIAGNOSTIC_SAMPLE_MAX);
    EXPECT_EQ(std::memcmp(out[0].sample, payload, DIAGNOSTIC_SAMPLE_MAX), 0);
}

TEST_F(MeshDiagnosticsTest, NullPayloadRecordsZeroLengthSample) {
    record_event(DiagnosticEventType::RawData, 10, "payload-node", nullptr, 12, 0);

    MeshDiagnosticEvent out[1] = {};
    ASSERT_EQ(slopos::mesh::diagnostics_export(out, 1), 1);
    EXPECT_EQ(out[0].payload_len, 12u);
    EXPECT_EQ(out[0].sample_len, 0u);
}

TEST_F(MeshDiagnosticsTest, PeerIsTruncatedAndNullTerminated) {
    const char* long_peer = "peer-name-that-is-longer-than-thirty-one-chars";
    record_event(DiagnosticEventType::PeerPath, 20, long_peer);

    MeshDiagnosticEvent out[1] = {};
    ASSERT_EQ(slopos::mesh::diagnostics_export(out, 1), 1);

    EXPECT_EQ(std::strlen(out[0].peer), sizeof(out[0].peer) - 1);
    EXPECT_EQ(std::strncmp(out[0].peer, long_peer, sizeof(out[0].peer) - 1), 0);
    EXPECT_EQ(out[0].peer[sizeof(out[0].peer) - 1], '\0');
}

TEST_F(MeshDiagnosticsTest, NullPeerRecordsEmptyString) {
    record_event(DiagnosticEventType::PeerPath, 21, nullptr);

    MeshDiagnosticEvent out[1] = {};
    ASSERT_EQ(slopos::mesh::diagnostics_export(out, 1), 1);
    EXPECT_STREQ(out[0].peer, "");
}

TEST_F(MeshDiagnosticsTest, RingWrapDropsOldestEvents) {
    for (int i = 0; i < DIAGNOSTIC_RING_MAX + 5; ++i) {
        record_event(DiagnosticEventType::FloodPath, static_cast<uint32_t>(1000 + i));
    }

    MeshDiagnosticEvent out[DIAGNOSTIC_RING_MAX] = {};
    ASSERT_EQ(slopos::mesh::diagnostics_count(), DIAGNOSTIC_RING_MAX);
    ASSERT_EQ(slopos::mesh::diagnostics_export(out, DIAGNOSTIC_RING_MAX), DIAGNOSTIC_RING_MAX);

    EXPECT_EQ(out[0].timestamp, 1005u);
    EXPECT_EQ(out[DIAGNOSTIC_RING_MAX - 1].timestamp, 1000u + DIAGNOSTIC_RING_MAX + 4u);
    for (int i = 0; i < DIAGNOSTIC_RING_MAX; ++i) {
        EXPECT_EQ(out[i].timestamp, 1005u + static_cast<uint32_t>(i));
    }
}

TEST_F(MeshDiagnosticsTest, ClearRemovesRecordedEvents) {
    record_event(DiagnosticEventType::RawRx, 1);
    record_event(DiagnosticEventType::RawData, 2);
    ASSERT_EQ(slopos::mesh::diagnostics_count(), 2);

    slopos::mesh::diagnostics_clear();

    MeshDiagnosticEvent out[2] = {};
    EXPECT_EQ(slopos::mesh::diagnostics_count(), 0);
    EXPECT_EQ(slopos::mesh::diagnostics_export(out, 2), 0);
}

TEST_F(MeshDiagnosticsTest, TypeNamesMatchPublicEventTypes) {
    EXPECT_STREQ(slopos::mesh::diagnostic_event_type_name(DiagnosticEventType::RawRx), "RawRx");
    EXPECT_STREQ(slopos::mesh::diagnostic_event_type_name(DiagnosticEventType::RawData), "RawData");
    EXPECT_STREQ(slopos::mesh::diagnostic_event_type_name(DiagnosticEventType::ControlData), "ControlData");
    EXPECT_STREQ(slopos::mesh::diagnostic_event_type_name(DiagnosticEventType::PeerPath), "PeerPath");
    EXPECT_STREQ(slopos::mesh::diagnostic_event_type_name(DiagnosticEventType::FloodPath), "FloodPath");
}

TEST_F(MeshDiagnosticsTest, UnknownTypeNameIsStable) {
    const auto unknown = static_cast<DiagnosticEventType>(255);
    EXPECT_STREQ(slopos::mesh::diagnostic_event_type_name(unknown), "Unknown");
}

} // anonymous namespace
