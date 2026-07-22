#!/usr/bin/env bun
// Tool role: entrypoint; invoked by package.json.
import {
  existsSync,
  mkdirSync,
  mkdtempSync,
  readFileSync,
  renameSync,
  rmSync,
  writeFileSync,
} from "fs";
import { dirname, join, resolve } from "path";
import { armSource, assembleAt } from "./sentou_kouka_runtime.ts";

export const ROM_BASE = 0x08000000;
const TMPDIR = Bun.env.TMPDIR ?? Bun.env.TMP ?? Bun.env.TEMP ?? "/tmp";
const JsonObject = Object as unknown as { keys(value: unknown): string[] };

type Json = Record<string, any>;

export interface ArmHelperSymbol {
  address: number;
  name: string;
}

export interface ArmHelperRegion {
  address: number;
  size: number;
  source: string;
  symbols: ArmHelperSymbol[];
  consumers: string[];
}

export interface ArmHelperLayout {
  address: number;
  end: number;
  size: number;
  regions: ArmHelperRegion[];
  source: Json;
}

export interface BuiltArmHelpers {
  address: number;
  end: number;
  data: Buffer;
  regions: Array<{ address: number; size: number; source: string; data: Buffer }>;
}

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function pretty(value: unknown): string {
  return `${JSON.stringify(value, null, 2)}\n`;
}

function exactKeys(value: unknown, keys: string[], name: string): asserts value is Json {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${name} must be an object`);
  const actual = JsonObject.keys(value).sort(), expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${name} has unknown or missing fields`);
  }
}

function canonicalDocument(path: string): Json {
  const text = readFileSync(path, "utf8"), value = JSON.parse(text);
  if (text !== pretty(value)) throw new Error(`${path}: source is not canonical JSON`);
  return value;
}

function integer(value: unknown, minimum: number, maximum: number, name: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < minimum || value > maximum) {
    throw new Error(`invalid ${name}`);
  }
  return value;
}

function address(value: unknown, name: string): number {
  if (typeof value !== "string" || !/^0x[0-9a-f]{8}$/.test(value)) throw new Error(`invalid ${name}`);
  return Number.parseInt(value.slice(2), 16);
}

function array(value: unknown, name: string): unknown[] {
  if (!Array.isArray(value)) throw new Error(`${name} must be an array`);
  return value;
}

function sourceName(value: unknown, addressValue: number, name: string): string {
  if (typeof value !== "string" || !/^[0-9a-f]{8}\.s$/.test(value) || value !== `${hexadecimal(addressValue).slice(2)}.s`) {
    throw new Error(`invalid ${name}`);
  }
  return value;
}

function symbolName(value: unknown, name: string): string {
  if (typeof value !== "string" || !/^(?:Func|Data|Value)_[0-9a-f]{8}$/.test(value)) throw new Error(`invalid ${name}`);
  return value;
}

