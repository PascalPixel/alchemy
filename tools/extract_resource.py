#!/usr/bin/env python3
import argparse
import struct
from pathlib import Path

ROM_BASE = 0x08000000
TABLE = 0x08320000


class DecodeError(ValueError):
    pass


class LsbBits:
    def __init__(self, data, cursor, end):
        self.data = data
        self.cursor = cursor
        self.end = end
        self.value = 0
        self.count = 0
        if cursor & 1:
            self.value = self.byte()
            self.count = 8
        self.fill()

    def need(self, size):
        if self.cursor + size > self.end:
            raise DecodeError("compressed input ended before terminator")

    def byte(self):
        self.need(1)
        value = self.data[self.cursor]
        self.cursor += 1
        return value

    def fill(self):
        self.need(2)
        self.value |= struct.unpack_from("<H", self.data, self.cursor)[0] << self.count
        self.cursor += 2
        self.count += 16

    def get(self, count):
        while self.count < count:
            self.fill()
        value = self.value & ((1 << count) - 1)
        self.value >>= count
        self.count -= count
        return value


def append_copy(output, distance, length, maximum):
    if not 1 <= distance <= len(output):
        raise DecodeError(
            f"invalid back-reference distance at output offset {len(output):#x}")
    if length < 1 or len(output) + length > maximum:
        raise DecodeError("decoded output crossed configured bound")
    for _ in range(length):
        output.append(output[-distance])


def decode_general_trace(data, start, end, maximum):
    if start >= end or data[start] != 0:
        raise DecodeError("general stream is missing its kind-zero header")
    bits = LsbBits(data, start + 1, end)
    output = bytearray()
    tokens = []
    while True:
        if bits.get(1):
            if len(output) >= maximum:
                raise DecodeError("decoded output crossed configured bound")
            output.append(bits.get(8))
            if tokens and tokens[-1][0] == "l":
                tokens[-1][1] += 1
            else:
                tokens.append(["l", 1])
            continue
        if not bits.get(1):
            length = 2
        elif not bits.get(1):
            length = 3
        elif not bits.get(1):
            length = 4
        elif not bits.get(1):
            length = 5
        elif not bits.get(1):
            length = 6 if not bits.get(1) else 7
        else:
            short = bits.get(2)
            if short:
                length = short + 7
            else:
                long = bits.get(7)
                if long == 0:
                    return bytes(output), bits.cursor, tokens
                length = long + 10
        if bits.get(1):
            distance = bits.get(5) + 1
        else:
            window = len(output) - 33
            width = window.bit_length() if 0 <= window < 2048 else 12
            distance = bits.get(width) + 33
        tokens.append(["c", length, distance])
        append_copy(output, distance, length, maximum)


def decode_general(data, start, end, maximum):
    output, cursor, _ = decode_general_trace(data, start, end, maximum)
    return output, cursor


def encode_general(decoded, tokens):
    bits = []
    replay = bytearray()
    cursor = 0
    for token in tokens:
        if token[0] == "l":
            count = int(token[1])
            if count < 1 or cursor + count > len(decoded):
                raise DecodeError("literal run crossed decoded input")
            for _ in range(count):
                put(bits, 1, 1)
                value = decoded[cursor]
                put(bits, value, 8)
                replay.append(value)
                cursor += 1
            continue
        if len(token) != 3 or token[0] != "c":
            raise DecodeError("invalid general-stream token")
        length, distance = map(int, token[1:])
        put(bits, 0, 1)
        if length == 2:
            put(bits, 0, 1)
        elif length == 3:
            put(bits, 1, 1)
            put(bits, 0, 1)
        elif length == 4:
            put(bits, 3, 2)
            put(bits, 0, 1)
        elif length == 5:
            put(bits, 7, 3)
            put(bits, 0, 1)
        elif length in (6, 7):
            put(bits, 15, 4)
            put(bits, 0, 1)
            put(bits, length - 6, 1)
        elif length in (8, 9, 10):
            put(bits, 31, 5)
            put(bits, length - 7, 2)
        elif 11 <= length <= 137:
            put(bits, 31, 5)
            put(bits, 0, 2)
            put(bits, length - 10, 7)
        else:
            raise DecodeError(f"unencodable copy length: {length}")
        if not 1 <= distance <= len(replay):
            raise DecodeError("copy distance crossed replay prefix")
        if distance <= 32:
            put(bits, 1, 1)
            put(bits, distance - 1, 5)
        else:
            put(bits, 0, 1)
            window = len(replay) - 33
            width = window.bit_length() if 0 <= window < 2048 else 12
            if distance - 33 >= 1 << width:
                raise DecodeError("copy distance exceeds position-dependent width")
            put(bits, distance - 33, width)
        for _ in range(length):
            replay.append(replay[-distance])
        cursor += length
    if cursor != len(decoded) or bytes(replay) != bytes(decoded):
        raise DecodeError("token plan does not reconstruct decoded input")
    put(bits, 0, 1)
    put(bits, 31, 5)
    put(bits, 0, 2)
    put(bits, 0, 7)
    packed = bytearray(b"\0")
    for offset in range(0, len(bits), 8):
        packed.append(sum(
            bit << index for index, bit in enumerate(bits[offset:offset + 8])))
    return bytes(packed)


