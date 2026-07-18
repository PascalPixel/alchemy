#!/usr/bin/env python3
"""Export and rebuild the 16-color image archive used by resource F0."""
import argparse
import json
import struct
import zlib
from pathlib import Path

from export_asset import chunk
from generated_files import prune_files
from import_asset import indexed_png


ROM_BASE = 0x08000000
PREFIX = {
    1: "00", 2: "010", 3: "011", 4: "100", 5: "101", 6: "110",
    7: "11100", 8: "11101", 9: "11110", 10: "1111100",
    11: "1111101", 12: "1111110", 13: "111111100",
    14: "111111101", 15: "111111110",
}


class Bits:
    def __init__(self, data):
        self.data = data
        self.offset = 0

    def get(self):
        if self.offset >= len(self.data) * 8:
            raise ValueError("F0 image stream ended before its terminator")
        value = self.data[self.offset // 8] >> (self.offset & 7) & 1
        self.offset += 1
        return value


def selection(bits):
    if bits.get() == 0:
        return 1 if bits.get() == 0 else (2 if bits.get() == 0 else 3)
    if bits.get() == 0:
        return 4 if bits.get() == 0 else 5
    if bits.get() == 0:
        return 6
    if bits.get() == 0:
        return 7 if bits.get() == 0 else 8
    if bits.get() == 0:
        return 9
    if bits.get() == 0:
        return 10 if bits.get() == 0 else 11
    if bits.get() == 0:
        return 12
    if bits.get() == 0:
        return 13 if bits.get() == 0 else 14
    if bits.get() == 0:
        return 15
    return None


def decode_pixels(data):
    bits = Bits(data)
    move_to_front = list(range(16))
    pixels = bytearray()
    while True:
        while bits.get() == 0:
            pixels.append(move_to_front[0])
        index = selection(bits)
        if index is None:
            return bytes(pixels), bits.offset
        value = move_to_front.pop(index)
        move_to_front.insert(0, value)
        pixels.append(value)


def encode_pixels(pixels):
    move_to_front = list(range(16))
    bits = []
    for value in pixels:
        if not 0 <= value < 16:
            raise ValueError("F0 image contains a non-4bpp pixel")
        if value == move_to_front[0]:
            bits.append(0)
            continue
        index = move_to_front.index(value)
        bits.append(1)
        bits.extend(int(bit) for bit in PREFIX[index])
        move_to_front.insert(0, move_to_front.pop(index))
    bits.extend((1,) * 10)
    bits.extend((1,) * (-len(bits) % 8))
    encoded = bytearray(len(bits) // 8)
    for index, value in enumerate(bits):
        encoded[index // 8] |= value << (index & 7)
    return bytes(encoded)


def image_png(pixels, palette):
    if len(pixels) != 32 * 32 or len(palette) != 32:
        raise ValueError("F0 images must be 32x32 with 16 colors")
    colors = bytearray()
    for value in struct.unpack("<16H", palette):
        if value & 0x8000:
            raise ValueError("F0 palette uses an unsupported high bit")
        colors.extend(((value & 31) << 3, (value >> 5 & 31) << 3,
                       (value >> 10 & 31) << 3))
    rows = bytearray()
    for y in range(32):
        rows.append(0)
        row = pixels[y * 32:(y + 1) * 32]
        rows.extend(row[x] << 4 | row[x + 1] for x in range(0, 32, 2))
    return (b"\x89PNG\r\n\x1a\n" +
            chunk(b"IHDR", struct.pack(">IIBBBBB", 32, 32, 4, 3, 0, 0, 0)) +
            chunk(b"PLTE", bytes(colors)) +
            chunk(b"IDAT", zlib.compress(bytes(rows), 9)) +
            chunk(b"IEND", b""))


def read_image(data):
    width, height, pixels, colors = indexed_png(data)
    if (width, height, len(colors)) != (32, 32, 16):
        raise ValueError("F0 source PNG must be 32x32 with 16 colors")
    palette = bytearray()
    for red, green, blue in colors:
        if red & 7 or green & 7 or blue & 7:
            raise ValueError("F0 palette channels must be multiples of eight")
        palette.extend(struct.pack(
            "<H", (red >> 3) | (green >> 3) << 5 | (blue >> 3) << 10))
    return bytes(pixels), bytes(palette)


def preview_png(directory, count, columns=8, scale=2):
    rows = (count + columns - 1) // columns
    width, height = columns * 32 * scale, rows * 32 * scale
    output = bytearray([255]) * (width * height * 3)
    for index in range(count):
        pixels, palette = read_image(
            (directory / f"image_{index:02d}.png").read_bytes())
        colors = []
        for value in struct.unpack("<16H", palette):
            channels = (value & 31, value >> 5 & 31, value >> 10 & 31)
            colors.append(tuple((channel << 3) | (channel >> 2)
                                for channel in channels))
        left, top = index % columns * 32 * scale, index // columns * 32 * scale
        for y in range(32):
            for x in range(32):
                color = bytes(colors[pixels[y * 32 + x]])
                for sy in range(scale):
                    for sx in range(scale):
                        target = (((top + y * scale + sy) * width + left +
                                   x * scale + sx) * 3)
                        output[target:target + 3] = color
    scanlines = b"".join(
        b"\0" + output[y * width * 3:(y + 1) * width * 3]
        for y in range(height))
    return (b"\x89PNG\r\n\x1a\n" +
            chunk(b"IHDR", struct.pack(">IIBBBBB",
                                        width, height, 8, 2, 0, 0, 0)) +
            chunk(b"IDAT", zlib.compress(scanlines, 9)) +
            chunk(b"IEND", b""))


def package(image):
    pixels, palette = read_image(image)
    data = palette + encode_pixels(pixels)
    return data + bytes(-len(data) % 4)


def build_archive(plan, directory):
    count = int(plan["images"])
    packages = [package((directory / f"image_{index:02d}.png").read_bytes())
                for index in range(count)]
    offset = len(plan["entries"]) * 2
    offsets = []
    for payload in packages:
        offsets.append(offset)
        offset += len(payload)
    table = [0 if image is None else offsets[int(image)]
             for image in plan["entries"]]
    if any(value >= 0x10000 for value in table):
        raise ValueError("F0 archive offset exceeds 16 bits")
    return struct.pack(f"<{len(table)}H", *table) + b"".join(packages)


def number(text):
    return int(text, 0)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    parser.add_argument("--address", type=number, required=True)
    parser.add_argument("--input-end", type=number, required=True)
    parser.add_argument("--plan", type=Path, required=True)
    parser.add_argument("--images", type=Path, required=True)
    parser.add_argument("--preview", type=Path)
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    start, end = args.address - ROM_BASE, args.input_end - ROM_BASE
    archive = rom[start:end]
    first = struct.unpack_from("<H", archive)[0]
    if not first or first % 2 or first > len(archive):
        raise ValueError("F0 archive has no valid offset table")
    entries = struct.unpack_from(f"<{first // 2}H", archive)
    unique = sorted(offset for offset in set(entries) if offset)
    if not unique or unique[0] != first:
        raise ValueError("F0 first image does not follow its offset table")
    bounds = {offset: unique[index + 1] if index + 1 < len(unique)
              else len(archive) for index, offset in enumerate(unique)}
    args.images.mkdir(parents=True, exist_ok=True)
    index_by_offset = {offset: index for index, offset in enumerate(unique)}
    for index, offset in enumerate(unique):
        palette = archive[offset:offset + 32]
        pixels, _ = decode_pixels(archive[offset + 32:bounds[offset]])
        if len(pixels) != 32 * 32:
            raise ValueError("F0 image does not decode to 32x32")
        rebuilt = palette + encode_pixels(pixels)
        rebuilt += bytes(-len(rebuilt) % 4)
        if rebuilt != archive[offset:bounds[offset]]:
            raise ValueError("F0 image package does not re-encode exactly")
        (args.images / f"image_{index:02d}.png").write_bytes(
            image_png(pixels, palette))
    plan = {"format": 1, "codec": "golden-sun-mtf4-archive",
            "images": len(unique),
            "entries": [None if not offset else index_by_offset[offset]
                        for offset in entries]}
    if build_archive(plan, args.images) != archive:
        raise ValueError("rebuilt F0 archive differs")
    prune_files(
        args.images, "image_*.png",
        {f"image_{index:02d}.png" for index in range(len(unique))})
    args.plan.parent.mkdir(parents=True, exist_ok=True)
    args.plan.write_text(json.dumps(plan, separators=(",", ":")) + "\n")
    if args.preview is not None:
        args.preview.parent.mkdir(parents=True, exist_ok=True)
        args.preview.write_bytes(preview_png(args.images, len(unique)))
    print(f"entries={len(entries)} images={len(unique)} encoded={len(archive)}")


if __name__ == "__main__":
    main()
