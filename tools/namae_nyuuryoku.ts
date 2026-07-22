#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import {
  decode_general_trace,
  encode_general,
  type GeneralToken,
} from "./extract_resource.ts";

export const ROM_BASE = 0x08000000;
export const REGION_ADDRESS = 0x08073852;
export const RESOURCE_TABLE_ADDRESS = 0x08073854;
export const TILEMAP_ADDRESS = 0x08073864;
export const TILE_POINTER_ADDRESS = 0x08073968;
export const REGION_END = 0x08077000;

const RESOURCE_IDS = [0x05a, 0x05b, 0x05c, 0x05d] as const;
const UI_TILE_ADDRESS = 0x080310a4;
const TILEMAP_WIDTH = 24;
const TILEMAP_HEIGHT = 13;
const COPIED_WIDTH = 24;
const COPIED_HEIGHT = 9;
const TILEMAP_ENTRIES = TILEMAP_WIDTH * TILEMAP_HEIGHT;
const TILEMAP_BYTES = TILEMAP_ENTRIES * 2;

interface TilemapSource {
  address: string;
  codec: "golden-sun-general-lz";
  decoded_size: number;
  width: number;
  height: number;
  copied_width: number;
  copied_height: number;
  palette: number;
  tiles: number[][];
  horizontal_flips: number[][];
  vertical_flips: number[][];
  tokens: GeneralToken[];
}

export interface NamaeNyuuryokuSource {
  format: 1;
  kind: "golden-sun-namae-nyuuryoku";
  address: string;
  end: string;
  resource_ids: string[];
  tilemap: TilemapSource;
  ui_tile_address: string;
  next_code_address: string;
}

