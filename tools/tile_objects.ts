#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts, tools/compose_scene.ts; invoked by CONVENTIONS.md, assets/README.md.
import { canonicalJson } from "./canonical_json.ts";
/**
 * 地図上で合成した物体画像から4bppタイルバンクを正確に構築する。
 *
 * 物体画像に隣接するタイルマップが格納位置、パレット、反転指定を保持する。
 * 構築時には表示用反転を戻し、正規形のタイルを元の位置へ書き戻す。
 * 未移行位置だけは残余画像から補う。
 */
import {
  existsSync,
  mkdirSync,
  readFileSync,
  unlinkSync,
  writeFileSync,
} from "node:fs";
import { basename, dirname, isAbsolute, join, relative, resolve } from "node:path";

import { resourceGraphicsDir } from "./asset_paths.ts";
import { tile_png } from "./export_asset.ts";
import { gba_graphics, indexed_png } from "./import_asset.ts";
import { decode_entry, flip_tile } from "./text_bg.ts";
import { export_tilemap, import_tilemap } from "./tilemap.ts";
import {
  cell_indices,
  parse_metatiles,
  scene_dimensions,
  tile_window_order,
} from "./compose_scene.ts";

export const ROOT = resolve(dirname(Bun.fileURLToPath(import.meta.url)), "..");
export const TILE = 8;
export const PACKED_TILE = 32;
export const SOURCE_COLUMNS = 32;

type JsonObject = Record<string, any>;
type Rectangle = [number, number, number, number];
type Color = [number, number, number];

export interface BankReport {
  tiles: number;
  object_tiles: number;
  placements: number;
  horizontal_flips: number;
  vertical_flips: number;
  objects: number;
}

export type BankResult = [Buffer, string[], BankReport];

function parse_base_zero(value: string): number {
  const text = value.trim();
  if (!/^[+-]?(?:0[xX][0-9a-fA-F]+|0[bB][01]+|0[oO][0-7]+|0|[1-9][0-9]*)$/.test(text)) {
    throw new Error(`invalid integer: ${value}`);
  }
  const parsed = Number(text);
  if (!Number.isSafeInteger(parsed)) {
    throw new Error(`integer is outside the safe range: ${value}`);
  }
  return parsed;
}

function parse_decimal(value: string): number {
  const text = value.trim();
  if (!/^[+-]?[0-9]+$/.test(text)) {
    throw new Error(`invalid integer: ${value}`);
  }
  const parsed = Number(text);
  if (!Number.isSafeInteger(parsed)) {
    throw new Error(`integer is outside the safe range: ${value}`);
  }
  return parsed;
}

export function number(value: string | number): number {
  if (typeof value === "string") {
    return parse_base_zero(value);
  }
  if (!Number.isSafeInteger(value)) {
    throw new Error(`invalid integer: ${value}`);
  }
  return value;
}

