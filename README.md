# MCWIN31 T-Deck Plus

MCWIN31 is custom firmware for the LilyGO T-Deck Plus with a Windows 3.1-inspired interface on top of MeshCore. It is derived from the GPL-3.0 [SlopOS T-Deck](https://github.com/hermes-gadget/SlopOS-tdeck) source base and uses [MC Term](https://github.com/dabeani/meshcoreterm) as a feature-parity reference.

The goal is a practical handheld mesh terminal: Program Manager-style home screen, gray beveled controls, blue title bars, compact modal dialogs, keyboard/trackball/touch input, and MeshCore messaging features that remain interoperable with existing MeshCore nodes.

## Status

| Area | Status |
|------|--------|
| Public fork under `n30nex/MCWIN31` | Done |
| PlatformIO firmware build env | `MCWIN31_TDeck` |
| Windows 3.1 theme primitives | Initial |
| Program Manager home screen | Initial |
| US/CA 902-928 MHz profile | Initial |
| TX gating until Radio Setup is saved | Initial |
| SlopOS feature preservation | In progress |
| MC Term feature parity | Tracked in `docs/FEATURE_PARITY.md` |

## Build

```powershell
pio run -e MCWIN31_TDeck
```

Compatibility aliases remain while the fork still carries upstream internals:

```powershell
pio run -e SlopOS_TDeck
```

## Test

```powershell
pio test -e native_test -v
```

## Flash And Monitor

Connect the T-Deck Plus over USB, confirm the port, then upload:

```powershell
pio device list
pio run -e MCWIN31_TDeck -t upload
pio device monitor -b 115200
```

On first boot, MCWIN31 uses the US/CA receive/UI radio profile but does not transmit until Radio Setup is saved on-device. Confirm local rules before enabling TX.

## Roadmap

The implementation roadmap and feature checklist live in:

- `docs/ROADMAP.md`
- `docs/FEATURE_PARITY.md`

## Hardware

| Component | Detail |
|-----------|--------|
| MCU | ESP32-S3, 16 MB flash, PSRAM |
| Display | ST7789 320x240 TFT |
| Touch | GT911 capacitive touch |
| Keyboard | T-Deck physical keyboard |
| Navigation | Trackball/button |
| LoRa | SX1262 |
| GPS | Serial1, 38400 baud |
| Storage | microSD over SPI |

## Attribution And License

MCWIN31 is GPL-3.0-or-later because it is derived from SlopOS T-Deck. MeshCore and other dependencies retain their original licenses. See `LICENSE` and upstream acknowledgments in the fork history.
