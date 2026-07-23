#!/usr/bin/env bun
// Tool role: both; imported by tools/bg_tile_usage.ts, tools/tile_objects.ts; invoked by CONVENTIONS.md.
/**
 * 地図コンテナの正規タイル素材を、確認用の一枚絵として合成する。
 *
 * 出力画像は表示確認専用であり、構築入力には使わない。
 * ヘッダ、読込表、グリッド、メタタイル、文字ブロック、パレットを
 * 実機の表示順に重ね、反転指定も各タイルへ適用する。
 */
import {
  copyFileSync,
  existsSync,
  mkdirSync,
  readFileSync,
  writeFileSync,
} from "node:fs";
import { dirname, extname, join, resolve } from "node:path";
import { deflateSync } from "./zlib.ts";

import { chunk } from "./export_asset.ts";
import { resourceGraphicsDir } from "./asset_paths.ts";
import { prune_files } from "./generated_files.ts";
import { indexed_png, type Rgb } from "./import_asset.ts";
import { records_by_container } from "./map_load_table.ts";
import { decode_entry } from "./text_bg.ts";
import { build_bank as build_object_bank, unpack_tile } from "./tile_objects.ts";

export const ROOT = resolve(dirname(Bun.fileURLToPath(import.meta.url)), "..");
export const TILE = 8;
export const CELL_TILES = 2;
export const CELL = TILE * CELL_TILES;
export const GRID = 128;
export const WINDOW = 0x200;
export const LOAD_TABLE = join(ROOT, "assets/maps/map_load_table.json");
export const MANIFEST = join(ROOT, "assets/manifest.json");

export type Tile = number[][];
export type Color = Rgb;
export type Linkage = {
  map_index: number;
  container: string;
  palette: string;
  vram_charblock1: string;
  vram_charblock2: string;
  vram_charblock3: string;
  animation_source: string;
};

export interface SceneImage {
  width: number;
  height: number;
  pixels: Buffer;
}

type Catalog = [Record<string, string>, Record<string, string>];
type SceneDimensions = [number, number, number, number];
type WindowOrder = [number, number];
type RenderResult = [string, [number, number], number];

function read_json(path: string): any {
  return JSON.parse(readFileSync(path, "utf8"));
}

/** ヘッダに記録された画面寸法と必要セル数を返す。 */
export function scene_dimensions(map_dir: string): SceneDimensions {
  const document = read_json(join(map_dir, "components/header.json"));
  const parameters = document.parameters ?? [];
  if (parameters.length !== 12) {
    throw new Error("map header requires twelve parameter bytes");
  }
  const width = Number(parameters[2]) * TILE;
  const height = Number(parameters[3]) * TILE;
  if (!(width > 0 && width <= GRID * CELL) || !(height > 0 && height <= GRID * CELL)) {
    throw new Error("map header dimensions exceed the 128x128-cell grid");
  }
  const cells_wide = Math.floor((width + CELL - 1) / CELL);
  const cells_high = Math.floor((height + CELL - 1) / CELL);
  return [width, height, cells_wide, cells_high];
}

/** ヘッダ指定から二個の表示窓へ割り当てる文字ブロック順を返す。 */
export function tile_window_order(map_dir: string): WindowOrder {
  const document = read_json(join(map_dir, "components/header.json"));
  const parameters = document.parameters ?? [];
  if (parameters.length !== 12) {
    throw new Error("map header requires twelve parameter bytes");
  }
  const selector = Number(parameters[7]);
  if (selector !== 0 && selector !== 1) {
    throw new Error("map header tile-window selector must be zero or one");
  }
  return selector === 0 ? [2, 1] : [1, 2];
}

/** 4bpp索引画像を8画素単位のタイル列として読む。 */
export function load_bank_indices(path: string): Tile[] {
  const [width, height, pixels] = indexed_png(readFileSync(path));
  const tiles: Tile[] = [];
  for (let ty = 0; ty < Math.floor(height / TILE); ty += 1) {
    for (let tx = 0; tx < Math.floor(width / TILE); tx += 1) {
      const block: Tile = [];
      for (let py = 0; py < TILE; py += 1) {
        const row: number[] = [];
        for (let px = 0; px < TILE; px += 1) {
          row.push(pixels[(ty * TILE + py) * width + tx * TILE + px] & 0x0f);
        }
        block.push(row);
      }
      tiles.push(block);
    }
  }
  return tiles;
}

