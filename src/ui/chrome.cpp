// SPDX-License-Identifier: GPL-3.0-or-later

#include "chrome.h"
#include "theme.h"
#include "../hal/battery.h"
#include "../hal/prefs.h"
#include "../mesh/mesh_wrapper.h"
#include "../mesh/radio_profile.h"
#include <cstdio>
#include <cstring>

namespace slopos::ui::chrome {

using namespace theme;

lv_obj_flag_t no_scroll_flags()
{
    return (lv_obj_flag_t)(
        LV_OBJ_FLAG_SCROLLABLE |
        LV_OBJ_FLAG_SCROLL_ELASTIC |
        LV_OBJ_FLAG_SCROLL_MOMENTUM |
        LV_OBJ_FLAG_SCROLL_CHAIN |
        LV_OBJ_FLAG_SCROLL_ON_FOCUS |
        LV_OBJ_FLAG_SCROLL_WITH_ARROW);
}

void disable_scroll(lv_obj_t* obj)
{
    if (!obj) return;
    lv_obj_remove_flag(obj, no_scroll_flags());
    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
}

void build_channel_string(char* buf, size_t sz)
{
    if (!buf || sz == 0) return;
    buf[0] = '\0';

    char names[8][32];
    int n = slopos::mesh::exportChannels(names, 8);
    if (n <= 0) {
        std::strncpy(buf, "no channels", sz - 1);
        buf[sz - 1] = '\0';
        return;
    }

    int pos = 0;
    for (int i = 0; i < n && pos < (int)sz - 20; i++) {
        const char* nm = names[i];
        int wrote = std::snprintf(buf + pos, sz - pos, "%s%s*  ",
                                  nm[0] == '#' ? "" : "#", nm);
        if (wrote <= 0) break;
        pos += wrote;
    }
    if (pos >= (int)sz) pos = (int)sz - 1;
    buf[pos] = '\0';
    while (pos > 0 && buf[pos - 1] == ' ') buf[--pos] = '\0';
}

void format_time(char* buf, size_t sz, uint32_t epoch)
{
    if (!buf || sz == 0) return;
    if (epoch == 0) {
        std::snprintf(buf, sz, "--:--");
        return;
    }
    uint32_t sec = epoch % 86400;
    std::snprintf(buf, sz, "%02lu:%02lu",
                  (unsigned long)((sec / 3600) % 24),
                  (unsigned long)((sec / 60) % 60));
}

void format_current_time(char* buf, size_t sz)
{
    format_time(buf, sz, slopos::mesh::getCurrentTime());
}

void format_transport_label(char* buf, size_t sz)
{
    if (!buf || sz == 0) return;
    const slopos::NodePrefs& p = slopos::prefs_get();
    const auto& profile = slopos::radio::default_profile();
    const float freq = p.configured ? p.freq : profile.freq_mhz;
    std::snprintf(buf, sz, "%s %.0f", p.configured ? "TX" : "RX", freq);
}

void format_duty_label(char* buf, size_t sz)
{
    if (!buf || sz == 0) return;
    const slopos::NodePrefs& p = slopos::prefs_get();
    std::snprintf(buf, sz, "%s", p.configured ? "DC0%" : "DC--");
}

const char* signal_meter(int rssi)
{
    if (rssi > -70)  return "||||";
    if (rssi > -85)  return "||| ";
    if (rssi > -100) return "||  ";
    if (rssi > -115) return "|   ";
    return "    ";
}

lv_obj_t* create_title_bar(lv_obj_t* parent, int height, const char* title)
{
    lv_obj_t* bar = lv_obj_create(parent);
    lv_obj_set_size(bar, LV_PCT(100), height);
    lv_obj_align(bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(bar, lv_color_hex(WIN31_TITLE), 0);
    lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(bar, 0, 0);
    lv_obj_set_style_border_width(bar, 0, 0);
    disable_scroll(bar);

    if (title && title[0]) {
        lv_obj_t* label = lv_label_create(bar);
        lv_label_set_text(label, title);
        lv_obj_set_style_text_color(label, lv_color_hex(WIN31_HIGHLIGHT), 0);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    }
    return bar;
}

lv_obj_t* create_title_button(lv_obj_t* parent, const char* text, int w, int h,
                              lv_event_cb_t cb, bool enabled)
{
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, w, h);
    apply_topbar_icon_btn(btn);
    if (enabled && cb) lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, nullptr);

    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text ? text : "");
    lv_obj_set_style_text_color(label,
        lv_color_hex(enabled ? TEXT_PRIMARY : TEXT_MUTED), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_center(label);
    disable_scroll(label);
    return btn;
}

lv_obj_t* create_dialog_window(lv_obj_t* parent, int w, int h, const char* title)
{
    lv_obj_t* dialog = lv_obj_create(parent);
    lv_obj_set_size(dialog, w, h);
    lv_obj_center(dialog);
    apply_win31_face(dialog);
    lv_obj_set_style_pad_all(dialog, 0, 0);
    disable_scroll(dialog);
    create_title_bar(dialog, 20, title);
    return dialog;
}

