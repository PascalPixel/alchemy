#!/usr/bin/env python3
"""Export a payload-free token plan for an exact custom-LZ stream."""
import argparse
import json
from pathlib import Path

from extract_resource import decode_general_trace, encode_general


ROM_BASE = 0x08000000


def number(text):
    try:
        return int(text, 0)
    except ValueError as error:
        raise argparse.ArgumentTypeError(f"invalid integer: {text}") from error


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    parser.add_argument("--address", type=number, required=True)
    parser.add_argument("--input-end", type=number, required=True)
    parser.add_argument("--max-output", type=number, required=True)
    parser.add_argument("--plan", type=Path, required=True)
    parser.add_argument("--decoded", type=Path)
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    start = args.address - ROM_BASE
    end = args.input_end - ROM_BASE
    if start < 0 or not start < end <= len(rom):
        parser.error("compressed range is outside the ROM or empty")
    decoded, cursor, tokens = decode_general_trace(
        rom, start, end, args.max_output)
    encoded = encode_general(decoded, tokens)
    original = rom[start:cursor]
    if not original.startswith(encoded):
        raise ValueError("token replay differs before decoder look-ahead")
    lookahead = original[len(encoded):]
    plan = {
        "format": 1,
        "codec": "golden-sun-general-lz",
        "decoded_size": len(decoded),
        "encoded_size": len(original),
        "tokens": tokens,
        "lookahead": lookahead.hex(),
    }
    args.plan.parent.mkdir(parents=True, exist_ok=True)
    args.plan.write_text(json.dumps(plan, separators=(",", ":")) + "\n")
    if args.decoded is not None:
        args.decoded.parent.mkdir(parents=True, exist_ok=True)
        args.decoded.write_bytes(decoded)
    print(
        f"tokens={len(tokens)} decoded={len(decoded)} encoded={len(original)} "
        f"lookahead={len(lookahead)}"
    )


if __name__ == "__main__":
    main()