def decode_general_prefill_trace(data, start, end, maximum, prefill):
    """Decode a general-LZ stream whose output window starts pre-filled.

    Identical to decode_general_trace except the output buffer begins with
    ``prefill`` zero bytes, so an early back-reference may read the cleared
    decompression buffer. The prefill is not part of the decoded resource; the
    window used to size long distances excludes it. Golden Sun uses this for a
    large family of resources that a bare general-LZ decode rejects at offset 0.
    """
    if start >= end or data[start] != 0:
        raise DecodeError("general stream is missing its kind-zero header")
    bits = LsbBits(data, start + 1, end)
    output = bytearray(prefill)
    tokens = []
    while True:
        if bits.get(1):
            if len(output) - prefill >= maximum:
                raise DecodeError("decoded output crossed configured bound")
            output.append(bits.get(8))
            if tokens and tokens[-1][0] == "l":
                tokens[-1][1] += 1
            else:
                tokens.append(["l", 1])
            continue
        if not bits.get(1):
            length = 2
        elif not bits.get(1):
            length = 3
        elif not bits.get(1):
            length = 4
        elif not bits.get(1):
            length = 5
        elif not bits.get(1):
            length = 6 if not bits.get(1) else 7
        else:
            short = bits.get(2)
            if short:
                length = short + 7
            else:
                long = bits.get(7)
                if long == 0:
                    return bytes(output[prefill:]), bits.cursor, tokens
                length = long + 10
        if bits.get(1):
            distance = bits.get(5) + 1
        else:
            window = len(output) - prefill - 33
            width = window.bit_length() if 0 <= window < 2048 else 12
            distance = bits.get(width) + 33
        tokens.append(["c", length, distance])
        append_copy(output, distance, length, prefill + maximum)


def encode_general_prefill(decoded, tokens, prefill):
    """Re-encode a pre-filled general-LZ stream (inverse of the decoder)."""
    bits = []
    replay = bytearray(prefill)
    cursor = 0
    for token in tokens:
        if token[0] == "l":
            count = int(token[1])
            if count < 1 or cursor + count > len(decoded):
                raise DecodeError("literal run crossed decoded input")
            for _ in range(count):
                put(bits, 1, 1)
                value = decoded[cursor]
                put(bits, value, 8)
                replay.append(value)
                cursor += 1
            continue
        if len(token) != 3 or token[0] != "c":
            raise DecodeError("invalid general-stream token")
        length, distance = map(int, token[1:])
        put(bits, 0, 1)
        if length == 2:
            put(bits, 0, 1)
        elif length == 3:
            put(bits, 1, 1)
            put(bits, 0, 1)
        elif length == 4:
            put(bits, 3, 2)
            put(bits, 0, 1)
        elif length == 5:
            put(bits, 7, 3)
            put(bits, 0, 1)
        elif length in (6, 7):
            put(bits, 15, 4)
            put(bits, 0, 1)
            put(bits, length - 6, 1)
        elif length in (8, 9, 10):
            put(bits, 31, 5)
            put(bits, length - 7, 2)
        elif 11 <= length <= 137:
            put(bits, 31, 5)
            put(bits, 0, 2)
            put(bits, length - 10, 7)
        else:
            raise DecodeError(f"unencodable copy length: {length}")
        if not 1 <= distance <= len(replay):
            raise DecodeError("copy distance crossed replay prefix")
        if distance <= 32:
            put(bits, 1, 1)
            put(bits, distance - 1, 5)
        else:
            put(bits, 0, 1)
            window = len(replay) - prefill - 33
            width = window.bit_length() if 0 <= window < 2048 else 12
            if distance - 33 >= 1 << width:
                raise DecodeError("copy distance exceeds position-dependent width")
            put(bits, distance - 33, width)
        for _ in range(length):
            replay.append(replay[-distance])
        cursor += length
    if cursor != len(decoded) or bytes(replay[prefill:]) != bytes(decoded):
        raise DecodeError("token plan does not reconstruct decoded input")
    put(bits, 0, 1)
    put(bits, 31, 5)
    put(bits, 0, 2)
    put(bits, 0, 7)
    packed = bytearray(b"\0")
    for offset in range(0, len(bits), 8):
        packed.append(sum(
            bit << index for index, bit in enumerate(bits[offset:offset + 8])))
    return bytes(packed)


