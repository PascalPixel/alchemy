#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { canonicalJson, isCanonicalJsonText } from "./canonical_json.ts";
import {
  existsSync, mkdirSync, mkdtempSync, readFileSync, renameSync, rmSync, writeFileSync,
} from "node:fs";
import { dirname, join, resolve } from "node:path";
import { indexed_png } from "./import_asset.ts";
import { png, type Rgb } from "./skip_sprite_archive.ts";

const ROM_BASE = 0x08000000;
export const SENTOU_HYOUJI_ADDRESS = 0x080c2a0a;
export const SENTOU_HYOUJI_END = 0x080c5b30;
export const SENTOU_HYOUJI_SIZE = SENTOU_HYOUJI_END - SENTOU_HYOUJI_ADDRESS;

const KIHON_END = 0x080c3734;
const KOMA_ADDRESS = KIHON_END;
const KOMA_END = 0x080c3f34;
const HAICHI_ADDRESS = KOMA_END;
const HAICHI_END = 0x080c5938;
const HOSEI_ADDRESS = HAICHI_END;
const HOSEI_END = 0x080c5a30;
const GAUGE_ADDRESS = HOSEI_END;
const GAUGE_END = SENTOU_HYOUJI_END;

const JsonObject = Object as unknown as { keys(value: unknown): string[] };
type Json = Record<string, any>;

const PALETTE: Rgb[] = Array.from({ length: 16 }, (_, index) => {
  const value = index * 8;
  return [value, value, value];
});

function integer(value: unknown, minimum: number, maximum: number, name: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < minimum || value > maximum)
    throw new Error(`invalid ${name}`);
  return value;
}

