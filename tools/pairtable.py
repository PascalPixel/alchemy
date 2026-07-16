#!/usr/bin/env python3
"""Losslessly convert little-endian u16 pairs to readable text."""
import argparse
import struct
from pathlib import Path


def export_pairs(data):
    if not data or len(data) % 4:
        raise ValueError("pair table must contain whole nonempty u16 pairs")
    values = struct.unpack(f"<{len(data) // 2}H", data)
    return "".join(
        f"0x{values[index]:04x} 0x{values[index + 1]:04x}\n"
        for index in range(0, len(values), 2)
    )


def import_pairs(text):
    values = []
    for line_number, line in enumerate(text.splitlines(), 1):
        tokens = line.split("#", 1)[0].split()
        if not tokens:
            continue
        if len(tokens) != 2:
            raise ValueError(
                f"line {line_number}: expected exactly two values")
        try:
            pair = [int(token, 0) for token in tokens]
        except ValueError as error:
            raise ValueError(
                f"line {line_number}: invalid pair value") from error
        if any(not 0 <= value <= 0xffff for value in pair):
            raise ValueError(
                f"line {line_number}: pair value is out of range")
        values.extend(pair)
    if not values:
        raise ValueError("pair table is empty")
    return struct.pack(f"<{len(values)}H", *values)


def self_test():
    raw = struct.pack("<6H", 0, 1, 0x1234, 0xabcd, 0xffff, 2)
    if import_pairs(export_pairs(raw)) != raw:
        raise AssertionError("pair-table round-trip failed")
    print("self-test=ok")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--self-test", action="store_true")
    commands = parser.add_subparsers(dest="command")
    export = commands.add_parser("export")
    export.add_argument("input", type=Path)
    export.add_argument("output", type=Path)
    build = commands.add_parser("build")
    build.add_argument("input", type=Path)
    build.add_argument("output", type=Path)
    args = parser.parse_args()
    if args.self_test:
        self_test()
        if args.command is None:
            return
    if args.command is None:
        parser.error("a pair-table command is required")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    if args.command == "export":
        args.output.write_text(export_pairs(args.input.read_bytes()))
    else:
        args.output.write_bytes(import_pairs(args.input.read_text()))


if __name__ == "__main__":
    main()
