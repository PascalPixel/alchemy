#!/usr/bin/env bun
import {
  existsSync, mkdirSync, mkdtempSync, readFileSync, realpathSync, renameSync, rmSync, writeFileSync,
} from "node:fs";
import { dirname, join, relative, resolve } from "node:path";
import { indexed_png } from "./import_asset.ts";
import { png, type Rgb } from "./skip_sprite_archive.ts";

const ROM_BASE = 0x08000000;
export const SENTOU_MENU_ADDRESS = 0x080b3940;
export const SENTOU_MENU_END = 0x080b5000;
export const SENTOU_MENU_SIZE = SENTOU_MENU_END - SENTOU_MENU_ADDRESS;

type Json = Record<string, any>;

interface GraphicSpec {
  address: number;
  size: number;
  source: string;
  frames: number;
  frame_tiles_wide: number;
  frame_tiles_high: number;
  columns: number;
}

const GRAPHICS: readonly GraphicSpec[] = [
  { address: 0x080b3940, size: 0x300, source: "hyouji_00.4bpp.png", frames: 6, frame_tiles_wide: 2, frame_tiles_high: 2, columns: 6 },
  { address: 0x080b3c40, size: 0x100, source: "hyouji_01.4bpp.png", frames: 2, frame_tiles_wide: 2, frame_tiles_high: 2, columns: 2 },
  { address: 0x080b3d40, size: 0x140, source: "moji.4bpp.png", frames: 10, frame_tiles_wide: 1, frame_tiles_high: 1, columns: 10 },
  { address: 0x080b3e80, size: 0x100, source: "hyouji_02.4bpp.png", frames: 2, frame_tiles_wide: 2, frame_tiles_high: 2, columns: 2 },
  { address: 0x080b3f80, size: 0x180, source: "hyouji_03.4bpp.png", frames: 3, frame_tiles_wide: 2, frame_tiles_high: 2, columns: 3 },
] as const;

const PALETTE: Rgb[] = Array.from({ length: 16 }, (_, index) => {
  const value = index * 8;
  return [value, value, value];
});

function hex(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function pretty(value: unknown): string {
  return `${JSON.stringify(value, null, 2)}\n`;
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
    "format", "kind", "address", "end", "size", "graphics", "cell_offsets", "row_offsets",
    "thresholds", "selector_ids", "loadouts", "sound_ids", "multipliers", "alignment",
  ], "battle-menu source");
  if (text !== pretty(value)) throw new Error("battle-menu source is not canonical JSON");
  if (value.format !== 1 || value.kind !== "golden-sun-sentou-menu-data" ||
      value.address !== hex(SENTOU_MENU_ADDRESS) || value.end !== hex(SENTOU_MENU_END) ||
      value.size !== SENTOU_MENU_SIZE) throw new Error("battle-menu extent differs");
  return value;
}

function child(indexPath: string, name: string): string {
  if (!GRAPHICS.some((item) => item.source === name)) throw new Error("battle-menu graphic name differs");
  const root = realpathSync(dirname(indexPath)), path = realpathSync(resolve(root, name));
  if (relative(root, path) !== name) throw new Error("battle-menu graphic escaped its directory");
  return path;
}

function readAtlas(path: string, spec: GraphicSpec): Buffer {
  const encoded = readFileSync(path);
  const [width, height, pixels, colors] = indexed_png(encoded);
  const frameWidth = spec.frame_tiles_wide * 8, frameHeight = spec.frame_tiles_high * 8;
  const rows = Math.ceil(spec.frames / spec.columns);
  if (width !== spec.columns * frameWidth || height !== rows * frameHeight)
    throw new Error(`${path}: atlas dimensions differ`);
  if (JSON.stringify(colors) !== JSON.stringify(PALETTE)) throw new Error(`${path}: symbolic palette differs`);
  if (pixels.some((value) => value >= 16)) throw new Error(`${path}: pixel exceeds 4bpp`);
  if (!encoded.equals(png(Buffer.from(pixels), width, height, colors)))
    throw new Error(`${path}: atlas is not a canonical source PNG`);
  const output = Buffer.alloc(spec.size);
  let target = 0;
  for (let frame = 0; frame < spec.frames; frame++) {
    const frameLeft = frame % spec.columns * frameWidth;
    const frameTop = Math.floor(frame / spec.columns) * frameHeight;
    for (let tileY = 0; tileY < spec.frame_tiles_high; tileY++) {
      for (let tileX = 0; tileX < spec.frame_tiles_wide; tileX++) {
        for (let y = 0; y < 8; y++) {
          for (let x = 0; x < 8; x += 2) {
            const offset = (frameTop + tileY * 8 + y) * width + frameLeft + tileX * 8 + x;
            output[target++] = pixels[offset] | pixels[offset + 1] << 4;
          }
        }
      }
    }
  }
  if (target !== spec.size) throw new Error("battle-menu atlas size differs");
  return output;
}

