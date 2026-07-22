#!/usr/bin/env bun
import { mkdirSync, readFileSync, realpathSync, writeFileSync } from "node:fs";
import { basename, dirname, join, resolve } from "node:path";
import { tile_png } from "./export_asset.ts";
import { prune_files } from "./generated_files.ts";
import { indexed_png, type Rgb } from "./import_asset.ts";

export const STILL_WIDTH = 256;
export const STILL_HEIGHT = 120;
export const STILL_PIXELS = STILL_WIDTH * STILL_HEIGHT;
export const STILL_PALETTE_ENTRIES = 128;
export const STILL_PALETTE_BYTES = STILL_PALETTE_ENTRIES * 2;
export const RESOURCE_TABLE = 0x08320000;
const ROM_BASE = 0x08000000;
const DEFAULT_INDEX = resolve(import.meta.dir, "../assets/graphics/Pre-rendered Backgrounds/index.json");

export interface StillIndexEntry {
  id: string;
  file: string;
  scene: string;
  location: string | null;
}

export function read_still_index(path = DEFAULT_INDEX): Map<string, StillIndexEntry> {
  const document = JSON.parse(readFileSync(path, "utf8")) as {
    format?: number;
    kind?: string;
    resources?: StillIndexEntry[];
  };
  if (document.format !== 1 || document.kind !== "golden-sun-pre-rendered-background-index" ||
      !Array.isArray(document.resources)) throw new Error("invalid pre-rendered background index");
  const result = new Map<string, StillIndexEntry>();
  for (const [offset, entry] of document.resources.entries()) {
    const expected = (0x1d + offset).toString(16);
    if (entry.id !== expected || basename(entry.file) !== entry.file ||
        !new RegExp(`^resource_${entry.id}_[a-z0-9_]+\\.8bpp\\.png$`).test(entry.file) ||
        typeof entry.scene !== "string" || entry.scene.length === 0 ||
        !(entry.location === null || typeof entry.location === "string")) {
      throw new Error(`invalid pre-rendered background index entry ${offset}`);
    }
    result.set(entry.id, entry);
  }
  if (result.size !== 0x3e - 0x1d + 1) throw new Error("pre-rendered background index is incomplete");
  return result;
}

export interface Delta7Decoded {
  pixels: Buffer;
  bits: number;
  bytes: number;
}

export interface StillDecoded extends Delta7Decoded {
  palette: Buffer;
  size: number;
}

class BitReader {
  cursor = 0;

  constructor(readonly data: Uint8Array) {}

  read(count = 1): number {
    if (!Number.isInteger(count) || count < 1 || count > 16) throw new Error("invalid bit count");
    if (this.cursor + count > this.data.length * 8) throw new Error("delta7 stream is truncated");
    let value = 0;
    for (let index = 0; index < count; index++, this.cursor++) {
      value |= (this.data[this.cursor >>> 3] >>> (this.cursor & 7) & 1) << index;
    }
    return value;
  }
}

function put(bits: number[], value: number, count: number): void {
  for (let index = 0; index < count; index++) bits.push(value >>> index & 1);
}

export function decode_delta7(data: Uint8Array, count = STILL_PIXELS): Delta7Decoded {
  if (!Number.isInteger(count) || count < 0) throw new Error("invalid delta7 pixel count");
  const reader = new BitReader(data);
  const pixels = Buffer.alloc(count);
  let previous = 0;
  for (let index = 0; index < count; index++) {
    if (reader.read() === 0) {
      if (reader.read() !== 0) {
        if (reader.read() === 0) {
          const negative = reader.read() !== 0;
          const magnitude = 11 + reader.read(4);
          previous = (previous + (negative ? -magnitude : magnitude)) & 0x7f;
        } else {
          previous = reader.read(7);
        }
      }
    } else if (reader.read() === 0) {
      const negative = reader.read() !== 0;
      const magnitude = 1 + reader.read();
      previous = (previous + (negative ? -magnitude : magnitude)) & 0x7f;
    } else {
      const negative = reader.read() !== 0;
      const magnitude = 3 + reader.read(3);
      previous = (previous + (negative ? -magnitude : magnitude)) & 0x7f;
    }
    pixels[index] = previous;
  }
  const bytes = Math.ceil(reader.cursor / 16) * 2;
  for (let bit = reader.cursor; bit < bytes * 8; bit++) {
    if (data[bit >>> 3] >>> (bit & 7) & 1) throw new Error("delta7 padding bits are not zero");
  }
  return { pixels, bits: reader.cursor, bytes };
}

