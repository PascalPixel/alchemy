#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, resolve } from "node:path";
import { build_static_sprite_series } from "./static_sprite_series.ts";
import { canonicalJson } from "./canonical_json.ts";

const ROM_BASE = 0x08000000;
export const CHARACTER_DESCRIPTOR_ADDRESS = 0x08185024;
export const CHARACTER_DESCRIPTOR_COUNT = 0x200;
export const CHARACTER_DESCRIPTOR_SIZE = 20;
export const CHARACTER_ANIMATION_ADDRESS = 0x08187824;
export const CHARACTER_ANIMATION_END = 0x0818d554;

const EXTRA_POINTER_TABLES = new Map<number, number>([
  [0x08188acc, 4],
  [0x081897ec, 5],
  [0x08189800, 5],
]);

type Command =
  | ["frame", number, number]
  | ["end", number]
  | ["set_draw", number]
  | ["hold", number]
  | ["jump", number]
  | ["select", number]
  | ["blank", number];

interface FrameDirectoryJson {
  name: string;
  address: string;
  ids: string[];
}

interface DescriptorJson {
  size: [number, number];
  scale: number;
  draw_kind: number;
  animation_count: number;
  adjust: [number, number];
  anchor: [number, number];
  frame_codec: 0 | 1 | 3;
  frames: string | null;
  animation: string;
}

interface LabelJson {
  name: string;
  command: number;
}

interface PointerTableJson {
  name: string;
  entries: string[];
}

interface AnimationGroupJson {
  name: string;
  ids: string[];
  commands: Command[];
  labels: LabelJson[];
  entries: string[];
  extra_tables?: PointerTableJson[];
}

export interface CharacterCatalogJson {
  format: 1;
  codec: "golden-sun-character-catalog";
  address: string;
  size: string;
  descriptor_count: number;
  descriptor_size: number;
  animation_address: string;
  frame_directories: FrameDirectoryJson[];
  descriptors: Record<string, DescriptorJson>;
  animation_groups: AnimationGroupJson[];
}

interface DescriptorRecord {
  id: number;
  offset: number;
  frameDirectory: number;
  animationDirectory: number;
  animationCount: number;
}

interface DirectoryGroup {
  address: number;
  count: number;
  ids: number[];
  name: string;
}

function number(value: unknown, name = "integer"): number {
  const parsed = typeof value === "string" ? Number(value) : value;
  if (!Number.isSafeInteger(parsed)) throw new Error(`invalid ${name}`);
  return parsed as number;
}

function bounded(value: unknown, minimum: number, maximum: number, name: string): number {
  const parsed = number(value, name);
  if (parsed < minimum || parsed > maximum) throw new Error(`invalid ${name}`);
  return parsed;
}

function text(value: unknown, name: string): string {
  if (typeof value !== "string" || value.length === 0) throw new Error(`invalid ${name}`);
  return value;
}

function hex(value: number, width = 8): string {
  return `0x${value.toString(16).padStart(width, "0")}`;
}

function id(value: number): string {
  return value.toString(16).padStart(3, "0");
}

function signedByte(value: number): number {
  return value < 0x80 ? value : value - 0x100;
}

function activeRecord(data: Uint8Array): boolean {
  return data.some((value) => value !== 0);
}

function decodeCommand(opcode: number, operand: number): Command {
  if (opcode < 0xef) return ["frame", opcode, operand];
  if (opcode === 0xef) return ["end", operand];
  if (opcode === 0xf0) return ["set_draw", operand];
  if (opcode === 0xf1) return ["hold", operand];
  if (opcode === 0xfd) return ["jump", operand];
  if (opcode === 0xfe) return ["select", operand];
  if (opcode === 0xff) return ["blank", operand];
  throw new Error(`unsupported animation opcode ${hex(opcode, 2)}`);
}

function encodeCommand(command: unknown): [number, number] {
  if (!Array.isArray(command) || command.length < 2) throw new Error("invalid animation command");
  const operation = text(command[0], "animation operation");
  if (operation === "frame") {
    if (command.length !== 3) throw new Error("invalid frame command");
    return [
      bounded(command[1], 0, 0xee, "frame number"),
      bounded(command[2], 0, 0xff, "frame time"),
    ];
  }
  if (command.length !== 2) throw new Error("invalid control command");
  const operand = bounded(command[1], 0, 0xff, "control operand");
  const opcode: Record<string, number> = {
    end: 0xef,
    set_draw: 0xf0,
    hold: 0xf1,
    jump: 0xfd,
    select: 0xfe,
    blank: 0xff,
  };
  if (!(operation in opcode)) throw new Error(`unknown animation operation ${operation}`);
  return [opcode[operation], operand];
}

