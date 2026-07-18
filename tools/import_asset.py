#!/usr/bin/env python3
"""Deterministic, game-independent asset import primitives."""
import argparse
import json
import struct
import zlib
from pathlib import Path


def chunks(data, signature):
    if not data.startswith(signature):
        raise ValueError("invalid file signature")
    cursor = len(signature)
    while cursor < len(data):
        if cursor + 12 > len(data):
            raise ValueError("truncated chunk")
        size = struct.unpack_from(">I", data, cursor)[0]
        kind = data[cursor + 4:cursor + 8]
        end = cursor + 12 + size
        if end > len(data):
            raise ValueError("truncated chunk payload")
        payload = data[cursor + 8:cursor + 8 + size]
        expected = struct.unpack_from(">I", data, cursor + 8 + size)[0]
        if zlib.crc32(kind + payload) & 0xffffffff != expected:
            raise ValueError("chunk CRC mismatch")
        yield kind, payload
        cursor = end


def paeth(left, above, upper_left):
    prediction = left + above - upper_left
    choices = (abs(prediction - left), abs(prediction - above),
               abs(prediction - upper_left))
    return (left, above, upper_left)[choices.index(min(choices))]


def indexed_png(data):
    width = height = depth = None
    palette = transparency = None
    compressed = bytearray()
    for kind, payload in chunks(data, b"\x89PNG\r\n\x1a\n"):
        if kind == b"IHDR":
            if len(payload) != 13:
                raise ValueError("invalid IHDR")
            width, height, depth, color, compression, filtering, interlace = (
                struct.unpack(">IIBBBBB", payload))
            if color != 3 or depth not in (1, 2, 4, 8):
                raise ValueError("PNG must use an indexed 1/2/4/8-bit palette")
            if compression or filtering or interlace:
                raise ValueError("unsupported PNG encoding")
        elif kind == b"PLTE":
            if len(payload) % 3 or not 3 <= len(payload) <= 768:
                raise ValueError("invalid PLTE")
            palette = [tuple(payload[i:i + 3])
                       for i in range(0, len(payload), 3)]
        elif kind == b"tRNS":
            transparency = payload
        elif kind == b"IDAT":
            compressed.extend(payload)
        elif kind == b"IEND":
            break
    if None in (width, height, depth) or palette is None:
        raise ValueError("PNG lacks IHDR or PLTE")
    if width == 0 or height == 0 or width % 8 or height % 8:
        raise ValueError("PNG dimensions must be nonzero multiples of eight")
    if len(palette) > 1 << depth:
        raise ValueError("palette exceeds indexed bit depth")
    if transparency is not None and (
            len(transparency) > len(palette) or
            any(alpha not in (0, 255) for alpha in transparency)):
        raise ValueError("transparency must contain only binary alpha")
    row_size = (width * depth + 7) // 8
    raw = zlib.decompress(bytes(compressed))
    if len(raw) != height * (row_size + 1):
        raise ValueError("unexpected decompressed PNG size")
    rows = []
    cursor = 0
    previous = bytearray(row_size)
    for _ in range(height):
        method = raw[cursor]
        source = raw[cursor + 1:cursor + 1 + row_size]
        cursor += row_size + 1
        if method > 4:
            raise ValueError("invalid PNG filter")
        row = bytearray(row_size)
        for index, value in enumerate(source):
            left = row[index - 1] if index else 0
            above = previous[index]
            upper_left = previous[index - 1] if index else 0
            predictor = (0, left, above, (left + above) // 2,
                         paeth(left, above, upper_left))[method]
            row[index] = (value + predictor) & 255
        rows.append(row)
        previous = row
    pixels = []
    mask = (1 << depth) - 1
    for row in rows:
        for x in range(width):
            shift = 8 - depth - (x * depth % 8)
            index = row[x * depth // 8] >> shift & mask
            if index >= len(palette):
                raise ValueError("pixel references a missing palette entry")
            pixels.append(index)
    return width, height, pixels, palette


def rgba_png(data):
    width = height = None
    compressed = bytearray()
    for kind, payload in chunks(data, b"\x89PNG\r\n\x1a\n"):
        if kind == b"IHDR":
            if len(payload) != 13:
                raise ValueError("invalid IHDR")
            width, height, depth, color, compression, filtering, interlace = (
                struct.unpack(">IIBBBBB", payload))
            if color != 6 or depth != 8:
                raise ValueError("PNG must use 8-bit RGBA pixels")
            if compression or filtering or interlace:
                raise ValueError("unsupported PNG encoding")
        elif kind == b"IDAT":
            compressed.extend(payload)
        elif kind == b"IEND":
            break
    if width is None or height is None or width == 0 or height == 0:
        raise ValueError("PNG lacks a nonempty IHDR")
    row_size = width * 4
    raw = zlib.decompress(bytes(compressed))
    if len(raw) != height * (row_size + 1):
        raise ValueError("unexpected decompressed PNG size")
    rows = []
    cursor = 0
    previous = bytearray(row_size)
    for _ in range(height):
        method = raw[cursor]
        source = raw[cursor + 1:cursor + row_size + 1]
        cursor += row_size + 1
        if method > 4:
            raise ValueError("invalid PNG filter")
        row = bytearray(row_size)
        for index, value in enumerate(source):
            left = row[index - 4] if index >= 4 else 0
            above = previous[index]
            upper_left = previous[index - 4] if index >= 4 else 0
            predictor = (0, left, above, (left + above) // 2,
                         paeth(left, above, upper_left))[method]
            row[index] = (value + predictor) & 255
        rows.append(row)
        previous = row
    return width, height, b"".join(rows)


def gba_palette_rgba(data):
    width, height, pixels = rgba_png(data)
    palette = bytearray()
    for offset in range(0, len(pixels), 4):
        red, green, blue, alpha = pixels[offset:offset + 4]
        if red & 7 or green & 7 or blue & 7:
            raise ValueError("RGBA palette colors must be multiples of eight")
        if alpha not in (254, 255):
            raise ValueError("RGBA palette alpha must be 254 or 255")
        value = ((red >> 3) | (green >> 3) << 5 |
                 (blue >> 3) << 10 | (255 - alpha) << 15)
        palette.extend(struct.pack("<H", value))
    return bytes(palette), {"width": width, "height": height,
                            "palette_entries": len(palette) // 2}


def gba_graphics(data, bpp):
    width, height, pixels, palette = indexed_png(data)
    limit = 16 if bpp == 4 else 256
    if len(palette) > limit or any(pixel >= limit for pixel in pixels):
        raise ValueError(f"image does not fit {bpp}bpp")
    colors = bytearray()
    for red, green, blue in palette:
        if red & 7 or green & 7 or blue & 7:
            raise ValueError(
                "palette channels must be exact five-bit values (multiples of 8)")
        value = (red >> 3) | (green >> 3) << 5 | (blue >> 3) << 10
        colors.extend(struct.pack("<H", value))
    tiles = bytearray()
    for top in range(0, height, 8):
        for left in range(0, width, 8):
            tile = [pixels[(top + y) * width + left + x]
                    for y in range(8) for x in range(8)]
            if bpp == 4:
                tiles.extend(tile[i] | tile[i + 1] << 4
                             for i in range(0, 64, 2))
            else:
                tiles.extend(tile)
    return bytes(tiles), bytes(colors), {
        "width": width, "height": height, "bpp": bpp,
        "tiles": width // 8 * (height // 8),
        "palette_entries": len(palette),
    }


def vlq(data, cursor):
    value = 0
    for _ in range(4):
        if cursor >= len(data):
            raise ValueError("truncated variable-length quantity")
        byte = data[cursor]
        cursor += 1
        value = value << 7 | byte & 0x7f
        if byte < 0x80:
            return value, cursor
    raise ValueError("overlong variable-length quantity")


def midi_events(data):
    parsed = []
    cursor = 0
    while cursor < len(data):
        if cursor + 8 > len(data):
            raise ValueError("truncated MIDI chunk")
        kind = data[cursor:cursor + 4]
        size = struct.unpack_from(">I", data, cursor + 4)[0]
        cursor += 8
        payload = data[cursor:cursor + size]
        cursor += size
        if len(payload) != size:
            raise ValueError("truncated MIDI chunk payload")
        parsed.append((kind, payload))
    if not parsed or parsed[0][0] != b"MThd" or len(parsed[0][1]) != 6:
        raise ValueError("invalid MIDI header")
    form, tracks, division = struct.unpack(">HHH", parsed[0][1])
    if form not in (0, 1) or tracks == 0 or division & 0x8000:
        raise ValueError("only format 0/1 PPQN MIDI is supported")
    track_chunks = [payload for kind, payload in parsed[1:] if kind == b"MTrk"]
    if len(track_chunks) != tracks:
        raise ValueError("MIDI track count mismatch")
    result = []
    lengths = {0xC0: 1, 0xD0: 1}
    for track, payload in enumerate(track_chunks):
        cursor = tick = order = 0
        running = None
        while cursor < len(payload):
            delta, cursor = vlq(payload, cursor)
            tick += delta
            if cursor >= len(payload):
                raise ValueError("truncated MIDI event")
            status = payload[cursor]
            if status < 0x80:
                if running is None:
                    raise ValueError("running status without channel status")
                status = running
            else:
                cursor += 1
            event = {"tick": tick, "track": track, "order": order}
            order += 1
            if status == 0xff:
                if cursor >= len(payload):
                    raise ValueError("truncated meta event")
                kind = payload[cursor]
                size, cursor = vlq(payload, cursor + 1)
                value = payload[cursor:cursor + size]
                cursor += size
                if len(value) != size:
                    raise ValueError("truncated meta payload")
                event.update(type="meta", meta=kind, data=value.hex())
                running = None
            elif status in (0xf0, 0xf7):
                size, cursor = vlq(payload, cursor)
                value = payload[cursor:cursor + size]
                cursor += size
                if len(value) != size:
                    raise ValueError("truncated system-exclusive payload")
                event.update(type="sysex", status=status, data=value.hex())
                running = None
            elif 0x80 <= status <= 0xef:
                running = status
                size = lengths.get(status & 0xf0, 2)
                value = payload[cursor:cursor + size]
                cursor += size
                if len(value) != size or any(byte & 0x80 for byte in value):
                    raise ValueError("invalid channel event")
                event.update(type="channel", status=status,
                             data=list(value))
            else:
                raise ValueError("unsupported MIDI system event")
            result.append(event)
    result.sort(key=lambda item: (item["tick"], item["track"], item["order"]))
    return {"format": form, "tracks": tracks, "ticks_per_quarter": division,
            "events": result}


def make_chunk(kind, payload):
    return (struct.pack(">I", len(payload)) + kind + payload +
            struct.pack(">I", zlib.crc32(kind + payload) & 0xffffffff))


def make_midi_chunk(kind, payload):
    return kind + struct.pack(">I", len(payload)) + payload


def self_test():
    palette = bytes((0, 0, 0, 248, 0, 0))
    rows = b"".join(b"\0" + bytes((0x55,)) for _ in range(8))
    png = (b"\x89PNG\r\n\x1a\n" +
           make_chunk(b"IHDR", struct.pack(">IIBBBBB", 8, 8, 1, 3, 0, 0, 0)) +
           make_chunk(b"PLTE", palette) +
           make_chunk(b"IDAT", zlib.compress(rows)) +
           make_chunk(b"IEND", b""))
    tiles, colors, report = gba_graphics(png, 4)
    assert tiles == bytes((0x10,)) * 32
    assert colors == b"\0\0\x1f\0" and report["tiles"] == 1
    track = b"\0\xc0\x05\0\x90\x3c\x40\x60\x80\x3c\0\0\xff\x2f\0"
    midi = (make_midi_chunk(b"MThd", struct.pack(">HHH", 0, 1, 96)) +
            make_midi_chunk(b"MTrk", track))
    report = midi_events(midi)
    assert report["ticks_per_quarter"] == 96
    assert [event["tick"] for event in report["events"]] == [0, 0, 96, 96]
    rgba = bytes((index * 29 + 3) & 255 for index in range(7 * 5 * 4))
    from export_asset import rgba_image
    image, _ = rgba_image(rgba, 7)
    assert rgba_png(image) == (7, 5, rgba)
    raw_palette = b"".join(struct.pack("<H", (index * 109) & 0xffff)
                           for index in range(128))
    from export_asset import palette_rgba_image
    image, _ = palette_rgba_image(raw_palette, 16)
    assert gba_palette_rgba(image)[0] == raw_palette
    print("self-test=ok")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--self-test", action="store_true")
    commands = parser.add_subparsers(dest="command")
    png = commands.add_parser("png")
    png.add_argument("input", type=Path)
    png.add_argument("--bpp", type=int, choices=(4, 8), required=True)
    png.add_argument("--tiles", type=Path, required=True)
    png.add_argument("--palette", type=Path, required=True)
    indexed = commands.add_parser("indexed")
    indexed.add_argument("input", type=Path)
    indexed.add_argument("-o", "--output", type=Path, required=True)
    midi = commands.add_parser("midi")
    midi.add_argument("input", type=Path)
    midi.add_argument("-o", "--output", type=Path, required=True)
    args = parser.parse_args()
    if args.self_test:
        self_test()
        if args.command is None:
            return
    if args.command == "png":
        tiles, palette, report = gba_graphics(args.input.read_bytes(), args.bpp)
        args.tiles.parent.mkdir(parents=True, exist_ok=True)
        args.palette.parent.mkdir(parents=True, exist_ok=True)
        args.tiles.write_bytes(tiles)
        args.palette.write_bytes(palette)
        print(json.dumps(report, sort_keys=True))
    elif args.command == "indexed":
        width, height, pixels, palette = indexed_png(args.input.read_bytes())
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_bytes(bytes(pixels))
        print(json.dumps({"width": width, "height": height,
                          "pixels": len(pixels),
                          "palette_entries": len(palette)}, sort_keys=True))
    elif args.command == "midi":
        report = midi_events(args.input.read_bytes())
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(report, indent=2) + "\n")
        print(f"tracks={report['tracks']} events={len(report['events'])}")
    else:
        parser.error("an asset command is required")


if __name__ == "__main__":
    main()
