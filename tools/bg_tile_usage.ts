#!/usr/bin/env bun
// Tool role: entrypoint; invoked by CONVENTIONS.md, package.json.
/**
 * 地図読込表、グリッド、メタタイルから背景タイルの使用箇所を索引化する。
 *
 * 出力は解析用であり構築入力には使わない。生成物は必ず無視対象の
 * 出力ディレクトリへ置き、素材ディレクトリは読み取り専用として扱う。
 */
import {
  mkdirSync,
  readFileSync,
  writeFileSync,
} from "node:fs";
import {
  dirname,
  isAbsolute,
  join,
  relative,
  resolve,
} from "node:path";

import {
  cell_indices,
  has_map_prefix,
  parse_metatiles,
  scene_dimensions,
  tile_window_order,
  type Linkage,
} from "./compose_scene.ts";
import { decode_entry, encode_entry } from "./text_bg.ts";

export const ROOT = resolve(dirname(Bun.fileURLToPath(import.meta.url)), "..");
export const OUT_ROOT = join(ROOT, "out");
export const LOAD_TABLE = join(ROOT, "assets/maps/map_load_table.json");
export const TILE_SIZE = 8;
export const CHARBLOCK_TILES = 0x200;

export interface VariantGeometry {
  linkage: Linkage;
  width: number;
  height: number;
  window_order: [number, number];
  metatiles: number[][];
  cells: number[][];
}

export interface TilePlacement {
  resource: string;
  slot: number;
  map_index: number;
  container: string;
  tile_x: number;
  tile_y: number;
  metatile: number;
  subtile: number;
  header_window: number;
  charblock: number;
  palette_bank: number;
  hflip: boolean;
  vflip: boolean;
}

export type TilePosition = [number, number];

export interface PlacementGroup {
  map_index: number;
  container: string;
  header_window: number;
  charblock: number;
  palette_bank: number;
  hflip: boolean;
  vflip: boolean;
  positions: TilePosition[];
}

export interface SlotUsage {
  slot: number;
  placement_count: number;
  uses: PlacementGroup[];
}

export interface ResourceUsage {
  resource: string;
  charblock_roles: number[];
  placement_count: number;
  slots: SlotUsage[];
}

export interface VariantSummary {
  map_index: number;
  container: string;
  palette: string;
  width_tiles: number;
  height_tiles: number;
  placement_count: number;
  windows: Array<{
    header_window: number;
    charblock: number;
    resource: string;
  }>;
}

export interface UnavailableVariant {
  map_index: number;
  container: string;
}

export interface BgTileUsageIndex {
  format: 1;
  kind: "golden-sun-bg-tile-usage-index";
  tile_width: number;
  tile_height: number;
  position_unit: "tile";
  position_fields: ["tile_x", "tile_y"];
  charblock_tiles: number;
  variant_count: number;
  unavailable_variants: UnavailableVariant[];
  placement_count: number;
  resource_count: number;
  slot_count: number;
  variants: VariantSummary[];
  resources: ResourceUsage[];
}

interface LoadDocument {
  format: number;
  records: Linkage[];
}

function normalize_id(value: string): string {
  const text = value.trim().toLowerCase().replace(/^0x/, "");
  if (!/^[0-9a-f]+$/.test(text)) {
    throw new Error(`invalid hexadecimal resource id: ${value}`);
  }
  return text.replace(/^0+(?=[0-9a-f])/, "");
}

function compare_id(left: string, right: string): number {
  return Number.parseInt(left, 16) - Number.parseInt(right, 16);
}

function resource_for(linkage: Linkage, charblock: number): string {
  if (charblock === 1) return normalize_id(linkage.vram_charblock1);
  if (charblock === 2) return normalize_id(linkage.vram_charblock2);
  if (charblock === 3) return normalize_id(linkage.vram_charblock3);
  throw new Error(`unsupported charblock role: ${charblock}`);
}

