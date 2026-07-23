#!/usr/bin/env bun
// Tool role: both; imported by tools/late_runtime_residual.ts; invoked by package.json.
import { canonicalJson, isCanonicalJsonText } from "./canonical_json.ts";
import {
  existsSync,
  lstatSync,
  mkdirSync,
  mkdtempSync,
  readFileSync,
  readdirSync,
  realpathSync,
  renameSync,
  rmSync,
  symlinkSync,
  writeFileSync,
} from "fs";
import { basename, dirname, isAbsolute, join, relative, resolve } from "path";
import { tmpdir } from "os";
import { decode_general_trace, encode_general, type GeneralToken } from "./extract_resource.ts";

export const ROM_BASE = 0x08000000;
export const ROM_SIZE = 0x00800000;

const PACKAGE_FILES = {
  catalog: "catalog.json",
  stream: "haikei.json",
  plan: "haikei.lz.json",
  tables: "tables.json",
  fills: "fills.json",
} as const;

const CANONICAL_RANGES = [
  ["range_080f38bc", 0x080f38bc, 0x080f4000],
  ["range_080f53ce", 0x080f53ce, 0x080f6000],
  ["range_080f86f8", 0x080f86f8, 0x080f9000],
] as const;

const CANONICAL_COMPONENTS = [
  ["haikei_stream", "general_lz_stream", 0x080f38bc, 0x080f39ab, 600],
  ["haichi_hyou", "u8_table", 0x080f39ab, 0x080f39b1, 6],
  ["henka_hyou", "u8_table", 0x080f39b1, 0x080f39ed, 60],
  ["soroe_080f39ed", "unresolved_fill", 0x080f39ed, 0x080f39ee, 0],
  ["iro_henka_00", "u16_table", 0x080f39ee, 0x080f3a2e, 32],
  ["iro_henka_01", "u16_table", 0x080f3a2e, 0x080f3a6e, 32],
  ["iro_henka_02", "u16_table", 0x080f3a6e, 0x080f3aae, 32],
  ["aki_080f3aae", "unresolved_fill", 0x080f3aae, 0x080f4000, 0],
  ["aki_080f53ce", "unresolved_fill", 0x080f53ce, 0x080f541a, 0],
  ["hantei_dankai", "s16_table", 0x080f541a, 0x080f5426, 6],
  ["aki_080f5426", "unresolved_fill", 0x080f5426, 0x080f6000, 0],
  ["zahyoubisei_hyou", "u16_table", 0x080f86f8, 0x080f870c, 10],
  ["hyouji_shoki_hyou", "u8_table", 0x080f870c, 0x080f8712, 6],
  ["haba_hyou_00", "u8_table", 0x080f8712, 0x080f871a, 8],
  ["haichi_hyou_00", "u8_table", 0x080f871a, 0x080f8728, 14],
  ["haichi_hyou_01", "u8_table", 0x080f8728, 0x080f8736, 14],
  ["haba_hyou_01", "u8_table", 0x080f8736, 0x080f873e, 8],
  ["aki_080f873e", "unresolved_fill", 0x080f873e, 0x080f9000, 0],
] as const;

type JsonObject = Record<string, unknown>;
type TableKind = "u8_table" | "s16_table" | "u16_table";
type ComponentKind = "general_lz_stream" | TableKind | "fill" | "unresolved_fill";

export interface LateRuntimeComponent {
  name: string;
  kind: ComponentKind;
  address: number;
  end: number;
  role: string;
  consumers: string[];
  count?: number;
  decodedBytes?: number;
  value?: number;
  reason?: string;
}

export interface LateRuntimeRange {
  name: string;
  address: number;
  end: number;
  components: LateRuntimeComponent[];
}

export interface LateRuntimeCatalog {
  format: 1;
  kind: "golden-sun-late-runtime-data-layout";
  ranges: LateRuntimeRange[];
}

export type LateRuntimeSource =
  | { kind: "general_lz_stream"; decoded: Buffer; tokens: GeneralToken[]; lookahead: Buffer }
  | { kind: TableKind; values: number[] }
  | { kind: "fill" };

export interface LateRuntimeExtraction {
  sources: Map<string, LateRuntimeSource>;
  unresolved: LateRuntimeComponent[];
}

export interface LateRuntimeBuild {
  regions: Map<number, Buffer>;
  unresolved: LateRuntimeComponent[];
  typedBytes: number;
}

interface PackageIndex {
  format: 1;
  kind: "golden-sun-late-runtime-data";
  range_bytes: 7294;
  sources: typeof PACKAGE_FILES;
}

interface StreamDocument {
  format: 1;
  kind: "golden-sun-late-runtime-u16-source";
  name: string;
  address: string;
  end: string;
  role: string;
  decoded_bytes: 600;
  element_type: "u16le";
  words: number[];
}

interface FillRecipe {
  pattern: "uniform";
  value: 0 | 255;
  bytes: number;
}

interface PlanDocument {
  format: 1;
  kind: "golden-sun-late-runtime-lz-plan";
  name: string;
  codec: "golden-sun-general-lz";
  address: string;
  end: string;
  decoded_bytes: 600;
  tokens: GeneralToken[];
  trailing_fill: FillRecipe | null;
}

interface TableDocumentEntry {
  name: string;
  type: "u8" | "s16" | "u16";
  address: string;
  end: string;
  role: string;
  values: number[];
}

interface TablesDocument {
  format: 1;
  kind: "golden-sun-late-runtime-tables";
  tables: TableDocumentEntry[];
}

interface FillDocumentEntry extends FillRecipe {
  name: string;
  address: string;
  end: string;
  role: string;
}

interface FillsDocument {
  format: 1;
  kind: "golden-sun-late-runtime-structural-fills";
  spans: FillDocumentEntry[];
}

export interface LateRuntimePackageBuild {
  ranges: Map<number, Buffer>;
  components: Map<number, Buffer>;
  sourceBytes: number;
}

