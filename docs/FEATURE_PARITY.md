# Feature Parity Checklist

Status values: `Done`, `Partial`, `Todo`, `Deferred`.

## SlopOS Baseline

| Feature | Status | Notes |
|---------|--------|-------|
| PlatformIO ESP32-S3 build | Done | `MCWIN31_TDeck` env added; SlopOS aliases retained. |
| MeshCore protocol integration | Partial | Preserved from SlopOS; message, advert, and trace TX are gated until Radio Setup is saved. |
| T-Deck HAL | Partial | Display, touch, keyboard, trackball, battery, GPS, SD preserved. |
| Home screen | Partial | Program Manager-style shell added; Messages badge now reflects unread chat count. |
| Chat screen | Partial | Existing behavior retained; add-channel and quick-reply dialogs use shared Win31 chrome; unread total exposed to home. |
| Contacts and heard lists | Partial | Existing behavior retained. |
| Channels | Partial | Existing behavior retained; add-channel dialog uses shared Win31 chrome. |
| Map | Partial | Existing renderer retained. |
| Settings and radio setup | Partial | US/CA profile and save-to-enable-TX flow started; date/time and keyboard backlight dialogs use shared Win31 chrome. |
| Terminal and trace | Partial | Existing behavior retained; `diag`/`diagnostics`/`rxlog` shows the latest mesh diagnostics event, `diag list`/`diag history` shows recent events, and `diag clear` clears the diagnostics ring. |
| Signal/noise diagnostics | Partial | Existing behavior retained. |
| Native unit tests | Done | Last validated `pio test -e native_test -v` pass had 212 succeeded and 1 expected native ESP32 skip. |

## MC Term Reference

| Feature | Status | Notes |
|---------|--------|-------|
| Contacts / Channels / Map / Mgmt navigation model | Todo | Home groups started; deeper tab parity pending. |
| Status bar device, transport, battery/duty cycle | Partial | Shared status bars show device, RX/TX frequency, signal, `DC--`/`DC0%`, and battery. Airtime accounting pending. |
| Unread channel and DM indicators | Partial | Channel list badges existed; Program Manager Messages badge now shows live unread total. DM-specific unread pending. |
| DM/channel message metadata | Partial | Received messages now carry and render RSSI/SNR; path, hops, delivery state, and repeater hints pending. |
| Quick replies with variables | Partial | Canned quick replies expand `{name}`, `{channel}`, and `{time}` into the chat compose field; per-channel preset editing pending. |
| Telemetry request/history | Todo | Not implemented yet. |
| Raw RX log and parsed packet detail | Partial | Fixed-size mesh diagnostics ring captures raw RX, raw custom, control data, and path callbacks; terminal diagnostics supports latest, history/list, and clear flows from `diag`/`diagnostics`/`rxlog`. Parsed packet detail and validation pending. |
| Ping and neighbor scan helpers | Partial | Terminal now supports `neighbors`, `ping <name|#>`, `scan`, and `neighbors scan` using the existing MeshCore trace path; UI scan history/detail pending. |
| Wi-Fi and BLE settings | Todo | Not implemented yet. |
| GPS distance tracking with advert flooding | Todo | GPS parser preserved; tracking flow pending. |
| Map tile cache behavior | Partial | SlopOS offline renderer retained; MC Term parity pending. |
| Clipboard persistence | Partial | Terminal now supports persisted `copy <text>`, `paste`, `clip`, and `clipboard` commands backed by NVS; Settings can clear the clipboard; chat copy/paste controls pending. |
| Room/repeater admin flows | Todo | Not implemented yet. |
| Embedded web UI | Deferred | Feasibility depends on flash/RAM budget after core parity. |
