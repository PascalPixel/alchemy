#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { canonicalJson, isCanonicalJsonText } from "./canonical_json.ts";
import {
  existsSync, lstatSync, mkdirSync, mkdtempSync, readFileSync, readdirSync, renameSync, realpathSync, rmSync, symlinkSync,
  writeFileSync,
} from "node:fs";
import { tmpdir } from "node:os";
import { basename, dirname, isAbsolute, join, relative, resolve } from "node:path";
import { byte_png } from "./export_asset.ts";
import { indexed_png } from "./import_asset.ts";
import { build_grid, export_grid, mask_png } from "./kind1_map_grid.ts";
import {
  build_blend_animation,
  build_descriptors,
  build_header,
  build_metatiles,
  build_queues,
  build_sparse,
  export_blend_animation,
  export_descriptors,
  export_header,
  export_metatiles,
  export_queues,
  export_sparse,
} from "./map_container_components.ts";
import { export_tilemap, import_tilemap } from "./tilemap.ts";

const ROM_BASE = 0x08000000;
const ROM_SIZE = 0x00800000;
const RESOURCE_TABLE = 0x08320000;
const HEADER_SIZE = 0x3c;

interface ResourceSpec {
  id: number;
  address: number;
  size: number;
}

interface ResourceEntry {
  id: string;
  address: string;
  size: string;
  directory: string;
}

interface IndexDocument {
  format: 1;
  kind: "golden-sun-chiiki-map-series";
  resources: ResourceEntry[];
}

type JsonObject = Record<string, unknown>;

export interface BuiltChiikiMap {
  id: number;
  address: number;
  data: Buffer;
  sources: string[];
}

export const CHIIKI_MAP_RESOURCES: readonly ResourceSpec[] = [
  { id: 0x1e5, address: 0x085bb860, size: 0x182c },
  { id: 0x320, address: 0x08753b18, size: 0x2040 },
] as const;