function exactKeys(value: unknown, keys: readonly string[], label: string): asserts value is JsonObject {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${label} must be an object`);
  const actual = Object.keys(value).sort(), expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${label} has unknown or missing fields`);
  }
}

function pretty(value: unknown): string {
  return `${canonicalJson(value)}\n`;
}

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function canonicalDocument(path: string, label: string): unknown {
  const text = readFileSync(path, "utf8"), value = JSON.parse(text);
  if (!isCanonicalJsonText(text, value)) throw new Error(`${label} is not canonical JSON`);
  return value;
}

function regularFile(path: string, label: string): void {
  if (!existsSync(path) || !lstatSync(path).isFile()) throw new Error(`${label} is not a regular file`);
}

function integer(value: unknown, minimum: number, maximum: number, label: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < minimum || value > maximum) {
    throw new Error(`${label} is outside its range`);
  }
  return value;
}

function address(value: unknown, label: string): number {
  if (typeof value !== "string" || !/^0x08[0-9a-f]{6}$/.test(value)) throw new Error(`${label} is not a canonical ROM address`);
  return Number.parseInt(value.slice(2), 16);
}

function identifier(value: unknown, label: string): string {
  if (typeof value !== "string" || !/^[a-z][a-z0-9_]*$/.test(value)) throw new Error(`${label} is not a canonical identifier`);
  return value;
}

function consumerList(value: unknown, label: string): string[] {
  if (!Array.isArray(value) || value.length === 0 || value.some((item) =>
    typeof item !== "string" || !/^Func_080[0-9a-f]{5}$/.test(item))) throw new Error(`${label} differs`);
  if (new Set(value).size !== value.length) throw new Error(`${label} contains duplicates`);
  return value as string[];
}

function tableWidth(kind: TableKind): number {
  return kind === "u8_table" ? 1 : 2;
}

function parseComponent(value: unknown, index: number): LateRuntimeComponent {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`component ${index} must be an object`);
  const raw = value as JsonObject;
  const kind = raw.kind;
  exactKeys(raw, [
    "name", "kind", "address", "end", "role", "consumers",
    ...(typeof kind === "string" && kind.endsWith("_table") ? ["count"] : []),
    ...(kind === "general_lz_stream" ? ["decoded_bytes"] : []),
    ...(kind === "fill" ? ["value"] : []),
    ...(kind === "unresolved_fill" ? ["reason"] : []),
  ], `late runtime component ${index}`);
  const name = identifier(raw.name, `component ${index} name`);
  if (!["general_lz_stream", "u8_table", "s16_table", "u16_table", "fill", "unresolved_fill"].includes(String(kind))) {
    throw new Error(`component ${name} kind differs`);
  }
  const start = address(raw.address, `component ${name} address`), end = address(raw.end, `component ${name} end`);
  if (end <= start) throw new Error(`component ${name} has an empty extent`);
  const result: LateRuntimeComponent = {
    name, kind: kind as ComponentKind, address: start, end,
    role: identifier(raw.role, `component ${name} role`),
    consumers: consumerList(raw.consumers, `component ${name} consumers`),
  };
  if (kind === "general_lz_stream") {
    result.decodedBytes = integer(raw.decoded_bytes, 1, 0x100000, `component ${name} decoded bytes`);
  } else if (kind === "u8_table" || kind === "s16_table" || kind === "u16_table") {
    result.count = integer(raw.count, 1, 0x100000, `component ${name} count`);
    if (result.count * tableWidth(kind) !== end - start) throw new Error(`component ${name} table extent differs`);
  } else if (kind === "fill") result.value = integer(raw.value, 0, 0xff, `component ${name} fill value`);
  else result.reason = identifier(raw.reason, `component ${name} reason`);
  return result;
}

export function parseLateRuntimeCatalog(value: unknown): LateRuntimeCatalog {
  exactKeys(value, ["format", "kind", "ranges"], "late runtime catalog");
  if (value.format !== 1 || value.kind !== "golden-sun-late-runtime-data-layout" ||
      !Array.isArray(value.ranges) || value.ranges.length === 0) throw new Error("unsupported late runtime catalog");
  const names = new Set<string>();
  let previousEnd = 0;
  const ranges = value.ranges.map((raw, rangeIndex): LateRuntimeRange => {
    exactKeys(raw, ["name", "address", "end", "components"], `late runtime range ${rangeIndex}`);
    const name = identifier(raw.name, `range ${rangeIndex} name`);
    const start = address(raw.address, `range ${name} address`), end = address(raw.end, `range ${name} end`);
    if (end <= start || start < previousEnd || !Array.isArray(raw.components) || raw.components.length === 0) {
      throw new Error(`range ${name} extent differs`);
    }
    previousEnd = end;
    const components = raw.components.map(parseComponent);
    let cursor = start;
    for (const component of components) {
      if (names.has(component.name)) throw new Error(`duplicate component ${component.name}`);
      names.add(component.name);
      if (component.address !== cursor || component.end > end) throw new Error(`range ${name} is not contiguous`);
      cursor = component.end;
    }
    if (cursor !== end) throw new Error(`range ${name} does not reach its end`);
    return { name, address: start, end, components };
  });
  const catalog: LateRuntimeCatalog = { format: 1, kind: "golden-sun-late-runtime-data-layout", ranges };
  if (ranges.length !== CANONICAL_RANGES.length || ranges.some((range, index) => {
    const expected = CANONICAL_RANGES[index];
    return range.name !== expected[0] || range.address !== expected[1] || range.end !== expected[2];
  })) throw new Error("late runtime canonical range layout differs");
  const components = ranges.flatMap((range) => range.components);
  if (components.length !== CANONICAL_COMPONENTS.length || components.some((component, index) => {
    const expected = CANONICAL_COMPONENTS[index];
    const quantity = component.kind === "general_lz_stream" ? component.decodedBytes : component.count ?? 0;
    return component.name !== expected[0] || component.kind !== expected[1] || component.address !== expected[2] ||
      component.end !== expected[3] || quantity !== expected[4];
  })) throw new Error("late runtime canonical component layout differs");
  return catalog;
}

