#!/usr/bin/env python3
import argparse
import json
import struct
from pathlib import Path

ROM_BASE = 0x08000000


def coalesce(ranges):
    result = []
    for start, end in sorted(ranges):
        if start >= end:
            continue
        if result and start <= result[-1][1]:
            result[-1][1] = max(result[-1][1], end)
        else:
            result.append([start, end])
    return result


def intersections(ranges, start, end):
    return sum(max(0, min(end, right) - max(start, left))
               for left, right in ranges if left < end and start < right)


def measure(ranges):
    return sum(end - start for start, end in coalesce(ranges))


def u16(data, offset):
    return struct.unpack_from("<H", data, offset)[0]


def u32(data, offset):
    return struct.unpack_from("<I", data, offset)[0]


def addresses(report, key, width):
    return [[item["address"], item["address"] + width(item)]
            for item in report.get(key, [])]


def literal_slots(data, report):
    slots = {}
    for item in report.get("instructions", []):
        address = item["address"]
        offset = address - ROM_BASE
        literal = None
        if item["mode"] == "thumb" and item["size"] == 2:
            half = u16(data, offset)
            if half & 0xF800 == 0x4800:
                literal = ((address + 4) & ~3) + ((half & 0xFF) << 2)
        elif item["mode"] == "arm" and item["size"] == 4:
            word = u32(data, offset)
            load = bool(word & (1 << 20))
            immediate = not bool(word & (1 << 25))
            preindexed = bool(word & (1 << 24))
            writeback = bool(word & (1 << 21))
            if ((word >> 26) & 3) == 1 and load and immediate:
                if (word >> 16) & 15 == 15 and preindexed and not writeback:
                    literal = address + 8
                    displacement = word & 0xFFF
                    literal += displacement if word & (1 << 23) else -displacement
        if literal is not None and ROM_BASE <= literal <= ROM_BASE + len(data) - 4:
            slots.setdefault(literal, []).append(address)
    ranges = coalesce([[slot, slot + 4] for slot in slots])
    return slots, ranges


def fill_runs(data, minimum, occupied):
    result = []
    cursor = 0
    while cursor < len(data):
        value = data[cursor]
        if value not in (0, 0xFF):
            cursor += 1
            continue
        end = cursor + 1
        while end < len(data) and data[end] == value:
            end += 1
        if end - cursor >= minimum:
            start_address = ROM_BASE + cursor
            end_address = ROM_BASE + end
            conflict = intersections(occupied, start_address, end_address)
            if conflict:
                classification = "fill_with_structural_overlap"
            elif end == len(data):
                classification = "terminal_padding"
            else:
                classification = "fill_candidate"
            result.append({
                "start": start_address,
                "end": end_address,
                "size": end - cursor,
                "fill": "zero" if value == 0 else "ff",
                "occupied_overlap_bytes": conflict,
                "classification": classification,
            })
        cursor = end
    return result


def pointer_runs(data, report, instruction_ranges, known_ranges, minimum):
    functions = {item["entry"] for item in report.get("functions", [])}
    values = []
    for offset in range(0, len(data) - 3, 4):
        value = u32(data, offset)
        target = value & ~1
        values.append(ROM_BASE <= target < ROM_BASE + len(data))
    runs = []
    cursor = 0
    while cursor < len(values):
        if not values[cursor]:
            cursor += 1
            continue
        end = cursor + 1
        while end < len(values) and values[end]:
            end += 1
        if end - cursor >= minimum:
            start_address = ROM_BASE + cursor * 4
            end_address = ROM_BASE + end * 4
            thumb = 0
            functions_hit = 0
            for index in range(cursor, end):
                value = u32(data, index * 4)
                thumb += bool(value & 1)
                functions_hit += (value & ~1) in functions
            code_overlap = intersections(
                instruction_ranges, start_address, end_address)
            known_overlap = intersections(
                known_ranges, start_address, end_address)
            runs.append({
                "start": start_address,
                "end": end_address,
                "entries": end - cursor,
                "thumb_targets": thumb,
                "known_function_targets": functions_hit,
                "instruction_overlap_bytes": code_overlap,
                "known_table_overlap_bytes": known_overlap,
                "classification": (
                    "known_table" if known_overlap == end_address - start_address
                    else "pointer_candidate" if code_overlap == 0
                    else "address_like_code"),
            })
        cursor = end
    return runs


