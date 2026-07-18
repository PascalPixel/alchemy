#!/usr/bin/env python3
"""Compose a map container's byte-exact tile banks into a viewable scene preview.

This is DISPLAY-ONLY. The byte-exact charblock/grid/metatile assets remain the
build source; this tool reads them and paints the composed 2D scene the map
actually renders, so a reviewer sees "four trees in a row" instead of a sheet of
tile fragments. It changes no build inputs and is never read back by the build.

Chain (all already extracted as clean-room assets):
  grid (128x128 cells)  -- value_low|value_high<<8 --> metatile index
  metatiles.tilemap     -- 492 metatiles, each 2x2 u16 tile entries
  tile entry            -- index = entry & 0x0fff, palette bank = entry >> 12
  index window          -- 0x000-0x1ff cb1, 0x200-0x3ff cb2, 0x400-0x5ff cb3,
                           0x600-0x7ff animation, 0x800-0xfff shared tileset
  charblock.banked.png  -- 8bpp where pixel & 0x0f is the real 4bpp index
  palette.NNN.png       -- 16 banks x 16 colours
"""
import argparse
import json
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
TILE = 8
CELL_TILES = 2                      # metatile is 2x2 tiles
CELL = TILE * CELL_TILES           # 16 px per grid cell
GRID = 128                          # cells per side
WINDOW = 0x200                      # tiles per source window


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


def load_palette(path):
    image = Image.open(path).convert("RGB")
    width, height = image.size
    colors = []
    for y in range(height):
        for x in range(width):
            colors.append(image.getpixel((x, y)))
    return colors                    # 256 entries: bank*16 + index


def parse_metatiles(path):
    entries = [int(token, 16)
               for line in path.read_text().split("\n") if line.strip()
               for token in line.split()]
    return [entries[i:i + 4] for i in range(0, len(entries), 4)]


def cell_indices(grid_dir, metatile_count):
    # A cell's metatile index is value_low plus the low bits of value_high. Only
    # ceil(log2(metatile_count)) - 8 high bits are index; the remaining high bits
    # (e.g. 0x40/0x80 seen in the wild) are layer/priority attributes. Deriving
    # the width from the actual metatile count is essential: maps range from ~492
    # metatiles (1 high bit) to ~2250 (4 high bits), so a fixed mask mis-decodes
    # every cell whose index exceeds the assumed width.
    import math
    high_bits = max(0, math.ceil(math.log2(metatile_count)) - 8) if metatile_count > 1 else 0
    high_mask = (1 << high_bits) - 1
    low = Image.open(grid_dir / "value_low.png").convert("P").load()
    high = Image.open(grid_dir / "value_high.png").convert("P").load()
    return [[low[x, y] | ((high[x, y] & high_mask) << 8) for x in range(GRID)]
            for y in range(GRID)]


def standalone_banks():
    """The globally shared map tilesets, in resource order, grouped into runs
    of adjacent resources. A map loads one run at virtual tile base 0x800."""
    manifest = json.loads((ROOT / "assets/manifest.json").read_text())
    ids = []
    for series in manifest.get("series", []):
        if series.get("kind") == "golden-sun-standalone-tile-series":
            ids = [entry["id"] for entry in series["resources"]]
    runs, current = [], []
    for rid in ids:
        if current and int(rid, 16) != int(current[-1], 16) + 1:
            runs.append(current)
            current = []
        current.append(rid)
    if current:
        runs.append(current)
    return runs


def load_shared(ids):
    tiles = []
    for rid in ids:
        path = ROOT / f"assets/graphics/resource_{rid}/tiles.4bpp.png"
        if path.exists():
            tiles.extend(load_bank_indices(path))
    return tiles


def pick_shared(map_dir, gfx_dir, metatiles, cells, sources, palette):
    """Choose which shared tileset run this map loads.

    The map data carries no tileset id we have recovered, so select the run
    empirically: the correct tileset's tiles are continuous in colour with the
    map tiles they sit beside, while a wrong one paints blocks that clash. Tile
    STRUCTURE does not discriminate (wrong runs contain structured tiles too);
    colour continuity across the shared/normal boundary does.
    """
    best, best_score = None, None
    for run in standalone_banks():
        tiles = load_shared(run)
        if not tiles:
            continue
        total, count = 0.0, 0
        for cy in range(0, GRID, 2):          # sample every other cell: same
            for cx in range(0, GRID, 2):      # ranking, a quarter of the work
                mt = cells[cy][cx]
                if mt >= len(metatiles):
                    continue
                mine = cell_colour(metatiles[mt], sources, tiles, palette)
                if mine is None or not mine[1]:
                    continue
                for dx, dy in ((1, 0), (0, 1)):
                    nx, ny = cx + dx, cy + dy
                    if not (0 <= nx < GRID and 0 <= ny < GRID):
                        continue
                    nmt = cells[ny][nx]
                    if nmt >= len(metatiles):
                        continue
                    other = cell_colour(metatiles[nmt], sources, tiles, palette)
                    if other is None or other[1]:
                        continue
                    total += sum(abs(a - b) for a, b in zip(mine[0], other[0]))
                    count += 1
        if count:
            score = total / count
            if best_score is None or score < best_score:
                best, best_score = tiles, score
    return best


