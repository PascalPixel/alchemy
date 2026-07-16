#!/usr/bin/env python3
import argparse
import json
from pathlib import Path


def coalesce(ranges):
    merged = []
    for start, end in sorted(ranges):
        if start >= end:
            continue
        if merged and start <= merged[-1][1]:
            merged[-1][1] = max(merged[-1][1], end)
        else:
            merged.append([start, end])
    return merged


def measure(ranges):
    return sum(end - start for start, end in ranges)


def intersected_bytes(ranges, start, end):
    return sum(
        max(0, min(right, end) - max(left, start))
        for left, right in ranges
        if left < end and right > start
    )


def complement(ranges, start, end):
    result = []
    cursor = start
    for left, right in ranges:
        if right <= start or left >= end:
            continue
        left = max(left, start)
        right = min(right, end)
        if cursor < left:
            result.append([cursor, left])
        cursor = max(cursor, right)
    if cursor < end:
        result.append([cursor, end])
    return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("discovery", type=Path)
    parser.add_argument("-o", "--output", type=Path, required=True)
    parser.add_argument("--claimed", type=Path)
    parser.add_argument("--bank-size", type=lambda value: int(value, 0),
                        default=0x10000)
    args = parser.parse_args()

    report = json.loads(args.discovery.read_text())
    if "instructions" not in report:
        raise ValueError("discovery report requires --details")
    rom_base = report["rom_base"]
    rom_end = rom_base + report["rom_size"]
    instruction_ranges = coalesce([
        [item["address"], item["address"] + item["size"]]
        for item in report["instructions"]
    ])
    envelope_ranges = coalesce([
        [item["min_address"], item["max_address"]]
        for item in report["functions"]
        if item["min_address"] is not None
    ])
    table_ranges = coalesce([
        [item["address"], item["address"] + 4 * len(item["targets"])]
        for item in report["pointer_tables"]
    ])
    jump_table_ranges = coalesce([
        [item["address"], item["address"] + 4 * len(item["targets"])]
        for item in report.get("jump_tables", [])
    ])
    claimed_ranges = []
    claimed_entries = []
    if args.claimed:
        claimed = json.loads(args.claimed.read_text())
        if claimed["rom_base"] != rom_base or claimed["rom_size"] != report["rom_size"]:
            raise ValueError("claimed manifest describes another ROM layout")
        claimed_entries = [item["address"] for item in claimed["regions"]]
        claimed_ranges = coalesce([
            [item["address"], item["end"]] for item in claimed["regions"]
        ])

    classified = coalesce(
        instruction_ranges + table_ranges + jump_table_ranges)
    unknown_ranges = complement(classified, rom_base, rom_end)
    functions = [item["entry"] for item in report["functions"]]
    missing_claimed_entries = sorted(set(claimed_entries) - set(functions))
    if missing_claimed_entries:
        raise ValueError(
            f"claimed entries absent from discovery: {missing_claimed_entries}")
    unresolved = report["unresolved"]
    data_refs = report["data_refs"]
    table_starts = [item["address"] for item in report["pointer_tables"]]
    jump_table_starts = [
        item["address"] for item in report.get("jump_tables", [])
    ]
    banks = []
    bank = rom_base
    while bank < rom_end:
        end = min(bank + args.bank_size, rom_end)
        summary = {
            "start": bank,
            "end": end,
            "function_entries": sum(bank <= value < end for value in functions),
            "instruction_bytes": intersected_bytes(
                instruction_ranges, bank, end),
            "function_envelope_bytes": intersected_bytes(
                envelope_ranges, bank, end),
            "pointer_tables": sum(
                bank <= value < end for value in table_starts),
            "pointer_table_bytes": intersected_bytes(
                table_ranges, bank, end),
            "jump_tables": sum(
                bank <= value < end for value in jump_table_starts),
            "jump_table_bytes": intersected_bytes(
                jump_table_ranges, bank, end),
            "data_refs": sum(bank <= value < end for value in data_refs),
            "unresolved_sites": sum(
                bank <= value < end for value in unresolved),
            "claimed_source_bytes": intersected_bytes(
                claimed_ranges, bank, end),
        }
        if any(value for key, value in summary.items()
               if key not in ("start", "end")):
            banks.append(summary)
        bank = end

    active_numbers = [
        (item["start"] - rom_base) // args.bank_size
        for item in banks
        if item["instruction_bytes"] or item["pointer_table_bytes"]
    ]
    clusters = []
    for number in active_numbers:
        if clusters and number == clusters[-1][1]:
            clusters[-1][1] += 1
        else:
            clusters.append([number, number + 1])
    active_clusters = [
        [rom_base + start * args.bank_size,
         min(rom_base + end * args.bank_size, rom_end)]
        for start, end in clusters
    ]

    layout = {
        "format": 1,
        "rom_base": rom_base,
        "rom_size": report["rom_size"],
        "bank_size": args.bank_size,
        "counts": {
            "functions": report["function_count"],
            "claimed_functions": len(claimed_entries),
            "instructions": report["instruction_count"],
            "calls": report["call_count"],
            "external_calls": report.get("external_call_count", 0),
            "unresolved": report["unresolved_count"],
            "pointer_tables": report["pointer_table_count"],
            "jump_tables": report.get("jump_table_count", 0),
            "data_refs": len(data_refs),
        },
        "bytes": {
            "instructions": measure(instruction_ranges),
            "function_envelopes": measure(envelope_ranges),
            "pointer_tables": measure(table_ranges),
            "jump_tables": measure(jump_table_ranges),
            "claimed_source": measure(claimed_ranges),
            "unclassified": measure(unknown_ranges),
        },
        "active_clusters": active_clusters,
        "instruction_ranges": instruction_ranges,
        "function_envelope_ranges": envelope_ranges,
        "pointer_table_ranges": table_ranges,
        "jump_table_ranges": jump_table_ranges,
        "claimed_source_ranges": claimed_ranges,
        "unclassified_ranges": unknown_ranges,
        "banks": banks,
    }
    args.output.write_text(json.dumps(layout, indent=2) + "\n")
    print(
        f"reported_banks={len(banks)} "
        f"code_banks={len(active_numbers)} clusters={len(active_clusters)} "
        f"instruction_bytes={layout['bytes']['instructions']} "
        f"envelope_bytes={layout['bytes']['function_envelopes']} "
        f"claimed_bytes={layout['bytes']['claimed_source']} "
        f"unclassified_bytes={layout['bytes']['unclassified']}"
    )


if __name__ == "__main__":
    main()