function exactKeys(value: unknown, keys: string[], name: string): asserts value is Json {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${name} must be an object`);
  const actual = JsonObject.keys(value).sort(), expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index]))
    throw new Error(`${name} has unknown fields`);
}

function document(path: string): Json {
  const text = readFileSync(path, "utf8");
  const source = JSON.parse(text);
  if (typeof source !== "object" || source === null || Array.isArray(source) || source.format !== 1)
    throw new Error(`${path}: unsupported source format`);
  if (!isCanonicalJsonText(text, source)) throw new Error(`${path}: source is not canonical JSON`);
  return source;
}

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function pretty(value: unknown): string {
  return `${canonicalJson(value)}\n`;
}

function extent(source: Json, address: number, size: number, name: string): void {
  if (source.address !== hexadecimal(address) || source.size !== size) throw new Error(`${name} extent differs`);
}

function array(value: unknown, count: number, name: string): unknown[] {
  if (!Array.isArray(value) || value.length !== count) throw new Error(`${name} requires ${count} entries`);
  return value;
}

function signedByte(value: unknown, name: string): number {
  return integer(value, -128, 127, name) & 0xff;
}

function unsignedByte(value: unknown, name: string): number {
  return integer(value, 0, 0xff, name);
}

function unsignedHalf(value: unknown, name: string): number {
  return integer(value, 0, 0xffff, name);
}

function signedWord(value: unknown, name: string): number {
  return integer(value, -0x80000000, 0x7fffffff, name);
}

function writeWords(values: unknown[], signed: boolean, name: string): Buffer {
  const output = Buffer.alloc(values.length * 4);
  values.forEach((value, index) => {
    const parsed = signed ? signedWord(value, `${name} ${index}`) : integer(value, 0, 0xffffffff, `${name} ${index}`);
    if (signed) output.writeInt32LE(parsed, index * 4);
    else output.writeUInt32LE(parsed, index * 4);
  });
  return output;
}

function sparseBytes(source: Json, count: number, name: string, allowed: ReadonlySet<number>): Buffer {
  exactKeys(source, ["count", "default", "entries"], name);
  if (source.count !== count || source.default !== 0 || !Array.isArray(source.entries))
    throw new Error(`${name} shape differs`);
  const output = Buffer.alloc(count), seen = new Set<number>();
  let previous = -1;
  source.entries.forEach((entry: unknown, index: number) => {
    exactKeys(entry, ["id", "value"], `${name} entry ${index}`);
    const id = integer(entry.id, 0, count - 1, `${name} ID`);
    if (seen.has(id) || id <= previous) throw new Error(`${name} IDs must be unique and increasing`);
    const value = unsignedByte(entry.value, `${name} value`);
    if (value === 0) throw new Error(`${name} sparse value is the default`);
    if (!allowed.has(value)) throw new Error(`${name} value is not a known mode`);
    output[id] = value; seen.add(id); previous = id;
  });
  return output;
}

function sparseFlags(source: Json): Buffer {
  exactKeys(source, ["count", "entries"], "display flags");
  if (source.count !== 519 || !Array.isArray(source.entries)) throw new Error("display flag shape differs");
  const output = Buffer.alloc(519 * 4), seen = new Set<number>();
  const allowedFlags = new Set([0, 1, 2, 4, 8, 0x40, 0x80, 0x81]);
  let previous = -1;
  source.entries.forEach((entry: unknown, index: number) => {
    exactKeys(entry, ["id", "animation_id", "descriptor_flags"], `display flag ${index}`);
    const id = integer(entry.id, 0, 518, "display flag ID");
    if (seen.has(id) || id <= previous) throw new Error("display flag IDs must be unique and increasing");
    const animation = integer(entry.animation_id, 0, 0x0fff, "animation ID");
    const flags = integer(entry.descriptor_flags, 0, 0x000fffff, "descriptor flags");
    if (!allowedFlags.has(flags)) throw new Error("display descriptor flags are not recognized");
    const value = animation + flags * 0x1000;
    if (value > 0xffffffff) throw new Error("display descriptor exceeds one word");
    if (value === 0) throw new Error("display sparse value is the default");
    output.writeUInt32LE(value, id * 4); seen.add(id); previous = id;
  });
  return output;
}

function pairs(values: unknown, count: number, name: string, terminator: boolean): Buffer {
  const entries = array(values, count, name), output = Buffer.alloc((count + Number(terminator)) * 4);
  const ids = new Set<number>(), slots = new Set<number>();
  entries.forEach((entry, index) => {
    exactKeys(entry, ["id", "slot"], `${name} ${index}`);
    const id = unsignedHalf(entry.id, `${name} ID`), slot = unsignedHalf(entry.slot, `${name} slot`);
    if (id === 0xffff && slot === 0xffff) throw new Error(`${name} contains an early terminator`);
    if (ids.has(id) || slots.has(slot)) throw new Error(`${name} IDs and slots must be unique`);
    ids.add(id); slots.add(slot);
    output.writeUInt16LE(id, index * 4);
    output.writeUInt16LE(slot, index * 4 + 2);
  });
  if (terminator) output.writeUInt32LE(0xffffffff, count * 4);
  return output;
}

function buildKihon(path: string): Buffer {
  const source = document(path);
  exactKeys(source, [
    "format", "address", "size", "alignment", "party_capacity", "party_order_offsets", "actor_pose_rows",
    "party_positions", "transform_q16", "formula_control", "rate_tables", "probability_curves", "action_modes",
    "action_alignment", "action_display", "selection_layout", "particle_offsets", "particle_dimensions",
    "particle_sources", "particle_adjustments", "short_id_map", "long_id_map",
  ], "basic battle tables");
  extent(source, SENTOU_HYOUJI_ADDRESS, KIHON_END - SENTOU_HYOUJI_ADDRESS, "basic battle tables");
  exactKeys(source.alignment, ["size", "fill"], "basic alignment");
  if (source.alignment.size !== 2 || source.alignment.fill !== 0 || source.party_capacity !== 16)
    throw new Error("basic header differs");
  const order = Buffer.from(array(source.party_order_offsets, 12, "party order offsets")
    .map((value, index) => signedByte(value, `party order offset ${index}`)));
  const lookups = Buffer.alloc(5 * 7 * 2), actors = [0, 1, 2, 3, 5];
  array(source.actor_pose_rows, 5, "actor pose rows").forEach((row, rowIndex) => {
    exactKeys(row, ["actor", "pose_ids"], `actor pose row ${rowIndex}`);
    if (row.actor !== actors[rowIndex]) throw new Error(`actor pose row ${rowIndex} actor differs`);
    array(row.pose_ids, 7, `actor pose row ${rowIndex}`).forEach((value, column) =>
      lookups.writeUInt16LE(unsignedHalf(value, `actor pose ${rowIndex}:${column}`), (rowIndex * 7 + column) * 2));
  });
  const positions = Buffer.alloc(26);
  array(source.party_positions, 13, "party positions").forEach((entry, index) => {
    exactKeys(entry, ["x", "y"], `party position ${index}`);
    positions[index * 2] = signedByte(entry.x, `party position ${index} x`);
    positions[index * 2 + 1] = signedByte(entry.y, `party position ${index} y`);
  });
  const transform = writeWords(array(source.transform_q16, 15, "transform"), true, "transform");
  const formulaControl = Buffer.from(array(source.formula_control, 8, "formula control")
    .map((value, index) => unsignedByte(value, `formula control ${index}`)));
  const rates = Buffer.alloc(8 * 6 * 4);
  array(source.rate_tables, 8, "rate tables").forEach((row, rowIndex) =>
    writeWords(array(row, 6, `rate table ${rowIndex}`), false, `rate table ${rowIndex}`).copy(rates, rowIndex * 24));
  const thresholds = Buffer.from(array(source.probability_curves, 3, "probability curves").flatMap((row, rowIndex) =>
    array(row, 8, `probability curve ${rowIndex}`).map((value, index) => unsignedByte(value, `probability ${rowIndex}:${index}`))));
  const actionClasses = sparseBytes(source.action_modes, 518, "action modes", new Set([2, 3, 5, 6, 8, 9]));
  exactKeys(source.action_alignment, ["size", "fill"], "action alignment");
  if (source.action_alignment.size !== 2 || source.action_alignment.fill !== 0)
    throw new Error("action alignment differs");
  const actionDisplay = sparseFlags(source.action_display);
  exactKeys(source.selection_layout, ["defaults", "order", "matrix_q16"], "selection layout");
  const waitModes = writeWords(array(source.selection_layout.defaults, 2, "selection defaults"), false, "selection defaults");
  const waitOrder = Buffer.from(array(source.selection_layout.order, 16, "selection order")
    .map((value, index) => unsignedByte(value, `selection order ${index}`)));
  if (new Set(waitOrder).size !== 16 || waitOrder.some((value) => value >= 16))
    throw new Error("selection order must be a permutation of 0..15");
  const waitMatrix = writeWords(array(source.selection_layout.matrix_q16, 4, "selection matrix").flatMap((row, rowIndex) =>
    array(row, 3, `selection matrix ${rowIndex}`)), true, "selection matrix");
  const particleOffsetValues = array(source.particle_offsets, 7, "particle offsets")
    .map((value, index) => integer(value, 0, 0xffffffff, `particle offset ${index}`));
  const particleDimensionValues = array(source.particle_dimensions, 7, "particle dimensions")
    .map((value, index) => integer(value, 1, 4, `particle dimension ${index}`));
  let particleExtent = 0;
  particleOffsetValues.forEach((offset, index) => {
    if (offset !== particleExtent) throw new Error("particle offsets must follow cumulative square dimensions");
    particleExtent += particleDimensionValues[index] ** 2;
  });
  const particleSourceValues = array(source.particle_sources, 4, "particle sources")
    .map((value, index) => integer(value, 0, 0xffffffff, `particle source ${index}`));
  particleSourceValues.forEach((value, index) => {
    if (value !== particleExtent + index * 1024) throw new Error("particle sources must follow the particle extent");
  });
  const particleOffsets = writeWords(particleOffsetValues, false, "particle offsets");
  const particleDimensions = Buffer.from(particleDimensionValues);
  const particleSources = writeWords(particleSourceValues, false, "particle sources");
  const particleAdjustments = writeWords(array(source.particle_adjustments, 4, "particle adjustments"), false, "particle adjustments");
  const shortMap = pairs(source.short_id_map, 8, "short ID map", true);
  const longMap = pairs(source.long_id_map, 49, "long ID map", true);
  source.long_id_map.forEach((entry: Json, index: number) => {
    if (entry.slot !== index) throw new Error("long ID map slots must be sequential");
  });
  const output = Buffer.concat([
    Buffer.alloc(2), writeWords([source.party_capacity], false, "party capacity"), order, lookups, positions, transform,
    formulaControl, rates, thresholds, actionClasses, Buffer.alloc(2), actionDisplay, waitModes, waitOrder,
    waitMatrix, particleOffsets, particleDimensions, Buffer.alloc(1), particleSources, particleAdjustments,
    shortMap, longMap,
  ]);
  if (output.length !== KIHON_END - SENTOU_HYOUJI_ADDRESS) throw new Error("basic battle table size differs");
  return output;
}

function readAtlas(path: string, frames: number, frameTilesWide: number, frameTilesHigh: number, columns: number): Buffer {
  const encoded = readFileSync(path);
  const [width, height, pixels, colors] = indexed_png(encoded);
  const frameWidth = frameTilesWide * 8, frameHeight = frameTilesHigh * 8;
  const rows = Math.ceil(frames / columns);
  if (width !== columns * frameWidth || height !== rows * frameHeight) throw new Error(`${path}: atlas dimensions differ`);
  if (JSON.stringify(colors) !== JSON.stringify(PALETTE)) throw new Error(`${path}: symbolic palette differs`);
  if (pixels.some((value) => value >= 16)) throw new Error(`${path}: pixel exceeds 4bpp`);
  if (!encoded.equals(png(Buffer.from(pixels), width, height, colors)))
    throw new Error(`${path}: atlas is not a canonical source PNG`);
  const output = Buffer.alloc(frames * frameTilesWide * frameTilesHigh * 32);
  let target = 0;
  for (let frame = 0; frame < frames; frame++) {
    const frameLeft = frame % columns * frameWidth, frameTop = Math.floor(frame / columns) * frameHeight;
    for (let tileY = 0; tileY < frameTilesHigh; tileY++) for (let tileX = 0; tileX < frameTilesWide; tileX++) {
      for (let y = 0; y < 8; y++) for (let x = 0; x < 8; x += 2) {
        const offset = (frameTop + tileY * 8 + y) * width + frameLeft + tileX * 8 + x;
        output[target++] = pixels[offset] | pixels[offset + 1] << 4;
      }
    }
  }
  return output;
}

function writeAtlas(raw: Uint8Array, path: string, frames: number, frameTilesWide: number, frameTilesHigh: number, columns: number): void {
  const frameWidth = frameTilesWide * 8, frameHeight = frameTilesHigh * 8;
  const width = columns * frameWidth, height = Math.ceil(frames / columns) * frameHeight;
  const pixels = Buffer.alloc(width * height);
  let source = 0;
  for (let frame = 0; frame < frames; frame++) {
    const frameLeft = frame % columns * frameWidth, frameTop = Math.floor(frame / columns) * frameHeight;
    for (let tileY = 0; tileY < frameTilesHigh; tileY++) for (let tileX = 0; tileX < frameTilesWide; tileX++) {
      for (let y = 0; y < 8; y++) for (let x = 0; x < 8; x += 2) {
        const byte = raw[source++], offset = (frameTop + tileY * 8 + y) * width + frameLeft + tileX * 8 + x;
        pixels[offset] = byte & 15; pixels[offset + 1] = byte >>> 4;
      }
    }
  }
  if (source !== raw.length) throw new Error("atlas source size differs");
  writeFileSync(path, png(pixels, width, height, PALETTE));
}

function buildHaichi(path: string): Buffer {
  const source = document(path);
  exactKeys(source, ["format", "address", "size", "record_size", "groups"], "formation display table");
  extent(source, HAICHI_ADDRESS, HAICHI_END - HAICHI_ADDRESS, "formation display table");
  if (source.record_size !== 20 || !Array.isArray(source.groups) || source.groups.length !== 72)
    throw new Error("formation display shape differs");
  const records: Buffer[] = [];
  source.groups.forEach((group: unknown, groupIndex: number) => {
    if (!Array.isArray(group) || group.length > 6) throw new Error(`formation group ${groupIndex} is invalid`);
    if ((groupIndex < 71 && group.length === 0) || (groupIndex === 71 && group.length !== 0))
      throw new Error("formation groups require 71 presets and one final terminator");
    const actors = new Set<number>();
    group.forEach((entry: unknown, entryIndex: number) => {
      exactKeys(entry, ["actor", "level", "djinn_counts", "equipment", "abilities"],
        `formation group ${groupIndex} entry ${entryIndex}`);
      const record = Buffer.alloc(20);
      const actor = integer(entry.actor, 0, 5, "preset actor");
      if (actor === 4 || actors.has(actor)) throw new Error("preset actors must be recognized and unique");
      actors.add(actor); record[0] = actor;
      record[1] = unsignedByte(entry.level, "preset level");
      array(entry.djinn_counts, 4, "Djinn counts").forEach((value, index) =>
        record[index + 2] = integer(value, 0, 7, `Djinn count ${index}`));
      array(entry.equipment, 4, "equipment").forEach((value, index) =>
        record.writeUInt16LE(unsignedHalf(value, `equipment ID ${index}`), 6 + index * 2));
      array(entry.abilities, 2, "abilities").forEach((value, index) =>
        record.writeUInt16LE(unsignedHalf(value, `ability ID ${index}`), 14 + index * 2));
      records.push(record);
    });
    const terminator = Buffer.alloc(20); terminator[0] = 0xff; records.push(terminator);
  });
  const output = Buffer.concat(records);
  if (output.length !== HAICHI_END - HAICHI_ADDRESS) throw new Error("formation display table size differs");
  return output;
}

function buildHosei(path: string): Buffer {
  const source = document(path);
  exactKeys(source, [
    "format", "address", "size", "object_descriptor", "weapon_map", "object_curves", "control",
  ], "battle correction tables");
  extent(source, HOSEI_ADDRESS, HOSEI_END - HOSEI_ADDRESS, "battle correction tables");
  if (!Array.isArray(source.weapon_map) || source.weapon_map.length !== 51)
    throw new Error("battle correction shape differs");
  const classMap = Buffer.alloc(52 * 2);
  let previousItem = -1;
  source.weapon_map.forEach((entry: unknown, index: number) => {
    exactKeys(entry, ["item_id", "weapon_class"], `weapon map ${index}`);
    const id = integer(entry.item_id, 0, 0x1ff, "item ID");
    const group = integer(entry.weapon_class, 0, 6, "weapon class");
    if (id <= previousItem) throw new Error("weapon map item IDs must be unique and increasing");
    previousItem = id;
    classMap.writeUInt16LE(id | group << 9, index * 2);
  });
  classMap.writeUInt16LE(0xffff, 51 * 2);
  exactKeys(source.object_curves, ["field_52_q16", "field_48_q16", "field_40_q16", "effect_percent"], "object curves");
  const curves = ["field_52_q16", "field_48_q16", "field_40_q16", "effect_percent"].map((name) =>
    writeWords(array(source.object_curves[name], 8, name), false, name));
  const control = writeWords(array(source.control, 3, "control"), false, "control");
  const output = Buffer.concat([writeWords([source.object_descriptor], false, "object descriptor"), classMap, ...curves, control]);
  if (output.length !== HOSEI_END - HOSEI_ADDRESS) throw new Error("battle correction table size differs");
  return output;
}

export function build_sentou_hyouji(indexPath: string): Buffer {
  const index = document(indexPath);
  exactKeys(index, ["format", "kind", "address", "size", "end", "sources"], "battle display index");
  if (index.kind !== "golden-sun-sentou-hyouji" || index.address !== hexadecimal(SENTOU_HYOUJI_ADDRESS) ||
      index.size !== SENTOU_HYOUJI_SIZE || index.end !== hexadecimal(SENTOU_HYOUJI_END))
    throw new Error("battle display index extent differs");
  exactKeys(index.sources, ["kihon", "koma", "haichi", "hosei", "gauge"], "battle display sources");
  for (const [name, expected] of [["kihon", "kihon.json"], ["haichi", "haichi.json"], ["hosei", "hosei.json"]] as const)
    if (index.sources[name] !== expected) throw new Error(`battle display ${name} source differs`);
  exactKeys(index.sources.koma, ["source", "banks", "frames_per_bank", "width", "height"], "battle frame source");
  if (index.sources.koma.source !== "koma.4bpp.png" || index.sources.koma.banks !== 2 ||
      index.sources.koma.frames_per_bank !== 8 || index.sources.koma.width !== 16 || index.sources.koma.height !== 16)
    throw new Error("battle frame layout differs");
  exactKeys(index.sources.gauge, ["source", "frames", "width", "height"], "gauge source");
  if (index.sources.gauge.source !== "gauge.4bpp.png" || index.sources.gauge.frames !== 8 ||
      index.sources.gauge.width !== 8 || index.sources.gauge.height !== 8)
    throw new Error("gauge layout differs");
  const directory = dirname(indexPath);
  const output = Buffer.concat([
    buildKihon(join(directory, index.sources.kihon)),
    readAtlas(join(directory, index.sources.koma.source), 16, 2, 2, 8),
    buildHaichi(join(directory, index.sources.haichi)),
    buildHosei(join(directory, index.sources.hosei)),
    readAtlas(join(directory, index.sources.gauge.source), 8, 1, 1, 8),
  ]);
  if (output.length !== SENTOU_HYOUJI_SIZE) throw new Error("battle display output size differs");
  return output;
}

function sparseByteExport(data: Uint8Array): Json {
  return {
    count: data.length,
    default: 0,
    entries: Array.from(data, (value, id) => value ? { id, value } : null).filter(Boolean),
  };
}

function flagsExport(data: Buffer): Json {
  const entries: Json[] = [];
  for (let id = 0; id < data.length / 4; id++) {
    const value = data.readUInt32LE(id * 4);
    if (!value) continue;
    entries.push({ id, animation_id: value & 0x0fff, descriptor_flags: value >>> 12 });
  }
  return { count: data.length / 4, entries };
}

function pairExport(data: Buffer, address: number, count: number): Json[] {
  return Array.from({ length: count }, (_, index) => ({
    id: data.readUInt16LE(address + index * 4),
    slot: data.readUInt16LE(address + index * 4 + 2),
  }));
}

function writeSentouHyouji(rom: Buffer, directory: string): void {
  const start = SENTOU_HYOUJI_ADDRESS - ROM_BASE, end = SENTOU_HYOUJI_END - ROM_BASE;
  if (start < 0 || end > rom.length) throw new Error("ROM is too small for battle display data");
  mkdirSync(directory, { recursive: true });
  const data = rom.subarray(start, end), at = (address: number) => address - SENTOU_HYOUJI_ADDRESS;
  if (data.subarray(at(0x080c2a0a), at(0x080c2a0c)).some(Boolean)) throw new Error("battle display alignment is not zero");
  if (data.subarray(at(0x080c2d9e), at(0x080c2da0)).some(Boolean)) throw new Error("action alignment is not zero");
  if (data[at(0x080c3627)] !== 0) throw new Error("particle dimension alignment is not zero");
  const classLookups = Array.from({ length: 5 }, (_, row) => Array.from({ length: 7 }, (_, column) =>
    data.readUInt16LE(at(0x080c2a1c) + (row * 7 + column) * 2)));
  const positions = Array.from({ length: 13 }, (_, index) => ({
    x: data.readInt8(at(0x080c2a62) + index * 2), y: data.readInt8(at(0x080c2a62) + index * 2 + 1),
  }));
  const rateTables = Array.from({ length: 8 }, (_, row) => Array.from({ length: 6 }, (_, column) =>
    data.readUInt32LE(at(0x080c2ac0) + (row * 6 + column) * 4)));
  const thresholds = Array.from({ length: 3 }, (_, row) =>
    Array.from(data.subarray(at(0x080c2b80) + row * 8, at(0x080c2b80) + (row + 1) * 8)));
  const waitMatrix = Array.from({ length: 4 }, (_, row) => Array.from({ length: 3 }, (_, column) =>
    data.readInt32LE(at(0x080c35d4) + (row * 3 + column) * 4)));
  const basic = {
    format: 1,
    address: hexadecimal(SENTOU_HYOUJI_ADDRESS),
    size: KIHON_END - SENTOU_HYOUJI_ADDRESS,
    alignment: { size: 2, fill: 0 },
    party_capacity: data.readUInt32LE(at(0x080c2a0c)),
    party_order_offsets: Array.from({ length: 12 }, (_, index) => data.readInt8(at(0x080c2a10) + index)),
    actor_pose_rows: [0, 1, 2, 3, 5].map((actor, index) => ({ actor, pose_ids: classLookups[index] })),
    party_positions: positions,
    transform_q16: Array.from({ length: 15 }, (_, index) => data.readInt32LE(at(0x080c2a7c) + index * 4)),
    formula_control: Array.from(data.subarray(at(0x080c2ab8), at(0x080c2ac0))),
    rate_tables: rateTables,
    probability_curves: thresholds,
    action_modes: sparseByteExport(data.subarray(at(0x080c2b98), at(0x080c2d9e))),
    action_alignment: { size: 2, fill: 0 },
    action_display: flagsExport(data.subarray(at(0x080c2da0), at(0x080c35bc))),
    selection_layout: {
      defaults: Array.from({ length: 2 }, (_, index) => data.readUInt32LE(at(0x080c35bc) + index * 4)),
      order: Array.from(data.subarray(at(0x080c35c4), at(0x080c35d4))),
      matrix_q16: waitMatrix,
    },
    particle_offsets: Array.from({ length: 7 }, (_, index) => data.readUInt32LE(at(0x080c3604) + index * 4)),
    particle_dimensions: Array.from(data.subarray(at(0x080c3620), at(0x080c3627))),
    particle_sources: Array.from({ length: 4 }, (_, index) => data.readUInt32LE(at(0x080c3628) + index * 4)),
    particle_adjustments: Array.from({ length: 4 }, (_, index) => data.readUInt32LE(at(0x080c3638) + index * 4)),
    short_id_map: pairExport(data, at(0x080c3648), 8),
    long_id_map: pairExport(data, at(0x080c366c), 49),
  };
  if (data.readUInt32LE(at(0x080c3668)) !== 0xffffffff || data.readUInt32LE(at(0x080c3730)) !== 0xffffffff)
    throw new Error("battle display map terminator differs");
  writeFileSync(join(directory, "kihon.json"), pretty(basic));
  writeAtlas(data.subarray(at(KOMA_ADDRESS), at(KOMA_END)), join(directory, "koma.4bpp.png"), 16, 2, 2, 8);
  const groups: Json[][] = [], group: Json[] = [];
  for (let offset = at(HAICHI_ADDRESS); offset < at(HAICHI_END); offset += 20) {
    const slot = data.readInt8(offset);
    if (slot === -1) {
      if (data.subarray(offset + 1, offset + 20).some(Boolean)) throw new Error("formation terminator is not empty");
      groups.push(group.splice(0)); continue;
    }
    if (data.readUInt16LE(offset + 18) !== 0) throw new Error("formation reserved field is not zero");
    group.push({
      actor: slot,
      level: data[offset + 1],
      djinn_counts: Array.from({ length: 4 }, (_, index) => data.readInt8(offset + 2 + index)),
      equipment: Array.from({ length: 4 }, (_, index) => data.readUInt16LE(offset + 6 + index * 2)),
      abilities: Array.from({ length: 2 }, (_, index) => data.readUInt16LE(offset + 14 + index * 2)),
    });
  }
  if (group.length || groups.length !== 72) throw new Error("formation grouping differs");
  writeFileSync(join(directory, "haichi.json"), pretty({
    format: 1, address: hexadecimal(HAICHI_ADDRESS), size: HAICHI_END - HAICHI_ADDRESS, record_size: 20, groups,
  }));
  const classMap = Array.from({ length: 51 }, (_, index) => data.readUInt16LE(at(0x080c593c) + index * 2));
  if (data.readUInt16LE(at(0x080c59a2)) !== 0xffff) throw new Error("class map terminator differs");
  writeFileSync(join(directory, "hosei.json"), pretty({
    format: 1,
    address: hexadecimal(HOSEI_ADDRESS),
    size: HOSEI_END - HOSEI_ADDRESS,
    object_descriptor: data.readUInt32LE(at(0x080c5938)),
    weapon_map: classMap.map((value) => ({ item_id: value & 0x1ff, weapon_class: value >>> 9 })),
    object_curves: {
      field_52_q16: Array.from({ length: 8 }, (_, index) => data.readUInt32LE(at(0x080c59a4) + index * 4)),
      field_48_q16: Array.from({ length: 8 }, (_, index) => data.readUInt32LE(at(0x080c59c4) + index * 4)),
      field_40_q16: Array.from({ length: 8 }, (_, index) => data.readUInt32LE(at(0x080c59e4) + index * 4)),
      effect_percent: Array.from({ length: 8 }, (_, index) => data.readUInt32LE(at(0x080c5a04) + index * 4)),
    },
    control: Array.from({ length: 3 }, (_, index) => data.readUInt32LE(at(0x080c5a24) + index * 4)),
  }));
  writeAtlas(data.subarray(at(GAUGE_ADDRESS), at(GAUGE_END)), join(directory, "gauge.4bpp.png"), 8, 1, 1, 8);
  writeFileSync(join(directory, "index.json"), pretty({
    format: 1,
    kind: "golden-sun-sentou-hyouji",
    address: hexadecimal(SENTOU_HYOUJI_ADDRESS),
    size: SENTOU_HYOUJI_SIZE,
    end: hexadecimal(SENTOU_HYOUJI_END),
    sources: {
      kihon: "kihon.json",
      koma: { source: "koma.4bpp.png", banks: 2, frames_per_bank: 8, width: 16, height: 16 },
      haichi: "haichi.json",
      hosei: "hosei.json",
      gauge: { source: "gauge.4bpp.png", frames: 8, width: 8, height: 8 },
    },
  }));
  const rebuilt = build_sentou_hyouji(join(directory, "index.json"));
  if (!rebuilt.equals(data)) throw new Error("exported battle display source does not round-trip");
}

export function export_sentou_hyouji(rom: Buffer, directory: string): void {
  const destination = resolve(directory), parent = dirname(destination);
  if (destination === parent) throw new Error("battle display export requires a dedicated directory");
  mkdirSync(parent, { recursive: true });
  const transaction = mkdtempSync(join(parent, ".sentou-hyouji-export-"));
  const staged = join(transaction, "new"), previous = join(transaction, "previous");
  let installed = false;
  try {
    writeSentouHyouji(rom, staged);
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

export function verify_sentou_hyouji(rom: Buffer, indexPath: string): void {
  const expected = rom.subarray(SENTOU_HYOUJI_ADDRESS - ROM_BASE, SENTOU_HYOUJI_END - ROM_BASE);
  const built = build_sentou_hyouji(indexPath);
  if (!built.equals(expected)) {
    let offset = 0; while (offset < built.length && built[offset] === expected[offset]) offset++;
    throw new Error(`battle display differs at ${hexadecimal(SENTOU_HYOUJI_ADDRESS + offset)}`);
  }
}

export function selfTest(): void {
  const temporary = mkdtempSync("/private/tmp/sentou-hyouji-self-test-");
  try {
    const raw = Buffer.alloc(16 * 4 * 32);
    for (let index = 0; index < raw.length; index++) raw[index] = index * 37 + 11;
    const image = join(temporary, "atlas.png");
    writeAtlas(raw, image, 16, 2, 2, 8);
    if (!readAtlas(image, 16, 2, 2, 8).equals(raw)) throw new Error("atlas round-trip failed");
    let rejected = 0;
    const reject = (action: () => void): void => { try { action(); } catch { rejected++; } };
    reject(() => exactKeys({ format: 1, extra: true }, ["format"], "test source"));
    reject(() => sparseBytes({ count: 2, default: 0, entries: [{ id: 1, value: 2 }, { id: 0, value: 2 }] },
      2, "test modes", new Set([2])));
    reject(() => sparseFlags({ count: 519, entries: [{ id: 0, animation_id: 1, descriptor_flags: 3 }] }));
    reject(() => pairs([{ id: 0xffff, slot: 0xffff }], 1, "test map", true));
    writeFileSync(image, Buffer.concat([readFileSync(image), Buffer.from("hidden")]));
    reject(() => readAtlas(image, 16, 2, 2, 8));
    const duplicate = join(temporary, "duplicate.json");
    writeFileSync(duplicate, "{\"format\":0,\"format\":1}\n");
    reject(() => document(duplicate));
    if (rejected !== 6) throw new Error("adversarial source validation failed");
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok");
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { selfTest(); return; }
  const command = args[0];
  if (command === "export" && args.length === 4 && args[2] === "--directory") {
    export_sentou_hyouji(readFileSync(args[1]), args[3]);
  } else if (command === "verify" && args.length === 3) {
    verify_sentou_hyouji(readFileSync(args[1]), args[2]);
    console.log(`address=${hexadecimal(SENTOU_HYOUJI_ADDRESS)} bytes=${SENTOU_HYOUJI_SIZE} exact=true`);
  } else {
    throw new Error("usage: sentou_hyouji.ts export ROM --directory DIR | verify ROM INDEX | --self-test");
  }
}

if (import.meta.main) main(process.argv.slice(2));
