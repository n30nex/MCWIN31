#pragma once

// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstddef>
#include <cstdint>
#include <lvgl.h>

namespace slopos::ui::chrome {

struct StatusBarParts {
    lv_obj_t* bar;
    lv_obj_t* device;
    lv_obj_t* signal;
    lv_obj_t* battery;
};

lv_obj_flag_t no_scroll_flags();
void disable_scroll(lv_obj_t* obj);

void build_channel_string(char* buf, size_t sz);
void format_time(char* buf, size_t sz, uint32_t epoch);
void format_current_time(char* buf, size_t sz);
const char* signal_meter(int rssi);

lv_obj_t* create_title_bar(lv_obj_t* parent, int height, const char* title);
lv_obj_t* create_title_button(lv_obj_t* parent, const char* text, int w, int h,
                              lv_event_cb_t cb, bool enabled = true);
lv_obj_t* create_divider(lv_obj_t* parent, int y, int height = 1);
StatusBarParts create_status_bar(lv_obj_t* parent, int display_h, int bar_h, int divider_h);
void update_battery_label(lv_obj_t* label, int pct);

} // namespace slopos::ui::chrome
