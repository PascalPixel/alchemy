#!/usr/bin/env python3
"""Export exact, palette-independent GBA tile indices to indexed PNG."""
import argparse
import json
import struct
import zlib
from pathlib import Path

from import_asset import gba_graphics, gba_palette_rgba, indexed_png, rgba_png


ROM_BASE = 0x08000000


def chunk(kind, payload):
    body = kind + payload
    return (struct.pack(">I", len(payload)) + body +
            struct.pack(">I", zlib.crc32(body) & 0xffffffff))


def tile_png(raw, bpp, columns, palette_colors=None):
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
    if palette_colors is None:
        palette_colors = (
            [(index * 16,) * 3 for index in range(16)]
            if bpp == 4 else
            [((index & 31) * 8, (index >> 5) * 8, 0)
             for index in range(256)]
        )
    if not 1 <= len(palette_colors) <= colors:
        raise ValueError("palette does not fit the requested tile depth")
    if max(pixels) >= len(palette_colors):
        raise ValueError("tile pixels reference a missing palette entry")
    palette = bytes(channel for color in palette_colors for channel in color)
    png = (
        b"\x89PNG\r\n\x1a\n" +
        chunk(b"IHDR", struct.pack(">IIBBBBB",
                                    width, height, bpp, 3, 0, 0, 0)) +
        chunk(b"PLTE", palette) +
        chunk(b"IDAT", zlib.compress(bytes(scanlines), 9)) +
        chunk(b"IEND", b"")
    )
    return png, {"width": width, "height": height, "bpp": bpp,
                 "tiles": count, "columns": columns}


def palette_png(raw):
    if not raw or len(raw) % 2 or len(raw) > 512:
        raise ValueError("palette must contain 1..256 BGR555 entries")
    entries = len(raw) // 2
    palette = bytearray()
    for offset in range(0, len(raw), 2):
        value = struct.unpack_from("<H", raw, offset)[0]
        if value & 0x8000:
            raise ValueError("palette contains a non-BGR555 high bit")
        palette.extend(((value & 31) << 3,
                        (value >> 5 & 31) << 3,
                        (value >> 10 & 31) << 3))
    pixels = list(range(entries)) + [0] * (256 - entries)
    scanlines = b"".join(
        b"\0" + bytes(pixels[row * 16:(row + 1) * 16])
        for row in range(16))
    png = (
        b"\x89PNG\r\n\x1a\n" +
        chunk(b"IHDR", struct.pack(">IIBBBBB", 16, 16, 8, 3, 0, 0, 0)) +
        chunk(b"PLTE", bytes(palette)) +
        chunk(b"IDAT", zlib.compress(scanlines, 9)) +
        chunk(b"IEND", b"")
    )
    return png, {"width": 16, "height": 16,
                 "palette_entries": entries}