/** 一件の読込表変種を画面位置ごとの使用記録へ展開する。 */
export function index_variant(geometry: VariantGeometry): TilePlacement[] {
  const { linkage, width, height, window_order, metatiles, cells } = geometry;
  if (
    !Number.isInteger(width) ||
    !Number.isInteger(height) ||
    width <= 0 ||
    height <= 0
  ) {
    throw new Error("map dimensions must be positive integers");
  }
  if (
    window_order.length !== 2 ||
    new Set(window_order).size !== 2 ||
    window_order.some((value) => value !== 1 && value !== 2)
  ) {
    throw new Error("header windows must select charblocks one and two exactly once");
  }
  const width_tiles = Math.ceil(width / TILE_SIZE);
  const height_tiles = Math.ceil(height / TILE_SIZE);
  const placements: TilePlacement[] = [];
  for (let tile_y = 0; tile_y < height_tiles; tile_y += 1) {
    const cell_y = Math.floor(tile_y / 2);
    if (cells[cell_y] === undefined) {
      throw new Error("map grid is shorter than the declared height");
    }
    for (let tile_x = 0; tile_x < width_tiles; tile_x += 1) {
      const cell_x = Math.floor(tile_x / 2);
      const metatile = cells[cell_y][cell_x];
      const quad = metatiles[metatile];
      if (quad === undefined || quad.length !== 4) {
        throw new Error("map grid references an invalid 2x2 metatile");
      }
      const subtile = tile_y % 2 * 2 + tile_x % 2;
      const [tile, palette_bank, hflip, vflip] = decode_entry(quad[subtile]);
      const header_window = Math.floor(tile / CHARBLOCK_TILES);
      const charblock = window_order[header_window];
      if (charblock === undefined) {
        throw new Error("tile entry references a header window outside zero and one");
      }
      placements.push({
        resource: resource_for(linkage, charblock),
        slot: tile % CHARBLOCK_TILES,
        map_index: Number(linkage.map_index),
        container: normalize_id(linkage.container),
        tile_x,
        tile_y,
        metatile,
        subtile,
        header_window,
        charblock,
        palette_bank,
        hflip,
        vflip,
      });
    }
  }
  return placements;
}

function placement_key(placement: TilePlacement): string {
  return [
    placement.map_index,
    placement.container,
    placement.header_window,
    placement.charblock,
    placement.palette_bank,
    Number(placement.hflip),
    Number(placement.vflip),
  ].join(":");
}

function compare_group(left: PlacementGroup, right: PlacementGroup): number {
  return left.map_index - right.map_index ||
    compare_id(left.container, right.container) ||
    left.header_window - right.header_window ||
    left.charblock - right.charblock ||
    left.palette_bank - right.palette_bank ||
    Number(left.hflip) - Number(right.hflip) ||
    Number(left.vflip) - Number(right.vflip);
}

function group_placements(placements: TilePlacement[]): PlacementGroup[] {
  const groups = new Map<string, PlacementGroup>();
  for (const placement of placements) {
    const key = placement_key(placement);
    const group = groups.get(key) ?? {
      map_index: placement.map_index,
      container: placement.container,
      header_window: placement.header_window,
      charblock: placement.charblock,
      palette_bank: placement.palette_bank,
      hflip: placement.hflip,
      vflip: placement.vflip,
      positions: [],
    };
    group.positions.push([placement.tile_x, placement.tile_y]);
    groups.set(key, group);
  }
  return [...groups.values()].map((group) => ({
    ...group,
    positions: group.positions.sort((left, right) => left[1] - right[1] || left[0] - right[0]),
  })).sort(compare_group);
}