/** 素材目録を読込資源番号ごとのパレット表と文字表へ展開する。 */
export function graphics_catalog(): Catalog {
  const manifest = read_json(MANIFEST);
  const palettes: Record<string, string> = {};
  const banks: Record<string, string> = {};
  for (const series of manifest.series ?? []) {
    const kind = series.kind;
    if (kind === "golden-sun-map-charblock-series") {
      for (const family of series.families) {
        const base = Number.parseInt(family.id, 16);
        const directory = join(ROOT, resourceGraphicsDir(family.id));
        palettes[(base + 1).toString(16)] = join(directory, "palette.224.png");
        for (const [index, resource] of family.charblocks.entries()) {
          banks[(base + 2 + index).toString(16)] = resource.source
            ? join(ROOT, resource.source)
            : join(directory, `charblock${index + 1}.4bpp.png`);
        }
        if (family.animation_source !== undefined && family.animation_source !== null) {
          banks[(base + 5).toString(16)] = join(directory, "animation_source.4bpp.png");
        }
      }
    } else if (kind === "golden-sun-standalone-palette-series") {
      for (const palette of series.palettes) {
        const resource = String(palette.id).toLowerCase();
        palettes[resource] = join(ROOT, resourceGraphicsDir(resource), "palette.224.png");
      }
    } else if (kind === "golden-sun-standalone-tile-series") {
      for (const bank of series.resources) {
        const resource = String(bank.id).toLowerCase();
        banks[resource] = join(ROOT, resourceGraphicsDir(resource), "tiles.4bpp.png");
      }
    }
  }
  return [palettes, banks];
}

/** 中立タイル画像または物体計画から表示用タイル列を読む。 */
export function load_bank_source(source: string): Tile[] {
  if (extname(source) === ".json") {
    const [packed] = build_object_bank(source);
    const tiles: Tile[] = [];
    for (let start = 0; start < packed.length; start += 32) {
      const flat = unpack_tile(packed.subarray(start, start + 32));
      const tile: Tile = [];
      for (let row = 0; row < TILE; row += 1) {
        tile.push(flat.slice(row * TILE, (row + 1) * TILE));
      }
      tiles.push(tile);
    }
    return tiles;
  }
  return load_bank_indices(source);
}

/** パレット画像を画素順の256色表として読む。 */
export function load_palette(path: string): Color[] {
  const [, , pixels, palette] = indexed_png(readFileSync(path));
  return pixels.map((index) => palette[index]);
}

/** 四要素ずつ並んだメタタイル表を読む。 */
export function parse_metatiles(path: string): number[][] {
  const entries = readFileSync(path, "utf8")
    .split("\n")
    .filter((line) => line.trim().length > 0)
    .flatMap((line) => line.trim().split(/\s+/).map((token) => Number.parseInt(token, 16)));
  const metatiles: number[][] = [];
  for (let index = 0; index < entries.length; index += 4) {
    metatiles.push(entries.slice(index, index + 4));
  }
  return metatiles;
}

/** 128×128グリッドから下位12bitのメタタイル番号を復元する。 */
export function cell_indices(grid_dir: string, metatile_count: number): number[][] {
  const [, , low] = indexed_png(readFileSync(join(grid_dir, "value_low.png")));
  const [, , high] = indexed_png(readFileSync(join(grid_dir, "value_high.png")));
  const cells: number[][] = [];
  for (let y = 0; y < GRID; y += 1) {
    const row: number[] = [];
    for (let x = 0; x < GRID; x += 1) {
      row.push(low[y * GRID + x] | (high[y * GRID + x] & 0x0f) << 8);
    }
    cells.push(row);
  }
  if (cells.some((row) => row.some((value) => value >= metatile_count))) {
    throw new Error("map grid references a metatile outside its table");
  }
  return cells;
}

/** 一件の地図を反転指定とパレット指定を含めて合成する。 */
export function compose(map_dir: string, linkage: Linkage): [SceneImage, number] {
  const [palettes, banks] = graphics_catalog();
  const sources = new Map<number, Tile[]>();
  for (const [window, charblock] of tile_window_order(map_dir).entries()) {
    const resource = linkage[`vram_charblock${charblock}` as keyof Linkage];
    if (typeof resource === "string" && banks[resource.toLowerCase()] !== undefined) {
      sources.set(window, load_bank_source(banks[resource.toLowerCase()]));
    }
  }
  const palette_path = palettes[linkage.palette.toLowerCase()];
  const palette = load_palette(palette_path);
  const metatiles = parse_metatiles(join(map_dir, "components/metatiles.tilemap"));
  const cells = cell_indices(join(map_dir, "grid"), metatiles.length);
  const [width, height, cells_wide, cells_high] = scene_dimensions(map_dir);
  const pixels = Buffer.alloc(width * height * 3);
  let missing_tiles = 0;
  for (let cy = 0; cy < cells_high; cy += 1) {
    for (let cx = 0; cx < cells_wide; cx += 1) {
      const metatile = cells[cy][cx];
      if (metatile >= metatiles.length) {
        continue;
      }
      const quad = metatiles[metatile];
      for (let sub = 0; sub < 4; sub += 1) {
        const [index, bank, hflip, vflip] = decode_entry(quad[sub]);
        const window = Math.floor(index / WINDOW);
        const local = index % WINDOW;
        const source = sources.get(window);
        if (source === undefined || local >= source.length) {
          missing_tiles += 1;
          continue;
        }
        const tile = source[local];
        const ox = cx * CELL + sub % CELL_TILES * TILE;
        const oy = cy * CELL + Math.floor(sub / CELL_TILES) * TILE;
        for (let py = 0; py < TILE; py += 1) {
          for (let px = 0; px < TILE; px += 1) {
            if (ox + px >= width || oy + py >= height) {
              continue;
            }
            const sx = hflip ? TILE - 1 - px : px;
            const sy = vflip ? TILE - 1 - py : py;
            const color = palette[(bank * 16 + tile[sy][sx]) & 0xff];
            if (color === undefined) {
              throw new Error("map tile references a missing palette color");
            }
            pixels.set(color, ((oy + py) * width + ox + px) * 3);
          }
        }
      }
    }
  }
  return [{ width, height, pixels }, missing_tiles];
}

