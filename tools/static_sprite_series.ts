#!/usr/bin/env bun
import {
  existsSync, lstatSync, mkdirSync, mkdtempSync, readFileSync, readdirSync,
  realpathSync, rmSync, rmdirSync, symlinkSync, unlinkSync, writeFileSync,
} from "node:fs";
import { tmpdir } from "node:os";
import { dirname, isAbsolute, join, relative, resolve } from "node:path";
import { prune_files } from "./generated_files.ts";
import { indexed_png, type Rgb } from "./import_asset.ts";
import { png, read_palette } from "./skip_sprite_archive.ts";
import { decode_static_frame_trace, encode_static_frame_plan } from "./static_sprite_bank.ts";

const ROM_BASE = 0x08000000;
export const STATIC_DESCRIPTOR_TABLE = 0x08185024;
export const STATIC_SERIES_ADDRESS = 0x08244fc0;
export const STATIC_SERIES_END = 0x08287774;
export const STATIC_SERIES_SIZE = STATIC_SERIES_END - STATIC_SERIES_ADDRESS;
export const STATIC_PALETTE_OFFSET = 16;
export const STATIC_PALETTE_ENTRIES = 224;
const DESCRIPTOR_SIZE = 20;
export const STATIC_DESCRIPTOR_COUNT = 0x200;
const ATLAS_COLUMNS = 8;

type Json = Record<string, any>;

interface Descriptor {
  id: number;
  width: number;
  height: number;
  directory: number;
}

interface PackageLayout {
  ids: number[];
  width: number;
  height: number;
  address: number;
  directory: number;
  end: number;
  pointers: number[];
  uniquePointers: number[];
}

export interface StaticSpriteSeriesOptions {
  address: number;
  end: number;
  descriptorTable: number;
  descriptorCount: number;
  paletteOffset: number;
  paletteEntries: number;
}

function number(value: string | number): number {
  const parsed = typeof value === "string" ? Number(value) : value;
  if (!Number.isSafeInteger(parsed)) throw new Error("invalid static-sprite integer");
  return parsed;
}

function integer(value: number, minimum: number, maximum: number, name: string): void {
  if (!Number.isSafeInteger(value) || value < minimum || value > maximum) {
    throw new Error(`invalid ${name}`);
  }
}

