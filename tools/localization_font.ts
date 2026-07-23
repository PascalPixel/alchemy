#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { chunk, tile_png } from "./export_asset.ts";
import { decode_pixels, encode_pixels } from "./f0_archive.ts";
import { decode_palette_trace, encode_palette, type PaletteGroup } from "./extract_resource.ts";
import { indexed_png, gba_graphics } from "./import_asset.ts";
import { deflateSync } from "./zlib.ts";
import { canonicalJson } from "./canonical_json.ts";

export const ROM_BASE = 0x08000000;
export const PREFIX_ADDRESS = 0x08029910;
export const PREFIX_END = 0x08033e60;

const MTF_LAYOUT = [
  [0x08029a10, 0x08029acc, 3, true],
  [0x08029acc, 0x08029b68, 3, false],
  [0x08029b68, 0x08029e00, 13, true],
  [0x08029e00, 0x08029ee4, 43, false],
  [0x08029ee4, 0x0802de88, 255, true],
  [0x0802de88, 0x080308a0, 160, true],
  [0x080308a0, 0x080310a4, 32, true],
] as const;

const TILE0_ADDRESS = 0x08029910;
const TILE0_END = 0x08029a10;
const UI_TILE_ADDRESS = 0x080310a4;
const UI_TILE_END = 0x08031864;
const PACKED_ADDRESS = 0x08031864;
const PACKED_END = 0x08032224;
const FONT_ADDRESS = 0x08032224;
const FONT_END = 0x08033e24;
const ARTICLE_DATA_ADDRESS = 0x08033e24;
const ARTICLE_TABLE_ADDRESS = 0x08033e40;

interface MtfBankSource {
  address: string;
  end: string;
  entries: number[];
  terminal: boolean;
  images: number;
  columns: number;
  tail_zeros: number;
  source: string;
}

interface PackedSource {
  address: string;
  end: string;
  images: number;
  columns: number;
  tail_zeros: number;
  source: string;
  tokens: PaletteGroup[][];
}

interface FontSource {
  address: string;
  first_code: number;
  glyphs: number;
  columns: number;
  source: string;
  advances: number[];
}

interface ArticleRecord {
  offset: number;
  text: string;
}

interface ArticleSource {
  data_address: string;
  table_address: string;
  data_size: number;
  records: ArticleRecord[];
  entries: number[];
}

export interface LocalizationFontSource {
  format: 1;
  kind: "golden-sun-localization-font";
  address: string;
  size: string;
  direct_tiles: { address: string; end: string; source: string; columns: number }[];
  mtf_banks: MtfBankSource[];
  packed_images: PackedSource;
  font: FontSource;
  articles: ArticleSource;
}