/** 黒以外の画素を囲む最小矩形へ切り詰める。 */
function crop_black(image: SceneImage): SceneImage {
  let left = image.width;
  let top = image.height;
  let right = -1;
  let bottom = -1;
  for (let y = 0; y < image.height; y += 1) {
    for (let x = 0; x < image.width; x += 1) {
      const offset = (y * image.width + x) * 3;
      if (image.pixels[offset] !== 0 || image.pixels[offset + 1] !== 0 || image.pixels[offset + 2] !== 0) {
        left = Math.min(left, x);
        top = Math.min(top, y);
        right = Math.max(right, x);
        bottom = Math.max(bottom, y);
      }
    }
  }
  if (right < left || bottom < top) {
    return image;
  }
  const width = right - left + 1;
  const height = bottom - top + 1;
  const pixels = Buffer.alloc(width * height * 3);
  for (let y = 0; y < height; y += 1) {
    const source = ((top + y) * image.width + left) * 3;
    image.pixels.copy(pixels, y * width * 3, source, source + width * 3);
  }
  return { width, height, pixels };
}

/** RGB画素を非交差PNGとして保存する。 */
function write_rgb_png(path: string, image: SceneImage): void {
  const header = Buffer.alloc(13);
  header.writeUInt32BE(image.width, 0);
  header.writeUInt32BE(image.height, 4);
  header.set([8, 2, 0, 0, 0], 8);
  const rows: Buffer[] = [];
  for (let y = 0; y < image.height; y += 1) {
    rows.push(Buffer.from([0]));
    rows.push(image.pixels.subarray(y * image.width * 3, (y + 1) * image.width * 3));
  }
  const data = Buffer.concat([
    Buffer.from("89504e470d0a1a0a", "hex"),
    chunk(Buffer.from("IHDR"), header),
    chunk(Buffer.from("IDAT"), deflateSync(Buffer.concat(rows), { level: 9 })),
    chunk(Buffer.from("IEND"), Buffer.alloc(0)),
  ]);
  writeFileSync(path, data);
}

/** 地図と必要な画像素材がすべて揃う読込表の行だけを返す。 */
export function renderable_records(): Linkage[] {
  const [palettes, banks] = graphics_catalog();
  const records: Linkage[] = [];
  const grouped = records_by_container(LOAD_TABLE);
  const order: string[] = [];
  const seen = new Set<string>();
  for (const row of read_json(LOAD_TABLE).records ?? []) {
    const cid = String(row.container).toLowerCase();
    if (!seen.has(cid)) {
      seen.add(cid);
      order.push(cid);
    }
  }
  for (const cid of order) {
    const rows = grouped[cid] ?? [];
    for (const row of rows) {
      const linkage = row as Linkage;
      const map_dir = join(ROOT, `assets/maps/resource_${cid}`);
      const order = existsSync(map_dir) ? tile_window_order(map_dir) : [];
      if (
        existsSync(map_dir) &&
        palettes[linkage.palette.toLowerCase()] !== undefined &&
        order.every((number) => banks[linkage[`vram_charblock${number}` as keyof Linkage]!.toString().toLowerCase()] !== undefined)
      ) {
        records.push(linkage);
      }
    }
  }
  return records;
}

