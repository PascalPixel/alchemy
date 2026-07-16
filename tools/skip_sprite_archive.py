#!/usr/bin/env python3
"""Build and export zero-skip indexed-sprite archives."""
import argparse
import json
import struct
import zlib
from pathlib import Path

from export_asset import chunk
from import_asset import indexed_png


def decode_stream(data, offset, pixels):
    output = bytearray(pixels)
    cursor = offset
    position = 0
    while cursor < len(data):
        value = data[cursor]
        cursor += 1
        if value == 0:
            if position != pixels:
                raise ValueError("sprite stream ended before its image boundary")
            return bytes(output), cursor
        if value <= 0xdf:
            if position >= pixels:
                raise ValueError("sprite literal crossed its image boundary")
            output[position] = value
            position += 1
        else:
            position += value - 0xdf
            if position > pixels:
                raise ValueError("sprite skip crossed its image boundary")
    raise ValueError("sprite stream has no terminator")


def encode_stream(pixels):
    output = bytearray()
    cursor = 0
    while cursor < len(pixels):
        value = pixels[cursor]
        if value:
            if value > 0xdf:
                raise ValueError("sprite pixel exceeds the literal range")
            output.append(value)
            cursor += 1
            continue
        end = cursor + 1
        while end < len(pixels) and pixels[end] == 0:
            end += 1
        remaining = end - cursor
        while remaining:
            count = min(remaining, 32)
            output.append(0xdf + count)
            remaining -= count
        cursor = end
    output.append(0)
    return bytes(output)


def png(pixels, width, height, palette):
    if len(pixels) != width * height or not 1 <= len(palette) <= 256:
        raise ValueError("invalid indexed sprite image")
    colors = bytes(channel for color in palette for channel in color)
    rows = b"".join(
        b"\0" + pixels[y * width:(y + 1) * width]
        for y in range(height))
    return (b"\x89PNG\r\n\x1a\n" +
            chunk(b"IHDR", struct.pack(">IIBBBBB",
                                        width, height, 8, 3, 0, 0, 0)) +
            chunk(b"PLTE", colors) +
            chunk(b"tRNS", b"\0" + b"\xff" * (len(palette) - 1)) +
            chunk(b"IDAT", zlib.compress(rows, 9)) +
            chunk(b"IEND", b""))


def read_palette(path, offset, entries):
    _, _, pixels, colors = indexed_png(path.read_bytes())
    if offset + entries > len(pixels):
        raise ValueError("shared palette PNG is too small")
    selected = [colors[pixels[index]] for index in range(offset, offset + entries)]
    return selected


def read_image(path, width, height, palette):
    got_width, got_height, pixels, colors = indexed_png(path.read_bytes())
    if (got_width, got_height) != (width, height):
        raise ValueError(f"{path}: expected {width}x{height}")
    if colors != palette:
        raise ValueError(f"{path}: palette differs from the proven shared palette")
    if any(value > 0xdf for value in pixels):
        raise ValueError(f"{path}: pixel exceeds the archive literal range")
    return bytes(pixels)


def build_archive(plan, directory, palette_path):
    if plan.get("format") != 1 or plan.get("codec") != "zero-skip-sprite-archive":
        raise ValueError("unsupported zero-skip sprite plan")
    width, height = int(plan["width"]), int(plan["height"])
    count = int(plan["images"])
    order = list(map(int, plan["stream_order"]))
    if sorted(order) != list(range(count)):
        raise ValueError("sprite stream order is not a permutation")
    palette = read_palette(
        palette_path, int(plan["palette_offset"]), int(plan["palette_entries"]))
    images = [read_image(directory / f"frame_{index:02d}.png",
                         width, height, palette)
              for index in range(count)]
    streams = [encode_stream(image) for image in images]
    header_size = (count + 1) * 4
    offsets = [None] * count
    cursor = header_size
    for index in order:
        offsets[index] = cursor
        cursor += len(streams[index])
    return (struct.pack(f"<{count + 1}I", *offsets, 0) +
            b"".join(streams[index] for index in order))


