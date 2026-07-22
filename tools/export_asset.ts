// Tool role: both; imported by tools/archive_asset.ts, tools/battle_effect_data.ts, tools/chiiki_map_resources.ts (+18 more); invoked by package.json.
import { mkdirSync, readFileSync, realpathSync, writeFileSync } from "node:fs";
import { dirname, resolve } from "node:path";
import { crc32, deflateSync } from "./zlib.ts";
import { gba_graphics, gba_palette_rgba, indexed_png, rgba_png, type Rgb, type Report } from "./import_asset.ts";

export const ROM_BASE = 0x08000000;

export function chunk(kind: Uint8Array, payload: Uint8Array): Buffer {
  const body = Buffer.concat([Buffer.from(kind), Buffer.from(payload)]);
  const size = Buffer.alloc(4), checksum = Buffer.alloc(4);
  size.writeUInt32BE(payload.length); checksum.writeUInt32BE(crc32(body));
  return Buffer.concat([size, body, checksum]);
}

function png(parts: Buffer[]): Buffer {
  return Buffer.concat([Buffer.from("89504e470d0a1a0a", "hex"), ...parts]);
}

function ihdr(width: number, height: number, depth: number, color: number): Buffer {
  const value = Buffer.alloc(13);
  value.writeUInt32BE(width, 0); value.writeUInt32BE(height, 4);
  value.set([depth, color, 0, 0, 0], 8);
  return value;
}

export function tile_png(raw: Uint8Array, bpp: number, columns: number, palette_colors?: readonly Rgb[] | null): [Buffer, Report] {
  const unit = bpp === 4 ? 32 : 64;
  if (raw.length === 0 || raw.length % unit !== 0) throw new Error("tile data must contain whole nonempty tiles");
  const count = raw.length / unit;
  if (columns <= 0 || count % columns !== 0) throw new Error("columns must divide the tile count exactly");
  const rows = count / columns, width = columns * 8, height = rows * 8;
  const pixels = Buffer.alloc(width * height);
  for (let tile = 0; tile < count; tile++) {
    const source = raw.subarray(tile * unit, (tile + 1) * unit);
    const indices = bpp === 4 ? Array.from(source).flatMap((byte) => [byte & 15, byte >>> 4]) : Array.from(source);
    const left = tile % columns * 8, top = Math.floor(tile / columns) * 8;
    for (let y = 0; y < 8; y++) pixels.set(indices.slice(y * 8, y * 8 + 8), (top + y) * width + left);
  }
  const scanlines: number[] = [];
  for (let y = 0; y < height; y++) {
    scanlines.push(0);
    const row = pixels.subarray(y * width, (y + 1) * width);
    if (bpp === 4) for (let index = 0; index < width; index += 2) scanlines.push(row[index] << 4 | row[index + 1]);
    else scanlines.push(...row);
  }
  const colors = bpp === 4 ? 16 : 256;
  let palette: readonly Rgb[];
  if (palette_colors == null) {
    palette = bpp === 4 ? Array.from({ length: 16 }, (_, index) => [index * 16, index * 16, index * 16] as Rgb) :
      Array.from({ length: 256 }, (_, index) => [(index & 31) * 8, (index >>> 5) * 8, 0] as Rgb);
  } else palette = palette_colors;
  if (palette.length < 1 || palette.length > colors) throw new Error("palette does not fit the requested tile depth");
  if (Math.max(...pixels) >= palette.length) throw new Error("tile pixels reference a missing palette entry");
  const paletteBytes = Buffer.from(palette.flatMap((color) => color));
  return [png([chunk(Buffer.from("IHDR"), ihdr(width, height, bpp, 3)), chunk(Buffer.from("PLTE"), paletteBytes),
    chunk(Buffer.from("IDAT"), deflateSync(Buffer.from(scanlines), { level: 9 })), chunk(Buffer.from("IEND"), Buffer.alloc(0))]),
  { width, height, bpp, tiles: count, columns }];
}