function records(rom: Buffer): DescriptorRecord[] {
  const table = CHARACTER_DESCRIPTOR_ADDRESS - ROM_BASE;
  const end = table + CHARACTER_DESCRIPTOR_COUNT * CHARACTER_DESCRIPTOR_SIZE;
  if (table < 0 || end > rom.length) throw new Error("character descriptor table is outside the ROM");
  const output: DescriptorRecord[] = [];
  for (let recordId = 0; recordId < CHARACTER_DESCRIPTOR_COUNT; recordId++) {
    const offset = table + recordId * CHARACTER_DESCRIPTOR_SIZE;
    if (!activeRecord(rom.subarray(offset, offset + CHARACTER_DESCRIPTOR_SIZE))) continue;
    output.push({
      id: recordId,
      offset,
      frameDirectory: rom.readUInt32LE(offset + 12),
      animationDirectory: rom.readUInt32LE(offset + 16),
      animationCount: rom[offset + 5],
    });
  }
  return output;
}

function namedDirectories(
  source: DescriptorRecord[], field: "frameDirectory" | "animationDirectory",
): Map<number, { name: string; ids: number[] }> {
  const grouped = new Map<number, number[]>();
  for (const item of source) {
    const address = item[field];
    if (address === 0) continue;
    const ids = grouped.get(address) ?? [];
    ids.push(item.id);
    grouped.set(address, ids);
  }
  const result = new Map<number, { name: string; ids: number[] }>();
  for (const [address, ids] of grouped) {
    ids.sort((left, right) => left - right);
    result.set(address, {
      name: `${field === "frameDirectory" ? "chr" : "anm"}_${id(ids[0])}`,
      ids,
    });
  }
  return result;
}

function animationGroups(
  source: DescriptorRecord[], names: Map<number, { name: string; ids: number[] }>,
): DirectoryGroup[] {
  const grouped = new Map<number, DirectoryGroup>();
  for (const item of source) {
    if (item.animationDirectory === 0) throw new Error(`descriptor ${id(item.id)} lacks animation data`);
    const named = names.get(item.animationDirectory)!;
    const group = grouped.get(item.animationDirectory) ?? {
      address: item.animationDirectory,
      count: 0,
      ids: named.ids,
      name: named.name,
    };
    group.count = Math.max(group.count, item.animationCount);
    grouped.set(item.animationDirectory, group);
  }
  return [...grouped.values()].sort((left, right) => left.address - right.address);
}

function readPointerTable(rom: Buffer, address: number, count: number): number[] {
  const offset = address - ROM_BASE;
  if (offset < 0 || offset + count * 4 > rom.length) throw new Error("animation pointer table is outside the ROM");
  return Array.from({ length: count }, (_, index) => rom.readUInt32LE(offset + index * 4));
}

