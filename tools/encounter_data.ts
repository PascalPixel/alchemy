#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { join } from "node:path";
import { canonicalJson } from "./canonical_json.ts";

const ROM_BASE = 0x08000000;
export const BRIGHTNESS_CURVE_ADDRESS = 0x080c5c10;
export const BRIGHTNESS_CURVE_SIZE = 0x20;
export const ENCOUNTER_TABLE_ADDRESS = 0x080c5c38;
export const ENCOUNTER_TABLE_SIZE = 0x1d48;
export const ALIGNMENT_ADDRESS = 0x080c7980;
export const ALIGNMENT_SIZE = 0x1680;
export const ALIGNMENT_END = 0x080c9000;
export const FORMATION_COUNT = 380;
export const PRELOAD_COUNT = 20;
export const METADATA_COUNT = 172;

type Json = Record<string, any>;

export interface EncounterRegion {
  address: number;
  size: number;
  source: string;
  data: Buffer;
}

function integer(value: unknown, minimum: number, maximum: number, name: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < minimum || value > maximum) {
    throw new Error(`invalid ${name}`);
  }
  return value;
}

function document(path: string): Json {
  const parsed = JSON.parse(readFileSync(path, "utf8"));
  if (typeof parsed !== "object" || parsed === null || Array.isArray(parsed))
    throw new Error(`${path}: source must be an object`);
  if (parsed.format !== 1) throw new Error(`${path}: unsupported source format`);
  return parsed;
}

function exactKeys(value: Json, keys: string[], name: string): void {
  const actual = Object.keys(value).sort();
  const expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index]))
    throw new Error(`${name} has unknown fields`);
}

function pretty(value: unknown): string {
  return `${canonicalJson(value)}\n`;
}

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function checkedExtent(source: Json, address: number, size: number, name: string): void {
  if (Number(source.address) !== address || Number(source.size) !== size) {
    throw new Error(`${name} extent differs`);
  }
}

function flagByte(source: Json, name: string): number {
  exactKeys(source, ["bit0", "bits_1_4", "bits_5_7"], name);
  const bit0 = source.bit0;
  if (typeof bit0 !== "boolean") throw new Error(`${name} bit0 is not boolean`);
  const middle = integer(source.bits_1_4, 0, 0x0f, `${name} bits 1..4`);
  const high = integer(source.bits_5_7, 0, 0x07, `${name} bits 5..7`);
  return Number(bit0) | middle << 1 | high << 5;
}

function splitFlags(value: number): Json {
  return {
    bit0: Boolean(value & 1),
    bits_1_4: value >> 1 & 0x0f,
    bits_5_7: value >> 5,
  };
}

export function build_brightness_curve(path: string): Buffer {
  const source = document(path);
  exactKeys(source, ["format", "address", "size", "phases"], "brightness curve");
  checkedExtent(source, BRIGHTNESS_CURVE_ADDRESS, BRIGHTNESS_CURVE_SIZE, "brightness curve");
  if (!Array.isArray(source.phases) || source.phases.length !== 2 ||
      source.phases.some((phase: unknown) => !Array.isArray(phase) || phase.length !== 16)) {
    throw new Error("brightness curve requires two sixteen-step phases");
  }
  const values = source.phases.flat().map((value: unknown) =>
    integer(value, -128, 127, "brightness coefficient"));
  return Buffer.from(values.map((value: number) => value & 0xff));
}

