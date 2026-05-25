// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <cstring>

#include "hal/clipboard.h"

namespace {

TEST(ClipboardTest, NullSourceProducesEmptyString) {
    char out[16] = "unchanged";

    size_t n = slopos::clipboard_prepare_text(out, sizeof(out), nullptr);

    EXPECT_EQ(n, 0u);
    EXPECT_STREQ(out, "");
}

TEST(ClipboardTest, TrimsOuterWhitespace) {
    char out[32];

    size_t n = slopos::clipboard_prepare_text(out, sizeof(out), "  hello mesh  \r\n");

    EXPECT_EQ(n, strlen("hello mesh"));
    EXPECT_STREQ(out, "hello mesh");
}

TEST(ClipboardTest, ReplacesLineBreaksAndTabsWithSpaces) {
    char out[32];

    slopos::clipboard_prepare_text(out, sizeof(out), "alpha\tbeta\ncharlie");

    EXPECT_STREQ(out, "alpha beta charlie");
}

TEST(ClipboardTest, TruncatesAndTerminates) {
    char out[8];

    size_t n = slopos::clipboard_prepare_text(out, sizeof(out), "1234567890");

    EXPECT_EQ(n, 7u);
    EXPECT_STREQ(out, "1234567");
}

} // namespace
