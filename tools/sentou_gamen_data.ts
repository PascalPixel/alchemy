#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { canonicalJson, isCanonicalJsonText } from "./canonical_json.ts";
import {
  existsSync, lstatSync, mkdirSync, mkdtempSync, readFileSync, realpathSync, renameSync, rmSync, writeFileSync,
} from "node:fs";
import { basename, dirname, isAbsolute, join, relative, resolve } from "node:path";
import { indexed_png } from "./import_asset.ts";
import { png, type Rgb } from "./skip_sprite_archive.ts";

const ROM_BASE = 0x08000000;
export const SENTOU_GAMEN_ADDRESS = 0x080aea4c;
export const SENTOU_GAMEN_END = 0x080b0000;
export const SENTOU_GAMEN_SIZE = SENTOU_GAMEN_END - SENTOU_GAMEN_ADDRESS;

const DISPLAY_GLYPH_ADDRESS = 0x080af20c;
const MASK_TILE_ADDRESS = 0x080af26c;
const ALIGNMENT_ADDRESS = 0x080af314;

type Json = Record<string, any>;

interface GraphicBase {
  address: number;
  size: number;
  source: string;
}

interface FrameAtlasSpec extends GraphicBase {
  role: "frame_atlas";
  frames: number;
  frame_tiles_wide: number;
  frame_tiles_high: number;
  columns: number;
}

interface FlatGraphicSpec extends GraphicBase {
  role: "horizontal_graphic" | "canvas" | "graphic" | "xor_mask_tile";
  tiles_wide: number;
  tiles_high: number;
}

type GraphicSpec = FrameAtlasSpec | FlatGraphicSpec;

const GRAPHICS: readonly GraphicSpec[] = [
  {
    address: 0x080aea4c,
    size: 0x600,
    source: "koma.4bpp.png",
    role: "frame_atlas",
    frames: 12,
    frame_tiles_wide: 2,
    frame_tiles_high: 2,
    columns: 3,
  },
  {
    address: 0x080af04c,
    size: 0x40,
    source: "obi.4bpp.png",
    role: "horizontal_graphic",
    tiles_wide: 2,
    tiles_high: 1,
  },
  {
    address: 0x080af08c,
    size: 0x100,
    source: "men.4bpp.png",
    role: "canvas",
    tiles_wide: 4,
    tiles_high: 2,
  },
  {
    address: 0x080af18c,
    size: 0x80,
    source: "hyouji.4bpp.png",
    role: "graphic",
    tiles_wide: 2,
    tiles_high: 2,
  },
  {
    address: MASK_TILE_ADDRESS,
    size: 0x20,
    source: "masuku.4bpp.png",
    role: "xor_mask_tile",
    tiles_wide: 1,
    tiles_high: 1,
  },
] as const;

const PALETTE: Rgb[] = Array.from({ length: 16 }, (_, index) => {
  const value = index * 8;
  return [value, value, value];
});

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function pretty(value: unknown): string {
  return `${canonicalJson(value)}\n`;
}

