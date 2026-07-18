#!/usr/bin/env python3
"""Build exact 4bpp tile banks from coherent, map-composed object PNGs.

An object PNG is the editable root for every tile slot named by its adjacent
text tilemap.  The tilemap retains the original slot, palette, and H/V flip
fields; the builder reverses those display flips and writes canonical 4bpp
tiles back to their exact bank slots.  A neutral fallback atlas covers only
slots that have not yet been migrated to named objects.
"""
import argparse
import json
import struct
from pathlib import Path

from export_asset import tile_png
from import_asset import gba_graphics, indexed_png
from text_bg import decode_entry, flip_tile
from tilemap import export_tilemap, import_tilemap


ROOT = Path(__file__).resolve().parents[1]
TILE = 8
PACKED_TILE = 32
SOURCE_COLUMNS = 32


def number(value):
    return int(value, 0) if isinstance(value, str) else int(value)


def child(plan_path, name):
    path = (plan_path.parent / name).resolve()
    if plan_path.parent.resolve() not in path.parents:
        raise ValueError("object-bank source must stay beside its plan")
    return path


def select_manifest_source(container, charblock, plan_path):
    """Point the map-family bank at its verified object plan."""
    manifest_path = ROOT / "assets/manifest.json"
    manifest = json.loads(manifest_path.read_text())
    for series in manifest.get("series", []):
        if series.get("kind") != "golden-sun-map-charblock-series":
            continue
        for family in series.get("families", []):
            if family.get("id") != container:
                continue
            banks = family.get("charblocks", [])
            if not 1 <= charblock <= len(banks):
                raise ValueError("manifest family does not contain this charblock")
            source = plan_path.resolve().relative_to(ROOT.resolve()).as_posix()
            banks[charblock - 1]["source"] = source
            manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")
            return
    raise ValueError("map graphics family is absent from the asset manifest")


def pack_tile(pixels):
    if len(pixels) != TILE * TILE or any(not 0 <= value < 16
                                         for value in pixels):
        raise ValueError("object tile does not contain 64 4bpp indices")
    return bytes(pixels[index] | pixels[index + 1] << 4
                 for index in range(0, len(pixels), 2))


def unpack_tile(data):
    if len(data) != PACKED_TILE:
        raise ValueError("packed 4bpp tile is not 32 bytes")
    return [value for byte in data for value in (byte & 15, byte >> 4)]


def image_tile(pixels, width, cell):
    columns = width // TILE
    left = cell % columns * TILE
    top = cell // columns * TILE
    return [pixels[(top + y) * width + left + x]
            for y in range(TILE) for x in range(TILE)]


def build_bank(plan_path):
    """Return ``(packed_bank, source_paths, details)`` for an object plan."""
    plan_path = Path(plan_path).resolve()
    plan = json.loads(plan_path.read_text())
    if plan.get("format") != 1 or plan.get("kind") != "gba-4bpp-object-bank":
        raise ValueError("unsupported tile-object plan")
    tile_count = number(plan["tile_count"])
    fallback_path = child(plan_path, plan["fallback"])
    fallback, _, _ = gba_graphics(fallback_path.read_bytes(), 4)
    if len(fallback) != tile_count * PACKED_TILE:
        raise ValueError("object-bank fallback has the wrong tile count")
    output = bytearray(fallback)
    claimed = {}
    sources = [plan_path, fallback_path]
    placements = 0
    horizontal_flips = 0
    vertical_flips = 0

    for item in plan.get("objects", []):
        source_path = child(plan_path, item["source"])
        tilemap_path = child(plan_path, item["tilemap"])
        palette_path = child(plan_path, item["palette"])
        width, height, pixels, colors = indexed_png(source_path.read_bytes())
        _, _, _, expected_colors = indexed_png(palette_path.read_bytes())
        if colors != expected_colors:
            raise ValueError(f"{source_path}: palette differs from {palette_path}")
        entries_raw = import_tilemap(tilemap_path.read_text())
        entries = struct.unpack(f"<{len(entries_raw) // 2}H", entries_raw)
        if width % TILE or height % TILE or len(entries) != width // TILE * height // TILE:
            raise ValueError(f"{source_path}: dimensions differ from its tilemap")
        base_tile = number(item["base_tile"])
        for cell_index, entry in enumerate(entries):
            tile, palette, hflip, vflip = decode_entry(entry)
            slot = tile - base_tile
            if not 0 <= slot < tile_count:
                if item.get("mixed_windows", False):
                    continue
                raise ValueError(f"{tilemap_path}: tile lies outside this bank")
            displayed = image_tile(pixels, width, cell_index)
            if any(value >> 4 != palette for value in displayed):
                raise ValueError(
                    f"{source_path}: cell {cell_index} differs from its palette bank")
            canonical = flip_tile(
                [value & 15 for value in displayed], hflip, vflip)
            packed = pack_tile(canonical)
            previous = claimed.get(slot)
            if previous is not None and previous != packed:
                raise ValueError(
                    f"{source_path}: repeated tile slot {slot} is inconsistent")
            claimed[slot] = packed
            placements += 1
            horizontal_flips += hflip
            vertical_flips += vflip
        sources.extend((source_path, tilemap_path, palette_path))

    for slot, packed in claimed.items():
        start = slot * PACKED_TILE
        if plan.get("require_blank_fallback", True) and any(
                output[start:start + PACKED_TILE]):
            raise ValueError(f"fallback still contains authoritative tile {slot}")
        output[start:start + PACKED_TILE] = packed
    return bytes(output), list(dict.fromkeys(sources)), {
        "tiles": tile_count,
        "object_tiles": len(claimed),
        "placements": placements,
        "horizontal_flips": horizontal_flips,
        "vertical_flips": vertical_flips,
        "objects": len(plan.get("objects", [])),
    }


