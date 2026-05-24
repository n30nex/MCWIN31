# MCWIN31 Roadmap

## Phase 1 - Foundation

- Public fork: `n30nex/MCWIN31`.
- Build environment: `MCWIN31_TDeck`.
- Keep upstream SlopOS internals for stability while product-facing identity moves to MCWIN31.
- Keep `upstream` remote pointing at `hermes-gadget/SlopOS-tdeck`.
- Remove stale prebuilt SlopOS binaries from the firmware folder.

## Phase 2 - Windows 3.1 Shell

- Replace the dark Discord-like palette with Win3.1-inspired theme primitives.
- Use a Program Manager home screen with grouped app icons.
- Reuse shared title bar, status bar, dialog window, and dialog button chrome.
- Convert settings/date/time/channel modal dialogs to the shared Win31 chrome.
- Show transport/TX gate and duty-cycle state in the shared status bar.
- Drive the Program Manager Messages badge from actual unread chat counts.
- Keep the current screen routing and hardware input behavior.
- Restyle existing screens before changing behavior.

## Phase 3 - Messaging And Mesh Parity

- Preserve SlopOS chat, channel, contact, trace, terminal, map, diagnostics, and advertise screens.
- Fill MC Term gaps for quick replies, message metadata, unread indicators, route hints, telemetry, raw packet logs, ping helpers, repeater/room admin, and settings depth.

## Phase 4 - Radio, Map, GPS, Diagnostics

- Use a US/CA 902-928 MHz default profile.
- Keep transmit blocked until Radio Setup is saved.
- Validate map cache, SD, GPS, battery, keyboard, touch, trackball, and SX1262 behavior on hardware.

## Phase 5 - Release

- Build `firmware.bin`, merged flash image, and web flasher artifacts.
- Capture photos/screenshots of the T-Deck Plus UI.
- Publish a prerelease once build, native tests, upload, and monitor checks pass.