export function readLateRuntimeCatalog(path: string): LateRuntimeCatalog {
  return parseLateRuntimeCatalog(canonicalDocument(path, "late runtime catalog"));
}

function componentBytes(image: Uint8Array, component: LateRuntimeComponent, imageBase: number): Buffer {
  const start = component.address - imageBase, end = component.end - imageBase;
  if (start < 0 || end > image.length) throw new Error(`component ${component.name} is outside the image`);
  return Buffer.from(image.subarray(start, end));
}

function decodeTable(kind: TableKind, data: Buffer): number[] {
  if (kind === "u8_table") return Array.from(data);
  const values: number[] = [];
  for (let offset = 0; offset < data.length; offset += 2) {
    values.push(kind === "s16_table" ? data.readInt16LE(offset) : data.readUInt16LE(offset));
  }
  return values;
}

export function extractLateRuntimeData(image: Uint8Array, catalog: LateRuntimeCatalog, imageBase = ROM_BASE): LateRuntimeExtraction {
  const sources = new Map<string, LateRuntimeSource>(), unresolved: LateRuntimeComponent[] = [];
  for (const range of catalog.ranges) for (const component of range.components) {
    if (component.kind === "unresolved_fill") { unresolved.push(component); continue; }
    const data = componentBytes(image, component, imageBase);
    if (component.kind === "fill") {
      if (data.some((value) => value !== component.value)) throw new Error(`component ${component.name} fill differs`);
      sources.set(component.name, { kind: "fill" });
    } else if (component.kind === "general_lz_stream") {
      const [decoded, , tokens] = decode_general_trace(data, 0, data.length, component.decodedBytes!);
      if (decoded.length !== component.decodedBytes) throw new Error(`component ${component.name} decoded extent differs`);
      const replay = encode_general(decoded, tokens);
      if (replay.length > data.length || !replay.equals(data.subarray(0, replay.length))) {
        throw new Error(`component ${component.name} compression replay differs`);
      }
      sources.set(component.name, { kind: "general_lz_stream", decoded, tokens, lookahead: data.subarray(replay.length) });
    } else sources.set(component.name, { kind: component.kind, values: decodeTable(component.kind, data) });
  }
  return { sources, unresolved };
}

function encodeTable(component: LateRuntimeComponent, values: number[]): Buffer {
  if (values.length !== component.count) throw new Error(`component ${component.name} table count differs`);
  const output = Buffer.alloc(component.end - component.address);
  values.forEach((value, index) => {
    if (!Number.isSafeInteger(value)) throw new Error(`component ${component.name} contains a non-integer`);
    if (component.kind === "u8_table") {
      if (value < 0 || value > 0xff) throw new Error(`component ${component.name} value does not fit u8`);
      output.writeUInt8(value, index);
    } else if (component.kind === "s16_table") {
      if (value < -0x8000 || value > 0x7fff) throw new Error(`component ${component.name} value does not fit s16`);
      output.writeInt16LE(value, index * 2);
    } else {
      if (value < 0 || value > 0xffff) throw new Error(`component ${component.name} value does not fit u16`);
      output.writeUInt16LE(value, index * 2);
    }
  });
  return output;
}

export function buildLateRuntimeData(catalog: LateRuntimeCatalog, sources: ReadonlyMap<string, LateRuntimeSource>): LateRuntimeBuild {
  const regions = new Map<number, Buffer>(), unresolved: LateRuntimeComponent[] = [];
  let typedBytes = 0;
  for (const range of catalog.ranges) for (const component of range.components) {
    if (component.kind === "unresolved_fill") { unresolved.push(component); continue; }
    const source = sources.get(component.name);
    if (source === undefined || source.kind !== component.kind) throw new Error(`component ${component.name} source differs`);
    let data: Buffer;
    if (component.kind === "fill") data = Buffer.alloc(component.end - component.address, component.value);
    else if (component.kind === "general_lz_stream") {
      const stream = source as Extract<LateRuntimeSource, { kind: "general_lz_stream" }>;
      if (stream.decoded.length !== component.decodedBytes) throw new Error(`component ${component.name} decoded extent differs`);
      data = Buffer.concat([encode_general(stream.decoded, stream.tokens), stream.lookahead]);
    } else data = encodeTable(component, (source as Extract<LateRuntimeSource, { values: number[] }>).values);
    if (data.length !== component.end - component.address) throw new Error(`component ${component.name} encoded extent differs`);
    regions.set(component.address, data); typedBytes += data.length;
  }
  return { regions, unresolved, typedBytes };
}

function packageComponents(catalog: LateRuntimeCatalog): LateRuntimeComponent[] {
  return catalog.ranges.flatMap((range) => range.components);
}

function singleStream(catalog: LateRuntimeCatalog): LateRuntimeComponent {
  const streams = packageComponents(catalog).filter((component) => component.kind === "general_lz_stream");
  if (streams.length !== 1) throw new Error("late runtime catalog must contain one compressed stream");
  return streams[0];
}

function tableType(kind: TableKind): TableDocumentEntry["type"] {
  return kind === "u8_table" ? "u8" : kind === "s16_table" ? "s16" : "u16";
}

function parseTokens(value: unknown, decodedBytes: number, label: string): GeneralToken[] {
  if (!Array.isArray(value) || value.length === 0 || value.length > decodedBytes) throw new Error(`${label} differs`);
  let decoded = 0;
  const tokens = value.map((raw, index): GeneralToken => {
    if (!Array.isArray(raw) || (raw[0] !== "l" && raw[0] !== "c")) throw new Error(`${label} token ${index} opcode differs`);
    if (raw[0] === "l") {
      if (raw.length !== 2) throw new Error(`${label} token ${index} literal tuple differs`);
      const count = integer(raw[1], 1, decodedBytes, `${label} token ${index} literal count`);
      if (decoded + count > decodedBytes) throw new Error(`${label} token ${index} crosses the decoded extent`);
      decoded += count;
      return ["l", count];
    }
    if (raw.length !== 3) throw new Error(`${label} token ${index} copy tuple differs`);
    const length = integer(raw[1], 2, 137, `${label} token ${index} copy length`);
    const distance = integer(raw[2], 1, decodedBytes, `${label} token ${index} copy distance`);
    if (distance > decoded || decoded + length > decodedBytes) throw new Error(`${label} token ${index} crosses its replay bounds`);
    decoded += length;
    return ["c", length, distance];
  });
  if (decoded !== decodedBytes) throw new Error(`${label} does not cover the decoded source`);
  return tokens;
}

