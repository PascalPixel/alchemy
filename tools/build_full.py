#!/usr/bin/env python3
"""Assemble a full private-ROM rebuild from verified source regions.

Unclaimed bytes deliberately remain ROM fallback bytes and are reported.
"""
import argparse
import json
import os
import subprocess
from pathlib import Path

ROM_BASE = 0x08000000


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument("-o", "--output", type=Path, default="out/full/rebuilt.gba")
    parser.add_argument("--claimed-output", type=Path, default="out/full/claimed")
    parser.add_argument("--jobs", type=int, default=min(16, os.cpu_count() or 1))
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    claimed = args.claimed_output
    subprocess.run([
        "python3", "tools/build_claimed.py", str(args.rom),
        "--jobs", str(args.jobs), "--output", str(claimed),
    ], check=True)
    manifest_path = claimed / "manifest.json"
    manifest = json.loads(manifest_path.read_text())
    image = (claimed / "claimed.bin").read_bytes()
    image_base = manifest["image_base"]
    rebuilt = bytearray(rom)
    claimed_mask = bytearray(len(rom))
    for region in manifest["regions"]:
        address = region["address"]
        size = region["size"]
        source = image[address - image_base:address - image_base + size]
        expected = rom[address - ROM_BASE:address - ROM_BASE + size]
        if source != expected:
            raise SystemExit(
                f"source mismatch at 0x{address:08x} ({size:#x} bytes)")
        start = address - ROM_BASE
        rebuilt[start:start + size] = source
        claimed_mask[start:start + size] = b"\1" * size
    if bytes(rebuilt) != rom:
        raise SystemExit("full rebuild differs from private ROM")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(rebuilt)
    report = {
        "format": 1,
        "rom_base": ROM_BASE,
        "rom_size": len(rom),
        "source_regions": len(manifest["regions"]),
        "source_bytes": sum(claimed_mask),
        "rom_fallback_bytes": len(rom) - sum(claimed_mask),
        "byte_identical": True,
        "output": str(args.output),
    }
    report_path = args.output.with_suffix(".json")
    report_path.write_text(json.dumps(report, indent=2) + "\n")
    print(
        f"identical={report['byte_identical']} regions={report['source_regions']} "
        f"source_bytes={report['source_bytes']} "
        f"rom_fallback_bytes={report['rom_fallback_bytes']}"
    )


if __name__ == "__main__":
    main()
