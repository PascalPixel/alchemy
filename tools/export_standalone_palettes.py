#!/usr/bin/env python3
"""Export standalone 224-color palette resources not tied to a map family.

Several LZ-compressed resources decode to exactly 0x1c0 bytes, a 224-entry
BGR555 palette, and are not claimed by any map series. Each is exported with the
shared palette encoder (a 224-color PNG plus the exact token-trace plan) so the
manifest reproduces the compressed ROM stream byte-for-byte.
"""
import argparse
from pathlib import Path

from export_map_charblock_series import ROM_BASE, export_palette, pointer

ROOT = Path(__file__).resolve().parents[1]
# Resources whose LZ stream decodes to a 0x1c0 palette and round-trips exactly.
RESOURCES = (0x1e6, 0x1f4, 0x1f7, 0x28d, 0x2b4,
             0x2d5, 0x308, 0x31d, 0x321, 0x332)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    for resource in RESOURCES:
        directory = ROOT / f"assets/graphics/resource_{resource:x}"
        directory.mkdir(parents=True, exist_ok=True)
        export_palette(rom, resource, directory)
        address = pointer(rom, resource)
        print(f"res 0x{resource:x} @ 0x{address:08x}")
    print(f"palettes={len(RESOURCES)}")


if __name__ == "__main__":
    main()
