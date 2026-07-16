#!/usr/bin/env python3
"""Render Resource 19 from its tracked palette, tiles, and tilemap sources."""
import argparse
import struct
import zlib
from pathlib import Path

from export_asset import chunk
from import_asset import gba_graphics
from tilemap import import_tilemap


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "assets/graphics/resource_19"


def bgr555(data):
    colors = []
    for offset in range(0, len(data), 2):
        value = struct.unpack_from("<H", data, offset)[0]
        channels = (value & 31, value >> 5 & 31, value >> 10 & 31)
        colors.append(tuple((channel << 3) | (channel >> 2)
                            for channel in channels))
    return colors


def compose(tilemap, tiles, colors):
    width = height = 256
    pixels = bytearray(width * height * 3)
    for cell in range(32 * 32):
        entry = struct.unpack_from("<H", tilemap, cell * 2)[0]
        tile = entry & 0x3ff
        horizontal = bool(entry & 0x400)
        vertical = bool(entry & 0x800)
        left, top = cell % 32 * 8, cell // 32 * 8
        for y in range(8):
            for x in range(8):
                source_x = 7 - x if horizontal else x
                source_y = 7 - y if vertical else y
                offset = tile * 64 + source_y * 8 + source_x
                if offset < len(tiles):
                    color = colors[tiles[offset]]
                else:
                    color = (255, 64, 0) if (x ^ y) & 1 else (0, 0, 0)
                target = ((top + y) * width + left + x) * 3
                pixels[target:target + 3] = bytes(color)
    return bytes(pixels)


def rgb_png(width, height, pixels):
    if len(pixels) != width * height * 3:
        raise ValueError("RGB pixel count differs from image dimensions")
    rows = b"".join(
        b"\0" + pixels[y * width * 3:(y + 1) * width * 3]
        for y in range(height))
    return (b"\x89PNG\r\n\x1a\n" +
            chunk(b"IHDR", struct.pack(">IIBBBBB",
                                        width, height, 8, 2, 0, 0, 0)) +
            chunk(b"IDAT", zlib.compress(rows, 9)) + chunk(b"IEND", b""))


def render():
    _, palette, _ = gba_graphics((SOURCE / "palette.png").read_bytes(), 8)
    static, _, _ = gba_graphics(
        (SOURCE / "static.8bpp.png").read_bytes(), 8)
    dynamic, _, _ = gba_graphics(
        (SOURCE / "dynamic.8bpp.png").read_bytes(), 8)
    tilemap = import_tilemap((SOURCE / "bg2.tilemap").read_text())
    if (len(palette), len(tilemap), len(static), len(dynamic)) != (
            0x1c0, 0x800, 0x4000, 0x1000):
        raise ValueError("Resource 19 source dimensions changed")
    colors = bgr555(palette) + [(255, 0, 255)] * (256 - len(palette) // 2)
    frames = []
    for frame in range(4):
        tiles = bytearray(static)
        start = frame * 0x400
        tiles[0x100:0x440] = dynamic[start:start + 0x340]
        frames.append(compose(tilemap, tiles, colors))
    width, height = 256 * 4, 256
    montage = bytearray(width * height * 3)
    for frame, source in enumerate(frames):
        for y in range(height):
            source_start = y * 256 * 3
            target = (y * width + frame * 256) * 3
            montage[target:target + 256 * 3] = source[
                source_start:source_start + 256 * 3]
    return rgb_png(width, height, bytes(montage))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output", type=Path, required=True)
    args = parser.parse_args()
    image = render()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(image)
    print("frames=4 width=1024 height=256")


if __name__ == "__main__":
    main()