function hex(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function address(value: string, expected: number, label: string): void {
  if (Number(value) !== expected) throw new Error(`${label} address differs`);
}

function exactKeys(value: object, keys: string[], label: string): void {
  if (Object.keys(value).sort().join(",") !== [...keys].sort().join(",")) {
    throw new Error(`${label} has unknown fields`);
  }
}

function imagePath(root: string, source: string): string {
  if (!/^graphics\/fonts\/localization_font\/[a-z0-9_.-]+\.png$/.test(source)) {
    throw new Error("localization-font image path differs");
  }
  return join(root, source);
}

function view(source: Uint8Array): DataView {
  return new DataView(source.buffer, source.byteOffset, source.byteLength);
}

function pngHeader(width: number, height: number, depth: number): Buffer {
  const result = Buffer.alloc(13);
  result.writeUInt32BE(width, 0);
  result.writeUInt32BE(height, 4);
  result.set([depth, 3, 0, 0, 0], 8);
  return result;
}

function indexedImage(pixels: Uint8Array, width: number, height: number, depth: 1 | 4): Buffer {
  if (width <= 0 || height <= 0 || width % 8 !== 0 || height % 8 !== 0 || pixels.length !== width * height)
    throw new Error("indexed image dimensions differ");
  const limit = 1 << depth;
  if (Array.from(pixels).some((value) => value >= limit)) throw new Error("indexed image pixel exceeds its depth");
  const palette = depth === 1 ? Buffer.from([0, 0, 0, 255, 255, 255]) :
    Buffer.from(Array.from({ length: 16 }, (_, index) => [index * 17, index * 17, index * 17]).flat());
  const rowBytes = width * depth / 8;
  const rows = Buffer.alloc(height * (rowBytes + 1));
  for (let y = 0; y < height; y++) {
    let target = y * (rowBytes + 1) + 1;
    for (let x = 0; x < width; x += 8 / depth) {
      let byte = 0;
      for (let part = 0; part < 8 / depth; part++)
        byte |= pixels[y * width + x + part] << (8 - depth * (part + 1));
      rows[target++] = byte;
    }
  }
  return Buffer.concat([
    Buffer.from("89504e470d0a1a0a", "hex"),
    chunk(Buffer.from("IHDR"), pngHeader(width, height, depth)),
    chunk(Buffer.from("PLTE"), palette),
    chunk(Buffer.from("IDAT"), deflateSync(rows, { level: 9 })),
    chunk(Buffer.from("IEND"), Buffer.alloc(0)),
  ]);
}

function atlas(frames: Uint8Array[], frameWidth: number, frameHeight: number, columns: number): Buffer {
  if (!frames.length || columns <= 0 || frames.some((frame) => frame.length !== frameWidth * frameHeight))
    throw new Error("atlas frame layout differs");
  const rows = Math.ceil(frames.length / columns);
  const width = columns * frameWidth;
  const height = rows * frameHeight;
  const pixels = Buffer.alloc(width * height);
  frames.forEach((frame, index) => {
    const left = index % columns * frameWidth;
    const top = Math.floor(index / columns) * frameHeight;
    for (let y = 0; y < frameHeight; y++)
      pixels.set(frame.subarray(y * frameWidth, (y + 1) * frameWidth), (top + y) * width + left);
  });
  return indexedImage(pixels, width, height, 4);
}

function atlasFrames(path: string, count: number, frameWidth: number, frameHeight: number, columns: number, depth: number): Buffer[] {
  const [width, height, pixels] = indexed_png(readFileSync(path));
  const rows = Math.ceil(count / columns);
  if (columns <= 0 || width !== columns * frameWidth || height !== rows * frameHeight)
    throw new Error(`${path}: atlas dimensions differ`);
  if (pixels.some((value) => value >= 2 ** depth)) throw new Error(`${path}: atlas exceeds ${depth}bpp`);
  const result: Buffer[] = [];
  for (let index = 0; index < count; index++) {
    const left = index % columns * frameWidth;
    const top = Math.floor(index / columns) * frameHeight;
    const frame = Buffer.alloc(frameWidth * frameHeight);
    for (let y = 0; y < frameHeight; y++)
      frame.set(pixels.slice((top + y) * width + left, (top + y) * width + left + frameWidth), y * frameWidth);
    result.push(frame);
  }
  return result;
}

function unpackTiles(source: Uint8Array, width: number, height: number): Buffer {
  if (width % 8 || height % 8 || source.length !== width * height / 2) throw new Error("4bpp tile extent differs");
  const pixels = Buffer.alloc(width * height);
  let cursor = 0;
  for (let top = 0; top < height; top += 8) for (let left = 0; left < width; left += 8)
    for (let y = 0; y < 8; y++) for (let x = 0; x < 8; x += 2) {
      const byte = source[cursor++];
      pixels[(top + y) * width + left + x] = byte & 15;
      pixels[(top + y) * width + left + x + 1] = byte >>> 4;
    }
  return pixels;
}

function packTiles(pixels: Uint8Array, width: number, height: number): Buffer {
  if (width % 8 || height % 8 || pixels.length !== width * height || Array.from(pixels).some((value) => value >= 16))
    throw new Error("4bpp pixels differ");
  const result: number[] = [];
  for (let top = 0; top < height; top += 8) for (let left = 0; left < width; left += 8)
    for (let y = 0; y < 8; y++) for (let x = 0; x < 8; x += 2)
      result.push(pixels[(top + y) * width + left + x] | pixels[(top + y) * width + left + x + 1] << 4);
  return Buffer.from(result);
}

function parse(value: unknown): LocalizationFontSource {
  if (typeof value !== "object" || value === null) throw new Error("localization-font source must be an object");
  const source = value as LocalizationFontSource;
  exactKeys(source, ["format", "kind", "address", "size", "direct_tiles", "mtf_banks", "packed_images", "font", "articles"], "localization-font source");
  if (source.format !== 1 || source.kind !== "golden-sun-localization-font" ||
      source.address !== hex(PREFIX_ADDRESS) || source.size !== hex(PREFIX_END - PREFIX_ADDRESS))
    throw new Error("localization-font source layout differs");
  if (!Array.isArray(source.direct_tiles) || !Array.isArray(source.mtf_banks) || source.mtf_banks.length !== MTF_LAYOUT.length)
    throw new Error("localization-font source collections differ");
  source.direct_tiles.forEach((item) => exactKeys(item, ["address", "end", "source", "columns"], "direct-tile source"));
  source.mtf_banks.forEach((item) => exactKeys(item, ["address", "end", "entries", "terminal", "images", "columns", "tail_zeros", "source"], "MTF-bank source"));
  exactKeys(source.packed_images, ["address", "end", "images", "columns", "tail_zeros", "source", "tokens"], "packed-image source");
  exactKeys(source.font, ["address", "first_code", "glyphs", "columns", "source", "advances"], "font source");
  exactKeys(source.articles, ["data_address", "table_address", "data_size", "records", "entries"], "article source");
  source.articles.records.forEach((record) => exactKeys(record, ["offset", "text"], "article record"));
  return source;
}

function directTiles(source: LocalizationFontSource, root: string): Buffer[] {
  const expected = [[TILE0_ADDRESS, TILE0_END, 2], [UI_TILE_ADDRESS, UI_TILE_END, 2]];
  if (source.direct_tiles.length !== expected.length) throw new Error("direct-tile source count differs");
  return source.direct_tiles.map((item, index) => {
    const [start, end, columns] = expected[index];
    address(item.address, start, "direct tile");
    address(item.end, end, "direct tile end");
    if (item.columns !== columns) throw new Error("direct-tile columns differ");
    const [raw, , report] = gba_graphics(readFileSync(imagePath(root, item.source)), 4);
    if (report.columns !== undefined || raw.length !== end - start) throw new Error("direct-tile source extent differs");
    return raw;
  });
}

function buildMtf(source: LocalizationFontSource, root: string): Buffer[] {
  return source.mtf_banks.map((bank, index) => {
    const [start, end, entryCount, terminal] = MTF_LAYOUT[index];
    address(bank.address, start, "MTF bank");
    address(bank.end, end, "MTF bank end");
    if (bank.entries.length !== entryCount || bank.terminal !== terminal || bank.images <= 0 ||
        bank.entries.some((entry) => !Number.isInteger(entry) || entry < 0 || entry >= bank.images) ||
        new Set(bank.entries).size !== bank.images || bank.tail_zeros < 0)
      throw new Error(`MTF bank ${index} metadata differs`);
    const frames = atlasFrames(imagePath(root, bank.source), bank.images, 16, 16, bank.columns, 4);
    const streams = frames.map(encode_pixels);
    const headerSize = bank.entries.length * 4 + (terminal ? 4 : 0);
    const pointers: number[] = [];
    let cursor = start + headerSize;
    for (const stream of streams) { pointers.push(cursor); cursor += stream.length; }
    const header = Buffer.alloc(headerSize);
    bank.entries.forEach((entry, slot) => header.writeUInt32LE(pointers[entry], slot * 4));
    if (terminal) header.writeUInt32LE(0xffffffff, bank.entries.length * 4);
    const result = Buffer.concat([header, ...streams, Buffer.alloc(bank.tail_zeros)]);
    if (result.length !== end - start || cursor + bank.tail_zeros !== end)
      throw new Error(`MTF bank ${index} extent differs`);
    return result;
  });
}

function buildPacked(source: LocalizationFontSource, root: string): Buffer {
  const packed = source.packed_images;
  address(packed.address, PACKED_ADDRESS, "packed image");
  address(packed.end, PACKED_END, "packed image end");
  if (packed.images !== 8 || packed.tokens.length !== packed.images || packed.tail_zeros < 0)
    throw new Error("packed-image metadata differs");
  const frames = atlasFrames(imagePath(root, packed.source), packed.images, 32, 32, packed.columns, 4);
  const streams = frames.map((frame, index) => encode_palette(packTiles(frame, 32, 32), packed.tokens[index]));
  const header = Buffer.alloc(packed.images * 4);
  let cursor = PACKED_ADDRESS + header.length;
  streams.forEach((stream, index) => { header.writeUInt32LE(cursor, index * 4); cursor += stream.length; });
  const result = Buffer.concat([header, ...streams, Buffer.alloc(packed.tail_zeros)]);
  if (result.length !== PACKED_END - PACKED_ADDRESS || cursor + packed.tail_zeros !== PACKED_END)
    throw new Error("packed-image extent differs");
  return result;
}

function buildFont(source: LocalizationFontSource, root: string): Buffer {
  const font = source.font;
  address(font.address, FONT_ADDRESS, "font");
  if (font.first_code !== 32 || font.glyphs !== 224 || font.advances.length !== font.glyphs ||
      font.advances.some((value) => !Number.isInteger(value) || value < 0 || value > 0xffff))
    throw new Error("font metadata differs");
  const frames = atlasFrames(imagePath(root, font.source), font.glyphs, 16, 16, font.columns, 1);
  const output = Buffer.alloc(font.glyphs * 32);
  frames.forEach((frame, index) => {
    if (frame.subarray(15 * 16).some((value) => value !== 0)) throw new Error("font atlas padding row is not blank");
    output.writeUInt16LE(font.advances[index], index * 32);
    for (let y = 0; y < 15; y++) {
      let row = 0;
      for (let x = 0; x < 16; x++) row |= frame[y * 16 + x] << (15 - x);
      output.writeUInt16LE(row, index * 32 + 2 + y * 2);
    }
  });
  if (output.length !== FONT_END - FONT_ADDRESS) throw new Error("font extent differs");
  return output;
}

function buildArticles(source: LocalizationFontSource): Buffer {
  const article = source.articles;
  address(article.data_address, ARTICLE_DATA_ADDRESS, "article data");
  address(article.table_address, ARTICLE_TABLE_ADDRESS, "article table");
  if (article.data_size !== ARTICLE_TABLE_ADDRESS - ARTICLE_DATA_ADDRESS || article.entries.length !== 8)
    throw new Error("article metadata differs");
  const data = Buffer.alloc(article.data_size);
  article.records.forEach((record) => {
    if (!Number.isInteger(record.offset) || record.offset < 0 || !/^[\x20-\x7e]*$/.test(record.text))
      throw new Error("article record differs");
    const encoded = Buffer.from(`${record.text}\0`, "ascii");
    if (record.offset + encoded.length > data.length) throw new Error("article record exceeds its data area");
    encoded.copy(data, record.offset);
  });
  const table = Buffer.alloc(article.entries.length * 4);
  article.entries.forEach((entry, index) => {
    if (!Number.isInteger(entry) || entry < 0 || entry >= article.records.length) throw new Error("article entry differs");
    table.writeUInt32LE(ARTICLE_DATA_ADDRESS + article.records[entry].offset, index * 4);
  });
  return Buffer.concat([data, table]);
}

export function build_localization_font(value: unknown, root: string): Buffer {
  const source = parse(value);
  const direct = directTiles(source, root);
  const mtf = buildMtf(source, root);
  const packed = buildPacked(source, root);
  const font = buildFont(source, root);
  const articles = buildArticles(source);
  const result = Buffer.concat([direct[0], ...mtf, direct[1], packed, font, articles]);
  if (result.length !== PREFIX_END - PREFIX_ADDRESS) throw new Error("localization-font package extent differs");
  return result;
}

function pointer(source: Uint8Array, addressValue: number): number {
  return view(source).getUint32(addressValue - ROM_BASE, true);
}

function range(source: Uint8Array, start: number, end: number): Buffer {
  if (start < ROM_BASE || end < start || end - ROM_BASE > source.length) throw new Error("ROM range differs");
  return Buffer.from(source.subarray(start - ROM_BASE, end - ROM_BASE));
}

function relative(path: string, root: string): string {
  const prefix = root.endsWith("/") ? root : `${root}/`;
  if (!path.startsWith(prefix)) throw new Error("source path is outside its root");
  return path.slice(prefix.length);
}

export function export_localization_font(rom: Uint8Array, outputRoot: string): LocalizationFontSource {
  const root = outputRoot.replace(/\/$/, "");
  const imageRoot = join(root, "graphics/fonts/localization_font");
  mkdirSync(imageRoot, { recursive: true });
  const direct: LocalizationFontSource["direct_tiles"] = [];
  for (const [name, start, end, columns] of [
    ["tile_08029910.4bpp.png", TILE0_ADDRESS, TILE0_END, 2],
    ["tile_080310a4.4bpp.png", UI_TILE_ADDRESS, UI_TILE_END, 2],
  ] as const) {
    const path = join(imageRoot, name);
    writeFileSync(path, tile_png(range(rom, start, end), 4, columns)[0]);
    direct.push({ address: hex(start), end: hex(end), source: relative(path, root), columns });
  }
  const banks: MtfBankSource[] = [];
  MTF_LAYOUT.forEach(([start, end, count, terminal], index) => {
    const slots = Array.from({ length: count }, (_, slot) => pointer(rom, start + slot * 4));
    if (terminal && pointer(rom, start + count * 4) !== 0xffffffff) throw new Error(`MTF bank ${index} lacks its terminal entry`);
    const pointers = [...new Set(slots)].sort((left, right) => left - right);
    const first = start + count * 4 + (terminal ? 4 : 0);
    if (!pointers.length || pointers[0] !== first || pointers.some((value) => value < first || value >= end))
      throw new Error(`MTF bank ${index} pointers differ`);
    const frames: Buffer[] = [];
    let encodedEnd = first;
    pointers.forEach((value, image) => {
      const bound = image + 1 < pointers.length ? pointers[image + 1] : end;
      const [pixels] = decode_pixels(range(rom, value, bound));
      if (pixels.length !== 256) throw new Error(`MTF bank ${index} glyph dimensions differ`);
      const encoded = encode_pixels(pixels);
      if (!encoded.equals(range(rom, value, value + encoded.length))) throw new Error(`MTF bank ${index} glyph replay differs`);
      if (image + 1 < pointers.length && value + encoded.length !== bound)
        throw new Error(`MTF bank ${index} has an interior gap`);
      frames.push(pixels);
      encodedEnd = value + encoded.length;
    });
    const tail = range(rom, encodedEnd, end);
    if (tail.some((value) => value !== 0)) throw new Error(`MTF bank ${index} has nonzero tail data`);
    const columns = Math.min(16, frames.length);
    const path = join(imageRoot, `mtf_${index.toString().padStart(2, "0")}.indexed.png`);
    writeFileSync(path, atlas(frames, 16, 16, columns));
    const imageByPointer = new Map(pointers.map((value, image) => [value, image]));
    banks.push({
      address: hex(start), end: hex(end), entries: slots.map((value) => imageByPointer.get(value)!), terminal,
      images: frames.length, columns, tail_zeros: tail.length, source: relative(path, root),
    });
  });
  const packedPointers = Array.from({ length: 8 }, (_, index) => pointer(rom, PACKED_ADDRESS + index * 4));
  if (packedPointers[0] !== PACKED_ADDRESS + 32 || packedPointers.some((value, index) =>
      value < PACKED_ADDRESS + 32 || value >= PACKED_END || index > 0 && value <= packedPointers[index - 1]))
    throw new Error("packed-image pointers differ");
  const packedFrames: Buffer[] = [];
  const packedTokens: PaletteGroup[][] = [];
  let packedUsed = PACKED_ADDRESS + 32;
  packedPointers.forEach((value, index) => {
    const bound = index + 1 < packedPointers.length ? packedPointers[index + 1] : PACKED_END;
    const [decoded, used, tokens] = decode_palette_trace(rom, value - ROM_BASE, bound - ROM_BASE, 512);
    const usedAddress = used + ROM_BASE;
    if (decoded.length !== 512 || !encode_palette(decoded, tokens).equals(range(rom, value, usedAddress)))
      throw new Error(`packed image ${index} differs`);
    if (index + 1 < packedPointers.length && usedAddress !== bound) throw new Error("packed images have an interior gap");
    packedFrames.push(unpackTiles(decoded, 32, 32));
    packedTokens.push(tokens);
    packedUsed = usedAddress;
  });
  const packedTail = range(rom, packedUsed, PACKED_END);
  if (packedTail.some((value) => value !== 0)) throw new Error("packed images have a nonzero tail");
  const packedPath = join(imageRoot, "packed_32x32.indexed.png");
  writeFileSync(packedPath, atlas(packedFrames, 32, 32, 4));
  const fontRaw = range(rom, FONT_ADDRESS, FONT_END);
  const fontView = view(fontRaw);
  const fontFrames: Buffer[] = [];
  const advances: number[] = [];
  for (let glyph = 0; glyph < 224; glyph++) {
    advances.push(fontView.getUint16(glyph * 32, true));
    const pixels = Buffer.alloc(16 * 16);
    for (let y = 0; y < 15; y++) {
      const row = fontView.getUint16(glyph * 32 + 2 + y * 2, true);
      for (let x = 0; x < 16; x++) pixels[y * 16 + x] = row >>> (15 - x) & 1;
    }
    fontFrames.push(pixels);
  }
  const fontColumns = 16;
  const fontPixels = Buffer.alloc(fontColumns * 16 * Math.ceil(fontFrames.length / fontColumns) * 16);
  const fontWidth = fontColumns * 16;
  fontFrames.forEach((frame, index) => {
    const left = index % fontColumns * 16;
    const top = Math.floor(index / fontColumns) * 16;
    for (let y = 0; y < 16; y++) fontPixels.set(frame.subarray(y * 16, y * 16 + 16), (top + y) * fontWidth + left);
  });
  const fontPath = join(imageRoot, "glyphs_0020_00ff.1bpp.png");
  writeFileSync(fontPath, indexedImage(fontPixels, fontWidth, Math.ceil(fontFrames.length / fontColumns) * 16, 1));
  const articleOffsets: number[] = [];
  for (let index = 0; index < 8; index++) articleOffsets.push(pointer(rom, ARTICLE_TABLE_ADDRESS + index * 4));
  const uniqueOffsets = [...new Set(articleOffsets)].sort((left, right) => left - right);
  const records: ArticleRecord[] = uniqueOffsets.map((recordAddress) => {
    if (recordAddress < ARTICLE_DATA_ADDRESS || recordAddress >= ARTICLE_TABLE_ADDRESS)
      throw new Error("article pointer differs");
    let end = recordAddress;
    while (end < ARTICLE_TABLE_ADDRESS && rom[end - ROM_BASE] !== 0) end++;
    if (end >= ARTICLE_TABLE_ADDRESS) throw new Error("article text lacks a terminator");
    const text = Buffer.from(rom.subarray(recordAddress - ROM_BASE, end - ROM_BASE)).toString("ascii");
    if (!/^[\x20-\x7e]*$/.test(text)) throw new Error("article text is not printable ASCII");
    return { offset: recordAddress - ARTICLE_DATA_ADDRESS, text };
  });
  const recordByAddress = new Map(uniqueOffsets.map((value, index) => [value, index]));
  const result: LocalizationFontSource = {
    format: 1,
    kind: "golden-sun-localization-font",
    address: hex(PREFIX_ADDRESS),
    size: hex(PREFIX_END - PREFIX_ADDRESS),
    direct_tiles: direct,
    mtf_banks: banks,
    packed_images: {
      address: hex(PACKED_ADDRESS), end: hex(PACKED_END), images: 8, columns: 4,
      tail_zeros: packedTail.length, source: relative(packedPath, root), tokens: packedTokens,
    },
    font: {
      address: hex(FONT_ADDRESS), first_code: 32, glyphs: 224, columns: fontColumns,
      source: relative(fontPath, root), advances,
    },
    articles: {
      data_address: hex(ARTICLE_DATA_ADDRESS), table_address: hex(ARTICLE_TABLE_ADDRESS),
      data_size: ARTICLE_TABLE_ADDRESS - ARTICLE_DATA_ADDRESS, records,
      entries: articleOffsets.map((value) => recordByAddress.get(value)!),
    },
  };
  if (!build_localization_font(result, root).equals(range(rom, PREFIX_ADDRESS, PREFIX_END)))
    throw new Error("exported localization-font package does not round-trip");
  return result;
}

export function self_test(): void {
  for (const depth of [1, 4] as const) {
    const limit = 1 << depth;
    const pixels = Buffer.from(Array.from({ length: 32 * 16 }, (_, index) => index * 7 % limit));
    const [width, height, decoded] = indexed_png(indexedImage(pixels, 32, 16, depth));
    if (width !== 32 || height !== 16 || !Buffer.from(decoded).equals(pixels))
      throw new Error(`${depth}bpp indexed-image self-test failed`);
  }
  const pixels = Buffer.from(Array.from({ length: 32 * 32 }, (_, index) => index * 11 & 15));
  if (!unpackTiles(packTiles(pixels, 32, 32), 32, 32).equals(pixels))
    throw new Error("localization tile self-test failed");
  let rejected = false;
  try { imagePath("assets", "../private.png"); } catch { rejected = true; }
  if (!rejected) throw new Error("localization-font path escape was accepted");
  console.log("self-test=ok");
}

function option(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : undefined;
}

function main(args: string[]): void {
  if (args.includes("--self-test")) {
    self_test();
    args = args.filter((item) => item !== "--self-test");
    if (args.length === 0) return;
  }
  const command = args[0];
  if (command === "export") {
    const romPath = args[1];
    const output = option(args, "--output");
    const root = option(args, "--root");
    if (!romPath || !output || !root) throw new Error("usage: localization_font.ts export ROM --root ASSETS --output SOURCE");
    const source = export_localization_font(readFileSync(romPath), root);
    mkdirSync(dirname(output), { recursive: true });
    writeFileSync(output, `${canonicalJson(source)}\n`);
    console.log(`identical=true bytes=${PREFIX_END - PREFIX_ADDRESS} glyphs=224 mtf_images=${source.mtf_banks.reduce((sum, bank) => sum + bank.images, 0)}`);
    return;
  }
  if (command === "build") {
    const input = args[1];
    const output = option(args, "--output");
    const root = option(args, "--root");
    if (!input || !output || !root) throw new Error("usage: localization_font.ts build SOURCE --root ASSETS --output FILE");
    const built = build_localization_font(JSON.parse(readFileSync(input, "utf8")), root);
    mkdirSync(dirname(output), { recursive: true });
    writeFileSync(output, built);
    console.log(`bytes=${built.length}`);
    return;
  }
  if (command === "verify") {
    const romPath = args[1];
    const input = args[2];
    const root = option(args, "--root");
    if (!romPath || !input || !root) throw new Error("usage: localization_font.ts verify ROM SOURCE --root ASSETS");
    const built = build_localization_font(JSON.parse(readFileSync(input, "utf8")), root);
    if (!built.equals(range(readFileSync(romPath), PREFIX_ADDRESS, PREFIX_END))) throw new Error("localization-font source differs from ROM");
    console.log(`identical=true bytes=${built.length}`);
    return;
  }
  throw new Error("usage: localization_font.ts {export ROM --root ASSETS --output SOURCE|build SOURCE --root ASSETS --output FILE|verify ROM SOURCE --root ASSETS|--self-test}");
}

if (import.meta.main) main(process.argv.slice(2));
