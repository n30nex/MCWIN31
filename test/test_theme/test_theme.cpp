// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 Ben

#include <gtest/gtest.h>
#include <cstdint>

#include "ui/theme.h"

namespace {

class ThemeTest : public ::testing::Test {};

TEST_F(ThemeTest, Win31CorePaletteMatchesExpectedColors) {
    using namespace slopos::theme;
    EXPECT_EQ(WIN31_DESKTOP, 0x008080U);
    EXPECT_EQ(WIN31_FACE, 0xc0c0c0U);
    EXPECT_EQ(WIN31_HIGHLIGHT, 0xffffffU);
    EXPECT_EQ(WIN31_SHADOW, 0x808080U);
    EXPECT_EQ(WIN31_DARK_SHADOW, 0x000000U);
    EXPECT_EQ(WIN31_TITLE, 0x000080U);
}

TEST_F(ThemeTest, PublicColorAliasesUseWin31Roles) {
    using namespace slopos::theme;
    EXPECT_EQ(BG_PRIMARY, WIN31_DESKTOP);
    EXPECT_EQ(BG_SECONDARY, WIN31_TITLE);
    EXPECT_EQ(BG_TERTIARY, WIN31_FACE);
    EXPECT_EQ(BG_INPUT, WIN31_HIGHLIGHT);
    EXPECT_EQ(ACCENT, WIN31_FACE);
    EXPECT_EQ(DIVIDER, WIN31_DARK_SHADOW);
}

TEST_F(ThemeTest, TextColorsAreReadableOnWin31Controls) {
    using namespace slopos::theme;
    auto brightness = [](uint32_t c) {
        return ((c >> 16) & 0xFF) + ((c >> 8) & 0xFF) + (c & 0xFF);
    };
    EXPECT_LT(brightness(TEXT_PRIMARY), brightness(WIN31_FACE));
    EXPECT_LT(brightness(TEXT_SECONDARY), brightness(WIN31_FACE));
    EXPECT_GT(brightness(WIN31_HIGHLIGHT), brightness(WIN31_TITLE));
}

TEST_F(ThemeTest, AlertAndStateColorsAreDistinct) {
    using namespace slopos::theme;
    EXPECT_NE(ACCENT, ACCENT_GREEN);
    EXPECT_NE(ACCENT, ACCENT_RED);
    EXPECT_NE(ACCENT_GREEN, ACCENT_RED);
    EXPECT_NE(ACCENT_YELLOW, WIN31_FACE);
    EXPECT_NE(WIN31_TITLE, WIN31_TITLE_INACTIVE);
}

TEST_F(ThemeTest, BorderAndSelectionConstantsAreValid) {
    using namespace slopos::theme;
    EXPECT_EQ(PIXEL_BORDER, 2);
    EXPECT_NE(WIN31_SELECTION, WIN31_FACE);
    EXPECT_NE(WIN31_DARK_SHADOW, WIN31_HIGHLIGHT);
}

} // anonymous namespace