function exactKeys(value: unknown, keys: readonly string[], label: string): asserts value is Json {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${label} must be an object`);
  const actual = Object.keys(value).sort(), expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index]))
    throw new Error(`${label} has unexpected fields`);
}

function integer(value: unknown, minimum: number, maximum: number, label: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < minimum || value > maximum)
    throw new Error(`${label} is outside its range`);
  return value;
}

function array(value: unknown, length: number, label: string): unknown[] {
  if (!Array.isArray(value) || value.length !== length) throw new Error(`${label} requires ${length} entries`);
  return value;
}

function document(path: string): Json {
  const text = readFileSync(path, "utf8"), value = JSON.parse(text);
  exactKeys(value, [
    "format", "kind", "address", "end", "size", "graphics", "display_glyphs", "mask_rows",
    "auxiliary_display_glyphs", "offsets", "orders", "lookup_ids", "mode_selectors", "object_resource_ids", "alignment",
  ], "battle-screen source");
  if (!isCanonicalJsonText(text, value)) throw new Error("battle-screen source is not canonical JSON");
  if (value.format !== 1 || value.kind !== "golden-sun-sentou-gamen-data" ||
      value.address !== hexadecimal(SENTOU_GAMEN_ADDRESS) || value.end !== hexadecimal(SENTOU_GAMEN_END) ||
      value.size !== SENTOU_GAMEN_SIZE) throw new Error("battle-screen extent differs");
  return value;
}

function child(indexPath: string, name: string): string {
  if (!GRAPHICS.some((item) => item.source === name)) throw new Error("battle-screen graphic name differs");
  const prefix = basename(indexPath).replace(/index\.json$/, "");
  const root = realpathSync(dirname(indexPath)), path = realpathSync(resolve(root, prefix + name));
  if (relative(root, path) !== prefix + name) throw new Error("battle-screen graphic escaped its directory");
  return path;
}

function graphicLayout(spec: GraphicSpec): { width: number; height: number; tiles: Array<[number, number]> } {
  const tiles: Array<[number, number]> = [];
  let width: number, height: number;
  if (spec.role === "frame_atlas") {
    const frameWidth = spec.frame_tiles_wide * 8, frameHeight = spec.frame_tiles_high * 8;
    width = spec.columns * frameWidth;
    height = Math.ceil(spec.frames / spec.columns) * frameHeight;
    for (let frame = 0; frame < spec.frames; frame++) {
      const frameLeft = frame % spec.columns * frameWidth;
      const frameTop = Math.floor(frame / spec.columns) * frameHeight;
      for (let tileY = 0; tileY < spec.frame_tiles_high; tileY++)
        for (let tileX = 0; tileX < spec.frame_tiles_wide; tileX++)
          tiles.push([frameLeft + tileX * 8, frameTop + tileY * 8]);
    }
  } else {
    width = spec.tiles_wide * 8;
    height = spec.tiles_high * 8;
    for (let tileY = 0; tileY < spec.tiles_high; tileY++)
      for (let tileX = 0; tileX < spec.tiles_wide; tileX++) tiles.push([tileX * 8, tileY * 8]);
  }
  if (tiles.length * 32 !== spec.size) throw new Error("battle-screen graphic geometry differs from its extent");
  return { width, height, tiles };
}

function readGraphic(path: string, spec: GraphicSpec): Buffer {
  const encoded = readFileSync(path);
  const [width, height, pixels, colors] = indexed_png(encoded);
  const layout = graphicLayout(spec);
  if (width !== layout.width || height !== layout.height) throw new Error(`${path}: graphic dimensions differ`);
  if (JSON.stringify(colors) !== JSON.stringify(PALETTE)) throw new Error(`${path}: symbolic palette differs`);
  if (pixels.some((value) => value >= 16)) throw new Error(`${path}: pixel exceeds 4bpp`);
  if (!encoded.equals(png(Buffer.from(pixels), width, height, colors)))
    throw new Error(`${path}: graphic is not a canonical source PNG`);
  const output = Buffer.alloc(spec.size);
  let target = 0;
  for (const [left, top] of layout.tiles) {
    for (let y = 0; y < 8; y++) {
      for (let x = 0; x < 8; x += 2) {
        const offset = (top + y) * width + left + x;
        output[target++] = pixels[offset] | pixels[offset + 1] << 4;
      }
    }
  }
  if (target !== spec.size) throw new Error("battle-screen graphic size differs");
  return output;
}

function writeGraphic(raw: Uint8Array, path: string, spec: GraphicSpec): void {
  if (raw.length !== spec.size) throw new Error("battle-screen graphic input size differs");
  const layout = graphicLayout(spec), { width, height } = layout;
  const pixels = Buffer.alloc(width * height);
  let source = 0;
  for (const [left, top] of layout.tiles) {
    for (let y = 0; y < 8; y++) {
      for (let x = 0; x < 8; x += 2) {
        const value = raw[source++];
        const offset = (top + y) * width + left + x;
        pixels[offset] = value & 15;
        pixels[offset + 1] = value >>> 4;
      }
    }
  }
  writeFileSync(path, png(pixels, width, height, PALETTE));
}

function words(value: unknown, rows: number, columns: number, maximum: number, label: string): Buffer {
  const output = Buffer.alloc(rows * columns * 4);
  array(value, rows, label).forEach((row, rowIndex) => {
    array(row, columns, `${label} row ${rowIndex}`).forEach((item, columnIndex) =>
      output.writeUInt32LE(integer(item, 0, maximum, `${label} ${rowIndex}:${columnIndex}`),
        (rowIndex * columns + columnIndex) * 4));
  });
  return output;
}

function glyphCells(value: unknown, rows: number, columns: number, label: string): Buffer {
  const output = Buffer.alloc(rows * columns * 4);
  array(value, rows, label).forEach((row, rowIndex) => {
    array(row, columns, `${label} row ${rowIndex}`).forEach((cell, columnIndex) => {
      if (!Array.isArray(cell) || cell.length < 1 || cell.length > 3)
        throw new Error(`${label} ${rowIndex}:${columnIndex} requires one to three glyphs`);
      cell.forEach((item, glyphIndex) => {
        output[rowIndex * columns * 4 + columnIndex * 4 + glyphIndex] =
          integer(item, 1, 0xff, `${label} ${rowIndex}:${columnIndex}:${glyphIndex}`);
      });
    });
  });
  return output;
}

function bytes(value: unknown, count: number, maximum: number, label: string): Buffer {
  return Buffer.from(array(value, count, label).map((item, index) =>
    integer(item, 0, maximum, `${label} ${index}`)));
}

function signedBytes(value: unknown, count: number, label: string): Buffer {
  return Buffer.from(array(value, count, label).map((item, index) =>
    integer(item, -0x80, 0x7f, `${label} ${index}`) & 0xff));
}

function terminatedOrders(value: unknown): Buffer {
  const lengths = [10, 10, 19, 19];
  return Buffer.concat(array(value, 4, "orders").map((entry, index) => {
    const values = bytes(entry, lengths[index], 0xfe, `order ${index}`);
    const expected = index < 2
      ? Array.from({ length: 10 }, (_, value) => value)
      : [...Array.from({ length: 10 }, (_, value) => value), ...Array.from({ length: 9 }, (_, value) => value + 0x81)];
    const actual = [...values].sort((left, right) => left - right);
    if (actual.some((item, position) => item !== expected[position]))
      throw new Error(`order ${index} is not the required permutation`);
    return Buffer.concat([values, Buffer.from([0xff])]);
  }));
}

function modeSelectors(value: unknown): Buffer {
  const result = bytes(value, 5, 7, "mode selectors");
  if (result.some((item) => item !== 6 && item !== 7)) throw new Error("mode selector is outside its known modes");
  return result;
}

function graphics(source: Json, indexPath: string): [Map<number, Buffer>, string[]] {
  if (!Array.isArray(source.graphics) || source.graphics.length !== GRAPHICS.length)
    throw new Error("battle-screen graphic catalog differs");
  const result = new Map<number, Buffer>(), nested = [realpathSync(indexPath)];
  source.graphics.forEach((entry: unknown, index: number) => {
    const spec = GRAPHICS[index];
    exactKeys(entry, spec.role === "frame_atlas"
      ? ["address", "size", "source", "role", "frames", "frame_tiles_wide", "frame_tiles_high", "columns"]
      : ["address", "size", "source", "role", "tiles_wide", "tiles_high"], `battle-screen graphic ${index}`);
    if (entry.address !== hexadecimal(spec.address) || entry.size !== spec.size || entry.source !== spec.source ||
        entry.role !== spec.role) throw new Error(`battle-screen graphic ${index} layout differs`);
    if (spec.role === "frame_atlas" &&
        (entry.frames !== spec.frames || entry.frame_tiles_wide !== spec.frame_tiles_wide ||
         entry.frame_tiles_high !== spec.frame_tiles_high || entry.columns !== spec.columns))
      throw new Error(`battle-screen graphic ${index} frame layout differs`);
    if (spec.role !== "frame_atlas" &&
        (entry.tiles_wide !== spec.tiles_wide || entry.tiles_high !== spec.tiles_high))
      throw new Error(`battle-screen graphic ${index} layout differs`);
    const path = child(indexPath, String(entry.source));
    nested.push(path);
    result.set(spec.address, readGraphic(path, spec));
  });
  return [result, nested];
}

export function build_sentou_gamen_data(indexPath: string): [Buffer, string[]] {
  const source = document(indexPath);
  const [images, nested] = graphics(source, indexPath);
  exactKeys(source.offsets, ["x", "y"], "battle-screen offsets");
  exactKeys(source.alignment, ["address", "end", "fill"], "battle-screen alignment");
  if (source.alignment.address !== hexadecimal(ALIGNMENT_ADDRESS) ||
      source.alignment.end !== hexadecimal(SENTOU_GAMEN_END) || source.alignment.fill !== 0)
    throw new Error("battle-screen alignment differs");
  const output = Buffer.concat([
    images.get(0x080aea4c)!,
    images.get(0x080af04c)!,
    images.get(0x080af08c)!,
    images.get(0x080af18c)!,
    glyphCells(source.display_glyphs, 3, 4, "display glyphs"),
    words(source.mask_rows, 6, 2, 0xffffffff, "mask rows"),
    images.get(MASK_TILE_ADDRESS)!,
    glyphCells([source.auxiliary_display_glyphs], 1, 2, "auxiliary display glyphs"),
    signedBytes(source.offsets.x, 9, "x offsets"),
    signedBytes(source.offsets.y, 9, "y offsets"),
    terminatedOrders(source.orders),
    words(source.lookup_ids, 2, 3, 0xffff, "lookup IDs"),
    modeSelectors(source.mode_selectors),
    Buffer.alloc(3),
    words([source.object_resource_ids], 1, 4, 0xffff, "object resource IDs"),
    Buffer.alloc(SENTOU_GAMEN_END - ALIGNMENT_ADDRESS),
  ]);
  if (output.length !== SENTOU_GAMEN_SIZE) throw new Error("battle-screen package size differs");
  return [output, nested];
}

function readWords(data: Buffer, address: number, rows: number, columns: number): number[][] {
  const base = address - SENTOU_GAMEN_ADDRESS;
  return Array.from({ length: rows }, (_, row) =>
    Array.from({ length: columns }, (_, column) => data.readUInt32LE(base + (row * columns + column) * 4)));
}

function decodeGlyphCell(cell: Buffer, label: string): number[] {
  if (cell.length !== 4) throw new Error(`${label} cell extent differs`);
  const terminator = cell.indexOf(0);
  if (terminator < 1 || cell.subarray(terminator).some(Boolean))
    throw new Error(`${label} cell is not a zero-padded glyph sequence`);
  return Array.from(cell.subarray(0, terminator));
}

function readGlyphCells(data: Buffer, address: number, rows: number, columns: number, label: string): number[][][] {
  const base = address - SENTOU_GAMEN_ADDRESS;
  return Array.from({ length: rows }, (_, row) =>
    Array.from({ length: columns }, (_, column) => {
      const offset = base + (row * columns + column) * 4;
      return decodeGlyphCell(data.subarray(offset, offset + 4), `${label} ${row}:${column}`);
    }));
}

function readSignedBytes(data: Buffer, address: number, count: number): number[] {
  const base = address - SENTOU_GAMEN_ADDRESS;
  return Array.from({ length: count }, (_, index) => data.readInt8(base + index));
}

function readOrder(data: Buffer, address: number, count: number): number[] {
  const base = address - SENTOU_GAMEN_ADDRESS;
  if (data[base + count] !== 0xff) throw new Error("battle-screen order terminator differs");
  return Array.from(data.subarray(base, base + count));
}

function writePackage(rom: Buffer, directory: string): void {
  if (rom.length !== 0x800000) throw new Error("battle-screen exporter requires the canonical 8 MiB ROM");
  const data = rom.subarray(SENTOU_GAMEN_ADDRESS - ROM_BASE, SENTOU_GAMEN_END - ROM_BASE);
  if (data.length !== SENTOU_GAMEN_SIZE) throw new Error("battle-screen package is outside the ROM");
  if (data.subarray(ALIGNMENT_ADDRESS - SENTOU_GAMEN_ADDRESS).some(Boolean))
    throw new Error("battle-screen alignment is not zero");
  mkdirSync(directory, { recursive: true });
  for (const spec of GRAPHICS) {
    const start = spec.address - SENTOU_GAMEN_ADDRESS;
    writeGraphic(data.subarray(start, start + spec.size), join(directory, spec.source), spec);
  }
  const source = {
    format: 1,
    kind: "golden-sun-sentou-gamen-data",
    address: hexadecimal(SENTOU_GAMEN_ADDRESS),
    end: hexadecimal(SENTOU_GAMEN_END),
    size: SENTOU_GAMEN_SIZE,
    graphics: GRAPHICS.map((item) => ({ ...item, address: hexadecimal(item.address) })),
    display_glyphs: readGlyphCells(data, DISPLAY_GLYPH_ADDRESS, 3, 4, "display glyphs"),
    mask_rows: readWords(data, 0x080af23c, 6, 2),
    auxiliary_display_glyphs: readGlyphCells(data, 0x080af28c, 1, 2, "auxiliary display glyphs")[0],
    offsets: {
      x: readSignedBytes(data, 0x080af294, 9),
      y: readSignedBytes(data, 0x080af29d, 9),
    },
    orders: [
      readOrder(data, 0x080af2a6, 10),
      readOrder(data, 0x080af2b1, 10),
      readOrder(data, 0x080af2bc, 19),
      readOrder(data, 0x080af2d0, 19),
    ],
    lookup_ids: readWords(data, 0x080af2e4, 2, 3),
    mode_selectors: Array.from(data.subarray(0x080af2fc - SENTOU_GAMEN_ADDRESS, 0x080af301 - SENTOU_GAMEN_ADDRESS)),
    object_resource_ids: readWords(data, 0x080af304, 1, 4)[0],
    alignment: { address: hexadecimal(ALIGNMENT_ADDRESS), end: hexadecimal(SENTOU_GAMEN_END), fill: 0 },
  };
  if (data.subarray(0x080af301 - SENTOU_GAMEN_ADDRESS, 0x080af304 - SENTOU_GAMEN_ADDRESS).some(Boolean))
    throw new Error("battle-screen selector alignment is not zero");
  const indexPath = join(directory, "index.json");
  writeFileSync(indexPath, pretty(source));
  if (!build_sentou_gamen_data(indexPath)[0].equals(data)) throw new Error("exported battle-screen package differs");
}

function validDestination(path: string): void {
  if (lstatSync(path).isSymbolicLink() || !lstatSync(path).isDirectory())
    throw new Error("battle-screen export destination must be a directory");
  build_sentou_gamen_data(join(path, "index.json"));
}

export function export_sentou_gamen_data(rom: Buffer, directory: string, romPath?: string): void {
  const requested = resolve(directory), requestedParent = dirname(requested);
  if (requested === requestedParent) throw new Error("battle-screen export requires a dedicated directory");
  mkdirSync(requestedParent, { recursive: true });
  const parent = realpathSync(requestedParent), destination = join(parent, basename(requested));
  if (romPath !== undefined) {
    const source = realpathSync(romPath), relation = relative(destination, source);
    if (relation === "" || (!relation.startsWith("..") && !isAbsolute(relation)))
      throw new Error("battle-screen export directory must not contain its input ROM");
  }
  if (existsSync(destination)) validDestination(destination);
  const transaction = mkdtempSync(join(parent, ".sentou-gamen-export-"));
  const staged = join(transaction, "new"), previous = join(transaction, "previous");
  let installed = false;
  try {
    writePackage(rom, staged);
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

export function verify_sentou_gamen_data(rom: Buffer, indexPath: string): void {
  const expected = rom.subarray(SENTOU_GAMEN_ADDRESS - ROM_BASE, SENTOU_GAMEN_END - ROM_BASE);
  if (!build_sentou_gamen_data(indexPath)[0].equals(expected)) throw new Error("battle-screen package differs from ROM");
}

export function self_test(): void {
  const temporary = mkdtempSync("/private/tmp/sentou-gamen-self-test-");
  try {
    const dimensions = [[48, 64], [16, 8], [32, 16], [16, 16], [8, 8]];
    let cursor = SENTOU_GAMEN_ADDRESS;
    for (const spec of GRAPHICS) {
      const layout = graphicLayout(spec), expected = dimensions[GRAPHICS.indexOf(spec)];
      if (layout.width !== expected[0] || layout.height !== expected[1])
        throw new Error("battle-screen graphic partition differs");
      if (spec.address < MASK_TILE_ADDRESS) {
        if (spec.address !== cursor) throw new Error("battle-screen display graphics are not contiguous");
        cursor += spec.size;
      }
      const raw = Buffer.alloc(spec.size);
      for (let index = 0; index < raw.length; index++) raw[index] = index * 41 + spec.size;
      const image = join(temporary, spec.source);
      writeGraphic(raw, image, spec);
      if (!readGraphic(image, spec).equals(raw)) throw new Error("battle-screen graphic round trip failed");
    }
    if (cursor !== DISPLAY_GLYPH_ADDRESS) throw new Error("battle-screen display graphic extent differs");
    const glyphs = glyphCells([[[1, 2], [3], [4]]], 1, 3, "test glyphs");
    if (!glyphs.equals(Buffer.from([1, 2, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0])))
      throw new Error("battle-screen glyph-cell encoding differs");
    let rejected = 0;
    const reject = (action: () => void) => { try { action(); } catch { rejected++; } };
    const image = join(temporary, GRAPHICS[0].source);
    writeFileSync(image, Buffer.concat([readFileSync(image), Buffer.from("hidden")]));
    reject(() => readGraphic(image, GRAPHICS[0]));
    reject(() => words([[0, 0, 0, 0x10000]], 1, 4, 0xffff, "test words"));
    reject(() => signedBytes([0, 0, 0, 0, 0, 0, 0, 0, 128], 9, "test offsets"));
    reject(() => terminatedOrders([Array(10).fill(0xff), Array(10).fill(0), Array(19).fill(0), Array(19).fill(0)]));
    reject(() => modeSelectors([6, 7, 7, 6, 5]));
    reject(() => glyphCells([[[1, 2, 3, 4]]], 1, 1, "test glyphs"));
    reject(() => glyphCells([[[1, 0]]], 1, 1, "test glyphs"));
    reject(() => glyphCells([[[]]], 1, 1, "test glyphs"));
    reject(() => decodeGlyphCell(Buffer.from([0, 0, 0, 0]), "test glyphs"));
    reject(() => decodeGlyphCell(Buffer.from([1, 0, 2, 0]), "test glyphs"));
    reject(() => decodeGlyphCell(Buffer.from([1, 2, 3, 4]), "test glyphs"));
    if (rejected !== 11) throw new Error("battle-screen adversarial validation failed");
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok");
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { self_test(); return; }
  if (args.length === 4 && args[0] === "export" && args[2] === "--directory") {
    const romPath = resolve(args[1]);
    export_sentou_gamen_data(readFileSync(romPath), args[3], romPath);
    console.log(`source_bytes=${SENTOU_GAMEN_SIZE}`);
  } else if (args.length === 3 && args[0] === "verify") {
    verify_sentou_gamen_data(readFileSync(args[1]), args[2]);
    console.log(`identical=true source_bytes=${SENTOU_GAMEN_SIZE}`);
  } else throw new Error("usage: sentou_gamen_data.ts export ROM --directory DIR | verify ROM INDEX | --self-test");
}

if (import.meta.main) main(process.argv.slice(2));