export function parseArmHelperLayout(path: string): ArmHelperLayout {
  const source = canonicalDocument(path);
  exactKeys(source, ["format", "kind", "instruction_set", "address", "end", "size", "regions"], "ARM helper layout");
  if (source.format !== 1 || source.kind !== "golden-sun-relocated-arm-helpers" || source.instruction_set !== "armv4t") {
    throw new Error("ARM helper layout identity differs");
  }
  const start = address(source.address, "layout address"), end = address(source.end, "layout end");
  const size = integer(source.size, 4, 0x00800000, "layout size");
  if ((start & 3) !== 0 || (end & 3) !== 0 || start < ROM_BASE || end <= start || end - start !== size) {
    throw new Error("ARM helper layout extent differs");
  }
  const seenSources = new Set<string>(), seenSymbols = new Set<string>();
  const regions = array(source.regions, "ARM helper regions").map((item, index): ArmHelperRegion => {
    exactKeys(item, ["address", "size", "source", "symbols", "consumers"], `ARM helper region ${index}`);
    const regionAddress = address(item.address, `region ${index} address`);
    const regionSize = integer(item.size, 4, size, `region ${index} size`);
    if ((regionAddress & 3) !== 0 || (regionSize & 3) !== 0) throw new Error(`ARM helper region ${index} is not word aligned`);
    const sourceFile = sourceName(item.source, regionAddress, `region ${index} source`);
    if (seenSources.has(sourceFile)) throw new Error(`duplicate ARM helper source ${sourceFile}`);
    seenSources.add(sourceFile);
    const symbols = array(item.symbols, `region ${index} symbols`).map((symbol, position): ArmHelperSymbol => {
      exactKeys(symbol, ["address", "name"], `region ${index} symbol ${position}`);
      const symbolAddress = address(symbol.address, `region ${index} symbol ${position} address`);
      const name = symbolName(symbol.name, `region ${index} symbol ${position} name`);
      if (symbolAddress < regionAddress || symbolAddress >= regionAddress + regionSize || (symbolAddress & 3) !== 0) {
        throw new Error(`region ${index} symbol ${position} is outside its region`);
      }
      if (seenSymbols.has(name)) throw new Error(`duplicate ARM helper symbol ${name}`);
      seenSymbols.add(name);
      return { address: symbolAddress, name };
    });
    if (symbols.length === 0 || symbols[0].address !== regionAddress) throw new Error(`region ${index} has no entry symbol`);
    for (let position = 1; position < symbols.length; position++) {
      if (symbols[position - 1].address >= symbols[position].address) throw new Error(`region ${index} symbols are not ordered`);
    }
    const consumers = array(item.consumers, `region ${index} consumers`).map((consumer, position) =>
      symbolName(consumer, `region ${index} consumer ${position}`));
    if (new Set(consumers).size !== consumers.length) throw new Error(`region ${index} has duplicate consumers`);
    return { address: regionAddress, size: regionSize, source: sourceFile, symbols, consumers };
  });
  if (regions.length === 0) throw new Error("ARM helper layout has no regions");
  let cursor = start;
  for (const [index, region] of regions.entries()) {
    if (region.address !== cursor) throw new Error(`ARM helper region ${index} is not contiguous`);
    cursor += region.size;
  }
  if (cursor !== end) throw new Error("ARM helper regions do not fill the layout");
  return { address: start, end, size, regions, source };
}

function packageSource(indexPath: string, source: string): string {
  const path = resolve(dirname(indexPath), source);
  if (dirname(path) !== resolve(dirname(indexPath))) throw new Error("ARM helper source escaped its package");
  return path;
}

export function buildRelocatedArmHelpers(indexPath: string): BuiltArmHelpers {
  const layout = parseArmHelperLayout(indexPath);
  const regions = layout.regions.map((region) => {
    const source = packageSource(indexPath, region.source);
    const symbols = region.symbols.map((symbol) => symbol.name);
    const data = assembleAt(source, region.address, region.size, symbols, true, true);
    return { address: region.address, size: region.size, source, data };
  });
  const data = Buffer.concat(regions.map((region) => region.data));
  if (data.length !== layout.size) throw new Error("ARM helper package size differs");
  return { address: layout.address, end: layout.end, data, regions };
}

function writePackage(rom: Buffer, layoutPath: string, directory: string): void {
  const layout = parseArmHelperLayout(layoutPath);
  const offset = layout.address - ROM_BASE, end = layout.end - ROM_BASE;
  if (offset < 0 || end > rom.length) throw new Error("ROM is too small for the ARM helper layout");
  mkdirSync(directory, { recursive: true });
  writeFileSync(join(directory, "index.json"), pretty(layout.source));
  for (const region of layout.regions) {
    const data = rom.subarray(region.address - ROM_BASE, region.address - ROM_BASE + region.size);
    const names = new Map(region.symbols.map((symbol) => [symbol.address, symbol.name]));
    writeFileSync(join(directory, region.source), armSource(data, region.address, region.address + region.size, names));
  }
  const rebuilt = buildRelocatedArmHelpers(join(directory, "index.json"));
  const expected = rom.subarray(offset, end);
  if (!rebuilt.data.equals(expected)) {
    let mismatch = 0;
    while (mismatch < rebuilt.data.length && rebuilt.data[mismatch] === expected[mismatch]) mismatch++;
    throw new Error(`exported ARM helper package differs at ${hexadecimal(layout.address + mismatch)}`);
  }
}

