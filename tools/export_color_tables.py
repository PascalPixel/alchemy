#!/usr/bin/env python3
"""Claim raw BGR555 colour-gradient tables among the pre-map resources.

A block of resources in the 0x46-0xcb range decode with NO codec (they fail
every LZ variant at offset 0) because they are not compressed at all: each is a
raw array of 66 little-endian u16 BGR555 colours with bit 15 clear on every
entry (a fade / lighting / psynergy colour ramp the engine indexes directly).
The give-away is decisive: 66 consecutive values all bit-15-clear happens by
chance with probability 2**-66. Each is stored as a 66x1 RGBA strip (BGR555 ->
multiples-of-eight RGB, bit 15 -> alpha per the gba-palette-rgba contract) so
the build reproduces the exact ROM bytes, and it renders as the real gradient.
"""
import argparse
import json
import struct
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
FIRST, LAST = 0x19, 0x3cd


def number(value):
    return int(value, 0) if isinstance(value, str) else int(value)


def pointer(rom, resource):
    off = 0x320000 + resource * 4
    return int.from_bytes(rom[off:off + 4], "little")


def claimed_mask(rom):
    mask = bytearray(len(rom))
    for name in ("out/claimed/manifest.json", "out/asm/manifest.json",
                 "out/assets/manifest.json", "assets/manifest.json"):
        path = ROOT / name
        if not path.exists():
            continue
        data = json.loads(path.read_text())
        for region in data.get("regions", []):
            start = number(region["address"]) - ROM_BASE
            for index in range(start, start + number(region["size"])):
                if 0 <= index < len(mask):
                    mask[index] = 1
        for series in data.get("series", []):
            if series.get("kind") == "golden-sun-color-table-series":
                continue  # do not count our own prior claims as blocking
            _mark_series(series, mask)
    return mask


def _mark_series(obj, mask):
    if isinstance(obj, dict):
        if "address" in obj and "size" in obj:
            start = number(obj["address"]) - ROM_BASE
            for index in range(start, start + number(obj["size"])):
                if 0 <= index < len(mask):
                    mask[index] = 1
        for value in obj.values():
            _mark_series(value, mask)
    elif isinstance(obj, list):
        for value in obj:
            _mark_series(value, mask)


def is_color_table(data):
    if len(data) < 32 or len(data) % 2:
        return False
    u16 = [int.from_bytes(data[i:i + 2], "little")
           for i in range(0, len(data), 2)]
    if any(value & 0x8000 for value in u16):
        return False
    return len(set(u16)) >= 8


def png_chunk(kind, payload):
    body = kind + payload
    return struct.pack(">I", len(payload)) + body + struct.pack(
        ">I", zlib.crc32(body) & 0xFFFFFFFF)


def write_rgba_strip(path, colors):
    width, height = len(colors), 1
    header = struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0)
    raw = bytearray()
    raw.append(0)  # filter: none
    for value in colors:
        red = (value & 0x1F) << 3
        green = ((value >> 5) & 0x1F) << 3
        blue = ((value >> 10) & 0x1F) << 3
        alpha = 255 - ((value >> 15) & 1)
        raw.extend((red, green, blue, alpha))
    png = (b"\x89PNG\r\n\x1a\n" + png_chunk(b"IHDR", header) +
           png_chunk(b"IDAT", zlib.compress(bytes(raw), 9)) +
           png_chunk(b"IEND", b""))
    path.write_bytes(png)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    mask = claimed_mask(rom)

    resources = []
    for resource in range(FIRST, LAST + 1):
        start = pointer(rom, resource) - ROM_BASE
        end = pointer(rom, resource + 1) - ROM_BASE
        if not 0 <= start < end <= len(rom) or mask[start]:
            continue
        data = rom[start:end]
        if not is_color_table(data):
            continue
        colors = [int.from_bytes(data[i:i + 2], "little")
                  for i in range(0, len(data), 2)]
        directory = ROOT / f"assets/graphics/resource_{resource:x}"
        directory.mkdir(parents=True, exist_ok=True)
        write_rgba_strip(directory / "color_table.rgba.png", colors)
        resources.append({
            "id": f"{resource:x}",
            "address": f"0x{start + ROM_BASE:08x}",
            "size": f"0x{end - start:x}",
        })

    manifest_path = ROOT / "assets/manifest.json"
    manifest = json.loads(manifest_path.read_text())
    series = [entry for entry in manifest.get("series", [])
              if entry.get("kind") != "golden-sun-color-table-series"]
    series.append({
        "kind": "golden-sun-color-table-series",
        "resources": resources,
    })
    manifest["series"] = series
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"color tables={len(resources)}")


if __name__ == "__main__":
    main()
