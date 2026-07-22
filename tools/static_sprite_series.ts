#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts, tools/character_catalog.ts, tools/split_sprite_bank.ts; invoked by package.json.
import {
  existsSync, lstatSync, mkdirSync, mkdtempSync, readFileSync, readdirSync,
  realpathSync, rmSync, rmdirSync, symlinkSync, unlinkSync, writeFileSync,
} from "node:fs";
import { tmpdir } from "node:os";
import { dirname, isAbsolute, join, relative, resolve } from "node:path";
import { prune_files } from "./generated_files.ts";
import { characterBankPath } from "./asset_paths.ts";
import { indexed_png, type Rgb } from "./import_asset.ts";
import { png, read_palette } from "./skip_sprite_archive.ts";
import {
  decode_static_frame_trace, encode_static_frame_plan, static_sprite_plan_mode,
  type StaticSpriteMode,
} from "./static_sprite_bank.ts";

const ROM_BASE = 0x08000000;
export const STATIC_DESCRIPTOR_TABLE = 0x08185024;
export const STATIC_SERIES_ADDRESS = 0x08244fc0;
export const STATIC_SERIES_END = 0x08287774;
export const STATIC_SERIES_SIZE = STATIC_SERIES_END - STATIC_SERIES_ADDRESS;
export const STATIC_PALETTE_OFFSET = 16;
export const STATIC_PALETTE_ENTRIES = 224;
const DESCRIPTOR_SIZE = 20;
export const STATIC_DESCRIPTOR_COUNT = 0x200;
const DEFAULT_ATLAS_COLUMNS = 8;

type Json = Record<string, any>;

interface Descriptor {
  id: number;
  width: number;
  height: number;
  mode: StaticSpriteMode;
  directory: number;
}

interface PackageLayout {
  ids: number[];
  width: number;
  height: number;
  mode: StaticSpriteMode;
  address: number;
  directory: number;
  end: number;
  pointers: number[];
  uniquePointers: number[];
}

export interface StaticSpriteAtlasLayout {
  source?: string;
  columns: number;
  frames: Array<number | null>;
}

interface AtlasDefinition {
  source: string;
  columns: number;
  frames: Array<number | null>;
}

