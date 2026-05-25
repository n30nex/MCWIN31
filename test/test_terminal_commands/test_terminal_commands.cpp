// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <cstring>

#include "ui/terminal_commands.h"

namespace {

using slopos::ui::TerminalCommand;
using slopos::ui::terminal_parse_command;

TEST(TerminalCommandsTest, NullAndWhitespaceAreEmpty) {
    EXPECT_EQ(terminal_parse_command(nullptr).type, TerminalCommand::Empty);
    EXPECT_EQ(terminal_parse_command("   \t ").type, TerminalCommand::Empty);
}

TEST(TerminalCommandsTest, ParsesSimpleCommands) {
    EXPECT_EQ(terminal_parse_command("help").type, TerminalCommand::Help);
    EXPECT_EQ(terminal_parse_command("status").type, TerminalCommand::Status);
    EXPECT_EQ(terminal_parse_command("advert").type, TerminalCommand::Advert);
    EXPECT_EQ(terminal_parse_command("scan").type, TerminalCommand::Scan);
    EXPECT_EQ(terminal_parse_command("copy").type, TerminalCommand::Copy);
    EXPECT_EQ(terminal_parse_command("paste").type, TerminalCommand::Paste);
}

TEST(TerminalCommandsTest, ParsesPingWithoutArgument) {
    auto cmd = terminal_parse_command("ping");

    EXPECT_EQ(cmd.type, TerminalCommand::Ping);
    EXPECT_STREQ(cmd.arg, "");
}

TEST(TerminalCommandsTest, ParsesPingArgumentAndTrimsWhitespace) {
    auto cmd = terminal_parse_command("  ping   Alice Base  \r\n");

    EXPECT_EQ(cmd.type, TerminalCommand::Ping);
    EXPECT_STREQ(cmd.arg, "Alice Base");
}

TEST(TerminalCommandsTest, ParsesNeighborAliases) {
    EXPECT_EQ(terminal_parse_command("neighbors").type, TerminalCommand::Neighbors);
    EXPECT_EQ(terminal_parse_command("neighbor").type, TerminalCommand::Neighbors);
    EXPECT_EQ(terminal_parse_command("neighbours").type, TerminalCommand::Neighbors);
    EXPECT_EQ(terminal_parse_command("neighbour").type, TerminalCommand::Neighbors);
}

TEST(TerminalCommandsTest, ParsesNeighborsScanArgument) {
    auto cmd = terminal_parse_command("neighbors scan");

    EXPECT_EQ(cmd.type, TerminalCommand::Neighbors);
    EXPECT_STREQ(cmd.arg, "scan");
}

TEST(TerminalCommandsTest, ParsesClipboardAliases) {
    EXPECT_EQ(terminal_parse_command("clip").type, TerminalCommand::Clipboard);
    EXPECT_EQ(terminal_parse_command("clipboard").type, TerminalCommand::Clipboard);
}

TEST(TerminalCommandsTest, ParsesCopyArgumentAndTrimsWhitespace) {
    auto cmd = terminal_parse_command("  copy   meet at the repeater  \r\n");

    EXPECT_EQ(cmd.type, TerminalCommand::Copy);
    EXPECT_STREQ(cmd.arg, "meet at the repeater");
}

TEST(TerminalCommandsTest, UnknownCommandPreservesOriginalInput) {
    auto cmd = terminal_parse_command("trace now");

    EXPECT_EQ(cmd.type, TerminalCommand::Unknown);
    EXPECT_STREQ(cmd.arg, "trace now");
}

TEST(TerminalCommandsTest, ArgumentIsTruncatedAndTerminated) {
    auto cmd = terminal_parse_command(
        "ping abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-extra");

    EXPECT_EQ(cmd.type, TerminalCommand::Ping);
    EXPECT_EQ(cmd.arg[sizeof(cmd.arg) - 1], '\0');
    EXPECT_LT(strlen(cmd.arg), sizeof(cmd.arg));
}

TEST(TerminalCommandsTest, CommandMatchingIsExact) {
    EXPECT_EQ(terminal_parse_command("helpful").type, TerminalCommand::Unknown);
    EXPECT_EQ(terminal_parse_command("pingpong").type, TerminalCommand::Unknown);
    EXPECT_EQ(terminal_parse_command("scanner").type, TerminalCommand::Unknown);
    EXPECT_EQ(terminal_parse_command("clipboarder").type, TerminalCommand::Unknown);
}

} // namespace