export function encode_delta7(pixels: Uint8Array): Buffer {
  const bits: number[] = [];
  let previous = 0;
  for (const pixel of pixels) {
    if (pixel > 0x7f) throw new Error("delta7 pixel exceeds seven bits");
    const delta = (pixel - previous) & 0x7f;
    if (delta === 0) {
      put(bits, 0, 2);
    } else if (delta <= 2) {
      put(bits, 1, 1); put(bits, 0, 1); put(bits, 0, 1); put(bits, delta - 1, 1);
    } else if (delta <= 10) {
      put(bits, 1, 1); put(bits, 1, 1); put(bits, 0, 1); put(bits, delta - 3, 3);
    } else if (delta <= 26) {
      put(bits, 0, 1); put(bits, 1, 1); put(bits, 0, 1); put(bits, 0, 1); put(bits, delta - 11, 4);
    } else if (delta <= 101) {
      put(bits, 0, 1); put(bits, 1, 1); put(bits, 1, 1); put(bits, pixel, 7);
    } else if (delta <= 117) {
      put(bits, 0, 1); put(bits, 1, 1); put(bits, 0, 1); put(bits, 1, 1); put(bits, 117 - delta, 4);
    } else if (delta <= 125) {
      put(bits, 1, 1); put(bits, 1, 1); put(bits, 1, 1); put(bits, 125 - delta, 3);
    } else {
      put(bits, 1, 1); put(bits, 0, 1); put(bits, 1, 1); put(bits, 127 - delta, 1);
    }
    previous = pixel;
  }
  const result = Buffer.alloc(Math.ceil(bits.length / 16) * 2);
  for (const [index, bit] of bits.entries()) result[index >>> 3] |= bit << (index & 7);
  return result;
}

function palette_rgb(data: Uint8Array): Rgb[] {
  if (data.length !== STILL_PALETTE_BYTES) throw new Error("still palette must contain 128 colors");
  const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
  const palette: Rgb[] = [];
  for (let index = 0; index < STILL_PALETTE_ENTRIES; index++) {
    const value = view.getUint16(index * 2, true);
    if (value & 0x8000) throw new Error("still palette contains a non-BGR555 high bit");
    palette.push([(value & 31) << 3, (value >>> 5 & 31) << 3, (value >>> 10 & 31) << 3]);
  }
  return palette;
}

function palette_bytes(palette: readonly Rgb[]): Buffer {
  if (palette.length !== STILL_PALETTE_ENTRIES) throw new Error("still PNG must contain exactly 128 palette entries");
  const result = Buffer.alloc(STILL_PALETTE_BYTES);
  palette.forEach(([red, green, blue], index) => {
    if ((red & 7) || (green & 7) || (blue & 7)) {
      throw new Error("still palette channels must be exact five-bit values");
    }
    result.writeUInt16LE((red >>> 3) | (green >>> 3) << 5 | (blue >>> 3) << 10, index * 2);
  });
  return result;
}

export function decode_still(data: Uint8Array): StillDecoded {
  if (data.length <= STILL_PALETTE_BYTES) throw new Error("still record is truncated");
  const palette = Buffer.from(data.subarray(0, STILL_PALETTE_BYTES));
  palette_rgb(palette);
  const decoded = decode_delta7(data.subarray(STILL_PALETTE_BYTES));
  return { ...decoded, palette, size: STILL_PALETTE_BYTES + decoded.bytes };
}

function raster_tiles(pixels: Uint8Array): Buffer {
  if (pixels.length !== STILL_PIXELS) throw new Error("still raster has the wrong pixel count");
  const result = Buffer.alloc(STILL_PIXELS);
  let cursor = 0;
  for (let top = 0; top < STILL_HEIGHT; top += 8) {
    for (let left = 0; left < STILL_WIDTH; left += 8) {
      for (let y = 0; y < 8; y++) {
        for (let x = 0; x < 8; x++) result[cursor++] = pixels[(top + y) * STILL_WIDTH + left + x];
      }
    }
  }
  return result;
}

export function export_still(data: Uint8Array): [Buffer, Record<string, number>] {
  const decoded = decode_still(data);
  const [image] = tile_png(raster_tiles(decoded.pixels), 8, STILL_WIDTH / 8, palette_rgb(decoded.palette));
  return [image, {
    width: STILL_WIDTH,
    height: STILL_HEIGHT,
    palette_entries: STILL_PALETTE_ENTRIES,
    bits: decoded.bits,
    encoded_bytes: decoded.bytes,
    source_bytes: decoded.size,
  }];
}

export function build_still(image: Uint8Array): [Buffer, Record<string, number>] {
  const [width, height, pixels, palette] = indexed_png(image);
  if (width !== STILL_WIDTH || height !== STILL_HEIGHT) throw new Error("still PNG must be 256x120");
  const colors = palette_bytes(palette);
  const encoded = encode_delta7(Uint8Array.from(pixels));
  const decoded = decode_delta7(encoded);
  if (!decoded.pixels.equals(Buffer.from(pixels))) throw new Error("delta7 encoder replay differs");
  return [Buffer.concat([colors, encoded]), {
    width,
    height,
    palette_entries: palette.length,
    bits: decoded.bits,
    encoded_bytes: encoded.length,
  }];
}

