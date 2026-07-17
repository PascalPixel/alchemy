#!/usr/bin/env python3
"""Export the palette and three VRAM charblocks of each traced map family."""
import argparse
import json
import struct
from pathlib import Path

from export_asset import palette_png
from extract_resource import decode_general_trace, encode_general
from kind2_resource import export_resource


ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
TABLE = 0x08320000 - ROM_BASE
BASES = (0x12e, 0x134, 0x13a, 0x140, 0x146, 0x14c, 0x152,
         0x158, 0x15e, 0x164, 0x169, 0x170, 0x176, 0x17c)


def pointer(rom, resource):
    return struct.unpack_from("<I", rom, TABLE + resource * 4)[0]


def span(rom, resource):
    start = pointer(rom, resource) - ROM_BASE
    end = pointer(rom, resource + 1) - ROM_BASE
    if not 0 <= start < end <= len(rom):
        raise ValueError(f"resource {resource:x} has an invalid range")
    return start, end


def export_palette(rom, resource, directory):
    start, end = span(rom, resource)
    decoded, _, tokens = decode_general_trace(rom, start, end, 0x1c0)
    if len(decoded) != 0x1c0:
        raise ValueError(f"resource {resource:x} is not a 224-color palette")
    encoded = encode_general(decoded, tokens)
    original = rom[start:end]
    if not original.startswith(encoded):
        raise ValueError(f"resource {resource:x} token replay differs")
    plan = {"format": 1, "codec": "golden-sun-general-lz",
            "decoded_size": len(decoded), "encoded_size": len(original),
            "tokens": tokens, "lookahead": original[len(encoded):].hex()}
    (directory / "palette.lz.json").write_text(
        json.dumps(plan, separators=(",", ":")) + "\n")
    image, _ = palette_png(decoded)
    (directory / "palette.224.png").write_bytes(image)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    count = 0
    for base in BASES:
        directory = ROOT / f"assets/graphics/resource_{base:x}"
        directory.mkdir(parents=True, exist_ok=True)
        export_palette(rom, base + 1, directory)
        for bank in range(3):
            resource = base + 2 + bank
            start, end = span(rom, resource)
            decoded, _, _ = export_resource(
                rom[start:end], directory / f"charblock{bank + 1}.4bpp.png",
                directory / f"charblock{bank + 1}.kind2.json", 4, 16)
            if decoded != 0x4000:
                raise ValueError(f"resource {resource:x} is not one charblock")
        count += 1
    print(f"families={count} assets={count * 4}")


if __name__ == "__main__":
    main()