function parseFillRecipe(value: unknown, expectedBytes: number, label: string): FillRecipe {
  exactKeys(value, ["pattern", "value", "bytes"], label);
  if (value.pattern !== "uniform" || (value.value !== 0 && value.value !== 255) ||
      value.bytes !== expectedBytes || expectedBytes < 1) throw new Error(`${label} differs`);
  return { pattern: "uniform", value: value.value, bytes: expectedBytes };
}

function uniformFill(data: Uint8Array, label: string): FillRecipe {
  if (data.length === 0) throw new Error(`${label} is empty`);
  const value = data[0];
  if ((value !== 0 && value !== 255) || data.some((item) => item !== value)) {
    throw new Error(`${label} is not a narrow structural fill pattern`);
  }
  return { pattern: "uniform", value, bytes: data.length };
}

function parseIndex(value: unknown): PackageIndex {
  exactKeys(value, ["format", "kind", "range_bytes", "sources"], "late runtime package index");
  exactKeys(value.sources, ["catalog", "stream", "plan", "tables", "fills"], "late runtime package sources");
  if (value.format !== 1 || value.kind !== "golden-sun-late-runtime-data" || value.range_bytes !== 7_294 ||
      !Bun.deepEquals(value.sources, PACKAGE_FILES, true)) throw new Error("late runtime package index differs");
  return value as unknown as PackageIndex;
}

function parseStreamDocument(value: unknown, component: LateRuntimeComponent): StreamDocument {
  exactKeys(value, ["format", "kind", "name", "address", "end", "role", "decoded_bytes", "element_type", "words"], "late runtime stream source");
  if (value.format !== 1 || value.kind !== "golden-sun-late-runtime-u16-source" ||
      value.name !== component.name || value.address !== hexadecimal(component.address) ||
      value.end !== hexadecimal(component.end) || value.role !== component.role || value.decoded_bytes !== component.decodedBytes ||
      value.element_type !== "u16le" || !Array.isArray(value.words) || value.words.length * 2 !== component.decodedBytes) {
    throw new Error("late runtime stream source differs");
  }
  const words = value.words.map((word, index) => integer(word, 0, 0xffff, `late runtime stream word ${index}`));
  return { ...value, words } as unknown as StreamDocument;
}

function parsePlanDocument(value: unknown, component: LateRuntimeComponent): PlanDocument {
  exactKeys(value, ["format", "kind", "name", "codec", "address", "end", "decoded_bytes", "tokens", "trailing_fill"], "late runtime compression plan");
  if (value.format !== 1 || value.kind !== "golden-sun-late-runtime-lz-plan" ||
      value.name !== component.name || value.codec !== "golden-sun-general-lz" ||
      value.address !== hexadecimal(component.address) || value.end !== hexadecimal(component.end) ||
      value.decoded_bytes !== component.decodedBytes) throw new Error("late runtime compression plan differs");
  const tokens = parseTokens(value.tokens, component.decodedBytes!, "late runtime compression plan");
  const fill = value.trailing_fill;
  if (fill !== null && (typeof fill !== "object" || Array.isArray(fill))) throw new Error("late runtime stream trailing fill differs");
  return { ...value, tokens, trailing_fill: fill as FillRecipe | null } as unknown as PlanDocument;
}

function parseTablesDocument(value: unknown, catalog: LateRuntimeCatalog): TablesDocument {
  exactKeys(value, ["format", "kind", "tables"], "late runtime tables source");
  const expected = packageComponents(catalog).filter((component): component is LateRuntimeComponent & { kind: TableKind } =>
    component.kind === "u8_table" || component.kind === "s16_table" || component.kind === "u16_table");
  if (value.format !== 1 || value.kind !== "golden-sun-late-runtime-tables" ||
      !Array.isArray(value.tables) || value.tables.length !== expected.length) throw new Error("late runtime tables source differs");
  const tables = value.tables.map((raw, index): TableDocumentEntry => {
    exactKeys(raw, ["name", "type", "address", "end", "role", "values"], `late runtime table ${index}`);
    const component = expected[index];
    if (raw.name !== component.name || raw.type !== tableType(component.kind) ||
        raw.address !== hexadecimal(component.address) || raw.end !== hexadecimal(component.end) ||
        raw.role !== component.role || !Array.isArray(raw.values) || raw.values.length !== component.count) {
      throw new Error(`late runtime table ${component.name} differs`);
    }
    const minimum = component.kind === "s16_table" ? -0x8000 : 0;
    const maximum = component.kind === "u8_table" ? 0xff : component.kind === "s16_table" ? 0x7fff : 0xffff;
    const values = raw.values.map((item, position) => integer(item, minimum, maximum, `${component.name} value ${position}`));
    return { name: component.name, type: tableType(component.kind), address: hexadecimal(component.address),
      end: hexadecimal(component.end), role: component.role, values };
  });
  return { format: 1, kind: "golden-sun-late-runtime-tables", tables };
}