function writeAtlas(raw: Uint8Array, path: string, spec: GraphicSpec): void {
  if (raw.length !== spec.size) throw new Error("battle-menu atlas input size differs");
  const frameWidth = spec.frame_tiles_wide * 8, frameHeight = spec.frame_tiles_high * 8;
  const width = spec.columns * frameWidth, height = Math.ceil(spec.frames / spec.columns) * frameHeight;
  const pixels = Buffer.alloc(width * height);
  let source = 0;
  for (let frame = 0; frame < spec.frames; frame++) {
    const frameLeft = frame % spec.columns * frameWidth;
    const frameTop = Math.floor(frame / spec.columns) * frameHeight;
    for (let tileY = 0; tileY < spec.frame_tiles_high; tileY++) {
      for (let tileX = 0; tileX < spec.frame_tiles_wide; tileX++) {
        for (let y = 0; y < 8; y++) {
          for (let x = 0; x < 8; x += 2) {
            const value = raw[source++];
            const offset = (frameTop + tileY * 8 + y) * width + frameLeft + tileX * 8 + x;
            pixels[offset] = value & 15;
            pixels[offset + 1] = value >>> 4;
          }
        }
      }
    }
  }
  writeFileSync(path, png(pixels, width, height, PALETTE));
}

function unsignedHalfwords(values: unknown, count: number, maximum: number, label: string): Buffer {
  const output = Buffer.alloc(count * 2);
  array(values, count, label).forEach((value, index) =>
    output.writeUInt16LE(integer(value, 0, maximum, `${label} ${index}`), index * 2));
  return output;
}

function signedHalfwords(values: unknown, count: number, label: string): Buffer {
  const output = Buffer.alloc(count * 2);
  array(values, count, label).forEach((value, index) =>
    output.writeInt16LE(integer(value, -0x8000, 0x7fff, `${label} ${index}`), index * 2));
  return output;
}

function signedBytes(values: unknown, count: number, label: string): Buffer {
  return Buffer.from(array(values, count, label).map((value, index) =>
    integer(value, -0x80, 0x7f, `${label} ${index}`) & 0xff));
}

function selectorIds(value: unknown): Buffer {
  const ids = array(value, 20, "selector IDs").map((item, index) =>
    integer(item, 1, 0x7fff, `selector ID ${index}`));
  if (new Set(ids).size !== ids.length) throw new Error("selector IDs must be unique before their terminator");
  return unsignedHalfwords(ids, 20, 0x7fff, "selector IDs");
}

function loadouts(value: unknown): Buffer {
  const output = Buffer.alloc(35 * 66);
  array(value, 35, "loadouts").forEach((entry, index) => {
    exactKeys(entry, ["main_ids", "extra_ids", "group"], `loadout ${index}`);
    if (!Array.isArray(entry.main_ids) || entry.main_ids.length > 23 ||
        !Array.isArray(entry.extra_ids) || entry.extra_ids.length > 7)
      throw new Error(`loadout ${index} exceeds its terminated fields`);
    const main = entry.main_ids.map((item: unknown, position: number) =>
      integer(item, 1, 0x7fff, `loadout ${index} main ID ${position}`));
    const extra = entry.extra_ids.map((item: unknown, position: number) =>
      integer(item, 1, 0x7fff, `loadout ${index} extra ID ${position}`));
    if (new Set(main).size !== main.length || new Set(extra).size !== extra.length)
      throw new Error(`loadout ${index} contains duplicate IDs`);
    main.forEach((item: number, position: number) => output.writeUInt16LE(item, index * 66 + position * 2));
    extra.forEach((item: number, position: number) => output.writeUInt16LE(item, index * 66 + 48 + position * 2));
    output.writeUInt16LE(integer(entry.group, 0, 2, `loadout ${index} group`), index * 66 + 64);
  });
  return output;
}

