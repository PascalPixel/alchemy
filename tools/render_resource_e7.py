#!/usr/bin/env python3
"""Render Resource E7's sixteen frames under its five palette banks."""
import argparse
from pathlib import Path

from import_asset import gba_graphics
from render_resource_19 import bgr555, rgb_png


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "assets/graphics/resource_e7"
DEFAULT_OUTPUT = ROOT / "out/previews/resource_e7.frames.png"


def render():
    tiles, _, tile_details = gba_graphics(
        (SOURCE / "frames.4bpp.png").read_bytes(), 4)
    _, palette, palette_details = gba_graphics(
        (SOURCE / "palettes.png").read_bytes(), 8)
    if (len(tiles), len(palette)) != (0x2000, 0xA0):
        raise ValueError("Resource E7 source dimensions changed")
    if tile_details["width"] != 32 or tile_details["height"] != 512:
        raise ValueError("Resource E7 tile order changed")
    if palette_details["palette_entries"] != 80:
        raise ValueError("Resource E7 palette count changed")

    banks = [bgr555(palette[start:start + 0x20])
             for start in range(0, 0xA0, 0x20)]
    indices = [value for byte in tiles for value in (byte & 15, byte >> 4)]
    width, height = 16 * 32, 5 * 32
    pixels = bytearray(width * height * 3)
    for bank, colors in enumerate(banks):
        for frame in range(16):
            for local_tile in range(16):
                tile = frame * 16 + local_tile
                left = frame * 32 + local_tile % 4 * 8
                top = bank * 32 + local_tile // 4 * 8
                for y in range(8):
                    for x in range(8):
                        color = colors[indices[tile * 64 + y * 8 + x]]
                        target = ((top + y) * width + left + x) * 3
                        pixels[target:target + 3] = bytes(color)
    return rgb_png(width, height, bytes(pixels))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args()
    image = render()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(image)
    print("frames=16 palettes=5 width=512 height=160")


if __name__ == "__main__":
    main()