/** 複数の変種を資源番号と文字位置で引ける決定的な索引へまとめる。 */
export function build_usage_index(
  geometries: VariantGeometry[],
  unavailable_variants: UnavailableVariant[] = [],
): BgTileUsageIndex {
  const resources = new Map<string, Map<number, TilePlacement[]>>();
  const variants: VariantSummary[] = [];
  let placement_count = 0;
  for (const geometry of geometries) {
    const placements = index_variant(geometry);
    placement_count += placements.length;
    const linkage = geometry.linkage;
    variants.push({
      map_index: Number(linkage.map_index),
      container: normalize_id(linkage.container),
      palette: normalize_id(linkage.palette),
      width_tiles: Math.ceil(geometry.width / TILE_SIZE),
      height_tiles: Math.ceil(geometry.height / TILE_SIZE),
      placement_count: placements.length,
      windows: geometry.window_order.map((charblock, header_window) => ({
        header_window,
        charblock,
        resource: resource_for(linkage, charblock),
      })),
    });
    for (const placement of placements) {
      const slots = resources.get(placement.resource) ?? new Map<number, TilePlacement[]>();
      const uses = slots.get(placement.slot) ?? [];
      uses.push(placement);
      slots.set(placement.slot, uses);
      resources.set(placement.resource, slots);
    }
  }
  variants.sort((left, right) => left.map_index - right.map_index || compare_id(left.container, right.container));
  const resource_entries: ResourceUsage[] = [...resources.entries()]
    .sort(([left], [right]) => compare_id(left, right))
    .map(([resource, slots]) => {
      const slot_entries = [...slots.entries()]
        .sort(([left], [right]) => left - right)
        .map(([slot, placements]) => ({
          slot,
          placement_count: placements.length,
          uses: group_placements(placements),
        }));
      const charblock_roles = [...new Set(slot_entries.flatMap((entry) =>
        entry.uses.map((use) => use.charblock)))].sort((left, right) => left - right);
      return {
        resource,
        charblock_roles,
        placement_count: slot_entries.reduce((sum, entry) => sum + entry.placement_count, 0),
        slots: slot_entries,
      };
    });
  const missing = unavailable_variants
    .map((entry) => ({ map_index: Number(entry.map_index), container: normalize_id(entry.container) }))
    .sort((left, right) => left.map_index - right.map_index || compare_id(left.container, right.container));
  return {
    format: 1,
    kind: "golden-sun-bg-tile-usage-index",
    tile_width: TILE_SIZE,
    tile_height: TILE_SIZE,
    position_unit: "tile",
    position_fields: ["tile_x", "tile_y"],
    charblock_tiles: CHARBLOCK_TILES,
    variant_count: variants.length,
    unavailable_variants: missing,
    placement_count,
    resource_count: resource_entries.length,
    slot_count: resource_entries.reduce((sum, resource) => sum + resource.slots.length, 0),
    variants,
    resources: resource_entries,
  };
}

/** 正規素材から一件の変種に必要な幾何情報を読む。 */
export function load_geometry(linkage: Linkage): VariantGeometry {
  const container = normalize_id(linkage.container);
  const map_dir = join(ROOT, `assets/maps/resource_${container}`);
  const metatiles = parse_metatiles(`${map_dir}_metatiles.tilemap`);
  const cells = cell_indices(map_dir, metatiles.length);
  const [width, height] = scene_dimensions(map_dir);
  return {
    linkage,
    width,
    height,
    window_order: tile_window_order(map_dir),
    metatiles,
    cells,
  };
}

export function load_table(path = LOAD_TABLE): LoadDocument {
  const document = JSON.parse(readFileSync(path, "utf8")) as LoadDocument;
  if (document.format !== 1 || !Array.isArray(document.records)) {
    throw new Error("unsupported map load table");
  }
  return document;
}