export function build_sentou_menu_data(indexPath: string): [Buffer, string[]] {
  const source = document(indexPath);
  if (!Array.isArray(source.graphics) || source.graphics.length !== GRAPHICS.length)
    throw new Error("battle-menu graphic catalog differs");
  const nested: string[] = [realpathSync(indexPath)];
  const graphics = source.graphics.map((entry: unknown, index: number) => {
    exactKeys(entry, ["address", "size", "source", "frames", "frame_tiles_wide", "frame_tiles_high", "columns"],
      `battle-menu graphic ${index}`);
    const spec = GRAPHICS[index];
    if (entry.address !== hex(spec.address) || entry.size !== spec.size || entry.source !== spec.source ||
        entry.frames !== spec.frames || entry.frame_tiles_wide !== spec.frame_tiles_wide ||
        entry.frame_tiles_high !== spec.frame_tiles_high || entry.columns !== spec.columns)
      throw new Error(`battle-menu graphic ${index} layout differs`);
    const path = child(indexPath, entry.source);
    nested.push(path);
    return readAtlas(path, spec);
  });
  const cellOffsets = unsignedHalfwords(source.cell_offsets, 30, 231, "cell offsets");
  const rowOffsets = unsignedHalfwords(source.row_offsets, 5, 978, "row offsets");
  const thresholds = Buffer.concat(array(source.thresholds, 6, "threshold rows").map((row, index) =>
    signedHalfwords(row, 5, `threshold row ${index}`)));
  const selectors = selectorIds(source.selector_ids);
  const records = loadouts(source.loadouts);
  const soundIds = Buffer.from(array(source.sound_ids, 4, "sound IDs").map((value, index) =>
    integer(value, 1, 0x7f, `sound ID ${index}`)));
  const multipliers = signedBytes(source.multipliers, 13, "multipliers");
  exactKeys(source.alignment, ["address", "end", "fill"], "battle-menu alignment");
  if (source.alignment.address !== hex(0x080b4ac3) || source.alignment.end !== hex(SENTOU_MENU_END) ||
      source.alignment.fill !== 0) throw new Error("battle-menu alignment differs");
  const output = Buffer.concat([
    ...graphics,
    cellOffsets,
    rowOffsets,
    thresholds,
    selectors,
    Buffer.alloc(2),
    records,
    soundIds,
    multipliers,
    Buffer.alloc(SENTOU_MENU_END - 0x080b4ac3),
  ]);
  if (output.length !== SENTOU_MENU_SIZE) throw new Error("battle-menu package size differs");
  return [output, nested];
}

function listUntilZero(data: Buffer, offset: number, count: number): number[] {
  const values: number[] = [];
  for (let index = 0; index < count; index++) {
    const value = data.readUInt16LE(offset + index * 2);
    if (value === 0) break;
    values.push(value);
  }
  return values;
}

function writePackage(rom: Buffer, directory: string): void {
  if (rom.length !== 0x800000) throw new Error("battle-menu exporter requires the canonical 8 MiB ROM");
  const data = rom.subarray(SENTOU_MENU_ADDRESS - ROM_BASE, SENTOU_MENU_END - ROM_BASE);
  if (data.length !== SENTOU_MENU_SIZE) throw new Error("battle-menu package is outside the ROM");
  mkdirSync(directory, { recursive: true });
  for (const spec of GRAPHICS) {
    const start = spec.address - SENTOU_MENU_ADDRESS;
    writeAtlas(data.subarray(start, start + spec.size), join(directory, spec.source), spec);
  }
  const at = (address: number) => address - SENTOU_MENU_ADDRESS;
  const u16 = (address: number, count: number) =>
    Array.from({ length: count }, (_, index) => data.readUInt16LE(at(address) + index * 2));
  const i16 = (address: number, count: number) =>
    Array.from({ length: count }, (_, index) => data.readInt16LE(at(address) + index * 2));
  if (data.readUInt16LE(at(0x080b41aa)) !== 0) throw new Error("selector terminator differs");
  if (data.subarray(at(0x080b4ac3), at(SENTOU_MENU_END)).some(Boolean))
    throw new Error("battle-menu alignment is not zero");
  const source = {
    format: 1,
    kind: "golden-sun-sentou-menu-data",
    address: hex(SENTOU_MENU_ADDRESS),
    end: hex(SENTOU_MENU_END),
    size: SENTOU_MENU_SIZE,
    graphics: GRAPHICS.map((item) => ({ ...item, address: hex(item.address) })),
    cell_offsets: u16(0x080b4100, 30),
    row_offsets: u16(0x080b413c, 5),
    thresholds: Array.from({ length: 6 }, (_, row) => i16(0x080b4146 + row * 10, 5)),
    selector_ids: u16(0x080b4182, 20),
    loadouts: Array.from({ length: 35 }, (_, index) => {
      const offset = at(0x080b41ac) + index * 66;
      return {
        main_ids: listUntilZero(data, offset, 24),
        extra_ids: listUntilZero(data, offset + 48, 8),
        group: data.readUInt16LE(offset + 64),
      };
    }),
    sound_ids: Array.from(data.subarray(at(0x080b4ab2), at(0x080b4ab6))),
    multipliers: Array.from({ length: 13 }, (_, index) => data.readInt8(at(0x080b4ab6) + index)),
    alignment: { address: hex(0x080b4ac3), end: hex(SENTOU_MENU_END), fill: 0 },
  };
  const indexPath = join(directory, "index.json");
  writeFileSync(indexPath, pretty(source));
  if (!build_sentou_menu_data(indexPath)[0].equals(data)) throw new Error("exported battle-menu package differs");
}