function parseFillsDocument(value: unknown, catalog: LateRuntimeCatalog): FillsDocument {
  exactKeys(value, ["format", "kind", "spans"], "late runtime fills source");
  const expected = packageComponents(catalog).filter((component) => component.kind === "unresolved_fill");
  if (value.format !== 1 || value.kind !== "golden-sun-late-runtime-structural-fills" ||
      !Array.isArray(value.spans) || value.spans.length !== expected.length) throw new Error("late runtime fills source differs");
  const spans = value.spans.map((raw, index): FillDocumentEntry => {
    exactKeys(raw, ["name", "address", "end", "role", "pattern", "value", "bytes"], `late runtime fill ${index}`);
    const component = expected[index];
    if (raw.name !== component.name || raw.address !== hexadecimal(component.address) ||
        raw.end !== hexadecimal(component.end) || raw.role !== component.role) throw new Error(`late runtime fill ${component.name} differs`);
    const recipe = parseFillRecipe({ pattern: raw.pattern, value: raw.value, bytes: raw.bytes },
      component.end - component.address, `late runtime fill ${component.name}`);
    return { name: component.name, address: hexadecimal(component.address), end: hexadecimal(component.end),
      role: component.role, ...recipe };
  });
  return { format: 1, kind: "golden-sun-late-runtime-structural-fills", spans };
}

function decodedWords(source: StreamDocument): Buffer {
  const output = Buffer.alloc(source.decoded_bytes);
  source.words.forEach((word, index) => output.writeUInt16LE(word, index * 2));
  return output;
}

function packageChild(indexPath: string, leaf: string, label: string): string {
  if (basename(leaf) !== leaf) throw new Error(`${label} must be a local leaf`);
  const directory = realpathSync(dirname(indexPath)), candidate = resolve(directory, leaf);
  regularFile(candidate, label);
  const physical = realpathSync(candidate), relation = relative(directory, physical);
  if (relation !== leaf) throw new Error(`${label} escaped its package`);
  return physical;
}

export function buildLateRuntimePackage(indexPath: string): LateRuntimePackageBuild {
  regularFile(indexPath, "late runtime package index");
  const index = parseIndex(canonicalDocument(indexPath, "late runtime package index"));
  const catalog = readLateRuntimeCatalog(packageChild(indexPath, index.sources.catalog, "late runtime catalog"));
  const streamComponent = singleStream(catalog);
  const stream = parseStreamDocument(canonicalDocument(packageChild(indexPath, index.sources.stream, "late runtime stream source"), "late runtime stream source"), streamComponent);
  const plan = parsePlanDocument(canonicalDocument(packageChild(indexPath, index.sources.plan, "late runtime compression plan"), "late runtime compression plan"), streamComponent);
  const tables = parseTablesDocument(canonicalDocument(packageChild(indexPath, index.sources.tables, "late runtime tables source"), "late runtime tables source"), catalog);
  const fills = parseFillsDocument(canonicalDocument(packageChild(indexPath, index.sources.fills, "late runtime fills source"), "late runtime fills source"), catalog);
  const components = new Map<number, Buffer>();
  const decoded = decodedWords(stream);
  const encoded = encode_general(decoded, plan.tokens);
  const trailingBytes = streamComponent.end - streamComponent.address - encoded.length;
  let trailing = Buffer.alloc(0);
  if (trailingBytes < 0) throw new Error("late runtime compressed stream crossed its extent");
  if (trailingBytes === 0) {
    if (plan.trailing_fill !== null) throw new Error("late runtime compression plan has a spurious trailing fill");
  } else {
    if (plan.trailing_fill === null) throw new Error("late runtime compression plan lacks its trailing fill");
    const recipe = parseFillRecipe(plan.trailing_fill, trailingBytes, "late runtime stream trailing fill");
    trailing = Buffer.alloc(recipe.bytes, recipe.value);
  }
  components.set(streamComponent.address, Buffer.concat([encoded, trailing]));
  const tableComponents = packageComponents(catalog).filter((component): component is LateRuntimeComponent & { kind: TableKind } =>
    component.kind === "u8_table" || component.kind === "s16_table" || component.kind === "u16_table");
  tableComponents.forEach((component, index) => components.set(component.address, encodeTable(component, tables.tables[index].values)));
  const fillComponents = packageComponents(catalog).filter((component) => component.kind === "unresolved_fill");
  fillComponents.forEach((component, index) => {
    const fill = fills.spans[index];
    components.set(component.address, Buffer.alloc(component.end - component.address, fill.value));
  });
  for (const component of packageComponents(catalog).filter((item) => item.kind === "fill")) {
    components.set(component.address, Buffer.alloc(component.end - component.address, component.value));
  }
  const ranges = new Map<number, Buffer>();
  for (const range of catalog.ranges) {
    const parts = range.components.map((component) => {
      const data = components.get(component.address);
      if (data === undefined || data.length !== component.end - component.address) throw new Error(`late runtime component ${component.name} is incomplete`);
      return data;
    });
    const data = Buffer.concat(parts);
    if (data.length !== range.end - range.address) throw new Error(`late runtime range ${range.name} is incomplete`);
    ranges.set(range.address, data);
  }
  const sourceBytes = [...ranges.values()].reduce((sum, data) => sum + data.length, 0);
  if (sourceBytes !== index.range_bytes) throw new Error("late runtime source-byte total differs");
  return { ranges, components, sourceBytes };
}

function streamDocuments(source: LateRuntimeSource, component: LateRuntimeComponent): [StreamDocument, PlanDocument] {
  if (source.kind !== "general_lz_stream") throw new Error("late runtime extracted stream source differs");
  if (source.decoded.length !== component.decodedBytes || source.decoded.length % 2 !== 0) {
    throw new Error("late runtime decoded stream is not a bounded halfword source");
  }
  const words = Array.from({ length: source.decoded.length / 2 }, (_, index) => source.decoded.readUInt16LE(index * 2));
  const trailingFill = source.lookahead.length === 0 ? null : uniformFill(source.lookahead, "late runtime stream trailing fill");
  return [{
    format: 1,
    kind: "golden-sun-late-runtime-u16-source",
    name: component.name,
    address: hexadecimal(component.address),
    end: hexadecimal(component.end),
    role: component.role,
    decoded_bytes: component.decodedBytes as 600,
    element_type: "u16le",
    words,
  }, {
    format: 1,
    kind: "golden-sun-late-runtime-lz-plan",
    name: component.name,
    codec: "golden-sun-general-lz",
    address: hexadecimal(component.address),
    end: hexadecimal(component.end),
    decoded_bytes: component.decodedBytes as 600,
    tokens: source.tokens,
    trailing_fill: trailingFill,
  }];
}