export function export_character_catalog(rom: Buffer): CharacterCatalogJson {
  const sourceRecords = records(rom);
  const frameNames = namedDirectories(sourceRecords, "frameDirectory");
  const animationNames = namedDirectories(sourceRecords, "animationDirectory");
  const groups = animationGroups(sourceRecords, animationNames);
  if (sourceRecords.length !== 367 || groups.length !== 364) {
    throw new Error("character catalog structure differs");
  }

  const descriptors: Record<string, DescriptorJson> = {};
  for (const item of sourceRecords) {
    const offset = item.offset;
    const codec = rom[offset + 10];
    if (codec !== 0 && codec !== 1 && codec !== 3) throw new Error("unsupported character frame codec");
    if (rom[offset + 11] !== 0) throw new Error("character descriptor reserve byte is not zero");
    descriptors[id(item.id)] = {
      size: [rom[offset], rom[offset + 1]],
      scale: rom.readUInt16LE(offset + 2),
      draw_kind: rom[offset + 4],
      animation_count: item.animationCount,
      adjust: [signedByte(rom[offset + 6]), signedByte(rom[offset + 7])],
      anchor: [signedByte(rom[offset + 8]), rom[offset + 9]],
      frame_codec: codec,
      frames: item.frameDirectory === 0 ? null : frameNames.get(item.frameDirectory)!.name,
      animation: animationNames.get(item.animationDirectory)!.name,
    };
  }

  const frameDirectories: FrameDirectoryJson[] = [...frameNames]
    .map(([address, named]) => ({
      name: named.name,
      address: hex(address),
      ids: named.ids.map(id),
    }))
    .sort((left, right) => number(left.address) - number(right.address));

  const outputGroups: AnimationGroupJson[] = [];
  let cursor = CHARACTER_ANIMATION_ADDRESS;
  for (const group of groups) {
    if (cursor > group.address || (group.address - cursor) % 2 !== 0) {
      throw new Error(`invalid animation command extent before ${group.name}`);
    }
    const commands: Command[] = [];
    for (let address = cursor; address < group.address; address += 2) {
      const offset = address - ROM_BASE;
      commands.push(decodeCommand(rom[offset], rom[offset + 1]));
    }
    const pointers = readPointerTable(rom, group.address, group.count);
    if (pointers.some((pointer) => pointer < cursor || pointer >= group.address || (pointer - cursor) % 2 !== 0)) {
      throw new Error(`animation table ${group.name} points outside its commands`);
    }
    const uniquePointers = [...new Set(pointers)].sort((left, right) => left - right);
    if (uniquePointers[0] !== cursor) throw new Error(`animation table ${group.name} leaves an unlabeled prefix`);
    const labelByAddress = new Map<number, string>();
    const labels = uniquePointers.map((address, index) => {
      const name = `${group.name}_${index.toString(16).padStart(3, "0")}`;
      labelByAddress.set(address, name);
      return { name, command: (address - cursor) / 2 };
    });
    const item: AnimationGroupJson = {
      name: group.name,
      ids: group.ids.map(id),
      commands,
      labels,
      entries: pointers.map((pointer) => labelByAddress.get(pointer)!),
    };
    cursor = group.address + group.count * 4;
    const extraTables: PointerTableJson[] = [];
    while (EXTRA_POINTER_TABLES.has(cursor)) {
      const count = EXTRA_POINTER_TABLES.get(cursor)!;
      const name = `anm_${cursor.toString(16).padStart(8, "0")}`;
      const pointers = readPointerTable(rom, cursor, count);
      if (pointers.some((pointer) => pointer !== cursor)) {
        throw new Error(`extra animation table ${name} is not self-referential`);
      }
      extraTables.push({ name, entries: pointers.map(() => name) });
      cursor += count * 4;
    }
    if (extraTables.length !== 0) item.extra_tables = extraTables;
    outputGroups.push(item);
  }
  if (cursor !== CHARACTER_ANIMATION_END) throw new Error("character animation extent differs");

  return {
    format: 1,
    codec: "golden-sun-character-catalog",
    address: hex(CHARACTER_DESCRIPTOR_ADDRESS),
    size: hex(CHARACTER_ANIMATION_END - CHARACTER_DESCRIPTOR_ADDRESS),
    descriptor_count: CHARACTER_DESCRIPTOR_COUNT,
    descriptor_size: CHARACTER_DESCRIPTOR_SIZE,
    animation_address: hex(CHARACTER_ANIMATION_ADDRESS),
    frame_directories: frameDirectories,
    descriptors,
    animation_groups: outputGroups,
  };
}

export function format_character_catalog(index: CharacterCatalogJson): string {
  return `${canonicalJson(index).replace(
    /^([ ]*)\[\n[ ]+"(frame|end|set_draw|hold|jump|select|blank)",\n[ ]+(\d+)(?:,\n[ ]+(\d+))?\n[ ]+\]/gm,
    (_match, indent: string, operation: string, first: string, second: string | undefined) =>
      `${indent}["${operation}", ${first}${second === undefined ? "" : `, ${second}`}]`,
  )}\n`;
}

function buildAnimations(index: CharacterCatalogJson): {
  data: Buffer;
  symbols: Map<string, number>;
  tableCounts: Map<string, number>;
} {
  const start = number(index.animation_address, "animation address");
  const chunks: Buffer[] = [];
  const symbols = new Map<string, number>();
  const tableCounts = new Map<string, number>();
  let cursor = start;

  const define = (name: string, address: number): void => {
    text(name, "animation symbol");
    if (symbols.has(name)) throw new Error(`duplicate animation symbol ${name}`);
    symbols.set(name, address);
  };

  for (const group of index.animation_groups) {
    const commandBytes = Buffer.alloc(group.commands.length * 2);
    group.commands.forEach((command, commandIndex) => {
      const [opcode, operand] = encodeCommand(command);
      commandBytes[commandIndex * 2] = opcode;
      commandBytes[commandIndex * 2 + 1] = operand;
    });
    const labelIndexes = new Set<number>();
    for (const label of group.labels) {
      // Tuple schema: [name, command].
      const [labelName, labelCommand] = label as [string, number];
      const command = bounded(labelCommand, 0, group.commands.length - 1, "animation label command");
      if (labelIndexes.has(command)) throw new Error(`duplicate command label in ${group.name}`);
      labelIndexes.add(command);
      define(labelName, cursor + command * 2);
    }
    chunks.push(commandBytes);
    cursor += commandBytes.length;

    define(group.name, cursor);
    tableCounts.set(group.name, group.entries.length);
    const table = Buffer.alloc(group.entries.length * 4);
    group.entries.forEach((entry, entryIndex) => {
      const address = symbols.get(text(entry, "animation entry"));
      if (address === undefined) throw new Error(`undefined animation entry ${entry}`);
      table.writeUInt32LE(address, entryIndex * 4);
    });
    chunks.push(table);
    cursor += table.length;

    for (const extra of group.extra_tables ?? []) {
      define(extra.name, cursor);
      const data = Buffer.alloc(extra.entries.length * 4);
      extra.entries.forEach((entry, entryIndex) => {
        const address = symbols.get(text(entry, "extra animation entry"));
        if (address === undefined) throw new Error(`undefined extra animation entry ${entry}`);
        data.writeUInt32LE(address, entryIndex * 4);
      });
      chunks.push(data);
      cursor += data.length;
    }
  }
  return { data: Buffer.concat(chunks), symbols, tableCounts };
}

