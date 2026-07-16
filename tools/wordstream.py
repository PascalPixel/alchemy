#!/usr/bin/env python3
"""Deterministic little-endian 16-bit word stream text conversion."""
import argparse
import struct
from pathlib import Path


def export_words(data):
    if len(data) % 2:
        raise ValueError("word stream has an odd byte count")
    words = struct.unpack(f"<{len(data) // 2}H", data)
    return "".join(f"0x{word:04x}\n" for word in words)


def import_words(text):
    words = []
    for line_number, line in enumerate(text.splitlines(), 1):
        value = line.split("#", 1)[0].strip()
        if not value:
            continue
        try:
            word = int(value, 0)
        except ValueError as error:
            raise ValueError(f"line {line_number}: invalid word") from error
        if not 0 <= word <= 0xffff:
            raise ValueError(f"line {line_number}: word is out of range")
        words.append(word)
    return struct.pack(f"<{len(words)}H", *words)


def main():
    parser = argparse.ArgumentParser()
    commands = parser.add_subparsers(dest="command", required=True)
    export = commands.add_parser("export")
    export.add_argument("input", type=Path)
    export.add_argument("output", type=Path)
    build = commands.add_parser("build")
    build.add_argument("input", type=Path)
    build.add_argument("output", type=Path)
    args = parser.parse_args()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    if args.command == "export":
        args.output.write_text(export_words(args.input.read_bytes()))
    else:
        args.output.write_bytes(import_words(args.input.read_text()))


if __name__ == "__main__":
    main()
