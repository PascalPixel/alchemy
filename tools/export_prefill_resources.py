#!/usr/bin/env python3
"""Claim the general-LZ resources that decompress against a pre-filled window.

A large family of tag-0 resources rejects a bare general-LZ decode at output
offset 0: the game decompresses them into a buffer that is pre-cleared to a
fixed size, so an early back-reference reads that zeroed window. Decoding with a
0x1000-byte zero pre-fill reverses them (entropy drops from ~7.9 to 2-6, i.e.
real decompression), and the captured token plan re-compresses to the ROM bytes
exactly. The decompressed payload is stored as an indexed byte image and the
plan records the pre-fill size, so build_assets rebuilds and byte-verifies each.
"""
import argparse
import json
from pathlib import Path

from export_asset import byte_png
from export_map_charblock_series import pointer
from extract_resource import (decode_general_prefill_trace,
                              encode_general_prefill, decode_general_trace)

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
FIRST, LAST = 0x19, 0x3cd
PREFILL = 0x1000
WIDTH = 64
STRIDE = WIDTH * 8


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
    # この出力器は担当系列を再生成するため、以前の確定領域は
    # 未確定として扱い、他の出力器の所有物として除外しない。
    manifest = ROOT / "assets/manifest.json"
    if manifest.exists():
        for series in json.loads(manifest.read_text()).get("series", []):
            if series.get("kind") != "golden-sun-prefill-lz-series":
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
        if not 0 <= start < end <= len(rom) or mask[start]:
            continue
        compressed = rom[start:end]
        # 通常のgeneral-LZ展開だけで扱えるリソースは除外する。
        try:
            decode_general_trace(compressed, 0, len(compressed), 0x40000)
            continue
        except Exception:
            pass
        # 通常の種別0ヘッダとヘッダなし形式を試す。
        decoded = tokens = header = None
        for candidate in (1, 0):
            try:
                payload, _, plan = decode_general_prefill_trace(
                    compressed, 0, len(compressed), 0x40000, PREFILL, candidate)
                rebuilt = encode_general_prefill(payload, plan, PREFILL, candidate)
            except Exception:
                continue
            if rebuilt == compressed[:len(rebuilt)] and len(compressed) - len(rebuilt) <= 8:
                decoded, tokens, header = payload, plan, candidate
                break
        if decoded is None:
            continue
        rebuilt = encode_general_prefill(decoded, tokens, PREFILL, header)

        directory = ROOT / f"assets/data/resource_{resource:x}"
        directory.mkdir(parents=True, exist_ok=True)
        padded = decoded + bytes(-len(decoded) % STRIDE)
        image, _ = byte_png(padded, WIDTH)
        (directory / "content.png").write_bytes(image)
        (directory / "stream.lz.json").write_text(json.dumps({
            "format": 1, "codec": "golden-sun-general-lz-prefill",
            "prefill": f"0x{PREFILL:x}", "header": header,
            "decoded_size": f"0x{len(decoded):x}",
            "lookahead": compressed[len(rebuilt):].hex(),
            "tokens": tokens,
        }, indent=2) + "\n")
        resources.append({
            "id": f"{resource:x}",
            "address": f"0x{start + ROM_BASE:08x}",
            "size": f"0x{end - start:x}",
            "decoded_size": f"0x{len(decoded):x}",
        })

    claimed_ids = {entry["id"] for entry in resources}
    for directory in (ROOT / "assets/data").glob("resource_*"):
        # この出力器が所有するcontent.png入りディレクトリだけを削除する。
        if not (directory / "content.png").exists():
            continue
        if directory.name.rsplit("_", 1)[1] not in claimed_ids:
            for child in directory.iterdir():
                child.unlink()
            directory.rmdir()

    manifest_path = ROOT / "assets/manifest.json"
    manifest = json.loads(manifest_path.read_text())
    series = [entry for entry in manifest.get("series", [])
              if entry.get("kind") != "golden-sun-prefill-lz-series"]
    series.append({
        "kind": "golden-sun-prefill-lz-series",
        "resources": resources,
    })
    manifest["series"] = series
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"prefill-lz resources={len(resources)}")


if __name__ == "__main__":
    main()