def decode_palette_trace(data, start, end, maximum):
    cursor = start
    output = bytearray()
    groups = []

    def take(size):
        nonlocal cursor
        if cursor + size > end:
            raise DecodeError("compressed input ended before terminator")
        result = data[cursor:cursor + size]
        cursor += size
        return result

    while True:
        flags = take(1)[0]
        if flags == 0:
            block = take(8)
            if len(output) + 8 > maximum:
                raise DecodeError("decoded output crossed configured bound")
            output.extend(block)
            groups.append(["z"])
            continue
        operations = []
        groups.append(["g", operations])
        for bit in range(7, -1, -1):
            if not flags & (1 << bit):
                if len(output) >= maximum:
                    raise DecodeError("decoded output crossed configured bound")
                output.extend(take(1))
                operations.append(["l"])
                continue
            first, second = take(2)
            distance = ((first & 0xF0) << 4) | second
            length = first & 0x0F
            if length == 0:
                if distance == 0:
                    operations.append(["e"])
                    return bytes(output), cursor, groups
                length = take(1)[0] + 17
            else:
                length += 1
            operations.append(["c", length, distance])
            append_copy(output, distance, length, maximum)


def decode_palette(data, start, end, maximum):
    output, cursor, _ = decode_palette_trace(data, start, end, maximum)
    return output, cursor


def encode_palette(decoded, groups):
    output = bytearray()
    encoded = bytearray()
    cursor = 0
    ended = False
    for group in groups:
        if ended:
            raise DecodeError("palette plan contains data after terminator")
        if group == ["z"]:
            if cursor + 8 > len(decoded):
                raise DecodeError("palette literal block crossed decoded input")
            encoded.append(0)
            encoded.extend(decoded[cursor:cursor + 8])
            output.extend(decoded[cursor:cursor + 8])
            cursor += 8
            continue
        if len(group) != 2 or group[0] != "g" or not group[1]:
            raise DecodeError("invalid palette token group")
        operations = group[1]
        if len(operations) > 8:
            raise DecodeError("palette group exceeds eight operations")
        flags = 0
        payload = bytearray()
        for index, token in enumerate(operations):
            if token == ["l"]:
                if cursor >= len(decoded):
                    raise DecodeError("palette literal crossed decoded input")
                payload.append(decoded[cursor])
                output.append(decoded[cursor])
                cursor += 1
                continue
            flags |= 1 << (7 - index)
            if token == ["e"]:
                payload.extend(b"\0\0")
                ended = True
                if index + 1 != len(operations):
                    raise DecodeError("palette terminator is not group-final")
                continue
            if len(token) != 3 or token[0] != "c":
                raise DecodeError("invalid palette copy token")
            length, distance = map(int, token[1:])
            if not 1 <= distance <= len(output) or distance > 0xfff:
                raise DecodeError("palette copy distance is invalid")
            if 2 <= length <= 16:
                payload.extend(((distance >> 8) << 4 | (length - 1),
                                distance & 0xff))
            elif 17 <= length <= 272:
                payload.extend(((distance >> 8) << 4,
                                distance & 0xff, length - 17))
            else:
                raise DecodeError("palette copy length is invalid")
            for _ in range(length):
                output.append(output[-distance])
            cursor += length
        encoded.append(flags)
        encoded.extend(payload)
    if not ended:
        raise DecodeError("palette plan lacks a terminator")
    if cursor != len(decoded) or bytes(output) != bytes(decoded):
        raise DecodeError("palette token plan does not reconstruct input")
    return bytes(encoded)


def decode(data, start, end, maximum, kind):
    decoders = {
        "general": decode_general,
        "palette": decode_palette,
    }
    if kind != "auto":
        output, cursor = decoders[kind](data, start, end, maximum)
        return kind, output, cursor
    valid = []
    errors = []
    for name, decoder in decoders.items():
        try:
            output, cursor = decoder(data, start, end, maximum)
            valid.append((name, output, cursor))
        except DecodeError as error:
            errors.append(f"{name}: {error}")
    if len(valid) == 1:
        return valid[0]
    if not valid:
        raise DecodeError("no decoder accepted stream (" + "; ".join(errors) + ")")
    raise DecodeError("stream is ambiguous; specify --format general or palette")


