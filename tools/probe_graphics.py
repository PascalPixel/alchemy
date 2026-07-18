#!/usr/bin/env python3
import argparse
import json
import math
import struct
import zlib
from collections import Counter
from pathlib import Path

from generated_files import prune_files

ROM_BASE = 0x08000000
KINDS = {0x10: "lz77", 0x30: "rle"}
COMPRESSION_SWIS = {
    0x11: "lz77_wram",
    0x12: "lz77_vram",
    0x13: "huffman",
    0x14: "rle_wram",
    0x15: "rle_vram",
}


def u32(data, offset):
    return struct.unpack_from("<I", data, offset)[0]


def decoded_size(data, offset):
    if offset + 4 > len(data):
        return 0
    return data[offset + 1] | data[offset + 2] << 8 | data[offset + 3] << 16


def decode_lz77(data, offset, maximum):
    size = decoded_size(data, offset)
    if data[offset] != 0x10 or not 1 <= size <= maximum:
        return None
    cursor = offset + 4
    output = bytearray()
    while len(output) < size:
        if cursor >= len(data):
            return None
        flags = data[cursor]
        cursor += 1
        for bit in range(7, -1, -1):
            if len(output) >= size:
                break
            if flags & (1 << bit):
                if cursor + 2 > len(data):
                    return None
                first, second = data[cursor:cursor + 2]
                cursor += 2
                length = (first >> 4) + 3
                distance = ((first & 15) << 8 | second) + 1
                if distance > len(output):
                    return None
                for _ in range(length):
                    output.append(output[-distance])
                    if len(output) == size:
                        break
            else:
                if cursor >= len(data):
                    return None
                output.append(data[cursor])
                cursor += 1
    return bytes(output), cursor - offset


def decode_rle(data, offset, maximum):
    size = decoded_size(data, offset)
    if data[offset] != 0x30 or not 1 <= size <= maximum:
        return None
    cursor = offset + 4
    output = bytearray()
    while len(output) < size:
        if cursor >= len(data):
            return None
        control = data[cursor]
        cursor += 1
        if control & 0x80:
            count = (control & 0x7f) + 3
            if cursor >= len(data):
                return None
            output.extend(data[cursor:cursor + 1] * count)
            cursor += 1
        else:
            count = (control & 0x7f) + 1
            if cursor + count > len(data):
                return None
            output.extend(data[cursor:cursor + count])
            cursor += count
        if len(output) > size:
            output = output[:size]
    return bytes(output), cursor - offset


def validate_huffman(data, offset, maximum):
    kind = data[offset]
    bits = kind & 15
    size = decoded_size(data, offset)
    if kind >> 4 != 2 or bits not in (4, 8) or not 1 <= size <= maximum:
        return None
    if offset + 6 > len(data):
        return None
    tree_bytes = (data[offset + 4] + 1) * 2
    tree_start = offset + 5
    tree_end = tree_start + tree_bytes
    stream = (tree_end + 3) & ~3
    if tree_start >= tree_end or stream + 4 > len(data):
        return None
    symbols = size if bits == 8 else size * 2
    cursor = stream
    word = 0
    remaining = 0
    node = tree_start
    leaves = 0
    steps = 0
    while leaves < symbols:
        if remaining == 0:
            if cursor + 4 > len(data):
                return None
            word = u32(data, cursor)
            cursor += 4
            remaining = 32
        direction = (word >> 31) & 1
        word = word << 1 & 0xffffffff
        remaining -= 1
        if not tree_start <= node < tree_end:
            return None
        descriptor = data[node]
        child = (node & ~1) + ((descriptor & 0x3f) + 1) * 2 + direction
        if not tree_start <= child < tree_end:
            return None
        leaf = descriptor & (0x80 if direction == 0 else 0x40)
        if leaf:
            leaves += 1
            node = tree_start
        else:
            node = child
        steps += 1
        if steps > symbols * 64:
            return None
    return {
        "decoded_size": size,
        "consumed_size": cursor - offset,
        "symbol_bits": bits,
        "tree_bytes": tree_bytes,
    }


def parse_stream(data, offset, maximum, keep_output=False):
    if not 0 <= offset < len(data):
        return None
    kind = data[offset]
    if kind == 0x10:
        result = decode_lz77(data, offset, maximum)
    elif kind == 0x30:
        result = decode_rle(data, offset, maximum)
    elif kind >> 4 == 2:
        return validate_huffman(data, offset, maximum)
    else:
        return None
    if result is None:
        return None
    output, consumed = result
    report = {
        "decoded_size": len(output),
        "consumed_size": consumed,
    }
    if keep_output:
        report["output"] = output
    return report


