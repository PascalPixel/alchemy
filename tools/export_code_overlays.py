#!/usr/bin/env python3
"""Claim the compressed Thumb code overlays as reconstructed assembly.

Resources in the 0x19-0x3cd table whose decompressed payload begins with the
interworking dispatch stub `ldr r4,[pc,#0]; bx r4` (bytes 00 4c 20 47) are
EWRAM-resident (base 0x02000000) code overlays, not graphics. Both encodings
appear: tag-0 general-LZ and tag-1 palette-LZ. For each, capture the exact
compression plan, reconstruct the decoded payload as assembly (overlay_disasm),
verify the full compressed round trip against the ROM, and record a manifest
series that build_assets rebuilds and byte-verifies. Overlays whose disassembly
does not re-assemble byte-identically are left unclaimed rather than dumped.
"""
import argparse
import json
from pathlib import Path

from export_map_charblock_series import pointer
from extract_resource import (decode_general_trace, decode_palette_trace,
                              encode_general, encode_palette)
from overlay_disasm import (build_overlay_source, assemble_overlay,
                            OVERLAY_BASE, _reachable)
from region_disasm import build_region

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
STUB = b"\x00\x4c\x20\x47"
FIRST, LAST = 0x19, 0x3cd


def decode_stream(rom, start, end):
    comp = rom[start:end]
    tag = comp[0]
    if tag == 1:
        decoded, _, plan = decode_palette_trace(comp, 1, len(comp), 0x400000)
        body = b"\x01" + encode_palette(decoded, plan)
        codec = "golden-sun-tagged-palette-lz"
    elif tag == 0:
        decoded, _, plan = decode_general_trace(comp, 0, len(comp), 0x400000)
        body = encode_general(decoded, plan)
        codec = "golden-sun-general-lz"
    else:
        return None
    # 符号器は圧縮列を正確に再現する。ROM側で境界まで埋める場合、
    # 末尾バイトは先読み分として保持する。
    if body != comp[:len(body)]:
        return None
    return decoded, plan, codec, comp[len(body):]


def _objdump_overlay(payload):
    return build_overlay_source(payload, OVERLAY_BASE)


def _v4t_overlay(payload):
    instructions = _reachable(payload, OVERLAY_BASE)
    source, _ = build_region(None, OVERLAY_BASE, OVERLAY_BASE + len(payload),
                             instructions, {}, data=payload,
                             data_base=OVERLAY_BASE, label="Overlay")
    return source


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    args = parser.parse_args()
    rom = args.rom.read_bytes()

    resources = []
    skipped = []
    for resource in range(FIRST, LAST + 1):
        start = pointer(rom, resource) - ROM_BASE
        end = pointer(rom, resource + 1) - ROM_BASE
        if not 0 <= start < end <= len(rom):
            continue
        try:
            decoded = decode_stream(rom, start, end)
        except Exception:
            continue
        if decoded is None:
            continue
        payload, plan, codec, lookahead = decoded
        if payload[:4] != STUB or len(payload) % 2:
            continue
        source = None
        for disassemble in (_objdump_overlay, _v4t_overlay):
            try:
                candidate = disassemble(payload)
                if assemble_overlay(candidate, OVERLAY_BASE) == payload:
                    source = candidate
                    break
            except Exception:
                continue
        if source is None:
            skipped.append(resource)
            continue

        directory = ROOT / f"assets/code/resource_{resource:x}"
        directory.mkdir(parents=True, exist_ok=True)
        (directory / "overlay.s").write_text(source)
        document = {
            "format": 1, "codec": codec,
            "decoded_size": f"0x{len(payload):x}",
            "lookahead": lookahead.hex(), "tokens": plan,
        }
        if codec == "golden-sun-tagged-palette-lz":
            document["tag"] = 1
        (directory / "stream.lz.json").write_text(
            json.dumps(document, indent=2) + "\n")
        resources.append({
            "id": f"{resource:x}",
            "address": f"0x{start + ROM_BASE:08x}",
            "size": f"0x{end - start:x}",
            "decoded_size": f"0x{len(payload):x}",
        })

    # 未使用になったリソース用ディレクトリを削除する。
    for directory in (ROOT / "assets/code").glob("resource_*"):
        name = directory.name.rsplit("_", 1)[1]
        if not any(entry["id"] == name for entry in resources):
            for child in directory.iterdir():
                child.unlink()
            directory.rmdir()

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
    print(f"overlays={len(resources)} skipped={len(skipped)} "
          f"({', '.join(f'0x{r:x}' for r in skipped)})")


if __name__ == "__main__":
    main()
