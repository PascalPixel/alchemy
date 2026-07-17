#!/usr/bin/env python3
"""Claim the six compressed Thumb code overlays as reconstructed assembly.

Each overlay is a tag-1 palette-LZ resource whose decompressed payload is EWRAM
code. For each one this captures the exact compression plan, reconstructs the
decoded payload as assembly (overlay_disasm), verifies the full compressed round
trip against the ROM, and records a manifest series that build_assets rebuilds
and byte-verifies.
"""
import argparse
import json
from pathlib import Path

from export_map_charblock_series import pointer
from extract_resource import decode_palette_trace, encode_palette
from overlay_disasm import build_overlay_source, assemble_overlay, OVERLAY_BASE

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
OVERLAYS = (0x372, 0x374, 0x378, 0x39d, 0x3b1, 0x3c6)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    args = parser.parse_args()
    rom = args.rom.read_bytes()

    resources = []
    for resource in OVERLAYS:
        start = pointer(rom, resource) - ROM_BASE
        end = pointer(rom, resource + 1) - ROM_BASE
        stream = rom[start:end]
        if stream[0] != 1:
            raise SystemExit(f"resource 0x{resource:x} is not a tag-1 stream")
        decoded, cursor, groups = decode_palette_trace(
            stream, 1, len(stream), 0x400000)
        payload = encode_palette(decoded, groups)
        lookahead = stream[cursor:]
        if b"\x01" + payload + lookahead != stream:
            raise SystemExit(f"resource 0x{resource:x} plan is not byte-exact")

        source = build_overlay_source(decoded, OVERLAY_BASE)
        if assemble_overlay(source, OVERLAY_BASE) != decoded:
            raise SystemExit(f"resource 0x{resource:x} source is not byte-exact")

        directory = ROOT / f"assets/code/resource_{resource:x}"
        directory.mkdir(parents=True, exist_ok=True)
        (directory / "overlay.s").write_text(source)
        (directory / "stream.lz.json").write_text(json.dumps({
            "format": 1,
            "codec": "golden-sun-tagged-palette-lz",
            "tag": 1,
            "decoded_size": f"0x{len(decoded):x}",
            "lookahead": lookahead.hex(),
            "tokens": groups,
        }, indent=2) + "\n")
        resources.append({
            "id": f"{resource:x}",
            "address": f"0x{start + ROM_BASE:08x}",
            "size": f"0x{end - start:x}",
            "decoded_size": f"0x{len(decoded):x}",
        })
        print(f"resource 0x{resource:x}: decoded 0x{len(decoded):x}, "
              f"compressed 0x{end - start:x}, exact")

    manifest_path = ROOT / "assets/manifest.json"
    manifest = json.loads(manifest_path.read_text())
    series = [entry for entry in manifest.get("series", [])
              if entry.get("kind") != "golden-sun-thumb-overlay-series"]
    series.append({
        "kind": "golden-sun-thumb-overlay-series",
        "base": f"0x{OVERLAY_BASE:08x}",
        "resources": resources,
    })
    manifest["series"] = series
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"overlays={len(resources)}")


if __name__ == "__main__":
    main()
