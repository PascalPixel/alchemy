#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import {
  existsSync,
  mkdirSync,
  mkdtempSync,
  readFileSync,
  realpathSync,
  renameSync,
  rmSync,
  writeFileSync,
} from "node:fs";
import { dirname, join, relative, resolve } from "node:path";
import { indexed_png } from "./import_asset.ts";
import { png, type Rgb } from "./skip_sprite_archive.ts";

const ROM_BASE = 0x08000000;
export const STAFF_ROLL_ADDRESS = 0x080f0a5c;
const RESOURCE_END = 0x080f0ae0;
const TEXT_ADDRESS = 0x080f0ae0;
const WIDTH_ADDRESS = 0x080f11bd;
const WIDTH_END = 0x080f121d;
const LINE_ADDRESS = 0x080f1220;
const FONT_ADDRESS = 0x080f1770;
const ALIGNMENT_ADDRESS = 0x080f1a70;
export const STAFF_ROLL_END = 0x080f2000;
export const STAFF_ROLL_SIZE = STAFF_ROLL_END - STAFF_ROLL_ADDRESS;

const RESOURCE_COUNT = 32;
const STRING_COUNT = 110;
const LINE_COUNT = 339;
const GLYPH_COUNT = 96;
const FONT_COLUMNS = 16;
const FONT_SOURCE = "moji.1bpp.png";
const PALETTE: Rgb[] = [[0, 0, 0], [255, 255, 255]];
const JsonObject = Object as unknown as { keys(value: unknown): string[] };
const TMPDIR = Bun.env.TMPDIR ?? Bun.env.TMP ?? Bun.env.TEMP ?? "/tmp";

type Json = Record<string, any>;

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function pretty(value: unknown): string {
  return `${JSON.stringify(value, null, 2)}\n`;
}