export function palette_png(raw: Uint8Array): [Buffer, Report] {
  if (raw.length === 0 || raw.length % 2 !== 0 || raw.length > 512)
    throw new Error("palette must contain 1..256 BGR555 entries");
  const entries = raw.length / 2;
  const palette: number[] = [];
  const view = new DataView(raw.buffer, raw.byteOffset, raw.byteLength);
  for (let offset = 0; offset < raw.length; offset += 2) {
    const value = view.getUint16(offset, true);
    if (value & 0x8000) throw new Error("palette contains a non-BGR555 high bit");
    palette.push((value & 31) << 3, (value >>> 5 & 31) << 3, (value >>> 10 & 31) << 3);
  }
  const pixels = [...Array.from({ length: entries }, (_, index) => index), ...Array(256 - entries).fill(0)];
  const scanlines: number[] = [];
  for (let row = 0; row < 16; row++) scanlines.push(0, ...pixels.slice(row * 16, (row + 1) * 16));
  return [png([chunk(Buffer.from("IHDR"), ihdr(16, 16, 8, 3)), chunk(Buffer.from("PLTE"), Buffer.from(palette)),
    chunk(Buffer.from("IDAT"), deflateSync(Buffer.from(scanlines), { level: 9 })), chunk(Buffer.from("IEND"), Buffer.alloc(0))]),
  { width: 16, height: 16, palette_entries: entries }];
}

export function byte_png(raw: Uint8Array, width: number): [Buffer, Report] {
  if (raw.length === 0 || width <= 0 || raw.length % width !== 0 || width % 8 !== 0 || (raw.length / width) % 8 !== 0)
    throw new Error("byte image dimensions must be nonzero multiples of eight");
  const height = raw.length / width;
  const palette = Buffer.from(Array.from({ length: 256 }, (_, index) => [index, index, index]).flat());
  const rows: Buffer[] = [];
  for (let row = 0; row < height; row++) rows.push(Buffer.from([0]), Buffer.from(raw.subarray(row * width, (row + 1) * width)));
  return [png([chunk(Buffer.from("IHDR"), ihdr(width, height, 8, 3)), chunk(Buffer.from("PLTE"), palette),
    chunk(Buffer.from("IDAT"), deflateSync(Buffer.concat(rows), { level: 9 })), chunk(Buffer.from("IEND"), Buffer.alloc(0))]),
  { width, height, bytes: raw.length }];
}

export function rgba_image(raw: Uint8Array, width: number): [Buffer, Report] {
  if (raw.length === 0 || raw.length % 4 !== 0 || width <= 0)
    throw new Error("RGBA input must contain whole nonempty pixels");
  const pixels = raw.length / 4;
  if (pixels % width !== 0) throw new Error("width must divide the RGBA pixel count");
  const height = pixels / width;
  const rows: Buffer[] = [];
  for (let row = 0; row < height; row++) rows.push(Buffer.from([0]), Buffer.from(raw.subarray(row * width * 4, (row + 1) * width * 4)));
  return [png([chunk(Buffer.from("IHDR"), ihdr(width, height, 8, 6)),
    chunk(Buffer.from("IDAT"), deflateSync(Buffer.concat(rows), { level: 9 })), chunk(Buffer.from("IEND"), Buffer.alloc(0))]),
  { width, height, pixels }];
}

export function palette_rgba_image(raw: Uint8Array, width: number): [Buffer, Report] {
  if (raw.length === 0 || raw.length % 2 !== 0 || width <= 0 || raw.length / 2 % width !== 0)
    throw new Error("palette RGBA dimensions must divide whole colors");
  const pixels = Buffer.alloc(raw.length * 2);
  const source = new DataView(raw.buffer, raw.byteOffset, raw.byteLength);
  for (let offset = 0; offset < raw.length; offset += 2) {
    const value = source.getUint16(offset, true), target = offset * 2;
    pixels.set([(value & 31) << 3, (value >>> 5 & 31) << 3, (value >>> 10 & 31) << 3, value & 0x8000 ? 254 : 255], target);
  }
  const [image, report] = rgba_image(pixels, width);
  report.palette_entries = raw.length / 2;
  return [image, report];
}

