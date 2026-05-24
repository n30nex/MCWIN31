#pragma once

// SPDX-License-Identifier: GPL-3.0-or-later

#include "hal/prefs.h"
#include <cstdint>
#include <cstring>

namespace slopos::radio {

struct RadioProfile {
    const char* id;
    const char* name;
    const char* legal_note;
    float freq_mhz;
    float bandwidth_khz;
    uint8_t spreading_factor;
    uint8_t coding_rate;
    int8_t tx_power_dbm;
};

inline const RadioProfile& default_profile()
{
    static const RadioProfile profile = {
        "us-ca-902-928",
        "US/CA 902-928 MHz",
        "Confirm FCC/ISED Part 15/RSS-247 compliance before transmitting.",
        915.000f,
        125.0f,
        8,
        5,
        20,
    };
    return profile;
}

inline void apply_profile(NodePrefs& prefs, const RadioProfile& profile, const char* node_name)
{
    prefs.set_defaults();
    prefs.freq = profile.freq_mhz;
    prefs.bw = profile.bandwidth_khz;
    prefs.sf = profile.spreading_factor;
    prefs.cr = profile.coding_rate;
    prefs.tx_power_dbm = profile.tx_power_dbm;
    prefs.configured = true;
    prefs.kbd_backlight = 127;
    std::strncpy(prefs.node_name, node_name, sizeof(prefs.node_name) - 1);
    prefs.node_name[sizeof(prefs.node_name) - 1] = '\0';
}

} // namespace slopos::radio
