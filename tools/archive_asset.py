#!/usr/bin/env python3
"""Export and rebuild offset-table archives of compressed RGBA cell maps."""
import argparse
import json
import struct
from pathlib import Path

from export_asset import byte_png, rgba_image
from extract_resource import decode_palette_trace, encode_palette
from import_asset import indexed_png, rgba_png


ROM_BASE = 0x08000000


def number(text):
    return int(text, 0)


def pixel_size(pixel_format):
    if pixel_format == "rgba":
        return 4
    if pixel_format == "indexed8":
        return 1
    raise ValueError("unsupported archive pixel format")


def make_atlas(chunks, chunk_width, chunk_height, columns, pixel_format):
    depth = pixel_size(pixel_format)
    unit = chunk_width * chunk_height * depth
    if not chunks or any(len(chunk) != unit for chunk in chunks):
        raise ValueError("archive chunks have inconsistent RGBA dimensions")
    rows = (len(chunks) + columns - 1) // columns
    padded = list(chunks) + [bytes(unit)] * (rows * columns - len(chunks))
    width = columns * chunk_width
    atlas = bytearray(width * rows * chunk_height * depth)
    for index, chunk in enumerate(padded):
        left = index % columns * chunk_width
        top = index // columns * chunk_height
        for y in range(chunk_height):
            source = y * chunk_width * depth
            target = ((top + y) * width + left) * depth
            atlas[target:target + chunk_width * depth] = (
                chunk[source:source + chunk_width * depth])
    encoder = rgba_image if pixel_format == "rgba" else byte_png
    return encoder(bytes(atlas), width)[0]


def read_atlas(data, count, chunk_width, chunk_height, columns, pixel_format):
    depth = pixel_size(pixel_format)
    if pixel_format == "rgba":
        width, height, pixels = rgba_png(data)
    else:
        width, height, values, _ = indexed_png(data)
        pixels = bytes(values)
    rows = (count + columns - 1) // columns
    if width != columns * chunk_width or height != rows * chunk_height:
        raise ValueError("archive atlas dimensions differ from its plan")
    chunks = []
    for index in range(count):
        left = index % columns * chunk_width
        top = index // columns * chunk_height
        chunk = bytearray()
        for y in range(chunk_height):
            start = ((top + y) * width + left) * depth
            chunk.extend(pixels[start:start + chunk_width * depth])
        chunks.append(bytes(chunk))
    return chunks


def build_archive(atlas, plan):
    streams = plan["streams"]
    chunks = read_atlas(
        atlas, len(streams), int(plan["chunk_width"]),
        int(plan["chunk_height"]), int(plan["columns"]),
        plan.get("pixel_format", "rgba"))
    encoded = []
    for chunk, stream in zip(chunks, streams):
        if len(chunk) != int(stream["decoded_size"]):
            raise ValueError("archive chunk size differs from its plan")
        payload = (encode_palette(chunk, stream["tokens"]) +
                   bytes.fromhex(stream.get("lookahead", "")))
        if len(payload) != int(stream["encoded_size"]):
            raise ValueError("archive stream size differs from its plan")
        encoded.append(payload)
    offset_width = int(plan.get("offset_width", 4))
    alignment = int(plan.get("stream_alignment", 1))
    offset = len(encoded) * offset_width
    offsets = []
    slots = []
    for payload in encoded:
        offsets.append(offset)
        padding = (-len(payload)) % alignment
        slot = payload + bytes(padding)
        slots.append(slot)
        offset += len(slot)
    code = "H" if offset_width == 2 else "I"
    if offset_width not in (2, 4) or any(value >= 1 << (offset_width * 8)
                                             for value in offsets):
        raise ValueError("archive offsets do not fit their configured width")
    return (struct.pack(f"<{len(offsets)}{code}", *offsets) + b"".join(slots))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    parser.add_argument("--address", type=number, required=True)
    parser.add_argument("--input-end", type=number, required=True)
    parser.add_argument("--chunk-width", type=int, required=True)
    parser.add_argument("--chunk-height", type=int, required=True)
    parser.add_argument("--columns", type=int, required=True)
    parser.add_argument("--pixel-format", choices=("rgba", "indexed8"),
                        default="rgba")
    parser.add_argument("--offset-width", type=int, choices=(2, 4), default=4)
    parser.add_argument("--stream-alignment", type=int, default=1)
    parser.add_argument("--plan", type=Path, required=True)
    parser.add_argument("--atlas", type=Path, required=True)
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    start = args.address - ROM_BASE
    end = args.input_end - ROM_BASE
    archive = rom[start:end]
    if len(archive) < 4:
        raise ValueError("archive is empty")
    if args.stream_alignment <= 0:
        raise ValueError("stream alignment must be positive")
    code = "H" if args.offset_width == 2 else "I"
    first = struct.unpack_from("<" + code, archive)[0]
    if first == 0 or first % args.offset_width or first > len(archive):
        raise ValueError("archive has no valid offset table")
    count = first // args.offset_width
    offsets = struct.unpack_from(f"<{count}{code}", archive)
    if offsets[0] != first or any(a > b for a, b in zip(offsets, offsets[1:])):
        raise ValueError("archive offsets are not monotonic")
    chunks = []
    streams = []
    maximum = (args.chunk_width * args.chunk_height *
               pixel_size(args.pixel_format))
    for index, offset in enumerate(offsets):
        stream_end = offsets[index + 1] if index + 1 < count else len(archive)
        decoded, cursor, tokens = decode_palette_trace(
            archive, offset, stream_end, maximum)
        if len(decoded) != maximum:
            raise ValueError("archive stream has an unexpected decoded size")
        replay = encode_palette(decoded, tokens)
        original = archive[offset:cursor]
        if not original.startswith(replay):
            raise ValueError("archive token replay differs")
        lookahead = original[len(replay):]
        padding = archive[cursor:stream_end]
        if len(padding) != (-len(original)) % args.stream_alignment or any(padding):
            raise ValueError("archive stream padding differs from its alignment")
        chunks.append(decoded)
        streams.append({
            "decoded_size": len(decoded), "encoded_size": len(original),
            "tokens": tokens, "lookahead": lookahead.hex(),
        })
    plan = {
        "format": 1, "codec": "golden-sun-offset-palette-lz",
        "chunk_width": args.chunk_width, "chunk_height": args.chunk_height,
        "columns": args.columns, "pixel_format": args.pixel_format,
        "offset_width": args.offset_width,
        "stream_alignment": args.stream_alignment, "streams": streams,
    }
    atlas = make_atlas(
        chunks, args.chunk_width, args.chunk_height, args.columns,
        args.pixel_format)
    if build_archive(atlas, plan) != archive:
        raise ValueError("rebuilt archive differs")
    args.plan.parent.mkdir(parents=True, exist_ok=True)
    args.atlas.parent.mkdir(parents=True, exist_ok=True)
    args.plan.write_text(json.dumps(plan, separators=(",", ":")) + "\n")
    args.atlas.write_bytes(atlas)
    print(f"streams={count} decoded={sum(map(len, chunks))} encoded={len(archive)}")


if __name__ == "__main__":
    main()