def pointer_targets(data):
    result = Counter()
    end = ROM_BASE + len(data)
    for offset in range(0, len(data) - 3, 4):
        value = u32(data, offset)
        if ROM_BASE <= value < end:
            result[value - ROM_BASE] += 1
    return result


def instruction_mask(data, discovery):
    mask = bytearray(len(data))
    instruction_addresses = []
    if discovery is None:
        return mask, instruction_addresses
    report = json.loads(discovery.read_text())
    for item in report.get("instructions", []):
        offset = item["address"] - ROM_BASE
        size = item["size"]
        if 0 <= offset < len(data) and offset + size <= len(data):
            mask[offset:offset + size] = b"\1" * size
            instruction_addresses.append((offset, item.get("mode"), size))
    return mask, instruction_addresses


def swi_calls(data, instructions):
    calls = []
    for offset, mode, size in instructions:
        if mode == "thumb" and size == 2:
            half = struct.unpack_from("<H", data, offset)[0]
            if half & 0xff00 == 0xdf00 and (half & 0xff) in COMPRESSION_SWIS:
                immediate = half & 0xff
                calls.append({
                    "address": ROM_BASE + offset,
                    "swi": immediate,
                    "operation": COMPRESSION_SWIS[immediate],
                    "mode": mode,
                })
        elif mode == "arm" and size == 4:
            word = u32(data, offset)
            immediate = word & 0xffffff
            number = immediate >> 16 if immediate & 0xffff == 0 else immediate
            if word >> 24 & 15 == 15 and number in COMPRESSION_SWIS:
                calls.append({
                    "address": ROM_BASE + offset,
                    "swi": number,
                    "operation": COMPRESSION_SWIS[number],
                    "mode": mode,
                })
    return calls


