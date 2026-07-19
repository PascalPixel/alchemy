#!/usr/bin/env bun
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";

import { tile_png } from "./export_asset.ts";
import { gba_graphics } from "./import_asset.ts";

export const ROM_BASE = 0x08000000;
export const BATTLE_DATA_ADDRESS = 0x0809c410;
export const BATTLE_DATA_END = 0x080a1000;

const DIRECT_GRAPHICS_END = 0x0809c610;
const WEIGHTED_RECORDS_END = 0x0809d170;
const RULE_TABLES_END = 0x0809e4ce;

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
  records: number[][];
}

export interface BattleEffectSource {
  format: 1;
  kind: "golden-sun-battle-effect-data";
  address: string;
  end: string;
  direct_graphics: DirectGraphicSource[];
  weighted_records: WeightedRecordSource;
  typed_tables: TypedTableSource[];
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
  exactKeys(source, ["format", "kind", "address", "end", "direct_graphics", "weighted_records", "typed_tables"], "battle-effect source");
  if (source.format !== 1 || source.kind !== "golden-sun-battle-effect-data" ||
      source.address !== hex(BATTLE_DATA_ADDRESS) || source.end !== hex(RULE_TABLES_END))
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
  return source;
}

function graphicPath(root: string, source: string): string {
  if (!/^graphics\/battle_effect_data\/[a-z0-9_.-]+\.png$/.test(source))
    throw new Error("battle-effect graphic path differs");
  return join(root, source);
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

type FieldKind = "s16" | "u16" | "u32";

function fieldKind(field: string): FieldKind {
  const match = /^[a-z][a-z0-9_]*:(s16|u16|u32)$/.exec(field);
  if (!match) throw new Error("typed-table field differs");
  return match[1] as FieldKind;
}

function fieldSize(kind: FieldKind): number {
  return kind === "u32" ? 4 : 2;
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
        if (kind === "s16") output.writeInt16LE(integer(value, -0x8000, 0x7fff, "signed table value"), offset);
        else if (kind === "u16") output.writeUInt16LE(integer(value, 0, 0xffff, "table value"), offset);
        else output.writeUInt32LE(integer(value, 0, 0xffffffff, "table value"), offset);
        offset += fieldSize(kind);
      });
    });
    return output;
  }));
}

export function build_battle_effect_prefix(value: unknown, root: string): Buffer {
  const source = parse(value);
  const output = Buffer.concat([buildDirectGraphics(source, root), buildWeightedRecords(source), buildTypedTables(source)]);
  if (output.length !== RULE_TABLES_END - BATTLE_DATA_ADDRESS)
    throw new Error("battle-effect prefix extent differs");
  return output;
}

export function export_battle_effect_prefix(rom: Uint8Array, root: string): BattleEffectSource {
  const graphicsRoot = join(root, "graphics/battle_effect_data");
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
      source: `graphics/battle_effect_data/${name}`, bpp: 4, columns: 4, role: "obj_tiles",
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
    format: 1,
    kind: "golden-sun-battle-effect-data",
    address: hex(BATTLE_DATA_ADDRESS),
    end: hex(RULE_TABLES_END),
    direct_graphics: direct,
    weighted_records: {
      address: hex(DIRECT_GRAPHICS_END),
      end: hex(WEIGHTED_RECORDS_END),
      stride: 28,
      fields: ["base_result:u16", "rank_origin:u16", "results:u16[8]", "weights:u8[8]"],
      consumers: ["Func_0808ae74", "Func_0808b05c"],
      records,
    },
    typed_tables: RULE_LAYOUT.map(([start, end, fields, consumers]) => {
      const raw = range(rom, start, end);
      const kinds = fields.map(fieldKind);
      const stride = kinds.reduce((sum, kind) => sum + fieldSize(kind), 0);
      const records: number[][] = [];
      for (let recordOffset = 0; recordOffset < raw.length; recordOffset += stride) {
        let offset = recordOffset;
        records.push(kinds.map((kind) => {
          const value = kind === "s16" ? raw.readInt16LE(offset) :
            kind === "u16" ? raw.readUInt16LE(offset) : raw.readUInt32LE(offset);
          offset += fieldSize(kind);
          return value;
        }));
      }
      return {
        address: hex(start), end: hex(end), stride, fields: [...fields], consumers: [...consumers], records,
      };
    }),
  };
  if (!build_battle_effect_prefix(source, root).equals(range(rom, BATTLE_DATA_ADDRESS, RULE_TABLES_END)))
    throw new Error("exported battle-effect prefix does not round-trip");
  return source;
}

function option(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : undefined;
}

function selfTest(): void {
  if (fieldKind("selector:s16") !== "s16" || fieldKind("value:u16") !== "u16" ||
      fieldKind("mask:u32") !== "u32" || fieldSize("s16") !== 2 || fieldSize("u32") !== 4)
    throw new Error("battle-effect field self-test failed");
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
  if (command === "export-prefix") {
    const romPath = args[1];
    const output = option(args, "--output");
    if (!romPath || !root || !output)
      throw new Error("usage: battle_effect_data.ts export-prefix ROM --root ASSETS --output SOURCE");
    const source = export_battle_effect_prefix(readFileSync(romPath), root);
    mkdirSync(dirname(output), { recursive: true });
    writeFileSync(output, `${JSON.stringify(source, null, 2)}\n`);
    console.log(`identical=true bytes=${RULE_TABLES_END - BATTLE_DATA_ADDRESS} records=104 tables=${RULE_LAYOUT.length}`);
    return;
  }
  if (command === "verify-prefix") {
    const romPath = args[1];
    const input = args[2];
    if (!romPath || !input || !root)
      throw new Error("usage: battle_effect_data.ts verify-prefix ROM SOURCE --root ASSETS");
    const built = build_battle_effect_prefix(JSON.parse(readFileSync(input, "utf8")), root);
    if (!built.equals(range(readFileSync(romPath), BATTLE_DATA_ADDRESS, RULE_TABLES_END)))
      throw new Error("battle-effect prefix differs from ROM");
    console.log(`identical=true bytes=${built.length} records=104`);
    return;
  }
  throw new Error("usage: battle_effect_data.ts {export-prefix ROM --root ASSETS --output SOURCE|verify-prefix ROM SOURCE --root ASSETS}");
}

if (import.meta.main) main(Bun.argv.slice(2));
