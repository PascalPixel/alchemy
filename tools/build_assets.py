#!/usr/bin/env python3
"""Build and byte-verify tracked source assets against the private ROM."""
import argparse
import json
from pathlib import Path

from extract_resource import encode_general
from import_asset import gba_graphics
from tilemap import import_tilemap


ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000


def number(value):
    return int(value, 0) if isinstance(value, str) else int(value)


def source_path(name):
    source = (ROOT / name).resolve()
    if ROOT.resolve() not in source.parents:
        raise ValueError("asset source must stay inside the repository")
    return source


def build_component(entry):
    kind = entry["kind"]
    source = source_path(entry["source"])
    if kind in ("gba-4bpp-tiles", "gba-8bpp-tiles"):
        bpp = 4 if kind == "gba-4bpp-tiles" else 8
        data, _, details = gba_graphics(source.read_bytes(), bpp)
    elif kind == "gba-palette":
        _, data, details = gba_graphics(source.read_bytes(), 8)
    elif kind == "gba-tilemap16":
        data = import_tilemap(source.read_text())
        details = {"entries": len(data) // 2}
    else:
        raise ValueError(f"unsupported asset component: {kind}")
    size = number(entry["size"])
    if len(data) != size:
        raise ValueError(
            f"{entry['source']}: built {len(data):#x}, expected {size:#x}")
    return data, details


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
        kind = entry["kind"]
        if kind in ("gba-4bpp-tiles", "gba-8bpp-tiles", "gba-palette"):
            built_data, report = build_component(entry)
            sources = [entry["source"]]
        elif kind == "golden-sun-general-lz":
            parts = []
            component_reports = []
            sources = []
            for component in entry["components"]:
                data, details = build_component(component)
                parts.append(data)
                sources.append(component["source"])
                component_reports.append({
                    "kind": component["kind"],
                    "source": component["source"], "details": details,
                })
            decoded = b"".join(parts)
            plan_path = source_path(entry["plan"])
            plan = json.loads(plan_path.read_text())
            if (plan.get("format") != 1 or
                    plan.get("codec") != "golden-sun-general-lz"):
                raise ValueError("unsupported custom-LZ plan")
            if len(decoded) != number(plan["decoded_size"]):
                raise ValueError("decoded components do not match plan size")
            built_data = (encode_general(decoded, plan["tokens"]) +
                          bytes.fromhex(plan.get("lookahead", "")))
            sources.append(entry["plan"])
            report = {"decoded_size": len(decoded),
                      "tokens": len(plan["tokens"]),
                      "components": component_reports}
        else:
            raise ValueError(f"unsupported asset kind: {kind}")
        if len(built_data) != size:
            raise ValueError(
                f"asset at 0x{address:08x}: built {len(built_data):#x}, "
                f"expected {size:#x}")
        expected = rom[address - ROM_BASE:address - ROM_BASE + size]
        if built_data != expected:
            raise ValueError(f"asset at 0x{address:08x}: encoded bytes differ")
        built = output / f"{address:08x}.bin"
        built.write_bytes(built_data)
        regions.append({
            "address": address, "size": size, "end": address + size,
            "kind": kind, "sources": sources,
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