function tableDocument(extraction: LateRuntimeExtraction, catalog: LateRuntimeCatalog): TablesDocument {
  const tables = packageComponents(catalog).filter((component): component is LateRuntimeComponent & { kind: TableKind } =>
    component.kind === "u8_table" || component.kind === "s16_table" || component.kind === "u16_table").map((component) => {
      const source = extraction.sources.get(component.name);
      if (source === undefined || source.kind !== component.kind) throw new Error(`late runtime extracted table ${component.name} differs`);
      return {
        name: component.name,
        type: tableType(component.kind),
        address: hexadecimal(component.address),
        end: hexadecimal(component.end),
        role: component.role,
        values: source.values,
      };
    });
  return { format: 1, kind: "golden-sun-late-runtime-tables", tables };
}

function fillDocument(image: Uint8Array, catalog: LateRuntimeCatalog, imageBase: number): FillsDocument {
  const spans = packageComponents(catalog).filter((component) => component.kind === "unresolved_fill").map((component) => ({
    name: component.name,
    address: hexadecimal(component.address),
    end: hexadecimal(component.end),
    role: component.role,
    ...uniformFill(componentBytes(image, component, imageBase), `late runtime fill ${component.name}`),
  }));
  return { format: 1, kind: "golden-sun-late-runtime-structural-fills", spans };
}

function packageIndex(): PackageIndex {
  return { format: 1, kind: "golden-sun-late-runtime-data", range_bytes: 7_294, sources: { ...PACKAGE_FILES } };
}

function writePackage(image: Uint8Array, catalogPath: string, directory: string, imageBase: number): LateRuntimePackageBuild {
  const catalogValue = canonicalDocument(catalogPath, "late runtime catalog"), catalog = parseLateRuntimeCatalog(catalogValue);
  const extraction = extractLateRuntimeData(image, catalog, imageBase), streamComponent = singleStream(catalog);
  const streamSource = extraction.sources.get(streamComponent.name);
  if (streamSource === undefined) throw new Error("late runtime stream was not extracted");
  const [stream, plan] = streamDocuments(streamSource, streamComponent);
  const tables = tableDocument(extraction, catalog), fills = fillDocument(image, catalog, imageBase);
  mkdirSync(directory, { recursive: true });
  writeFileSync(join(directory, "index.json"), pretty(packageIndex()));
  writeFileSync(join(directory, PACKAGE_FILES.catalog), pretty(catalogValue));
  writeFileSync(join(directory, PACKAGE_FILES.stream), pretty(stream));
  writeFileSync(join(directory, PACKAGE_FILES.plan), pretty(plan));
  writeFileSync(join(directory, PACKAGE_FILES.tables), pretty(tables));
  writeFileSync(join(directory, PACKAGE_FILES.fills), pretty(fills));
  const built = buildLateRuntimePackage(join(directory, "index.json"));
  verifyLateRuntimePackageImage(image, join(directory, "index.json"), imageBase, built);
  return built;
}

function physicalPath(path: string): string {
  const absolute = resolve(path);
  let ancestor = absolute;
  while (!existsSync(ancestor)) {
    const parent = dirname(ancestor);
    if (parent === ancestor) return absolute;
    ancestor = parent;
  }
  return resolve(realpathSync(ancestor), relative(ancestor, absolute));
}

function samePath(left: string, right: string): boolean {
  return physicalPath(left) === physicalPath(right);
}

function containsPath(directory: string, path: string): boolean {
  const position = relative(physicalPath(directory), physicalPath(path));
  return position === "" || (position !== ".." && !position.startsWith("../") && !position.startsWith("..\\") && !isAbsolute(position));
}

function validateExportDestination(romPath: string, catalogPath: string, directory: string): void {
  if (samePath(romPath, directory) || containsPath(directory, romPath)) {
    throw new Error("late runtime export directory must not contain its input ROM");
  }
  const destination = resolve(directory);
  if (!existsSync(destination)) return;
  if (!lstatSync(destination).isDirectory()) throw new Error("late runtime export destination must be a directory");
  const names = readdirSync(destination).sort();
  if (names.length === 0) return;
  if (names.length === 1 && names[0] === basename(catalogPath) && samePath(join(destination, names[0]), catalogPath)) {
    readLateRuntimeCatalog(catalogPath);
    return;
  }
  const index = join(destination, "index.json");
  if (!existsSync(index) || !lstatSync(index).isFile()) throw new Error("refusing to replace a directory that is not a late runtime package");
  try { buildLateRuntimePackage(index); }
  catch { throw new Error("refusing to replace a directory that is not a canonical late runtime package"); }
}