def byte_png(raw, width):
    if (not raw or width <= 0 or len(raw) % width or width % 8 or
            (len(raw) // width) % 8):
        raise ValueError("byte image dimensions must be nonzero multiples of eight")
    height = len(raw) // width
    palette = bytes(channel for index in range(256)
                    for channel in (index, index, index))
    scanlines = b"".join(
        b"\0" + raw[row * width:(row + 1) * width]
        for row in range(height))
    png = (
        b"\x89PNG\r\n\x1a\n" +
        chunk(b"IHDR", struct.pack(">IIBBBBB",
                                    width, height, 8, 3, 0, 0, 0)) +
        chunk(b"PLTE", palette) +
        chunk(b"IDAT", zlib.compress(scanlines, 9)) +
        chunk(b"IEND", b"")
    )
    return png, {"width": width, "height": height, "bytes": len(raw)}


def rgba_image(raw, width):
    if not raw or len(raw) % 4 or width <= 0:
        raise ValueError("RGBA input must contain whole nonempty pixels")
    pixels = len(raw) // 4
    if pixels % width:
        raise ValueError("width must divide the RGBA pixel count")
    height = pixels // width
    scanlines = b"".join(
        b"\0" + raw[row * width * 4:(row + 1) * width * 4]
        for row in range(height))
    png = (
        b"\x89PNG\r\n\x1a\n" +
        chunk(b"IHDR", struct.pack(">IIBBBBB",
                                    width, height, 8, 6, 0, 0, 0)) +
        chunk(b"IDAT", zlib.compress(scanlines, 9)) +
        chunk(b"IEND", b"")
    )
    return png, {"width": width, "height": height, "pixels": pixels}


def palette_rgba_image(raw, width):
    if not raw or len(raw) % 2 or width <= 0 or len(raw) // 2 % width:
        raise ValueError("palette RGBA dimensions must divide whole colors")
    pixels = bytearray()
    for offset in range(0, len(raw), 2):
        value = struct.unpack_from("<H", raw, offset)[0]
        pixels.extend(((value & 31) << 3, (value >> 5 & 31) << 3,
                       (value >> 10 & 31) << 3,
                       254 if value & 0x8000 else 255))
    png, report = rgba_image(bytes(pixels), width)
    report["palette_entries"] = len(raw) // 2
    return png, report


def self_test():
    for bpp, size in ((4, 32 * 7), (8, 64 * 4)):
        raw = bytes((index * 37 + 11) & 255 for index in range(size))
        png, _ = tile_png(raw, bpp, 7 if bpp == 4 else 4)
        tiles, _, _ = gba_graphics(png, bpp)
        if tiles != raw:
            raise AssertionError(f"{bpp}bpp tile round-trip failed")
    colors = [((index & 31) << 3, (index >> 3 & 31) << 3,
               (index >> 6 & 3) << 3) for index in range(256)]
    raw = bytes((index * 53 + 7) & 255 for index in range(64 * 4))
    png, _ = tile_png(raw, 8, 4, colors)
    tiles, palette, _ = gba_graphics(png, 8)
    expected_palette = b"".join(
        struct.pack("<H", (red >> 3) | (green >> 3) << 5 |
                    (blue >> 3) << 10) for red, green, blue in colors)
    if tiles != raw or palette != expected_palette:
        raise AssertionError("palette-correct tile round-trip failed")
    raw = b"".join(struct.pack("<H", (index * 109) & 0x7fff)
                   for index in range(224))
    png, _ = palette_png(raw)
    _, palette, _ = gba_graphics(png, 8)
    if palette != raw:
        raise AssertionError("palette round-trip failed")
    raw = bytes((index * 53 + 7) & 255 for index in range(128 * 64))
    png, _ = byte_png(raw, 128)
    _, _, pixels, _ = indexed_png(png)
    if bytes(pixels) != raw:
        raise AssertionError("indexed byte-image round-trip failed")
    raw = bytes((index * 29 + 3) & 255 for index in range(11 * 7 * 4))
    png, _ = rgba_image(raw, 11)
    if rgba_png(png) != (11, 7, raw):
        raise AssertionError("RGBA image round-trip failed")
    raw = b"".join(struct.pack("<H", (index * 109) & 0xffff)
                   for index in range(128))
    png, _ = palette_rgba_image(raw, 16)
    if gba_palette_rgba(png)[0] != raw:
        raise AssertionError("RGBA palette round-trip failed")
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
    tiles.add_argument("--palette", type=Path)
    tiles.add_argument("-o", "--output", type=Path, required=True)
    tile_file = commands.add_parser("tiles-file")
    tile_file.add_argument("input", type=Path)
    tile_file.add_argument("--bpp", type=int, choices=(4, 8), required=True)
    tile_file.add_argument("--columns", type=int, required=True)
    tile_file.add_argument("--palette", type=Path)
    tile_file.add_argument("-o", "--output", type=Path, required=True)
    palette = commands.add_parser("palette-file")
    palette.add_argument("input", type=Path)
    palette.add_argument("-o", "--output", type=Path, required=True)
    byte_file = commands.add_parser("bytes-file")
    byte_file.add_argument("input", type=Path)
    byte_file.add_argument("--width", type=int, required=True)
    byte_file.add_argument("-o", "--output", type=Path, required=True)
    rgba_file = commands.add_parser("rgba-file")
    rgba_file.add_argument("input", type=Path)
    rgba_file.add_argument("--width", type=int, required=True)
    rgba_file.add_argument("-o", "--output", type=Path, required=True)
    rgba_palette = commands.add_parser("palette-rgba-file")
    rgba_palette.add_argument("input", type=Path)
    rgba_palette.add_argument("--width", type=int, default=16)
    rgba_palette.add_argument("-o", "--output", type=Path, required=True)
    rgba_palette_rom = commands.add_parser("palette-rgba")
    rgba_palette_rom.add_argument("rom", type=Path)
    rgba_palette_rom.add_argument("--address", type=number, required=True)
    rgba_palette_rom.add_argument("--size", type=number, required=True)
    rgba_palette_rom.add_argument("--width", type=int, default=16)
    rgba_palette_rom.add_argument("-o", "--output", type=Path, required=True)
    args = parser.parse_args()
    if args.self_test:
        self_test()
        if args.command is None:
            return
    if args.command == "tiles":
        rom = args.rom.read_bytes()
        start = args.address - ROM_BASE
        end = start + args.size
        if start < 0 or end > len(rom) or start >= end:
            parser.error("tile range is outside the ROM or empty")
        if args.output.resolve() == args.rom.resolve():
            parser.error("refusing to overwrite the input ROM")
        colors = (indexed_png(args.palette.read_bytes())[3]
                  if args.palette is not None else None)
        png, report = tile_png(
            rom[start:end], args.bpp, args.columns, colors)
    elif args.command == "tiles-file":
        if args.output.resolve() == args.input.resolve():
            parser.error("refusing to overwrite the input")
        colors = (indexed_png(args.palette.read_bytes())[3]
                  if args.palette is not None else None)
        png, report = tile_png(
            args.input.read_bytes(), args.bpp, args.columns, colors)
    elif args.command == "palette-file":
        if args.output.resolve() == args.input.resolve():
            parser.error("refusing to overwrite the input")
        png, report = palette_png(args.input.read_bytes())
    elif args.command == "bytes-file":
        if args.output.resolve() == args.input.resolve():
            parser.error("refusing to overwrite the input")
        png, report = byte_png(args.input.read_bytes(), args.width)
    elif args.command == "rgba-file":
        if args.output.resolve() == args.input.resolve():
            parser.error("refusing to overwrite the input")
        png, report = rgba_image(args.input.read_bytes(), args.width)
    elif args.command == "palette-rgba-file":
        if args.output.resolve() == args.input.resolve():
            parser.error("refusing to overwrite the input")
        png, report = palette_rgba_image(
            args.input.read_bytes(), args.width)
    elif args.command == "palette-rgba":
        rom = args.rom.read_bytes()
        start = args.address - ROM_BASE
        end = start + args.size
        if start < 0 or end > len(rom) or start >= end:
            parser.error("palette range is outside the ROM or empty")
        if args.output.resolve() == args.rom.resolve():
            parser.error("refusing to overwrite the input ROM")
        png, report = palette_rgba_image(rom[start:end], args.width)
    else:
        parser.error("an asset command is required")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(png)
    print(json.dumps(report, sort_keys=True))


if __name__ == "__main__":
    main()
