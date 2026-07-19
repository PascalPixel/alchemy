#!/usr/bin/env bun
import {
  mkdirSync, readFileSync, realpathSync, writeFileSync,
} from "node:fs";
import { dirname, join, relative, resolve } from "node:path";
import { build_grid, export_grid } from "./kind1_map_grid.ts";
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

const ROM_BASE = 0x08000000;
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
  kind: "golden-sun-tokushu-map-series";
  resources: ResourceEntry[];
}

export interface BuiltTokushuMap {
  id: number;
  address: number;
  data: Buffer;
  sources: string[];
}

export const TOKUSHU_MAP_RESOURCES: readonly ResourceSpec[] = [
  { id: 0x1f3, address: 0x085cd0d8, size: 0x1e00 },
  { id: 0x1f6, address: 0x085d0188, size: 0x1d5c },
  { id: 0x28c, address: 0x086abcc0, size: 0x1f68 },
  { id: 0x2d4, address: 0x086f4ae8, size: 0x1b04 },
  { id: 0x307, address: 0x08735e84, size: 0x1b68 },
  { id: 0x331, address: 0x0876a00c, size: 0x0a1c },
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

function number(value: unknown, label: string): number {
  const parsed = typeof value === "string" ? Number(value) : value;
  if (!Number.isSafeInteger(parsed)) throw new Error(`${label} must be an integer`);
  return parsed as number;
}

function same(left: string, right: string): boolean {
  try { return realpathSync(left) === realpathSync(right); }
  catch { return resolve(left) === resolve(right); }
}

function confined(root: string, name: string): string {
  if (!/^resource_[0-9a-f]{3}$/.test(name)) throw new Error("special-map directory name is not canonical");
  const canonicalRoot = realpathSync(root);
  const path = realpathSync(resolve(root, name));
  const position = relative(canonicalRoot, path);
  if (position === ".." || position.startsWith("../") || position.startsWith("..\\")) {
    throw new Error("special-map source escaped its index directory");
  }
  return path;
}

function parseIndex(indexPath: string): [IndexDocument, ResourceSpec[]] {
  const value = JSON.parse(readFileSync(indexPath, "utf8"));
  if (typeof value !== "object" || value === null || Array.isArray(value)) {
    throw new Error("special-map index must be an object");
  }
  const index = value as IndexDocument;
  exactKeys(index as unknown as Record<string, unknown>, ["format", "kind", "resources"], "special-map index");
  if (index.format !== 1 || index.kind !== "golden-sun-tokushu-map-series" || !Array.isArray(index.resources)) {
    throw new Error("unsupported special-map index");
  }
  if (index.resources.length !== TOKUSHU_MAP_RESOURCES.length) {
    throw new Error("special-map index has a different resource count");
  }
  index.resources.forEach((entry, position) => {
    if (typeof entry !== "object" || entry === null || Array.isArray(entry)) {
      throw new Error("special-map resource entry must be an object");
    }
    exactKeys(entry as unknown as Record<string, unknown>, ["id", "address", "size", "directory"], "special-map resource entry");
    const spec = TOKUSHU_MAP_RESOURCES[position];
    if (entry.id !== idText(spec.id) || number(entry.address, "special-map address") !== spec.address ||
        number(entry.size, "special-map size") !== spec.size || entry.directory !== `resource_${idText(spec.id)}`) {
      throw new Error("special-map resource entry differs from the audited catalog");
    }
  });
  return [index, [...TOKUSHU_MAP_RESOURCES]];
}

function componentOffsets(header: Uint8Array, size: number): number[] {
  if (header.length !== HEADER_SIZE || size <= HEADER_SIZE) throw new Error("invalid special-map container extent");
  const view = new DataView(header.buffer, header.byteOffset, header.byteLength);
  const offsets = Array.from({ length: 6 }, (_, slot) => view.getUint32(0x24 + slot * 4, true));
  if (offsets[0] !== HEADER_SIZE || offsets[1] === 0 || offsets[2] === 0 || offsets[3] === 0 || offsets[5] === 0) {
    throw new Error("special-map container lacks a required component");
  }
  let previous = HEADER_SIZE - 1;
  for (const offset of offsets) {
    if (offset === 0) continue;
    if (offset <= previous || offset >= size) throw new Error("special-map component offsets are not ordered");
    previous = offset;
  }
  return offsets;
}

function componentEnd(offsets: readonly number[], slot: number, size: number): number {
  const start = offsets[slot];
  if (!start) throw new Error("special-map component is absent");
  return offsets.slice(slot + 1).find((offset) => offset !== 0) ?? size;
}

function componentSources(directory: string, slot: number): string[] {
  const components = join(directory, "components");
  if (slot === 0) return [join(components, "metatiles.tilemap"), join(components, "metatiles.lz.json")];
  if (slot === 1) return [join(components, "descriptors.json"), join(components, "descriptors.lz.json")];
  if (slot === 2) return [
    join(directory, "grid/grid.kind1.json"),
    ...["value_low.png", "value_high.png", "attribute_a.png", "attribute_b.png", "sentinels.png"]
      .map((name) => join(directory, "grid", name)),
  ];
  if (slot === 3) return [join(components, "animation_queues.json"), join(components, "animation_queues.lz.json")];
  if (slot === 4) return [join(components, "blend_animation.json"), join(components, "blend_animation.lz.json")];
  if (slot === 5) return [join(components, "sparse_cells.json")];
  throw new Error("unsupported special-map component slot");
}

function buildComponent(directory: string, slot: number): Buffer {
  const sources = componentSources(directory, slot);
  if (slot === 0) return build_metatiles(sources[0], sources[1]);
  if (slot === 1) return build_descriptors(sources[0], sources[1]);
  if (slot === 2) return build_grid(JSON.parse(readFileSync(sources[0], "utf8")), join(directory, "grid"));
  if (slot === 3) return build_queues(sources[0], sources[1]);
  if (slot === 4) return build_blend_animation(sources[0], sources[1]);
  return build_sparse(sources[0]);
}

function buildResource(entry: ResourceEntry, spec: ResourceSpec, root: string): BuiltTokushuMap {
  const directory = confined(root, entry.directory);
  const headerPath = join(directory, "components/header.json");
  const header = build_header(headerPath);
  const offsets = componentOffsets(header, spec.size);
  const parts = [header];
  const sources = [headerPath];
  for (let slot = 0; slot < offsets.length; slot++) {
    if (!offsets[slot]) continue;
    const built = buildComponent(directory, slot);
    const expected = componentEnd(offsets, slot, spec.size) - offsets[slot];
    if (built.length !== expected) throw new Error(`resource ${entry.id} component ${slot} has the wrong size`);
    parts.push(built);
    sources.push(...componentSources(directory, slot));
  }
  const data = Buffer.concat(parts);
  if (data.length !== spec.size) throw new Error(`resource ${entry.id} has the wrong rebuilt size`);
  return { id: spec.id, address: spec.address, data, sources };
}

export function build_tokushu_map_series(indexPath: string): BuiltTokushuMap[] {
  const [index, specs] = parseIndex(indexPath);
  const root = dirname(realpathSync(indexPath));
  return index.resources.map((entry, position) => buildResource(entry, specs[position], root));
}

function exportComponent(container: Buffer, directory: string, offsets: readonly number[], slot: number): void {
  const start = offsets[slot];
  const encoded = container.subarray(start, componentEnd(offsets, slot, container.length));
  const sources = componentSources(directory, slot);
  if (slot === 0) export_metatiles(encoded, sources[0], sources[1]);
  else if (slot === 1) export_descriptors(encoded, sources[0], sources[1]);
  else if (slot === 2) export_grid(encoded, join(directory, "grid"));
  else if (slot === 3) export_queues(encoded, sources[0], sources[1]);
  else if (slot === 4) export_blend_animation(encoded, sources[0], sources[1]);
  else export_sparse(encoded, sources[0]);
}

function resourcePointer(rom: Buffer, id: number): number {
  const offset = RESOURCE_TABLE - ROM_BASE + id * 4;
  if (offset < 0 || offset + 4 > rom.length) throw new Error("resource table is outside the ROM");
  return rom.readUInt32LE(offset);
}

export function export_tokushu_map_series(romPath: string, directory: string): IndexDocument {
  const rom = readFileSync(romPath);
  mkdirSync(directory, { recursive: true });
  const resources: ResourceEntry[] = [];
  for (const spec of TOKUSHU_MAP_RESOURCES) {
    if (resourcePointer(rom, spec.id) !== spec.address || resourcePointer(rom, spec.id + 1) !== spec.address + spec.size) {
      throw new Error(`resource ${idText(spec.id)} differs from the audited directory bounds`);
    }
    const start = spec.address - ROM_BASE;
    const container = rom.subarray(start, start + spec.size);
    if (container.length !== spec.size) throw new Error(`resource ${idText(spec.id)} is outside the ROM`);
    const name = `resource_${idText(spec.id)}`;
    const resourceDirectory = join(directory, name);
    const components = join(resourceDirectory, "components");
    mkdirSync(components, { recursive: true });
    const offsets = export_header(container, join(components, "header.json"));
    componentOffsets(container.subarray(0, HEADER_SIZE), spec.size);
    for (let slot = 0; slot < offsets.length; slot++) {
      if (offsets[slot]) exportComponent(container, resourceDirectory, offsets, slot);
    }
    resources.push({ id: idText(spec.id), address: hex(spec.address), size: hex(spec.size, 1), directory: name });
  }
  const index: IndexDocument = { format: 1, kind: "golden-sun-tokushu-map-series", resources };
  const indexPath = join(directory, "index.json");
  writeFileSync(indexPath, `${JSON.stringify(index, null, 2)}\n`);
  const built = build_tokushu_map_series(indexPath);
  built.forEach((item, position) => {
    const spec = TOKUSHU_MAP_RESOURCES[position];
    const original = rom.subarray(spec.address - ROM_BASE, spec.address - ROM_BASE + spec.size);
    if (!item.data.equals(original)) throw new Error(`resource ${idText(item.id)} round trip differs`);
  });
  return index;
}

export function verify_tokushu_map_series(romPath: string, indexPath: string): void {
  const rom = readFileSync(romPath);
  const built = build_tokushu_map_series(indexPath);
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
    throw new Error("special-map component extent self-test failed");
  }
  try {
    componentOffsets(Buffer.alloc(HEADER_SIZE), 0x100);
    throw new Error("empty special-map header was accepted");
  } catch (error) {
    if ((error as Error).message === "empty special-map header was accepted") throw error;
  }
  console.log("self-test=ok");
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || args[index + 1] === undefined) throw new Error(`${name} is required`);
  return args[index + 1];
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: tokushu_map_resources.ts [--self-test] {export ROM --directory DIR|verify ROM --index INDEX}");
    return;
  }
  if (args.includes("--self-test")) {
    self_test();
    args = args.filter((argument) => argument !== "--self-test");
    if (!args.length) return;
  }
  const command = args[0], romPath = args[1];
  if (!romPath || (command !== "export" && command !== "verify")) throw new Error("export or verify and a ROM are required");
  if (command === "export") {
    const directory = option(args, "--directory");
    if (same(romPath, directory)) throw new Error("refusing to overwrite the input ROM");
    const index = export_tokushu_map_series(romPath, directory);
    const bytes = TOKUSHU_MAP_RESOURCES.reduce((sum, item) => sum + item.size, 0);
    console.log(`resources=${index.resources.length} source_bytes=${bytes}`);
  } else verify_tokushu_map_series(romPath, option(args, "--index"));
}

if (import.meta.main) main(Bun.argv.slice(2));