export function self_test(): void {
  for (const [bpp, size, columns] of [[4, 32 * 7, 7], [8, 64 * 4, 4]]) {
    const raw = Buffer.from(Array.from({ length: size }, (_, index) => (index * 37 + 11) & 255));
    const [image] = tile_png(raw, bpp, columns);
    if (!gba_graphics(image, bpp)[0].equals(raw)) throw new Error(`${bpp}bpp tile round-trip failed`);
  }
  const colors = Array.from({ length: 256 }, (_, index) => [(index & 31) << 3, (index >>> 3 & 31) << 3, (index >>> 6 & 3) << 3] as Rgb);
  let raw = Buffer.from(Array.from({ length: 64 * 4 }, (_, index) => (index * 53 + 7) & 255));
  let [image] = tile_png(raw, 8, 4, colors);
  const [tiles, palette] = gba_graphics(image, 8);
  const expectedPalette = Buffer.alloc(512);
  colors.forEach(([red, green, blue], index) => expectedPalette.writeUInt16LE((red >>> 3) | (green >>> 3) << 5 | (blue >>> 3) << 10, index * 2));
  if (!tiles.equals(raw) || !palette.equals(expectedPalette)) throw new Error("palette-correct tile round-trip failed");
  raw = Buffer.alloc(448); for (let index = 0; index < 224; index++) raw.writeUInt16LE(index * 109 & 0x7fff, index * 2);
  [image] = palette_png(raw); if (!gba_graphics(image, 8)[1].equals(raw)) throw new Error("palette round-trip failed");
  raw = Buffer.from(Array.from({ length: 128 * 64 }, (_, index) => (index * 53 + 7) & 255));
  [image] = byte_png(raw, 128); if (!Buffer.from(indexed_png(image)[2]).equals(raw)) throw new Error("indexed byte-image round-trip failed");
  raw = Buffer.from(Array.from({ length: 11 * 7 * 4 }, (_, index) => (index * 29 + 3) & 255));
  [image] = rgba_image(raw, 11); const rgba = rgba_png(image);
  if (rgba[0] !== 11 || rgba[1] !== 7 || !rgba[2].equals(raw)) throw new Error("RGBA image round-trip failed");
  raw = Buffer.alloc(256); for (let index = 0; index < 128; index++) raw.writeUInt16LE(index * 109 & 0xffff, index * 2);
  [image] = palette_rgba_image(raw, 16); if (!gba_palette_rgba(image)[0].equals(raw)) throw new Error("RGBA palette round-trip failed");
  console.log("self-test=ok");
}

function option(args: string[], ...names: string[]): string {
  const index = args.findIndex((arg) => names.includes(arg));
  if (index < 0 || index + 1 >= args.length) throw new Error(`${names.at(-1)} is required`);
  return args[index + 1];
}

function number(text: string): number {
  const value = Number(text);
  if (!Number.isInteger(value)) throw new Error(`invalid integer: ${text}`);
  return value;
}

function same(a: string, b: string): boolean {
  try { return realpathSync(a) === realpathSync(b); } catch { return resolve(a) === resolve(b); }
}

function sortedJson(value: Record<string, unknown>): string {
  const sorted = Object.fromEntries(Object.keys(value).sort().map((key) => [key, value[key]]));
  return JSON.stringify(sorted).replace(/:/g, ": ").replace(/,/g, ", ");
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: export_asset.ts [--self-test] {tiles,tiles-file,palette-file,bytes-file,rgba-file,palette-rgba-file,palette-rgba} ...");
    return;
  }
  if (args.includes("--self-test")) {
    self_test(); args = args.filter((arg) => arg !== "--self-test"); if (args.length === 0) return;
  }
  const command = args[0], input = args[1];
  if (!command || !input) throw new Error("an asset command is required");
  const output = option(args, "-o", "--output");
  let image: Buffer, report: Report;
  if (command === "tiles" || command === "palette-rgba") {
    const rom = readFileSync(input), address = number(option(args, "--address")), size = number(option(args, "--size"));
    const start = address - ROM_BASE, end = start + size;
    if (start < 0 || end > rom.length || start >= end) throw new Error(`${command === "tiles" ? "tile" : "palette"} range is outside the ROM or empty`);
    if (same(output, input)) throw new Error("refusing to overwrite the input ROM");
    if (command === "tiles") {
      const palettePath = args.includes("--palette") ? option(args, "--palette") : undefined;
      [image, report] = tile_png(rom.subarray(start, end), number(option(args, "--bpp")), number(option(args, "--columns")), palettePath ? indexed_png(readFileSync(palettePath))[3] : undefined);
    } else [image, report] = palette_rgba_image(rom.subarray(start, end), number(args.includes("--width") ? option(args, "--width") : "16"));
  } else {
    if (same(output, input)) throw new Error("refusing to overwrite the input");
    const raw = readFileSync(input);
    if (command === "tiles-file") {
      const palettePath = args.includes("--palette") ? option(args, "--palette") : undefined;
      [image, report] = tile_png(raw, number(option(args, "--bpp")), number(option(args, "--columns")), palettePath ? indexed_png(readFileSync(palettePath))[3] : undefined);
    } else if (command === "palette-file") [image, report] = palette_png(raw);
    else if (command === "bytes-file") [image, report] = byte_png(raw, number(option(args, "--width")));
    else if (command === "rgba-file") [image, report] = rgba_image(raw, number(option(args, "--width")));
    else if (command === "palette-rgba-file") [image, report] = palette_rgba_image(raw, number(args.includes("--width") ? option(args, "--width") : "16"));
    else throw new Error("an asset command is required");
  }
  mkdirSync(dirname(output), { recursive: true }); writeFileSync(output, image); console.log(sortedJson(report));
}

if (import.meta.main) main(process.argv.slice(2));
