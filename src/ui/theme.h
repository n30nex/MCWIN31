#pragma once

// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 Ben
//
// MCWIN31 keeps the upstream GPL firmware base and replaces the product UI
// with original Windows 3.1-inspired primitives.

#include <lvgl.h>

namespace slopos::theme {

constexpr uint32_t WIN31_DESKTOP        = 0x008080;
constexpr uint32_t WIN31_FACE           = 0xc0c0c0;
constexpr uint32_t WIN31_HIGHLIGHT      = 0xffffff;
constexpr uint32_t WIN31_SHADOW         = 0x808080;
constexpr uint32_t WIN31_DARK_SHADOW    = 0x000000;
constexpr uint32_t WIN31_TITLE          = 0x000080;
constexpr uint32_t WIN31_TITLE_INACTIVE = 0x808080;
constexpr uint32_t WIN31_SELECTION      = 0x000080;
constexpr uint32_t WIN31_ALERT          = 0xffff00;

constexpr uint32_t BG_PRIMARY   = WIN31_DESKTOP;
constexpr uint32_t BG_SECONDARY = WIN31_TITLE;
constexpr uint32_t BG_TERTIARY  = WIN31_FACE;
constexpr uint32_t BG_INPUT     = WIN31_HIGHLIGHT;

constexpr uint32_t ACCENT        = WIN31_FACE;
constexpr uint32_t ACCENT_HOVER  = WIN31_SHADOW;
constexpr uint32_t ACCENT_GREEN  = 0x008000;
constexpr uint32_t ACCENT_RED    = 0x800000;
constexpr uint32_t ACCENT_ORANGE = 0x808000;
constexpr uint32_t ACCENT_YELLOW = WIN31_ALERT;

constexpr uint32_t MSG_INCOMING = WIN31_HIGHLIGHT;

constexpr uint32_t TEXT_PRIMARY   = 0x000000;
constexpr uint32_t TEXT_SECONDARY = 0x202020;
constexpr uint32_t TEXT_MUTED     = 0x606060;
constexpr uint32_t TEXT_LINK      = 0x0000ff;

constexpr uint32_t CHANNEL_HASH   = WIN31_HIGHLIGHT;
constexpr uint32_t CHANNEL_ACTIVE = WIN31_SELECTION;
constexpr uint32_t DIVIDER        = WIN31_DARK_SHADOW;

constexpr int32_t PIXEL_BORDER = 2;

inline void apply_dark_bg(lv_obj_t* obj) {
    lv_obj_set_style_bg_color(obj, lv_color_hex(BG_PRIMARY), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
}

inline void apply_focus_style(lv_obj_t* obj) {
    lv_obj_set_style_border_color(obj, lv_color_hex(WIN31_ALERT), LV_STATE_FOCUSED);
}

inline void apply_win31_face(lv_obj_t* obj) {
    lv_obj_set_style_bg_color(obj, lv_color_hex(WIN31_FACE), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(obj, 0, 0);
    lv_obj_set_style_border_width(obj, 1, 0);
    lv_obj_set_style_border_color(obj, lv_color_hex(WIN31_DARK_SHADOW), 0);
}

inline void apply_pixel_card(lv_obj_t* obj) {
    apply_win31_face(obj);
    lv_obj_set_style_pad_all(obj, 6, 0);
    apply_focus_style(obj);
}

inline void apply_pixel_card_accent(lv_obj_t* obj) {
    apply_pixel_card(obj);
    lv_obj_set_style_border_width(obj, PIXEL_BORDER, 0);
    lv_obj_set_style_border_color(obj, lv_color_hex(WIN31_SELECTION), 0);
}

inline void apply_pixel_btn(lv_obj_t* obj) {
    apply_win31_face(obj);
    lv_obj_set_style_bg_color(obj, lv_color_hex(WIN31_HIGHLIGHT), LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_STATE_PRESSED);
    lv_obj_set_style_pad_all(obj, 6, 0);
    apply_focus_style(obj);
}

inline void apply_pixel_btn_outline(lv_obj_t* obj) {
    apply_win31_face(obj);
    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(obj, 6, 0);
    apply_focus_style(obj);
}

inline void apply_topbar_icon_btn(lv_obj_t* obj) {
    apply_pixel_btn(obj);
    lv_obj_set_style_pad_all(obj, 0, 0);
}

inline void apply_pixel_input(lv_obj_t* obj) {
    lv_obj_set_style_bg_color(obj, lv_color_hex(BG_INPUT), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(obj, 0, 0);
    lv_obj_set_style_border_width(obj, 1, 0);
    lv_obj_set_style_border_color(obj, lv_color_hex(WIN31_DARK_SHADOW), 0);
    lv_obj_set_style_pad_all(obj, 6, 0);
    apply_focus_style(obj);
}

inline void apply_pixel_badge(lv_obj_t* obj) {
    lv_obj_set_style_bg_color(obj, lv_color_hex(WIN31_ALERT), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(obj, 0, 0);
    lv_obj_set_style_border_width(obj, 1, 0);
    lv_obj_set_style_border_color(obj, lv_color_hex(WIN31_DARK_SHADOW), 0);
    lv_obj_set_style_pad_all(obj, 2, 0);
}

inline void apply_card_style(lv_obj_t* obj) {
    apply_pixel_card(obj);
}

} // namespace slopos::theme
