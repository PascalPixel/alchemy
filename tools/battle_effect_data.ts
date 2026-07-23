#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { canonicalJson } from "./canonical_json.ts";

import { tile_png } from "./export_asset.ts";
import { gba_graphics } from "./import_asset.ts";
import {
  decode_palette_trace,
  encode_palette,
  type PaletteGroup,
} from "./extract_resource.ts";

export const ROM_BASE = 0x08000000;
export const BATTLE_DATA_ADDRESS = 0x0809c410;
export const BATTLE_DATA_END = 0x080a1000;

const DIRECT_GRAPHICS_END = 0x0809c610;
const WEIGHTED_RECORDS_END = 0x0809d170;
const RULE_TABLES_END = 0x0809e4ce;
const HALFWORD_GRAPHIC_END = 0x0809e680;
const SINE_TABLE_ADDRESS = 0x0809ed84;
const SINE_TABLE_END = 0x0809ef84;
const GRADIENT_ADDRESS = 0x0809f840;
const GRADIENT_END = 0x0809fbc0;
const PALETTE_GRAPHICS_ADDRESS = 0x0809ff58;
const PALETTE_GRAPHICS_END = 0x080a0107;
const SPARSE_TABLE_ADDRESS = 0x080a0138;
const SPARSE_TABLE_END = 0x080a0288;

const RULE_LAYOUT = [
  [0x0809d170, 0x0809d7a8, ["selector_a:s16", "selector_b:s16", "condition:s16", "result:s16"], ["Func_0808ace0"]],
  [0x0809d7a8, 0x0809d8b0, ["selector_a:s16", "selector_b:s16", "condition:s16", "result:s16"], ["Func_0808adf0"]],
  [0x0809d8b0, 0x0809d9f0, ["value:u16", "phase:u16"], []],
  [0x0809d9f0, 0x0809ddd8, ["selector_a:s16", "selector_b:s16", "condition:s16", "result:s16"], ["Func_0808b090"]],
  [0x0809ddd8, 0x0809e1d8, ["selector_a:s16", "selector_b:s16", "result:u32"], ["Func_0808b158"]],
  [0x0809e1d8, 0x0809e270, ["selector_a:s16", "selector_b:s16", "value_a:u16", "value_b:u16"], ["Func_0808b1d8"]],
  [0x0809e270, 0x0809e488, ["value:u32"], ["Func_0808b25c"]],
  [0x0809e488, 0x0809e4ce, ["value:u16"], ["Func_0808b320"]],
] as const;

const TAIL_TABLE_LAYOUT = [
  [0x0809e680, 0x0809e686, ["phase:u8"], ["Func_0808d9a4"]],
  [0x0809e6b8, 0x0809e6c0, ["phase:u8"], ["Func_0808e0b0"]],
  [0x0809e8ac, 0x0809e8ce, ["intensity:u8"], ["Func_0808f52c"]],
  [0x0809e8ce, 0x0809e8ee, ["curve:u8"], ["Func_0808f52c"]],
  [0x0809e8ee, 0x0809e92e, ["order:u8"], ["Func_08090658"]],
  [0x0809e92e, 0x0809e96e, ["color:u16"], ["Func_08090a5c"]],
  [0x0809e96e, 0x0809e9ae, ["color:u16"], ["Func_08090a5c"]],
  [0x0809e9ae, 0x0809e9ee, ["color:u16"], ["Func_08090a5c"]],
  [0x0809e9f0, 0x0809ebfc, ["id:u16", "value:s16"], ["Func_08091560"]],
  [0x0809ed80, 0x0809ed84, ["phase:u16"], ["Func_08092980"]],
  [0x0809ef84, 0x0809f024,
    ["x:s16", "y:s16", "tile:u16", "attribute_a:u16", "attribute_b:u16"], ["Func_08094820"]],
  [0x0809f024, 0x0809f0b0, ["y_offset:s16", "tile_offset:u16"], ["Func_08094e7c"]],
  [0x0809f0f8, 0x0809f118, ["phase:u16"], ["Func_08097b54"]],
  [0x0809f118, 0x0809f11c, ["order:u8"], ["Func_08097c3c"]],
  [0x0809f128, 0x0809f12c, ["order:u8"], []],
  [0x0809f160, 0x0809f168, ["slot:s8"], ["Func_0809ad70"]],
  [0x0809f168, 0x0809f188, ["curve:u8"], ["Func_0809bcf8"]],
  [0x0809f188, 0x0809f1a8, ["offset:s16"], ["Func_0809bcf8"]],
  [0x0809f1a8, 0x0809f810,
    ["resource_id:s16", "group:s8", "variant:s8", "effect_id:u16", "reserved:u16"],
    ["Func_0808a8e4", "Func_0808ab48", "Func_0808ab74"]],
  [0x0809f810, 0x0809f840,
    ["word_0:u32", "script:ptr", "word_2:u32", "word_3:u32", "word_4:u32", "word_5:u32"],
    ["FunctionHead_0808b674"]],
  [0x0809fbc0, 0x0809fbd0, ["packed_config:u32"], []],
  [0x0809fc28, 0x0809fc2c, ["value:u8"], ["Func_08093304"]],
  [0x080a0108, 0x080a0128, ["palette_code:u32"], ["Func_08097384"]],
] as const;

const OPCODE_LAYOUT = [
  [1, "Func_0800d654"], [0, "Func_0800d674"], [3, "Func_0800d9f0"], [3, "Func_0800da18"],
  [3, "Func_0800da40"], [0, "Func_0800da78"], [0, "Func_0800daa0"], [2, "Func_0800dac0"],
  [3, "Func_0800dd70"], [3, "Func_0800df04"], [0, "Func_0800dcdc"], [0, "Func_0800daf0"],
  [2, "Func_0800d710"], [1, "Func_0800d760"], [1, "Func_0800d780"], [1, "Func_0800d7b4"],
  [0, "Func_0800d7e8"], [1, "Func_0800d7f8"], [1, "Func_0800d820"], [1, "Func_0800d850"],
  [1, "Func_0800d880"], [2, "Func_0800e9a0"], [2, "Func_0800e9dc"], [2, "Func_0800ea18"],
  [0, "FunctionHead_0800ebec"], [0, "Func_0800f7f4"], [0, "Func_0800f2f8"], [0, "Func_0800d8e8"],
  [1, "Func_0800d8c4"], [1, "Func_0800d8f4"], [1, "Func_0800d900"], [0, "Func_0800ca2c"],
  [0, "Func_0800ca44"], [0, "Func_0800ca58"], [1, "Func_0800d6a4"], [0, "Func_0800f7dc"],
  [0, "Func_0800d7e8"], [0, "Func_0800d7e8"], [0, "Func_0800d7e8"], [0, "Func_0800d7e8"],
] as const;