export interface StaticSpriteSeriesOptions {
  address: number;
  end: number;
  descriptorTable: number;
  descriptorCount: number;
  paletteOffset: number;
  paletteEntries: number;
  suffixZeros: number;
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

function appendZeroSuffix(result: Buffer, expectedSize: number, suffix: number): Buffer {
  if (result.length !== expectedSize - suffix) throw new Error("static-sprite series size differs");
  return Buffer.concat([result, Buffer.alloc(suffix)]);
}

function contentSizeBeforeZeroSuffix(arena: Uint8Array, suffix: number): number {
  integer(suffix, 0, arena.length - 1, "static-sprite suffix alignment");
  const contentSize = arena.length - suffix;
  if (arena.subarray(contentSize).some((byte) => byte !== 0)) {
    throw new Error("static-sprite suffix alignment is not zero");
  }
  return contentSize;
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
    const mode = rom[offset + 10];
    if (mode !== 0 && mode !== 1 && mode !== 3) {
      throw new Error(`resource ${id.toString(16)} has an unsupported sprite mode`);
    }
    const descriptor: Descriptor = { id, width: rom[offset], height: rom[offset + 1], mode, directory };
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
    if (descriptors.some((item) => item.width !== descriptors[0].width ||
        item.height !== descriptors[0].height || item.mode !== descriptors[0].mode)) {
      throw new Error(`sprite aliases at ${hex(directory)} disagree on their descriptor`);
    }
    if (!descriptors[0].width || !descriptors[0].height || descriptors[0].width % 8 || descriptors[0].height % 8) {
      throw new Error(`sprite package ${hex(directory)} has invalid dimensions`);
    }
    packages.push({
      ids: descriptors.map((item) => item.id).sort((left, right) => left - right),
      width: descriptors[0].width,
      height: descriptors[0].height,
      mode: descriptors[0].mode,
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

function atlasDefinitions(value: unknown, frameCount: number): AtlasDefinition[] {
  if (!Array.isArray(value) || value.length === 0) throw new Error("sprite atlas list is empty");
  const definitions: AtlasDefinition[] = [];
  const covered = new Set<number>();
  const sources = new Set<string>();
  for (const item of value) {
    if (!item || typeof item !== "object") throw new Error("invalid sprite atlas entry");
    const source = String((item as Json).source ?? "");
    if (!/^koma(?:_[0-9a-f]{2})?\.8bpp\.png$/.test(source) || sources.has(source)) {
      throw new Error("invalid sprite atlas source");
    }
    sources.add(source);
    const columns = number((item as Json).columns);
    const sourceFrames = (item as Json).frames;
    if (!Array.isArray(sourceFrames) || sourceFrames.length === 0 || columns < 1 || columns > sourceFrames.length) {
      throw new Error("invalid sprite atlas layout");
    }
    const frames = sourceFrames.map((entry: unknown) => entry === null ? null : number(entry as number));
    const present = frames.filter((frame): frame is number => frame !== null);
    if (new Set(present).size !== present.length) throw new Error("sprite atlas repeats a frame");
    for (const frame of frames) {
      if (frame === null) continue;
      if (frame < 0 || frame >= frameCount) throw new Error("sprite atlas frame is outside its bank");
      covered.add(frame);
    }
    definitions.push({ source, columns, frames });
  }
  if (covered.size !== frameCount) throw new Error("sprite atlases do not cover their bank");
  return definitions;
}

function localAtlasPath(directory: string, source: string, existing: boolean): string {
  if (!/^koma(?:_[0-9a-f]{2})?\.8bpp\.png$/.test(source)) throw new Error("invalid sprite atlas source");
  return existing ? confined(directory, source) : join(directory, source);
}

function makeAtlas(
  frames: readonly Uint8Array[], width: number, height: number,
  palette: readonly Rgb[], columns: number,
): Buffer {
  if (frames.length === 0) throw new Error("sprite atlas cannot be empty");
  integer(columns, 1, frames.length, "sprite atlas columns");
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

function atlasFrames(
  image: Uint8Array, frameWidth: number, frameHeight: number,
  frameCount: number, columns: number, palette: readonly Rgb[],
): Buffer[] {
  const [width, height, sourcePixels, actualPalette] = indexed_png(image);
  const pixels = Buffer.from(sourcePixels);
  if (!Bun.deepEquals(actualPalette, palette, true)) {
    throw new Error("sprite atlas does not use the shared OBJ palette");
  }
  integer(frameWidth, 1, 0x10000, "sprite frame width");
  integer(frameHeight, 1, 0x10000, "sprite frame height");
  integer(frameCount, 1, 0x100000, "sprite atlas frame count");
  integer(columns, 1, frameCount, "sprite atlas columns");
  const rows = Math.ceil(frameCount / columns);
  if (width !== columns * frameWidth || height !== rows * frameHeight) {
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

// フレーム毎PNGの名前。フレーム番号から導出し、JSONへ別名を持たせない。
export function static_sprite_frame_name(index: number): string {
  integer(index, 0, 0xfffff, "sprite frame index");
  return `koma_${index.toString().padStart(3, "0")}.png`;
}

// フレーム毎PNGを読む。各画像はwidth×heightで共有OBJパレットに一致する。
function readFrameImages(
  directory: string, frameWidth: number, frameHeight: number,
  frameCount: number, palette: readonly Rgb[],
): Buffer[] {
  integer(frameCount, 1, 0x100000, "sprite frame count");
  const frames: Buffer[] = [];
  for (let frameIndex = 0; frameIndex < frameCount; frameIndex++) {
    const path = join(directory, static_sprite_frame_name(frameIndex));
    const [width, height, sourcePixels, actualPalette] = indexed_png(readFileSync(path));
    if (width !== frameWidth || height !== frameHeight) {
      throw new Error("sprite frame image differs from its plan");
    }
    if (!Bun.deepEquals(actualPalette, palette, true)) {
      throw new Error("sprite frame image does not use the shared OBJ palette");
    }
    frames.push(Buffer.from(sourcePixels));
  }
  return frames;
}

function readAtlasFrames(
  root: string, item: Json, planPath: string, plan: Json, palette: readonly Rgb[],
): Buffer[] {
  const frameWidth = number(plan.width), frameHeight = number(plan.height);
  const frameCount = plan.frames.length;
  if (plan.atlases === undefined && plan.atlas_columns === undefined) {
    if (item.source !== undefined) throw new Error(`sprite package ${item.id} mixes atlas layouts`);
    return readFrameImages(dirname(planPath), frameWidth, frameHeight, frameCount, palette);
  }
  if (plan.atlases === undefined) {
    if (typeof item.source !== "string") throw new Error(`sprite package ${item.id} lacks its atlas source`);
    const columns = number(plan.atlas_columns);
    return atlasFrames(
      readFileSync(confined(root, item.source)), frameWidth, frameHeight,
      frameCount, columns, palette,
    );
  }
  if (item.source !== undefined || plan.atlas_columns !== undefined) {
    throw new Error(`sprite package ${item.id} mixes atlas layouts`);
  }
  const definitions = atlasDefinitions(plan.atlases, frameCount);
  const frames: Array<Buffer | undefined> = Array(frameCount);
  for (const definition of definitions) {
    const sourceFrames = atlasFrames(
      readFileSync(localAtlasPath(dirname(planPath), definition.source, true)),
      frameWidth, frameHeight, definition.frames.length, definition.columns, palette,
    );
    definition.frames.forEach((frameIndex, index) => {
      if (frameIndex === null) {
        if (sourceFrames[index].some((pixel) => pixel !== 0)) {
          throw new Error(`sprite package ${item.id} atlas spacer is not transparent`);
        }
        return;
      }
      const previous = frames[frameIndex];
      if (previous && !previous.equals(sourceFrames[index])) {
        throw new Error(`sprite package ${item.id} shared frame differs between atlases`);
      }
      frames[frameIndex] = sourceFrames[index];
    });
  }
  if (frames.some((frame) => frame === undefined)) throw new Error(`sprite package ${item.id} lacks a frame`);
  return frames as Buffer[];
}

function storedAtlasDefinitions(planPath: string, frameCount: number): AtlasDefinition[] | undefined {
  if (!existsSync(planPath)) return undefined;
  const plan = JSON.parse(readFileSync(planPath, "utf8"));
  if (plan.atlases === undefined) return undefined;
  return atlasDefinitions(plan.atlases, frameCount);
}

function writeAtlases(
  directory: string, definitions: readonly AtlasDefinition[], frames: readonly Buffer[],
  width: number, height: number, palette: readonly Rgb[],
): string[] {
  const paths: string[] = [];
  for (const definition of definitions) {
    const path = localAtlasPath(directory, definition.source, false);
    const selected = definition.frames.map((frame) =>
      frame === null ? Buffer.alloc(width * height) : frames[frame]);
    writeFileSync(path, makeAtlas(selected, width, height, palette, definition.columns));
    paths.push(path);
  }
  return paths;
}

export function build_static_sprite_series(indexPath: string, palettePath: string): [Buffer, Json] {
  const index = JSON.parse(readFileSync(indexPath, "utf8"));
  if (index.format !== 1 || index.codec !== "golden-sun-static-sprite-series") {
    throw new Error("unsupported static-sprite series index");
  }
  const base = number(index.address), expectedSize = number(index.size);
  const prefix = number(index.prefix_zeros);
  const suffix = number(index.suffix_zeros ?? 0);
  seriesExtent(base, expectedSize);
  const descriptorTable = number(index.descriptor_table), descriptorCount = number(index.descriptor_count);
  const paletteOffset = number(index.palette_offset), paletteEntries = number(index.palette_entries);
  if (descriptorTable < ROM_BASE || descriptorTable > 0xffffffff ||
      descriptorCount < 1 || descriptorCount > 0x10000 ||
      paletteOffset < 0 || paletteOffset > 255 ||
      paletteEntries < 1 || paletteOffset + paletteEntries > 256 ||
      prefix < 0 || suffix < 0 || prefix + suffix > expectedSize ||
      !Array.isArray(index.packages) || index.packages.length === 0) {
    throw new Error("invalid static-sprite series index");
  }
  const palette = selectedPalette(palettePath, paletteOffset, paletteEntries);
  let result = Buffer.alloc(prefix);
  let framesBuilt = 0, directoryEntries = 0;
  const root = dirname(indexPath);
  for (const item of index.packages) {
    const address = number(item.address), end = address + number(item.size);
    if (base + result.length !== address) throw new Error(`sprite package ${item.id} is not contiguous`);
    // 分類済みの battle/field 配下も許容する。plan は family 名を保つ。
    const planPath = characterBankPath(root, String(item.plan));
    const plan = JSON.parse(readFileSync(planPath, "utf8"));
    if (plan.format !== 1 || plan.codec !== "golden-sun-static-sprite-bank" || !Array.isArray(plan.frames) || !Array.isArray(plan.directory)) {
      throw new Error(`unsupported sprite package plan: ${item.id}`);
    }
    const planMode = plan.mode === undefined ? 3 : number(plan.mode);
    if (planMode !== 0 && planMode !== 1 && planMode !== 3) {
      throw new Error(`sprite package ${item.id} has an unsupported mode`);
    }
    if (item.mode !== undefined && number(item.mode) !== planMode) {
      throw new Error(`sprite package ${item.id} mode differs from its index`);
    }
    const frames = readAtlasFrames(root, item, planPath, plan, palette);
    const offsets: number[] = [];
    for (const [frameIndex, frame] of frames.entries()) {
      if (static_sprite_plan_mode(plan.frames[frameIndex]) !== planMode) {
        throw new Error(`sprite package ${item.id} frame mode differs from its bank`);
      }
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
  result = appendZeroSuffix(result, expectedSize, suffix);
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
  const suffixZeros = options.suffixZeros ?? 0;
  const [, checkedEnd] = seriesExtent(seriesAddress, seriesEnd - seriesAddress);
  if (checkedEnd - ROM_BASE > rom.length) throw new Error("ROM is too small for the static-sprite series");
  const arena = rom.subarray(seriesAddress - ROM_BASE, checkedEnd - ROM_BASE);
  const contentEnd = seriesAddress + contentSizeBeforeZeroSuffix(arena, suffixZeros);
  const packages = descriptorPackages(rom, seriesAddress, contentEnd, descriptorTable, descriptorCount);
  const palette = selectedPalette(palettePath, paletteOffset, paletteEntries);
  const prefix = packages[0].address - seriesAddress;
  const packageEntries: Json[] = [];
  for (const item of packages) {
    const id = item.ids[0].toString(16).padStart(3, "0");
    const resourceDirectory = join(directory, `chr_${id}`);
    const planPath = join(resourceDirectory, "bank.json");
    const frames: Buffer[] = [];
    const plans: Json[] = [];
    let alignment = 0;
    for (const [frameIndex, pointer] of item.uniquePointers.entries()) {
      const next = item.uniquePointers[frameIndex + 1] ?? item.directory;
      const trace = decode_static_frame_trace(
        arena, pointer - seriesAddress, item.mode, item.width * item.height,
        next - seriesAddress,
      );
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
    const storedDefinitions = storedAtlasDefinitions(planPath, frames.length);
    const definitions = storedDefinitions ?? [{
      source: "koma.8bpp.png",
      columns: Math.min(DEFAULT_ATLAS_COLUMNS, frames.length),
      frames: Array.from({ length: frames.length }, (_, index) => index),
    }];
    const plan = {
      format: 1,
      codec: "golden-sun-static-sprite-bank",
      ...(item.mode === 3 ? {} : { mode: item.mode }),
      width: item.width,
      height: item.height,
      ...(storedDefinitions ? { atlases: definitions } : { atlas_columns: definitions[0].columns }),
      alignment_zeros: alignment,
      frames: plans,
      directory: item.pointers.map((pointer) => pointerIndex.get(pointer)),
    };
    mkdirSync(resourceDirectory, { recursive: true });
    const imagePaths = writeAtlases(resourceDirectory, definitions, frames, item.width, item.height, palette);
    writeFileSync(planPath, JSON.stringify(plan) + "\n");
    prune_files(resourceDirectory, "koma*.png", imagePaths);
    const relativeRoot = `chr_${id}`;
    packageEntries.push({
      id,
      ...(item.mode === 3 ? {} : { mode: item.mode }),
      aliases: item.ids.slice(1).map((alias) => alias.toString(16)),
      address: hex(item.address),
      size: hex(item.end - item.address),
      directory: hex(item.directory),
      ...(storedDefinitions ? {} : { source: `${relativeRoot}/koma.8bpp.png` }),
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
    ...(suffixZeros ? { suffix_zeros: suffixZeros } : {}),
    packages: packageEntries,
  };
  const indexPath = join(directory, "index.json");
  writeFileSync(indexPath, JSON.stringify(index, null, 2) + "\n");
  const [rebuilt] = build_static_sprite_series(indexPath, palettePath);
  if (!rebuilt.equals(arena)) throw new Error("static-sprite series round trip differs");
  prune_static_sprite_packages(directory, packageEntries.map((item) => `chr_${item.id}`));
  return index;
}

export function repack_static_sprite_atlases(
  indexPath: string, palettePath: string,
  layouts: Readonly<Record<string, readonly StaticSpriteAtlasLayout[]>>,
): Json {
  const index = JSON.parse(readFileSync(indexPath, "utf8"));
  if (index.format !== 1 || index.codec !== "golden-sun-static-sprite-series" || !Array.isArray(index.packages)) {
    throw new Error("unsupported static-sprite series index");
  }
  const [before] = build_static_sprite_series(indexPath, palettePath);
  const palette = selectedPalette(palettePath, number(index.palette_offset), number(index.palette_entries));
  const root = dirname(indexPath);
  const prepared: Array<{
    item: Json;
    plan: Json;
    planPath: string;
    directory: string;
    frames: Buffer[];
    definitions: AtlasDefinition[];
  }> = [];
  let atlasCount = 0, sourceCells = 0, uniqueFrames = 0;
  for (const [id, requested] of Object.entries(layouts)) {
    const item = index.packages.find((entry: Json) => String(entry.id) === id);
    if (!item) throw new Error(`sprite package ${id} is absent`);
    const planPath = confined(root, String(item.plan));
    const plan = JSON.parse(readFileSync(planPath, "utf8"));
    if (plan.format !== 1 || plan.codec !== "golden-sun-static-sprite-bank" || !Array.isArray(plan.frames)) {
      throw new Error(`unsupported sprite package plan: ${id}`);
    }
    const frames = readAtlasFrames(root, item, planPath, plan, palette);
    const generated = requested.map((entry, atlasIndex) => ({
      source: entry.source ?? (requested.length === 1
        ? "koma.8bpp.png"
        : `koma_${atlasIndex.toString(16).padStart(2, "0")}.8bpp.png`),
      columns: entry.columns,
      frames: [...entry.frames],
    }));
    const definitions = atlasDefinitions(generated, frames.length);
    prepared.push({ item, plan, planPath, directory: dirname(planPath), frames, definitions });
    atlasCount += definitions.length;
    sourceCells += definitions.reduce((sum, entry) => sum + entry.frames.length, 0);
    uniqueFrames += frames.length;
  }
  for (const entry of prepared) {
    const paths = writeAtlases(
      entry.directory, entry.definitions, entry.frames,
      number(entry.plan.width), number(entry.plan.height), palette,
    );
    delete entry.plan.atlas_columns;
    entry.plan.atlases = entry.definitions;
    writeFileSync(entry.planPath, JSON.stringify(entry.plan) + "\n");
    delete entry.item.source;
    prune_files(entry.directory, "koma*.png", paths);
  }
  writeFileSync(indexPath, JSON.stringify(index, null, 2) + "\n");
  const [after] = build_static_sprite_series(indexPath, palettePath);
  if (!after.equals(before)) throw new Error("repacked sprite atlases do not round trip");
  return {
    packages: prepared.length,
    atlases: atlasCount,
    source_cells: sourceCells,
    unique_frames: uniqueFrames,
    bytes: after.length,
  };
}

// アトラスをフレーム毎PNGへ分割する。バイト完全一致を確認してから旧アトラスを削除する。
export function split_static_sprite_banks(indexPath: string, palettePath: string): Json {
  const index = JSON.parse(readFileSync(indexPath, "utf8"));
  if (index.format !== 1 || index.codec !== "golden-sun-static-sprite-series" || !Array.isArray(index.packages)) {
    throw new Error("unsupported static-sprite series index");
  }
  const [before] = build_static_sprite_series(indexPath, palettePath);
  const palette = selectedPalette(palettePath, number(index.palette_offset), number(index.palette_entries));
  const root = dirname(indexPath);
  const converted: Array<{ directory: string; written: string[] }> = [];
  const skipped: string[] = [];
  for (const item of index.packages) {
    const planPath = confined(root, String(item.plan));
    const plan = JSON.parse(readFileSync(planPath, "utf8"));
    if (plan.format !== 1 || plan.codec !== "golden-sun-static-sprite-bank" || !Array.isArray(plan.frames)) {
      throw new Error(`unsupported sprite package plan: ${item.id}`);
    }
    if (plan.atlases === undefined && plan.atlas_columns === undefined) continue;
    const directory = dirname(planPath);
    const savedSource = item.source;
    const written: string[] = [];
    try {
      const width = number(plan.width), height = number(plan.height);
      const frames = readAtlasFrames(root, item, planPath, plan, palette);
      frames.forEach((frame, frameIndex) => {
        const path = join(directory, static_sprite_frame_name(frameIndex));
        writeFileSync(path, png(frame, width, height, [...palette]));
        written.push(path);
      });
      const replay = readFrameImages(directory, width, height, frames.length, palette);
      replay.forEach((frame, frameIndex) => {
        if (!frame.equals(frames[frameIndex])) throw new Error("sprite frame image differs from its atlas cell");
      });
      delete plan.atlas_columns;
      delete plan.atlases;
      delete item.source;
      writeFileSync(planPath, JSON.stringify(plan) + "\n");
      converted.push({ directory, written });
    } catch (error) {
      for (const path of written) if (existsSync(path)) unlinkSync(path);
      item.source = savedSource;
      skipped.push(`${item.id}: ${(error as Error).message}`);
    }
  }
  writeFileSync(indexPath, JSON.stringify(index, null, 2) + "\n");
  const [after] = build_static_sprite_series(indexPath, palettePath);
  if (!after.equals(before)) throw new Error("split sprite banks do not round trip");
  for (const entry of converted) prune_files(entry.directory, "koma*.png", entry.written);
  return { packages: index.packages.length, converted: converted.length, skipped };
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
  const aligned = appendZeroSuffix(Buffer.from([1, 2, 3]), 7, 4);
  if (!aligned.equals(Buffer.from([1, 2, 3, 0, 0, 0, 0])) ||
      contentSizeBeforeZeroSuffix(aligned, 4) !== 3) {
    throw new Error("static-sprite suffix self-test failed");
  }
  try {
    contentSizeBeforeZeroSuffix(Buffer.from([1, 2, 0, 3]), 2);
    throw new Error("nonzero static-sprite suffix was accepted");
  } catch (error) {
    if ((error as Error).message === "nonzero static-sprite suffix was accepted") throw error;
  }
  const testPalette: Rgb[] = [[0, 0, 0], [255, 255, 255]];
  const testFrames = [
    Buffer.alloc(64, 0), Buffer.alloc(64, 1), Buffer.from(Array.from({ length: 64 }, (_, index) => index & 1)),
  ];
  const testAtlas = makeAtlas(testFrames, 8, 8, testPalette, 2);
  const replayFrames = atlasFrames(testAtlas, 8, 8, testFrames.length, 2, testPalette);
  if (!replayFrames.every((frame, index) => frame.equals(testFrames[index]))) {
    throw new Error("static-sprite atlas layout self-test failed");
  }
  const frameTemp = mkdtempSync(join(tmpdir(), "alchemy-static-frames-"));
  try {
    testFrames.forEach((frame, index) => {
      writeFileSync(join(frameTemp, static_sprite_frame_name(index)), png(frame, 8, 8, testPalette));
    });
    const frameReplay = readFrameImages(frameTemp, 8, 8, testFrames.length, testPalette);
    if (!frameReplay.every((frame, index) => frame.equals(testFrames[index]))) {
      throw new Error("static-sprite per-frame layout self-test failed");
    }
  } finally {
    rmSync(frameTemp, { recursive: true, force: true });
  }
  const shared = atlasDefinitions([
    { source: "koma_00.8bpp.png", columns: 2, frames: [null, 0, 1] },
    { source: "koma_01.8bpp.png", columns: 2, frames: [0, 2] },
  ], 3);
  if (shared.length !== 2 || shared[0].frames[0] !== null || shared[1].frames[1] !== 2) {
    throw new Error("static-sprite shared-atlas self-test failed");
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
    console.log("usage: static_sprite_series.ts export-series ROM --directory DIR --palette PNG [--address N --end N] [--descriptor-table N --descriptor-count N] [--palette-offset N --palette-entries N] [--suffix-zeros N]");
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
  const suffixZeros = number(optional(args, "--suffix-zeros") ?? 0);
  if (same(romPath, directory) || same(romPath, palette)) throw new Error("refusing to overwrite an input");
  const index = export_static_sprite_series(readFileSync(romPath), directory, palette, {
    address, end, descriptorTable, descriptorCount, paletteOffset, paletteEntries, suffixZeros,
  });
  const frames = index.packages.reduce((sum: number, item: Json) => {
    const plan = JSON.parse(readFileSync(join(directory, item.plan), "utf8"));
    return sum + plan.frames.length;
  }, 0);
  console.log(`packages=${index.packages.length} frames=${frames} bytes=${number(index.size)}`);
}

if (import.meta.main) main(Bun.argv.slice(2));
