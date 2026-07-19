#!/usr/bin/env bun
import { mkdirSync, readFileSync, realpathSync, writeFileSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
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
const DESCRIPTOR_COUNT = 0x200;
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

function number(value: string | number): number {
  const parsed = typeof value === "string" ? Number(value) : Math.trunc(value);
  if (!Number.isInteger(parsed)) throw new Error("invalid static-sprite integer");
  return parsed;
}

function hex(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function same(left: string, right: string): boolean {
  try { return realpathSync(left) === realpathSync(right); } catch { return resolve(left) === resolve(right); }
}

function selectedPalette(path: string): Rgb[] {
  return read_palette(path, STATIC_PALETTE_OFFSET, STATIC_PALETTE_ENTRIES);
}

function descriptorPackages(rom: Buffer): PackageLayout[] {
  const table = STATIC_DESCRIPTOR_TABLE - ROM_BASE;
  if (table < 0 || table + DESCRIPTOR_SIZE * DESCRIPTOR_COUNT > rom.length) {
    throw new Error("static-sprite descriptor table is outside the ROM");
  }
  const byDirectory = new Map<number, Descriptor[]>();
  for (let id = 0; id < DESCRIPTOR_COUNT; id++) {
    const offset = table + id * DESCRIPTOR_SIZE;
    const directory = rom.readUInt32LE(offset + 12);
    if (directory < STATIC_SERIES_ADDRESS || directory >= STATIC_SERIES_END) continue;
    if (rom[offset + 10] !== 3) throw new Error(`resource ${id.toString(16)} is not mode 3`);
    const descriptor: Descriptor = { id, width: rom[offset], height: rom[offset + 1], directory };
    const aliases = byDirectory.get(directory) ?? [];
    aliases.push(descriptor);
    byDirectory.set(directory, aliases);
  }
  const directories = [...byDirectory.keys()].sort((left, right) => left - right);
  if (directories.length === 0) throw new Error("static-sprite series has no packages");
  let end = STATIC_SERIES_END;
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
    if (uniquePointers.some((pointer) => pointer < STATIC_SERIES_ADDRESS || pointer >= directory)) {
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
  const prefix = rom.subarray(STATIC_SERIES_ADDRESS - ROM_BASE, end - ROM_BASE);
  if (prefix.length === 0 || prefix.some((byte) => byte !== 0)) throw new Error("static-sprite prefix is not zero alignment");
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
  if (base !== STATIC_SERIES_ADDRESS || expectedSize !== STATIC_SERIES_SIZE || prefix < 0) {
    throw new Error("static-sprite series extent differs from the known layout");
  }
  const palette = selectedPalette(palettePath);
  let result = Buffer.alloc(prefix);
  let framesBuilt = 0, directoryEntries = 0;
  const root = dirname(indexPath);
  for (const item of index.packages) {
    const address = number(item.address), end = address + number(item.size);
    if (base + result.length !== address) throw new Error(`sprite package ${item.id} is not contiguous`);
    const planPath = resolve(root, String(item.plan));
    const imagePath = resolve(root, String(item.source));
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

export function export_static_sprite_series(rom: Buffer, directory: string, palettePath: string): Json {
  if (rom.length < STATIC_SERIES_END - ROM_BASE) throw new Error("ROM is too small for the static-sprite series");
  const packages = descriptorPackages(rom);
  const palette = selectedPalette(palettePath);
  const arena = rom.subarray(STATIC_SERIES_ADDRESS - ROM_BASE, STATIC_SERIES_END - ROM_BASE);
  const prefix = packages[0].address - STATIC_SERIES_ADDRESS;
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
      const trace = decode_static_frame_trace(arena, pointer - STATIC_SERIES_ADDRESS);
      const next = item.uniquePointers[frameIndex + 1] ?? item.directory;
      const gap = next - pointer - trace.encodedSize;
      if (frameIndex + 1 < item.uniquePointers.length) {
        if (gap !== 0) throw new Error(`resource ${id} frame ${frameIndex} does not fill its stream extent`);
      } else {
        if (gap < 0 || gap > 3 || arena.subarray(next - STATIC_SERIES_ADDRESS - gap, next - STATIC_SERIES_ADDRESS).some((byte) => byte !== 0)) {
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
    address: hex(STATIC_SERIES_ADDRESS),
    size: hex(STATIC_SERIES_SIZE),
    descriptor_table: hex(STATIC_DESCRIPTOR_TABLE),
    palette_offset: STATIC_PALETTE_OFFSET,
    palette_entries: STATIC_PALETTE_ENTRIES,
    prefix_zeros: prefix,
    packages: packageEntries,
  };
  const indexPath = join(directory, "index.json");
  writeFileSync(indexPath, JSON.stringify(index, null, 2) + "\n");
  const [rebuilt] = build_static_sprite_series(indexPath, palettePath);
  if (!rebuilt.equals(arena)) throw new Error("static-sprite series round trip differs");
  return index;
}

function option(args: string[], ...names: string[]): string {
  const index = args.findIndex((argument) => names.includes(argument));
  if (index < 0 || index + 1 >= args.length) throw new Error(`${names.at(-1)} is required`);
  return args[index + 1];
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: static_sprite_series.ts export-series ROM --directory DIR --palette PNG");
    return;
  }
  if (args[0] !== "export-series" || !args[1]) throw new Error("a static-sprite series command and ROM are required");
  const romPath = args[1], directory = option(args, "--directory"), palette = option(args, "--palette");
  if (same(romPath, directory) || same(romPath, palette)) throw new Error("refusing to overwrite an input");
  const index = export_static_sprite_series(readFileSync(romPath), directory, palette);
  const frames = index.packages.reduce((sum: number, item: Json) => {
    const plan = JSON.parse(readFileSync(join(directory, item.plan), "utf8"));
    return sum + plan.frames.length;
  }, 0);
  console.log(`packages=${index.packages.length} frames=${frames} bytes=${STATIC_SERIES_SIZE}`);
}

if (import.meta.main) main(Bun.argv.slice(2));
