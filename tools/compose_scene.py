#!/usr/bin/env python3
"""Compose a map container's byte-exact tile banks into a viewable scene preview.

This is DISPLAY-ONLY. The byte-exact charblock/grid/metatile assets remain the
build source; this tool reads them and paints the composed 2D scene the map
actually renders, so a reviewer sees "four trees in a row" instead of a sheet of
tile fragments. It changes no build inputs and is never read back by the build.

Chain (all already extracted as clean-room assets):
  header parameters 2/3 -- visible width/height in 8px tiles
  header parameter 7    -- order of the two 0x200-tile display windows
  map load table        -- container, palette, three VRAM banks, animation bank
  grid (128x128 backing cells) -- value_low|value_high<<8 --> metatile index
  metatiles.tilemap     -- variable-length 2x2 u16 tile-entry table
  tile entry            -- 10-bit index, H/V flip flags, 4-bit palette bank
  index half            -- 0x000-0x1ff / 0x200-0x3ff selected charblock
  charblock.4bpp.png    -- palette-independent canonical 4bpp tile indices
  palette.NNN.png       -- 16 banks x 16 colours
"""
import argparse
import json
from pathlib import Path

from PIL import Image

from generated_files import prune_files
from map_load_table import records_by_container
from text_bg import decode_entry
from tile_objects import build_bank as build_object_bank, unpack_tile

ROOT = Path(__file__).resolve().parents[1]
TILE = 8
CELL_TILES = 2                      # メタタイルは2×2タイル
CELL = TILE * CELL_TILES           # グリッドセルは16画素
GRID = 128                          # 一辺のセル数
WINDOW = 0x200                      # ソース窓あたりのタイル数
LOAD_TABLE = ROOT / "assets/maps/map_load_table.json"
MANIFEST = ROOT / "assets/manifest.json"


def scene_dimensions(map_dir):
    """Return the header-declared scene size in pixels and backing cells."""
    document = json.loads((map_dir / "components" / "header.json").read_text())
    parameters = document.get("parameters", [])
    if len(parameters) != 12:
        raise ValueError("map header requires twelve parameter bytes")
    width = int(parameters[2]) * TILE
    height = int(parameters[3]) * TILE
    if not 0 < width <= GRID * CELL or not 0 < height <= GRID * CELL:
        raise ValueError("map header dimensions exceed the 128x128-cell grid")
    cells_wide = (width + CELL - 1) // CELL
    cells_high = (height + CELL - 1) // CELL
    return width, height, cells_wide, cells_high


def tile_window_order(map_dir):
    """Return the two loader charblock roles selected by the map header.

    マップチップ切替。Tile indices stay local to the two 0x200-tile display
    windows. The header orders those roles, then the map load table resolves
    each role to the actual shared or local compressed resource.
    """
    document = json.loads((map_dir / "components" / "header.json").read_text())
    parameters = document.get("parameters", [])
    if len(parameters) != 12:
        raise ValueError("map header requires twelve parameter bytes")
    selector = int(parameters[7])
    if selector not in (0, 1):
        raise ValueError("map header tile-window selector must be zero or one")
    return (2, 1) if selector == 0 else (1, 2)


