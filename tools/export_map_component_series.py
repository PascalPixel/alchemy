#!/usr/bin/env python3
"""Export exact semantic sources for map-container components 0, 1, 3, 4, and 5."""
import argparse
import struct
from pathlib import Path

from export_map_charblock_series import BASES, ROM_BASE, pointer
from map_container_components import (
    export_blend_animation, export_descriptors, export_metatiles, export_queues,
    export_sparse,
)


ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    claimed = 0
    for base in BASES:
        start = pointer(rom, base) - ROM_BASE
        end = pointer(rom, base + 1) - ROM_BASE
        container = rom[start:end]
        offsets = struct.unpack_from("<6I", container, 0x24)
        directory = ROOT / f"assets/maps/resource_{base:x}/components"
        directory.mkdir(parents=True, exist_ok=True)

        def data(index):
            begin = offsets[index]
            finish = min(value for value in offsets if value > begin)
            return container[begin:finish]

        metatiles, mode = export_metatiles(
            data(0), directory / "metatiles.tilemap",
            directory / "metatiles.lz.json")
        descriptors = export_descriptors(
            data(1), directory / "descriptors.json",
            directory / "descriptors.lz.json")
        queues = commands = 0
        if offsets[3]:
            queues, commands = export_queues(
                data(3), directory / "animation_queues.json",
                directory / "animation_queues.lz.json")
        blend_commands = 0
        blend_codec = "absent"
        if offsets[4]:
            blend_commands, blend_codec = export_blend_animation(
                data(4), directory / "blend_animation.json",
                directory / "blend_animation.lz.json")
        sparse = export_sparse(container[offsets[5]:], directory / "sparse_cells.json")
        sizes = [len(data(0)), len(data(1)),
                 len(data(3)) if offsets[3] else 0,
                 len(data(4)) if offsets[4] else 0,
                 len(container) - offsets[5]]
        claimed += sum(sizes)
        print(f"{base:x} mode={mode} metatiles={metatiles} "
              f"descriptors={descriptors} queues={queues} commands={commands} "
              f"blend_commands={blend_commands} blend_codec={blend_codec} "
              f"sparse={sparse} bytes={sum(sizes):#x}")
    print(f"families={len(BASES)} claimed_bytes={claimed:#x}")


if __name__ == "__main__":
    main()