function replaceDirectory(directory: string, prefix: string, writer: (staged: string) => void): void {
  const destination = resolve(directory), parent = dirname(destination);
  if (destination === parent) throw new Error("late runtime operation requires a dedicated directory");
  mkdirSync(parent, { recursive: true });
  const transaction = mkdtempSync(join(parent, prefix));
  const staged = join(transaction, "new"), previous = join(transaction, "previous");
  let installed = false;
  try {
    writer(staged);
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

export function exportLateRuntimePackage(romPath: string, catalogPath: string, directory: string): void {
  regularFile(romPath, "late runtime input ROM");
  regularFile(catalogPath, "late runtime catalog");
  validateExportDestination(romPath, catalogPath, directory);
  const image = readFileSync(romPath);
  if (image.length !== ROM_SIZE) throw new Error("late runtime exporter requires an 8 MiB ROM");
  replaceDirectory(directory, ".late-runtime-export-", (staged) => { writePackage(image, catalogPath, staged, ROM_BASE); });
}

export function verifyLateRuntimePackageImage(
  image: Uint8Array,
  indexPath: string,
  imageBase = ROM_BASE,
  previous?: LateRuntimePackageBuild,
): LateRuntimePackageBuild {
  const built = previous ?? buildLateRuntimePackage(indexPath);
  for (const [start, data] of built.ranges) {
    const first = start - imageBase, end = first + data.length;
    if (first < 0 || end > image.length || !data.equals(Buffer.from(image.subarray(first, end)))) {
      let offset = 0;
      while (offset < data.length && first + offset < image.length && data[offset] === image[first + offset]) offset++;
      throw new Error(`late runtime package differs at ${hexadecimal(start + offset)}`);
    }
  }
  return built;
}

export function verifyLateRuntimePackage(romPath: string, indexPath: string): LateRuntimePackageBuild {
  regularFile(romPath, "late runtime input ROM");
  const image = readFileSync(romPath);
  if (image.length !== ROM_SIZE) throw new Error("late runtime verifier requires an 8 MiB ROM");
  return verifyLateRuntimePackageImage(image, indexPath);
}

function buildReport(built: LateRuntimePackageBuild): JsonObject {
  return {
    format: 1,
    kind: "golden-sun-late-runtime-build",
    source_bytes: built.sourceBytes,
    ranges: [...built.ranges].map(([address, data]) => ({
      address: hexadecimal(address), end: hexadecimal(address + data.length), size: data.length,
      output: `${address.toString(16).padStart(8, "0")}.bin`,
    })),
  };
}

function validateBuildDestination(directory: string): void {
  if (!existsSync(directory)) return;
  if (!lstatSync(directory).isDirectory()) throw new Error("late runtime build destination must be a directory");
  const names = readdirSync(directory);
  if (names.length === 0) return;
  const report = join(directory, "report.json");
  if (!existsSync(report)) throw new Error("refusing to replace a directory that is not a late runtime build");
  const value = canonicalDocument(report, "late runtime build report");
  exactKeys(value, ["format", "kind", "source_bytes", "ranges"], "late runtime build report");
  if (value.format !== 1 || value.kind !== "golden-sun-late-runtime-build") {
    throw new Error("refusing to replace a directory that is not a canonical late runtime build");
  }
}

export function writeLateRuntimeBuild(indexPath: string, directory: string): LateRuntimePackageBuild {
  const built = buildLateRuntimePackage(indexPath);
  validateBuildDestination(directory);
  replaceDirectory(directory, ".late-runtime-build-", (staged) => {
    mkdirSync(staged, { recursive: true });
    for (const [address, data] of built.ranges) writeFileSync(join(staged, `${address.toString(16).padStart(8, "0")}.bin`), data);
    writeFileSync(join(staged, "report.json"), pretty(buildReport(built)));
  });
  return built;
}

function zeroSources(catalog: LateRuntimeCatalog): Map<string, LateRuntimeSource> {
  const sources = new Map<string, LateRuntimeSource>();
  for (const range of catalog.ranges) for (const component of range.components) {
    if (component.kind === "unresolved_fill") continue;
    if (component.kind === "fill") sources.set(component.name, { kind: "fill" });
    else if (component.kind === "general_lz_stream") {
      const decoded = Buffer.alloc(component.decodedBytes!), tokens: GeneralToken[] = [["l", 1]];
      let remaining = decoded.length - 1;
      while (remaining > 0) { const count = Math.min(137, remaining); tokens.push(["c", count, 1]); remaining -= count; }
      const encoded = encode_general(decoded, tokens);
      sources.set(component.name, { kind: "general_lz_stream", decoded, tokens,
        lookahead: Buffer.alloc(component.end - component.address - encoded.length) });
    } else sources.set(component.name, { kind: component.kind, values: Array(component.count!).fill(0) });
  }
  return sources;
}

export function selfTest(): void {
  const temporary = mkdtempSync(join(tmpdir(), "alchemy-late-runtime-"));
  const catalogPath = resolve("assets/data/late_runtime/catalog.json");
  let rejected = 0;
  const reject = (action: () => void): void => { try { action(); } catch { rejected++; } };
  try {
    const catalog = readLateRuntimeCatalog(catalogPath), sources = zeroSources(catalog);
    for (const component of packageComponents(catalog)) {
      const source = sources.get(component.name);
      if (source?.kind === "general_lz_stream") source.lookahead.fill(255);
      else if (source && "values" in source) {
        if (source.kind === "s16_table") source.values = [-0x8000, -1, 0, 1, 1234, 0x7fff];
        else source.values = source.values.map((_, index) => source.kind === "u8_table" ? (index * 17 + component.address) & 0xff : (index * 257 + component.address) & 0xffff);
      }
    }
    const typed = buildLateRuntimeData(catalog, sources);
    const rangeBytes = catalog.ranges.reduce((sum, range) => sum + range.end - range.address, 0);
    const unresolvedBytes = typed.unresolved.reduce((sum, component) => sum + component.end - component.address, 0);
    if (rangeBytes !== 7_294 || typed.typedBytes !== 579 || unresolvedBytes !== 6_715 || typed.regions.size !== 13) {
      throw new Error("late runtime catalog totals differ");
    }
    const base = 0x080f0000, image = Buffer.alloc(0x9000);
    for (const [start, data] of typed.regions) data.copy(image, start - base);
    typed.unresolved.forEach((component, index) => image.fill(index & 1 ? 255 : 0, component.address - base, component.end - base));
    const packageDirectory = join(temporary, "package");
    const packaged = writePackage(image, catalogPath, packageDirectory, base);
    if (packaged.sourceBytes !== 7_294 || packaged.ranges.size !== 3 || packaged.components.size !== 18) {
      throw new Error("late runtime package extent differs");
    }
    verifyLateRuntimePackageImage(image, join(packageDirectory, "index.json"), base);
    const concatenated = Buffer.concat([...packaged.ranges.values()]);
    const expected = Buffer.concat(catalog.ranges.map((range) => image.subarray(range.address - base, range.end - base)));
    if (concatenated.length !== 7_294 || !concatenated.equals(expected)) throw new Error("late runtime full-range round-trip differs");

    const buildDirectory = join(temporary, "build");
    writeLateRuntimeBuild(join(packageDirectory, "index.json"), buildDirectory);
    if (!existsSync(join(buildDirectory, "report.json")) || readdirSync(buildDirectory).length !== 4) {
      throw new Error("late runtime build output differs");
    }

    const rom = Buffer.alloc(ROM_SIZE);
    image.copy(rom, base - ROM_BASE);
    const romPath = join(temporary, "gs1-en.gba");
    writeFileSync(romPath, rom);
    const exported = join(temporary, "exported");
    mkdirSync(exported);
    writeFileSync(join(exported, "catalog.json"), readFileSync(catalogPath));
    exportLateRuntimePackage(romPath, join(exported, "catalog.json"), exported);
    verifyLateRuntimePackage(romPath, join(exported, "index.json"));

    const malformedCatalog = canonicalDocument(catalogPath, "test catalog") as any;
    malformedCatalog.ranges[0].components[1].address = "0x080f39ac";
    reject(() => parseLateRuntimeCatalog(malformedCatalog));

    const firstFill = typed.unresolved[0];
    const wideFill = typed.unresolved.find((component) => component.end - component.address > 1)!;
    const nonuniform = Buffer.from(image);
    nonuniform[wideFill.address - base + 1] = 1;
    reject(() => writePackage(nonuniform, catalogPath, join(temporary, "nonuniform"), base));

    const unsupported = Buffer.from(image);
    unsupported.fill(0x7f, firstFill.address - base, firstFill.end - base);
    reject(() => writePackage(unsupported, catalogPath, join(temporary, "unsupported"), base));

    const streamTail = Buffer.from(image), streamComponent = singleStream(catalog);
    streamTail[streamComponent.end - base - 1] = 0;
    reject(() => writePackage(streamTail, catalogPath, join(temporary, "stream-tail"), base));

    const tablesPath = join(packageDirectory, PACKAGE_FILES.tables);
    const malformedTables = canonicalDocument(tablesPath, "test tables") as any;
    malformedTables.tables[0].values[0] = 256;
    writeFileSync(tablesPath, pretty(malformedTables));
    reject(() => buildLateRuntimePackage(join(packageDirectory, "index.json")));
    writePackage(image, catalogPath, packageDirectory, base);

    const fillsPath = join(packageDirectory, PACKAGE_FILES.fills);
    const malformedFills = canonicalDocument(fillsPath, "test fills") as any;
    malformedFills.spans[0].value = 1;
    writeFileSync(fillsPath, pretty(malformedFills));
    reject(() => buildLateRuntimePackage(join(packageDirectory, "index.json")));
    writePackage(image, catalogPath, packageDirectory, base);

    const indexPath = join(packageDirectory, "index.json");
    const escapedIndex = canonicalDocument(indexPath, "test index") as any;
    escapedIndex.sources.fills = "../fills.json";
    writeFileSync(indexPath, pretty(escapedIndex));
    reject(() => buildLateRuntimePackage(indexPath));
    writePackage(image, catalogPath, packageDirectory, base);

    const outside = join(temporary, "outside-fills.json"), linkedFills = join(packageDirectory, PACKAGE_FILES.fills);
    writeFileSync(outside, readFileSync(linkedFills));
    rmSync(linkedFills);
    symlinkSync(outside, linkedFills);
    reject(() => buildLateRuntimePackage(join(packageDirectory, "index.json")));
    rmSync(linkedFills);
    writePackage(image, catalogPath, packageDirectory, base);

    const changed = Buffer.from(image);
    changed[catalog.ranges[0].address - base] ^= 1;
    reject(() => verifyLateRuntimePackageImage(changed, join(packageDirectory, "index.json"), base));

    const rollback = join(temporary, "rollback");
    mkdirSync(rollback);
    writeFileSync(join(rollback, "stale"), "old");
    reject(() => replaceDirectory(rollback, ".late-runtime-test-", (staged) => {
      mkdirSync(staged); writeFileSync(join(staged, "partial"), "new"); throw new Error("synthetic failure");
    }));
    if (!existsSync(join(rollback, "stale")) || existsSync(join(rollback, "partial"))) {
      throw new Error("failed late runtime transaction changed its destination");
    }

    const unsafe = join(temporary, "unsafe");
    mkdirSync(unsafe); writeFileSync(join(unsafe, "foreign"), "x");
    reject(() => validateExportDestination(romPath, catalogPath, unsafe));
    reject(() => validateBuildDestination(unsafe));

    const inside = join(temporary, "inside");
    mkdirSync(inside);
    const physicalRom = join(inside, "physical.gba"), romLink = join(temporary, "rom-link.gba");
    writeFileSync(physicalRom, "rom"); symlinkSync(physicalRom, romLink);
    reject(() => validateExportDestination(romLink, catalogPath, inside));

    const duplicate = join(temporary, "duplicate.json");
    writeFileSync(duplicate, "{\"format\":1,\"format\":1}\n");
    reject(() => canonicalDocument(duplicate, "duplicate source"));

    if (rejected !== 14) throw new Error(`late runtime adversarial count differs: ${rejected}`);
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok source_bytes=7294 typed_json=579 structural_fill=6715 adversarial=14");
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { selfTest(); return; }
  if (args.length === 6 && args[0] === "export" && args[2] === "--catalog" && args[4] === "--directory") {
    exportLateRuntimePackage(args[1], args[3], args[5]);
    console.log("identical=true ranges=3 source_bytes=7294");
    return;
  }
  if (args.length === 4 && args[0] === "build" && args[2] === "--directory") {
    const built = writeLateRuntimeBuild(args[1], args[3]);
    console.log(`ranges=${built.ranges.size} source_bytes=${built.sourceBytes}`);
    return;
  }
  if (args.length === 4 && args[0] === "verify" && args[2] === "--index") {
    const built = verifyLateRuntimePackage(args[1], args[3]);
    console.log(`identical=true ranges=${built.ranges.size} source_bytes=${built.sourceBytes}`);
    return;
  }
  throw new Error("usage: late_runtime_data.ts export ROM --catalog CATALOG --directory DIR | build INDEX --directory DIR | verify ROM --index INDEX | --self-test");
}

if (import.meta.main) main(Bun.argv.slice(2));
