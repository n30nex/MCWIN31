# Feature Parity Checklist

Status values: `Done`, `Partial`, `Todo`, `Deferred`.

## SlopOS Baseline

| Feature | Status | Notes |
|---------|--------|-------|
| PlatformIO ESP32-S3 build | Done | `MCWIN31_TDeck` env added; SlopOS aliases retained. |
| MeshCore protocol integration | Partial | Preserved from SlopOS; TX now gated until Radio Setup is saved. |
| T-Deck HAL | Partial | Display, touch, keyboard, trackball, battery, GPS, SD preserved. |
| Home screen | Partial | Program Manager-style shell added. |
| Chat screen | Partial | Existing behavior retained; theme pass still broad. |
| Contacts and heard lists | Partial | Existing behavior retained. |
| Channels | Partial | Existing behavior retained. |
| Map | Partial | Existing renderer retained. |
| Settings and radio setup | Partial | US/CA profile and save-to-enable-TX flow started. |
| Terminal and trace | Partial | Existing behavior retained. |
| Signal/noise diagnostics | Partial | Existing behavior retained. |
| Native unit tests | Partial | Theme and build tests updated; full suite pending verification. |

## MC Term Reference

| Feature | Status | Notes |
|---------|--------|-------|
| Contacts / Channels / Map / Mgmt navigation model | Todo | Home groups started; deeper tab parity pending. |
| Status bar device, transport, battery/duty cycle | Todo | Current bars show device, signal, battery, time only. |
| Unread channel and DM indicators | Todo | Home badge placeholder exists. |
| DM/channel message metadata | Todo | Path, hops, RSSI, SNR, repeater hints pending. |
| Quick replies with variables | Todo | Not implemented yet. |
| Telemetry request/history | Todo | Not implemented yet. |
| Raw RX log and parsed packet detail | Todo | Not implemented yet. |
| Ping and neighbor scan helpers | Todo | Trace path exists; ping/scan parity pending. |
| Wi-Fi and BLE settings | Todo | Not implemented yet. |
| GPS distance tracking with advert flooding | Todo | GPS parser preserved; tracking flow pending. |
| Map tile cache behavior | Partial | SlopOS offline renderer retained; MC Term parity pending. |
| Clipboard persistence | Todo | Not implemented yet. |
| Room/repeater admin flows | Todo | Not implemented yet. |
| Embedded web UI | Deferred | Feasibility depends on flash/RAM budget after core parity. |