export function build_encounter_tables(path: string): Buffer {
  const source = document(path);
  exactKeys(source, [
    "format", "address", "size", "formation_record_size", "metadata_record_size",
    "formations", "preload_ids", "metadata",
  ], "encounter tables");
  checkedExtent(source, ENCOUNTER_TABLE_ADDRESS, ENCOUNTER_TABLE_SIZE, "encounter tables");
  if (source.formation_record_size !== 16 || source.metadata_record_size !== 8)
    throw new Error("encounter record sizes differ");
  if (!Array.isArray(source.formations) || source.formations.length !== FORMATION_COUNT) {
    throw new Error(`encounter source requires ${FORMATION_COUNT} formations`);
  }
  const formations = Buffer.alloc(FORMATION_COUNT * 16);
  source.formations.forEach((formation: Json | null, index: number) => {
    if (formation === null) return;
    // Tuple schema: [layout, [member_id, minimum, maximum] x slots].
    if (!Array.isArray(formation) || formation.length < 1 || formation.length > 6) {
      throw new Error(`formation ${index} has invalid slots`);
    }
    const offset = index * 16;
    formations[offset] = integer(formation[0], 0, 0xff, `formation ${index} layout`);
    (formation.slice(1) as Json[]).forEach((slot: Json, slotIndex: number) => {
      const [memberRaw, minimumRaw, maximumRaw] = slot as [number, number, number];
      const member = integer(memberRaw, 1, 0xff, `formation ${index} member`);
      const minimum = integer(minimumRaw, 0, 0xff, `formation ${index} minimum`);
      const maximum = integer(maximumRaw, minimum, 0xff, `formation ${index} maximum`);
      formations[offset + 1 + slotIndex] = member;
      formations[offset + 6 + slotIndex] = minimum;
      formations[offset + 11 + slotIndex] = maximum;
    });
  });
  if (!Array.isArray(source.preload_ids) || source.preload_ids.length !== PRELOAD_COUNT) {
    throw new Error(`encounter source requires ${PRELOAD_COUNT} preload IDs`);
  }
  const preload = Buffer.alloc(PRELOAD_COUNT * 2);
  source.preload_ids.forEach((value: unknown, index: number) =>
    preload.writeUInt16LE(integer(value, 0, 0xffff, `preload ID ${index}`), index * 2));
  if (!Array.isArray(source.metadata) || source.metadata.length !== METADATA_COUNT) {
    throw new Error(`encounter source requires ${METADATA_COUNT} metadata records`);
  }
  const metadata = Buffer.alloc(METADATA_COUNT * 8);
  source.metadata.forEach((entry: Json, index: number) => {
    // Tuple schema: [value, flags_0, flags_1, attribute_4].
    const [valueRaw, flags0Raw, flags1Raw, attribute4Raw] = entry as [number, unknown, unknown, number];
    const offset = index * 8;
    metadata.writeUInt16LE(integer(valueRaw, 0, 0xffff, `metadata ${index} value`), offset);
    metadata[offset + 2] = flagByte(flags0Raw, `metadata ${index} flags 0`);
    metadata[offset + 3] = flagByte(flags1Raw, `metadata ${index} flags 1`);
    metadata[offset + 4] = integer(attribute4Raw, 0, 0xff, `metadata ${index} attribute 4`);
  });
  const output = Buffer.concat([formations, preload, metadata]);
  if (output.length !== ENCOUNTER_TABLE_SIZE) throw new Error("encounter table size differs");
  return output;
}

export function build_alignment(path: string): Buffer {
  const source = document(path);
  exactKeys(source, ["format", "address", "size", "end", "fill"], "encounter alignment");
  checkedExtent(source, ALIGNMENT_ADDRESS, ALIGNMENT_SIZE, "encounter alignment");
  if (Number(source.end) !== ALIGNMENT_END || source.fill !== 0) {
    throw new Error("encounter alignment boundary differs");
  }
  return Buffer.alloc(ALIGNMENT_SIZE);
}

export function build_encounter_regions(directory: string): EncounterRegion[] {
  const sources = [
    [BRIGHTNESS_CURVE_ADDRESS, BRIGHTNESS_CURVE_SIZE, "brightness_curve.json", build_brightness_curve],
    [ENCOUNTER_TABLE_ADDRESS, ENCOUNTER_TABLE_SIZE, "encounter_tables.json", build_encounter_tables],
    [ALIGNMENT_ADDRESS, ALIGNMENT_SIZE, "alignment.json", build_alignment],
  ] as const;
  return sources.map(([address, size, source, builder]) => {
    const data = builder(`${directory}_${source}`);
    if (data.length !== size) throw new Error(`${source}: built size differs`);
    return { address, size, source, data };
  });
}

