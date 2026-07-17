#!/usr/bin/env python3
"""Decode, encode, and export the ROM's tag-2 nibble-MTF LZ format."""
import argparse
import json
from pathlib import Path

from export_asset import tile_png
from import_asset import indexed_png


ROM_BASE = 0x08000000


class BitReader:
    def __init__(self, data, bit=8):
        self.data = data
        self.bit = bit

    def get(self):
        if self.bit >= len(self.data) * 8:
            raise ValueError("tag-2 stream ended before its terminator")
        value = self.data[self.bit // 8] >> (self.bit & 7) & 1
        self.bit += 1
        return value

    def take(self, count):
        return sum(self.get() << index for index in range(count))


class BitWriter:
    def __init__(self):
        self.bits = []

    def put(self, value):
        self.bits.append(int(value))

    def write(self, value, count):
        self.bits.extend(value >> index & 1 for index in range(count))

    def finish(self):
        output = bytearray((len(self.bits) + 7) // 8)
        for index, value in enumerate(self.bits):
            output[index // 8] |= value << (index & 7)
        return bytes(output)


def literal_width(bits):
    if bits.get():
        return 2
    return 3 if bits.get() else 4


def match_length(bits):
    if not bits.get():
        return 2
    if not bits.get():
        return 3
    if not bits.get():
        return 4
    if not bits.get():
        return 5
    if not bits.get():
        return 7 if bits.get() else 6
    value = bits.take(2)
    if value:
        return value + 7
    value = bits.take(7)
    return None if value == 0 else value + 10


def distance_width(produced):
    if produced < 33 or produced >= 2081:
        return 12
    return (produced - 33).bit_length()


def decode_kind2(data):
    if not data or data[0] != 2:
        raise ValueError("tag-2 resource must begin with byte 2")
    bits = BitReader(data)
    move_to_front = list(range(16))
    output = bytearray()
    tokens = []

    def select(index):
        value = move_to_front.pop(index)
        move_to_front.insert(0, value)
        return value

    while True:
        if bits.get():
            width = literal_width(bits)
            low = select(bits.take(width))
            high = select(bits.take(width))
            output.append(low | high << 4)
            tokens.append(width)
            continue
        length = match_length(bits)
        if length is None:
            used = (bits.bit + 7) // 8
            return bytes(output), used, bits.bit, tokens
        if bits.get():
            distance = bits.take(5) + 1
        else:
            distance = bits.take(distance_width(len(output))) + 33
        if distance > len(output):
            raise ValueError("tag-2 copy precedes decoded output")
        tokens.append([distance, length])
        for _ in range(length):
            output.append(output[-distance])


def put_length(bits, length):
    if length == 2:
        bits.put(0)
    elif length == 3:
        bits.write(1, 1); bits.put(0)
    elif length == 4:
        bits.write(3, 2); bits.put(0)
    elif length == 5:
        bits.write(7, 3); bits.put(0)
    elif length in (6, 7):
        bits.write(15, 4); bits.put(0); bits.put(length - 6)
    elif 8 <= length <= 10:
        bits.write(31, 5); bits.write(length - 7, 2)
    elif 11 <= length <= 137:
        bits.write(31, 5); bits.write(0, 2); bits.write(length - 10, 7)
    else:
        raise ValueError("tag-2 copy length is outside 2..137")


def encode_kind2(decoded, plan):
    if plan.get("format") != 1 or plan.get("codec") != "golden-sun-kind2-lz":
        raise ValueError("unsupported tag-2 plan")
    bits = BitWriter()
    move_to_front = list(range(16))
    cursor = 0

    def index(value):
        position = move_to_front.index(value)
        move_to_front.insert(0, move_to_front.pop(position))
        return position

    for token in plan["tokens"]:
        if isinstance(token, int):
            width = token
            if width not in (2, 3, 4) or cursor >= len(decoded):
                raise ValueError("invalid tag-2 literal token")
            bits.put(1)
            if width == 2:
                bits.put(1)
            else:
                bits.put(0); bits.put(width == 3)
            value = decoded[cursor]
            low, high = index(value & 15), index(value >> 4)
            if low >= 1 << width or high >= 1 << width:
                raise ValueError("literal does not fit its recorded MTF width")
            bits.write(low, width); bits.write(high, width)
            cursor += 1
            continue
        if (not isinstance(token, list) or len(token) != 2 or
                any(not isinstance(value, int) for value in token)):
            raise ValueError("invalid tag-2 copy token")
        distance, length = token
        if distance <= 0 or cursor + length > len(decoded):
            raise ValueError("tag-2 copy is outside decoded data")
        for index_ in range(length):
            if decoded[cursor + index_] != decoded[cursor + index_ - distance]:
                raise ValueError("tag-2 copy token differs from source pixels")
        bits.put(0); put_length(bits, length)
        if distance <= 32:
            bits.put(1); bits.write(distance - 1, 5)
        else:
            width = distance_width(cursor)
            value = distance - 33
            if value < 0 or value >= 1 << width:
                raise ValueError("tag-2 long distance does not fit")
            bits.put(0); bits.write(value, width)
        cursor += length
    if cursor != len(decoded):
        raise ValueError("tag-2 tokens do not cover decoded data")
    bits.put(0); bits.write(31, 5); bits.write(0, 2); bits.write(0, 7)
    encoded = b"\2" + bits.finish() + bytes.fromhex(plan.get("lookahead", ""))
    return encoded


def export_resource(data, source, plan_path, bpp, columns, palette_path=None,
                    layout=None):
    decoded, used, bit, tokens = decode_kind2(data)
    colors = (indexed_png(palette_path.read_bytes())[3]
              if palette_path is not None else None)
    image, _ = tile_png(decoded, bpp, columns, colors)
    plan = {"format": 1, "codec": "golden-sun-kind2-lz",
            "decoded_size": len(decoded), "encoded_size": len(data),
            "tokens": tokens, "lookahead": data[used:].hex()}
    if layout is not None:
        plan["layout"] = layout
    if encode_kind2(decoded, plan) != data:
        raise AssertionError("tag-2 resource does not re-encode exactly")
    source.parent.mkdir(parents=True, exist_ok=True)
    source.write_bytes(image)
    plan_path.parent.mkdir(parents=True, exist_ok=True)
    plan_path.write_text(json.dumps(plan, separators=(",", ":")) + "\n")
    return len(decoded), len(tokens), bit


def self_test():
    prefix = bytes((index * 37 + 11) & 255 for index in range(32))
    decoded = prefix + prefix[-16:] * 2
    plan = {"format": 1, "codec": "golden-sun-kind2-lz",
            "tokens": [4] * 32 + [[16, 32]], "lookahead": "a55a"}
    encoded = encode_kind2(decoded, plan)
    rebuilt, used, _, tokens = decode_kind2(encoded)
    if rebuilt != decoded or tokens != plan["tokens"] or encoded[used:] != b"\xa5Z":
        raise AssertionError("tag-2 codec self-test failed")
    print("self-test=ok")


def number(text):
    return int(text, 0)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--self-test", action="store_true")
    parser.add_argument("rom", type=Path, nargs="?")
    parser.add_argument("--address", type=number)
    parser.add_argument("--size", type=number)
    parser.add_argument("--source", type=Path)
    parser.add_argument("--plan", type=Path)
    parser.add_argument("--bpp", type=int, choices=(4, 8), default=4)
    parser.add_argument("--columns", type=int, default=16)
    parser.add_argument("--palette", type=Path)
    args = parser.parse_args()
    if args.self_test:
        self_test()
        if args.rom is None:
            return
    if None in (args.rom, args.address, args.size, args.source, args.plan):
        parser.error("rom, --address, --size, --source, and --plan are required")
    rom = args.rom.read_bytes()
    start = args.address - ROM_BASE
    data = rom[start:start + args.size]
    if len(data) != args.size:
        parser.error("resource range is outside the ROM")
    decoded, tokens, bit = export_resource(
        data, args.source, args.plan, args.bpp, args.columns, args.palette)
    print(f"decoded={decoded:#x} tokens={tokens} consumed_bits={bit}")


if __name__ == "__main__":
    main()