/** 読込表の利用可能な変種を全て索引化し、不足地図を明記する。 */
export function index_linkages(linkages: Linkage[]): BgTileUsageIndex {
  const geometries: VariantGeometry[] = [];
  const unavailable: UnavailableVariant[] = [];
  for (const linkage of linkages) {
    const container = normalize_id(linkage.container);
    const map_dir = join(ROOT, `assets/maps/resource_${container}`);
    if (!has_map_prefix(map_dir)) {
      unavailable.push({ map_index: Number(linkage.map_index), container });
      continue;
    }
    geometries.push(load_geometry(linkage));
  }
  if (geometries.length === 0) {
    throw new Error("none of the selected variants has reconstructed map geometry");
  }
  return build_usage_index(geometries, unavailable);
}

/** 生成報告が無視対象の出力ディレクトリから出ないよう検査する。 */
export function report_path(value: string): string {
  const target = isAbsolute(value) ? resolve(value) : resolve(ROOT, value);
  const local = relative(OUT_ROOT, target);
  if (!local || local === ".." || local.startsWith("../") || local.startsWith("..\\") || isAbsolute(local)) {
    throw new Error("usage reports must stay below out/");
  }
  if (!target.endsWith(".json")) {
    throw new Error("usage report must use a .json suffix");
  }
  return target;
}

export function self_test(): void {
  const linkage: Linkage = {
    map_index: 7,
    container: "abc",
    palette: "abd",
    vram_charblock1: "120",
    vram_charblock2: "121",
    vram_charblock3: "122",
    animation_source: "123",
  };
  const metatiles = [[
    encode_entry(1, 2),
    encode_entry(0x203, 4, true),
    encode_entry(5, 6, false, true),
    encode_entry(0x207, 8, true, true),
  ]];
  const index = build_usage_index([{
    linkage,
    width: 16,
    height: 16,
    window_order: [2, 1],
    metatiles,
    cells: [[0]],
  }], [{ map_index: 8, container: "abe" }]);
  const expected = [
    ["121", 1, 0, 0, 0, 2, false, false],
    ["120", 3, 1, 0, 1, 4, true, false],
    ["121", 5, 0, 1, 0, 6, false, true],
    ["120", 7, 1, 1, 1, 8, true, true],
  ];
  const actual = index.resources.flatMap((resource) => resource.slots.flatMap((slot) =>
    slot.uses.flatMap((use) => use.positions.map(([tile_x, tile_y]) => [
      resource.resource,
      slot.slot,
      tile_x,
      tile_y,
      use.header_window,
      use.palette_bank,
      use.hflip,
      use.vflip,
    ])))).sort((left, right) => Number(left[2]) - Number(right[2]) || Number(left[3]) - Number(right[3]));
  expected.sort((left, right) => Number(left[2]) - Number(right[2]) || Number(left[3]) - Number(right[3]));
  if (
    JSON.stringify(actual) !== JSON.stringify(expected) ||
    index.variant_count !== 1 ||
    index.placement_count !== 4 ||
    index.resource_count !== 2 ||
    index.slot_count !== 4 ||
    JSON.stringify(index.unavailable_variants) !== JSON.stringify([{ map_index: 8, container: "abe" }])
  ) {
    throw new Error("BG tile usage index self-test failed");
  }
  const accepted = report_path("out/bg_tile_usage/test.json");
  if (accepted !== join(OUT_ROOT, "bg_tile_usage/test.json")) {
    throw new Error("BG tile usage report path self-test failed");
  }
  let escaped = false;
  try {
    report_path("assets/usage.json");
  } catch {
    escaped = true;
  }
  if (!escaped) {
    throw new Error("BG tile usage report accepted a tracked-source path");
  }
  resource_184_fixture();
  console.log("self-test=ok");
}