def number(text):
    try:
        return int(text, 0)
    except ValueError as error:
        raise argparse.ArgumentTypeError(f"invalid integer: {text}") from error


def put(bits, value, count):
    bits.extend((value >> index) & 1 for index in range(count))


def synthetic_general():
    bits = []
    for value in b"AB":
        put(bits, 1, 1)
        put(bits, value, 8)
    put(bits, 0, 1)
    put(bits, 0, 1)
    put(bits, 1, 1)
    put(bits, 1, 5)
    put(bits, 0, 1)
    put(bits, 0x1F, 5)
    put(bits, 0, 2)
    put(bits, 0, 7)
    packed = bytearray(b"\0")
    for offset in range(0, len(bits), 8):
        packed.append(sum(bit << index for index, bit in enumerate(bits[offset:offset + 8])))
    packed.extend(b"\0\0")
    return bytes(packed)


def self_test():
    general = synthetic_general()
    output, cursor, tokens = decode_general_trace(general, 0, len(general), 4)
    if output != b"ABAB" or cursor > len(general):
        raise AssertionError("general decoder self-test failed")
    encoded = encode_general(output, tokens)
    if not general.startswith(encoded):
        raise AssertionError("general encoder self-test failed")
    palette = bytes((0x30, ord("A"), ord("B"), 0x01, 0x02, 0, 0))
    output, cursor, groups = decode_palette_trace(
        palette, 0, len(palette), 4)
    if output != b"ABAB" or cursor != len(palette):
        raise AssertionError("palette decoder self-test failed")
    if encode_palette(output, groups) != palette:
        raise AssertionError("palette encoder self-test failed")
    for decoder, stream in ((decode_general, general[:-2]),
                            (decode_palette, palette[:-1])):
        try:
            decoder(stream, 0, len(stream), 4)
        except DecodeError:
            pass
        else:
            raise AssertionError("truncated stream was accepted")
    print("self-test=ok")


def main():
    parser = argparse.ArgumentParser(
        description="Bounded extractor for the ROM's two custom resource formats.")
    parser.add_argument("rom", type=Path, nargs="?")
    source = parser.add_mutually_exclusive_group()
    source.add_argument("--id", type=number, dest="resource_id")
    source.add_argument("--address", type=number)
    parser.add_argument(
        "--format", choices=("auto", "general", "palette"), default="auto")
    parser.add_argument("--input-end", type=number)
    parser.add_argument("--max-output", type=number, default=0x4000000)
    parser.add_argument("-o", "--output", type=Path)
    parser.add_argument("--verify-only", action="store_true")
    parser.add_argument("--self-test", action="store_true")
    args = parser.parse_args()

    if args.self_test:
        self_test()
        if args.rom is None:
            return
    if args.rom is None:
        parser.error("ROM is required unless only --self-test is used")
    if args.resource_id is None and args.address is None:
        parser.error("one of --id or --address is required")
    if args.verify_only and args.output is not None:
        parser.error("--verify-only and --output are mutually exclusive")
    if not args.verify_only and args.output is None:
        parser.error("--output is required unless --verify-only is used")
    if args.max_output < 0:
        parser.error("--max-output cannot be negative")

    data = args.rom.read_bytes()
    rom_end = ROM_BASE + len(data)
    if args.resource_id is not None:
        if args.resource_id < 0:
            parser.error("--id cannot be negative")
        entry = TABLE - ROM_BASE + args.resource_id * 4
        if entry < 0 or entry + 4 > len(data):
            parser.error("resource ID lies outside the ROM pointer table")
        address = struct.unpack_from("<I", data, entry)[0]
    else:
        address = args.address
    input_end = args.input_end if args.input_end is not None else rom_end
    if not ROM_BASE <= address < input_end <= rom_end:
        parser.error("source/input bound is outside ROM or empty")

    kind, output, cursor = decode(
        data, address - ROM_BASE, input_end - ROM_BASE,
        args.max_output, args.format)
    end_address = ROM_BASE + cursor
    if not args.verify_only:
        if args.output.resolve() == args.rom.resolve():
            parser.error("refusing to overwrite the input ROM")
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_bytes(output)
    print(
        f"format={kind} source={address:08x} end={end_address:08x} "
        f"consumed={end_address - address:#x} decoded={len(output):#x}")


if __name__ == "__main__":
    main()
