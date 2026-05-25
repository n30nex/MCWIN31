// SPDX-License-Identifier: GPL-3.0-or-later

#include "clipboard.h"

#include <Preferences.h>

namespace slopos {

static constexpr const char* CLIPBOARD_NVS_NS = "mcwin31";
static constexpr const char* CLIPBOARD_NVS_KEY = "clip";

bool clipboard_load(char* out, size_t out_sz)
{
    if (!out || out_sz == 0) return false;
    out[0] = '\0';

    Preferences nvs;
    if (!nvs.begin(CLIPBOARD_NVS_NS, true)) return false;
    nvs.getString(CLIPBOARD_NVS_KEY, out, out_sz);
    nvs.end();
    return true;
}

bool clipboard_save(const char* text)
{
    char prepared[CLIPBOARD_TEXT_MAX];
    clipboard_prepare_text(prepared, sizeof(prepared), text);

    Preferences nvs;
    if (!nvs.begin(CLIPBOARD_NVS_NS, false)) return false;
    size_t written = nvs.putString(CLIPBOARD_NVS_KEY, prepared);
    nvs.end();
    return written == strlen(prepared);
}

bool clipboard_clear()
{
    return clipboard_save("");
}

} // namespace slopos