def load_bank_indices(path):
    """Return (width_tiles, height_tiles, [tile_index_grid]) of 4bpp indices."""
    image = Image.open(path).convert("P")
    width, height = image.size
    pixels = image.load()
    tiles = []
    for ty in range(height // TILE):
        for tx in range(width // TILE):
            block = []
            for py in range(TILE):
                row = []
                for px in range(TILE):
                    row.append(pixels[tx * TILE + px, ty * TILE + py] & 0x0F)
                block.append(row)
            tiles.append(block)
    return tiles


def graphics_catalog():
    """Index each tracked palette and tile resource by its loader resource id."""
    manifest = json.loads(MANIFEST.read_text())
    palettes = {}
    banks = {}
    for series in manifest.get("series", []):
        kind = series.get("kind")
        if kind == "golden-sun-map-charblock-series":
            for family in series["families"]:
                base = int(family["id"], 16)
                directory = ROOT / f"assets/graphics/resource_{family['id']}"
                palettes[f"{base + 1:x}"] = directory / "palette.224.png"
                for number, resource in enumerate(family["charblocks"], 1):
                    source = resource.get("source")
                    banks[f"{base + 1 + number:x}"] = (
                        ROOT / source if source else
                        directory / f"charblock{number}.4bpp.png")
                animation = family.get("animation_source")
                if animation is not None:
                    banks[f"{base + 5:x}"] = (
                        directory / "animation_source.4bpp.png")
        elif kind == "golden-sun-standalone-palette-series":
            for palette in series["palettes"]:
                resource = str(palette["id"]).lower()
                palettes[resource] = (
                    ROOT / f"assets/graphics/resource_{resource}/palette.224.png")
        elif kind == "golden-sun-standalone-tile-series":
            for bank in series["resources"]:
                resource = str(bank["id"]).lower()
                banks[resource] = (
                    ROOT / f"assets/graphics/resource_{resource}/tiles.4bpp.png")
    return palettes, banks


def load_bank_source(source):
    """Load a neutral atlas or a bank assembled from coherent objects."""
    if source.suffix == ".json":
        packed, _, _ = build_object_bank(source)
        return [
            [unpack_tile(packed[start:start + 32])[row * TILE:(row + 1) * TILE]
             for row in range(TILE)]
            for start in range(0, len(packed), 32)
        ]
    return load_bank_indices(source)


def load_palette(path):
    image = Image.open(path).convert("RGB")
    width, height = image.size
    colors = []
    for y in range(height):
        for x in range(width):
            colors.append(image.getpixel((x, y)))
    return colors                    # 256色：バンク×16＋色番号


def parse_metatiles(path):
    entries = [int(token, 16)
               for line in path.read_text().split("\n") if line.strip()
               for token in line.split()]
    return [entries[i:i + 4] for i in range(0, len(entries), 4)]


def cell_indices(grid_dir, metatile_count):
    # セル番号は下位12bit。上位4bitは独立した属性値である。
    # 単一マップから番号幅を推定すると系列全体の分離規則を隠してしまう。
    low = Image.open(grid_dir / "value_low.png").convert("P").load()
    high = Image.open(grid_dir / "value_high.png").convert("P").load()
    cells = [[low[x, y] | ((high[x, y] & 0x0F) << 8)
              for x in range(GRID)] for y in range(GRID)]
    if any(value >= metatile_count for row in cells for value in row):
        raise ValueError("map grid references a metatile outside its table")
    return cells


def compose(map_dir, linkage):
    palettes, banks = graphics_catalog()
    sources = {
        window: load_bank_source(banks[resource])
        for window, charblock in enumerate(tile_window_order(map_dir))
        if (resource := linkage[f"vram_charblock{charblock}"].lower()) in banks
    }
    palette_path = palettes[linkage["palette"].lower()]
    palette = load_palette(palette_path)
    metatiles = parse_metatiles(map_dir / "components" / "metatiles.tilemap")
    cells = cell_indices(map_dir / "grid", len(metatiles))
    width, height, cells_wide, cells_high = scene_dimensions(map_dir)

    canvas = Image.new("RGB", (width, height), (0, 0, 0))
    out = canvas.load()
    missing_tiles = 0
    for cy in range(cells_high):
        for cx in range(cells_wide):
            mt = cells[cy][cx]
            if mt >= len(metatiles):
                continue
            quad = metatiles[mt]
            for sub in range(4):
                entry = quad[sub]
                index, bank, hflip, vflip = decode_entry(entry)
                window = index // WINDOW
                local = index % WINDOW
                if window in sources and local < len(sources[window]):
                    tile = sources[window][local]
                else:
                    missing_tiles += 1
                    continue
                ox = cx * CELL + (sub % CELL_TILES) * TILE
                oy = cy * CELL + (sub // CELL_TILES) * TILE
                for py in range(TILE):
                    for px in range(TILE):
                        if ox + px >= width or oy + py >= height:
                            continue
                        sx = TILE - 1 - px if hflip else px
                        sy = TILE - 1 - py if vflip else py
                        color = palette[(bank * 16 + tile[sy][sx]) & 0xFF]
                        out[ox + px, oy + py] = color
    return canvas, missing_tiles


def renderable_records():
    """Return loader records whose reconstructed map and graphics all exist."""
    palettes, banks = graphics_catalog()
    records = []
    for rows in records_by_container(LOAD_TABLE).values():
        for row in rows:
            cid = row["container"].lower()
            map_dir = ROOT / f"assets/maps/resource_{cid}"
            order = tile_window_order(map_dir) if map_dir.exists() else ()
            if (map_dir.exists() and row["palette"].lower() in palettes and
                    all(row[f"vram_charblock{number}"].lower() in banks
                        for number in order)):
                records.append(row)
    return records


def select_record(cid, map_index=None):
    rows = records_by_container(LOAD_TABLE).get(cid.lower(), [])
    if map_index is not None:
        rows = [row for row in rows if int(row["map_index"]) == map_index]
    if not rows:
        raise ValueError(f"no map load record for container {cid}")
    if len(rows) > 1 and map_index is None:
        indices = ", ".join(str(row["map_index"]) for row in rows)
        raise ValueError(
            f"container {cid} has map variants {indices}; give --map-index")
    return rows[0]


def render(cid, out_dir, crop=True, linkage=None, filename=None):
    map_dir = ROOT / f"assets/maps/resource_{cid}"
    linkage = linkage or select_record(cid)
    canvas, missing = compose(map_dir, linkage)
    if crop:
        from PIL import ImageChops
        bbox = ImageChops.difference(
            canvas, Image.new("RGB", canvas.size, (0, 0, 0))).getbbox()
        if bbox:
            canvas = canvas.crop(bbox)
    out_dir.mkdir(parents=True, exist_ok=True)
    output = out_dir / (filename or f"scene_{cid}.png")
    canvas.save(output)
    return output, canvas.size, missing


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("container", nargs="?", help="hex id, e.g. 152")
    parser.add_argument("-o", "--output", type=Path)
    parser.add_argument("--map-index", type=int,
                        help="select a loader-table variant for this container")
    parser.add_argument("--all", action="store_true",
                        help="render every loader-linked map to out/scenes/")
    args = parser.parse_args()
    out_dir = ROOT / "out/scenes"
    if args.all:
        records = renderable_records()
        counts = {}
        for row in records:
            cid = row["container"].lower()
            counts[cid] = counts.get(cid, 0) + 1
        expected = set()
        print(f"rendering {len(records)} loader-linked map scenes to {out_dir}/")
        for row in records:
            cid = row["container"].lower()
            filename = (f"scene_{cid}.png" if counts[cid] == 1 else
                        f"scene_{cid}_map{int(row['map_index']):03d}.png")
            expected.add(filename)
            output, size, missing = render(
                cid, out_dir, linkage=row, filename=filename)
            print(f"  {cid} map={row['map_index']}: {size[0]}x{size[1]} "
                  f"(unresolved tiles={missing})")
        removed = prune_files(out_dir, "scene_*.png", expected)
        if removed:
            print(f"pruned stale scenes={len(removed)}")
        return
    if not args.container:
        parser.error("give a container id or --all")
    linkage = select_record(args.container, args.map_index)
    output, size, missing = render(args.container, out_dir, linkage=linkage)
    if args.output:
        Image.open(output).save(args.output)
        output = args.output
    print(f"wrote {output} ({size[0]}x{size[1]}), "
          f"unresolved tiles={missing}")


if __name__ == "__main__":
    main()
