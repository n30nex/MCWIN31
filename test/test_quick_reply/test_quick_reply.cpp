// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <cstring>

#include "ui/quick_reply.h"

namespace {

using slopos::ui::QuickReplyContext;
using slopos::ui::quick_reply_expand_template;
using slopos::ui::quick_reply_format_time;

TEST(QuickReplyTest, ExpandsNameChannelAndTime) {
    char out[96];
    QuickReplyContext ctx{"N30", "#general", 13 * 3600 + 7 * 60};

    quick_reply_expand_template("{name} on {channel} at {time}", ctx, out, sizeof(out));

    EXPECT_STREQ(out, "N30 on #general at 13:07");
}

TEST(QuickReplyTest, LeavesUnknownVariablesLiteral) {
    char out[64];
    QuickReplyContext ctx{"N30", "#ops", 0};

    quick_reply_expand_template("Status {unknown} {channel}", ctx, out, sizeof(out));

    EXPECT_STREQ(out, "Status {unknown} #ops");
}

TEST(QuickReplyTest, HandlesMissingContextValues) {
    char out[64];
    QuickReplyContext ctx{nullptr, nullptr, 0};

    quick_reply_expand_template("{name}|{channel}|{time}", ctx, out, sizeof(out));

    EXPECT_STREQ(out, "||--:--");
}

TEST(QuickReplyTest, TruncatesSafely) {
    char out[10];
    QuickReplyContext ctx{"LongNodeName", "#general", 0};

    quick_reply_expand_template("Node {name}", ctx, out, sizeof(out));

    EXPECT_STREQ(out, "Node Long");
    EXPECT_EQ(out[sizeof(out) - 1], '\0');
}

TEST(QuickReplyTest, NullTemplateProducesEmptyString) {
    char out[16] = "unchanged";
    QuickReplyContext ctx{"N30", "#general", 0};

    quick_reply_expand_template(nullptr, ctx, out, sizeof(out));

    EXPECT_STREQ(out, "");
}

TEST(QuickReplyTest, TimeWrapsWithinDay) {
    char out[8];

    quick_reply_format_time(out, sizeof(out), 90000);

    EXPECT_STREQ(out, "01:00");
}

} // namespace