function pair(value: unknown, minimum: number, maximum: number, name: string): [number, number] {
  if (!Array.isArray(value) || value.length !== 2) throw new Error(`invalid ${name}`);
  return [
    bounded(value[0], minimum, maximum, `${name} first value`),
    bounded(value[1], minimum, maximum, `${name} second value`),
  ];
}

export function build_character_catalog(index: CharacterCatalogJson): Buffer {
  if (index.format !== 1 || index.codec !== "golden-sun-character-catalog") {
    throw new Error("unsupported character catalog format");
  }
  const address = number(index.address, "catalog address");
  const size = number(index.size, "catalog size");
  const descriptorCount = number(index.descriptor_count, "descriptor count");
  const descriptorSize = number(index.descriptor_size, "descriptor size");
  const animationAddress = number(index.animation_address, "animation address");
  if (descriptorCount !== CHARACTER_DESCRIPTOR_COUNT || descriptorSize !== CHARACTER_DESCRIPTOR_SIZE ||
      animationAddress !== address + descriptorCount * descriptorSize) {
    throw new Error("character descriptor layout differs");
  }

  const frames = new Map<string, number>();
  for (const item of index.frame_directories) {
    // Tuple schema: [name, address, ids].
    const [itemName, itemAddress] = item as [string, number | string, unknown];
    const name = text(itemName, "frame directory name");
    if (frames.has(name)) throw new Error(`duplicate frame directory ${name}`);
    frames.set(name, bounded(itemAddress, ROM_BASE, 0xffffffff, "frame directory address"));
  }

  const animation = buildAnimations(index);
  if (animationAddress + animation.data.length !== address + size) {
    throw new Error("character animation size differs");
  }

  const descriptors = Buffer.alloc(descriptorCount * descriptorSize);
  const used = new Set<number>();
  for (const [key, item] of Object.entries(index.descriptors)) {
    if (!/^[0-9a-f]{3}$/.test(key)) throw new Error(`invalid descriptor id ${key}`);
    const recordId = Number.parseInt(key, 16);
    if (recordId >= descriptorCount || used.has(recordId)) throw new Error(`duplicate descriptor id ${key}`);
    used.add(recordId);
    const offset = recordId * descriptorSize;
    const sizePair = pair(item.size, 0, 0xff, "descriptor size");
    const adjust = pair(item.adjust, -0x80, 0x7f, "descriptor adjustment");
    const anchor = pair(item.anchor, -0x80, 0xff, "descriptor anchor");
    descriptors[offset] = sizePair[0];
    descriptors[offset + 1] = sizePair[1];
    descriptors.writeUInt16LE(bounded(item.scale, 0, 0xffff, "descriptor scale"), offset + 2);
    descriptors[offset + 4] = bounded(item.draw_kind, 0, 0xff, "descriptor draw kind");
    const animationCount = bounded(item.animation_count, 0, 0xff, "descriptor animation count");
    descriptors[offset + 5] = animationCount;
    descriptors.writeInt8(adjust[0], offset + 6);
    descriptors.writeInt8(adjust[1], offset + 7);
    descriptors.writeInt8(anchor[0], offset + 8);
    descriptors[offset + 9] = bounded(anchor[1], 0, 0xff, "descriptor anchor y");
    const codec = bounded(item.frame_codec, 0, 3, "descriptor frame codec");
    if (codec !== 0 && codec !== 1 && codec !== 3) throw new Error("unsupported descriptor frame codec");
    descriptors[offset + 10] = codec;
    if (item.frames !== null) {
      const frameAddress = frames.get(text(item.frames, "descriptor frame directory"));
      if (frameAddress === undefined) throw new Error(`undefined frame directory ${item.frames}`);
      descriptors.writeUInt32LE(frameAddress, offset + 12);
    }
    const animationName = text(item.animation, "descriptor animation table");
    const animationDirectory = animation.symbols.get(animationName);
    if (animationDirectory === undefined || !animation.tableCounts.has(animationName)) {
      throw new Error(`undefined animation table ${animationName}`);
    }
    if (animationCount > animation.tableCounts.get(animationName)!) {
      throw new Error(`descriptor ${key} exceeds its animation table`);
    }
    descriptors.writeUInt32LE(animationDirectory, offset + 16);
  }
  return Buffer.concat([descriptors, animation.data]);
}

