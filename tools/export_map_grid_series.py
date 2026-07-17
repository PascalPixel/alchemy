#!/usr/bin/env python3
"""Export the tagged kind-1 map-record grid from each traced container."""
import argparse
import struct
from pathlib import Path

from export_map_charblock_series import BASES, ROM_BASE, pointer
from kind1_map_grid import export_grid


ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    count = total = 0
    for base in BASES:
        container = pointer(rom, base) - ROM_BASE
        offsets = struct.unpack_from("<6I", rom, container + 0x24)
        begin = offsets[2]
        end = min(value for value in offsets if value > begin)
        data = rom[container + begin:container + end]
        directory = ROOT / f"assets/maps/resource_{base:x}/grid"
        tokens, sentinels = export_grid(data, directory)
        print(f"{base:x} address={container + begin + ROM_BASE:#010x} "
              f"size={len(data):#x} tokens={tokens} sentinels={sentinels}")
        count += 1
        total += len(data)
    print(f"families={count} encoded={total}")


if __name__ == "__main__":
    main()
