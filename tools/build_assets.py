#!/usr/bin/env python3
"""Build and byte-verify tracked source assets against the private ROM."""
import argparse
import json
from pathlib import Path

from import_asset import gba_graphics


ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000


def number(value):
    return int(value, 0) if isinstance(value, str) else int(value)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument("--manifest", type=Path,
                        default=ROOT / "assets/manifest.json")
    parser.add_argument("-o", "--output", type=Path,
                        default=ROOT / "out/assets")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    manifest = json.loads(args.manifest.read_text())
    if manifest.get("format") != 1:
        raise ValueError("unsupported asset manifest format")
    output = args.output if args.output.is_absolute() else ROOT / args.output
    output.mkdir(parents=True, exist_ok=True)
    regions = []
    previous_end = ROM_BASE
    for entry in sorted(manifest.get("regions", []),
                        key=lambda item: number(item["address"])):
        address = number(entry["address"])
        size = number(entry["size"])
        if address < previous_end:
            raise ValueError(f"overlapping asset region at 0x{address:08x}")
        previous_end = address + size
        if not ROM_BASE <= address < previous_end <= ROM_BASE + len(rom):
            raise ValueError(f"asset region outside ROM at 0x{address:08x}")
        source = (ROOT / entry["source"]).resolve()
        if ROOT.resolve() not in source.parents:
            raise ValueError("asset source must stay inside the repository")
        kind = entry["kind"]
        if kind not in ("gba-4bpp-tiles", "gba-8bpp-tiles"):
            raise ValueError(f"unsupported asset kind: {kind}")
        bpp = 4 if kind == "gba-4bpp-tiles" else 8
        tiles, _, report = gba_graphics(source.read_bytes(), bpp)
        if len(tiles) != size:
            raise ValueError(
                f"{entry['source']}: built {len(tiles):#x}, expected {size:#x}")
        expected = rom[address - ROM_BASE:address - ROM_BASE + size]
        if tiles != expected:
            raise ValueError(f"{entry['source']}: encoded bytes differ")
        built = output / f"{address:08x}.bin"
        built.write_bytes(tiles)
        regions.append({
            "address": address, "size": size, "end": address + size,
            "kind": kind, "source": entry["source"],
            "output": str(built), "details": report,
        })
    (output / "manifest.json").write_text(json.dumps({
        "format": 1, "rom_base": ROM_BASE, "rom_size": len(rom),
        "asset_bytes": sum(item["size"] for item in regions),
        "regions": regions,
    }, indent=2) + "\n")
    print(f"assets={len(regions)} bytes={sum(item['size'] for item in regions)}")


if __name__ == "__main__":
    main()