def preview(images, width, height, palette, columns=4, scale=2):
    rows = (len(images) + columns - 1) // columns
    out_width, out_height = columns * width * scale, rows * height * scale
    output = bytearray((32, 32, 32)) * (out_width * out_height)
    for index, pixels in enumerate(images):
        left = index % columns * width * scale
        top = index // columns * height * scale
        for y in range(height):
            for x in range(width):
                value = pixels[y * width + x]
                color = palette[value] if value else (32, 32, 32)
                for sy in range(scale):
                    for sx in range(scale):
                        target = (((top + y * scale + sy) * out_width + left +
                                   x * scale + sx) * 3)
                        output[target:target + 3] = bytes(color)
    scanlines = b"".join(
        b"\0" + output[y * out_width * 3:(y + 1) * out_width * 3]
        for y in range(out_height))
    return (b"\x89PNG\r\n\x1a\n" +
            chunk(b"IHDR", struct.pack(">IIBBBBB", out_width, out_height,
                                        8, 2, 0, 0, 0)) +
            chunk(b"IDAT", zlib.compress(scanlines, 9)) +
            chunk(b"IEND", b""))


def export(decoded, directory, plan_path, palette_path, preview_path=None,
           width=64, height=64):
    entries = []
    for offset in range(0, min(len(decoded), 256 * 4), 4):
        value = struct.unpack_from("<I", decoded, offset)[0]
        entries.append(value)
        if value == 0:
            break
    if not entries or entries[-1] != 0:
        raise ValueError("archive pointer table lacks its null terminator")
    offsets = entries[:-1]
    first = len(entries) * 4
    if not offsets or min(offsets) != first:
        raise ValueError("archive streams do not follow its pointer table")
    if len(set(offsets)) != len(offsets) or any(
            value < first or value >= len(decoded) for value in offsets):
        raise ValueError("archive contains an invalid sprite pointer")
    images = []
    ends = {}
    for offset in offsets:
        image, end = decode_stream(decoded, offset, width * height)
        images.append(image)
        ends[offset] = end
    order = sorted(range(len(offsets)), key=offsets.__getitem__)
    for position, index in enumerate(order):
        expected = offsets[order[position + 1]] if position + 1 < len(order) else len(decoded)
        if ends[offsets[index]] != expected:
            raise ValueError("sprite streams are not contiguous")
    palette_offset, palette_entries = 16, 224
    palette = read_palette(palette_path, palette_offset, palette_entries)
    directory.mkdir(parents=True, exist_ok=True)
    for index, image in enumerate(images):
        (directory / f"frame_{index:02d}.png").write_bytes(
            png(image, width, height, palette))
    plan = {"format": 1, "codec": "zero-skip-sprite-archive",
            "width": width, "height": height, "images": len(images),
            "palette_offset": palette_offset,
            "palette_entries": palette_entries, "stream_order": order}
    plan_path.parent.mkdir(parents=True, exist_ok=True)
    plan_path.write_text(json.dumps(plan, separators=(",", ":")) + "\n")
    if build_archive(plan, directory, palette_path) != decoded:
        raise AssertionError("exported sprite archive does not round-trip")
    if preview_path is not None:
        preview_path.parent.mkdir(parents=True, exist_ok=True)
        preview_path.write_bytes(preview(images, width, height, palette))
    return len(images)


def self_test():
    source = bytes(([0] * 65 + [1, 2, 3] + [0] * 31) * 8)
    encoded = encode_stream(source)
    decoded, end = decode_stream(encoded, 0, len(source))
    if decoded != source or end != len(encoded):
        raise AssertionError("zero-skip stream round-trip failed")
    print("self-test=ok")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--self-test", action="store_true")
    parser.add_argument("--decoded", type=Path)
    parser.add_argument("--images", type=Path)
    parser.add_argument("--plan", type=Path)
    parser.add_argument("--palette", type=Path)
    parser.add_argument("--preview", type=Path)
    parser.add_argument("--width", type=int, default=64)
    parser.add_argument("--height", type=int, default=64)
    args = parser.parse_args()
    if args.self_test:
        self_test()
        if args.decoded is None:
            return
    if None in (args.decoded, args.images, args.plan, args.palette):
        parser.error("--decoded, --images, --plan, and --palette are required")
    count = export(args.decoded.read_bytes(), args.images, args.plan,
                   args.palette, args.preview, args.width, args.height)
    print(f"images={count} decoded={args.decoded.stat().st_size}")


if __name__ == "__main__":
    main()