function hex(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function resourceId(value: number): string {
  return `0x${value.toString(16).padStart(3, "0")}`;
}

function integer(value: unknown, label: string): number {
  if (!Number.isSafeInteger(value)) throw new Error(`${label} must be an integer`);
  return value as number;
}

function exactKeys(value: object, keys: string[], label: string): void {
  if (Object.keys(value).sort().join(",") !== [...keys].sort().join(",")) {
    throw new Error(`${label} has unknown fields`);
  }
}

function align4(value: number): number {
  return (value + 3) & ~3;
}

function romRange(rom: Uint8Array, start: number, end: number): Buffer {
  const first = start - ROM_BASE;
  const last = end - ROM_BASE;
  if (first < 0 || last > rom.length || first > last) throw new Error("ROM range differs");
  return Buffer.from(rom.subarray(first, last));
}

function coordinateSet(value: unknown, rows: number[][], label: string): Set<string> {
  if (!Array.isArray(value)) throw new Error(`${label} must be an array`);
  const result = new Set<string>();
  for (const raw of value) {
    if (!Array.isArray(raw) || raw.length !== 2) throw new Error(`${label} coordinate differs`);
    const row = integer(raw[0], `${label} row`);
    const column = integer(raw[1], `${label} column`);
    if (row < 0 || row >= rows.length || column < 0 || column >= rows[row].length) {
      throw new Error(`${label} coordinate is outside the tilemap`);
    }
    const key = `${row},${column}`;
    if (result.has(key)) throw new Error(`${label} has a duplicate coordinate`);
    result.add(key);
  }
  return result;
}

function generalTokens(value: unknown): GeneralToken[] {
  if (!Array.isArray(value) || value.length < 1 || value.length > TILEMAP_BYTES) {
    throw new Error("name-entry token plan length differs");
  }
  return value.map((raw, index) => {
    if (!Array.isArray(raw) || (raw[0] !== "l" && raw[0] !== "c")) {
      throw new Error(`name-entry token ${index} opcode differs`);
    }
    if (raw[0] === "l") {
      if (raw.length !== 2) throw new Error(`name-entry token ${index} literal tuple differs`);
      const count = integer(raw[1], `name-entry token ${index} literal count`);
      if (count < 1 || count > TILEMAP_BYTES) {
        throw new Error(`name-entry token ${index} literal count is outside its range`);
      }
      return ["l", count] as GeneralToken;
    }
    if (raw.length !== 3) throw new Error(`name-entry token ${index} copy tuple differs`);
    const length = integer(raw[1], `name-entry token ${index} copy length`);
    const distance = integer(raw[2], `name-entry token ${index} copy distance`);
    if (length < 2 || length > 137) {
      throw new Error(`name-entry token ${index} copy length is outside its range`);
    }
    if (distance < 1 || distance > TILEMAP_BYTES) {
      throw new Error(`name-entry token ${index} copy distance is outside its range`);
    }
    return ["c", length, distance] as GeneralToken;
  });
}

function tilemapData(source: TilemapSource): Buffer {
  exactKeys(source, [
    "address", "codec", "decoded_size", "width", "height", "copied_width", "copied_height",
    "palette", "tiles", "horizontal_flips", "vertical_flips", "tokens",
  ], "name-entry tilemap");
  if (source.address !== hex(TILEMAP_ADDRESS) || source.codec !== "golden-sun-general-lz" ||
      source.decoded_size !== TILEMAP_BYTES || source.width !== TILEMAP_WIDTH ||
      source.height !== TILEMAP_HEIGHT || source.copied_width !== COPIED_WIDTH ||
      source.copied_height !== COPIED_HEIGHT || source.palette !== 15 || !Array.isArray(source.tiles)) {
    throw new Error("name-entry tilemap layout differs");
  }
  if (source.tiles.length !== TILEMAP_HEIGHT || source.tiles.some((row) =>
      !Array.isArray(row) || row.length !== TILEMAP_WIDTH)) {
    throw new Error("name-entry tilemap row extents differ");
  }
  const horizontal = coordinateSet(source.horizontal_flips, source.tiles, "horizontal flips");
  const vertical = coordinateSet(source.vertical_flips, source.tiles, "vertical flips");
  const output = Buffer.alloc(TILEMAP_BYTES);
  let cursor = 0;
  source.tiles.forEach((row, rowIndex) => row.forEach((raw, column) => {
    const tile = integer(raw, "tile index");
    if (tile < 0 || tile > 0x03ff) throw new Error("tile index is outside the screen-entry range");
    const key = `${rowIndex},${column}`;
    const entry = tile | source.palette << 12 |
      (horizontal.has(key) ? 0x0400 : 0) | (vertical.has(key) ? 0x0800 : 0);
    output.writeUInt16LE(entry, cursor * 2);
    cursor++;
  }));
  return output;
}

export function build_namae_nyuuryoku(value: unknown): Buffer {
  if (typeof value !== "object" || value === null) throw new Error("name-entry source must be an object");
  const source = value as NamaeNyuuryokuSource;
  exactKeys(source, [
    "format", "kind", "address", "end", "resource_ids", "tilemap",
    "ui_tile_address", "next_code_address",
  ], "name-entry source");
  if (source.format !== 1 || source.kind !== "golden-sun-namae-nyuuryoku" ||
      source.address !== hex(REGION_ADDRESS) || source.end !== hex(REGION_END) ||
      source.next_code_address !== hex(REGION_END) || source.ui_tile_address !== hex(UI_TILE_ADDRESS) ||
      !Array.isArray(source.resource_ids) ||
      JSON.stringify(source.resource_ids) !== JSON.stringify(RESOURCE_IDS.map(resourceId))) {
    throw new Error("name-entry source metadata differs");
  }
  if (RESOURCE_TABLE_ADDRESS - REGION_ADDRESS !== 2 || RESOURCE_TABLE_ADDRESS % 4 !== 0 ||
      TILEMAP_ADDRESS !== RESOURCE_TABLE_ADDRESS + RESOURCE_IDS.length * 4) {
    throw new Error("name-entry structural alignment differs");
  }
  const decoded = tilemapData(source.tilemap);
  const compressed = encode_general(decoded, generalTokens(source.tilemap.tokens));
  if (align4(TILEMAP_ADDRESS + compressed.length) !== TILE_POINTER_ADDRESS) {
    throw new Error("name-entry compressed stream does not reach its aligned pointer table");
  }
  const output = Buffer.alloc(REGION_END - REGION_ADDRESS);
  RESOURCE_IDS.forEach((value, index) =>
    output.writeUInt32LE(value, RESOURCE_TABLE_ADDRESS - REGION_ADDRESS + index * 4));
  output.set(compressed, TILEMAP_ADDRESS - REGION_ADDRESS);
  output.writeUInt32LE(UI_TILE_ADDRESS, TILE_POINTER_ADDRESS - REGION_ADDRESS);
  return output;
}

function tileRows(words: number[]): number[][] {
  const rows: number[][] = [];
  for (let cursor = 0; cursor < words.length; cursor += TILEMAP_WIDTH) {
    rows.push(words.slice(cursor, cursor + TILEMAP_WIDTH).map((value) => value & 0x03ff));
  }
  return rows;
}

function flipCoordinates(words: number[], mask: number): number[][] {
  const result: number[][] = [];
  words.forEach((value, index) => {
    if (value & mask) result.push([Math.floor(index / TILEMAP_WIDTH), index % TILEMAP_WIDTH]);
  });
  return result;
}

export function export_namae_nyuuryoku(rom: Uint8Array): NamaeNyuuryokuSource {
  const region = romRange(rom, REGION_ADDRESS, REGION_END);
  if (region[0] !== 0 || region[1] !== 0) throw new Error("name-entry leading alignment is not zero");
  const resources = RESOURCE_IDS.map((_, index) =>
    region.readUInt32LE(RESOURCE_TABLE_ADDRESS - REGION_ADDRESS + index * 4));
  if (resources.some((value, index) => value !== RESOURCE_IDS[index])) {
    throw new Error("name-entry resource table differs");
  }
  const streamStart = TILEMAP_ADDRESS - ROM_BASE;
  const pointerStart = TILE_POINTER_ADDRESS - ROM_BASE;
  const [decoded, used, tokens] = decode_general_trace(rom, streamStart, pointerStart, 0x10000);
  const checkedTokens = generalTokens(tokens);
  const compressed = encode_general(decoded, checkedTokens);
  if (decoded.length !== TILEMAP_BYTES || used > pointerStart ||
      !compressed.equals(Buffer.from(rom.subarray(streamStart, streamStart + compressed.length)))) {
    throw new Error("name-entry compressed tilemap differs");
  }
  const aligned = align4(TILEMAP_ADDRESS + compressed.length);
  if (aligned !== TILE_POINTER_ADDRESS ||
      rom.subarray(TILEMAP_ADDRESS - ROM_BASE + compressed.length, pointerStart).some((value) => value !== 0)) {
    throw new Error("name-entry compressed-stream alignment differs");
  }
  if (region.readUInt32LE(TILE_POINTER_ADDRESS - REGION_ADDRESS) !== UI_TILE_ADDRESS ||
      region.subarray(TILE_POINTER_ADDRESS - REGION_ADDRESS + 4).some((value) => value !== 0)) {
    throw new Error("name-entry tile pointer or trailing fill differs");
  }
  const view = new DataView(decoded.buffer, decoded.byteOffset, decoded.byteLength);
  const words = Array.from({ length: TILEMAP_ENTRIES }, (_, index) => view.getUint16(index * 2, true));
  const palettes = new Set(words.map((value) => value >>> 12));
  if (palettes.size !== 1 || !palettes.has(15)) throw new Error("name-entry tilemap palette differs");
  const source: NamaeNyuuryokuSource = {
    format: 1,
    kind: "golden-sun-namae-nyuuryoku",
    address: hex(REGION_ADDRESS),
    end: hex(REGION_END),
    resource_ids: RESOURCE_IDS.map(resourceId),
    tilemap: {
      address: hex(TILEMAP_ADDRESS),
      codec: "golden-sun-general-lz",
      decoded_size: TILEMAP_BYTES,
      width: TILEMAP_WIDTH,
      height: TILEMAP_HEIGHT,
      copied_width: COPIED_WIDTH,
      copied_height: COPIED_HEIGHT,
      palette: 15,
      tiles: tileRows(words),
      horizontal_flips: flipCoordinates(words, 0x0400),
      vertical_flips: flipCoordinates(words, 0x0800),
      tokens: checkedTokens,
    },
    ui_tile_address: hex(UI_TILE_ADDRESS),
    next_code_address: hex(REGION_END),
  };
  if (!build_namae_nyuuryoku(source).equals(region)) throw new Error("exported name-entry source differs from ROM");
  return source;
}

function option(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : undefined;
}

function selfTest(): void {
  if (align4(TILEMAP_ADDRESS + 257) !== TILE_POINTER_ADDRESS ||
      coordinateSet([[0, 1]], [[0, 0]], "coordinates").size !== 1)
    throw new Error("name-entry structural self-test failed");
  let rejected = false;
  try { coordinateSet([[0, 0], [0, 0]], [[0]], "coordinates"); } catch { rejected = true; }
  if (!rejected) throw new Error("name-entry duplicate coordinate was accepted");
  rejected = false;
  try { exactKeys({ format: 1, extra: 0 }, ["format"], "source"); } catch { rejected = true; }
  if (!rejected) throw new Error("name-entry unknown field was accepted");
  console.log("self-test=ok");
}

async function readRom(path: string): Promise<Buffer> {
  return Buffer.from(await Bun.file(path).arrayBuffer());
}

async function readSource(path: string): Promise<unknown> {
  return JSON.parse(await Bun.file(path).text());
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    selfTest();
    return;
  }
  const command = args[0];
  if (command === "export") {
    const romPath = args[1];
    const output = option(args, "--output");
    if (!romPath || !output) throw new Error("usage: namae_nyuuryoku.ts export ROM --output SOURCE");
    const source = export_namae_nyuuryoku(await readRom(romPath));
    await Bun.write(output, `${JSON.stringify(source, null, 2)}\n`);
    const built = build_namae_nyuuryoku(source);
    console.log(`identical=true bytes=${built.length} tilemap_entries=${TILEMAP_ENTRIES}`);
    return;
  }
  if (command === "verify") {
    const romPath = args[1];
    const sourcePath = args[2];
    if (!romPath || !sourcePath) throw new Error("usage: namae_nyuuryoku.ts verify ROM SOURCE");
    const rom = await readRom(romPath);
    const source = await readSource(sourcePath);
    const built = build_namae_nyuuryoku(source);
    if (!built.equals(romRange(rom, REGION_ADDRESS, REGION_END))) throw new Error("name-entry source differs from ROM");
    const tilemap = (source as NamaeNyuuryokuSource).tilemap;
    const compressed = encode_general(tilemapData(tilemap), generalTokens(tilemap.tokens));
    console.log(`identical=true bytes=${built.length} compressed=${compressed.length} decoded=${TILEMAP_BYTES} resources=${RESOURCE_IDS.length}`);
    return;
  }
  throw new Error("usage: namae_nyuuryoku.ts {export ROM --output SOURCE|verify ROM SOURCE}");
}

if (import.meta.main) await main(process.argv.slice(2));