def map_entries(map_dir):
    """Return the header-visible 8px entry grid for one reconstructed map."""
    from compose_scene import (cell_indices, parse_metatiles,
                               scene_dimensions)

    metatiles = parse_metatiles(map_dir / "components/metatiles.tilemap")
    cells = cell_indices(map_dir / "grid", len(metatiles))
    width, height, _, _ = scene_dimensions(map_dir)
    rows = []
    for tile_y in range((height + TILE - 1) // TILE):
        row = []
        cell_y, sub_y = divmod(tile_y, 2)
        for tile_x in range((width + TILE - 1) // TILE):
            cell_x, sub_x = divmod(tile_x, 2)
            metatile = cells[cell_y][cell_x]
            if metatile >= len(metatiles):
                raise ValueError("map grid references a missing metatile")
            row.append(metatiles[metatile][sub_y * 2 + sub_x])
        rows.append(row)
    return rows


def extract_map_object(container, charblock, rectangle, name, append=False,
                       mixed_windows=False, name_ja=None, name_en=None):
    """Bootstrap one coherent object and neutral remainder from existing source."""
    if charblock not in (1, 2):
        raise ValueError("map object extraction currently supports charblocks 1/2")
    left, top, width_tiles, height_tiles = rectangle
    if min(left, top) < 0 or min(width_tiles, height_tiles) <= 0:
        raise ValueError("object rectangle must be positive")
    directory = ROOT / f"assets/graphics/resource_{container}"
    map_dir = ROOT / f"assets/maps/resource_{container}"
    bank_path = directory / f"charblock{charblock}.4bpp.png"
    plan_path = directory / f"charblock{charblock}.objects.json"
    if plan_path.exists():
        if not append:
            raise ValueError(f"{plan_path} already exists; use --append")
        plan = json.loads(plan_path.read_text())
        if any(item.get("name") == name for item in plan.get("objects", [])):
            raise ValueError(f"object {name!r} already exists")
        bank, _, _ = build_bank(plan_path)
        fallback_name = plan["fallback"]
        fallback_path = child(plan_path, fallback_name)
        remainder, _, _ = gba_graphics(fallback_path.read_bytes(), 4)
    else:
        bank, _, details = gba_graphics(bank_path.read_bytes(), 4)
        if len(bank) != 0x4000 or details["tiles"] != 512:
            raise ValueError("object extraction requires one complete charblock")
        fallback_name = f"charblock{charblock}.remaining.4bpp.png"
        fallback_path = directory / fallback_name
        remainder = bank
    from compose_scene import tile_window_order

    window_order = tile_window_order(map_dir)
    if charblock not in window_order:
        raise ValueError("selected charblock is not in the map's display windows")
    base_tile = window_order.index(charblock) * 0x200
    display_banks = {}
    for source_charblock in window_order:
        if source_charblock == charblock:
            source_bank = bank
        else:
            source_plan = directory / f"charblock{source_charblock}.objects.json"
            source_png = directory / f"charblock{source_charblock}.4bpp.png"
            if source_plan.exists():
                source_bank, _, _ = build_bank(source_plan)
            elif source_png.exists():
                source_bank, _, _ = gba_graphics(source_png.read_bytes(), 4)
            else:
                raise ValueError("map object needs an unavailable display charblock")
        if len(source_bank) != 0x4000:
            raise ValueError("map display charblock is not exactly 512 tiles")
        display_banks[source_charblock] = source_bank

    palette_path = directory / "palette.224.png"
    _, _, _, colors = indexed_png(palette_path.read_bytes())
    grid = map_entries(map_dir)
    if top + height_tiles > len(grid) or left + width_tiles > len(grid[0]):
        raise ValueError("object rectangle exceeds the declared map")
    entries = [grid[y][x]
               for y in range(top, top + height_tiles)
               for x in range(left, left + width_tiles)]
    display_tiles = []
    claimed = set()
    for entry in entries:
        tile, palette, hflip, vflip = decode_entry(entry)
        window, slot = divmod(tile, 0x200)
        source_charblock = window_order[window]
        if source_charblock != charblock and not mixed_windows:
            raise ValueError("object rectangle crosses into another display window")
        canonical = unpack_tile(
            display_banks[source_charblock][
                slot * PACKED_TILE:(slot + 1) * PACKED_TILE])
        displayed = flip_tile(canonical, hflip, vflip)
        if palette * 16 + max(displayed) >= len(colors):
            raise ValueError("object rectangle needs a palette bank not stored locally")
        display_tiles.extend(palette * 16 + value for value in displayed)
        if source_charblock == charblock:
            claimed.add(slot)

    if not claimed:
        raise ValueError("object rectangle does not reference the selected charblock")

    objects_dir = directory / "objects"
    objects_dir.mkdir(parents=True, exist_ok=True)
    object_path = objects_dir / f"{name}.png"
    tilemap_path = objects_dir / f"{name}.tilemap"
    object_png, _ = tile_png(
        bytes(display_tiles), 8, width_tiles, palette_colors=colors)
    if object_path.exists() and object_path.read_bytes() != object_png:
        raise ValueError(f"{object_path}: another bank produced a different object")
    object_path.write_bytes(object_png)
    tilemap_raw = b"".join(struct.pack("<H", entry) for entry in entries)
    tilemap_text = export_tilemap(tilemap_raw, width_tiles)
    if tilemap_path.exists() and tilemap_path.read_text() != tilemap_text:
        raise ValueError(f"{tilemap_path}: another bank produced a different tilemap")
    tilemap_path.write_text(tilemap_text)

    remainder = bytearray(remainder)
    for slot in claimed:
        start = slot * PACKED_TILE
        remainder[start:start + PACKED_TILE] = bytes(PACKED_TILE)
    fallback_png, _ = tile_png(bytes(remainder), 4, SOURCE_COLUMNS)
    fallback_path.write_bytes(fallback_png)
    item = {
        "name": name,
        "source": f"objects/{name}.png",
        "tilemap": f"objects/{name}.tilemap",
        "palette": "palette.224.png",
        "base_tile": f"0x{base_tile:x}",
    }
    if mixed_windows:
        item["mixed_windows"] = True
    if name_ja:
        item["name_ja"] = name_ja
    if name_en:
        item["name_en"] = name_en
    if plan_path.exists():
        plan["objects"].append(item)
    else:
        plan = {
            "format": 1,
            "kind": "gba-4bpp-object-bank",
            "tile_count": 512,
            "fallback": fallback_name,
            "require_blank_fallback": True,
            "objects": [item],
        }
    plan_path.write_text(json.dumps(plan, indent=2) + "\n")
    rebuilt, _, report = build_bank(plan_path)
    if rebuilt != bank:
        raise AssertionError("extracted object bank does not reproduce its source")
    select_manifest_source(container, charblock, plan_path)
    superseded = bank_path.exists()
    if superseded:
        bank_path.unlink()
    print(f"object={object_path} size={width_tiles * TILE}x{height_tiles * TILE} "
          f"tiles={report['object_tiles']} placements={report['placements']}")
    print(f"plan={plan_path} fallback={fallback_path}")
    if superseded:
        print(f"superseded={bank_path}")


def parse_rectangle(value):
    try:
        fields = tuple(int(field, 0) for field in value.split(","))
    except ValueError as error:
        raise argparse.ArgumentTypeError("rectangle must be x,y,width,height") from error
    if len(fields) != 4:
        raise argparse.ArgumentTypeError("rectangle must be x,y,width,height")
    return fields


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    build = commands.add_parser("build")
    build.add_argument("plan", type=Path)
    build.add_argument("-o", "--output", type=Path)
    extract = commands.add_parser("extract-map")
    extract.add_argument("container", help="hex resource id, e.g. 152")
    extract.add_argument("--charblock", type=int, required=True)
    extract.add_argument("--rect", type=parse_rectangle, required=True,
                         help="8px-tile rectangle: x,y,width,height")
    extract.add_argument("--name", required=True)
    extract.add_argument("--append", action="store_true",
                         help="add another object to an existing bank plan")
    extract.add_argument("--mixed-windows", action="store_true",
                         help="compose both display windows; claim this bank's slots")
    extract.add_argument("--name-ja", help="period-style Japanese semantic name")
    extract.add_argument("--name-en", help="short English gloss")
    args = parser.parse_args()
    if args.command == "build":
        data, _, report = build_bank(args.plan)
        if args.output:
            args.output.parent.mkdir(parents=True, exist_ok=True)
            args.output.write_bytes(data)
        print(" ".join(f"{key}={value}" for key, value in report.items()))
    else:
        extract_map_object(args.container.lower(), args.charblock,
                           args.rect, args.name, args.append,
                           args.mixed_windows, args.name_ja, args.name_en)


if __name__ == "__main__":
    main()