function hex(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function same(left: string, right: string): boolean {
  try { return realpathSync(left) === realpathSync(right); } catch { return resolve(left) === resolve(right); }
}

function confined(root: string, name: string): string {
  const canonicalRoot = realpathSync(root);
  const path = realpathSync(resolve(root, name));
  const position = relative(canonicalRoot, path);
  if (position === ".." || position.startsWith("../") || position.startsWith("..\\") || isAbsolute(position)) {
    throw new Error("static-sprite source must stay beneath its index directory");
  }
  return path;
}

export function prune_static_sprite_packages(directory: string, keep: Iterable<string>): string[] {
  const expected = new Set(keep);
  const removed: string[] = [];
  if (!existsSync(directory)) return removed;
  for (const name of readdirSync(directory).sort()) {
    if (!/^chr_[0-9a-f]{3}$/.test(name) || expected.has(name)) continue;
    const path = join(directory, name);
    if (!lstatSync(path).isDirectory()) continue;
    const files = readdirSync(path).sort();
    if (files.some((file) => file !== "bank.json" && !/^koma.*\.png$/.test(file))) continue;
    for (const file of files) unlinkSync(join(path, file));
    rmdirSync(path);
    removed.push(path);
  }
  return removed;
}

function selectedPalette(path: string, offset: number, entries: number): Rgb[] {
  integer(offset, 0, 255, "static-sprite palette offset");
  integer(entries, 1, 256 - offset, "static-sprite palette entries");
  return read_palette(path, offset, entries);
}

function seriesExtent(address: number, size: number): [number, number] {
  if (!Number.isSafeInteger(address) || !Number.isSafeInteger(size) ||
      address < ROM_BASE || size <= 0 || address + size > 0x100000000) {
    throw new Error("invalid static-sprite series extent");
  }
  return [address, address + size];
}

function descriptorPackages(
  rom: Buffer, seriesAddress: number, seriesEnd: number,
  descriptorTable: number, descriptorCount: number,
): PackageLayout[] {
  integer(descriptorTable, ROM_BASE, ROM_BASE + rom.length, "static-sprite descriptor table");
  integer(descriptorCount, 1, 0x10000, "static-sprite descriptor count");
  const table = descriptorTable - ROM_BASE;
  if (table + DESCRIPTOR_SIZE * descriptorCount > rom.length) {
    throw new Error("static-sprite descriptor table is outside the ROM");
  }
  const byDirectory = new Map<number, Descriptor[]>();
  for (let id = 0; id < descriptorCount; id++) {
    const offset = table + id * DESCRIPTOR_SIZE;
    const directory = rom.readUInt32LE(offset + 12);
    if (directory < seriesAddress || directory >= seriesEnd) continue;
    if (rom[offset + 10] !== 3) throw new Error(`resource ${id.toString(16)} is not mode 3`);
    const descriptor: Descriptor = { id, width: rom[offset], height: rom[offset + 1], directory };
    const aliases = byDirectory.get(directory) ?? [];
    aliases.push(descriptor);
    byDirectory.set(directory, aliases);
  }
  const directories = [...byDirectory.keys()].sort((left, right) => left - right);
  if (directories.length === 0) throw new Error("static-sprite series has no packages");
  let end = seriesEnd;
  const packages: PackageLayout[] = [];
  for (let index = directories.length - 1; index >= 0; index--) {
    const directory = directories[index];
    if (directory >= end || (end - directory) % 4 !== 0) throw new Error("invalid sprite directory extent");
    const words: number[] = [];
    for (let address = directory; address < end; address += 4) {
      words.push(rom.readUInt32LE(address - ROM_BASE));
    }
    if (words.length < 2 || words.at(-1) !== 0 || words.slice(0, -1).some((value) => value === 0)) {
      throw new Error(`sprite directory ${hex(directory)} lacks its sole zero terminator`);
    }
    const pointers = words.slice(0, -1);
    const uniquePointers = [...new Set(pointers)].sort((left, right) => left - right);
    if (uniquePointers.some((pointer) => pointer < seriesAddress || pointer >= directory)) {
      throw new Error(`sprite directory ${hex(directory)} contains an invalid pointer`);
    }
    const address = uniquePointers[0];
    const descriptors = byDirectory.get(directory)!;
    if (descriptors.some((item) => item.width !== descriptors[0].width || item.height !== descriptors[0].height)) {
      throw new Error(`sprite aliases at ${hex(directory)} disagree on dimensions`);
    }
    if (!descriptors[0].width || !descriptors[0].height || descriptors[0].width % 8 || descriptors[0].height % 8) {
      throw new Error(`sprite package ${hex(directory)} has invalid dimensions`);
    }
    packages.push({
      ids: descriptors.map((item) => item.id).sort((left, right) => left - right),
      width: descriptors[0].width,
      height: descriptors[0].height,
      address,
      directory,
      end,
      pointers,
      uniquePointers,
    });
    end = address;
  }
  packages.reverse();
  const prefix = rom.subarray(seriesAddress - ROM_BASE, end - ROM_BASE);
  if (prefix.some((byte) => byte !== 0)) throw new Error("static-sprite prefix is not zero alignment");
  return packages;
}

function makeAtlas(frames: readonly Uint8Array[], width: number, height: number, palette: readonly Rgb[]): Buffer {
  if (frames.length === 0) throw new Error("sprite atlas cannot be empty");
  const columns = Math.min(ATLAS_COLUMNS, frames.length);
  const rows = Math.ceil(frames.length / columns);
  const atlasWidth = columns * width;
  const atlas = Buffer.alloc(atlasWidth * rows * height);
  frames.forEach((frame, frameIndex) => {
    if (frame.length !== width * height) throw new Error("decoded sprite frame has the wrong size");
    const cellLeft = frameIndex % columns * width;
    const cellTop = Math.floor(frameIndex / columns) * height;
    for (let y = 0; y < height; y++) {
      atlas.set(frame.subarray(y * width, (y + 1) * width), (cellTop + y) * atlasWidth + cellLeft);
    }
  });
  return png(atlas, atlasWidth, rows * height, [...palette]);
}

function atlasFrames(image: Uint8Array, plan: Json, palette: readonly Rgb[]): Buffer[] {
  const [width, height, sourcePixels, actualPalette] = indexed_png(image);
  const pixels = Buffer.from(sourcePixels);
  if (!Bun.deepEquals(actualPalette, palette, true)) {
    throw new Error("sprite atlas does not use the shared OBJ palette");
  }
  const frameWidth = number(plan.width), frameHeight = number(plan.height);
  const frameCount = plan.frames.length, columns = number(plan.atlas_columns);
  const rows = Math.ceil(frameCount / columns);
  if (columns !== Math.min(ATLAS_COLUMNS, frameCount) || width !== columns * frameWidth || height !== rows * frameHeight) {
    throw new Error("sprite atlas dimensions differ from its plan");
  }
  const frames: Buffer[] = [];
  for (let frameIndex = 0; frameIndex < frameCount; frameIndex++) {
    const frame = Buffer.alloc(frameWidth * frameHeight);
    const cellLeft = frameIndex % columns * frameWidth;
    const cellTop = Math.floor(frameIndex / columns) * frameHeight;
    for (let y = 0; y < frameHeight; y++) {
      const source = (cellTop + y) * width + cellLeft;
      frame.set(pixels.subarray(source, source + frameWidth), y * frameWidth);
    }
    frames.push(frame);
  }
  const cells = columns * rows;
  for (let frameIndex = frameCount; frameIndex < cells; frameIndex++) {
    const cellLeft = frameIndex % columns * frameWidth;
    const cellTop = Math.floor(frameIndex / columns) * frameHeight;
    for (let y = 0; y < frameHeight; y++) {
      const source = (cellTop + y) * width + cellLeft;
      if (pixels.subarray(source, source + frameWidth).some((byte) => byte !== 0)) {
        throw new Error("sprite atlas padding must be transparent");
      }
    }
  }
  return frames;
}

export function build_static_sprite_series(indexPath: string, palettePath: string): [Buffer, Json] {
  const index = JSON.parse(readFileSync(indexPath, "utf8"));
  if (index.format !== 1 || index.codec !== "golden-sun-static-sprite-series") {
    throw new Error("unsupported static-sprite series index");
  }
  const base = number(index.address), expectedSize = number(index.size);
  const prefix = number(index.prefix_zeros);
  seriesExtent(base, expectedSize);
  const descriptorTable = number(index.descriptor_table), descriptorCount = number(index.descriptor_count);
  const paletteOffset = number(index.palette_offset), paletteEntries = number(index.palette_entries);
  if (descriptorTable < ROM_BASE || descriptorTable > 0xffffffff ||
      descriptorCount < 1 || descriptorCount > 0x10000 ||
      paletteOffset < 0 || paletteOffset > 255 ||
      paletteEntries < 1 || paletteOffset + paletteEntries > 256 ||
      prefix < 0 || prefix > expectedSize || !Array.isArray(index.packages) || index.packages.length === 0) {
    throw new Error("invalid static-sprite series index");
  }
  const palette = selectedPalette(palettePath, paletteOffset, paletteEntries);
  let result = Buffer.alloc(prefix);
  let framesBuilt = 0, directoryEntries = 0;
  const root = dirname(indexPath);
  for (const item of index.packages) {
    const address = number(item.address), end = address + number(item.size);
    if (base + result.length !== address) throw new Error(`sprite package ${item.id} is not contiguous`);
    const planPath = confined(root, String(item.plan));
    const imagePath = confined(root, String(item.source));
    const plan = JSON.parse(readFileSync(planPath, "utf8"));
    if (plan.format !== 1 || plan.codec !== "golden-sun-static-sprite-bank" || !Array.isArray(plan.frames) || !Array.isArray(plan.directory)) {
      throw new Error(`unsupported sprite package plan: ${item.id}`);
    }
    const frames = atlasFrames(readFileSync(imagePath), plan, palette);
    const offsets: number[] = [];
    for (const [frameIndex, frame] of frames.entries()) {
      offsets.push(result.length);
      const encoded = encode_static_frame_plan(frame, plan.frames[frameIndex], result);
      result = Buffer.concat([result, encoded]);
    }
    const alignment = number(plan.alignment_zeros ?? 0);
    if (alignment < 0 || alignment > 3) throw new Error(`sprite package ${item.id} has invalid alignment`);
    result = Buffer.concat([result, Buffer.alloc(alignment)]);
    if (base + result.length !== number(item.directory)) throw new Error(`sprite package ${item.id} stream extent differs`);
    const directory = Buffer.alloc((plan.directory.length + 1) * 4);
    plan.directory.forEach((frameIndex: unknown, entry: number) => {
      const selected = number(frameIndex as number);
      if (selected < 0 || selected >= offsets.length) throw new Error(`sprite package ${item.id} has an invalid alias`);
      directory.writeUInt32LE(base + offsets[selected], entry * 4);
    });
    result = Buffer.concat([result, directory]);
    if (base + result.length !== end) throw new Error(`sprite package ${item.id} directory extent differs`);
    framesBuilt += frames.length;
    directoryEntries += plan.directory.length;
  }
  if (result.length !== expectedSize) throw new Error("static-sprite series size differs");
  return [result, { packages: index.packages.length, frames: framesBuilt, directory_entries: directoryEntries }];
}

export function export_static_sprite_series(
  rom: Buffer, directory: string, palettePath: string,
  options: Partial<StaticSpriteSeriesOptions> = {},
): Json {
  const seriesAddress = options.address ?? STATIC_SERIES_ADDRESS;
  const seriesEnd = options.end ?? STATIC_SERIES_END;
  const descriptorTable = options.descriptorTable ?? STATIC_DESCRIPTOR_TABLE;
  const descriptorCount = options.descriptorCount ?? STATIC_DESCRIPTOR_COUNT;
  const paletteOffset = options.paletteOffset ?? STATIC_PALETTE_OFFSET;
  const paletteEntries = options.paletteEntries ?? STATIC_PALETTE_ENTRIES;
  const [, checkedEnd] = seriesExtent(seriesAddress, seriesEnd - seriesAddress);
  if (checkedEnd - ROM_BASE > rom.length) throw new Error("ROM is too small for the static-sprite series");
  const packages = descriptorPackages(rom, seriesAddress, checkedEnd, descriptorTable, descriptorCount);
  const palette = selectedPalette(palettePath, paletteOffset, paletteEntries);
  const arena = rom.subarray(seriesAddress - ROM_BASE, checkedEnd - ROM_BASE);
  const prefix = packages[0].address - seriesAddress;
  const packageEntries: Json[] = [];
  for (const item of packages) {
    const id = item.ids[0].toString(16).padStart(3, "0");
    const resourceDirectory = join(directory, `chr_${id}`);
    const imagePath = join(resourceDirectory, "koma.8bpp.png");
    const planPath = join(resourceDirectory, "bank.json");
    const frames: Buffer[] = [];
    const plans: Json[] = [];
    let alignment = 0;
    for (const [frameIndex, pointer] of item.uniquePointers.entries()) {
      const trace = decode_static_frame_trace(arena, pointer - seriesAddress);
      const next = item.uniquePointers[frameIndex + 1] ?? item.directory;
      const gap = next - pointer - trace.encodedSize;
      if (frameIndex + 1 < item.uniquePointers.length) {
        if (gap !== 0) throw new Error(`resource ${id} frame ${frameIndex} does not fill its stream extent`);
      } else {
        if (gap < 0 || gap > 3 || arena.subarray(next - seriesAddress - gap, next - seriesAddress).some((byte) => byte !== 0)) {
          throw new Error(`resource ${id} has invalid directory alignment`);
        }
        alignment = gap;
      }
      if (trace.pixels.length !== item.width * item.height) throw new Error(`resource ${id} frame ${frameIndex} has the wrong dimensions`);
      frames.push(Buffer.from(trace.pixels));
      plans.push(trace.plan);
    }
    const pointerIndex = new Map(item.uniquePointers.map((pointer, index) => [pointer, index]));
    const plan = {
      format: 1,
      codec: "golden-sun-static-sprite-bank",
      width: item.width,
      height: item.height,
      atlas_columns: Math.min(ATLAS_COLUMNS, frames.length),
      alignment_zeros: alignment,
      frames: plans,
      directory: item.pointers.map((pointer) => pointerIndex.get(pointer)),
    };
    mkdirSync(resourceDirectory, { recursive: true });
    writeFileSync(imagePath, makeAtlas(frames, item.width, item.height, palette));
    writeFileSync(planPath, JSON.stringify(plan) + "\n");
    prune_files(resourceDirectory, "koma*.png", [imagePath]);
    const relativeRoot = `chr_${id}`;
    packageEntries.push({
      id,
      aliases: item.ids.slice(1).map((alias) => alias.toString(16)),
      address: hex(item.address),
      size: hex(item.end - item.address),
      directory: hex(item.directory),
      source: `${relativeRoot}/koma.8bpp.png`,
      plan: `${relativeRoot}/bank.json`,
    });
  }
  const index = {
    format: 1,
    codec: "golden-sun-static-sprite-series",
    address: hex(seriesAddress),
    size: hex(checkedEnd - seriesAddress),
    descriptor_table: hex(descriptorTable),
    descriptor_count: descriptorCount,
    palette_offset: paletteOffset,
    palette_entries: paletteEntries,
    prefix_zeros: prefix,
    packages: packageEntries,
  };
  const indexPath = join(directory, "index.json");
  writeFileSync(indexPath, JSON.stringify(index, null, 2) + "\n");
  const [rebuilt] = build_static_sprite_series(indexPath, palettePath);
  if (!rebuilt.equals(arena)) throw new Error("static-sprite series round trip differs");
  prune_static_sprite_packages(directory, packageEntries.map((item) => `chr_${item.id}`));
  return index;
}

export function selfTest(): void {
  try {
    number(1.5);
    throw new Error("fractional static-sprite integer was accepted");
  } catch (error) {
    if ((error as Error).message === "fractional static-sprite integer was accepted") throw error;
  }
  if (seriesExtent(0x08000100, 0x200)[1] !== 0x08000300) {
    throw new Error("static-sprite extent self-test failed");
  }
  const temporary = mkdtempSync(join(tmpdir(), "alchemy-static-series-"));
  try {
    const source = join(temporary, "source");
    mkdirSync(source);
    writeFileSync(join(temporary, "outside.png"), Buffer.alloc(1));
    symlinkSync("../outside.png", join(source, "escape.png"));
    try {
      confined(source, "escape.png");
      throw new Error("static-sprite symlink escape was accepted");
    } catch (error) {
      if ((error as Error).message === "static-sprite symlink escape was accepted") throw error;
    }
    for (const name of ["chr_001", "chr_002", "chr_003"]) mkdirSync(join(source, name));
    writeFileSync(join(source, "chr_001", "bank.json"), "{}\n");
    writeFileSync(join(source, "chr_002", "bank.json"), "{}\n");
    writeFileSync(join(source, "chr_002", "koma.8bpp.png"), Buffer.alloc(1));
    writeFileSync(join(source, "chr_003", "memo.txt"), "keep\n");
    const removed = prune_static_sprite_packages(source, ["chr_001"]);
    if (removed.length !== 1 || existsSync(join(source, "chr_002")) ||
        !existsSync(join(source, "chr_001")) || !existsSync(join(source, "chr_003"))) {
      throw new Error("static-sprite stale-package pruning self-test failed");
    }
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok");
}

function option(args: string[], ...names: string[]): string {
  const index = args.findIndex((argument) => names.includes(argument));
  if (index < 0 || index + 1 >= args.length) throw new Error(`${names.at(-1)} is required`);
  return args[index + 1];
}

function optional(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  if (index < 0) return undefined;
  if (index + 1 >= args.length) throw new Error(`${name} requires a value`);
  return args[index + 1];
}

function main(args: string[]): void {
  if (args.includes("--self-test")) {
    selfTest();
    return;
  }
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: static_sprite_series.ts export-series ROM --directory DIR --palette PNG [--address N --end N] [--descriptor-table N --descriptor-count N] [--palette-offset N --palette-entries N]");
    return;
  }
  if (args[0] !== "export-series" || !args[1]) throw new Error("a static-sprite series command and ROM are required");
  const romPath = args[1], directory = option(args, "--directory"), palette = option(args, "--palette");
  const addressText = optional(args, "--address"), endText = optional(args, "--end");
  if ((addressText === undefined) !== (endText === undefined)) throw new Error("--address and --end must be used together");
  const address = addressText === undefined ? STATIC_SERIES_ADDRESS : number(addressText);
  const end = endText === undefined ? STATIC_SERIES_END : number(endText);
  const descriptorTable = number(optional(args, "--descriptor-table") ?? STATIC_DESCRIPTOR_TABLE);
  const descriptorCount = number(optional(args, "--descriptor-count") ?? STATIC_DESCRIPTOR_COUNT);
  const paletteOffset = number(optional(args, "--palette-offset") ?? STATIC_PALETTE_OFFSET);
  const paletteEntries = number(optional(args, "--palette-entries") ?? STATIC_PALETTE_ENTRIES);
  if (same(romPath, directory) || same(romPath, palette)) throw new Error("refusing to overwrite an input");
  const index = export_static_sprite_series(readFileSync(romPath), directory, palette, {
    address, end, descriptorTable, descriptorCount, paletteOffset, paletteEntries,
  });
  const frames = index.packages.reduce((sum: number, item: Json) => {
    const plan = JSON.parse(readFileSync(join(directory, item.plan), "utf8"));
    return sum + plan.frames.length;
  }, 0);
  console.log(`packages=${index.packages.length} frames=${frames} bytes=${number(index.size)}`);
}

if (import.meta.main) main(Bun.argv.slice(2));