function exactKeys(value: unknown, keys: readonly string[], name: string): asserts value is Json {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${name} must be an object`);
  const actual = JsonObject.keys(value).sort(), expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index]))
    throw new Error(`${name} has unknown or missing fields`);
}

function integer(value: unknown, minimum: number, maximum: number, name: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < minimum || value > maximum)
    throw new Error(`${name} is outside its range`);
  return value;
}

function array(value: unknown, count: number, name: string): unknown[] {
  if (!Array.isArray(value) || value.length !== count) throw new Error(`${name} requires ${count} entries`);
  return value;
}

function document(path: string): Json {
  const text = readFileSync(path, "utf8"), value = JSON.parse(text);
  if (text !== pretty(value)) throw new Error(`${path}: source is not canonical JSON`);
  exactKeys(value, [
    "format", "kind", "address", "end", "size", "preload_table", "text_pool", "glyph_widths", "lines", "font", "alignment",
  ], "staff-roll source");
  if (value.format !== 1 || value.kind !== "golden-sun-staff-roll" ||
      value.address !== hexadecimal(STAFF_ROLL_ADDRESS) || value.end !== hexadecimal(STAFF_ROLL_END) ||
      value.size !== STAFF_ROLL_SIZE) throw new Error("staff-roll extent differs");
  return value;
}

function sourcePath(indexPath: string, name: string): string {
  if (name !== FONT_SOURCE) throw new Error("staff-roll font source differs");
  const root = realpathSync(dirname(indexPath)), path = realpathSync(resolve(root, name));
  if (relative(root, path) !== name) throw new Error("staff-roll font escaped its directory");
  return path;
}

function encodeResources(value: unknown): Buffer {
  exactKeys(value, ["address", "end", "slots", "resource_ids", "terminator"], "preload resource table");
  if (value.address !== hexadecimal(STAFF_ROLL_ADDRESS) || value.end !== hexadecimal(RESOURCE_END) ||
      value.slots !== RESOURCE_COUNT + 1 || value.terminator !== 0)
    throw new Error("preload resource-table layout differs");
  const ids = array(value.resource_ids, RESOURCE_COUNT, "preload resource IDs").map((item, index) =>
    integer(item, 1, 0xffff, `preload resource ID ${index}`));
  if (new Set(ids).size !== ids.length) throw new Error("preload resource IDs must be unique");
  const output = Buffer.alloc(RESOURCE_END - STAFF_ROLL_ADDRESS);
  ids.forEach((id, index) => output.writeUInt32LE(id, index * 4));
  return output;
}

function encodeTextPool(value: unknown): { data: Buffer; addresses: number[] } {
  exactKeys(value, ["address", "end", "alignment", "strings"], "staff-roll text pool");
  if (value.address !== hexadecimal(TEXT_ADDRESS) || value.end !== hexadecimal(WIDTH_ADDRESS) || value.alignment !== 4)
    throw new Error("staff-roll text-pool layout differs");
  const strings = array(value.strings, STRING_COUNT, "staff-roll strings");
  const chunks: Buffer[] = [], addresses: number[] = [];
  let address = TEXT_ADDRESS;
  strings.forEach((item, index) => {
    if (typeof item !== "string" || [...item].some((character) => {
      const code = character.charCodeAt(0);
      return code < 0x20 || code > 0x7e;
    })) throw new Error(`staff-roll string ${index} is not printable ASCII`);
    const encoded = Buffer.from(item, "ascii");
    if (encoded.toString("ascii") !== item) throw new Error(`staff-roll string ${index} is not exact ASCII`);
    addresses.push(address);
    chunks.push(encoded, Buffer.from([0]));
    address += encoded.length + 1;
    if (index + 1 !== strings.length) {
      const padding = -address & 3;
      if (padding !== 0) chunks.push(Buffer.alloc(padding));
      address += padding;
    }
  });
  const data = Buffer.concat(chunks);
  if (address !== WIDTH_ADDRESS || data.length !== WIDTH_ADDRESS - TEXT_ADDRESS)
    throw new Error("staff-roll text pool has the wrong encoded size");
  return { data, addresses };
}

function encodeLines(value: unknown, addresses: readonly number[]): Buffer {
  exactKeys(value, ["address", "count", "string_indices", "terminator"], "staff-roll lines");
  if (value.address !== hexadecimal(LINE_ADDRESS) || value.count !== LINE_COUNT || value.terminator !== 0)
    throw new Error("staff-roll line-table layout differs");
  const indices = array(value.string_indices, LINE_COUNT, "staff-roll line indices").map((item, index) =>
    integer(item, 0, STRING_COUNT - 1, `staff-roll line index ${index}`));
  if (new Set(indices).size !== STRING_COUNT) throw new Error("staff-roll line table must reference every string");
  const output = Buffer.alloc((LINE_COUNT + 1) * 4);
  indices.forEach((stringIndex, index) => output.writeUInt32LE(addresses[stringIndex], index * 4));
  return output;
}

function encodeGlyphWidths(value: unknown): Buffer {
  exactKeys(value, ["address", "end", "count", "values", "padding"], "staff-roll glyph widths");
  exactKeys(value.padding, ["end", "size", "fill"], "staff-roll glyph-width padding");
  if (value.address !== hexadecimal(WIDTH_ADDRESS) || value.end !== hexadecimal(WIDTH_END) ||
      value.count !== GLYPH_COUNT || value.padding.end !== hexadecimal(LINE_ADDRESS) ||
      value.padding.size !== LINE_ADDRESS - WIDTH_END || value.padding.fill !== 0)
    throw new Error("staff-roll glyph-width layout differs");
  const widths = array(value.values, GLYPH_COUNT, "staff-roll glyph widths").map((item, index) =>
    integer(item, 1, 8, `staff-roll glyph width ${index}`));
  return Buffer.concat([Buffer.from(widths), Buffer.alloc(value.padding.size)]);
}

function encodeFont(value: unknown, indexPath: string): { data: Buffer; path: string } {
  exactKeys(value, [
    "address", "end", "source", "encoding", "first_code", "glyphs", "columns",
  ], "staff-roll font");
  if (value.address !== hexadecimal(FONT_ADDRESS) || value.end !== hexadecimal(ALIGNMENT_ADDRESS) ||
      value.source !== FONT_SOURCE || value.encoding !== "gba-1bpp-msb-glyph-rows" || value.first_code !== 32 ||
      value.glyphs !== GLYPH_COUNT || value.columns !== FONT_COLUMNS)
    throw new Error("staff-roll font layout differs");
  const path = sourcePath(indexPath, value.source), encoded = readFileSync(path);
  const [width, height, pixels, colors] = indexed_png(encoded);
  const expectedWidth = FONT_COLUMNS * 8, expectedHeight = Math.ceil(GLYPH_COUNT / FONT_COLUMNS) * 8;
  if (width !== expectedWidth || height !== expectedHeight) throw new Error("staff-roll font dimensions differ");
  if (JSON.stringify(colors) !== JSON.stringify(PALETTE)) throw new Error("staff-roll font palette differs");
  if (pixels.some((pixel) => pixel > 1)) throw new Error("staff-roll font exceeds one bit per pixel");
  if (!encoded.equals(png(Buffer.from(pixels), width, height, PALETTE)))
    throw new Error("staff-roll font is not a canonical source PNG");
  const rows = Buffer.alloc(GLYPH_COUNT * 8);
  for (let glyph = 0; glyph < GLYPH_COUNT; glyph++) {
    const left = glyph % FONT_COLUMNS * 8, top = Math.floor(glyph / FONT_COLUMNS) * 8;
    for (let y = 0; y < 8; y++) {
      let row = 0;
      for (let x = 0; x < 8; x++) row |= pixels[(top + y) * width + left + x] << (7 - x);
      rows[glyph * 8 + y] = row;
    }
  }
  return { data: rows, path };
}

export function build_staff_roll(indexPath: string): [Buffer, string[]] {
  const source = document(indexPath);
  const resources = encodeResources(source.preload_table);
  const text = encodeTextPool(source.text_pool);
  const widths = encodeGlyphWidths(source.glyph_widths);
  const lines = encodeLines(source.lines, text.addresses);
  const font = encodeFont(source.font, indexPath);
  exactKeys(source.alignment, ["address", "end", "fill"], "staff-roll alignment");
  if (source.alignment.address !== hexadecimal(ALIGNMENT_ADDRESS) ||
      source.alignment.end !== hexadecimal(STAFF_ROLL_END) || source.alignment.fill !== 0)
    throw new Error("staff-roll alignment differs");
  const output = Buffer.concat([
    resources,
    text.data,
    widths,
    lines,
    font.data,
    Buffer.alloc(STAFF_ROLL_END - ALIGNMENT_ADDRESS),
  ]);
  if (output.length !== STAFF_ROLL_SIZE) throw new Error("staff-roll encoded size differs");
  return [output, [realpathSync(indexPath), font.path]];
}

function fontPng(rows: Uint8Array): Buffer {
  if (rows.length !== GLYPH_COUNT * 8) throw new Error("staff-roll font input size differs");
  const width = FONT_COLUMNS * 8, height = Math.ceil(GLYPH_COUNT / FONT_COLUMNS) * 8;
  const pixels = Buffer.alloc(width * height);
  for (let glyph = 0; glyph < GLYPH_COUNT; glyph++) {
    const left = glyph % FONT_COLUMNS * 8, top = Math.floor(glyph / FONT_COLUMNS) * 8;
    for (let y = 0; y < 8; y++) {
      const row = rows[glyph * 8 + y];
      for (let x = 0; x < 8; x++) pixels[(top + y) * width + left + x] = row >>> (7 - x) & 1;
    }
  }
  return png(pixels, width, height, PALETTE);
}

function readRom(path: string): Buffer {
  const rom = readFileSync(realpathSync(path));
  if (rom.length < STAFF_ROLL_END - ROM_BASE) throw new Error("ROM is shorter than the staff-roll region");
  return rom;
}

function extractSource(rom: Buffer): { index: Json; font: Buffer; raw: Buffer } {
  const offset = (address: number) => address - ROM_BASE;
  const raw = rom.subarray(offset(STAFF_ROLL_ADDRESS), offset(STAFF_ROLL_END));
  const resources: number[] = [];
  for (let index = 0; index < RESOURCE_COUNT; index++)
    resources.push(rom.readUInt32LE(offset(STAFF_ROLL_ADDRESS) + index * 4));
  if (resources.some((id) => id === 0 || id > 0xffff) || new Set(resources).size !== resources.length ||
      rom.readUInt32LE(offset(RESOURCE_END) - 4) !== 0)
    throw new Error("ROM staff-roll preload table differs");

  const pointers: number[] = [];
  for (let index = 0; index < LINE_COUNT; index++) pointers.push(rom.readUInt32LE(offset(LINE_ADDRESS) + index * 4));
  if (rom.readUInt32LE(offset(LINE_ADDRESS) + LINE_COUNT * 4) !== 0)
    throw new Error("ROM staff-roll line terminator differs");
  const addresses = [...new Set(pointers)].sort((left, right) => left - right);
  if (addresses.length !== STRING_COUNT || addresses[0] !== TEXT_ADDRESS ||
      addresses.some((address) => address < TEXT_ADDRESS || address >= WIDTH_ADDRESS || (address & 3) !== 0))
    throw new Error("ROM staff-roll text pointers differ");
  const strings: string[] = [];
  addresses.forEach((address, index) => {
    const end = index + 1 === addresses.length ? WIDTH_ADDRESS : addresses[index + 1];
    let terminator = offset(address);
    while (terminator < offset(end) && rom[terminator] !== 0) terminator++;
    if (terminator === offset(end)) throw new Error(`ROM staff-roll string ${index} lacks a terminator`);
    const encoded = rom.subarray(offset(address), terminator);
    if (encoded.some((byte) => byte < 0x20 || byte > 0x7e) ||
        rom.subarray(terminator, offset(end)).some((byte) => byte !== 0))
      throw new Error(`ROM staff-roll string ${index} is not canonical ASCII`);
    strings.push(encoded.toString("ascii"));
  });
  const stringIndex = new Map(addresses.map((address, index) => [address, index]));
  const lineIndices = pointers.map((pointer) => stringIndex.get(pointer));
  if (lineIndices.some((value) => value === undefined)) throw new Error("ROM staff-roll line pointer is unresolved");

  const widths = Array.from(rom.subarray(offset(WIDTH_ADDRESS), offset(WIDTH_END)));
  if (widths.some((width) => width < 1 || width > 8) ||
      rom.subarray(offset(WIDTH_END), offset(LINE_ADDRESS)).some((byte) => byte !== 0))
    throw new Error("ROM staff-roll glyph-width table differs");
  const rows = rom.subarray(offset(FONT_ADDRESS), offset(ALIGNMENT_ADDRESS));
  if (rom.subarray(offset(ALIGNMENT_ADDRESS), offset(STAFF_ROLL_END)).some((byte) => byte !== 0))
    throw new Error("ROM staff-roll alignment differs");

  const index: Json = {
    format: 1,
    kind: "golden-sun-staff-roll",
    address: hexadecimal(STAFF_ROLL_ADDRESS),
    end: hexadecimal(STAFF_ROLL_END),
    size: STAFF_ROLL_SIZE,
    preload_table: {
      address: hexadecimal(STAFF_ROLL_ADDRESS),
      end: hexadecimal(RESOURCE_END),
      slots: RESOURCE_COUNT + 1,
      resource_ids: resources,
      terminator: 0,
    },
    text_pool: {
      address: hexadecimal(TEXT_ADDRESS),
      end: hexadecimal(WIDTH_ADDRESS),
      alignment: 4,
      strings,
    },
    glyph_widths: {
      address: hexadecimal(WIDTH_ADDRESS),
      end: hexadecimal(WIDTH_END),
      count: GLYPH_COUNT,
      values: widths,
      padding: {
        end: hexadecimal(LINE_ADDRESS),
        size: LINE_ADDRESS - WIDTH_END,
        fill: 0,
      },
    },
    lines: {
      address: hexadecimal(LINE_ADDRESS),
      count: LINE_COUNT,
      string_indices: lineIndices,
      terminator: 0,
    },
    font: {
      address: hexadecimal(FONT_ADDRESS),
      end: hexadecimal(ALIGNMENT_ADDRESS),
      source: FONT_SOURCE,
      encoding: "gba-1bpp-msb-glyph-rows",
      first_code: 32,
      glyphs: GLYPH_COUNT,
      columns: FONT_COLUMNS,
    },
    alignment: {
      address: hexadecimal(ALIGNMENT_ADDRESS),
      end: hexadecimal(STAFF_ROLL_END),
      fill: 0,
    },
  };
  return { index, font: fontPng(rows), raw: Buffer.from(raw) };
}

function replaceDirectory(stage: string, destination: string): void {
  const parent = dirname(destination), backup = join(parent, `.staff-roll-old-${process.pid}-${Date.now()}`);
  if (!existsSync(destination)) {
    renameSync(stage, destination);
    return;
  }
  build_staff_roll(join(destination, "index.json"));
  renameSync(destination, backup);
  try {
    renameSync(stage, destination);
    rmSync(backup, { recursive: true, force: true });
  } catch (error) {
    if (existsSync(destination)) rmSync(destination, { recursive: true, force: true });
    renameSync(backup, destination);
    throw error;
  }
}

export function export_staff_roll(romPath: string, directory: string): void {
  const destination = resolve(directory), parent = dirname(destination);
  if (destination === parent) throw new Error("staff-roll destination is unsafe");
  mkdirSync(parent, { recursive: true });
  const stage = mkdtempSync(join(parent, ".staff-roll-new-"));
  try {
    const extracted = extractSource(readRom(romPath));
    writeFileSync(join(stage, "index.json"), pretty(extracted.index));
    writeFileSync(join(stage, FONT_SOURCE), extracted.font);
    const [built] = build_staff_roll(join(stage, "index.json"));
    if (!built.equals(extracted.raw)) throw new Error("exported staff-roll source does not rebuild exactly");
    replaceDirectory(stage, destination);
  } catch (error) {
    if (existsSync(stage)) rmSync(stage, { recursive: true, force: true });
    throw error;
  }
}

export function verify_staff_roll(romPath: string, indexPath: string): void {
  const rom = readRom(romPath), [built] = build_staff_roll(indexPath);
  const expected = rom.subarray(STAFF_ROLL_ADDRESS - ROM_BASE, STAFF_ROLL_END - ROM_BASE);
  if (!built.equals(expected)) throw new Error("staff-roll source differs from ROM");
}

function expectReject(action: () => void, name: string): void {
  try {
    action();
  } catch {
    return;
  }
  throw new Error(`${name} was accepted`);
}

type Command =
  | { kind: "self-test" }
  | { kind: "export"; rom: string; directory: string }
  | { kind: "verify"; rom: string; index: string };

function parseArguments(args: string[]): Command {
  if (args.length === 1 && args[0] === "--self-test") return { kind: "self-test" };
  if (args.length === 4 && args[0] === "export" && args[2] === "--directory" &&
      args[1].length !== 0 && !args[1].startsWith("-") && args[3].length !== 0 && !args[3].startsWith("-"))
    return { kind: "export", rom: args[1], directory: args[3] };
  if (args.length === 3 && args[0] === "verify" &&
      args[1].length !== 0 && !args[1].startsWith("-") && args[2].length !== 0 && !args[2].startsWith("-"))
    return { kind: "verify", rom: args[1], index: args[2] };
  throw new Error("usage: staff_roll.ts --self-test | export ROM --directory DIR | verify ROM INDEX");
}

function selfTest(): void {
  const root = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
  const source = join(root, "assets/text/staff_roll/index.json");
  const [built] = build_staff_roll(source);
  if (built.length !== STAFF_ROLL_SIZE) throw new Error("staff-roll self-test size differs");
  const temporary = mkdtempSync(join(TMPDIR, "alchemy-staff-roll-"));
  try {
    const romPath = join(temporary, "fixture.gba"), destination = join(temporary, "export");
    const rom = Buffer.alloc(STAFF_ROLL_END - ROM_BASE);
    built.copy(rom, STAFF_ROLL_ADDRESS - ROM_BASE);
    writeFileSync(romPath, rom);
    export_staff_roll(romPath, destination);
    const [roundTrip] = build_staff_roll(join(destination, "index.json"));
    if (!roundTrip.equals(built)) throw new Error("staff-roll self-test round trip differs");
    writeFileSync(join(destination, "stale.tmp"), "stale");
    export_staff_roll(romPath, destination);
    if (existsSync(join(destination, "stale.tmp"))) throw new Error("staff-roll export retained a stale file");

    const invalid = JSON.parse(readFileSync(join(destination, "index.json"), "utf8"));
    invalid.lines.string_indices[0] = STRING_COUNT;
    writeFileSync(join(destination, "index.json"), pretty(invalid));
    expectReject(() => build_staff_roll(join(destination, "index.json")), "out-of-range staff-roll string index");

    const canonical = JSON.parse(readFileSync(source, "utf8"));
    canonical.preload_table.terminator = 1;
    writeFileSync(join(destination, "index.json"), pretty(canonical));
    expectReject(() => build_staff_roll(join(destination, "index.json")), "nonzero preload terminator");
    canonical.preload_table.terminator = 0;
    canonical.glyph_widths.padding.size = 2;
    writeFileSync(join(destination, "index.json"), pretty(canonical));
    expectReject(() => build_staff_roll(join(destination, "index.json")), "incorrect glyph-width padding");

    const foreign = join(temporary, "foreign");
    mkdirSync(foreign);
    writeFileSync(join(foreign, "index.json"), "{}\n");
    expectReject(() => export_staff_roll(romPath, foreign), "foreign staff-roll destination");
    if (readFileSync(join(foreign, "index.json"), "utf8") !== "{}\n")
      throw new Error("staff-roll export changed a foreign destination");

    const invalidArguments = [
      ["--self-test", "extra"],
      ["export", "rom", "--directory", "out", "extra"],
      ["export", "rom", "--unknown", "out"],
      ["export", "rom", "--directory", "one", "--directory", "two"],
      ["export", "rom", "--directory"],
      ["export", "rom", "--directory", "--flag"],
      ["verify", "rom", "index", "extra"],
      ["verify", "--flag", "index"],
    ];
    invalidArguments.forEach((args, index) =>
      expectReject(() => parseArguments(args), `invalid CLI case ${index}`));
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
}

if (import.meta.main) {
  const command = parseArguments(Bun.argv.slice(2));
  if (command.kind === "self-test") {
    selfTest();
    console.log("staff_roll: self-test passed");
  } else if (command.kind === "export") {
    export_staff_roll(command.rom, command.directory);
    console.log(`staff_roll: exported ${STAFF_ROLL_SIZE} bytes`);
  } else {
    verify_staff_roll(command.rom, command.index);
    console.log(`staff_roll: verified ${STAFF_ROLL_SIZE} bytes`);
  }
}