export function diagnose_still(data: Uint8Array): Record<string, number> | null {
  try {
    const decoded = decode_still(data);
    if (Math.ceil(decoded.size / 4) * 4 !== data.length) return null;
    const replay = encode_delta7(decoded.pixels);
    if (!replay.equals(Buffer.from(data.subarray(STILL_PALETTE_BYTES, decoded.size)))) return null;
    return {
      width: STILL_WIDTH,
      height: STILL_HEIGHT,
      palette_entries: STILL_PALETTE_ENTRIES,
      bits: decoded.bits,
      source_bytes: decoded.size,
      alignment_bytes: data.length - decoded.size,
    };
  } catch {
    return null;
  }
}

export function self_test(): void {
  const deltas = [0, 1, 2, 3, 10, 11, 26, 27, 64, 101, 102, 117, 118, 125, 126, 127];
  const pixels = Buffer.alloc(STILL_PIXELS);
  let value = 0;
  for (let index = 0; index < pixels.length; index++) {
    value = (value + deltas[index % deltas.length]) & 0x7f;
    pixels[index] = value;
  }
  const encoded = encode_delta7(pixels);
  const decoded = decode_delta7(encoded);
  if (!decoded.pixels.equals(pixels) || decoded.bytes !== encoded.length) throw new Error("delta7 self-test failed");
  const palette = Buffer.alloc(STILL_PALETTE_BYTES);
  for (let index = 0; index < STILL_PALETTE_ENTRIES; index++) {
    palette.writeUInt16LE((index & 31) | (index >>> 2 & 31) << 5 | (index >>> 4 & 31) << 10, index * 2);
  }
  const record = Buffer.concat([palette, encoded]);
  const [image] = export_still(record);
  const [rebuilt] = build_still(image);
  if (!rebuilt.equals(record) || diagnose_still(record) === null) throw new Error("still-image self-test failed");
  console.log("self-test=ok");
}

function option(args: string[], ...names: string[]): string {
  const index = args.findIndex((argument) => names.includes(argument));
  if (index < 0 || index + 1 >= args.length) throw new Error(`${names.at(-1)} is required`);
  return args[index + 1];
}

function optional_option(args: string[], ...names: string[]): string | undefined {
  const index = args.findIndex((argument) => names.includes(argument));
  return index < 0 ? undefined : option(args, ...names);
}

function same(left: string, right: string): boolean {
  try { return realpathSync(left) === realpathSync(right); } catch { return resolve(left) === resolve(right); }
}

function resource_span(rom: Buffer, id: number): Buffer {
  const table = RESOURCE_TABLE - ROM_BASE;
  if (id < 0 || table + (id + 1) * 4 + 4 > rom.length) throw new Error("resource ID is outside the pointer table");
  const start = rom.readUInt32LE(table + id * 4) - ROM_BASE;
  const end = rom.readUInt32LE(table + (id + 1) * 4) - ROM_BASE;
  if (start < 0 || start >= end || end > rom.length) throw new Error("resource span is outside the ROM");
  return rom.subarray(start, end);
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: indexed_still.ts [--self-test] export-series ROM [--index FILE] [--directory DIR]");
    return;
  }
  if (args.includes("--self-test")) {
    self_test();
    read_still_index();
    args = args.filter((argument) => argument !== "--self-test");
    if (args.length === 0) return;
  }
  if (args[0] !== "export-series" || !args[1]) throw new Error("an indexed-still command and ROM are required");
  const romPath = args[1];
  const indexPath = optional_option(args, "--index") ?? DEFAULT_INDEX;
  const directory = optional_option(args, "--directory") ?? dirname(indexPath);
  const names = read_still_index(indexPath);
  if (same(romPath, directory)) throw new Error("refusing to overwrite the input ROM");
  const rom = readFileSync(romPath);
  let bytes = 0;
  const outputs: string[] = [];
  for (let id = 0x1d; id <= 0x3e; id++) {
    const record = resource_span(rom, id);
    const decoded = decode_still(record);
    const [image] = export_still(record);
    const name = id.toString(16);
    const output = join(directory, names.get(name)!.file);
    mkdirSync(dirname(output), { recursive: true });
    writeFileSync(output, image);
    outputs.push(output);
    const [rebuilt] = build_still(image);
    if (!rebuilt.equals(record.subarray(0, decoded.size))) throw new Error(`resource ${name} round trip differs`);
    bytes += decoded.size;
  }
  prune_files(directory, "resource_*.8bpp.png", outputs);
  console.log(`images=34 bytes=${bytes}`);
}

if (import.meta.main) main(Bun.argv.slice(2));