const SCRIPT_LAYOUT = [
  [0x0809e6c0, 0x0809e75c, [0x0809e6c0]],
  [0x0809e75c, 0x0809e87c, [0x0809e75c]],
  [0x0809e87c, 0x0809e8a0, [0x0809e87c]],
  [0x0809e8a0, 0x0809e8ac, [0x0809e8a0]],
  [0x0809ebfc, 0x0809ed80, [0x0809ebfc, 0x0809ec7c, 0x0809ecfc]],
  [0x0809f0b0, 0x0809f0b4, [0x0809f0b0]],
  [0x0809f0b4, 0x0809f0bc, [0x0809f0b4]],
  [0x0809f0bc, 0x0809f0f8, [0x0809f0bc, 0x0809f0d4]],
  [0x0809f11c, 0x0809f128, [0x0809f11c]],
  [0x0809f12c, 0x0809f13c, [0x0809f12c]],
  [0x0809f13c, 0x0809f148, [0x0809f13c]],
  [0x0809f148, 0x0809f154, [0x0809f148]],
  [0x0809f154, 0x0809f160, [0x0809f154]],
  [0x0809f80c, 0x0809f810, [0x0809f80c]],
  [0x0809fbcc, 0x0809fbec, [0x0809fbcc]],
  [0x0809fbec, 0x0809fc04, [0x0809fbec]],
  [0x0809fc04, 0x0809fc1c, [0x0809fc04]],
  [0x0809fc1c, 0x0809fc28, [0x0809fc1c]],
  [0x0809fc2c, 0x0809fd38, [0x0809fc2c]],
  [0x0809fd38, 0x0809fd44, [0x0809fd38]],
  [0x0809fd44, 0x0809fe00, [0x0809fd44]],
  [0x0809fe00, 0x0809fe04, [0x0809fe00]],
  [0x0809fe04, 0x0809fe10, [0x0809fe04]],
  [0x0809fe10, 0x0809fecc, [0x0809fe10]],
  [0x0809fecc, 0x0809ff18, [0x0809fecc]],
  [0x0809ff18, 0x0809ff2c, [0x0809ff18]],
  [0x0809ff2c, 0x0809ff40, [0x0809ff2c]],
  [0x0809ff40, 0x0809ff58, [0x0809ff40]],
  [0x080a0128, 0x080a012c, [0x080a0128]],
] as const;

const CALLBACK_SYMBOLS = new Map<number, string>([
  [0x0808f305, "Func_0808f304"],
  [0x08092a75, "Func_08092a74"],
  [0x08093965, "Func_08093964"],
  [0x0809397d, "Func_0809397c"],
  [0x08093a15, "Func_08093a14"],
  [0x08098c09, "Func_08098c08"],
]);

interface DirectGraphicSource {
  address: string;
  end: string;
  source: string;
  bpp: 4;
  columns: number;
  role: "obj_tiles";
}

interface WeightedRecord {
  base_result: number;
  rank_origin: number;
  results: number[];
  weights: number[];
}

interface WeightedRecordSource {
  address: string;
  end: string;
  stride: 28;
  fields: ["base_result:u16", "rank_origin:u16", "results:u16[8]", "weights:u8[8]"];
  consumers: string[];
  records: WeightedRecord[];
}

interface TypedTableSource {
  address: string;
  end: string;
  stride: number;
  fields: string[];
  consumers: string[];
  records: (number | string | null)[][];
}

type HalfwordToken = ["l", number] | ["c", number, number] | ["e"];

interface CompressedGraphicSource {
  address: string;
  end: string;
  decoded_bytes: number;
  source: string;
  bpp: 4;
  columns: number;
  codec: "halfword-lz" | "palette-lz";
  tokens: HalfwordToken[] | PaletteGroup[];
}

interface SentinelLookupSource {
  address: string;
  end: string;
  fields: ["key:s16", "value:s16"];
  consumers: string[];
  records: number[][];
  sentinel: number;
}

interface ScriptEntryPoint {
  name: string;
  address: string;
}

interface ScriptCommand {
  opcode: string;
  handler: string;
  arguments: (number | string)[];
}

interface ScriptSkipWord {
  skip_word: number;
}

interface ObjectScriptSource {
  address: string;
  end: string;
  entry_points: ScriptEntryPoint[];
  commands: (ScriptCommand | ScriptSkipWord)[];
}

interface GeneratedSineSource {
  address: string;
  end: string;
  count: 256;
  type: "s16";
  formula: "trunc(sin(index*pi/128)*0x1000)";
}

interface GradientRun {
  length: number;
  color: number;
  start_low: number;
  end_low: number;
  start_high: number;
  end_high: number;
}

interface GradientRecord {
  first_color: number;
  second_color: number;
  runs: GradientRun[];
}

interface GradientSource {
  address: string;
  end: string;
  stride: 28;
  consumers: ["Func_0808f52c"];
  records: GradientRecord[];
}

interface SymbolicPointerSource {
  address: string;
  end: string;
  pointers: string[];
}

interface SparseRecord {
  index: number;
  key: number;
  value: number;
}

interface SparseTableSource {
  address: string;
  end: string;
  count: 41;
  fields: ["key:s32", "value:s32"];
  consumers: ["Func_0809bcf8"];
  records: SparseRecord[];
  sentinel: [-1, 0];
}

interface ZeroFillSource {
  address: string;
  end: string;
}

export interface BattleEffectSource {
  format: 2;
  kind: "golden-sun-battle-effect-data";
  address: string;
  end: string;
  direct_graphics: DirectGraphicSource[];
  weighted_records: WeightedRecordSource;
  typed_tables: TypedTableSource[];
  halfword_graphic: CompressedGraphicSource;
  tail_tables: TypedTableSource[];
  sentinel_lookup: SentinelLookupSource;
  object_scripts: ObjectScriptSource[];
  sine_table: GeneratedSineSource;
  gradients: GradientSource;
  palette_graphics: CompressedGraphicSource[];
  symbolic_pointers: SymbolicPointerSource;
  sparse_table: SparseTableSource;
  zero_fills: ZeroFillSource[];
}