export function exportRelocatedArmHelpers(rom: Buffer, layoutPath: string, directory: string): void {
  const destination = resolve(directory), parent = dirname(destination);
  if (destination === parent) throw new Error("ARM helper export requires a dedicated directory");
  if (existsSync(destination)) parseArmHelperLayout(join(destination, "index.json"));
  mkdirSync(parent, { recursive: true });
  const transaction = mkdtempSync(join(parent, ".arm-helper-export-"));
  const staged = join(transaction, "new"), previous = join(transaction, "previous");
  let installed = false;
  try {
    writePackage(rom, layoutPath, staged);
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

export function verifyRelocatedArmHelpers(rom: Buffer, indexPath: string): BuiltArmHelpers {
  const built = buildRelocatedArmHelpers(indexPath);
  const expected = rom.subarray(built.address - ROM_BASE, built.end - ROM_BASE);
  if (expected.length !== built.data.length || !built.data.equals(expected)) {
    let offset = 0;
    while (offset < expected.length && offset < built.data.length && expected[offset] === built.data[offset]) offset++;
    throw new Error(`ARM helper package differs at ${hexadecimal(built.address + offset)}`);
  }
  return built;
}

function syntheticRom(layout: ArmHelperLayout): Buffer {
  const rom = Buffer.alloc(layout.end - ROM_BASE);
  for (const region of layout.regions) {
    const data = rom.subarray(region.address - ROM_BASE, region.address - ROM_BASE + region.size);
    for (let offset = 0; offset < data.length; offset += 4) data.writeUInt32LE(0xe1a00000, offset);
    data.writeUInt32LE(0xe3a00000, 0);
    data.writeUInt32LE(0xea000000, 4);
    data.writeUInt32LE(0xe3a01001, 8);
    data.writeUInt32LE(0xe12fff1e, 12);
    data.writeUInt32LE(0xffffffff, data.length - 4);
  }
  return rom;
}

function reject(action: () => void): boolean {
  try { action(); } catch { return true; }
  return false;
}

export function selfTest(layoutPath = resolve(dirname(dirname(Bun.fileURLToPath(import.meta.url))), "assets/data/saihouchi_arm.json")): void {
  const layout = parseArmHelperLayout(layoutPath);
  if (layout.address !== 0x08015430 || layout.end !== 0x08015e8c || layout.size !== 2652 || layout.regions.length !== 7 ||
      layout.regions.reduce((sum, region) => sum + region.size, 0) !== 2652) {
    throw new Error("canonical ARM helper layout differs");
  }
  const temporary = mkdtempSync(join(TMPDIR, "arm-helper-self-test-"));
  try {
    const rom = syntheticRom(layout), output = join(temporary, "package");
    exportRelocatedArmHelpers(rom, layoutPath, output);
    const built = buildRelocatedArmHelpers(join(output, "index.json"));
    if (!built.data.equals(rom.subarray(layout.address - ROM_BASE, layout.end - ROM_BASE))) {
      throw new Error("synthetic ARM helper package differs");
    }
    verifyRelocatedArmHelpers(rom, join(output, "index.json"));
    const firstSource = join(output, layout.regions[0].source), original = readFileSync(firstSource, "utf8");
    writeFileSync(firstSource, `${original}.incbin "private.bin"\n`);
    if (!reject(() => buildRelocatedArmHelpers(join(output, "index.json")))) throw new Error("ARM helper builder accepted incbin");
    writeFileSync(firstSource, original);
    const bad = structuredClone(layout.source);
    bad.regions[0].source = "../private.s";
    const badPath = join(temporary, "bad.json");
    writeFileSync(badPath, pretty(bad));
    if (!reject(() => parseArmHelperLayout(badPath))) throw new Error("ARM helper layout accepted an escaped source");
    bad.regions[0].source = "08015430.s";
    bad.regions[1].address = "0x08015574";
    writeFileSync(badPath, pretty(bad));
    if (!reject(() => parseArmHelperLayout(badPath))) throw new Error("ARM helper layout accepted a gap");
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok regions=7 bytes=2652");
}

function usage(): never {
  console.log("usage: relocated_arm_helpers.ts export ROM LAYOUT --directory DIR | build INDEX -o OUTPUT | verify ROM INDEX | --self-test");
  process.exit(0);
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { selfTest(); return; }
  if (args[0] === "export" && args.length === 5 && args[3] === "--directory") {
    exportRelocatedArmHelpers(readFileSync(args[1]), args[2], args[4]);
    return;
  }
  if (args[0] === "build" && args.length === 4 && (args[2] === "-o" || args[2] === "--output")) {
    const built = buildRelocatedArmHelpers(args[1]);
    writeFileSync(args[3], built.data);
    console.log(`address=${hexadecimal(built.address)} bytes=${built.data.length}`);
    return;
  }
  if (args[0] === "verify" && args.length === 3) {
    const built = verifyRelocatedArmHelpers(readFileSync(args[1]), args[2]);
    console.log(`address=${hexadecimal(built.address)} bytes=${built.data.length} exact=true`);
    return;
  }
  if (args.includes("-h") || args.includes("--help")) usage();
  usage();
}

if (import.meta.main) main(process.argv.slice(2));