/** 実在地図を用いて変種集約と繰返し配置の不変条件を検査する。 */
export function resource_184_fixture(): void {
  const table = load_table();
  const linkages = table.records.filter((linkage) => normalize_id(linkage.container) === "184");
  const index = index_linkages(linkages);
  const resource = index.resources[0];
  if (
    linkages.length !== 1 ||
    index.variant_count !== 1 ||
    index.placement_count !== 4096 ||
    index.resource_count !== 1 ||
    index.slot_count !== 238 ||
    resource?.resource !== "186" ||
    resource.placement_count !== 4096
  ) {
    throw new Error("resource 184 BG tile usage fixture differs");
  }
  const placements = index_variant(load_geometry(linkages[0]));
  const by_position = new Map(placements.map((placement) => [
    `${placement.tile_x},${placement.tile_y}`,
    placement,
  ]));
  const origins: TilePosition[] = [[16, 0], [44, 14], [0, 50], [26, 59]];
  for (const [left, top] of origins) {
    for (let y = 0; y < 4; y += 1) {
      for (let x = 0; x < 4; x += 1) {
        const placement = by_position.get(`${left + x},${top + y}`);
        if (
          placement?.resource !== "186" ||
          placement.slot !== 0x0c + y * 0x20 + x ||
          placement.palette_bank !== 0 ||
          placement.hflip ||
          placement.vflip
        ) {
          throw new Error("resource 184 repeated tile rectangle differs");
        }
      }
    }
  }
}

function option_value(arguments_: string[], index: number, option: string): [string, number] {
  if (index + 1 >= arguments_.length) {
    throw new Error(`${option} requires a value`);
  }
  return [arguments_[index + 1], index + 1];
}

function usage(): never {
  throw new Error(
    "usage: bg_tile_usage.ts [--self-test] [--all | CONTAINER] " +
    "[--map-index N] [-o out/REPORT.json]",
  );
}

function main(arguments_ = Bun.argv.slice(2)): void {
  if (arguments_.includes("-h") || arguments_.includes("--help")) {
    console.log(
      "usage: bg_tile_usage.ts [--self-test] [--all | CONTAINER] " +
      "[--map-index N] [-o out/REPORT.json]",
    );
    return;
  }
  let container: string | undefined;
  let map_index: number | undefined;
  let output: string | undefined;
  let all = false;
  let tested = false;
  for (let index = 0; index < arguments_.length; index += 1) {
    const argument = arguments_[index];
    let value: string;
    if (argument === "--self-test") {
      self_test();
      tested = true;
    } else if (argument === "--all") {
      all = true;
    } else if (argument === "--map-index") {
      [value, index] = option_value(arguments_, index, argument);
      if (!/^[0-9]+$/.test(value)) throw new Error("--map-index requires a decimal integer");
      map_index = Number(value);
    } else if (argument === "-o" || argument === "--output") {
      [output, index] = option_value(arguments_, index, argument);
    } else if (argument.startsWith("-")) {
      usage();
    } else if (container === undefined) {
      container = normalize_id(argument);
    } else {
      usage();
    }
  }
  if (tested && container === undefined && !all && map_index === undefined && output === undefined) return;
  if (all === (container !== undefined)) usage();
  const table = load_table();
  let linkages = table.records;
  if (container !== undefined) {
    linkages = linkages.filter((linkage) => normalize_id(linkage.container) === container);
  }
  if (map_index !== undefined) {
    linkages = linkages.filter((linkage) => Number(linkage.map_index) === map_index);
  }
  if (linkages.length === 0) {
    throw new Error("no map load variant matches the requested filter");
  }
  const report = index_linkages(linkages);
  const suffix = map_index === undefined ? "" : `_map_${map_index}`;
  const default_output = all
    ? `out/bg_tile_usage/all${suffix}.json`
    : `out/bg_tile_usage/resource_${container}${suffix}.json`;
  const target = report_path(output ?? default_output);
  mkdirSync(dirname(target), { recursive: true });
  writeFileSync(target, `${JSON.stringify(report)}\n`);
  console.log(
    `report=${target} variants=${report.variant_count} ` +
    `resources=${report.resource_count} slots=${report.slot_count} ` +
    `placements=${report.placement_count} unavailable=${report.unavailable_variants.length}`,
  );
}

if (import.meta.main) main();
