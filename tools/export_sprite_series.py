#!/usr/bin/env python3
"""Export a manifest-declared series of zero-skip sprite resources."""
import argparse
import json
from pathlib import Path

from extract_resource import decode_general_trace, encode_general
from skip_sprite_archive import export


ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000


def number(value):
    return int(value, 0) if isinstance(value, str) else int(value)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    parser.add_argument("--manifest", type=Path,
                        default=ROOT / "assets/manifest.json")
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    manifest = json.loads(args.manifest.read_text())
    count = images = encoded_bytes = 0
    for series in manifest.get("series", []):
        if series.get("kind") != "golden-sun-zero-skip-sprite-series":
            continue
        palette = ROOT / series["palette"]
        for resource in series["resources"]:
            name = str(resource["id"]).lower()
            address = number(resource["address"])
            size = number(resource["size"])
            decoded_size = number(resource["decoded_size"])
            start = address - ROM_BASE
            decoded, cursor, tokens = decode_general_trace(
                rom, start, start + size, decoded_size)
            if cursor != start + size or len(decoded) != decoded_size:
                raise ValueError(f"resource {name} crossed its manifest bounds")
            encoded = encode_general(decoded, tokens)
            original = rom[start:cursor]
            if not original.startswith(encoded):
                raise ValueError(f"resource {name} custom-LZ replay differs")
            lookahead = original[len(encoded):]
            directory = ROOT / f"assets/graphics/resource_{name}"
            directory.mkdir(parents=True, exist_ok=True)
            (directory / "stream.lz.json").write_text(json.dumps({
                "format": 1, "codec": "golden-sun-general-lz",
                "decoded_size": len(decoded), "encoded_size": size,
                "tokens": tokens, "lookahead": lookahead.hex(),
            }, separators=(",", ":")) + "\n")
            image_count = export(
                decoded, directory / "images", directory / "archive.json",
                palette, directory / "preview.atlas.png",
                int(resource["width"]), int(resource["height"]))
            count += 1
            images += image_count
            encoded_bytes += size
    print(f"resources={count} images={images} bytes={encoded_bytes}")


if __name__ == "__main__":
    main()