export function export_sentou_menu_data(rom: Buffer, directory: string): void {
  const destination = resolve(directory), parent = dirname(destination);
  if (destination === parent) throw new Error("battle-menu export requires a dedicated directory");
  mkdirSync(parent, { recursive: true });
  const transaction = mkdtempSync(join(parent, ".sentou-menu-export-"));
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

export function verify_sentou_menu_data(rom: Buffer, indexPath: string): void {
  const expected = rom.subarray(SENTOU_MENU_ADDRESS - ROM_BASE, SENTOU_MENU_END - ROM_BASE);
  const [built] = build_sentou_menu_data(indexPath);
  if (!built.equals(expected)) throw new Error("battle-menu package differs from ROM");
}

export function self_test(): void {
  const temporary = mkdtempSync("/private/tmp/sentou-menu-self-test-");
  try {
    const spec = GRAPHICS[0], raw = Buffer.alloc(spec.size);
    for (let index = 0; index < raw.length; index++) raw[index] = index * 41 + 7;
    const image = join(temporary, spec.source);
    writeAtlas(raw, image, spec);
    if (!readAtlas(image, spec).equals(raw)) throw new Error("battle-menu atlas round trip failed");
    let rejected = 0;
    const reject = (action: () => void) => { try { action(); } catch { rejected++; } };
    writeFileSync(image, Buffer.concat([readFileSync(image), Buffer.from("hidden")]));
    reject(() => readAtlas(image, spec));
    reject(() => loadouts(Array.from({ length: 35 }, () => ({ main_ids: [1, 1], extra_ids: [], group: 0 }))));
    const cellOffsets = Array(30).fill(0); cellOffsets[0] = 232;
    reject(() => unsignedHalfwords(cellOffsets, 30, 231, "cell offsets"));
    const rowOffsets = Array(5).fill(0); rowOffsets[0] = 979;
    reject(() => unsignedHalfwords(rowOffsets, 5, 978, "row offsets"));
    reject(() => selectorIds(Array(20).fill(1)));
    reject(() => selectorIds([0, ...Array.from({ length: 19 }, (_, index) => index + 1)]));
    reject(() => signedHalfwords([-0x8001], 1, "signed threshold"));
    const signed = signedBytes([-0x80, -1, 0, 0x7f], 4, "signed bytes");
    if (!signed.equals(Buffer.from([0x80, 0xff, 0, 0x7f]))) throw new Error("signed-byte encoding differs");
    if (rejected !== 7) throw new Error("battle-menu adversarial validation failed");
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok");
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { self_test(); return; }
  if (args.length === 4 && args[0] === "export" && args[2] === "--directory") {
    export_sentou_menu_data(readFileSync(args[1]), args[3]);
    console.log(`source_bytes=${SENTOU_MENU_SIZE}`);
  } else if (args.length === 3 && args[0] === "verify") {
    verify_sentou_menu_data(readFileSync(args[1]), args[2]);
    console.log(`identical=true source_bytes=${SENTOU_MENU_SIZE}`);
  } else throw new Error("usage: sentou_menu_data.ts export ROM --directory DIR | verify ROM INDEX | --self-test");
}

if (import.meta.main) main(process.argv.slice(2));
