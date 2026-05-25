# MCWIN31 T-Deck Plus Test Suite

## Running Tests

```bash
# Run all 213 currently registered native tests on native platform (no hardware needed)
pio test -e native_test -v

# Run specific test file
pio test -e native_test -f test_battery -v

# Run with verbose output
pio test -e native_test -vv
```

## Test Structure

```text
test/
|-- mocks/                    # Mock headers for native testing
|   |-- Arduino.h             # Mock Arduino core
|   |-- lvgl.h                # Mock LVGL v9 API
|   |-- RadioLib.h            # Mock SX1262 radio driver
|   |-- LovyanGFX.hpp         # Mock LovyanGFX display driver
|   |-- MeshCore.h            # Mock MeshCore base classes
|   `-- mesh_helpers.h        # Mock AutoDiscoverRTCClock, StdRNG
|-- test_battery/             # Battery mV to percent conversion
|-- test_build/               # Headers compile together
|-- test_clipboard/           # Clipboard normalization and truncation
|-- test_gps/                 # NMEA parsing and fix detection
|-- test_keyboard/            # Matrix scan, keymap, debounce
|-- test_map/                 # Tile math and zoom levels
|-- test_mesh_messaging/      # Message queue and channel ops
|-- test_mesh_diagnostics/    # Diagnostics ring capture and event summaries
|-- test_mesh_wrapper/        # Mesh API contract
|-- test_navigation/          # Screen routing and back nav
|-- test_pins/                # Pin conflicts and bus consistency
|-- test_quick_reply/         # Quick-reply template expansion
|-- test_sdcard/              # SPI init, mount, read/write
|-- test_terminal_commands/   # Terminal command parsing, including diagnostics controls
|-- test_terminal_diagnostics/# Terminal diagnostics output formatting
|-- test_theme/               # Color constants and readability
|-- test_trackball/           # Direction/click events
`-- test_touch/               # GT911 coordinate mapping
```

## What's Tested

| Module | Tests | Coverage |
|--------|-------|----------|
| Touch (GT911) | 22 | Coordinate mapping, multitouch, press to release, edge cases |
| Keyboard | 20 | Matrix scan, keymap, debounce, ghost detection, LVGL mapping |
| Battery HAL | 16 | mV to percent, clamping, monotonicity, ADC math, edge cases |
| SD Card | 15 | SPI init, mount, read/write, directory listing, edge cases |
| Mesh messaging | 15 | Message queue, send/receive, channel ops, contact export |
| Mesh diagnostics | 11 | Fixed-size RX/custom/control/path callback ring, truncation, wrap, clear, type names |
| Map renderer | 14 | Tile math, zoom levels, bounding box |
| Mesh wrapper | 16 | API signatures, return ranges, message metadata, unread count init |
| Navigation | 12 | Forward/back, history stack, deep nav, all pairs |
| Terminal commands | 13 | Command parsing, ping arguments, neighbor aliases, scan, diagnostics arguments, and clipboard commands |
| Terminal diagnostics | 7 | Latest event, history/list, clear, usage, metadata, and sample formatting |
| GPS | 12 | NMEA parsing, coordinate conversion, fix detection |
| Pin definitions | 9 | GPIO range, SPI/I2C conflicts, duplicates, LoRa params |
| Trackball | 9 | Direction/click events, deadtime, idle calibration |
| Quick replies | 6 | Template variables, truncation, missing context values |
| Theme constants | 5 | Win31 palette roles, distinctness, readability |
| Clipboard | 4 | Text trimming, control-character cleanup, truncation |
| Build integration | 7 | Header inclusion, API existence, cross-module consistency |

## Adding New Tests

1. Create `test/test_newthing/` with `main.cpp` and `test_newthing.cpp`.
2. Include the header under test plus `gtest/gtest.h`.
3. Use `arduino_mock::reset()` in `SetUp()` for clean state when needed.
4. Follow Arrange, Act, Assert.
5. Use `EXPECT_*` for non-fatal assertions and `ASSERT_*` for fatal assertions.

### Mock Guidelines

- `arduino_mock::current_millis`: control time.
- `arduino_mock::analog_values[pin]`: set ADC values.
- `arduino_mock::pin_states[pin]`: set digital pin states.
- LVGL object creation returns dummy objects and does not render.
