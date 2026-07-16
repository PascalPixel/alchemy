#!/usr/bin/env python3
"""Export exact, palette-independent GBA tile indices to indexed PNG."""
import argparse
import json
import struct
import zlib
from pathlib import Path

from import_asset import gba_graphics


ROM_BASE = 0x08000000


def chunk(kind, payload):
    body = kind + payload
    return (struct.pack(">I", len(payload)) + body +
            struct.pack(">I", zlib.crc32(body) & 0xffffffff))


def tile_png(raw, bpp, columns):
    unit = 32 if bpp == 4 else 64
    if not raw or len(raw) % unit:
        raise ValueError("tile data must contain whole nonempty tiles")
    count = len(raw) // unit
    if columns <= 0 or count % columns:
        raise ValueError("columns must divide the tile count exactly")
    rows = count // columns
    width, height = columns * 8, rows * 8
    pixels = bytearray(width * height)
    for tile in range(count):
        source = raw[tile * unit:(tile + 1) * unit]
        indices = (
            [value for byte in source for value in (byte & 15, byte >> 4)]
            if bpp == 4 else list(source)
        )
        left = tile % columns * 8
        top = tile // columns * 8
        for y in range(8):
            start = (top + y) * width + left
            pixels[start:start + 8] = bytes(indices[y * 8:y * 8 + 8])
    scanlines = bytearray()
    for y in range(height):
        row = pixels[y * width:(y + 1) * width]
        scanlines.append(0)
        if bpp == 4:
            scanlines.extend(row[index] << 4 | row[index + 1]
                             for index in range(0, width, 2))
        else:
            scanlines.extend(row)
    colors = 16 if bpp == 4 else 256
    palette = bytearray()
    for index in range(colors):
        palette.extend(((index & 31) * 8, (index >> 5) * 8, 0))
    png = (
        b"\x89PNG\r\n\x1a\n" +
        chunk(b"IHDR", struct.pack(">IIBBBBB",
                                    width, height, bpp, 3, 0, 0, 0)) +
        chunk(b"PLTE", bytes(palette)) +
        chunk(b"IDAT", zlib.compress(bytes(scanlines), 9)) +
        chunk(b"IEND", b"")
    )
    return png, {"width": width, "height": height, "bpp": bpp,
                 "tiles": count, "columns": columns}


def self_test():
    for bpp, size in ((4, 32 * 7), (8, 64 * 4)):
        raw = bytes((index * 37 + 11) & 255 for index in range(size))
        png, _ = tile_png(raw, bpp, 7 if bpp == 4 else 4)
        tiles, _, _ = gba_graphics(png, bpp)
        if tiles != raw:
            raise AssertionError(f"{bpp}bpp tile round-trip failed")
    print("self-test=ok")


def number(text):
    try:
        return int(text, 0)
    except ValueError as error:
        raise argparse.ArgumentTypeError(f"invalid integer: {text}") from error


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--self-test", action="store_true")
    commands = parser.add_subparsers(dest="command")
    tiles = commands.add_parser("tiles")
    tiles.add_argument("rom", type=Path)
    tiles.add_argument("--address", type=number, required=True)
    tiles.add_argument("--size", type=number, required=True)
    tiles.add_argument("--bpp", type=int, choices=(4, 8), required=True)
    tiles.add_argument("--columns", type=int, required=True)
    tiles.add_argument("-o", "--output", type=Path, required=True)
    args = parser.parse_args()
    if args.self_test:
        self_test()
        if args.command is None:
            return
    if args.command != "tiles":
        parser.error("an asset command is required")
    rom = args.rom.read_bytes()
    start = args.address - ROM_BASE
    end = start + args.size
    if start < 0 or end > len(rom) or start >= end:
        parser.error("tile range is outside the ROM or empty")
    if args.output.resolve() == args.rom.resolve():
        parser.error("refusing to overwrite the input ROM")
    png, report = tile_png(rom[start:end], args.bpp, args.columns)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(png)
    print(json.dumps(report, sort_keys=True))


if __name__ == "__main__":
    main()