def cell_colour(quad, sources, shared, palette):
    """(mean RGB, uses_shared) for one metatile, or None if nothing painted."""
    acc = [0, 0, 0]
    seen = 0
    uses_shared = False
    for entry in quad:
        index = entry & 0x0FFF
        bank = entry >> 12
        window, local = index // WINDOW, index % WINDOW
        if window < 4 and window in sources and local < len(sources[window]):
            tile = sources[window][local]
        elif shared is not None and 0x800 <= index and (index - 0x800) < len(shared):
            tile = shared[index - 0x800]
            uses_shared = True
        else:
            continue
        for row in tile:
            for value in row:
                red, green, blue = palette[(bank * 16 + value) & 0xFF]
                acc[0] += red
                acc[1] += green
                acc[2] += blue
                seen += 1
    if not seen:
        return None
    return ([channel / seen for channel in acc], uses_shared)


def compose(map_dir, gfx_dir, shared_tiles=None):
    window_files = {
        0: "charblock1.banked.png",
        1: "charblock2.banked.png",
        2: "charblock3.banked.png",
        3: "animation_source.banked.png",
    }
    sources = {w: load_bank_indices(gfx_dir / name)
               for w, name in window_files.items()
               if (gfx_dir / name).exists()}
    palette_path = next(gfx_dir.glob("palette.*.png"))
    palette = load_palette(palette_path)
    metatiles = parse_metatiles(map_dir / "components" / "metatiles.tilemap")
    cells = cell_indices(map_dir / "grid", len(metatiles))
    if shared_tiles is None:
        shared_tiles = pick_shared(map_dir, gfx_dir, metatiles, cells,
                                   sources, palette)

    canvas = Image.new("RGB", (GRID * CELL, GRID * CELL), (0, 0, 0))
    out = canvas.load()
    missing_shared = 0
    for cy in range(GRID):
        for cx in range(GRID):
            mt = cells[cy][cx]
            if mt >= len(metatiles):
                continue
            quad = metatiles[mt]
            for sub in range(4):
                entry = quad[sub]
                index = entry & 0x0FFF
                bank = entry >> 12
                window = index // WINDOW
                local = index % WINDOW
                if window < 4 and window in sources and local < len(sources[window]):
                    tile = sources[window][local]
                elif (shared_tiles is not None and index >= 0x800
                      and (index - 0x800) < len(shared_tiles)):
                    tile = shared_tiles[index - 0x800]
                else:
                    missing_shared += 1
                    continue
                ox = cx * CELL + (sub % CELL_TILES) * TILE
                oy = cy * CELL + (sub // CELL_TILES) * TILE
                for py in range(TILE):
                    for px in range(TILE):
                        color = palette[(bank * 16 + tile[py][px]) & 0xFF]
                        out[ox + px, oy + py] = color
    return canvas, missing_shared


def self_contained_maps():
    """Map containers that own their charblocks (renderable without the shared
    tileset / cross-container family linkage, which is not yet recovered)."""
    ids = []
    for grid in sorted((ROOT / "assets/maps").glob("resource_*/grid")):
        cid = grid.parent.name.split("_")[1]
        gfx = ROOT / f"assets/graphics/resource_{cid}"
        if (gfx / "charblock1.banked.png").exists():
            ids.append(cid)
    return ids


def render(cid, out_dir, crop=True):
    map_dir = ROOT / f"assets/maps/resource_{cid}"
    gfx_dir = ROOT / f"assets/graphics/resource_{cid}"
    canvas, missing = compose(map_dir, gfx_dir)
    if crop:
        from PIL import ImageChops
        bbox = ImageChops.difference(
            canvas, Image.new("RGB", canvas.size, (0, 0, 0))).getbbox()
        if bbox:
            canvas = canvas.crop(bbox)
    out_dir.mkdir(parents=True, exist_ok=True)
    output = out_dir / f"scene_{cid}.png"
    canvas.save(output)
    return output, canvas.size, missing


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("container", nargs="?", help="hex id, e.g. 152")
    parser.add_argument("-o", "--output", type=Path)
    parser.add_argument("--all", action="store_true",
                        help="render every self-contained map to out/scenes/")
    args = parser.parse_args()
    out_dir = ROOT / "out/scenes"
    if args.all:
        ids = self_contained_maps()
        print(f"rendering {len(ids)} self-contained map scenes to {out_dir}/")
        for cid in ids:
            output, size, missing = render(cid, out_dir)
            print(f"  {cid}: {size[0]}x{size[1]} (unresolved shared refs={missing})")
        return
    if not args.container:
        parser.error("give a container id or --all")
    output, size, missing = render(args.container, out_dir)
    if args.output:
        Image.open(output).save(args.output)
        output = args.output
    print(f"wrote {output} ({size[0]}x{size[1]}), "
          f"unresolved shared-tile refs={missing}")


if __name__ == "__main__":
    main()