function hex(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function exactKeys(value: object, keys: string[], label: string): void {
  if (Object.keys(value).sort().join(",") !== [...keys].sort().join(","))
    throw new Error(`${label} has unknown fields`);
}

function range(source: Uint8Array, start: number, end: number): Buffer {
  const first = start - ROM_BASE;
  const last = end - ROM_BASE;
  if (first < 0 || last < first || last > source.length) throw new Error("ROM range differs");
  return Buffer.from(source.subarray(first, last));
}

function parse(value: unknown): BattleEffectSource {
  if (typeof value !== "object" || value === null) throw new Error("battle-effect source must be an object");
  const source = value as BattleEffectSource;
  exactKeys(source, [
    "format", "kind", "address", "end", "direct_graphics", "weighted_records", "typed_tables",
    "halfword_graphic", "tail_tables", "sentinel_lookup", "object_scripts", "sine_table", "gradients",
    "palette_graphics", "symbolic_pointers", "sparse_table", "zero_fills",
  ], "battle-effect source");
  if (source.format !== 2 || source.kind !== "golden-sun-battle-effect-data" ||
      source.address !== hex(BATTLE_DATA_ADDRESS) || source.end !== hex(BATTLE_DATA_END))
    throw new Error("battle-effect source layout differs");
  if (!Array.isArray(source.direct_graphics) || source.direct_graphics.length !== 2)
    throw new Error("direct-graphics collection differs");
  source.direct_graphics.forEach((item) =>
    exactKeys(item, ["address", "end", "source", "bpp", "columns", "role"], "direct graphic"));
  exactKeys(source.weighted_records,
    ["address", "end", "stride", "fields", "consumers", "records"], "weighted-record source");
  source.weighted_records.records.forEach((record) =>
    exactKeys(record, ["base_result", "rank_origin", "results", "weights"], "weighted record"));
  if (!Array.isArray(source.typed_tables) || source.typed_tables.length !== RULE_LAYOUT.length)
    throw new Error("typed-table collection differs");
  source.typed_tables.forEach((table) =>
    exactKeys(table, ["address", "end", "stride", "fields", "consumers", "records"], "typed table"));
  exactKeys(source.halfword_graphic,
    ["address", "end", "decoded_bytes", "source", "bpp", "columns", "codec", "tokens"], "halfword graphic");
  if (!Array.isArray(source.tail_tables) || source.tail_tables.length !== TAIL_TABLE_LAYOUT.length)
    throw new Error("tail-table collection differs");
  source.tail_tables.forEach((table) =>
    exactKeys(table, ["address", "end", "stride", "fields", "consumers", "records"], "tail table"));
  exactKeys(source.sentinel_lookup,
    ["address", "end", "fields", "consumers", "records", "sentinel"], "sentinel lookup");
  if (!Array.isArray(source.object_scripts) || source.object_scripts.length !== SCRIPT_LAYOUT.length)
    throw new Error("object-script collection differs");
  source.object_scripts.forEach((script) => {
    exactKeys(script, ["address", "end", "entry_points", "commands"], "object script");
    script.entry_points.forEach((entry) => exactKeys(entry, ["name", "address"], "script entry point"));
    script.commands.forEach((command) => {
      if ("skip_word" in command) exactKeys(command, ["skip_word"], "script skip word");
      else exactKeys(command, ["opcode", "handler", "arguments"], "script command");
    });
  });
  exactKeys(source.sine_table, ["address", "end", "count", "type", "formula"], "sine table");
  exactKeys(source.gradients, ["address", "end", "stride", "consumers", "records"], "gradient source");
  source.gradients.records.forEach((record) => {
    exactKeys(record, ["first_color", "second_color", "runs"], "gradient record");
    record.runs.forEach((run) => exactKeys(run,
      ["length", "color", "start_low", "end_low", "start_high", "end_high"], "gradient run"));
  });
  if (!Array.isArray(source.palette_graphics) || source.palette_graphics.length !== 3)
    throw new Error("palette-graphic collection differs");
  source.palette_graphics.forEach((graphic) => exactKeys(graphic,
    ["address", "end", "decoded_bytes", "source", "bpp", "columns", "codec", "tokens"], "palette graphic"));
  exactKeys(source.symbolic_pointers, ["address", "end", "pointers"], "symbolic pointers");
  exactKeys(source.sparse_table,
    ["address", "end", "count", "fields", "consumers", "records", "sentinel"], "sparse table");
  source.sparse_table.records.forEach((record) => exactKeys(record, ["index", "key", "value"], "sparse record"));
  if (!Array.isArray(source.zero_fills) || source.zero_fills.length !== 3)
    throw new Error("zero-fill collection differs");
  source.zero_fills.forEach((fill) => exactKeys(fill, ["address", "end"], "zero fill"));
  return source;
}

function graphicPath(root: string, source: string): string {
  if (!/^graphics\/battle\/effects\/[a-z0-9_.-]+\.png$/.test(source))
    throw new Error("battle-effect graphic path differs");
  // Flat layout: collapse the package segments into the file name.
  return join(root, source.replace(/^graphics\/battle\/effects\//, "graphics/battle_effects_"));
}

function buildDirectGraphics(source: BattleEffectSource, root: string): Buffer {
  const expected = [
    [BATTLE_DATA_ADDRESS, BATTLE_DATA_ADDRESS + 0x100],
    [BATTLE_DATA_ADDRESS + 0x100, DIRECT_GRAPHICS_END],
  ] as const;
  const output: Buffer[] = [];
  source.direct_graphics.forEach((item, index) => {
    const [start, end] = expected[index];
    if (item.address !== hex(start) || item.end !== hex(end) || item.bpp !== 4 ||
        item.columns !== 4 || item.role !== "obj_tiles")
      throw new Error("direct-graphic layout differs");
    const [tiles, , report] = gba_graphics(readFileSync(graphicPath(root, item.source)), 4);
    if (report.width !== 32 || report.height !== 16 || tiles.length !== end - start)
      throw new Error("direct-graphic dimensions differ");
    output.push(tiles);
  });
  return Buffer.concat(output);
}

function integer(value: unknown, minimum: number, maximum: number, label: string): number {
  if (!Number.isInteger(value) || Number(value) < minimum || Number(value) > maximum)
    throw new Error(`${label} is out of range`);
  return Number(value);
}

function buildWeightedRecords(source: BattleEffectSource): Buffer {
  const table = source.weighted_records;
  if (table.address !== hex(DIRECT_GRAPHICS_END) || table.end !== hex(WEIGHTED_RECORDS_END) ||
      table.stride !== 28 || JSON.stringify(table.fields) !== JSON.stringify([
        "base_result:u16", "rank_origin:u16", "results:u16[8]", "weights:u8[8]",
      ]) || JSON.stringify(table.consumers) !== JSON.stringify(["Func_0808ae74", "Func_0808b05c"]) ||
      !Array.isArray(table.records) || table.records.length !== 104)
    throw new Error("weighted-record layout differs");
  const output = Buffer.alloc(table.records.length * table.stride);
  table.records.forEach((record, index) => {
    const offset = index * table.stride;
    output.writeUInt16LE(integer(record.base_result, 0, 0xffff, "base result"), offset);
    output.writeUInt16LE(integer(record.rank_origin, 0, 0xffff, "rank origin"), offset + 2);
    if (!Array.isArray(record.results) || record.results.length !== 8 ||
        !Array.isArray(record.weights) || record.weights.length !== 8)
      throw new Error("weighted-record array extent differs");
    record.results.forEach((value, result) =>
      output.writeUInt16LE(integer(value, 0, 0xffff, "weighted result"), offset + 4 + result * 2));
    record.weights.forEach((value, weight) =>
      output.writeUInt8(integer(value, 0, 0xff, "result weight"), offset + 20 + weight));
  });
  return output;
}

type FieldKind = "s8" | "u8" | "s16" | "u16" | "s32" | "u32" | "ptr";

function fieldKind(field: string): FieldKind {
  const match = /^[a-z][a-z0-9_]*:(s8|u8|s16|u16|s32|u32|ptr)$/.exec(field);
  if (!match) throw new Error("typed-table field differs");
  return match[1] as FieldKind;
}

function fieldSize(kind: FieldKind): number {
  return kind.endsWith("8") ? 1 : kind.endsWith("16") ? 2 : 4;
}

function writeField(output: Buffer, offset: number, kind: FieldKind, value: unknown): void {
  if (kind === "s8") output.writeInt8(integer(value, -0x80, 0x7f, "signed byte table value"), offset);
  else if (kind === "u8") output.writeUInt8(integer(value, 0, 0xff, "byte table value"), offset);
  else if (kind === "s16") output.writeInt16LE(integer(value, -0x8000, 0x7fff, "signed table value"), offset);
  else if (kind === "u16") output.writeUInt16LE(integer(value, 0, 0xffff, "table value"), offset);
  else if (kind === "s32") output.writeInt32LE(integer(value, -0x80000000, 0x7fffffff, "signed word table value"), offset);
  else if (kind === "u32") output.writeUInt32LE(integer(value, 0, 0xffffffff, "word table value"), offset);
  else if (value === null) output.writeUInt32LE(0, offset);
  else if (typeof value === "string" && scriptSymbols().has(value)) output.writeUInt32LE(scriptSymbols().get(value)!, offset);
  else throw new Error("symbolic table pointer differs");
}

function readField(input: Buffer, offset: number, kind: FieldKind): number | string | null {
  if (kind === "s8") return input.readInt8(offset);
  if (kind === "u8") return input.readUInt8(offset);
  if (kind === "s16") return input.readInt16LE(offset);
  if (kind === "u16") return input.readUInt16LE(offset);
  if (kind === "s32") return input.readInt32LE(offset);
  if (kind === "u32") return input.readUInt32LE(offset);
  const address = input.readUInt32LE(offset);
  if (address === 0) return null;
  const symbol = scriptName(address);
  if (!scriptSymbols().has(symbol)) throw new Error("exported table pointer lacks an object-script symbol");
  return symbol;
}

function buildTypedTables(source: BattleEffectSource): Buffer {
  return Buffer.concat(source.typed_tables.map((table, tableIndex) => {
    const [start, end, fields, consumers] = RULE_LAYOUT[tableIndex];
    if (table.address !== hex(start) || table.end !== hex(end) ||
        JSON.stringify(table.fields) !== JSON.stringify(fields) ||
        JSON.stringify(table.consumers) !== JSON.stringify(consumers))
      throw new Error("typed-table layout differs");
    const kinds = table.fields.map(fieldKind);
    const stride = kinds.reduce((sum, kind) => sum + fieldSize(kind), 0);
    if (table.stride !== stride || !Array.isArray(table.records) || table.records.length * stride !== end - start)
      throw new Error("typed-table extent differs");
    const output = Buffer.alloc(end - start);
    table.records.forEach((record, recordIndex) => {
      if (!Array.isArray(record) || record.length !== kinds.length) throw new Error("typed-table record extent differs");
      let offset = recordIndex * stride;
      record.forEach((value, fieldIndex) => {
        const kind = kinds[fieldIndex];
        writeField(output, offset, kind, value);
        offset += fieldSize(kind);
      });
    });
    return output;
  }));
}

function buildTailTables(source: BattleEffectSource): [number, number, Buffer][] {
  return source.tail_tables.map((table, tableIndex) => {
    const [start, end, fields, consumers] = TAIL_TABLE_LAYOUT[tableIndex];
    if (table.address !== hex(start) || table.end !== hex(end) ||
        JSON.stringify(table.fields) !== JSON.stringify(fields) ||
        JSON.stringify(table.consumers) !== JSON.stringify(consumers))
      throw new Error("tail-table layout differs");
    const kinds = table.fields.map(fieldKind);
    const stride = kinds.reduce((sum, kind) => sum + fieldSize(kind), 0);
    if (table.stride !== stride || !Array.isArray(table.records) || table.records.length * stride !== end - start)
      throw new Error("tail-table extent differs");
    const output = Buffer.alloc(end - start);
    table.records.forEach((record, recordIndex) => {
      if (!Array.isArray(record) || record.length !== kinds.length) throw new Error("tail-table record extent differs");
      let offset = recordIndex * stride;
      record.forEach((value, fieldIndex) => {
        writeField(output, offset, kinds[fieldIndex], value);
        offset += fieldSize(kinds[fieldIndex]);
      });
    });
    return [start, end, output];
  });
}

function appendHalfwordCopy(output: number[], distance: number, length: number): void {
  if (distance < 1 || distance > output.length || length < 2 || length > 33)
    throw new Error("halfword copy differs");
  for (let index = 0; index < length; index++) output.push(output[output.length - distance]);
}

function decodeHalfwordTrace(encoded: Uint8Array): [Buffer, HalfwordToken[]] {
  const input = Buffer.from(encoded);
  const output: number[] = [], tokens: HalfwordToken[] = [];
  let cursor = 0, ended = false;
  while (!ended) {
    if (cursor + 2 > input.length) throw new Error("halfword flags are truncated");
    const flags = input.readUInt16LE(cursor); cursor += 2;
    for (let bit = 15; bit >= 0; bit--) {
      if (cursor + 2 > input.length) throw new Error("halfword token is truncated");
      const token = input.readUInt16LE(cursor); cursor += 2;
      if (!(flags & 1 << bit)) {
        output.push(token);
        const previous = tokens.at(-1);
        if (previous?.[0] === "l") previous[1]++;
        else tokens.push(["l", 1]);
      } else if (token === 0) {
        tokens.push(["e"]); ended = true; break;
      } else {
        const distance = token >>> 5, length = (token & 31) + 2;
        tokens.push(["c", length, distance]);
        appendHalfwordCopy(output, distance, length);
      }
    }
  }
  if (cursor !== input.length) throw new Error("halfword stream has trailing bytes");
  const decoded = Buffer.alloc(output.length * 2);
  output.forEach((value, index) => decoded.writeUInt16LE(value, index * 2));
  return [decoded, tokens];
}

function encodeHalfword(decoded: Uint8Array, tokens: HalfwordToken[]): Buffer {
  const input = Buffer.from(decoded);
  if (input.length % 2) throw new Error("halfword pixels have an odd size");
  const operations: (["l"] | ["c", number, number] | ["e"])[] = [];
  for (const token of tokens) {
    if (!Array.isArray(token) || token.length < 1) throw new Error("halfword plan token differs");
    if (token[0] === "l") {
      if (token.length !== 2) throw new Error("halfword literal token differs");
      const count = integer(token[1], 1, input.length / 2, "halfword literal count");
      for (let index = 0; index < count; index++) operations.push(["l"]);
    } else if (token[0] === "c") {
      if (token.length !== 3) throw new Error("halfword copy token differs");
      operations.push(["c", integer(token[1], 2, 33, "halfword copy length"),
        integer(token[2], 1, 0x7ff, "halfword copy distance")]);
    } else if (token[0] === "e" && token.length === 1) operations.push(["e"]);
    else throw new Error("halfword plan token differs");
  }
  const encoded: Buffer[] = [], replay: number[] = [];
  let operation = 0, cursor = 0, ended = false;
  while (operation < operations.length) {
    let flags = 0;
    const words: number[] = [];
    for (let index = 0; index < 16 && operation < operations.length; index++, operation++) {
      const token = operations[operation];
      if (ended) throw new Error("halfword plan has data after terminator");
      if (token[0] === "l") {
        if (cursor + 2 > input.length) throw new Error("halfword literal crossed decoded pixels");
        const value = input.readUInt16LE(cursor); cursor += 2; replay.push(value); words.push(value);
      } else if (token[0] === "c") {
        flags |= 1 << (15 - index);
        const [, length, distance] = token;
        if (distance > replay.length) throw new Error("halfword copy crossed replay prefix");
        words.push(distance << 5 | length - 2);
        appendHalfwordCopy(replay, distance, length); cursor += length * 2;
      } else {
        flags |= 1 << (15 - index); words.push(0); ended = true;
        if (operation + 1 !== operations.length) throw new Error("halfword terminator is not final");
      }
    }
    const group = Buffer.alloc(2 + words.length * 2);
    group.writeUInt16LE(flags, 0);
    words.forEach((value, index) => group.writeUInt16LE(value, 2 + index * 2));
    encoded.push(group);
  }
  const replayBytes = Buffer.alloc(replay.length * 2);
  replay.forEach((value, index) => replayBytes.writeUInt16LE(value, index * 2));
  if (!ended || cursor !== input.length || !replayBytes.equals(input))
    throw new Error("halfword plan does not reconstruct decoded pixels");
  return Buffer.concat(encoded);
}

function graphicTiles(source: CompressedGraphicSource, root: string): Buffer {
  if (source.bpp !== 4) throw new Error("compressed graphic depth differs");
  const [tiles, , report] = gba_graphics(readFileSync(graphicPath(root, source.source)), 4);
  if (report.width !== source.columns * 8 || tiles.length !== source.decoded_bytes)
    throw new Error("compressed graphic dimensions differ");
  return tiles;
}

function buildHalfwordGraphic(source: BattleEffectSource, root: string): Buffer {
  const graphic = source.halfword_graphic;
  if (graphic.address !== hex(RULE_TABLES_END) || graphic.end !== hex(HALFWORD_GRAPHIC_END) ||
      graphic.decoded_bytes !== 0x600 || graphic.columns !== 12 || graphic.codec !== "halfword-lz")
    throw new Error("halfword graphic layout differs");
  const encoded = encodeHalfword(graphicTiles(graphic, root), graphic.tokens as HalfwordToken[]);
  if (encoded.length !== HALFWORD_GRAPHIC_END - RULE_TABLES_END)
    throw new Error("halfword graphic encoded size differs");
  return encoded;
}

function buildSentinelLookup(source: BattleEffectSource): Buffer {
  const table = source.sentinel_lookup;
  if (table.address !== hex(0x0809e686) || table.end !== hex(0x0809e6b8) ||
      JSON.stringify(table.fields) !== JSON.stringify(["key:s16", "value:s16"]) ||
      JSON.stringify(table.consumers) !== JSON.stringify(["Func_0808ddb8"]) ||
      !Array.isArray(table.records) || table.records.length !== 12 || table.sentinel !== -1)
    throw new Error("sentinel lookup layout differs");
  const output = Buffer.alloc(50);
  table.records.forEach((record, index) => {
    if (!Array.isArray(record) || record.length !== 2) throw new Error("sentinel lookup record differs");
    output.writeInt16LE(integer(record[0], -0x8000, 0x7fff, "lookup key"), index * 4);
    output.writeInt16LE(integer(record[1], -0x8000, 0x7fff, "lookup value"), index * 4 + 2);
  });
  output.writeInt16LE(-1, 48);
  return output;
}

function scriptName(address: number): string {
  return `ObjectScript_${address.toString(16).padStart(8, "0")}`;
}

function scriptSymbols(): Map<string, number> {
  const symbols = new Map<string, number>();
  for (const [, , entries] of SCRIPT_LAYOUT) for (const address of entries) symbols.set(scriptName(address), address);
  for (const [address, name] of CALLBACK_SYMBOLS) symbols.set(name, address);
  return symbols;
}

function scriptWord(value: unknown, symbols: Map<string, number>): number {
  if (typeof value === "string") {
    const address = symbols.get(value);
    if (address === undefined) throw new Error(`unknown script symbol: ${value}`);
    return address;
  }
  return integer(value, -0x80000000, 0xffffffff, "script word") >>> 0;
}

function opcodeNumber(name: string): number {
  const match = /^op_([0-9]+)$/.exec(name);
  if (!match) throw new Error("object-script opcode differs");
  return integer(Number(match[1]), 0, OPCODE_LAYOUT.length - 1, "object-script opcode");
}

function buildObjectScripts(source: BattleEffectSource): [number, number, Buffer][] {
  const symbols = scriptSymbols();
  return source.object_scripts.map((script, index) => {
    const [start, end, entries] = SCRIPT_LAYOUT[index];
    if (script.address !== hex(start) || script.end !== hex(end) || script.entry_points.length !== entries.length)
      throw new Error("object-script layout differs");
    const chunks: Buffer[] = [];
    const boundaries = new Set<number>();
    let size = 0;
    for (const command of script.commands) {
      boundaries.add(start + size);
      if ("skip_word" in command) {
        const word = scriptWord(command.skip_word, symbols);
        if (word <= 63) throw new Error("object-script skip word is dispatchable");
        const output = Buffer.alloc(4); output.writeUInt32LE(word); chunks.push(output); size += 4; continue;
      }
      const opcode = opcodeNumber(command.opcode);
      const [arity, handler] = OPCODE_LAYOUT[opcode];
      if (command.handler !== handler || !Array.isArray(command.arguments) || command.arguments.length !== arity)
        throw new Error("object-script command layout differs");
      const output = Buffer.alloc((1 + arity) * 4); output.writeUInt32LE(opcode);
      command.arguments.forEach((argument, argumentIndex) =>
        output.writeUInt32LE(scriptWord(argument, symbols), 4 + argumentIndex * 4));
      chunks.push(output); size += output.length;
    }
    if (start + size !== end) throw new Error("object-script extent differs");
    script.entry_points.forEach((entry, entryIndex) => {
      const expected = entries[entryIndex];
      if (entry.name !== scriptName(expected) || entry.address !== hex(expected) || !boundaries.has(expected))
        throw new Error("object-script entry point differs");
    });
    return [start, end, Buffer.concat(chunks)];
  });
}

function buildSineTable(source: BattleEffectSource): Buffer {
  const table = source.sine_table;
  if (table.address !== hex(SINE_TABLE_ADDRESS) || table.end !== hex(SINE_TABLE_END) || table.count !== 256 ||
      table.type !== "s16" || table.formula !== "trunc(sin(index*pi/128)*0x1000)")
    throw new Error("sine-table formula differs");
  const output = Buffer.alloc(512);
  for (let index = 0; index < 256; index++)
    output.writeInt16LE(Math.trunc(Math.sin(index * Math.PI / 128) * 0x1000), index * 2);
  return output;
}

function buildGradients(source: BattleEffectSource): Buffer {
  const table = source.gradients;
  if (table.address !== hex(GRADIENT_ADDRESS) || table.end !== hex(GRADIENT_END) || table.stride !== 28 ||
      JSON.stringify(table.consumers) !== JSON.stringify(["Func_0808f52c"]) || table.records.length !== 32)
    throw new Error("gradient layout differs");
  const output = Buffer.alloc(GRADIENT_END - GRADIENT_ADDRESS);
  table.records.forEach((record, recordIndex) => {
    if (record.runs.length !== 3) throw new Error("gradient run count differs");
    const offset = recordIndex * 28;
    output.writeUInt16LE(integer(record.first_color, 0, 0xffff, "gradient first color"), offset);
    output.writeUInt16LE(integer(record.second_color, 0, 0xffff, "gradient second color"), offset + 2);
    record.runs.forEach((run, runIndex) => {
      const runOffset = offset + 4 + runIndex * 8;
      output.writeUInt16LE(integer(run.length, 0, 0xffff, "gradient run length"), runOffset);
      output.writeUInt16LE(integer(run.color, 0, 0xffff, "gradient color"), runOffset + 2);
      output.writeUInt8(integer(run.start_low, 0, 0xff, "gradient low start"), runOffset + 4);
      output.writeUInt8(integer(run.end_low, 0, 0xff, "gradient low end"), runOffset + 5);
      output.writeUInt8(integer(run.start_high, 0, 0xff, "gradient high start"), runOffset + 6);
      output.writeUInt8(integer(run.end_high, 0, 0xff, "gradient high end"), runOffset + 7);
    });
  });
  return output;
}

function buildPaletteGraphics(source: BattleEffectSource, root: string): [number, number, Buffer][] {
  const expected = [
    [0x0809ff58, 0x080a001e, 0x300],
    [0x080a001e, 0x080a00b8, 0x300],
    [0x080a00b8, 0x080a0107, 0x200],
  ] as const;
  return source.palette_graphics.map((graphic, index) => {
    const [start, end, decodedBytes] = expected[index];
    if (graphic.address !== hex(start) || graphic.end !== hex(end) || graphic.decoded_bytes !== decodedBytes ||
        graphic.columns !== 2 || graphic.codec !== "palette-lz")
      throw new Error("palette graphic layout differs");
    const encoded = encode_palette(graphicTiles(graphic, root), graphic.tokens as PaletteGroup[]);
    if (encoded.length !== end - start) throw new Error("palette graphic encoded size differs");
    return [start, end, encoded];
  });
}

function buildSymbolicPointers(source: BattleEffectSource): Buffer {
  const table = source.symbolic_pointers;
  const expected = [scriptName(0x0809f13c), scriptName(0x0809f148), scriptName(0x0809f154)];
  if (table.address !== hex(0x080a012c) || table.end !== hex(0x080a0138) ||
      JSON.stringify(table.pointers) !== JSON.stringify(expected))
    throw new Error("symbolic pointer table differs");
  const symbols = scriptSymbols(), output = Buffer.alloc(12);
  table.pointers.forEach((name, index) => output.writeUInt32LE(symbols.get(name) ?? 0, index * 4));
  return output;
}

function buildSparseTable(source: BattleEffectSource): Buffer {
  const table = source.sparse_table;
  if (table.address !== hex(SPARSE_TABLE_ADDRESS) || table.end !== hex(SPARSE_TABLE_END) || table.count !== 41 ||
      JSON.stringify(table.fields) !== JSON.stringify(["key:s32", "value:s32"]) ||
      JSON.stringify(table.consumers) !== JSON.stringify(["Func_0809bcf8"]) ||
      JSON.stringify(table.sentinel) !== JSON.stringify([-1, 0]))
    throw new Error("sparse-table layout differs");
  const output = Buffer.alloc(SPARSE_TABLE_END - SPARSE_TABLE_ADDRESS);
  const indices = new Set<number>();
  table.records.forEach((record) => {
    const index = integer(record.index, 0, 40, "sparse-table index");
    if (indices.has(index)) throw new Error("sparse-table index is duplicated");
    indices.add(index);
    output.writeInt32LE(integer(record.key, -0x80000000, 0x7fffffff, "sparse-table key"), index * 8);
    output.writeInt32LE(integer(record.value, -0x80000000, 0x7fffffff, "sparse-table value"), index * 8 + 4);
  });
  output.writeInt32LE(-1, 41 * 8);
  return output;
}

function buildZeroFills(source: BattleEffectSource): [number, number, Buffer][] {
  const expected = [
    [0x0809e9ee, 0x0809e9f0], [0x080a0107, 0x080a0108], [0x080a0288, BATTLE_DATA_END],
  ] as const;
  return source.zero_fills.map((fill, index) => {
    const [start, end] = expected[index];
    if (fill.address !== hex(start) || fill.end !== hex(end)) throw new Error("zero-fill layout differs");
    return [start, end, Buffer.alloc(end - start)];
  });
}

function assembleTail(segments: [number, number, Buffer][]): Buffer {
  const output = Buffer.alloc(BATTLE_DATA_END - RULE_TABLES_END);
  const written = new Uint8Array(output.length);
  for (const [start, end, bytes] of segments) {
    if (start < RULE_TABLES_END || end > BATTLE_DATA_END || start >= end || bytes.length !== end - start)
      throw new Error("battle-effect tail segment differs");
    const offset = start - RULE_TABLES_END;
    for (let index = 0; index < bytes.length; index++) {
      if (written[offset + index] && output[offset + index] !== bytes[index])
        throw new Error(`battle-effect overlap differs at ${hex(start + index)}`);
      output[offset + index] = bytes[index]; written[offset + index] = 1;
    }
  }
  const hole = written.indexOf(0);
  if (hole >= 0) throw new Error(`battle-effect tail has a hole at ${hex(RULE_TABLES_END + hole)}`);
  return output;
}

type TableLayout = readonly (readonly [number, number, readonly string[], readonly string[]])[];

function exportTableCollection(rom: Uint8Array, layout: TableLayout): TypedTableSource[] {
  return layout.map(([start, end, fields, consumers]) => {
    const raw = range(rom, start, end);
    const kinds = fields.map(fieldKind);
    const stride = kinds.reduce((sum, kind) => sum + fieldSize(kind), 0);
    if (raw.length % stride) throw new Error("exported typed-table extent differs");
    const records: (number | string | null)[][] = [];
    for (let recordOffset = 0; recordOffset < raw.length; recordOffset += stride) {
      let offset = recordOffset;
      records.push(kinds.map((kind) => {
        const value = readField(raw, offset, kind); offset += fieldSize(kind); return value;
      }));
    }
    return { address: hex(start), end: hex(end), stride, fields: [...fields], consumers: [...consumers], records };
  });
}

function exportHalfwordGraphic(rom: Uint8Array, root: string): CompressedGraphicSource {
  const encoded = range(rom, RULE_TABLES_END, HALFWORD_GRAPHIC_END);
  const [decoded, tokens] = decodeHalfwordTrace(encoded);
  if (decoded.length !== 0x600) throw new Error("halfword graphic decoded size differs");
  const name = "battle_glyphs.4bpp.png";
  writeFileSync(join(root, `graphics/battle/effects/${name}`), tile_png(decoded, 4, 12)[0]);
  return {
    address: hex(RULE_TABLES_END), end: hex(HALFWORD_GRAPHIC_END), decoded_bytes: decoded.length,
    source: `graphics/battle/effects/${name}`, bpp: 4, columns: 12, codec: "halfword-lz", tokens,
  };
}

function exportSentinelLookup(rom: Uint8Array): SentinelLookupSource {
  const raw = range(rom, 0x0809e686, 0x0809e6b8);
  const records = Array.from({ length: 12 }, (_, index) =>
    [raw.readInt16LE(index * 4), raw.readInt16LE(index * 4 + 2)]);
  return {
    address: hex(0x0809e686), end: hex(0x0809e6b8), fields: ["key:s16", "value:s16"],
    consumers: ["Func_0808ddb8"], records, sentinel: raw.readInt16LE(48),
  };
}

function sourceWord(value: number): number {
  return value >= 0x80000000 ? value - 0x100000000 : value;
}

function exportObjectScripts(rom: Uint8Array): ObjectScriptSource[] {
  return SCRIPT_LAYOUT.map(([start, end, entries]) => {
    const raw = range(rom, start, end);
    const commands: (ScriptCommand | ScriptSkipWord)[] = [];
    let cursor = 0;
    while (cursor < raw.length) {
      const opcode = raw.readUInt32LE(cursor); cursor += 4;
      if (opcode > 63) { commands.push({ skip_word: sourceWord(opcode) }); continue; }
      if (opcode >= OPCODE_LAYOUT.length) throw new Error("object-script opcode is outside dispatch table");
      const [arity, handler] = OPCODE_LAYOUT[opcode];
      if (cursor + arity * 4 > raw.length) throw new Error("object-script command is truncated");
      const arguments_: (number | string)[] = [];
      for (let index = 0; index < arity; index++) {
        const word = raw.readUInt32LE(cursor); cursor += 4;
        arguments_.push(opcode === 34 && CALLBACK_SYMBOLS.has(word) ? CALLBACK_SYMBOLS.get(word)! : sourceWord(word));
      }
      commands.push({ opcode: `op_${opcode}`, handler, arguments: arguments_ });
    }
    return {
      address: hex(start), end: hex(end),
      entry_points: entries.map((address) => ({ name: scriptName(address), address: hex(address) })), commands,
    };
  });
}

function exportGradients(rom: Uint8Array): GradientSource {
  const raw = range(rom, GRADIENT_ADDRESS, GRADIENT_END);
  const records: GradientRecord[] = [];
  for (let offset = 0; offset < raw.length; offset += 28) {
    const runs: GradientRun[] = [];
    for (let run = 0; run < 3; run++) {
      const cursor = offset + 4 + run * 8;
      runs.push({
        length: raw.readUInt16LE(cursor), color: raw.readUInt16LE(cursor + 2),
        start_low: raw[cursor + 4], end_low: raw[cursor + 5],
        start_high: raw[cursor + 6], end_high: raw[cursor + 7],
      });
    }
    records.push({ first_color: raw.readUInt16LE(offset), second_color: raw.readUInt16LE(offset + 2), runs });
  }
  return {
    address: hex(GRADIENT_ADDRESS), end: hex(GRADIENT_END), stride: 28,
    consumers: ["Func_0808f52c"], records,
  };
}

function exportPaletteGraphics(rom: Uint8Array, root: string): CompressedGraphicSource[] {
  const specs = [
    [0x0809ff58, 0x080a001e, 0x300, "particle_slash.4bpp.png"],
    [0x080a001e, 0x080a00b8, 0x300, "particle_spark.4bpp.png"],
    [0x080a00b8, 0x080a0107, 0x200, "particle_beam.4bpp.png"],
  ] as const;
  return specs.map(([start, end, decodedBytes, name]) => {
    const encoded = range(rom, start, end);
    const [decoded, cursor, tokens] = decode_palette_trace(encoded, 0, encoded.length, decodedBytes);
    if (cursor !== encoded.length || decoded.length !== decodedBytes)
      throw new Error("palette graphic stream extent differs");
    writeFileSync(join(root, `graphics/battle/effects/${name}`), tile_png(decoded, 4, 2)[0]);
    return {
      address: hex(start), end: hex(end), decoded_bytes: decodedBytes,
      source: `graphics/battle/effects/${name}`, bpp: 4 as const, columns: 2,
      codec: "palette-lz" as const, tokens,
    };
  });
}

function exportSparseTable(rom: Uint8Array): SparseTableSource {
  const raw = range(rom, SPARSE_TABLE_ADDRESS, SPARSE_TABLE_END);
  const records: SparseRecord[] = [];
  for (let index = 0; index < 41; index++) {
    const key = raw.readInt32LE(index * 8), value = raw.readInt32LE(index * 8 + 4);
    if (key !== 0 || value !== 0) records.push({ index, key, value });
  }
  return {
    address: hex(SPARSE_TABLE_ADDRESS), end: hex(SPARSE_TABLE_END), count: 41,
    fields: ["key:s32", "value:s32"], consumers: ["Func_0809bcf8"], records,
    sentinel: [raw.readInt32LE(41 * 8), raw.readInt32LE(41 * 8 + 4)],
  };
}

export function build_battle_effect_data(value: unknown, root: string): Buffer {
  const source = parse(value);
  const prefix = Buffer.concat([buildDirectGraphics(source, root), buildWeightedRecords(source), buildTypedTables(source)]);
  if (prefix.length !== RULE_TABLES_END - BATTLE_DATA_ADDRESS)
    throw new Error("battle-effect table prefix extent differs");
  const segments: [number, number, Buffer][] = [
    [RULE_TABLES_END, HALFWORD_GRAPHIC_END, buildHalfwordGraphic(source, root)],
    ...buildTailTables(source),
    [0x0809e686, 0x0809e6b8, buildSentinelLookup(source)],
    ...buildObjectScripts(source),
    [SINE_TABLE_ADDRESS, SINE_TABLE_END, buildSineTable(source)],
    [GRADIENT_ADDRESS, GRADIENT_END, buildGradients(source)],
    ...buildPaletteGraphics(source, root),
    [0x080a012c, 0x080a0138, buildSymbolicPointers(source)],
    [SPARSE_TABLE_ADDRESS, SPARSE_TABLE_END, buildSparseTable(source)],
    ...buildZeroFills(source),
  ];
  const output = Buffer.concat([prefix, assembleTail(segments)]);
  if (output.length !== BATTLE_DATA_END - BATTLE_DATA_ADDRESS)
    throw new Error("battle-effect source extent differs");
  return output;
}

export function export_battle_effect_data(rom: Uint8Array, root: string): BattleEffectSource {
  const graphicsRoot = join(root, "graphics/battle/effects");
  mkdirSync(graphicsRoot, { recursive: true });
  const direct: DirectGraphicSource[] = [];
  for (const [name, start, end] of [
    ["obj_tiles_0809c410.4bpp.png", BATTLE_DATA_ADDRESS, BATTLE_DATA_ADDRESS + 0x100],
    ["obj_tiles_0809c510.4bpp.png", BATTLE_DATA_ADDRESS + 0x100, DIRECT_GRAPHICS_END],
  ] as const) {
    const path = join(graphicsRoot, name);
    writeFileSync(path, tile_png(range(rom, start, end), 4, 4)[0]);
    direct.push({
      address: hex(start), end: hex(end),
      source: `graphics/battle/effects/${name}`, bpp: 4, columns: 4, role: "obj_tiles",
    });
  }
  const raw = range(rom, DIRECT_GRAPHICS_END, WEIGHTED_RECORDS_END);
  const records: WeightedRecord[] = [];
  for (let offset = 0; offset < raw.length; offset += 28) {
    records.push({
      base_result: raw.readUInt16LE(offset),
      rank_origin: raw.readUInt16LE(offset + 2),
      results: Array.from({ length: 8 }, (_, index) => raw.readUInt16LE(offset + 4 + index * 2)),
      weights: Array.from(raw.subarray(offset + 20, offset + 28)),
    });
  }
  const source: BattleEffectSource = {
    format: 2,
    kind: "golden-sun-battle-effect-data",
    address: hex(BATTLE_DATA_ADDRESS),
    end: hex(BATTLE_DATA_END),
    direct_graphics: direct,
    weighted_records: {
      address: hex(DIRECT_GRAPHICS_END),
      end: hex(WEIGHTED_RECORDS_END),
      stride: 28,
      fields: ["base_result:u16", "rank_origin:u16", "results:u16[8]", "weights:u8[8]"],
      consumers: ["Func_0808ae74", "Func_0808b05c"],
      records,
    },
    typed_tables: exportTableCollection(rom, RULE_LAYOUT),
    halfword_graphic: exportHalfwordGraphic(rom, root),
    tail_tables: exportTableCollection(rom, TAIL_TABLE_LAYOUT),
    sentinel_lookup: exportSentinelLookup(rom),
    object_scripts: exportObjectScripts(rom),
    sine_table: {
      address: hex(SINE_TABLE_ADDRESS), end: hex(SINE_TABLE_END), count: 256, type: "s16",
      formula: "trunc(sin(index*pi/128)*0x1000)",
    },
    gradients: exportGradients(rom),
    palette_graphics: exportPaletteGraphics(rom, root),
    symbolic_pointers: {
      address: hex(0x080a012c), end: hex(0x080a0138),
      pointers: [scriptName(0x0809f13c), scriptName(0x0809f148), scriptName(0x0809f154)],
    },
    sparse_table: exportSparseTable(rom),
    zero_fills: [
      { address: hex(0x0809e9ee), end: hex(0x0809e9f0) },
      { address: hex(0x080a0107), end: hex(0x080a0108) },
      { address: hex(0x080a0288), end: hex(BATTLE_DATA_END) },
    ],
  };
  if (!build_battle_effect_data(source, root).equals(range(rom, BATTLE_DATA_ADDRESS, BATTLE_DATA_END)))
    throw new Error("exported battle-effect source does not round-trip");
  return source;
}

function option(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : undefined;
}

function selfTest(): void {
  if (fieldKind("selector:s16") !== "s16" || fieldKind("value:u16") !== "u16" ||
      fieldKind("mask:u32") !== "u32" || fieldKind("slot:s8") !== "s8" ||
      fieldSize("s8") !== 1 || fieldSize("s16") !== 2 || fieldSize("u32") !== 4)
    throw new Error("battle-effect field self-test failed");
  const halfwords = Buffer.from([0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x78, 0x56]);
  const plan: HalfwordToken[] = [["l", 1], ["c", 2, 1], ["l", 1], ["e"]];
  const encoded = encodeHalfword(halfwords, plan);
  const [decoded, replay] = decodeHalfwordTrace(encoded);
  if (!decoded.equals(halfwords) || JSON.stringify(replay) !== JSON.stringify(plan))
    throw new Error("halfword codec self-test failed");
  let rejected = false;
  try { graphicPath("assets", "../private.png"); } catch { rejected = true; }
  if (!rejected) throw new Error("battle-effect path escape was accepted");
  console.log("self-test=ok");
}

function main(args: string[]): void {
  if (args.includes("--self-test")) {
    selfTest();
    return;
  }
  const command = args[0];
  const root = option(args, "--root");
  if (command === "export" || command === "export-prefix") {
    const romPath = args[1];
    const output = option(args, "--output");
    if (!romPath || !root || !output)
      throw new Error("usage: battle_effect_data.ts export ROM --root ASSETS --output SOURCE");
    const source = export_battle_effect_data(readFileSync(romPath), root);
    mkdirSync(dirname(output), { recursive: true });
    writeFileSync(output, `${canonicalJson(source)}\n`);
    console.log(`identical=true bytes=${BATTLE_DATA_END - BATTLE_DATA_ADDRESS} records=104 tables=${RULE_LAYOUT.length + TAIL_TABLE_LAYOUT.length}`);
    return;
  }
  if (command === "verify" || command === "verify-prefix") {
    const romPath = args[1];
    const input = args[2];
    if (!romPath || !input || !root)
      throw new Error("usage: battle_effect_data.ts verify ROM SOURCE --root ASSETS");
    const built = build_battle_effect_data(JSON.parse(readFileSync(input, "utf8")), root);
    if (!built.equals(range(readFileSync(romPath), BATTLE_DATA_ADDRESS, BATTLE_DATA_END)))
      throw new Error("battle-effect source differs from ROM");
    console.log(`identical=true bytes=${built.length} records=104`);
    return;
  }
  throw new Error("usage: battle_effect_data.ts {export ROM --root ASSETS --output SOURCE|verify ROM SOURCE --root ASSETS}");
}

if (import.meta.main) main(Bun.argv.slice(2));