export function export_encounter_data(rom: Buffer, directory: string): EncounterRegion[] {
  if (rom.length < ALIGNMENT_END - ROM_BASE) throw new Error("ROM is too small for encounter data");
  mkdirSync(directory, { recursive: true });
  const curve = rom.subarray(
    BRIGHTNESS_CURVE_ADDRESS - ROM_BASE,
    BRIGHTNESS_CURVE_ADDRESS - ROM_BASE + BRIGHTNESS_CURVE_SIZE,
  );
  writeFileSync(join(directory, "brightness_curve.json"), pretty({
    format: 1,
    address: hexadecimal(BRIGHTNESS_CURVE_ADDRESS),
    size: BRIGHTNESS_CURVE_SIZE,
    phases: [0, 1].map((phase) => Array.from(
      curve.subarray(phase * 16, phase * 16 + 16),
      (value) => value > 0x7f ? value - 0x100 : value,
    )),
  }));
  const encounter = rom.subarray(
    ENCOUNTER_TABLE_ADDRESS - ROM_BASE,
    ENCOUNTER_TABLE_ADDRESS - ROM_BASE + ENCOUNTER_TABLE_SIZE,
  );
  const formations = Array.from({ length: FORMATION_COUNT }, (_, index) => {
    const record = encounter.subarray(index * 16, index * 16 + 16);
    if (record.every((value) => value === 0)) return null;
    const slots: Json[] = [];
    let ended = false;
    for (let slot = 0; slot < 5; slot++) {
      const member = record[1 + slot];
      const minimum = record[6 + slot];
      const maximum = record[11 + slot];
      if (member === 0) {
        ended = true;
        if (minimum !== 0 || maximum !== 0) throw new Error(`formation ${index} has data without a member`);
        continue;
      }
      if (ended || minimum > maximum) throw new Error(`formation ${index} has invalid member ordering`);
      slots.push({ member_id: member, minimum, maximum });
    }
    return { layout: record[0], slots };
  });
  const preloadOffset = FORMATION_COUNT * 16;
  const metadataOffset = preloadOffset + PRELOAD_COUNT * 2;
  const preloadIds = Array.from({ length: PRELOAD_COUNT }, (_, index) =>
    encounter.readUInt16LE(preloadOffset + index * 2));
  const metadata = Array.from({ length: METADATA_COUNT }, (_, index) => {
    const offset = metadataOffset + index * 8;
    if (encounter.subarray(offset + 5, offset + 8).some((value) => value !== 0)) {
      throw new Error(`metadata ${index} has nonzero reserved bytes`);
    }
    return {
      value: encounter.readUInt16LE(offset),
      flags_0: splitFlags(encounter[offset + 2]),
      flags_1: splitFlags(encounter[offset + 3]),
      attribute_4: encounter[offset + 4],
    };
  });
  writeFileSync(join(directory, "encounter_tables.json"), pretty({
    format: 1,
    address: hexadecimal(ENCOUNTER_TABLE_ADDRESS),
    size: ENCOUNTER_TABLE_SIZE,
    formation_record_size: 16,
    metadata_record_size: 8,
    formations,
    preload_ids: preloadIds,
    metadata,
  }));
  const alignment = rom.subarray(ALIGNMENT_ADDRESS - ROM_BASE, ALIGNMENT_END - ROM_BASE);
  if (alignment.some((value) => value !== 0)) throw new Error("encounter alignment is not zero-filled");
  writeFileSync(join(directory, "alignment.json"), pretty({
    format: 1,
    address: hexadecimal(ALIGNMENT_ADDRESS),
    size: ALIGNMENT_SIZE,
    end: hexadecimal(ALIGNMENT_END),
    fill: 0,
  }));
  const built = build_encounter_regions(directory);
  for (const region of built) {
    const start = region.address - ROM_BASE;
    if (!region.data.equals(rom.subarray(start, start + region.size))) {
      throw new Error(`${region.source}: round trip differs`);
    }
  }
  return built;
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || index + 1 >= args.length) throw new Error(`${name} is required`);
  return args[index + 1];
}

function selfTest(): void {
  const flags = splitFlags(0xad);
  if (flagByte(flags, "flags") !== 0xad) throw new Error("encounter flag self-test failed");
  let rejected = false;
  try { integer("1", 0, 1, "string value"); } catch { rejected = true; }
  if (!rejected) throw new Error("encounter integer self-test accepted a string");
  console.log("self-test=ok");
}

function main(args: string[]): void {
  if (args.includes("--self-test")) {
    selfTest();
    return;
  }
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: encounter_data.ts export ROM --directory DIR | verify ROM --directory DIR");
    return;
  }
  if ((args[0] !== "export" && args[0] !== "verify") || !args[1]) {
    throw new Error("an encounter-data command and ROM are required");
  }
  const rom = readFileSync(args[1]);
  const directory = option(args, "--directory");
  const regions = args[0] === "export"
    ? export_encounter_data(rom, directory)
    : build_encounter_regions(directory);
  for (const region of regions) {
    const start = region.address - ROM_BASE;
    if (!region.data.equals(rom.subarray(start, start + region.size))) {
      throw new Error(`${region.source}: verification differs`);
    }
  }
  const bytes = regions.reduce((sum, region) => sum + region.size, 0);
  console.log(`regions=${regions.length} bytes=${bytes} unclaimed=8 identical=true`);
}

if (import.meta.main) main(Bun.argv.slice(2));