def header_report(data):
    reset = u32(data, 0)
    reset_branch = reset >> 24 == 0xEA
    destination = ROM_BASE + 8
    displacement = reset & 0xFFFFFF
    if displacement & 0x800000:
        displacement -= 0x1000000
    destination += displacement << 2
    checksum = (-(sum(data[0xA0:0xBD]) + 0x19)) & 0xFF
    return {
        "start": ROM_BASE,
        "end": ROM_BASE + 0xC0,
        "size": 0xC0,
        "reset_is_arm_branch": reset_branch,
        "reset_target_in_rom": ROM_BASE <= destination < ROM_BASE + len(data),
        "fixed_value_valid": data[0xB2] == 0x96,
        "complement_checksum_valid": checksum == data[0xBD],
        "reserved_ranges_zero": (
            all(value == 0 for value in data[0xB5:0xBC]) and
            all(value == 0 for value in data[0xBE:0xC0])),
    }


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    parser.add_argument("discovery", type=Path)
    parser.add_argument("-o", "--output", type=Path, required=True)
    parser.add_argument("--minimum-fill", type=lambda value: int(value, 0),
                        default=32)
    parser.add_argument("--minimum-pointers", type=int, default=4)
    args = parser.parse_args()

    data = args.rom.read_bytes()
    report = json.loads(args.discovery.read_text())
    if report.get("rom_base") != ROM_BASE or report.get("rom_size") != len(data):
        raise ValueError("discovery report describes another ROM")
    if not report.get("instructions"):
        raise ValueError("discovery report requires --details")

    instruction_ranges = coalesce(addresses(
        report, "instructions", lambda item: item["size"]))
    pointer_table_ranges = coalesce(addresses(
        report, "pointer_tables", lambda item: 4 * len(item["targets"])))
    jump_table_ranges = coalesce(addresses(
        report, "jump_tables", lambda item: 4 * len(item["targets"])))
    slots, literal_ranges = literal_slots(data, report)
    known_ranges = coalesce(pointer_table_ranges + jump_table_ranges)
    occupied = coalesce(
        instruction_ranges + known_ranges + literal_ranges +
        [[ROM_BASE, ROM_BASE + 0xC0]])
    fills = fill_runs(data, args.minimum_fill, occupied)
    pointers = pointer_runs(
        data, report, instruction_ranges, known_ranges,
        args.minimum_pointers)

    known_conflicts = []
    for kind, ranges in (
            ("pointer_table", pointer_table_ranges),
            ("jump_table", jump_table_ranges),
            ("literal_pool", literal_ranges)):
        for start, end in ranges:
            overlap = intersections(instruction_ranges, start, end)
            if overlap:
                known_conflicts.append({
                    "kind": kind, "start": start, "end": end,
                    "instruction_overlap_bytes": overlap,
                })

    fill_candidates = [[item["start"], item["end"]] for item in fills
                       if item["classification"] == "fill_candidate"]
    terminal_padding = [[item["start"], item["end"]] for item in fills
                        if item["classification"] == "terminal_padding"]
    header = header_report(data)
    result = {
        "format": 1,
        "rom_base": ROM_BASE,
        "rom_size": len(data),
        "header": header,
        "header_valid": all((
            header["reset_is_arm_branch"],
            header["reset_target_in_rom"],
            header["fixed_value_valid"],
            header["complement_checksum_valid"],
            header["reserved_ranges_zero"],
        )),
        "counts": {
            "fill_runs": len(fills),
            "fill_candidates": len(fill_candidates),
            "terminal_padding_runs": len(terminal_padding),
            "pointer_candidates": sum(
                item["classification"] == "pointer_candidate"
                for item in pointers),
            "address_like_code_runs": sum(
                item["classification"] == "address_like_code"
                for item in pointers),
            "known_pointer_tables": len(report.get("pointer_tables", [])),
            "known_jump_tables": len(report.get("jump_tables", [])),
            "literal_slots": len(slots),
            "literal_pool_ranges": len(literal_ranges),
            "structural_conflicts": len(known_conflicts),
        },
        "bytes": {
            "instructions": measure(instruction_ranges),
            "known_pointer_tables": measure(pointer_table_ranges),
            "known_jump_tables": measure(jump_table_ranges),
            "literal_pools": measure(literal_ranges),
            "fill_candidates": measure(fill_candidates),
            "terminal_padding": measure(terminal_padding),
        },
        "fill_runs": fills,
        "pointer_runs": pointers,
        "known_pointer_table_ranges": pointer_table_ranges,
        "known_jump_table_ranges": jump_table_ranges,
        "literal_pool_ranges": [
            {"start": start, "end": end, "size": end - start,
             "slots": (end - start) // 4,
             "references": sum(len(sites) for slot, sites in slots.items()
                               if start <= slot < end)}
            for start, end in literal_ranges
        ],
        "structural_conflicts": known_conflicts,
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n")
    print(
        f"header_valid={result['header_valid']} "
        f"fill_candidates={result['counts']['fill_candidates']} "
        f"terminal_padding={result['bytes']['terminal_padding']} "
        f"pointer_candidates={result['counts']['pointer_candidates']} "
        f"literal_slots={result['counts']['literal_slots']} "
        f"conflicts={result['counts']['structural_conflicts']}")


if __name__ == "__main__":
    main()
