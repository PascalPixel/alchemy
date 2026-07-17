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
    parser.add_argument("--asm-output", type=Path, default="out/full/asm")
    parser.add_argument("--asset-manifest", type=Path,
                        default=Path("assets/manifest.json"))
    parser.add_argument("--asset-output", type=Path,
                        default=Path("out/full/assets"))
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
    asm_regions = []
    if (Path("asm").exists() and any(Path("asm").glob("*.s"))):
        subprocess.run([
            "python3", "tools/build_asm.py", str(args.rom),
            "--output", str(args.asm_output),
        ], check=True)
        asm_manifest = json.loads(
            (Path(args.asm_output) / "manifest.json").read_text())
        for region in asm_manifest["regions"]:
            address = region["address"]
            size = region["size"]
            start = address - ROM_BASE
            end = start + size
            if any(claimed_mask[start:end]):
                raise SystemExit(
                    f"assembly overlaps another source at 0x{address:08x}")
            source = Path(region["output"]).read_bytes()
            if source != rom[start:end]:
                raise SystemExit(
                    f"assembly mismatch at 0x{address:08x} ({size:#x} bytes)")
            rebuilt[start:end] = source
            claimed_mask[start:end] = b"\1" * size
            asm_regions.append(region)
    asset_regions = []
    if args.asset_manifest.exists():
        subprocess.run([
            "python3", "tools/build_assets.py", str(args.rom),
            "--manifest", str(args.asset_manifest),
            "--output", str(args.asset_output),
        ], check=True)
        asset_manifest = json.loads(
            (args.asset_output / "manifest.json").read_text())
        for region in asset_manifest["regions"]:
            address = region["address"]
            size = region["size"]
            start = address - ROM_BASE
            end = start + size
            if any(claimed_mask[start:end]):
                raise SystemExit(
                    f"asset overlaps another source at 0x{address:08x}")
            source = Path(region["output"]).read_bytes()
            expected = rom[start:end]
            if source != expected:
                raise SystemExit(
                    f"asset mismatch at 0x{address:08x} ({size:#x} bytes)")
            rebuilt[start:end] = source
            claimed_mask[start:end] = b"\1" * size
            asset_regions.append(region)
    if bytes(rebuilt) != rom:
        raise SystemExit("full rebuild differs from private ROM")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(rebuilt)
    report = {
        "format": 1,
        "rom_base": ROM_BASE,
        "rom_size": len(rom),
        "code_regions": len(manifest["regions"]),
        "asm_regions": len(asm_regions),
        "asset_regions": len(asset_regions),
        "source_regions": (len(manifest["regions"]) + len(asm_regions) +
                           len(asset_regions)),
        "source_bytes": sum(claimed_mask),
        "rom_fallback_bytes": len(rom) - sum(claimed_mask),
        "byte_identical": True,
        "output": str(args.output),
    }
    report_path = args.output.with_suffix(".json")
    report_path.write_text(json.dumps(report, indent=2) + "\n")
    print(
        f"identical={report['byte_identical']} regions={report['source_regions']} "
        f"code={report['code_regions']} asm={report['asm_regions']} "
        f"assets={report['asset_regions']} "
        f"source_bytes={report['source_bytes']} "
        f"rom_fallback_bytes={report['rom_fallback_bytes']}"
    )


if __name__ == "__main__":
    main()
