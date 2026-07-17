#!/usr/bin/env python3
"""Rebuild the tagged kind-1 128x128 four-byte map-record grid."""
import argparse
import json
import struct
import zlib
from pathlib import Path

from export_asset import byte_png, chunk
from extract_resource import decode_palette_trace, encode_palette
from import_asset import indexed_png


ROM_BASE = 0x08000000
PLANE = 0x4000
FILES = ("value_low.png", "value_high.png",
         "attribute_a.png", "attribute_b.png")


def mask_png(mask):
    if len(mask) != PLANE or any(value not in (0, 1) for value in mask):
        raise ValueError("sentinel mask must contain 128x128 binary pixels")
    rows = bytearray()
    for y in range(128):
        rows.append(0)
        for x in range(0, 128, 8):
            rows.append(sum(mask[y * 128 + x + bit] << (7 - bit)
                            for bit in range(8)))
    return (b"\x89PNG\r\n\x1a\n" +
            chunk(b"IHDR", struct.pack(">IIBBBBB", 128, 128,
                                        1, 3, 0, 0, 0)) +
            chunk(b"PLTE", b"\0\0\0\xff\xff\xff") +
            chunk(b"IDAT", zlib.compress(bytes(rows), 9)) +
            chunk(b"IEND", b""))


def transform(decoded):
    if len(decoded) != 4 * PLANE:
        raise ValueError("kind-1 map input must contain three 128x128 planes")
    plane_a = decoded[:PLANE]
    plane_b = decoded[PLANE:2 * PLANE]
    values = decoded[2 * PLANE:]
    planes = [bytearray() for _ in range(4)]
    mask = bytearray(PLANE)
    ordinal = 0
    for index in range(PLANE):
        value = struct.unpack_from("<H", values, index * 2)[0]
        if value & 0xfff == 0xfff:
            mask[index] = 1
            if ordinal < 0xfff:
                value = (value & 0xf000) | ordinal
                ordinal += 1
        planes[0].append(value & 0xff)
        planes[1].append(value >> 8)
        planes[2].append(plane_a[index])
        planes[3].append(plane_b[index])
    return tuple(map(bytes, planes)), bytes(mask)


def inverse(planes, mask):
    if len(planes) != 4 or any(len(plane) != PLANE for plane in planes):
        raise ValueError("map source must contain four 128x128 byte planes")
    if len(mask) != PLANE or any(value not in (0, 1) for value in mask):
        raise ValueError("map sentinel mask is invalid")
    values = bytearray()
    ordinal = 0
    for index in range(PLANE):
        value = planes[0][index] | planes[1][index] << 8
        if mask[index]:
            expected = ((value & 0xf000) | ordinal
                        if ordinal < 0xfff else value)
            if value != expected:
                raise ValueError("sentinel mask disagrees with transformed value")
            value = (value & 0xf000) | 0xfff
            if ordinal < 0xfff:
                ordinal += 1
        elif value & 0xfff == 0xfff:
            raise ValueError("unmasked source value collides with sentinel form")
        values.extend(struct.pack("<H", value))
    return planes[2] + planes[3] + bytes(values)


def read_plane(path):
    width, height, pixels, _ = indexed_png(path.read_bytes())
    if (width, height, len(pixels)) != (128, 128, PLANE):
        raise ValueError(f"{path}: expected one 128x128 byte plane")
    return bytes(pixels)


def build_grid(plan, directory):
    if plan.get("format") != 1 or plan.get("codec") != "golden-sun-kind1-grid":
        raise ValueError("unsupported kind-1 grid plan")
    planes = tuple(read_plane(directory / name) for name in FILES)
    _, _, mask, _ = indexed_png((directory / "sentinels.png").read_bytes())
    decoded = inverse(planes, bytes(mask))
    encoded = b"\1" + encode_palette(decoded, plan["tokens"])
    return encoded + bytes.fromhex(plan.get("lookahead", ""))


def export_grid(data, directory):
    if not data or data[0] != 1:
        raise ValueError("kind-1 map grid must begin with byte 1")
    decoded, cursor, tokens = decode_palette_trace(data, 1, len(data), 0x10000)
    planes, mask = transform(decoded)
    directory.mkdir(parents=True, exist_ok=True)
    for name, plane in zip(FILES, planes):
        image, _ = byte_png(plane, 128)
        (directory / name).write_bytes(image)
    (directory / "sentinels.png").write_bytes(mask_png(mask))
    encoded = b"\1" + encode_palette(decoded, tokens)
    if not data.startswith(encoded):
        raise AssertionError("kind-1 token replay differs")
    plan = {"format": 1, "codec": "golden-sun-kind1-grid",
            "decoded_size": len(decoded), "encoded_size": len(data),
            "tokens": tokens, "lookahead": data[len(encoded):].hex()}
    (directory / "grid.kind1.json").write_text(
        json.dumps(plan, separators=(",", ":")) + "\n")
    if build_grid(plan, directory) != data:
        raise AssertionError("exported kind-1 grid does not round-trip")
    return len(tokens), sum(mask)


def self_test():
    decoded = bytearray(4 * PLANE)
    for index, value in enumerate((0x0fff, 0x2fff, 0x1234, 0x4fff)):
        struct.pack_into("<H", decoded, 2 * PLANE + index * 2, value)
    planes, mask = transform(bytes(decoded))
    if inverse(planes, mask) != decoded or sum(mask) != 3:
        raise AssertionError("kind-1 map transform self-test failed")
    print("self-test=ok")


def number(text):
    return int(text, 0)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--self-test", action="store_true")
    parser.add_argument("rom", type=Path, nargs="?")
    parser.add_argument("--address", type=number)
    parser.add_argument("--size", type=number)
    parser.add_argument("--directory", type=Path)
    args = parser.parse_args()
    if args.self_test:
        self_test()
        if args.rom is None:
            return
    if None in (args.rom, args.address, args.size, args.directory):
        parser.error("rom, --address, --size, and --directory are required")
    rom = args.rom.read_bytes()
    start = args.address - ROM_BASE
    data = rom[start:start + args.size]
    if len(data) != args.size:
        parser.error("kind-1 grid range is outside the ROM")
    tokens, sentinels = export_grid(data, args.directory)
    print(f"tokens={tokens} sentinels={sentinels} encoded={len(data):#x}")


if __name__ == "__main__":
    main()
