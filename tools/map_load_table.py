#!/usr/bin/env python3
"""Exact semantic converter for the map graphics load table (マップ読込表)."""
import argparse
import json
import struct
from pathlib import Path


ROM_BASE = 0x08000000
TABLE_ADDRESS = 0x08013784
RESOURCE_BASE = 0x128
RECORDS = 186
FIELDS = (
    "container",
    "palette",
    "vram_charblock1",
    "vram_charblock2",
    "vram_charblock3",
    "animation_source",
)


def export_table(rom, source):
    start = TABLE_ADDRESS - ROM_BASE
    rows = []
    for map_index in range(RECORDS):
        values = struct.unpack_from("<6H", rom, start + map_index * 12)
        rows.append({
            "map_index": map_index,
            **{field: f"{value + RESOURCE_BASE:x}"
               for field, value in zip(FIELDS, values)},
        })
    document = {
        "format": 1,
        "resource_base": f"0x{RESOURCE_BASE:x}",
        "records": rows,
    }
    source.parent.mkdir(parents=True, exist_ok=True)
    source.write_text(json.dumps(document, indent=2) + "\n")
    rebuilt = build_table(source)
    if rebuilt != rom[start:start + RECORDS * 12]:
        raise AssertionError("map graphics load table does not round-trip")
    return len(rows)


def build_table(source):
    document = json.loads(source.read_text())
    if document.get("format") != 1:
        raise ValueError("unsupported map graphics load table")
    resource_base = int(document.get("resource_base", "0"), 0)
    if resource_base != RESOURCE_BASE:
        raise ValueError("map graphics load table has the wrong resource base")
    rows = document.get("records", [])
    if len(rows) != RECORDS:
        raise ValueError(f"map graphics load table requires {RECORDS} records")
    encoded = bytearray()
    for map_index, row in enumerate(rows):
        if int(row.get("map_index", -1)) != map_index:
            raise ValueError("map graphics load table indices must be sequential")
        values = [int(row[field], 16) - RESOURCE_BASE for field in FIELDS]
        if any(not 0 <= value <= 0xffff for value in values):
            raise ValueError("map graphics resource offset is outside u16")
        encoded.extend(struct.pack("<6H", *values))
    return bytes(encoded)


def records_by_container(source):
    document = json.loads(source.read_text())
    result = {}
    for row in document["records"]:
        result.setdefault(row["container"].lower(), []).append(row)
    return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    parser.add_argument(
        "-o", "--output", type=Path,
        default=Path("assets/maps/map_load_table.json"))
    args = parser.parse_args()
    rows = export_table(args.rom.read_bytes(), args.output)
    print(f"map_load_records={rows} bytes={rows * 12:#x}")


if __name__ == "__main__":
    main()