function exactKeys(value: Record<string, unknown>, keys: readonly string[], label: string): void {
  const actual = Object.keys(value).sort();
  const expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${label} has unexpected fields`);
  }
}

function hex(value: number, width = 8): string {
  return `0x${value.toString(16).padStart(width, "0")}`;
}

function idText(value: number): string {
  return value.toString(16).padStart(3, "0");
}

function object(value: unknown, label: string): JsonObject {
  if (typeof value !== "object" || value === null || Array.isArray(value)) {
    throw new Error(`${label} must be an object`);
  }
  return value as JsonObject;
}

function document(path: string, label: string, compact = false): JsonObject {
  const text = readFileSync(path, "utf8");
  const value = JSON.parse(text);
  if (!isCanonicalJsonText(text, value)) throw new Error(`${label} is not canonical JSON`);
  return object(value, label);
}

function integer(value: unknown, label: string, minimum = 0, maximum = Number.MAX_SAFE_INTEGER): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < minimum || value > maximum) {
    throw new Error(`${label} must be an integer from ${minimum} through ${maximum}`);
  }
  return value;
}

function array(value: unknown, label: string): unknown[] {
  if (!Array.isArray(value)) throw new Error(`${label} must be an array`);
  return value;
}

function canonicalHex(value: unknown, label: string, width = 0): number {
  if (typeof value !== "string" || !/^0x(?:0|[0-9a-f]+)$/.test(value)) {
    throw new Error(`${label} must be canonical lowercase hexadecimal`);
  }
  const parsed = Number(value);
  if (!Number.isSafeInteger(parsed) || value !== hex(parsed, width || 1)) {
    throw new Error(`${label} is not canonical`);
  }
  return parsed;
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

function same(left: string, right: string): boolean {
  return physicalPath(left) === physicalPath(right);
}

function containsPath(directory: string, path: string): boolean {
  const position = relative(physicalPath(directory), physicalPath(path));
  return position === "" || (position !== ".." && !position.startsWith("../") &&
    !position.startsWith("..\\") && !isAbsolute(position));
}

function validateExportDestination(romPath: string, directory: string): void {
  if (same(romPath, directory) || containsPath(directory, romPath)) {
    throw new Error("regional-map export directory must not contain its input ROM");
  }
  const destination = resolve(directory);
  if (!existsSync(destination)) return;
  if (!lstatSync(destination).isDirectory()) throw new Error("regional-map export destination must be a directory");
  if (readdirSync(destination).length === 0) return;
  const marker = join(destination, "index.json");
  if (!existsSync(marker) || !lstatSync(marker).isFile()) {
    throw new Error("refusing to replace a directory that is not a regional-map package");
  }
  try { parseIndex(marker); }
  catch { throw new Error("refusing to replace a directory that is not a canonical regional-map package"); }
}

function confined(root: string, name: string, prefix = ""): string {
  if (!/^resource_[0-9a-f]{3}$/.test(name)) throw new Error("regional-map directory name is not canonical");
  const canonicalRoot = realpathSync(root);
  const path = resolve(canonicalRoot, prefix + name);
  const position = relative(canonicalRoot, path);
  if (position !== prefix + name) throw new Error("map source is not its canonical path");
  return path;
}

function parseIndex(indexPath: string): [IndexDocument, ResourceSpec[]] {
  const index = document(indexPath, "regional-map index") as unknown as IndexDocument;
  exactKeys(index as unknown as Record<string, unknown>, ["format", "kind", "resources"], "regional-map index");
  if (index.format !== 1 || index.kind !== "golden-sun-chiiki-map-series" || !Array.isArray(index.resources)) {
    throw new Error("unsupported regional-map index");
  }
  if (index.resources.length !== CHIIKI_MAP_RESOURCES.length) {
    throw new Error("regional-map index has a different resource count");
  }
  index.resources.forEach((entry, position) => {
    if (typeof entry !== "object" || entry === null || Array.isArray(entry)) {
      throw new Error("regional-map resource entry must be an object");
    }
    exactKeys(entry as unknown as Record<string, unknown>, ["id", "address", "size", "directory"], "regional-map resource entry");
    const spec = CHIIKI_MAP_RESOURCES[position];
    if (entry.id !== idText(spec.id) || entry.address !== hex(spec.address) ||
        entry.size !== hex(spec.size, 1) || entry.directory !== `resource_${idText(spec.id)}`) {
      throw new Error("regional-map resource entry differs from the audited catalog");
    }
  });
  return [index, [...CHIIKI_MAP_RESOURCES]];
}

function componentOffsets(header: Uint8Array, size: number): number[] {
  if (header.length !== HEADER_SIZE || size <= HEADER_SIZE) throw new Error("invalid regional-map container extent");
  const view = new DataView(header.buffer, header.byteOffset, header.byteLength);
  const offsets = Array.from({ length: 6 }, (_, slot) => view.getUint32(0x24 + slot * 4, true));
  if (offsets[0] !== HEADER_SIZE || offsets[1] === 0 || offsets[2] === 0 || offsets[5] === 0) {
    throw new Error("regional-map container lacks a required component");
  }
  let previous = HEADER_SIZE - 1;
  for (const offset of offsets) {
    if (offset === 0) continue;
    if (offset <= previous || offset >= size) throw new Error("regional-map component offsets are not ordered");
    previous = offset;
  }
  return offsets;
}

function componentEnd(offsets: readonly number[], slot: number, size: number): number {
  const start = offsets[slot];
  if (!start) throw new Error("regional-map component is absent");
  return offsets.slice(slot + 1).find((offset) => offset !== 0) ?? size;
}

function componentSources(directory: string, slot: number): string[] {
  const components = directory;
  if (slot === 0) return [`${components}_metatiles.tilemap`, `${components}_metatiles.lz.json`];
  if (slot === 1) return [`${components}_descriptors.json`, `${components}_descriptors.lz.json`];
  if (slot === 2) return [
    `${directory}_grid_grid.kind1.json`,
    ...["value_low.png", "value_high.png", "attribute_a.png", "attribute_b.png", "sentinels.png"]
      .map((name) => `${directory}_grid_${name}`),
  ];
  if (slot === 3) return [`${components}_animation_queues.json`, `${components}_animation_queues.lz.json`];
  if (slot === 4) return [`${components}_blend_animation.json`, `${components}_blend_animation.lz.json`];
  if (slot === 5) return [`${components}_sparse_cells.json`];
  throw new Error("unsupported regional-map component slot");
}

function checkedSources(directory: string, slot: number): string[] {
  // Flat layout: directory is a resource path prefix, not a directory. Confine
  // each source to the prefix's parent directory instead of realpathing it.
  const root = realpathSync(dirname(directory));
  return componentSources(directory, slot).map((source) => {
    const canonical = realpathSync(source);
    const position = relative(root, canonical);
    if (position !== relative(root, resolve(source))) {
      throw new Error("regional-map component source is not its canonical path");
    }
    return canonical;
  });
}

function validateGeneralTokens(value: unknown, label: string): void {
  for (const [index, item] of array(value, label).entries()) {
    const token = array(item, `${label}[${index}]`);
    if (token[0] === "l") {
      if (token.length !== 2) throw new Error(`${label}[${index}] has an invalid literal shape`);
      integer(token[1], `${label}[${index}] literal count`, 1);
    } else if (token[0] === "c") {
      if (token.length !== 3) throw new Error(`${label}[${index}] has an invalid copy shape`);
      integer(token[1], `${label}[${index}] copy length`, 2, 137);
      integer(token[2], `${label}[${index}] copy distance`, 1, 0x1020);
    } else throw new Error(`${label}[${index}] has an unsupported operation`);
  }
}

function validatePaletteTokens(value: unknown, label: string): void {
  let terminators = 0;
  for (const [groupIndex, item] of array(value, label).entries()) {
    const group = array(item, `${label}[${groupIndex}]`);
    if (group[0] === "z") {
      if (group.length !== 1) throw new Error(`${label}[${groupIndex}] has an invalid literal-block shape`);
      continue;
    }
    if (group.length !== 2 || group[0] !== "g") throw new Error(`${label}[${groupIndex}] has an invalid group shape`);
    const operations = array(group[1], `${label}[${groupIndex}] operations`);
    if (!operations.length || operations.length > 8) throw new Error(`${label}[${groupIndex}] must contain one through eight operations`);
    for (const [operationIndex, operationValue] of operations.entries()) {
      const operation = array(operationValue, `${label}[${groupIndex}][${operationIndex}]`);
      if (operation[0] === "l" || operation[0] === "e") {
        if (operation.length !== 1) throw new Error(`${label}[${groupIndex}][${operationIndex}] has an invalid operation shape`);
        if (operation[0] === "e") terminators++;
      } else if (operation[0] === "c") {
        if (operation.length !== 3) throw new Error(`${label}[${groupIndex}][${operationIndex}] has an invalid copy shape`);
        integer(operation[1], `${label}[${groupIndex}][${operationIndex}] copy length`, 2, 272);
        integer(operation[2], `${label}[${groupIndex}][${operationIndex}] copy distance`, 1, 0xfff);
      } else throw new Error(`${label}[${groupIndex}][${operationIndex}] has an unsupported operation`);
    }
  }
  if (terminators !== 1) throw new Error(`${label} must contain exactly one terminator`);
}

function validateLookahead(value: unknown, label: string): number {
  if (typeof value !== "string" || !/^(?:00){0,3}$/.test(value)) {
    throw new Error(`${label} must contain zero through three zero bytes`);
  }
  return value.length / 2;
}

function validateHeader(path: string): void {
  const value = document(path, "regional-map header");
  exactKeys(value, ["format", "parameters", "records", "component_offsets"], "regional-map header");
  if (value.format !== 1) throw new Error("regional-map header has an unsupported format");
  const parameters = array(value.parameters, "regional-map header parameters");
  if (parameters.length !== 12) throw new Error("regional-map header must contain twelve parameters");
  parameters.forEach((item, index) => integer(item, `regional-map header parameter ${index}`, 0, 0xff));
  const records = array(value.records, "regional-map header records");
  if (records.length !== 3) throw new Error("regional-map header must contain three records");
  records.forEach((record, recordIndex) => {
    const fields = array(record, `regional-map header record ${recordIndex}`);
    if (fields.length !== 4) throw new Error("regional-map header records must contain four fields");
    fields.forEach((item, field) => integer(item, `regional-map header record ${recordIndex} field ${field}`, 0, 0xffff));
  });
  const offsets = array(value.component_offsets, "regional-map header offsets");
  if (offsets.length !== 6) throw new Error("regional-map header must contain six offsets");
  offsets.forEach((item, index) => canonicalHex(item, `regional-map header offset ${index}`));
}

function validateCompressionPlan(
  path: string, expectedSize: number, component: string, extraKeys: readonly string[],
): JsonObject {
  const plan = document(path, `${component} plan`, true);
  const tagged = plan.codec === "golden-sun-tagged-palette-lz";
  const keys = [
    "format", "codec", ...(tagged ? ["tag"] : []), "decoded_size", "encoded_size",
    "tokens", "lookahead", "component", ...extraKeys,
  ];
  exactKeys(plan, keys, `${component} plan`);
  if (plan.format !== 1 || (!tagged && plan.codec !== "golden-sun-general-lz") ||
      (tagged && plan.tag !== 1) || plan.component !== component) {
    throw new Error(`${component} plan differs from its audited codec`);
  }
  integer(plan.decoded_size, `${component} decoded size`, 1);
  if (integer(plan.encoded_size, `${component} encoded size`, 1) !== expectedSize) {
    throw new Error(`${component} encoded size differs from its container span`);
  }
  validateLookahead(plan.lookahead, `${component} lookahead`);
  if (tagged) validatePaletteTokens(plan.tokens, `${component} tokens`);
  else validateGeneralTokens(plan.tokens, `${component} tokens`);
  return plan;
}

function validateByteRecords(value: unknown, width: number, label: string): unknown[][] {
  return array(value, label).map((record, recordIndex) => {
    const fields = array(record, `${label}[${recordIndex}]`);
    if (fields.length !== width) throw new Error(`${label}[${recordIndex}] has the wrong width`);
    fields.forEach((item, field) => integer(item, `${label}[${recordIndex}][${field}]`, 0, 0xff));
    return fields;
  });
}

function validateGridPng(path: string, sentinel: boolean): void {
  const encoded = readFileSync(path);
  const [width, height, pixels] = indexed_png(encoded);
  if (width !== 128 || height !== 128) throw new Error("regional-map grid source has the wrong dimensions");
  const canonical = sentinel ? mask_png(Buffer.from(pixels)) : byte_png(Buffer.from(pixels), 128)[0];
  if (!encoded.equals(canonical)) throw new Error("regional-map grid source is not a canonical PNG");
}

function validateComponent(directory: string, slot: number, expectedSize: number): void {
  const sources = checkedSources(directory, slot);
  if (slot === 0) {
    const tilemap = readFileSync(sources[0], "utf8");
    const raw = import_tilemap(tilemap);
    if (tilemap !== export_tilemap(raw, 4)) throw new Error("regional-map metatile source is not canonical text");
    const plan = validateCompressionPlan(sources[1], expectedSize, "map-metatiles-2x2", ["transform_mode", "metatiles"]);
    integer(plan.transform_mode, "metatile transform mode", 0, 2);
    const count = integer(plan.metatiles, "metatile count", 1);
    if (plan.decoded_size !== 1 + count * 8 || raw.length !== count * 8) {
      throw new Error("metatile decoded size differs from its count");
    }
    return;
  }
  if (slot === 1) {
    const source = document(sources[0], "regional-map descriptors");
    exactKeys(source, ["format", "record_size", "records"], "regional-map descriptors");
    if (source.format !== 1 || source.record_size !== 4) throw new Error("regional-map descriptor layout differs");
    const records = validateByteRecords(source.records, 4, "regional-map descriptor records");
    const plan = validateCompressionPlan(sources[1], expectedSize, "map-descriptors-4byte", ["records"]);
    if (integer(plan.records, "descriptor count") !== records.length || plan.decoded_size !== records.length * 4) {
      throw new Error("descriptor plan count differs from its source");
    }
    return;
  }
  if (slot === 2) {
    const plan = document(sources[0], "regional-map grid plan", true);
    exactKeys(plan, ["format", "codec", "decoded_size", "encoded_size", "tokens", "lookahead"], "regional-map grid plan");
    if (plan.format !== 1 || plan.codec !== "golden-sun-kind1-grid" || plan.decoded_size !== 0x10000 ||
        plan.encoded_size !== expectedSize) {
      throw new Error("regional-map grid plan differs from its audited layout");
    }
    validateLookahead(plan.lookahead, "regional-map grid lookahead");
    validatePaletteTokens(plan.tokens, "regional-map grid tokens");
    for (const path of sources.slice(1)) validateGridPng(path, basename(path).endsWith("_grid_sentinels.png"));
    return;
  }
  if (slot === 3) {
    const source = document(sources[0], "regional-map animation source");
    if ("queues" in source) {
      exactKeys(source, ["format", "terminators", "queues"], "regional-map animation source");
      if (source.format !== 1 || !Bun.deepEquals(source.terminators, ["0xfe00", "0xffff"], true)) {
        throw new Error("regional-map animation terminators differ");
      }
      const queues = array(source.queues, "regional-map animation queues");
      let commands = 0;
      queues.forEach((queueValue, queueIndex) => {
        const queue = object(queueValue, `regional-map animation queue ${queueIndex}`);
        exactKeys(queue, ["header", "commands"], `regional-map animation queue ${queueIndex}`);
        if (typeof queue.header !== "string" || !/^0xfd[0-9a-f]{2}$/.test(queue.header)) {
          throw new Error(`regional-map animation queue ${queueIndex} has an invalid header`);
        }
        const rows = array(queue.commands, `regional-map animation queue ${queueIndex} commands`);
        rows.forEach((row, rowIndex) => {
          const pair = array(row, `regional-map animation command ${queueIndex}:${rowIndex}`);
          if (pair.length !== 2) throw new Error("regional-map animation commands must contain two fields");
          pair.forEach((item, field) => integer(item, `regional-map animation command ${queueIndex}:${rowIndex}:${field}`, 0, 0xffff));
        });
        commands += rows.length;
      });
      const plan = validateCompressionPlan(sources[1], expectedSize, "map-animation-queues", ["queues", "commands"]);
      if (integer(plan.queues, "animation queue count") !== queues.length ||
          integer(plan.commands, "animation command count") !== commands ||
          plan.decoded_size !== 2 + queues.length * 4 + commands * 4) {
        throw new Error("animation queue plan differs from its source");
      }
    } else {
      exactKeys(source, ["format", "word_size", "words"], "regional-map animation source");
      const words = array(source.words, "regional-map animation words");
      words.forEach((item, index) => canonicalHex(item, `regional-map animation word ${index}`, 4));
      const plan = validateCompressionPlan(sources[1], expectedSize, "map-animation-words", ["words"]);
      if (source.format !== 1 || source.word_size !== 2 || integer(plan.words, "animation word count") !== words.length ||
          plan.decoded_size !== words.length * 2) {
        throw new Error("animation word plan differs from its source");
      }
    }
    return;
  }
  if (slot === 4) {
    const source = document(sources[0], "regional-map blend source");
    if ("commands" in source) {
      exactKeys(source, ["format", "word_size", "commands"], "regional-map blend source");
      const commands = array(source.commands, "regional-map blend commands");
      commands.forEach((commandValue, index) => {
        const command = object(commandValue, `regional-map blend command ${index}`);
        const op = command.op;
        const keys: Record<string, string[]> = {
          reset: ["op"], stop: ["op"], jump: ["op", "target_pair"],
          set_blend_control: ["op", "value"], write_blend_value: ["op", "value", "duration"],
        };
        if (typeof op !== "string" || keys[op] === undefined) throw new Error(`regional-map blend command ${index} has an invalid operation`);
        exactKeys(command, keys[op], `regional-map blend command ${index}`);
        if (op === "jump") integer(command.target_pair, `regional-map blend command ${index} target`, 0, 0xfe);
        else if (op === "set_blend_control") canonicalHex(command.value, `regional-map blend command ${index} value`, 4);
        else if (op === "write_blend_value") {
          canonicalHex(command.value, `regional-map blend command ${index} value`, 4);
          integer(command.duration, `regional-map blend command ${index} duration`, 0, 0xffff);
        }
      });
      const plan = validateCompressionPlan(sources[1], expectedSize, "map-blend-animation", ["commands"]);
      if (source.format !== 1 || source.word_size !== 2 || integer(plan.commands, "blend command count") !== commands.length) {
        throw new Error("blend plan differs from its source");
      }
    } else {
      exactKeys(source, ["format", "word_size", "words"], "regional-map blend source");
      const words = array(source.words, "regional-map blend words");
      words.forEach((item, index) => canonicalHex(item, `regional-map blend word ${index}`, 4));
      const plan = validateCompressionPlan(sources[1], expectedSize, "map-blend-words", ["words"]);
      if (source.format !== 1 || source.word_size !== 2 || integer(plan.words, "blend word count") !== words.length ||
          plan.decoded_size !== words.length * 2) {
        throw new Error("blend word plan differs from its source");
      }
    }
    return;
  }
  const source = document(sources[0], "regional-map sparse cells");
  exactKeys(source, ["format", "record_size", "terminator", "alignment_zeros", "records"], "regional-map sparse cells");
  if (source.format !== 1 || source.record_size !== 3 || source.terminator !== "ffffff") {
    throw new Error("regional-map sparse-cell layout differs");
  }
  const records = validateByteRecords(source.records, 3, "regional-map sparse-cell records");
  const alignment = integer(source.alignment_zeros, "regional-map sparse-cell alignment", 0, 3);
  if (records.length * 3 + 3 + alignment !== expectedSize) throw new Error("regional-map sparse-cell extent differs");
}

function buildComponent(directory: string, slot: number, expectedSize: number): Buffer {
  const sources = checkedSources(directory, slot);
  validateComponent(directory, slot, expectedSize);
  if (slot === 0) return build_metatiles(sources[0], sources[1]);
  if (slot === 1) return build_descriptors(sources[0], sources[1]);
  if (slot === 2) return build_grid(JSON.parse(readFileSync(sources[0], "utf8")), directory);
  if (slot === 3) return build_queues(sources[0], sources[1]);
  if (slot === 4) return build_blend_animation(sources[0], sources[1]);
  return build_sparse(sources[0]);
}

function buildResource(entry: ResourceEntry, spec: ResourceSpec, root: string, seriesPrefix = ""): BuiltChiikiMap {
  const directory = confined(root, entry.directory, seriesPrefix);
  const headerSource = `${directory}_header.json`;
  const headerPath = realpathSync(headerSource);
  if (headerPath !== resolve(headerSource)) throw new Error("regional-map header is not at its canonical path");
  validateHeader(headerPath);
  const header = build_header(headerPath);
  const offsets = componentOffsets(header, spec.size);
  const parts = [header];
  const sources = [headerPath];
  for (let slot = 0; slot < offsets.length; slot++) {
    if (!offsets[slot]) continue;
    const expected = componentEnd(offsets, slot, spec.size) - offsets[slot];
    const built = buildComponent(directory, slot, expected);
    if (built.length !== expected) throw new Error(`resource ${entry.id} component ${slot} has the wrong size`);
    parts.push(built);
    sources.push(...checkedSources(directory, slot));
  }
  const data = Buffer.concat(parts);
  if (data.length !== spec.size) throw new Error(`resource ${entry.id} has the wrong rebuilt size`);
  return { id: spec.id, address: spec.address, data, sources };
}

export function build_chiiki_map_series(indexPath: string): BuiltChiikiMap[] {
  const [index, specs] = parseIndex(indexPath);
  const root = dirname(realpathSync(indexPath));
  const seriesPrefix = basename(indexPath).replace(/index\.json$/, "");
  return index.resources.map((entry, position) => buildResource(entry, specs[position], root, seriesPrefix));
}

function exportComponent(container: Buffer, directory: string, offsets: readonly number[], slot: number): void {
  const start = offsets[slot];
  const encoded = container.subarray(start, componentEnd(offsets, slot, container.length));
  const sources = componentSources(directory, slot);
  if (slot === 0) export_metatiles(encoded, sources[0], sources[1]);
  else if (slot === 1) export_descriptors(encoded, sources[0], sources[1]);
  else if (slot === 2) export_grid(encoded, directory);
  else if (slot === 3) export_queues(encoded, sources[0], sources[1]);
  else if (slot === 4) export_blend_animation(encoded, sources[0], sources[1]);
  else export_sparse(encoded, sources[0]);
}

function resourcePointer(rom: Buffer, id: number): number {
  const offset = RESOURCE_TABLE - ROM_BASE + id * 4;
  if (offset < 0 || offset + 4 > rom.length) throw new Error("resource table is outside the ROM");
  return rom.readUInt32LE(offset);
}

function replaceDirectory<T>(directory: string, prefix: string, write: (staged: string) => T): T {
  const destination = resolve(directory), parent = dirname(destination);
  if (destination === parent) throw new Error("regional-map export requires a dedicated directory");
  mkdirSync(parent, { recursive: true });
  const transaction = mkdtempSync(join(parent, `.${prefix}-`));
  const staged = join(transaction, "new"), previous = join(transaction, "previous");
  let installed = false;
  try {
    const result = write(staged);
    if (existsSync(destination)) renameSync(destination, previous);
    try {
      renameSync(staged, destination);
      installed = true;
    } catch (error) {
      if (existsSync(previous)) renameSync(previous, destination);
      throw error;
    }
    return result;
  } finally {
    if (!installed && existsSync(previous) && !existsSync(destination)) renameSync(previous, destination);
    rmSync(transaction, { recursive: true, force: true });
  }
}

function writeChiikiMapSeries(rom: Buffer, directory: string): IndexDocument {
  if (rom.length !== ROM_SIZE) throw new Error("regional-map exporter requires the 8 MiB canonical ROM");
  mkdirSync(directory, { recursive: true });
  const resources: ResourceEntry[] = [];
  for (const spec of CHIIKI_MAP_RESOURCES) {
    if (resourcePointer(rom, spec.id) !== spec.address || resourcePointer(rom, spec.id + 1) !== spec.address + spec.size) {
      throw new Error(`resource ${idText(spec.id)} differs from the audited directory bounds`);
    }
    const start = spec.address - ROM_BASE;
    const container = rom.subarray(start, start + spec.size);
    if (container.length !== spec.size) throw new Error(`resource ${idText(spec.id)} is outside the ROM`);
    const name = `resource_${idText(spec.id)}`;
    const resourceDirectory = join(directory, name);
    const components = resourceDirectory;
    mkdirSync(components, { recursive: true });
    const offsets = export_header(container, `${components}_header.json`);
    componentOffsets(container.subarray(0, HEADER_SIZE), spec.size);
    for (let slot = 0; slot < offsets.length; slot++) {
      if (offsets[slot]) exportComponent(container, resourceDirectory, offsets, slot);
    }
    resources.push({ id: idText(spec.id), address: hex(spec.address), size: hex(spec.size, 1), directory: name });
  }
  const index: IndexDocument = { format: 1, kind: "golden-sun-chiiki-map-series", resources };
  const indexPath = `${directory}_index.json`;
  writeFileSync(indexPath, `${canonicalJson(index)}\n`);
  const built = build_chiiki_map_series(indexPath);
  built.forEach((item, position) => {
    const spec = CHIIKI_MAP_RESOURCES[position];
    const original = rom.subarray(spec.address - ROM_BASE, spec.address - ROM_BASE + spec.size);
    if (!item.data.equals(original)) throw new Error(`resource ${idText(item.id)} round trip differs`);
  });
  return index;
}

export function export_chiiki_map_series(romPath: string, directory: string): IndexDocument {
  validateExportDestination(romPath, directory);
  const rom = readFileSync(romPath);
  return replaceDirectory(directory, "chiiki-map-export", (staged) => writeChiikiMapSeries(rom, staged));
}

export function verify_chiiki_map_series(romPath: string, indexPath: string): void {
  const rom = readFileSync(romPath);
  if (rom.length !== ROM_SIZE) throw new Error("regional-map verifier requires the 8 MiB canonical ROM");
  const built = build_chiiki_map_series(indexPath);
  let bytes = 0;
  for (const item of built) {
    if (resourcePointer(rom, item.id) !== item.address || resourcePointer(rom, item.id + 1) !== item.address + item.data.length) {
      throw new Error(`resource ${idText(item.id)} directory bounds differ`);
    }
    const original = rom.subarray(item.address - ROM_BASE, item.address - ROM_BASE + item.data.length);
    if (!item.data.equals(original)) throw new Error(`resource ${idText(item.id)} differs from ROM`);
    bytes += item.data.length;
  }
  console.log(`identical=true resources=${built.length} source_bytes=${bytes}`);
}

export function self_test(): void {
  const header = Buffer.alloc(HEADER_SIZE);
  [0x3c, 0x80, 0xa0, 0x120, 0, 0x140].forEach((offset, slot) => header.writeUInt32LE(offset, 0x24 + slot * 4));
  const offsets = componentOffsets(header, 0x148);
  if (componentEnd(offsets, 3, 0x148) !== 0x140 || componentEnd(offsets, 5, 0x148) !== 0x148) {
    throw new Error("regional-map component extent self-test failed");
  }
  for (const test of [
    () => componentOffsets(Buffer.alloc(HEADER_SIZE), 0x100),
    () => validateLookahead("0000zz", "test lookahead"),
    () => validateGeneralTokens([["l", 1, "ignored"]], "test general tokens"),
    () => validatePaletteTokens([["g", [["e", "ignored"]]]], "test palette tokens"),
  ]) {
    let rejected = false;
    try { test(); } catch { rejected = true; }
    if (!rejected) throw new Error("regional-map strictness self-test failed");
  }
  const temporary = mkdtempSync(join(tmpdir(), "chiiki-map-self-test-"));
  try {
    const reject = (action: () => void): void => {
      let rejected = false;
      try { action(); } catch { rejected = true; }
      if (!rejected) throw new Error("regional-map adversarial self-test failed");
    };
    const duplicate = join(temporary, "duplicate.json");
    writeFileSync(duplicate, "{\"format\":0,\"format\":1}\n");
    reject(() => document(duplicate, "test document"));
    const hidden = join(temporary, "hidden.png");
    writeFileSync(hidden, Buffer.concat([byte_png(Buffer.alloc(128 * 128), 128)[0], Buffer.from("hidden")]));
    reject(() => validateGridPng(hidden, false));
    const destination = join(temporary, "destination");
    mkdirSync(destination);
    reject(() => validateExportDestination(join(destination, "rom.gba"), destination));
    const physicalRom = join(destination, "physical.gba");
    const romLink = join(temporary, "rom-link.gba");
    writeFileSync(physicalRom, "rom");
    symlinkSync(physicalRom, romLink);
    reject(() => validateExportDestination(romLink, destination));
    const unrelated = join(temporary, "unrelated");
    mkdirSync(unrelated);
    writeFileSync(join(unrelated, "keep"), "old");
    reject(() => validateExportDestination(join(temporary, "rom.gba"), unrelated));
    writeFileSync(join(destination, "stale"), "old");
    reject(() => replaceDirectory(destination, "chiiki-map-test", (staged) => {
      mkdirSync(staged);
      writeFileSync(join(staged, "partial"), "new");
      throw new Error("synthetic failure");
    }));
    if (!existsSync(join(destination, "stale")) || existsSync(join(destination, "partial"))) {
      throw new Error("failed regional-map export changed its destination");
    }
    replaceDirectory(destination, "chiiki-map-test", (staged) => {
      mkdirSync(staged);
      writeFileSync(join(staged, "current"), "new");
    });
    if (existsSync(join(destination, "stale")) || !existsSync(join(destination, "current"))) {
      throw new Error("regional-map export did not replace its whole destination");
    }
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok");
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { self_test(); return; }
  if (args.length === 1 && (args[0] === "-h" || args[0] === "--help")) {
    console.log("usage: chiiki_map_resources.ts export ROM --directory DIR | verify ROM --index INDEX | --self-test");
    return;
  }
  if (args.length === 4 && args[0] === "export" && args[2] === "--directory") {
    const index = export_chiiki_map_series(args[1], args[3]);
    const bytes = CHIIKI_MAP_RESOURCES.reduce((sum, item) => sum + item.size, 0);
    console.log(`resources=${index.resources.length} source_bytes=${bytes}`);
    return;
  }
  if (args.length === 4 && args[0] === "verify" && args[2] === "--index") {
    verify_chiiki_map_series(args[1], args[3]);
    return;
  }
  throw new Error("usage: chiiki_map_resources.ts export ROM --directory DIR | verify ROM --index INDEX | --self-test");
}

if (import.meta.main) main(Bun.argv.slice(2));
