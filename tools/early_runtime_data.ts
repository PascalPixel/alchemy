#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { canonicalJson, isCanonicalJsonText } from "./canonical_json.ts";
import {
  existsSync, lstatSync, mkdirSync, mkdtempSync, readFileSync, readdirSync, realpathSync,
  renameSync, rmSync, symlinkSync, unlinkSync, writeFileSync,
} from "fs";
import { basename, dirname, isAbsolute, join, relative, resolve } from "path";
import { tmpdir } from "os";
import { indexed_png } from "./import_asset.ts";
import { png, type Rgb } from "./skip_sprite_archive.ts";

type Encoding = "u8" | "s16le" | "s32le" | "u32le" | "pointer32le" |
  "command32le" | "gba_4bpp_tiles" | "overlapping_views" | "unresolved";
type Json = Record<string, any>;

interface Entry {
  id: string;
  address: string;
  end: string;
  encoding: Encoding;
  shape?: number[];
  segments?: number[];
  source_state?: string;
  consumers: string[];
}

interface Region {
  id: string;
  address: string;
  end: string;
  claim: string;
  partitions: Entry[];
  views: Entry[];
}

interface Catalog {
  format: number;
  address_space: string;
  canonical_target: string;
  regions: Region[];
}

export interface EarlyRuntimeBuild {
  regions: Map<number, Buffer>;
  source_bytes: number;
}

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const DEFAULT_CATALOG = join(ROOT, "assets/data/early_runtime_data.json");
const ROM_BASE = 0x08000000;
export const EARLY_RUNTIME_ADDRESS = 0x08012f20;
export const EARLY_RUNTIME_END = 0x08013784;
export const POST_MAP_ADDRESS = 0x0801403c;
export const POST_MAP_END = 0x08015000;
export const EARLY_RUNTIME_SOURCE_BYTES =
  EARLY_RUNTIME_END - EARLY_RUNTIME_ADDRESS + POST_MAP_END - POST_MAP_ADDRESS;
const DISPLAY_SOURCE = "display.4bpp.png";
const DISPLAY_WIDTH = 32;
const DISPLAY_HEIGHT = 8;
const PALETTE: Rgb[] = Array.from({ length: 16 }, (_, index) => {
  const value = index * 8;
  return [value, value, value];
});
const TYPED_WIDTH: Partial<Record<Encoding, number>> = {
  u8: 1,
  s16le: 2,
  s32le: 4,
  u32le: 4,
  pointer32le: 4,
  command32le: 4,
};

function pretty(value: unknown): string {
  return `${canonicalJson(value)}\n`;
}

function hexadecimal(value: number): string {
  return `0x${(value >>> 0).toString(16).padStart(8, "0")}`;
}

function address(value: unknown, label = "address"): number {
  if (typeof value !== "string" || !/^0x080[0-9a-f]{5}$/.test(value)) {
    throw new Error(`${label} is not a canonical ROM address`);
  }
  return Number(value);
}

function object(value: unknown, label: string): Json {
  if (typeof value !== "object" || value === null || Array.isArray(value)) {
    throw new Error(`${label} must be an object`);
  }
  return value as Json;
}

