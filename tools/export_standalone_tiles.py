#!/usr/bin/env python3
"""Claim kind2-LZ tile banks the map-charblock family discovery did not reach.

Several tag-2 resources decode cleanly to a full 0x4000-byte (512-tile) 4bpp
bank but are not part of any discovered charblock family (they are the globally
shared map tilesets, loaded at a fixed tile base for every map). They decode and
re-compress byte-identically with the ordinary kind2 codec, so they are claimed
here as standalone tile assets. Their palette is map-dependent and not carried
with the tiles, so they are stored as sequential 4bpp indices (grayscale),
exactly like a raw charblock, pending later per-map palette linking.
"""
import argparse
import json
from pathlib import Path

from kind2_resource import decode_kind2, export_resource
from export_map_charblock_series import pointer

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
FIRST, LAST = 0x19, 0x3cd


def number(value):
    return int(value, 0) if isinstance(value, str) else int(value)


def claimed_mask(rom):
    mask = bytearray(len(rom))
    for name in ("out/claimed/manifest.json", "out/asm/manifest.json",
                 "out/assets/manifest.json", "assets/manifest.json"):
        path = ROOT / name
        if not path.exists():
            continue
        for region in json.loads(path.read_text()).get("regions", []):
            start = number(region["address"]) - ROM_BASE
            for index in range(start, start + number(region["size"])):
                if 0 <= index < len(mask):
                    mask[index] = 1
    manifest = ROOT / "assets/manifest.json"
    if manifest.exists():
        for series in json.loads(manifest.read_text()).get("series", []):
            if series.get("kind") != "golden-sun-standalone-tile-series":
                continue
            for resource in series["resources"]:
                start = number(resource["address"]) - ROM_BASE
                for index in range(start, start + number(resource["size"])):
                    if 0 <= index < len(mask):
                        mask[index] = 0
    return mask


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    mask = claimed_mask(rom)

    resources = []
    for resource in range(FIRST, LAST + 1):
        start = pointer(rom, resource) - ROM_BASE
        end = pointer(rom, resource + 1) - ROM_BASE
        if not 0 <= start < end <= len(rom) or mask[start] or rom[start] != 2:
            continue
        try:
            decoded, _, _, _ = decode_kind2(rom[start:end])
        except Exception:
            continue
        if len(decoded) != 0x4000:
            continue
        directory = ROOT / f"assets/graphics/resource_{resource:x}"
        directory.mkdir(parents=True, exist_ok=True)
        export_resource(rom[start:end], directory / "tiles.4bpp.png",
                        directory / "tiles.kind2.json", 4, 16)
        resources.append({
            "id": f"{resource:x}",
            "address": f"0x{start + ROM_BASE:08x}",
            "size": f"0x{end - start:x}",
        })

    manifest_path = ROOT / "assets/manifest.json"
    manifest = json.loads(manifest_path.read_text())
    series = [entry for entry in manifest.get("series", [])
              if entry.get("kind") != "golden-sun-standalone-tile-series"]
    series.append({
        "kind": "golden-sun-standalone-tile-series",
        "resources": resources,
    })
    manifest["series"] = series
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"standalone tiles={len(resources)}")


if __name__ == "__main__":
    main()
