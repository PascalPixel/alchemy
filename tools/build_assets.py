#!/usr/bin/env python3
"""Build and byte-verify tracked source assets against the private ROM."""
import argparse
import json
from pathlib import Path

from extract_resource import (encode_general, encode_palette,
                              encode_general_prefill)
from import_asset import (gba_graphics, gba_palette_rgba, gba_4bpp_banked,
                          indexed_png, rgba_png)
from archive_asset import build_archive
from tilemap import import_tilemap
from wordstream import import_words
from pairtable import import_pairs
from f0_archive import build_archive as build_f0_archive
from skip_sprite_archive import build_archive as build_skip_sprite_archive
from kind2_resource import encode_kind2
from kind1_map_grid import build_grid
from overlay_disasm import assemble_overlay
from map_container_components import (
    build_blend_animation, build_descriptors, build_header, build_metatiles,
    build_queues, build_sparse,
)


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
    if kind == "gba-4bpp-tiles-banked":
        data, details = gba_4bpp_banked(source.read_bytes())
    elif kind in ("gba-4bpp-tiles", "gba-8bpp-tiles"):
        bpp = 4 if kind == "gba-4bpp-tiles" else 8
        data, _, details = gba_graphics(source.read_bytes(), bpp)
    elif kind == "gba-palette":
        _, data, details = gba_graphics(source.read_bytes(), 8)
    elif kind == "gba-palette-rgba":
        data, details = gba_palette_rgba(source.read_bytes())
    elif kind == "gba-tilemap16":
        data = import_tilemap(source.read_text())
        details = {"entries": len(data) // 2}
    elif kind == "indexed-bytes":
        width, height, pixels, _ = indexed_png(source.read_bytes())
        data = bytes(pixels)
        details = {"width": width, "height": height}
    elif kind == "raw-lz-bytes":
        width, height, pixels, _ = indexed_png(source.read_bytes())
        data = bytes(pixels)[:number(entry["size"])]
        details = {"width": width, "height": height, "bytes": len(data)}
    elif kind == "rgba-bytes":
        width, height, data = rgba_png(source.read_bytes())
        details = {"width": width, "height": height, "pixels": len(data) // 4}
    elif kind == "little-u16-text":
        data = import_words(source.read_text())
        details = {"words": len(data) // 2}
    elif kind == "little-u16-pairs":
        data = import_pairs(source.read_text())
        details = {"pairs": len(data) // 4}
    elif kind == "zero-skip-sprite-archive":
        plan_path = source_path(entry["plan"])
        palette_path = source_path(entry["palette"])
        plan = json.loads(plan_path.read_text())
        data = build_skip_sprite_archive(plan, source, palette_path)
        details = {"images": int(plan["images"]),
                   "width": int(plan["width"]),
                   "height": int(plan["height"])}
    elif kind == "golden-sun-thumb-overlay":
        base = number(entry["base"])
        data = assemble_overlay(source, base)
        details = {"base": f"0x{base:08x}", "bytes": len(data)}
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
    entries = list(manifest.get("regions", []))
    grid_addresses = {
        str(grid["id"]).lower(): number(grid["address"])
        for series in manifest.get("series", [])
        if series.get("kind") == "golden-sun-map-grid-series"
        for grid in series["grids"]}
    for series in manifest.get("series", []):
        if series.get("kind") == "golden-sun-zero-skip-sprite-series":
            palette = series["palette"]
            for resource in series["resources"]:
                name = str(resource["id"]).lower()
                directory = f"assets/graphics/resource_{name}"
                entries.append({
                    "address": resource["address"],
                    "size": resource["size"],
                    "kind": "golden-sun-general-lz",
                    "plan": f"{directory}/stream.lz.json",
                    "components": [{
                        "kind": "zero-skip-sprite-archive",
                        "size": resource["decoded_size"],
                        "source": f"{directory}/images",
                        "plan": f"{directory}/archive.json",
                        "palette": palette,
                    }],
                })
        elif series.get("kind") == "golden-sun-map-charblock-series":
            animation_layout = series.get("animation_layout")
            for family in series["families"]:
                name = str(family["id"]).lower()
                directory = f"assets/graphics/resource_{name}"
                palette = family["palette"]
                entries.append({
                    "address": palette["address"], "size": palette["size"],
                    "kind": "golden-sun-general-lz",
                    "plan": f"{directory}/palette.lz.json",
                    "components": [{
                        "kind": "gba-palette", "size": "0x1c0",
                        "source": f"{directory}/palette.224.png",
                    }],
                })
                for bank, resource in enumerate(family["charblocks"], 1):
                    entries.append({
                        "address": resource["address"],
                        "size": resource["size"],
                        "kind": "golden-sun-kind2-lz",
                        "plan": f"{directory}/charblock{bank}.kind2.json",
                        "components": [{
                            "kind": "gba-4bpp-tiles-banked", "size": "0x4000",
                            "source": f"{directory}/charblock{bank}.banked.png",
                        }],
                    })
                animation = family.get("animation_source")
                if animation is not None:
                    entries.append({
                        "address": animation["address"],
                        "size": animation["size"],
                        "kind": "golden-sun-kind2-lz",
                        "plan": f"{directory}/animation_source.kind2.json",
                        "layout": animation_layout,
                        "components": [{
                            "kind": "gba-4bpp-tiles-banked", "size": "0x4000",
                            "source": f"{directory}/animation_source.banked.png",
                        }],
                    })
        elif series.get("kind") == "golden-sun-standalone-palette-series":
            for palette in series["palettes"]:
                name = str(palette["id"]).lower()
                directory = f"assets/graphics/resource_{name}"
                entries.append({
                    "address": palette["address"], "size": palette["size"],
                    "kind": "golden-sun-general-lz",
                    "plan": f"{directory}/palette.lz.json",
                    "components": [{
                        "kind": "gba-palette", "size": "0x1c0",
                        "source": f"{directory}/palette.224.png",
                    }],
                })
        elif series.get("kind") == "golden-sun-standalone-tile-series":
            for resource in series["resources"]:
                name = str(resource["id"]).lower()
                directory = f"assets/graphics/resource_{name}"
                entries.append({
                    "address": resource["address"], "size": resource["size"],
                    "kind": "golden-sun-kind2-lz",
                    "plan": f"{directory}/tiles.kind2.json",
                    "components": [{
                        "kind": "gba-4bpp-tiles", "size": "0x4000",
                        "source": f"{directory}/tiles.4bpp.png",
                    }],
                })
        elif series.get("kind") == "golden-sun-prefill-lz-series":
            for resource in series["resources"]:
                name = str(resource["id"]).lower()
                directory = f"assets/data/resource_{name}"
                entries.append({
                    "address": resource["address"], "size": resource["size"],
                    "kind": "golden-sun-general-lz",
                    "plan": f"{directory}/stream.lz.json",
                    "components": [{
                        "kind": "raw-lz-bytes",
                        "size": resource["decoded_size"],
                        "source": f"{directory}/content.png",
                    }],
                })
        elif series.get("kind") == "golden-sun-thumb-overlay-series":
            base = series["base"]
            for resource in series["resources"]:
                name = str(resource["id"]).lower()
                directory = f"assets/code/resource_{name}"
                entries.append({
                    "address": resource["address"], "size": resource["size"],
                    "kind": "golden-sun-general-lz",
                    "plan": f"{directory}/stream.lz.json",
                    "components": [{
                        "kind": "golden-sun-thumb-overlay",
                        "size": resource["decoded_size"],
                        "source": f"{directory}/overlay.s",
                        "base": base,
                    }],
                })
        elif series.get("kind") == "golden-sun-map-grid-series":
            for grid in series["grids"]:
                name = str(grid["id"]).lower()
                directory = f"assets/maps/resource_{name}/grid"
                entries.append({
                    "address": grid["address"], "size": grid["size"],
                    "kind": "golden-sun-kind1-grid",
                    "source": directory,
                    "plan": f"{directory}/grid.kind1.json",
                })
        elif series.get("kind") == "golden-sun-map-component-series":
            for family in series["families"]:
                name = str(family["id"]).lower()
                directory = f"assets/maps/resource_{name}/components"
                header = family["header"]
                container = number(header["address"])
                offsets_check = {
                    int(component["slot"]):
                        number(component["address"]) - container
                    for component in family["components"]}
                offsets_check[2] = grid_addresses[name] - container
                entries.append({
                    "address": header["address"], "size": header["size"],
                    "kind": "golden-sun-map-container-header",
                    "source": f"{directory}/header.json",
                    "offsets_check": offsets_check,
                })
                for component in family["components"]:
                    slot = int(component["slot"])
                    sources = {
                        0: ("golden-sun-map-metatiles",
                            f"{directory}/metatiles.tilemap",
                            f"{directory}/metatiles.lz.json"),
                        1: ("golden-sun-map-descriptors",
                            f"{directory}/descriptors.json",
                            f"{directory}/descriptors.lz.json"),
                        3: ("golden-sun-map-animation-queues",
                            f"{directory}/animation_queues.json",
                            f"{directory}/animation_queues.lz.json"),
                        4: ("golden-sun-map-blend-animation",
                            f"{directory}/blend_animation.json",
                            f"{directory}/blend_animation.lz.json"),
                        5: ("golden-sun-map-sparse-cells",
                            f"{directory}/sparse_cells.json", None),
                    }
                    if slot not in sources:
                        raise ValueError("unsupported map component slot")
                    kind, source, plan = sources[slot]
                    entry = {
                        "address": component["address"],
                        "size": component["size"],
                        "kind": kind, "source": source,
                    }
                    if plan is not None:
                        entry["plan"] = plan
                    entries.append(entry)
        else:
            raise ValueError("unsupported asset series")
    regions = []
    previous_end = ROM_BASE
    for entry in sorted(entries,
                        key=lambda item: number(item["address"])):
        address = number(entry["address"])
        size = number(entry["size"])
        if address < previous_end:
            raise ValueError(f"overlapping asset region at 0x{address:08x}")
        previous_end = address + size
        if not ROM_BASE <= address < previous_end <= ROM_BASE + len(rom):
            raise ValueError(f"asset region outside ROM at 0x{address:08x}")
        kind = entry["kind"]
        if kind in ("gba-4bpp-tiles", "gba-8bpp-tiles", "gba-palette",
                    "gba-palette-rgba"):
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
                if component["kind"] == "zero-skip-sprite-archive":
                    sources.extend((component["plan"], component["palette"]))
                component_reports.append({
                    "kind": component["kind"],
                    "source": component["source"], "details": details,
                })
            decoded = b"".join(parts)
            plan_path = source_path(entry["plan"])
            plan = json.loads(plan_path.read_text())
            if plan.get("format") != 1 or plan.get("codec") not in (
                    "golden-sun-general-lz", "golden-sun-palette-lz",
                    "golden-sun-tagged-palette-lz",
                    "golden-sun-general-lz-prefill"):
                raise ValueError("unsupported custom-LZ plan")
            if len(decoded) != number(plan["decoded_size"]):
                raise ValueError("decoded components do not match plan size")
            if plan["codec"] == "golden-sun-general-lz-prefill":
                built_data = encode_general_prefill(
                    decoded, plan["tokens"], number(plan["prefill"]),
                    number(plan.get("header", 1)))
            elif plan["codec"] == "golden-sun-general-lz":
                built_data = encode_general(decoded, plan["tokens"])
            else:
                built_data = encode_palette(decoded, plan["tokens"])
                if plan["codec"] == "golden-sun-tagged-palette-lz":
                    if int(plan.get("tag", -1)) != 1:
                        raise ValueError(
                            "tagged palette-LZ plan is missing tag 1")
                    built_data = b"\x01" + built_data
            built_data += bytes.fromhex(plan.get("lookahead", ""))
            sources.append(entry["plan"])
            report = {"decoded_size": len(decoded),
                      "tokens": len(plan["tokens"]),
                      "components": component_reports}
        elif kind == "golden-sun-kind2-lz":
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
            expected_layout = entry.get("layout")
            if (expected_layout is not None and
                    plan.get("layout") != expected_layout):
                raise ValueError("tag-2 plan layout differs from manifest")
            if len(decoded) != number(plan["decoded_size"]):
                raise ValueError("decoded tag-2 components do not match plan")
            built_data = encode_kind2(decoded, plan)
            sources.append(entry["plan"])
            report = {"decoded_size": len(decoded),
                      "tokens": len(plan["tokens"]),
                      "layout": plan.get("layout"),
                      "components": component_reports}
        elif kind == "golden-sun-kind1-grid":
            directory = source_path(entry["source"])
            plan_path = source_path(entry["plan"])
            plan = json.loads(plan_path.read_text())
            built_data = build_grid(plan, directory)
            sources = [entry["plan"], *[
                str(Path(entry["source"]) / name) for name in
                ("value_low.png", "value_high.png", "attribute_a.png",
                 "attribute_b.png", "sentinels.png")]]
            report = {"decoded_size": number(plan["decoded_size"]),
                      "tokens": len(plan["tokens"]), "planes": 4}
        elif kind in ("golden-sun-map-metatiles",
                      "golden-sun-map-descriptors",
                      "golden-sun-map-animation-queues",
                      "golden-sun-map-blend-animation"):
            source = source_path(entry["source"])
            plan_path = source_path(entry["plan"])
            builders = {
                "golden-sun-map-metatiles": build_metatiles,
                "golden-sun-map-descriptors": build_descriptors,
                "golden-sun-map-animation-queues": build_queues,
                "golden-sun-map-blend-animation": build_blend_animation,
            }
            built_data = builders[kind](source, plan_path)
            plan = json.loads(plan_path.read_text())
            sources = [entry["source"], entry["plan"]]
            report = {
                "decoded_size": number(plan["decoded_size"]),
                "tokens": len(plan["tokens"]),
                "component": plan["component"],
            }
        elif kind == "golden-sun-map-container-header":
            source = source_path(entry["source"])
            built_data = build_header(source, entry["offsets_check"])
            document = json.loads(source.read_text())
            sources = [entry["source"]]
            report = {"records": len(document["records"]),
                      "component_offsets": document["component_offsets"]}
        elif kind == "golden-sun-map-sparse-cells":
            source = source_path(entry["source"])
            built_data = build_sparse(source)
            document = json.loads(source.read_text())
            sources = [entry["source"]]
            report = {"records": len(document["records"]),
                      "alignment_zeros": document["alignment_zeros"]}
        elif kind == "golden-sun-offset-palette-lz":
            plan_path = source_path(entry["plan"])
            atlas_path = source_path(entry["source"])
            plan = json.loads(plan_path.read_text())
            if plan.get("format") != 1 or plan.get("codec") != kind:
                raise ValueError("unsupported archive plan")
            built_data = build_archive(atlas_path.read_bytes(), plan)
            sources = [entry["source"], entry["plan"]]
            report = {"streams": len(plan["streams"]),
                      "chunk_width": plan["chunk_width"],
                      "chunk_height": plan["chunk_height"]}
        elif kind == "golden-sun-mtf4-archive":
            plan_path = source_path(entry["plan"])
            directory = source_path(entry["source"])
            plan = json.loads(plan_path.read_text())
            if plan.get("format") != 1 or plan.get("codec") != kind:
                raise ValueError("unsupported F0 archive plan")
            built_data = build_f0_archive(plan, directory)
            image_sources = [
                str(Path(entry["source"]) / f"image_{index:02d}.png")
                for index in range(int(plan["images"]))]
            sources = [entry["plan"], *image_sources]
            report = {"entries": len(plan["entries"]),
                      "images": int(plan["images"])}
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