function exactKeys(value: Json, keys: readonly string[], label: string): void {
  const actual = Object.keys(value).sort();
  const expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${label} fields differ`);
  }
}

function integer(value: unknown, minimum: number, maximum: number, label: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < minimum || value > maximum) {
    throw new Error(`${label} is outside its range`);
  }
  return value;
}

function array(value: unknown, length: number, label: string): unknown[] {
  if (!Array.isArray(value) || value.length !== length) throw new Error(`${label} requires ${length} entries`);
  return value;
}

function elements(shape: number[] | undefined): number {
  if (shape === undefined || shape.length === 0 || shape.some((value) => !Number.isInteger(value) || value <= 0)) {
    throw new Error("typed entry requires a positive shape");
  }
  return shape.reduce((product, value) => product * value, 1);
}

function expectedSize(entry: Entry): number | null {
  const width = TYPED_WIDTH[entry.encoding];
  if (width !== undefined) return elements(entry.shape) * width;
  if (entry.encoding === "gba_4bpp_tiles") {
    if (entry.shape?.length !== 3 || entry.shape[1] !== 8 || entry.shape[2] !== 8) {
      throw new Error(`${entry.id}: 4bpp tiles require [count,8,8]`);
    }
    return entry.shape[0] * 32;
  }
  return null;
}

function validateEntry(entry: Entry, regionStart: number, regionEnd: number): void {
  if (!/^[a-z0-9_]+$/.test(entry.id) || !Array.isArray(entry.consumers) ||
      entry.consumers.some((item) => typeof item !== "string" || item.includes("..") || item.startsWith("/"))) {
    throw new Error("invalid catalog entry");
  }
  const start = address(entry.address, `${entry.id} address`);
  const end = address(entry.end, `${entry.id} end`);
  if (start < regionStart || end <= start || end > regionEnd) throw new Error(`${entry.id}: invalid range`);
  const size = expectedSize(entry);
  if (size !== null && size !== end - start) throw new Error(`${entry.id}: shape does not match range`);
  if (entry.segments !== undefined) {
    if (entry.encoding !== "command32le" || entry.segments.some((value) => !Number.isInteger(value) || value <= 0) ||
        entry.segments.reduce((sum, value) => sum + value, 0) !== elements(entry.shape)) {
      throw new Error(`${entry.id}: invalid command segments`);
    }
  }
}

function entry(region: Region, id: string, view = false): Entry {
  const found = (view ? region.views : region.partitions).find((item) => item.id === id);
  if (found === undefined) throw new Error(`${region.id}: missing ${id}`);
  return found;
}

function sameShape(actual: number[] | undefined, expected: number[]): boolean {
  return actual !== undefined && actual.length === expected.length && actual.every((value, index) => value === expected[index]);
}

function exactEntry(item: Entry, start: number, end: number, encoding: Encoding, shape?: number[]): void {
  if (address(item.address) !== start || address(item.end) !== end || item.encoding !== encoding ||
      (shape !== undefined && !sameShape(item.shape, shape))) throw new Error(`${item.id}: canonical layout differs`);
}

function canonicalLayout(catalog: Catalog): { early: Region; residual: Region } {
  const early = catalog.regions.find((region) => region.id === "early_runtime_tables");
  const residual = catalog.regions.find((region) => region.id === "post_map_load_residual");
  if (early === undefined || residual === undefined || catalog.regions.length !== 2) {
    throw new Error("canonical early-runtime regions differ");
  }
  const partitions: Array<[string, number, number, Encoding, number[]?]> = [
    ["display_tiles", 0x08012f20, 0x08012fa0, "gba_4bpp_tiles", [4, 8, 8]],
    ["shared_lookup_storage", 0x08012fa0, 0x080134fc, "overlapping_views"],
    ["interpolation_coefficients", 0x080134fc, 0x0801353c, "u32le", [16]],
    ["surface_lookup", 0x0801353c, 0x0801356c, "u8", [3, 16]],
    ["unclassified_0801356c", 0x0801356c, 0x08013584, "unresolved"],
    ["render_limits", 0x08013584, 0x0801358c, "s32le", [2]],
    ["object_programs", 0x0801358c, 0x08013624, "command32le", [38]],
    ["object_handlers", 0x08013624, 0x08013724, "pointer32le", [64]],
    ["unclassified_08013724", 0x08013724, 0x08013784, "unresolved"],
  ];
  const views: Array<[string, number, number, Encoding, number[]]> = [
    ["byte_translation_pages", 0x08012fa0, 0x080134a0, "u8", [5, 256]],
    ["grid_lookup_a", 0x080132fc, 0x080133fc, "u8", [16, 16]],
    ["grid_lookup_b", 0x080133fc, 0x080134fc, "u8", [16, 16]],
    ["resource_slot_descriptors", 0x080132cc, 0x080132fc, "u32le", [2, 6]],
    ["signed_mode_table", 0x08013254, 0x08013274, "s16le", [16]],
  ];
  if (early.partitions.length !== partitions.length || early.views.length !== views.length) {
    throw new Error("canonical early-runtime catalog size differs");
  }
  for (const [id, start, end, encoding, shape] of partitions) exactEntry(entry(early, id), start, end, encoding, shape);
  for (const [id, start, end, encoding, shape] of views) exactEntry(entry(early, id, true), start, end, encoding, shape);
  exactEntry(entry(residual, "unreferenced_storage"), POST_MAP_ADDRESS, POST_MAP_END, "unresolved");
  if (early.claim !== "pending_private_verification" || residual.claim !== "unresolved") {
    throw new Error("canonical claim state differs");
  }
  return { early, residual };
}

export function validateCatalog(catalog: Catalog): void {
  if (catalog.format !== 1 || catalog.address_space !== "gba-rom" || catalog.canonical_target !== "gs1-en" ||
      !Array.isArray(catalog.regions) || catalog.regions.length === 0) {
    throw new Error("unsupported early-runtime catalog");
  }
  const ids = new Set<string>();
  let previousRegionEnd = 0;
  for (const region of catalog.regions) {
    if (!/^[a-z0-9_]+$/.test(region.id) || ids.has(region.id)) throw new Error("invalid or duplicate region id");
    ids.add(region.id);
    const start = address(region.address, `${region.id} address`);
    const end = address(region.end, `${region.id} end`);
    if (start < previousRegionEnd || end <= start) throw new Error(`${region.id}: invalid region range`);
    previousRegionEnd = end;
    if (!Array.isArray(region.partitions) || region.partitions.length === 0 || !Array.isArray(region.views)) {
      throw new Error(`${region.id}: missing partitions or views`);
    }
    const entryIds = new Set<string>();
    let cursor = start;
    for (const item of region.partitions) {
      validateEntry(item, start, end);
      if (entryIds.has(item.id)) throw new Error(`${region.id}: duplicate entry id`);
      entryIds.add(item.id);
      if (address(item.address) !== cursor) throw new Error(`${item.id}: partition gap or overlap`);
      cursor = address(item.end);
    }
    if (cursor !== end) throw new Error(`${region.id}: partitions do not cover region`);
    for (const item of region.views) {
      validateEntry(item, start, end);
      if (entryIds.has(item.id)) throw new Error(`${region.id}: duplicate entry id`);
      entryIds.add(item.id);
    }
  }
  const { early } = canonicalLayout(catalog);
  const backing = entry(early, "shared_lookup_storage");
  for (const view of early.views) {
    if (address(view.address) < address(backing.address) || address(view.end) > address(backing.end)) {
      throw new Error(`${view.id}: view escapes canonical backing storage`);
    }
  }
  const total = catalog.regions.reduce((sum, region) => sum + address(region.end) - address(region.address), 0);
  if (total !== EARLY_RUNTIME_SOURCE_BYTES) throw new Error("canonical source-byte total differs");
}

function loadCatalog(path = DEFAULT_CATALOG): Catalog {
  const text = readFileSync(path, "utf8");
  const catalog = JSON.parse(text) as Catalog;
  if (!isCanonicalJsonText(text, catalog)) throw new Error("early-runtime catalog is not canonical JSON");
  validateCatalog(catalog);
  return catalog;
}

function valueRange(encoding: Encoding): [number, number] {
  if (encoding === "u8") return [0, 0xff];
  if (encoding === "s16le") return [-0x8000, 0x7fff];
  if (encoding === "s32le") return [-0x80000000, 0x7fffffff];
  if (encoding === "u32le" || encoding === "pointer32le" || encoding === "command32le") return [0, 0xffffffff];
  throw new Error(`cannot encode ${encoding}`);
}

export function decodeTyped(data: Uint8Array, encoding: Encoding): number[] {
  const width = TYPED_WIDTH[encoding];
  if (width === undefined) throw new Error(`cannot decode ${encoding}`);
  if (data.length % width !== 0) throw new Error("typed data has a partial element");
  const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
  const result: number[] = [];
  for (let offset = 0; offset < data.length; offset += width) {
    if (encoding === "u8") result.push(view.getUint8(offset));
    else if (encoding === "s16le") result.push(view.getInt16(offset, true));
    else if (encoding === "s32le") result.push(view.getInt32(offset, true));
    else result.push(view.getUint32(offset, true));
  }
  return result;
}

export function encodeTyped(values: number[], encoding: Encoding): Buffer {
  const width = TYPED_WIDTH[encoding];
  if (width === undefined) throw new Error(`cannot encode ${encoding}`);
  const [minimum, maximum] = valueRange(encoding);
  const data = Buffer.alloc(values.length * width);
  for (let index = 0; index < values.length; index++) {
    const value = integer(values[index], minimum, maximum, `${encoding} value ${index}`);
    const offset = index * width;
    if (encoding === "u8") data.writeUInt8(value, offset);
    else if (encoding === "s16le") data.writeInt16LE(value, offset);
    else if (encoding === "s32le") data.writeInt32LE(value, offset);
    else data.writeUInt32LE(value, offset);
  }
  return data;
}

function flatValues(value: unknown, count: number, encoding: Encoding, label: string): number[] {
  const [minimum, maximum] = valueRange(encoding);
  return array(value, count, label).map((item, index) => integer(item, minimum, maximum, `${label} ${index}`));
}

function rows(value: unknown, rowCount: number, columns: number, encoding: Encoding, label: string): number[][] {
  return array(value, rowCount, label).map((row, index) =>
    flatValues(row, columns, encoding, `${label} row ${index}`));
}

function flatten(rows_: number[][]): number[] {
  return rows_.flatMap((row) => row);
}

function writeDisplayTiles(raw: Uint8Array, path: string): void {
  if (raw.length !== 128) throw new Error("display tile extent differs");
  const pixels = Buffer.alloc(DISPLAY_WIDTH * DISPLAY_HEIGHT);
  let source = 0;
  for (let tile = 0; tile < 4; tile++) {
    for (let y = 0; y < 8; y++) {
      for (let x = 0; x < 8; x += 2) {
        const value = raw[source++];
        const target = y * DISPLAY_WIDTH + tile * 8 + x;
        pixels[target] = value & 15;
        pixels[target + 1] = value >>> 4;
      }
    }
  }
  writeFileSync(path, png(pixels, DISPLAY_WIDTH, DISPLAY_HEIGHT, PALETTE));
}

function readDisplayTiles(path: string): Buffer {
  const encoded = readFileSync(path);
  const [width, height, pixels, colors] = indexed_png(encoded);
  if (width !== DISPLAY_WIDTH || height !== DISPLAY_HEIGHT || JSON.stringify(colors) !== JSON.stringify(PALETTE) ||
      pixels.some((value) => value >= 16)) throw new Error("display tile PNG layout differs");
  if (!encoded.equals(png(Buffer.from(pixels), width, height, colors))) {
    throw new Error("display tile PNG is not canonical");
  }
  const output = Buffer.alloc(128);
  let target = 0;
  for (let tile = 0; tile < 4; tile++) {
    for (let y = 0; y < 8; y++) {
      for (let x = 0; x < 8; x += 2) {
        const source = y * DISPLAY_WIDTH + tile * 8 + x;
        output[target++] = pixels[source] | pixels[source + 1] << 4;
      }
    }
  }
  return output;
}

// 保留領域は、証明できる埋め値なら簡潔に保持し、それ以外は明示的な
// バイト列として保持する。後者も ROM の代替入力ではなく、検証可能な
// 正規ソースであり、意味が判明した時点でより強い表現に置き換える。
function residualSource(data: Uint8Array, label: string): Json {
  if (data.length === 0) throw new Error(`${label} is empty`);
  if ((data[0] === 0 || data[0] === 0xff) && data.every((value) => value === data[0])) {
    return { kind: "uniform_fill", value: `0x${data[0].toString(16).padStart(2, "0")}` };
  }
  return { kind: "byte_values", values: [...data] };
}

function buildResidual(value: unknown, size: number, label: string): Buffer {
  const source = object(value, label);
  if (source.kind === "uniform_fill") {
    exactKeys(source, ["kind", "value"], label);
    if (source.value !== "0x00" && source.value !== "0xff") throw new Error(`${label} has an unsupported uniform fill`);
    return Buffer.alloc(size, Number(source.value));
  }
  exactKeys(source, ["kind", "values"], label);
  if (source.kind !== "byte_values") throw new Error(`${label} has an unsupported residual representation`);
  return encodeTyped(flatValues(source.values, size, "u8", `${label} values`), "u8");
}

function viewSources(region: Region): Json[] {
  return region.views.map((item) => ({
    id: item.id,
    address: item.address,
    end: item.end,
    encoding: item.encoding,
    shape: item.shape,
  }));
}

function validateViewSources(value: unknown, region: Region): void {
  const expected = viewSources(region);
  const actual = array(value, expected.length, "shared lookup views");
  for (let index = 0; index < expected.length; index++) {
    const item = object(actual[index], `shared lookup view ${index}`);
    exactKeys(item, ["id", "address", "end", "encoding", "shape"], `shared lookup view ${index}`);
    if (pretty(item) !== pretty(expected[index])) throw new Error(`shared lookup view ${index} differs from catalog`);
  }
}

function word(value: unknown, label: string): number {
  if (typeof value !== "string" || !/^0x[0-9a-f]{8}$/.test(value)) throw new Error(`${label} is not a canonical word`);
  return Number(value);
}

function handlerSource(value: number): string | null {
  if (value === 0) return null;
  const target = value & ~1;
  if ((value & 1) === 0 || target < ROM_BASE || target >= ROM_BASE + 0x00800000) {
    throw new Error("object handler is not a null or Thumb ROM function pointer");
  }
  return `Func_${target.toString(16).padStart(8, "0")}`;
}

function handlerValue(value: unknown, label: string): number {
  if (value === null) return 0;
  if (typeof value !== "string" || !/^Func_080[0-7][0-9a-f]{4}$/.test(value)) {
    throw new Error(`${label} is not a canonical Thumb function symbol`);
  }
  return (Number.parseInt(value.slice(5), 16) | 1) >>> 0;
}

function programSources(data: Uint8Array, spec: Entry): Json[] {
  const values = decodeTyped(data, "command32le");
  const result: Json[] = [];
  let cursor = 0;
  for (const [index, count] of spec.segments!.entries()) {
    result.push({
      id: `program_${index.toString().padStart(2, "0")}`,
      address: hexadecimal(address(spec.address) + cursor * 4),
      words: values.slice(cursor, cursor + count).map(hexadecimal),
    });
    cursor += count;
  }
  return result;
}

function buildPrograms(value: unknown, spec: Entry): Buffer {
  const programs = array(value, spec.segments!.length, "object programs");
  const values: number[] = [];
  let cursor = 0;
  for (let index = 0; index < programs.length; index++) {
    const program = object(programs[index], `object program ${index}`);
    exactKeys(program, ["id", "address", "words"], `object program ${index}`);
    const expectedId = `program_${index.toString().padStart(2, "0")}`;
    const expectedAddress = hexadecimal(address(spec.address) + cursor * 4);
    if (program.id !== expectedId || program.address !== expectedAddress) throw new Error(`object program ${index} identity differs`);
    const words = array(program.words, spec.segments![index], `object program ${index} words`);
    words.forEach((item, wordIndex) => values.push(word(item, `object program ${index} word ${wordIndex}`)));
    cursor += words.length;
  }
  return encodeTyped(values, "command32le");
}

function range(rom: Uint8Array, start: number, end: number): Buffer {
  const from = start - ROM_BASE, to = end - ROM_BASE;
  if (from < 0 || to <= from || to > rom.length) throw new Error("ROM does not contain the early-runtime range");
  return Buffer.from(rom.subarray(from, to));
}

function sliceRegion(region: Buffer, regionStart: number, spec: Entry): Buffer {
  return region.subarray(address(spec.address) - regionStart, address(spec.end) - regionStart);
}

function sourceDocument(rom: Uint8Array, catalog: Catalog): Json {
  const { early, residual } = canonicalLayout(catalog);
  const earlyData = range(rom, EARLY_RUNTIME_ADDRESS, EARLY_RUNTIME_END);
  const residualData = range(rom, POST_MAP_ADDRESS, POST_MAP_END);
  const part = (id: string): Buffer => sliceRegion(earlyData, EARLY_RUNTIME_ADDRESS, entry(early, id));
  const lookup = part("shared_lookup_storage");
  const coefficients = decodeTyped(part("interpolation_coefficients"), "u32le");
  const surface = decodeTyped(part("surface_lookup"), "u8");
  const limits = decodeTyped(part("render_limits"), "s32le");
  const handlers = decodeTyped(part("object_handlers"), "pointer32le").map(handlerSource);
  return {
    format: 1,
    kind: "golden-sun-early-runtime-data",
    catalog_format: catalog.format,
    source_bytes: EARLY_RUNTIME_SOURCE_BYTES,
    early_runtime_tables: {
      address: early.address,
      end: early.end,
      display_tiles: { source: DISPLAY_SOURCE, encoding: "gba-4bpp", tiles: 4 },
      shared_lookup_storage: {
        encoding: "u8-backing",
        values: decodeTyped(lookup, "u8"),
        views: viewSources(early),
      },
      interpolation_coefficients: { encoding: "u32le", values: coefficients },
      surface_lookup: { encoding: "u8", rows: [surface.slice(0, 16), surface.slice(16, 32), surface.slice(32, 48)] },
      unclassified_0801356c: residualSource(part("unclassified_0801356c"), "unclassified 0801356c span"),
      render_limits: { encoding: "s32le", values: limits },
      object_programs: {
        encoding: "command32le",
        programs: programSources(part("object_programs"), entry(early, "object_programs")),
      },
      object_handlers: { encoding: "thumb-function-pointer32le", values: handlers },
      unclassified_08013724: residualSource(part("unclassified_08013724"), "unclassified 08013724 span"),
    },
    post_map_load_residual: {
      address: residual.address,
      end: residual.end,
      unreferenced_storage: residualSource(residualData, "post-map residual"),
    },
  };
}

function sourceIndex(path: string, catalog: Catalog): Json {
  const text = readFileSync(path, "utf8");
  const source = object(JSON.parse(text), "early-runtime source");
  if (!isCanonicalJsonText(text, source)) throw new Error("early-runtime source is not canonical JSON");
  exactKeys(source, [
    "format", "kind", "catalog_format", "source_bytes", "early_runtime_tables", "post_map_load_residual",
  ], "early-runtime source");
  if (source.format !== 1 || source.kind !== "golden-sun-early-runtime-data" ||
      source.catalog_format !== catalog.format || source.source_bytes !== EARLY_RUNTIME_SOURCE_BYTES) {
    throw new Error("unsupported early-runtime source");
  }
  return source;
}

function child(indexPath: string, name: unknown): string {
  if (name !== DISPLAY_SOURCE) throw new Error("display tile source name differs");
  const prefix = basename(indexPath).replace(/index\.json$/, "");
  const root = realpathSync(dirname(indexPath));
  const path = realpathSync(resolve(root, prefix + name));
  if (relative(root, path) !== prefix + name) throw new Error("display tile source escaped its package");
  return path;
}

function checkedPackageFiles(indexPath: string, catalogPath: string): void {
  const root = dirname(indexPath);
  const prefix = basename(indexPath).replace(/index\.json$/, "");
  const expected = [prefix + DISPLAY_SOURCE, prefix + "index.json"].sort();
  const catalogName = basename(catalogPath);
  // assets/data/ is a flat, shared directory: filtering by prefix alone would
  // also catch this package's own catalog sibling (early_runtime_data.json),
  // which is a real, expected neighbor rather than a stray package file.
  const names = readdirSync(root)
    .filter((name) => name.startsWith(prefix) && name !== catalogName)
    .sort();
  if (names.length !== expected.length || names.some((name, index) => name !== expected[index])) {
    throw new Error("early-runtime package contains unexpected files");
  }
  for (const name of names) {
    if (lstatSync(join(root, name)).isSymbolicLink()) throw new Error("early-runtime package contains a symbolic link");
  }
}

function place(output: Buffer, regionStart: number, spec: Entry, data: Uint8Array): void {
  const size = address(spec.end) - address(spec.address);
  if (data.length !== size) throw new Error(`${spec.id}: built size differs`);
  Buffer.from(data).copy(output, address(spec.address) - regionStart);
}

export function build_early_runtime_data(indexPath: string, catalogPath = DEFAULT_CATALOG): EarlyRuntimeBuild {
  const catalog = loadCatalog(catalogPath);
  const { early, residual } = canonicalLayout(catalog);
  const source = sourceIndex(indexPath, catalog);
  checkedPackageFiles(indexPath, catalogPath);
  const earlySource = object(source.early_runtime_tables, "early-runtime tables");
  exactKeys(earlySource, [
    "address", "end", "display_tiles", "shared_lookup_storage", "interpolation_coefficients", "surface_lookup",
    "unclassified_0801356c", "render_limits", "object_programs", "object_handlers", "unclassified_08013724",
  ], "early-runtime tables");
  if (earlySource.address !== early.address || earlySource.end !== early.end) throw new Error("early-runtime table extent differs");
  const output = Buffer.alloc(EARLY_RUNTIME_END - EARLY_RUNTIME_ADDRESS);

  const display = object(earlySource.display_tiles, "display tiles");
  exactKeys(display, ["source", "encoding", "tiles"], "display tiles");
  if (display.encoding !== "gba-4bpp" || display.tiles !== 4) throw new Error("display tile source metadata differs");
  place(output, EARLY_RUNTIME_ADDRESS, entry(early, "display_tiles"), readDisplayTiles(child(indexPath, display.source)));

  const lookup = object(earlySource.shared_lookup_storage, "shared lookup storage");
  exactKeys(lookup, ["encoding", "values", "views"], "shared lookup storage");
  if (lookup.encoding !== "u8-backing") throw new Error("shared lookup backing encoding differs");
  validateViewSources(lookup.views, early);
  const lookupSpec = entry(early, "shared_lookup_storage");
  place(output, EARLY_RUNTIME_ADDRESS, lookupSpec,
    encodeTyped(flatValues(lookup.values, address(lookupSpec.end) - address(lookupSpec.address), "u8", "shared lookup values"), "u8"));

  const coefficients = object(earlySource.interpolation_coefficients, "interpolation coefficients");
  exactKeys(coefficients, ["encoding", "values"], "interpolation coefficients");
  if (coefficients.encoding !== "u32le") throw new Error("interpolation coefficient encoding differs");
  place(output, EARLY_RUNTIME_ADDRESS, entry(early, "interpolation_coefficients"),
    encodeTyped(flatValues(coefficients.values, 16, "u32le", "interpolation coefficients"), "u32le"));

  const surface = object(earlySource.surface_lookup, "surface lookup");
  exactKeys(surface, ["encoding", "rows"], "surface lookup");
  if (surface.encoding !== "u8") throw new Error("surface lookup encoding differs");
  place(output, EARLY_RUNTIME_ADDRESS, entry(early, "surface_lookup"),
    encodeTyped(flatten(rows(surface.rows, 3, 16, "u8", "surface lookup")), "u8"));

  const unresolvedA = entry(early, "unclassified_0801356c");
  place(output, EARLY_RUNTIME_ADDRESS, unresolvedA,
    buildResidual(earlySource.unclassified_0801356c, address(unresolvedA.end) - address(unresolvedA.address), unresolvedA.id));

  const limits = object(earlySource.render_limits, "render limits");
  exactKeys(limits, ["encoding", "values"], "render limits");
  if (limits.encoding !== "s32le") throw new Error("render-limit encoding differs");
  place(output, EARLY_RUNTIME_ADDRESS, entry(early, "render_limits"),
    encodeTyped(flatValues(limits.values, 2, "s32le", "render limits"), "s32le"));

  const programs = object(earlySource.object_programs, "object programs");
  exactKeys(programs, ["encoding", "programs"], "object programs");
  if (programs.encoding !== "command32le") throw new Error("object-program encoding differs");
  const programSpec = entry(early, "object_programs");
  place(output, EARLY_RUNTIME_ADDRESS, programSpec, buildPrograms(programs.programs, programSpec));

  const handlers = object(earlySource.object_handlers, "object handlers");
  exactKeys(handlers, ["encoding", "values"], "object handlers");
  if (handlers.encoding !== "thumb-function-pointer32le") throw new Error("object-handler encoding differs");
  const handlerValues = array(handlers.values, 64, "object handlers").map((item, index) => handlerValue(item, `object handler ${index}`));
  place(output, EARLY_RUNTIME_ADDRESS, entry(early, "object_handlers"), encodeTyped(handlerValues, "pointer32le"));

  const unresolvedB = entry(early, "unclassified_08013724");
  place(output, EARLY_RUNTIME_ADDRESS, unresolvedB,
    buildResidual(earlySource.unclassified_08013724, address(unresolvedB.end) - address(unresolvedB.address), unresolvedB.id));

  const residualSource = object(source.post_map_load_residual, "post-map residual");
  exactKeys(residualSource, ["address", "end", "unreferenced_storage"], "post-map residual");
  if (residualSource.address !== residual.address || residualSource.end !== residual.end) throw new Error("post-map residual extent differs");
  const residualOutput = buildResidual(residualSource.unreferenced_storage, POST_MAP_END - POST_MAP_ADDRESS, "post-map residual");
  return {
    regions: new Map([[EARLY_RUNTIME_ADDRESS, output], [POST_MAP_ADDRESS, residualOutput]]),
    source_bytes: output.length + residualOutput.length,
  };
}

function compareBuild(rom: Uint8Array, built: EarlyRuntimeBuild): void {
  if (built.source_bytes !== EARLY_RUNTIME_SOURCE_BYTES || built.regions.size !== 2) {
    throw new Error("early-runtime build extent differs");
  }
  for (const [start, data] of built.regions) {
    if (!data.equals(range(rom, start, start + data.length))) throw new Error(`${hexadecimal(start)} differs from ROM`);
  }
}

function validDestination(path: string, catalogPath: string): void {
  if (lstatSync(path).isSymbolicLink() || !lstatSync(path).isDirectory()) {
    throw new Error("early-runtime export destination must be a directory");
  }
  build_early_runtime_data(join(path, "index.json"), catalogPath);
}

function writePackage(rom: Uint8Array, directory: string, catalog: Catalog, catalogPath: string): void {
  mkdirSync(directory, { recursive: true });
  const earlyData = range(rom, EARLY_RUNTIME_ADDRESS, EARLY_RUNTIME_END);
  const { early } = canonicalLayout(catalog);
  writeDisplayTiles(sliceRegion(earlyData, EARLY_RUNTIME_ADDRESS, entry(early, "display_tiles")), join(directory, DISPLAY_SOURCE));
  const indexPath = join(directory, "index.json");
  writeFileSync(indexPath, pretty(sourceDocument(rom, catalog)));
  compareBuild(rom, build_early_runtime_data(indexPath, catalogPath));
}

export function export_early_runtime_data(
  rom: Uint8Array,
  directory: string,
  romPath?: string,
  catalogPath = DEFAULT_CATALOG,
): void {
  const catalog = loadCatalog(catalogPath);
  const requested = resolve(directory), requestedParent = dirname(requested);
  if (requested === requestedParent) throw new Error("early-runtime export requires a dedicated directory");
  mkdirSync(requestedParent, { recursive: true });
  const parent = realpathSync(requestedParent), destination = join(parent, basename(requested));
  if (romPath !== undefined) {
    const source = realpathSync(romPath), relation = relative(destination, source);
    if (relation === "" || (!relation.startsWith("..") && !isAbsolute(relation))) {
      throw new Error("early-runtime export directory must not contain its input ROM");
    }
  }
  if (existsSync(destination)) validDestination(destination, catalogPath);
  const transaction = mkdtempSync(join(parent, ".early-runtime-export-"));
  const staged = join(transaction, "new"), previous = join(transaction, "previous");
  let installed = false;
  try {
    writePackage(rom, staged, catalog, catalogPath);
    if (existsSync(destination)) renameSync(destination, previous);
    try {
      renameSync(staged, destination);
      installed = true;
    } catch (error) {
      if (existsSync(previous)) renameSync(previous, destination);
      throw error;
    }
  } finally {
    if (!installed && existsSync(previous) && !existsSync(destination)) renameSync(previous, destination);
    rmSync(transaction, { recursive: true, force: true });
  }
}

export function verify_early_runtime_data(
  rom: Uint8Array,
  indexPath: string,
  catalogPath = DEFAULT_CATALOG,
): void {
  compareBuild(rom, build_early_runtime_data(indexPath, catalogPath));
}

function privateOutput(path: string): boolean {
  const target = resolve(path);
  const roots = [resolve(ROOT, "out"), resolve(tmpdir()), "/private/tmp"];
  return roots.some((root) => {
    const relation = relative(root, target);
    return relation !== "" && !relation.startsWith("..") && !isAbsolute(relation);
  });
}

function writeBuild(indexPath: string, directory: string, catalogPath = DEFAULT_CATALOG): void {
  if (!privateOutput(directory)) throw new Error("early-runtime build output must stay under out/ or a temporary directory");
  const built = build_early_runtime_data(indexPath, catalogPath);
  mkdirSync(directory, { recursive: true });
  const outputs: Json[] = [];
  for (const [start, data] of built.regions) {
    const name = `${start.toString(16).padStart(8, "0")}.bin`;
    writeFileSync(join(directory, name), data);
    outputs.push({ address: hexadecimal(start), size: data.length, output: name });
  }
  writeFileSync(join(directory, "manifest.json"), pretty({
    format: 1,
    kind: "golden-sun-early-runtime-build",
    source_bytes: built.source_bytes,
    regions: outputs,
  }));
}

function syntheticRom(catalog: Catalog): Buffer {
  const rom = Buffer.alloc(POST_MAP_END - ROM_BASE, 0xff);
  const early = Buffer.alloc(EARLY_RUNTIME_END - EARLY_RUNTIME_ADDRESS);
  for (let index = 0; index < early.length; index++) early[index] = (index * 73 + (index >>> 2) * 19 + 11) & 0xff;
  const { early: region } = canonicalLayout(catalog);
  const fillA = entry(region, "unclassified_0801356c");
  const fillB = entry(region, "unclassified_08013724");
  early.fill(0, address(fillA.address) - EARLY_RUNTIME_ADDRESS, address(fillA.end) - EARLY_RUNTIME_ADDRESS);
  early.fill(0xff, address(fillB.address) - EARLY_RUNTIME_ADDRESS, address(fillB.end) - EARLY_RUNTIME_ADDRESS);
  const handlers = entry(region, "object_handlers");
  const handlerData = Buffer.alloc(address(handlers.end) - address(handlers.address));
  for (let index = 0; index < 64; index++) handlerData.writeUInt32LE(index % 5 === 0 ? 0 : 0x08000101 + index * 2, index * 4);
  handlerData.copy(early, address(handlers.address) - EARLY_RUNTIME_ADDRESS);
  early.copy(rom, EARLY_RUNTIME_ADDRESS - ROM_BASE);
  rom.fill(0xff, POST_MAP_ADDRESS - ROM_BASE, POST_MAP_END - ROM_BASE);
  return rom;
}

function reject(action: () => void): boolean {
  try {
    action();
    return false;
  } catch {
    return true;
  }
}

export function self_test(): void {
  const catalog = loadCatalog();
  const temporary = mkdtempSync(join(tmpdir(), "alchemy-early-runtime-test-"));
  try {
    const rom = syntheticRom(catalog);
    const romPath = join(temporary, "gs1-en.gba");
    const destination = join(temporary, "source");
    writeFileSync(romPath, rom);
    export_early_runtime_data(rom, destination, romPath);
    const indexPath = join(destination, "index.json");
    verify_early_runtime_data(rom, indexPath);
    const built = build_early_runtime_data(indexPath);
    if (built.source_bytes !== EARLY_RUNTIME_SOURCE_BYTES) throw new Error("synthetic source-byte count differs");
    const buildDirectory = join(temporary, "build");
    writeBuild(indexPath, buildDirectory);
    if (!existsSync(join(buildDirectory, "08012f20.bin")) || !existsSync(join(buildDirectory, "0801403c.bin"))) {
      throw new Error("synthetic build output differs");
    }

    const sourceText = readFileSync(indexPath, "utf8");
    const image = readFileSync(join(destination, DISPLAY_SOURCE));
    const source = JSON.parse(sourceText);
    let adversarial = 0;
    const mutate = (change: (value: Json) => void): void => {
      const value = JSON.parse(sourceText);
      change(value);
      writeFileSync(indexPath, pretty(value));
      if (reject(() => build_early_runtime_data(indexPath))) adversarial++;
      writeFileSync(indexPath, sourceText);
    };
    mutate((value) => value.extra = true);
    mutate((value) => value.early_runtime_tables.shared_lookup_storage.values.pop());
    mutate((value) => value.early_runtime_tables.shared_lookup_storage.values[0] = 256);
    mutate((value) => value.early_runtime_tables.shared_lookup_storage.views[0].end = "0x0801349f");
    mutate((value) => value.early_runtime_tables.surface_lookup.rows[0].push(0));
    mutate((value) => value.early_runtime_tables.unclassified_0801356c.value = "0x7f");
    mutate((value) => value.early_runtime_tables.object_programs.programs[0].words[0] = "0X00000000");
    mutate((value) => value.early_runtime_tables.object_handlers.values[1] = "0x08000100");
    mutate((value) => value.post_map_load_residual.unreferenced_storage.kind = "bytes");
    writeFileSync(indexPath, `${sourceText}\n`);
    if (reject(() => build_early_runtime_data(indexPath))) adversarial++;
    writeFileSync(indexPath, sourceText);
    writeFileSync(join(destination, DISPLAY_SOURCE), Buffer.concat([image, Buffer.from([0])]));
    if (reject(() => build_early_runtime_data(indexPath))) adversarial++;
    writeFileSync(join(destination, DISPLAY_SOURCE), image);
    writeFileSync(join(destination, "hidden.bin"), Buffer.alloc(1));
    if (reject(() => build_early_runtime_data(indexPath))) adversarial++;
    unlinkSync(join(destination, "hidden.bin"));
    const outside = join(temporary, "outside.png");
    writeFileSync(outside, image);
    unlinkSync(join(destination, DISPLAY_SOURCE));
    symlinkSync(outside, join(destination, DISPLAY_SOURCE));
    if (reject(() => build_early_runtime_data(indexPath))) adversarial++;
    unlinkSync(join(destination, DISPLAY_SOURCE));
    writeFileSync(join(destination, DISPLAY_SOURCE), image);
    if (reject(() => encodeTyped([256], "u8"))) adversarial++;
    if (reject(() => encodeTyped([-32769], "s16le"))) adversarial++;

    const badCatalog = structuredClone(catalog);
    badCatalog.regions[0].views[0].end = "0x080134ff";
    if (reject(() => validateCatalog(badCatalog))) adversarial++;
    const badSegments = structuredClone(catalog);
    badSegments.regions[0].partitions.find((item) => item.id === "object_programs")!.segments![0]++;
    if (reject(() => validateCatalog(badSegments))) adversarial++;

    const preservedIndex = readFileSync(indexPath);
    const preservedImage = readFileSync(join(destination, DISPLAY_SOURCE));
    const badRom = Buffer.from(rom);
    // A malformed handler pointer is rejected before the transactional export
    // can replace an existing package. Ordinary residual bytes are valid
    // byte-value sources and must no longer be used as an artificial failure.
    badRom.writeUInt32LE(0x08000000, 0x08013624 - ROM_BASE);
    if (reject(() => export_early_runtime_data(badRom, destination, romPath))) adversarial++;
    if (!readFileSync(indexPath).equals(preservedIndex) || !readFileSync(join(destination, DISPLAY_SOURCE)).equals(preservedImage)) {
      throw new Error("failed transactional export changed the installed package");
    }
    if (adversarial !== 18) throw new Error(`adversarial validation count differs: ${adversarial}`);

    const changedRom = Buffer.from(rom);
    changedRom[EARLY_RUNTIME_ADDRESS - ROM_BASE] ^= 0x11;
    export_early_runtime_data(changedRom, destination, romPath);
    verify_early_runtime_data(changedRom, join(destination, "index.json"));
    if (readFileSync(join(destination, DISPLAY_SOURCE)).equals(preservedImage)) {
      throw new Error("successful transactional export did not replace the package");
    }
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log(`self-test=ok source_bytes=${EARLY_RUNTIME_SOURCE_BYTES} adversarial=18 transaction=atomic`);
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") {
    self_test();
  } else if (args.length === 4 && args[0] === "export" && args[2] === "--directory") {
    const romPath = resolve(args[1]);
    const rom = readFileSync(romPath);
    export_early_runtime_data(rom, args[3], romPath);
    console.log(`source_bytes=${EARLY_RUNTIME_SOURCE_BYTES}`);
  } else if (args.length === 3 && args[0] === "verify") {
    verify_early_runtime_data(readFileSync(args[1]), args[2]);
    console.log(`identical=true source_bytes=${EARLY_RUNTIME_SOURCE_BYTES}`);
  } else if (args.length === 4 && args[0] === "build" && args[2] === "--directory") {
    writeBuild(args[1], args[3]);
    console.log(`source_bytes=${EARLY_RUNTIME_SOURCE_BYTES}`);
  } else {
    throw new Error("usage: early_runtime_data.ts export ROM --directory DIR | build INDEX --directory DIR | verify ROM INDEX | --self-test");
  }
}

if (import.meta.main) main(process.argv.slice(2));