export function child(plan_path: string, name: string): string {
  const parent = resolve(dirname(plan_path));
  // Flat layout: siblings share the plan's resource prefix (text before the
  // final component name, e.g. map_resource_152_charblock2.objects.json ->
  // map_resource_152_).
  const base = basename(plan_path);
  const marker = base.lastIndexOf("_");
  const prefix = marker >= 0 ? base.slice(0, marker + 1) : "";
  const flat = resolve(parent, prefix + name.replace(/\//g, "_"));
  const path = existsSync(flat) ? flat : resolve(parent, name);
  const local = relative(parent, path);
  if (!local || local === ".." || local.startsWith(`..${process.platform === "win32" ? "\\" : "/"}`) || isAbsolute(local)) {
    throw new Error("object-bank source must stay beside its plan");
  }
  return path;
}

function json_text(value: unknown): string {
  const text = canonicalJson(value);
  if (text === undefined) {
    throw new Error("value cannot be represented as JSON");
  }
  let escaped = "";
  for (let index = 0; index < text.length; index += 1) {
    const code = text.charCodeAt(index);
    escaped += code > 0x7f ? `\\u${code.toString(16).padStart(4, "0")}` : text[index];
  }
  return `${escaped}\n`;
}

function read_json(path: string): JsonObject {
  return JSON.parse(readFileSync(path, "utf8")) as JsonObject;
}

function equal_bytes(left: Uint8Array, right: Uint8Array): boolean {
  return left.length === right.length && Buffer.from(left).equals(Buffer.from(right));
}

function equal_colors(left: Color[], right: Color[]): boolean {
  return left.length === right.length && left.every((color, index) => (
    color.length === right[index].length &&
    color.every((channel, channel_index) => channel === right[index][channel_index])
  ));
}

export function select_manifest_source(
  container: string,
  charblock: number,
  plan_path: string,
): void {
  const manifest_path = join(ROOT, "assets/manifest.json");
  const manifest = read_json(manifest_path);
  for (const series of manifest.series ?? []) {
    if (series.kind !== "golden-sun-map-charblock-series") {
      continue;
    }
    for (const family of series.families ?? []) {
      if (family.id !== container) {
        continue;
      }
      const banks = family.charblocks ?? [];
      if (charblock < 1 || charblock > banks.length) {
        throw new Error("manifest family does not contain this charblock");
      }
      banks[charblock - 1].source = relative(ROOT, resolve(plan_path)).split("\\").join("/");
      writeFileSync(manifest_path, json_text(manifest));
      return;
    }
  }
  throw new Error("map graphics family is absent from the asset manifest");
}

export function pack_tile(pixels: ArrayLike<number>): Buffer {
  const values = Array.from(pixels);
  if (
    values.length !== TILE * TILE ||
    values.some((value) => !Number.isInteger(value) || value < 0 || value >= 16)
  ) {
    throw new Error("object tile does not contain 64 4bpp indices");
  }
  const packed = Buffer.alloc(PACKED_TILE);
  for (let index = 0; index < values.length; index += 2) {
    packed[index >> 1] = values[index] | (values[index + 1] << 4);
  }
  return packed;
}

export function unpack_tile(data: Uint8Array): number[] {
  if (data.length !== PACKED_TILE) {
    throw new Error("packed 4bpp tile is not 32 bytes");
  }
  return Array.from(data).flatMap((value) => [value & 15, value >> 4]);
}

export function image_tile(
  pixels: ArrayLike<number>,
  width: number,
  cell: number,
): number[] {
  const columns = Math.floor(width / TILE);
  const left = (cell % columns) * TILE;
  const top = Math.floor(cell / columns) * TILE;
  const output: number[] = [];
  for (let y = 0; y < TILE; y += 1) {
    for (let x = 0; x < TILE; x += 1) {
      output.push(pixels[(top + y) * width + left + x]);
    }
  }
  return output;
}

/** 物体計画から圧縮前バンク、参照元一覧、集計を返す。 */
export function build_bank(plan_path_value: string): BankResult {
  const plan_path = resolve(plan_path_value);
  const plan = read_json(plan_path);
  if (plan.format !== 1 || plan.kind !== "gba-4bpp-object-bank") {
    throw new Error("unsupported tile-object plan");
  }
  const tile_count = number(plan.tile_count);
  const fallback_path = child(plan_path, plan.fallback);
  const [fallback] = gba_graphics(readFileSync(fallback_path), 4);
  if (fallback.length !== tile_count * PACKED_TILE) {
    throw new Error("object-bank fallback has the wrong tile count");
  }
  const output = Buffer.from(fallback);
  const claimed = new Map<number, Buffer>();
  const sources = [plan_path, fallback_path];
  let placements = 0;
  let horizontal_flips = 0;
  let vertical_flips = 0;

  for (const item of plan.objects ?? []) {
    const source_path = child(plan_path, item.source);
    const tilemap_path = child(plan_path, item.tilemap);
    const palette_path = child(plan_path, item.palette);
    const [width, height, pixels, colors] = indexed_png(readFileSync(source_path));
    const [, , , expected_colors] = indexed_png(readFileSync(palette_path));
    if (!equal_colors(colors, expected_colors)) {
      throw new Error(`${source_path}: palette differs from ${palette_path}`);
    }
    const entries_raw = import_tilemap(readFileSync(tilemap_path, "utf8"));
    const entries: number[] = [];
    for (let offset = 0; offset < entries_raw.length; offset += 2) {
      entries.push(entries_raw.readUInt16LE(offset));
    }
    if (
      width % TILE !== 0 ||
      height % TILE !== 0 ||
      entries.length !== Math.floor(width / TILE) * Math.floor(height / TILE)
    ) {
      throw new Error(`${source_path}: dimensions differ from its tilemap`);
    }
    const base_tile = number(item.base_tile);
    for (const [cell_index, entry] of entries.entries()) {
      const [tile, palette, hflip, vflip] = decode_entry(entry);
      const slot = tile - base_tile;
      if (slot < 0 || slot >= tile_count) {
        if (item.mixed_windows ?? false) {
          continue;
        }
        throw new Error(`${tilemap_path}: tile lies outside this bank`);
      }
      const displayed = image_tile(pixels, width, cell_index);
      if (displayed.some((value) => value >> 4 !== palette)) {
        throw new Error(`${source_path}: cell ${cell_index} differs from its palette bank`);
      }
      const canonical = flip_tile(displayed.map((value) => value & 15), hflip, vflip);
      const packed = pack_tile(canonical);
      const previous = claimed.get(slot);
      if (previous !== undefined && !equal_bytes(previous, packed)) {
        throw new Error(`${source_path}: repeated tile slot ${slot} is inconsistent`);
      }
      claimed.set(slot, packed);
      placements += 1;
      horizontal_flips += Number(hflip);
      vertical_flips += Number(vflip);
    }
    sources.push(source_path, tilemap_path, palette_path);
  }

  for (const [slot, packed] of claimed) {
    const start = slot * PACKED_TILE;
    if (
      (plan.require_blank_fallback ?? true) &&
      output.subarray(start, start + PACKED_TILE).some((value) => value !== 0)
    ) {
      throw new Error(`fallback still contains authoritative tile ${slot}`);
    }
    packed.copy(output, start);
  }
  return [output, [...new Set(sources)], {
    tiles: tile_count,
    object_tiles: claimed.size,
    placements,
    horizontal_flips,
    vertical_flips,
    objects: (plan.objects ?? []).length,
  }];
}

/** 再構築済み地図から画面上の8画素単位タイル表を返す。 */
export function map_entries(map_dir: string): number[][] {
  const metatiles = parse_metatiles(join(map_dir, "metatiles.tilemap"));
  const cells = cell_indices(map_dir, metatiles.length);
  const [width, height] = scene_dimensions(map_dir);
  const rows: number[][] = [];
  for (let tile_y = 0; tile_y < Math.ceil(height / TILE); tile_y += 1) {
    const row: number[] = [];
    const cell_y = Math.floor(tile_y / 2);
    const sub_y = tile_y % 2;
    for (let tile_x = 0; tile_x < Math.ceil(width / TILE); tile_x += 1) {
      const cell_x = Math.floor(tile_x / 2);
      const sub_x = tile_x % 2;
      const metatile = cells[cell_y][cell_x];
      if (metatile >= metatiles.length) {
        throw new Error("map grid references a missing metatile");
      }
      row.push(metatiles[metatile][sub_y * 2 + sub_x]);
    }
    rows.push(row);
  }
  return rows;
}

/** 既存素材から一個の物体画像と中立な残余画像を作る。 */
export function extract_map_object(
  container: string,
  charblock: number,
  rectangle: Rectangle,
  name: string,
  append = false,
  mixed_windows = false,
  name_ja?: string,
  name_en?: string,
): void {
  if (charblock !== 1 && charblock !== 2) {
    throw new Error("map object extraction currently supports charblocks 1/2");
  }
  const [left, top, width_tiles, height_tiles] = rectangle;
  if (Math.min(left, top) < 0 || Math.min(width_tiles, height_tiles) <= 0) {
    throw new Error("object rectangle must be positive");
  }
  const directory = join(ROOT, resourceGraphicsDir(container));
  const map_dir = join(ROOT, `assets/maps/resource_${container}`);
  const bank_path = join(directory, `charblock${charblock}.4bpp.png`);
  const plan_path = join(directory, `charblock${charblock}.objects.json`);
  let plan: JsonObject;
  let bank: Buffer;
  let fallback_name: string;
  let fallback_path: string;
  let remainder: Buffer;
  if (existsSync(plan_path)) {
    if (!append) {
      throw new Error(`${plan_path} already exists; use --append`);
    }
    plan = read_json(plan_path);
    if ((plan.objects ?? []).some((item: JsonObject) => item.name === name)) {
      throw new Error(`object ${JSON.stringify(name)} already exists`);
    }
    [bank] = build_bank(plan_path);
    fallback_name = plan.fallback;
    fallback_path = child(plan_path, fallback_name);
    [remainder] = gba_graphics(readFileSync(fallback_path), 4);
  } else {
    let details: JsonObject;
    [bank, , details] = gba_graphics(readFileSync(bank_path), 4);
    if (bank.length !== 0x4000 || details.tiles !== 512) {
      throw new Error("object extraction requires one complete charblock");
    }
    fallback_name = `charblock${charblock}.remaining.4bpp.png`;
    fallback_path = join(directory, fallback_name);
    remainder = bank;
    plan = {};
  }

  const window_order = tile_window_order(map_dir);
  if (!window_order.includes(charblock)) {
    throw new Error("selected charblock is not in the map's display windows");
  }
  const base_tile = window_order.indexOf(charblock) * 0x200;
  const display_banks = new Map<number, Buffer>();
  for (const source_charblock of window_order) {
    let source_bank: Buffer;
    if (source_charblock === charblock) {
      source_bank = bank;
    } else {
      const source_plan = join(directory, `charblock${source_charblock}.objects.json`);
      const source_png = join(directory, `charblock${source_charblock}.4bpp.png`);
      if (existsSync(source_plan)) {
        [source_bank] = build_bank(source_plan);
      } else if (existsSync(source_png)) {
        [source_bank] = gba_graphics(readFileSync(source_png), 4);
      } else {
        throw new Error("map object needs an unavailable display charblock");
      }
    }
    if (source_bank.length !== 0x4000) {
      throw new Error("map display charblock is not exactly 512 tiles");
    }
    display_banks.set(source_charblock, source_bank);
  }

  const palette_path = join(directory, "palette.224.png");
  const [, , , colors] = indexed_png(readFileSync(palette_path));
  const grid = map_entries(map_dir);
  if (top + height_tiles > grid.length || left + width_tiles > grid[0].length) {
    throw new Error("object rectangle exceeds the declared map");
  }
  const entries: number[] = [];
  for (let y = top; y < top + height_tiles; y += 1) {
    for (let x = left; x < left + width_tiles; x += 1) {
      entries.push(grid[y][x]);
    }
  }
  const display_tiles: number[] = [];
  const claimed = new Set<number>();
  for (const entry of entries) {
    const [tile, palette, hflip, vflip] = decode_entry(entry);
    const window = Math.floor(tile / 0x200);
    const slot = tile % 0x200;
    const source_charblock = window_order[window];
    if (source_charblock !== charblock && !mixed_windows) {
      throw new Error("object rectangle crosses into another display window");
    }
    const source_bank = display_banks.get(source_charblock);
    if (source_bank === undefined) {
      throw new Error("object rectangle references an unavailable display window");
    }
    const canonical = unpack_tile(
      source_bank.subarray(slot * PACKED_TILE, (slot + 1) * PACKED_TILE),
    );
    const displayed = flip_tile(canonical, hflip, vflip);
    if (palette * 16 + Math.max(...displayed) >= colors.length) {
      throw new Error("object rectangle needs a palette bank not stored locally");
    }
    display_tiles.push(...displayed.map((value) => palette * 16 + value));
    if (source_charblock === charblock) {
      claimed.add(slot);
    }
  }

  if (claimed.size === 0) {
    throw new Error("object rectangle does not reference the selected charblock");
  }

  const objects_dir = join(directory, "objects");
  mkdirSync(objects_dir, { recursive: true });
  const object_path = join(objects_dir, `${name}.png`);
  const tilemap_path = join(objects_dir, `${name}.tilemap`);
  const [object_png] = tile_png(Buffer.from(display_tiles), 8, width_tiles, colors);
  if (existsSync(object_path) && !equal_bytes(readFileSync(object_path), object_png)) {
    throw new Error(`${object_path}: another bank produced a different object`);
  }
  writeFileSync(object_path, object_png);
  const tilemap_raw = Buffer.alloc(entries.length * 2);
  entries.forEach((entry, index) => tilemap_raw.writeUInt16LE(entry, index * 2));
  const tilemap_text = export_tilemap(tilemap_raw, width_tiles);
  if (existsSync(tilemap_path) && readFileSync(tilemap_path, "utf8") !== tilemap_text) {
    throw new Error(`${tilemap_path}: another bank produced a different tilemap`);
  }
  writeFileSync(tilemap_path, tilemap_text);

  remainder = Buffer.from(remainder);
  for (const slot of claimed) {
    remainder.fill(0, slot * PACKED_TILE, (slot + 1) * PACKED_TILE);
  }
  const [fallback_png] = tile_png(remainder, 4, SOURCE_COLUMNS);
  writeFileSync(fallback_path, fallback_png);
  const item: JsonObject = {
    name,
    source: `objects/${name}.png`,
    tilemap: `objects/${name}.tilemap`,
    palette: "palette.224.png",
    base_tile: `0x${base_tile.toString(16)}`,
  };
  if (mixed_windows) {
    item.mixed_windows = true;
  }
  if (name_ja) {
    item.name_ja = name_ja;
  }
  if (name_en) {
    item.name_en = name_en;
  }
  if (existsSync(plan_path)) {
    plan.objects.push(item);
  } else {
    plan = {
      format: 1,
      kind: "gba-4bpp-object-bank",
      tile_count: 512,
      fallback: fallback_name,
      require_blank_fallback: true,
      objects: [item],
    };
  }
  writeFileSync(plan_path, json_text(plan));
  const [rebuilt, , report] = build_bank(plan_path);
  if (!equal_bytes(rebuilt, bank)) {
    throw new Error("extracted object bank does not reproduce its source");
  }
  select_manifest_source(container, charblock, plan_path);
  const superseded = existsSync(bank_path);
  if (superseded) {
    unlinkSync(bank_path);
  }
  console.log(
    `object=${object_path} size=${width_tiles * TILE}x${height_tiles * TILE} ` +
    `tiles=${report.object_tiles} placements=${report.placements}`,
  );
  console.log(`plan=${plan_path} fallback=${fallback_path}`);
  if (superseded) {
    console.log(`superseded=${bank_path}`);
  }
}

export function parse_rectangle(value: string): Rectangle {
  let fields: number[];
  try {
    fields = value.split(",").map((field) => parse_base_zero(field));
  } catch {
    throw new Error("rectangle must be x,y,width,height");
  }
  if (fields.length !== 4) {
    throw new Error("rectangle must be x,y,width,height");
  }
  return fields as Rectangle;
}

function usage(): never {
  throw new Error(
    "usage: tile_objects.ts build PLAN [-o OUTPUT] | " +
    "extract-map CONTAINER --charblock N --rect X,Y,W,H --name NAME",
  );
}

function option_value(arguments_: string[], index: number, option: string): [string, number] {
  if (index + 1 >= arguments_.length) {
    throw new Error(`${option} requires a value`);
  }
  return [arguments_[index + 1], index + 1];
}

export function main(arguments_ = Bun.argv.slice(2)): void {
  if (arguments_.includes("-h") || arguments_.includes("--help")) {
    console.log(
      "usage: tile_objects.ts build PLAN [-o OUTPUT] | " +
      "extract-map CONTAINER --charblock N --rect X,Y,W,H --name NAME",
    );
    return;
  }
  const command = arguments_[0];
  if (command === "build") {
    let plan: string | undefined;
    let output: string | undefined;
    for (let index = 1; index < arguments_.length; index += 1) {
      const argument = arguments_[index];
      if (argument === "-o" || argument === "--output") {
        [output, index] = option_value(arguments_, index, argument);
      } else if (argument.startsWith("-")) {
        usage();
      } else if (plan === undefined) {
        plan = argument;
      } else {
        usage();
      }
    }
    if (plan === undefined) {
      usage();
    }
    const [data, , report] = build_bank(plan);
    if (output !== undefined) {
      mkdirSync(dirname(resolve(output)), { recursive: true });
      writeFileSync(output, data);
    }
    console.log(Object.entries(report).map(([key, value]) => `${key}=${value}`).join(" "));
    return;
  }
  if (command !== "extract-map") {
    usage();
  }
  const positional: string[] = [];
  let charblock: number | undefined;
  let rectangle: Rectangle | undefined;
  let name: string | undefined;
  let append = false;
  let mixed_windows = false;
  let name_ja: string | undefined;
  let name_en: string | undefined;
  for (let index = 1; index < arguments_.length; index += 1) {
    const argument = arguments_[index];
    let value: string;
    if (argument === "--append") {
      append = true;
    } else if (argument === "--mixed-windows") {
      mixed_windows = true;
    } else if (["--charblock", "--rect", "--name", "--name-ja", "--name-en"].includes(argument)) {
      [value, index] = option_value(arguments_, index, argument);
      if (argument === "--charblock") {
        charblock = parse_decimal(value);
      } else if (argument === "--rect") {
        rectangle = parse_rectangle(value);
      } else if (argument === "--name") {
        name = value;
      } else if (argument === "--name-ja") {
        name_ja = value;
      } else {
        name_en = value;
      }
    } else if (argument.startsWith("-")) {
      usage();
    } else {
      positional.push(argument);
    }
  }
  if (
    positional.length !== 1 ||
    charblock === undefined ||
    rectangle === undefined ||
    name === undefined
  ) {
    usage();
  }
  extract_map_object(
    positional[0].toLowerCase(),
    charblock,
    rectangle,
    name,
    append,
    mixed_windows,
    name_ja,
    name_en,
  );
}

if (import.meta.main) {
  main();
}