lv_obj_t* create_dialog_button(lv_obj_t* parent, const char* text, int w, int h,
                               uint32_t bg_color, uint32_t text_color,
                               lv_event_cb_t cb, void* user_data)
{
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, w, h);
    apply_pixel_btn(btn);
    lv_obj_set_style_bg_color(btn, lv_color_hex(bg_color), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0);
    if (cb) lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, user_data);

    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text ? text : "");
    lv_obj_set_style_text_color(label, lv_color_hex(text_color), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_10, 0);
    lv_obj_center(label);
    disable_scroll(label);
    return btn;
}

lv_obj_t* create_divider(lv_obj_t* parent, int y, int height)
{
    lv_obj_t* div = lv_obj_create(parent);
    lv_obj_set_size(div, LV_PCT(100), height);
    lv_obj_align(div, LV_ALIGN_TOP_MID, 0, y);
    lv_obj_set_style_bg_color(div, lv_color_hex(DIVIDER), 0);
    lv_obj_set_style_bg_opa(div, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(div, 0, 0);
    disable_scroll(div);
    return div;
}

StatusBarParts create_status_bar(lv_obj_t* parent, int display_h, int bar_h, int divider_h)
{
    create_divider(parent, display_h - bar_h - divider_h, divider_h);

    StatusBarParts parts{};
    parts.bar = lv_obj_create(parent);
    lv_obj_set_size(parts.bar, LV_PCT(100), bar_h);
    lv_obj_align(parts.bar, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(parts.bar, lv_color_hex(WIN31_FACE), 0);
    lv_obj_set_style_bg_opa(parts.bar, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(parts.bar, 0, 0);
    lv_obj_set_style_border_width(parts.bar, 0, 0);
    disable_scroll(parts.bar);

    parts.device = lv_label_create(parts.bar);
    lv_label_set_text(parts.device, slopos::mesh::getOwnName());
    lv_label_set_long_mode(parts.device, LV_LABEL_LONG_DOT);
    lv_obj_set_width(parts.device, 76);
    lv_obj_set_style_text_color(parts.device, lv_color_hex(TEXT_PRIMARY), 0);
    lv_obj_set_style_text_font(parts.device, &lv_font_montserrat_10, 0);
    lv_obj_align(parts.device, LV_ALIGN_LEFT_MID, 4, 0);

    parts.transport = lv_label_create(parts.bar);
    update_transport_label(parts.transport);
    lv_obj_set_style_text_font(parts.transport, &lv_font_montserrat_10, 0);
    lv_obj_set_width(parts.transport, 60);
    lv_obj_align(parts.transport, LV_ALIGN_LEFT_MID, 84, 0);

    parts.signal = lv_label_create(parts.bar);
    lv_label_set_text(parts.signal, signal_meter(slopos::mesh::getLastRSSI()));
    lv_obj_set_style_text_color(parts.signal, lv_color_hex(TEXT_PRIMARY), 0);
    lv_obj_set_style_text_font(parts.signal, &lv_font_montserrat_10, 0);
    lv_obj_align(parts.signal, LV_ALIGN_CENTER, 12, 0);

    parts.duty = lv_label_create(parts.bar);
    update_duty_label(parts.duty);
    lv_obj_set_style_text_font(parts.duty, &lv_font_montserrat_10, 0);
    lv_obj_align(parts.duty, LV_ALIGN_RIGHT_MID, -40, 0);

    parts.battery = lv_label_create(parts.bar);
    update_battery_label(parts.battery, slopos_battery_pct());
    lv_obj_set_style_text_font(parts.battery, &lv_font_montserrat_10, 0);
    lv_obj_align(parts.battery, LV_ALIGN_RIGHT_MID, -4, 0);

    return parts;
}

void update_transport_label(lv_obj_t* label)
{
    if (!label) return;
    char buf[12];
    format_transport_label(buf, sizeof(buf));
    lv_label_set_text(label, buf);
    lv_obj_set_style_text_color(label, lv_color_hex(TEXT_PRIMARY), 0);
}

void update_duty_label(lv_obj_t* label)
{
    if (!label) return;
    char buf[8];
    format_duty_label(buf, sizeof(buf));
    lv_label_set_text(label, buf);
    lv_obj_set_style_text_color(label,
        lv_color_hex(slopos::prefs_get().configured ? TEXT_PRIMARY : TEXT_MUTED), 0);
}

void update_battery_label(lv_obj_t* label, int pct)
{
    if (!label) return;
    char buf[8];
    std::snprintf(buf, sizeof(buf), "%d%%", pct);
    lv_label_set_text(label, buf);
    lv_obj_set_style_text_color(label,
        lv_color_hex(pct > 20 ? TEXT_PRIMARY : ACCENT_RED), 0);
}

} // namespace slopos::ui::chrome
