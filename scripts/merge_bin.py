#!/usr/bin/env python3
"""Post-build: merge bootloader, partitions, and firmware into one image."""

Import("env")


def merge_bin_action(target, source, env):
    board_config = env.BoardConfig()
    build_dir = env.subst("$BUILD_DIR")
    merged_bin = env.subst("$BUILD_DIR/${PROGNAME}-merged.bin")
    firmware_bin = env.subst("$BUILD_DIR/${PROGNAME}.bin")

    import os as _os

    bootloader = f"{build_dir}/bootloader.bin"
    partitions = f"{build_dir}/partitions.bin"

    for f in [bootloader, partitions, firmware_bin]:
        if not _os.path.isfile(f):
            print(f"MCWIN31: skipping merge - missing: {f}")
            return

    flash_images = [
        *env.Flatten(env.get("FLASH_EXTRA_IMAGES", [])),
        "$ESP32_APP_OFFSET",
        firmware_bin,
    ]

    merge_cmd = " ".join([
        '"$PYTHONEXE"',
        '"$OBJCOPY"',
        "--chip",
        board_config.get("build.mcu", "esp32s3"),
        "merge_bin",
        "-o", merged_bin,
        "--flash_mode",
        board_config.get("build.flash_mode", "dio"),
        "--flash_freq",
        "${__get_board_f_flash(__env__)}",
        "--flash_size",
        board_config.get("upload.flash_size", "16MB"),
        *flash_images,
    ])

    print(f"MCWIN31: merging firmware ({board_config.get('build.mcu', 'esp32s3')})...")
    env.Execute(merge_cmd)

    if _os.path.isfile(merged_bin):
        print(f"MCWIN31: merged -> firmware-merged.bin ({_os.path.getsize(merged_bin):,} bytes)")

    import json as _json
    from datetime import datetime, timezone as _timezone

    proj_dir = env.subst("$PROJECT_DIR")
    web_dir = _os.path.join(proj_dir, "webflasher")
    _os.makedirs(web_dir, exist_ok=True)

    mcu = board_config.get("build.mcu", "esp32s3")
    flash_mode = "keep"
    flash_size = board_config.get("upload.flash_size", "16MB")

    offsets = {
        "bootloader": "0x0000",
        "partitions": "0x8000",
        "boot_app0": "0xe000",
        "firmware": "0x10000",
    }

    boot_app0_src = _os.path.join(_os.path.dirname(build_dir),
                                  ".pio/build/MCWIN31_TDeck/boot_app0.bin")
    for root, dirs, files in _os.walk(_os.path.join(env.subst("$PROJECT_PACKAGES_DIR"),
                                                    "framework-arduinoespressif32")):
        if "boot_app0.bin" in files:
            boot_app0_src = _os.path.join(root, "boot_app0.bin")
            break

    artifacts = {
        "bootloader": bootloader,
        "partitions": partitions,
        "boot_app0": boot_app0_src,
        "firmware": firmware_bin,
        "full": merged_bin,
    }

    pins_h = _os.path.join(proj_dir, "src", "hal", "tdeck_pins.h")
    version = "unknown"
    try:
        with open(pins_h, encoding="utf-8") as f:
            for line in f:
                if ("MCWIN31_VERSION" in line or "SLOPOS_VERSION" in line) and '"' in line:
                    version = line.split('"')[1]
                    break
    except OSError:
        pass

    manifest = {
        "name": "MCWIN31 T-Deck Plus",
        "board": "LilyGo T-Deck Plus",
        "mcu": mcu,
        "firmware_version": version,
        "built_at_utc": datetime.now(_timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "chip_family": "ESP32-S3",
        "flash_mode": flash_mode,
        "flash_size": flash_size,
        "artifacts": {},
        "flash_offsets": offsets,
    }

    for name, src in artifacts.items():
        if _os.path.isfile(src):
            dst_name = f"mcwin31-tdeck-{name}.bin"
            dst = _os.path.join(web_dir, dst_name)
            with open(src, "rb") as fsrc:
                with open(dst, "wb") as fdst:
                    fdst.write(fsrc.read())
            size = _os.path.getsize(dst)
            manifest["artifacts"][name] = {
                "file": dst_name,
                "size": size,
                "offset": offsets.get(name, "0x0"),
            }
            print(f"MCWIN31 webflasher: {dst_name} ({size:,} bytes)")
        else:
            print(f"MCWIN31 webflasher: SKIP {name} - not found")

    manifest_path = _os.path.join(web_dir, "manifest.json")
    with open(manifest_path, "w", encoding="utf-8") as f:
        _json.dump(manifest, f, indent=2)
    print("MCWIN31 webflasher: manifest written")


env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", merge_bin_action)
