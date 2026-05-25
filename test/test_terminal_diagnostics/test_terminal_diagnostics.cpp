// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <cstring>

#include "mesh/mesh_diagnostics.h"
#include "ui/terminal_commands.h"
#include "ui/terminal_diagnostics.h"

namespace {

using slopos::mesh::DiagnosticEventType;
using slopos::ui::terminal_describe_diagnostics;
using slopos::ui::terminal_parse_command;

class TerminalDiagnosticsTest : public ::testing::Test {
protected:
    void SetUp() override {
        slopos::mesh::diagnostics_clear();
    }

    void TearDown() override {
        slopos::mesh::diagnostics_clear();
    }
};

void describe(const char* command, char* out, size_t out_sz)
{
    auto parsed = terminal_parse_command(command);
    terminal_describe_diagnostics(parsed, out, out_sz);
}

void record_event(DiagnosticEventType type, uint32_t timestamp, const char* peer = "node-a")
{
    uint8_t payload[] = {
        static_cast<uint8_t>(timestamp & 0xff),
        static_cast<uint8_t>((timestamp + 1) & 0xff),
        static_cast<uint8_t>((timestamp + 2) & 0xff),
    };

    slopos::mesh::diagnostics_record(type, timestamp, -90 + static_cast<int>(timestamp),
                                     2.5f, peer, payload, sizeof(payload), 2);
}

TEST_F(TerminalDiagnosticsTest, LatestReportsEmptyRing) {
    char result[256];
    describe("diag", result, sizeof(result));

    EXPECT_STREQ(result, "Diag: no mesh events yet");
}

TEST_F(TerminalDiagnosticsTest, LatestFormatsEventMetadataAndSample) {
    uint8_t payload[] = {0x01, 0xa0, 0xff};
    slopos::mesh::diagnostics_record(DiagnosticEventType::RawRx, 321, -74, 4.5f,
                                     "alpha", payload, sizeof(payload), 3);

    char result[256];
    describe("diag", result, sizeof(result));

    EXPECT_NE(std::strstr(result, "Diag latest (1 stored): t=321 RawRx"), nullptr);
    EXPECT_NE(std::strstr(result, "len=3 path=3 RSSI:-74 SNR:4.5"), nullptr);
    EXPECT_NE(std::strstr(result, "peer:alpha sample:01 A0 FF"), nullptr);
}

TEST_F(TerminalDiagnosticsTest, LatestAliasUsesLatestFormat) {
    record_event(DiagnosticEventType::ControlData, 7, "beta");

    char result[256];
    describe("diag latest", result, sizeof(result));

    EXPECT_NE(std::strstr(result, "Diag latest (1 stored): t=7 ControlData"), nullptr);
}

TEST_F(TerminalDiagnosticsTest, HistoryShowsLatestFourEventsInOrder) {
    for (uint32_t i = 1; i <= 6; ++i) {
        record_event(DiagnosticEventType::RawData, i, "peer");
    }

    char result[256];
    describe("diag list", result, sizeof(result));

    EXPECT_NE(std::strstr(result, "Diag history (6 stored):"), nullptr);
    EXPECT_EQ(std::strstr(result, "#2:"), nullptr);
    EXPECT_NE(std::strstr(result, "#3:RawData"), nullptr);
    EXPECT_NE(std::strstr(result, "#4:RawData"), nullptr);
    EXPECT_NE(std::strstr(result, "#5:RawData"), nullptr);
    EXPECT_NE(std::strstr(result, "#6:RawData"), nullptr);
}

TEST_F(TerminalDiagnosticsTest, HistoryAliasUsesHistoryFormat) {
    record_event(DiagnosticEventType::PeerPath, 42, "relay");

    char result[256];
    describe("diagnostics history", result, sizeof(result));

    EXPECT_NE(std::strstr(result, "Diag history (1 stored): #1:PeerPath"), nullptr);
}

TEST_F(TerminalDiagnosticsTest, ClearRemovesRecordedEvents) {
    record_event(DiagnosticEventType::FloodPath, 11, "relay");

    char result[256];
    describe("rxlog clear", result, sizeof(result));

    EXPECT_STREQ(result, "Diag cleared");
    EXPECT_EQ(slopos::mesh::diagnostics_count(), 0);

    describe("rxlog", result, sizeof(result));
    EXPECT_STREQ(result, "Diag: no mesh events yet");
}

TEST_F(TerminalDiagnosticsTest, UnknownArgumentReturnsUsage) {
    char result[256];
    describe("diag verbose", result, sizeof(result));

    EXPECT_STREQ(result, "Diag usage: diag [latest|list|history|clear]");
}

} // namespace
