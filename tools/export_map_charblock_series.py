#!/usr/bin/env python3
"""Export the palette and kind-2 tile banks of each discovered map family."""
import argparse
import json
import struct
from pathlib import Path

from collections import Counter, defaultdict

from export_asset import palette_png
from extract_resource import (
    decode_general_trace, decode_palette_trace, encode_general, encode_palette,
)
from kind2_resource import export_resource
from map_container_components import decode_component, decode_metatiles


ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
TABLE = 0x08320000 - ROM_BASE
FIRST_CONTAINER = 0x128
LAST_CONTAINER = 0x369
ANIMATION_LAYOUT = {
    "format": "sequential-gba-4bpp-tiles",
    "purpose": "map-animation-source",
    "virtual_base_tile": "0x600",
    "tile_count": 512,
    "tile_width": 8,
    "tile_height": 8,
    "columns": 32,
    "rows": 16,
}


def pointer(rom, resource):
    return struct.unpack_from("<I", rom, TABLE + resource * 4)[0]


def span(rom, resource):
    start = pointer(rom, resource) - ROM_BASE
    end = pointer(rom, resource + 1) - ROM_BASE
    if not 0 <= start < end <= len(rom):
        raise ValueError(f"resource {resource:x} has an invalid range")
    return start, end


def is_container(data):
    if len(data) < 0x3c or data[0] or data[1] or data[4] != 3:
        return False
    offsets = struct.unpack_from("<6I", data, 0x24)
    present = [offset for offset in offsets if offset]
    return (offsets[0] == 0x3c and
            all(0x3c <= offset < len(data) for offset in present) and
            sorted(present) == present)


def discover_families(rom):
    """Group resources 0x128..0x369 into container families.

    Each family is (base, palette_resource_or_None, bank_resources); the
    graphics tail is the greedy run of one 0x1c0 palette stream plus up to
    four kind-2 banks directly after the container. Resources between a
    family's classified tail and the next container stay unclaimed.
    """
    containers = []
    for resource in range(FIRST_CONTAINER, LAST_CONTAINER + 1):
        start, end = span(rom, resource)
        if end - start >= 0x3c and is_container(rom[start:end]):
            containers.append(resource)
    if containers[0] != FIRST_CONTAINER or containers[-1] != LAST_CONTAINER:
        raise ValueError("map container discovery bounds changed")
    families = []
    for index, base in enumerate(containers):
        nxt = (containers[index + 1] if index + 1 < len(containers)
               else LAST_CONTAINER + 6)
        palette = None
        banks = []
        cursor = base + 1
        if cursor < nxt and rom[span(rom, cursor)[0]] in (0, 1):
            try:
                decode_stream(rom, cursor)
            except Exception:
                pass
            else:
                palette = cursor
                cursor += 1
                while cursor < nxt and len(banks) < 4:
                    if rom[span(rom, cursor)[0]] != 2:
                        break
                    banks.append(cursor)
                    cursor += 1
        families.append((base, palette, tuple(banks)))
    return families


def decode_stream(rom, resource):
    """Decode a tag-0/tag-1 resource as a 224-color palette stream."""
    start, end = span(rom, resource)
    if rom[start] == 1:
        decoded, _, _ = decode_palette_trace(rom, start + 1, end, 0x1c0)
    else:
        decoded, _, _ = decode_general_trace(rom, start, end, 0x1c0)
    if len(decoded) != 0x1c0:
        raise ValueError(f"resource {resource:x} is not a 224-color palette")
    return decoded


def export_palette(rom, resource, directory):
    start, end = span(rom, resource)
    if rom[start] == 1:
        decoded, _, tokens = decode_palette_trace(rom, start + 1, end, 0x1c0)
        encoded = b"\x01" + encode_palette(decoded, tokens)
        codec = {"codec": "golden-sun-tagged-palette-lz", "tag": 1}
    else:
        decoded, _, tokens = decode_general_trace(rom, start, end, 0x1c0)
        encoded = encode_general(decoded, tokens)
        codec = {"codec": "golden-sun-general-lz"}
    if len(decoded) != 0x1c0:
        raise ValueError(f"resource {resource:x} is not a 224-color palette")
    original = rom[start:end]
    if not original.startswith(encoded):
        raise ValueError(f"resource {resource:x} token replay differs")
    plan = {"format": 1, **codec,
            "decoded_size": len(decoded), "encoded_size": len(original),
            "tokens": tokens, "lookahead": original[len(encoded):].hex()}
    (directory / "palette.lz.json").write_text(
        json.dumps(plan, separators=(",", ":")) + "\n")
    image, _ = palette_png(decoded)
    (directory / "palette.224.png").write_bytes(image)


def tile_palette_banks(rom, base):
    """Map each virtual tile index to the palette bank the map assigns it.

    The container's 2x2-metatile map holds one GBA cell per tile use, with the
    palette bank in the top nibble and the virtual tile index in the low twelve
    bits (charblock 1/2/3 at 0x000/0x200/0x400, the animation source at 0x600).
    A tile placed under several banks takes its most frequent one.
    """
    start, end = span(rom, base)
    container = rom[start:end]
    offsets = struct.unpack_from("<6I", container, 0x24)
    begin = offsets[0]
    finish = min(value for value in offsets if value > begin)
    decoded, _, _, _ = decode_component(container[begin:finish])
    _, entries = decode_metatiles(decoded)
    votes = defaultdict(Counter)
    overall = Counter()
    for value in entries:
        votes[value & 0x0fff][value >> 12] += 1
        overall[value >> 12] += 1
    banks = {tile: counter.most_common(1)[0][0]
             for tile, counter in votes.items()}
    default_bank = overall.most_common(1)[0][0] if overall else 0
    return banks, default_bank


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    count = assets = 0
    for base, palette, banks in discover_families(rom):
        if palette is None:
            continue
        directory = ROOT / f"assets/graphics/resource_{base:x}"
        directory.mkdir(parents=True, exist_ok=True)
        export_palette(rom, palette, directory)
        palette_path = directory / "palette.224.png"
        tile_banks, default_bank = tile_palette_banks(rom, base)
        count += 1
        assets += 1
        for bank, resource in enumerate(banks[:3], 1):
            start, end = span(rom, resource)
            decoded, _, _ = export_resource(
                rom[start:end], directory / f"charblock{bank}.banked.png",
                directory / f"charblock{bank}.kind2.json", 4, 16,
                palette_path=palette_path, banks=tile_banks,
                base_tile=(bank - 1) * 0x200, default_bank=default_bank)
            if decoded != 0x4000:
                raise ValueError(f"resource {resource:x} is not one charblock")
            assets += 1
        if len(banks) == 4:
            resource = banks[3]
            start, end = span(rom, resource)
            decoded, _, _ = export_resource(
                rom[start:end], directory / "animation_source.banked.png",
                directory / "animation_source.kind2.json", 4, 32,
                layout=ANIMATION_LAYOUT, palette_path=palette_path,
                banks=tile_banks, base_tile=0x600, default_bank=default_bank)
            if decoded != 0x4000:
                raise ValueError(
                    f"resource {resource:x} is not a 512-tile animation bank")
            assets += 1
    print(f"families={count} assets={assets}")


if __name__ == "__main__":
    main()
