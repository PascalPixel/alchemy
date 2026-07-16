#!/usr/bin/env python3
"""Losslessly convert little-endian 16-bit tilemaps to readable text."""
import argparse
import re
import struct
from pathlib import Path


WORD = re.compile(r"[0-9a-fA-F]{4}")


def export_tilemap(data, width):
    if width <= 0 or len(data) % 2:
        raise ValueError("tilemap must contain whole 16-bit entries")
    values = struct.unpack(f"<{len(data) // 2}H", data)
    if not values or len(values) % width:
        raise ValueError("width must divide the tilemap entry count")
    return "".join(
        " ".join(f"{value:04x}" for value in values[start:start + width]) + "\n"
        for start in range(0, len(values), width)
    )


def import_tilemap(text):
    tokens = text.split()
    if not tokens or any(WORD.fullmatch(token) is None for token in tokens):
        raise ValueError("tilemap text must contain four-digit hex words")
    return b"".join(struct.pack("<H", int(token, 16)) for token in tokens)


def self_test():
    raw = b"".join(struct.pack("<H", index * 257) for index in range(64))
    if import_tilemap(export_tilemap(raw, 8)) != raw:
        raise AssertionError("tilemap round-trip failed")
    print("self-test=ok")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--self-test", action="store_true")
    commands = parser.add_subparsers(dest="command")
    export = commands.add_parser("export")
    export.add_argument("input", type=Path)
    export.add_argument("--width", type=int, required=True)
    export.add_argument("-o", "--output", type=Path, required=True)
    build = commands.add_parser("import")
    build.add_argument("input", type=Path)
    build.add_argument("-o", "--output", type=Path, required=True)
    args = parser.parse_args()
    if args.self_test:
        self_test()
        if args.command is None:
            return
    if args.command == "export":
        result = export_tilemap(args.input.read_bytes(), args.width)
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(result)
        print(f"entries={len(result.split())}")
    elif args.command == "import":
        result = import_tilemap(args.input.read_text())
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_bytes(result)
        print(f"bytes={len(result)}")
    else:
        parser.error("a tilemap command is required")


if __name__ == "__main__":
    main()