/** コンテナ番号と任意の地図番号から読込表の一行を選ぶ。 */
export function select_record(cid: string, map_index?: number): Linkage {
  let rows = (records_by_container(LOAD_TABLE)[cid.toLowerCase()] ?? []) as Linkage[];
  if (map_index !== undefined) {
    rows = rows.filter((row) => Number(row.map_index) === map_index);
  }
  if (rows.length === 0) {
    throw new Error(`no map load record for container ${cid}`);
  }
  if (rows.length > 1 && map_index === undefined) {
    const indices = rows.map((row) => String(row.map_index)).join(", ");
    throw new Error(`container ${cid} has map variants ${indices}; give --map-index`);
  }
  return rows[0];
}

/** 一件の地図を出力し、出力先、寸法、未解決タイル数を返す。 */
export function render(
  cid: string,
  out_dir: string,
  crop = true,
  linkage?: Linkage,
  filename?: string,
): RenderResult {
  const map_dir = join(ROOT, `assets/maps/resource_${cid}`);
  const [composed, missing] = compose(map_dir, linkage ?? select_record(cid));
  const canvas = crop ? crop_black(composed) : composed;
  mkdirSync(out_dir, { recursive: true });
  const output = join(out_dir, filename ?? `scene_${cid}.png`);
  write_rgb_png(output, canvas);
  return [output, [canvas.width, canvas.height], missing];
}

function usage(): never {
  throw new Error("give a container id or --all");
}

function print_help(): void {
  console.log(
    "usage: compose_scene.ts [-h] [-o OUTPUT] [--map-index MAP_INDEX] [--all] [container]\n\n" +
    "compose a reconstructed map into a display-only scene preview\n\n" +
    "positional arguments:\n" +
    "  container             hex container id, e.g. 152\n\n" +
    "options:\n" +
    "  -h, --help            show this help message and exit\n" +
    "  -o, --output OUTPUT   copy the rendered scene to OUTPUT\n" +
    "  --map-index MAP_INDEX select a loader-table variant\n" +
    "  --all                 render every loader-linked map",
  );
}

function integer(value: string, option: string): number {
  if (!/^[+-]?\d+$/.test(value)) {
    throw new Error(`${option} requires an integer`);
  }
  return Number(value);
}

/** 表示用地図画像の生成命令を処理する。 */
export function main(arguments_ = Bun.argv.slice(2)): void {
  if (arguments_.includes("-h") || arguments_.includes("--help")) {
    print_help();
    return;
  }
  let container: string | undefined;
  let output_path: string | undefined;
  let map_index: number | undefined;
  let render_all = false;
  for (let index = 0; index < arguments_.length; index += 1) {
    const argument = arguments_[index];
    if (argument === "--all") {
      render_all = true;
    } else if (argument === "-o" || argument === "--output") {
      if (index + 1 >= arguments_.length) {
        throw new Error(`${argument} requires a value`);
      }
      output_path = arguments_[++index];
    } else if (argument.startsWith("--output=")) {
      output_path = argument.slice("--output=".length);
    } else if (argument === "--map-index") {
      if (index + 1 >= arguments_.length) {
        throw new Error("--map-index requires a value");
      }
      map_index = integer(arguments_[++index], "--map-index");
    } else if (argument.startsWith("--map-index=")) {
      map_index = integer(argument.slice("--map-index=".length), "--map-index");
    } else if (argument.startsWith("-")) {
      throw new Error(`unrecognized argument: ${argument}`);
    } else if (container === undefined) {
      container = argument;
    } else {
      throw new Error(`unrecognized argument: ${argument}`);
    }
  }

  const out_dir = join(ROOT, "out/scenes");
  if (render_all) {
    const records = renderable_records();
    const counts = new Map<string, number>();
    for (const row of records) {
      const cid = row.container.toLowerCase();
      counts.set(cid, (counts.get(cid) ?? 0) + 1);
    }
    const expected = new Set<string>();
    console.log(`rendering ${records.length} loader-linked map scenes to ${out_dir}/`);
    for (const row of records) {
      const cid = row.container.toLowerCase();
      const filename = counts.get(cid) === 1
        ? `scene_${cid}.png`
        : `scene_${cid}_map${Number(row.map_index).toString().padStart(3, "0")}.png`;
      expected.add(filename);
      const [, size, missing] = render(cid, out_dir, true, row, filename);
      console.log(`  ${cid} map=${row.map_index}: ${size[0]}x${size[1]} (unresolved tiles=${missing})`);
    }
    const removed = prune_files(out_dir, "scene_*.png", expected);
    if (removed.length > 0) {
      console.log(`pruned stale scenes=${removed.length}`);
    }
    return;
  }
  if (container === undefined) {
    usage();
  }
  const linkage = select_record(container, map_index);
  let [output, size, missing] = render(container, out_dir, true, linkage);
  if (output_path !== undefined) {
    copyFileSync(output, output_path);
    output = output_path;
  }
  console.log(`wrote ${output} (${size[0]}x${size[1]}), unresolved tiles=${missing}`);
}

if (import.meta.main) {
  main();
}