def palette_score(data, offset, colors):
    size = colors * 2
    if offset < 0 or offset + size > len(data):
        return None
    values = struct.unpack_from(f"<{colors}H", data, offset)
    legal = sum(value < 0x8000 for value in values) / colors
    if legal < 0.97:
        return None
    unique = len(set(values))
    if unique < min(8, colors // 2):
        return None
    channels = [
        component
        for value in values
        for component in (value & 31, value >> 5 & 31, value >> 10 & 31)
    ]
    span = max(channels) - min(channels)
    if span < 12:
        return None
    score = legal * 4 + min(unique / min(colors, 64), 1) + span / 31
    if values[0] in (0, 0x7fff):
        score += 0.25
    return round(score, 4)


def tile_metrics(raw, bits):
    unit = 32 if bits == 4 else 64
    tiles = len(raw) // unit
    if tiles == 0:
        return None
    raw = raw[:tiles * unit]
    if bits == 4:
        pixels = [value for byte in raw for value in (byte & 15, byte >> 4)]
    else:
        pixels = list(raw)
    equal = 0
    edges = 0
    nonempty = 0
    distinct_total = 0
    for tile in range(tiles):
        block = pixels[tile * 64:(tile + 1) * 64]
        if any(block):
            nonempty += 1
        distinct_total += len(set(block))
        for y in range(8):
            for x in range(8):
                index = y * 8 + x
                if x:
                    equal += block[index] == block[index - 1]
                    edges += 1
                if y:
                    equal += block[index] == block[index - 8]
                    edges += 1
    baseline = 1 / (16 if bits == 4 else 256)
    adjacency = equal / edges
    zero = pixels.count(0) / len(pixels)
    score = max(0, adjacency - baseline) * 4 * (1 - zero)
    score += min(zero, 0.4)
    score += min(nonempty / tiles, 0.5)
    if nonempty == 0:
        score = 0
    return {
        "tiles": tiles,
        "zero_fraction": round(zero, 4),
        "adjacent_equal_fraction": round(adjacency, 4),
        "mean_distinct_indices": round(distinct_total / tiles, 3),
        "nonempty_fraction": round(nonempty / tiles, 4),
        "score": round(score, 4),
    }


def png_chunk(kind, payload):
    body = kind + payload
    return struct.pack(">I", len(payload)) + body + struct.pack(
        ">I", zlib.crc32(body) & 0xffffffff)


def write_index_sheet(path, raw, bits, maximum_tiles, scale):
    unit = 32 if bits == 4 else 64
    count = min(len(raw) // unit, maximum_tiles)
    columns = min(16, count)
    rows = math.ceil(count / columns)
    width, height = columns * 8, rows * 8
    pixels = bytearray(width * height)
    for tile in range(count):
        chunk = raw[tile * unit:(tile + 1) * unit]
        values = (
            [value for byte in chunk for value in (byte & 15, byte >> 4)]
            if bits == 4 else list(chunk)
        )
        left = tile % columns * 8
        top = tile // columns * 8
        for y in range(8):
            pixels[(top + y) * width + left:(top + y) * width + left + 8] = (
                bytes(values[y * 8:y * 8 + 8]))
    scaled_width, scaled_height = width * scale, height * scale
    rows_data = bytearray()
    denominator = 15 if bits == 4 else 255
    for y in range(height):
        row = bytearray()
        for value in pixels[y * width:(y + 1) * width]:
            shade = value * 255 // denominator
            row.extend((shade, shade, shade) * scale)
        scan = b"\0" + bytes(row)
        rows_data.extend(scan * scale)
    ihdr = struct.pack(">IIBBBBB", scaled_width, scaled_height, 8, 2, 0, 0, 0)
    path.write_bytes(
        b"\x89PNG\r\n\x1a\n" +
        png_chunk(b"IHDR", ihdr) +
        png_chunk(b"IDAT", zlib.compress(bytes(rows_data), 9)) +
        png_chunk(b"IEND", b""))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument("--discovery", type=Path)
    parser.add_argument("-o", "--output", type=Path,
                        default=Path("work/graphics-probe/report.json"))
    parser.add_argument("--png-dir", type=Path)
    parser.add_argument("--max-decoded", type=lambda value: int(value, 0),
                        default=0x400000)
    parser.add_argument("--control-max", type=lambda value: int(value, 0),
                        default=0x10000)
    parser.add_argument("--previews", type=int, default=16)
    parser.add_argument("--preview-tiles", type=int, default=256)
    parser.add_argument("--scale", type=int, default=2)
    parser.add_argument("--raw-window", type=lambda value: int(value, 0),
                        default=0x800)
    parser.add_argument("--raw-step", type=lambda value: int(value, 0),
                        default=0x800)
    parser.add_argument("--raw-candidates", type=int, default=32)
    args = parser.parse_args()

    data = args.rom.read_bytes()
    pointers = pointer_targets(data)
    code, instructions = instruction_mask(data, args.discovery)
    headers = Counter()
    for offset in range(0, len(data) - 4, 4):
        byte = data[offset]
        kind = KINDS.get(byte)
        if kind is None and byte >> 4 == 2 and byte & 15 in (4, 8):
            kind = "huffman"
        if kind is not None:
            headers[kind] += 1

    valid = []
    for offset, references in sorted(pointers.items()):
        parsed = parse_stream(data, offset, args.max_decoded, keep_output=True)
        if parsed is None:
            continue
        byte = data[offset]
        kind = KINDS.get(byte, "huffman")
        output = parsed.pop("output", None)
        item = {
            "address": ROM_BASE + offset,
            "kind": kind,
            **parsed,
            "pointer_references": references,
            "inside_discovered_code": bool(code[offset]),
        }
        if output is not None:
            item["tile_4bpp"] = tile_metrics(output, 4)
            item["tile_8bpp"] = tile_metrics(output, 8)
            item["_output"] = output
        valid.append(item)

    controls = {}
    targets = sorted(pointers)
    for shift in (1, 2, 4, 16, 256, 4096):
        counts = Counter()
        tested = 0
        for target in targets:
            offset = target + shift
            if offset >= len(data):
                continue
            tested += 1
            parsed = parse_stream(data, offset, args.control_max)
            if parsed is not None:
                byte = data[offset]
                kind = KINDS.get(byte, "huffman")
                counts[kind] += 1
        controls[str(shift)] = {"tested": tested, "valid": dict(counts)}

    palette_candidates = []
    palette_controls = Counter()
    for offset, references in pointers.items():
        if code[offset]:
            continue
        for colors in (16, 256):
            score = palette_score(data, offset, colors)
            if score is not None:
                palette_candidates.append({
                    "address": ROM_BASE + offset,
                    "colors": colors,
                    "score": score,
                    "pointer_references": references,
                })
            for shift in (1, 2, 4, 16, 256):
                if palette_score(data, offset + shift, colors) is not None:
                    palette_controls[f"{colors}:{shift}"] += 1
    palette_candidates.sort(
        key=lambda item: (-item["score"], -item["pointer_references"],
                          item["address"]))
    palette_candidate_counts = dict(Counter(
        item["colors"] for item in palette_candidates))
    palette_candidates = palette_candidates[:128]

    raw_by_bits = {}
    for bits in (4, 8):
        candidates = []
        for offset in range(0, len(data) - args.raw_window + 1,
                            args.raw_step):
            raw = data[offset:offset + args.raw_window]
            if sum(code[offset:offset + args.raw_window]) > args.raw_window // 10:
                continue
            metrics = tile_metrics(raw, bits)
            shifted = tile_metrics(
                data[offset + 1:offset + 1 + args.raw_window], bits)
            if metrics is None or shifted is None:
                continue
            if (metrics["zero_fraction"] >= 0.98 or
                    metrics["nonempty_fraction"] < 0.1 or
                    metrics["mean_distinct_indices"] < 1.5):
                continue
            candidates.append({
                "address": ROM_BASE + offset,
                **metrics,
                "alignment_advantage": round(
                    metrics["score"] - shifted["score"], 4),
                "pointer_references": pointers[offset],
            })
        candidates.sort(key=lambda item: (
            -item["score"], -item["alignment_advantage"],
            -item["pointer_references"], item["address"]))
        raw_by_bits[str(bits)] = candidates[:args.raw_candidates]

    previewable = [
        item for item in valid
        if item.get("_output") and item["pointer_references"] and
        not item["inside_discovered_code"]
    ]
    previewable.sort(key=lambda item: (
        -max(
            (item.get("tile_4bpp") or {}).get("score", 0),
            (item.get("tile_8bpp") or {}).get("score", 0)),
        item["address"],
    ))
    preview_files = []
    if args.png_dir:
        args.png_dir.mkdir(parents=True, exist_ok=True)
        for item in previewable[:args.previews]:
            raw = item["_output"]
            for bits in (4, 8):
                metrics = item[f"tile_{bits}bpp"]
                if metrics is None:
                    continue
                name = f"{item['address']:08x}_{item['kind']}_{bits}bpp-index.png"
                write_index_sheet(
                    args.png_dir / name, raw, bits,
                    args.preview_tiles, args.scale)
                preview_files.append(name)
        for bits in (4, 8):
            for item in raw_by_bits[str(bits)][:args.previews]:
                offset = item["address"] - ROM_BASE
                name = f"{item['address']:08x}_raw_{bits}bpp-index.png"
                write_index_sheet(
                    args.png_dir / name,
                    data[offset:offset + args.raw_window], bits,
                    args.preview_tiles, args.scale)
                preview_files.append(name)

    for item in valid:
        item.pop("_output", None)
    credible_tiled_streams = [
        {
            "address": item["address"],
            "kind": item["kind"],
            "decoded_size": item["decoded_size"],
            "consumed_size": item["consumed_size"],
        }
        for item in valid
        if item["address"] % 4 == 0
        and not item["inside_discovered_code"]
        and item["consumed_size"] < item["decoded_size"]
        and item["decoded_size"] % 32 == 0
    ]
    report = {
        "format": 1,
        "rom_base": ROM_BASE,
        "rom_size": len(data),
        "method": {
            "compression": (
                "BIOS-format headers at ROM pointer targets accepted only "
                "after complete bounded stream validation"),
            "pointer_support": (
                "aligned little-endian ROM addresses found elsewhere in ROM"),
            "palette": (
                "heuristic BGR555 legality, diversity, and channel span; "
                "not asset identification"),
            "png": (
                "grayscale pixel-index tile sheets; no palette or tilemap "
                "association is claimed"),
            "credible_tiled_stream": (
                "validated stream, four-byte header alignment, outside "
                "discovered code, positive compression, decoded size a "
                "multiple of one 4bpp tile"),
            "controls": (
                "the same validators at fixed offsets from every ROM pointer "
                f"target, bounded to {args.control_max} decoded bytes"),
        },
        "rom_pointer_targets": len(pointers),
        "aligned_header_counts": dict(headers),
        "valid_stream_counts": dict(Counter(
            item["kind"] for item in valid)),
        "pointer_supported_stream_counts": dict(Counter(
            item["kind"] for item in valid if item["pointer_references"])),
        "compression_swi_calls": swi_calls(data, instructions),
        "compression_controls": controls,
        "valid_streams": valid,
        "credible_tiled_streams": credible_tiled_streams,
        "palette_candidate_counts": palette_candidate_counts,
        "palette_candidates": palette_candidates,
        "palette_control_counts": dict(palette_controls),
        "raw_tile_windows": raw_by_bits,
        "preview_files": preview_files,
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n")
    if args.png_dir:
        removed = prune_files(args.png_dir, "*-index.png", preview_files)
        if removed:
            print(f"pruned stale previews={len(removed)}")
    print(
        f"pointers={len(pointers)} headers={sum(headers.values())} "
        f"valid={len(valid)} supported="
        f"{sum(bool(item['pointer_references']) for item in valid)} "
        f"palettes={len(palette_candidates)} previews={len(preview_files)}")


if __name__ == "__main__":
    main()