export function load_character_catalog(path: string): CharacterCatalogJson {
  return JSON.parse(readFileSync(path, "utf8")) as CharacterCatalogJson;
}

export function verify_character_catalog(rom: Buffer, index: CharacterCatalogJson): void {
  const built = build_character_catalog(index);
  const address = number(index.address) - ROM_BASE;
  if (address < 0 || address + built.length > rom.length) throw new Error("character catalog is outside the ROM");
  if (!built.equals(rom.subarray(address, address + built.length))) {
    throw new Error("character catalog differs from the ROM");
  }
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || index + 1 >= args.length) throw new Error(`missing ${name}`);
  return args[index + 1];
}

function selfTest(): void {
  const commands: Command[] = [
    ["frame", 0xee, 0x55], ["end", 1], ["set_draw", 2], ["hold", 3],
    ["jump", 4], ["select", 5], ["blank", 6],
  ];
  for (const command of commands) {
    const encoded = encodeCommand(command);
    if (!Bun.deepEquals(decodeCommand(encoded[0], encoded[1]), command, true)) {
      throw new Error("character command self-test failed");
    }
  }
  console.log("self-test=ok");
}

if (import.meta.main) {
  const args = process.argv.slice(2);
  const command = args[0];
  if (command === "--self-test") {
    selfTest();
  } else if (command === "export") {
    const romPath = args[1];
    if (!romPath) throw new Error("missing ROM path");
    const output = resolve(option(args, "--output"));
    if (resolve(romPath) === output) throw new Error("refusing to overwrite the ROM");
    const catalog = export_character_catalog(readFileSync(romPath));
    mkdirSync(dirname(output), { recursive: true });
    writeFileSync(output, format_character_catalog(catalog));
    verify_character_catalog(readFileSync(romPath), catalog);
    console.log(`descriptors=${Object.keys(catalog.descriptors).length} groups=${catalog.animation_groups.length} bytes=${number(catalog.size)}`);
  } else if (command === "verify") {
    const romPath = args[1], indexPath = args[2];
    if (!romPath || !indexPath) throw new Error("usage: character_catalog.ts verify ROM INDEX");
    const catalog = load_character_catalog(indexPath);
    verify_character_catalog(readFileSync(romPath), catalog);
    console.log(`identical=true bytes=${number(catalog.size)}`);
  } else if (command === "verify-unit") {
    const romPath = args[1];
    if (!romPath) throw new Error("missing ROM path");
    const catalogPath = option(args, "--catalog");
    const seriesPath = option(args, "--series");
    const palettePath = option(args, "--palette");
    const catalog = load_character_catalog(catalogPath);
    const catalogData = build_character_catalog(catalog);
    const [seriesData] = build_static_sprite_series(seriesPath, palettePath);
    const series = JSON.parse(readFileSync(seriesPath, "utf8")) as { address: string; size: string };
    const catalogAddress = number(catalog.address);
    if (catalogAddress + catalogData.length !== number(series.address) || seriesData.length !== number(series.size)) {
      throw new Error("character unit extents are not adjacent");
    }
    const unit = Buffer.concat([catalogData, seriesData]);
    const offset = catalogAddress - ROM_BASE;
    const rom = readFileSync(romPath);
    if (!unit.equals(rom.subarray(offset, offset + unit.length))) throw new Error("character unit differs from the ROM");
    console.log(`identical=true descriptors=${Object.keys(catalog.descriptors).length} bytes=${unit.length}`);
  } else {
    throw new Error("usage: character_catalog.ts export ROM --output INDEX | verify ROM INDEX | verify-unit ROM --catalog INDEX --series INDEX --palette PNG | --self-test");
  }
}
