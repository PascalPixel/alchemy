#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { isCanonicalJsonText } from "./canonical_json.ts";
import {
  existsSync, lstatSync, mkdirSync, mkdtempSync, readFileSync, readdirSync, renameSync, realpathSync, rmSync, symlinkSync,
  writeFileSync,
} from "node:fs";
import { dirname, isAbsolute, join, relative, resolve } from "node:path";
import { tile_png, type Rgb } from "./export_asset.ts";
import { decode_general_trace, encode_general, type GeneralToken } from "./extract_resource.ts";
import { gba_graphics } from "./import_asset.ts";

const ROM_BASE = 0x08000000;
const ROM_SIZE = 0x00800000;
const RESOURCE_TABLE = 0x08320000;
export const RESOURCE_ID = 0x01c;
export const RESOURCE_ADDRESS = 0x0833ac08;
export const RESOURCE_SIZE = 0x02b4;
export const RESOURCE_END = RESOURCE_ADDRESS + RESOURCE_SIZE;
export const PALETTE_BYTES = 0x20;
export const GLYPHS = 36;
export const ATLAS_COLUMNS = 9;
const TILE_BYTES = 0x20;

interface GlyphPlan {
  format: 1;
  kind: "golden-sun-kana-glyph-bank";
  codec: "golden-sun-general-lz";
  resource_id: string;
  address: string;
  size: string;
  end: string;
  decoded_size: number;
  palette_entries: number;
  glyph_width: number;
  glyph_height: number;
  glyphs: number;
  atlas_columns: number;
  source: string;
  tokens: GeneralToken[];
}

function exactKeys(value: Record<string, unknown>, keys: readonly string[], label: string): void {
  const actual = Object.keys(value).sort();
  const expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${label} has unexpected fields`);
  }
}

function hex(value: number, width = 8): string {
  return `0x${value.toString(16).padStart(width, "0")}`;
}

function physicalPath(path: string): string {
  const absolute = resolve(path);
  let ancestor = absolute;
  while (!existsSync(ancestor)) {
    const parent = dirname(ancestor);
    if (parent === ancestor) return absolute;
    ancestor = parent;
  }
  return resolve(realpathSync(ancestor), relative(ancestor, absolute));
}

function same(left: string, right: string): boolean {
  return physicalPath(left) === physicalPath(right);
}

function containsPath(directory: string, path: string): boolean {
  const position = relative(physicalPath(directory), physicalPath(path));
  return position === "" || (position !== ".." && !position.startsWith("../") &&
    !position.startsWith("..\\") && !isAbsolute(position));
}

function validateExportDestination(romPath: string, directory: string): void {
  if (same(romPath, directory) || containsPath(directory, romPath)) {
    throw new Error("kana export directory must not contain its input ROM");
  }
  const destination = resolve(directory);
  if (!existsSync(destination)) return;
  if (!lstatSync(destination).isDirectory()) throw new Error("kana export destination must be a directory");
  if (readdirSync(destination).length === 0) return;
  const marker = join(destination, "stream.json");
  if (!existsSync(marker) || !lstatSync(marker).isFile()) {
    throw new Error("refusing to replace a directory that is not a kana package");
  }
  try { readPlan(marker); }
  catch { throw new Error("refusing to replace a directory that is not a canonical kana package"); }
}

function sourcePath(planPath: string, name: string): string {
  if (name !== "kana.4bpp.png") throw new Error("kana source name differs");
  const root = realpathSync(dirname(planPath));
  const path = realpathSync(resolve(root, name));
  const position = relative(root, path);
  if (position !== name) throw new Error("kana source escaped its directory");
  return path;
}

function bgr555(data: Uint8Array): Rgb[] {
  if (data.length !== PALETTE_BYTES) throw new Error("kana palette has the wrong size");
  const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
  return Array.from({ length: PALETTE_BYTES / 2 }, (_, index) => {
    const value = view.getUint16(index * 2, true);
    if (value & 0x8000) throw new Error("kana palette uses bit 15");
    return [(value & 31) << 3, (value >>> 5 & 31) << 3, (value >>> 10 & 31) << 3] as Rgb;
  });
}

function validateTokens(tokens: unknown): asserts tokens is GeneralToken[] {
  if (!Array.isArray(tokens)) throw new Error("kana tokens must be an array");
  for (const [index, value] of tokens.entries()) {
    if (!Array.isArray(value)) throw new Error(`kana token ${index} must be an array`);
    if (value[0] === "l") {
      if (value.length !== 2 || typeof value[1] !== "number" || !Number.isSafeInteger(value[1]) || value[1] < 1) {
        throw new Error(`kana token ${index} has an invalid literal shape`);
      }
    } else if (value[0] === "c") {
      if (value.length !== 3 || typeof value[1] !== "number" || !Number.isSafeInteger(value[1]) ||
          value[1] < 2 || value[1] > 137 || typeof value[2] !== "number" ||
          !Number.isSafeInteger(value[2]) || value[2] < 1 || value[2] > 0x1020) {
        throw new Error(`kana token ${index} has an invalid copy shape`);
      }
    } else throw new Error(`kana token ${index} has an unsupported operation`);
  }
}

function jsonDocument(path: string, label: string): unknown {
  const text = readFileSync(path, "utf8");
  const value = JSON.parse(text);
  if (!isCanonicalJsonText(text, value)) throw new Error(`${label} is not canonical JSON`);
  return value;
}

function readPlan(planPath: string): GlyphPlan {
  const value = jsonDocument(planPath, "kana plan");
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error("kana plan must be an object");
  const plan = value as GlyphPlan;
  exactKeys(plan as unknown as Record<string, unknown>, [
    "format", "kind", "codec", "resource_id", "address", "size", "end", "decoded_size",
    "palette_entries", "glyph_width", "glyph_height", "glyphs", "atlas_columns", "source", "tokens",
  ], "kana plan");
  if (plan.format !== 1 || plan.kind !== "golden-sun-kana-glyph-bank" || plan.codec !== "golden-sun-general-lz" ||
      plan.resource_id !== "01c" || plan.address !== hex(RESOURCE_ADDRESS) || plan.size !== hex(RESOURCE_SIZE, 1) ||
      plan.end !== hex(RESOURCE_END) || plan.decoded_size !== PALETTE_BYTES + GLYPHS * TILE_BYTES ||
      plan.palette_entries !== 16 || plan.glyph_width !== 8 || plan.glyph_height !== 8 ||
      plan.glyphs !== GLYPHS || plan.atlas_columns !== ATLAS_COLUMNS || plan.source !== "kana.4bpp.png") {
    throw new Error("kana plan differs from the audited layout");
  }
  validateTokens(plan.tokens);
  return plan;
}

function readKanaImage(imagePath: string): [Buffer, Buffer, ReturnType<typeof gba_graphics>[2]] {
  const encoded = readFileSync(imagePath);
  const [tiles, palette, report] = gba_graphics(encoded, 4);
  if (!encoded.equals(tile_png(tiles, 4, ATLAS_COLUMNS, bgr555(palette))[0])) {
    throw new Error("kana atlas is not a canonical source PNG");
  }
  return [tiles, palette, report];
}

function resourcePointer(rom: Buffer, id: number): number {
  const offset = RESOURCE_TABLE - ROM_BASE + id * 4;
  if (offset < 0 || offset + 4 > rom.length) throw new Error("resource table is outside the ROM");
  return rom.readUInt32LE(offset);
}

function validateDirectoryBounds(rom: Buffer): void {
  if (resourcePointer(rom, RESOURCE_ID) !== RESOURCE_ADDRESS || resourcePointer(rom, RESOURCE_ID + 1) !== RESOURCE_END) {
    throw new Error("kana resource differs from the audited directory bounds");
  }
}

export function build_resource_01c(planPath: string): [Buffer, string[]] {
  const plan = readPlan(planPath);
  const imagePath = sourcePath(planPath, plan.source);
  const [tiles, palette, report] = readKanaImage(imagePath);
  if (report.width !== ATLAS_COLUMNS * 8 || report.height !== GLYPHS / ATLAS_COLUMNS * 8 ||
      report.tiles !== GLYPHS || report.palette_entries !== 16 || palette.length !== PALETTE_BYTES ||
      tiles.length !== GLYPHS * TILE_BYTES) {
    throw new Error("kana atlas differs from the audited dimensions");
  }
  const decoded = Buffer.concat([palette, tiles]);
  const encoded = encode_general(decoded, plan.tokens);
  if (encoded.length !== RESOURCE_SIZE) throw new Error("kana resource has the wrong encoded size");
  return [encoded, [realpathSync(planPath), imagePath]];
}

function replaceDirectory<T>(directory: string, write: (staged: string) => T): T {
  const destination = resolve(directory), parent = dirname(destination);
  if (destination === parent) throw new Error("kana export requires a dedicated directory");
  mkdirSync(parent, { recursive: true });
  const transaction = mkdtempSync(join(parent, ".kana-export-"));
  const staged = join(transaction, "new"), previous = join(transaction, "previous");
  let installed = false;
  try {
    const result = write(staged);
    if (existsSync(destination)) renameSync(destination, previous);
    try {
      renameSync(staged, destination);
      installed = true;
    } catch (error) {
      if (existsSync(previous)) renameSync(previous, destination);
      throw error;
    }
    return result;
  } finally {
    if (!installed && existsSync(previous) && !existsSync(destination)) renameSync(previous, destination);
    rmSync(transaction, { recursive: true, force: true });
  }
}

function writeResource01c(rom: Buffer, directory: string): GlyphPlan {
  if (rom.length !== ROM_SIZE) throw new Error("kana exporter requires the 8 MiB canonical ROM");
  validateDirectoryBounds(rom);
  const start = RESOURCE_ADDRESS - ROM_BASE;
  const encoded = rom.subarray(start, start + RESOURCE_SIZE);
  if (encoded.length !== RESOURCE_SIZE) throw new Error("kana resource is outside the ROM");
  const [decoded, used, tokens] = decode_general_trace(encoded, 0, encoded.length, 0x10000);
  if (used !== encoded.length || decoded.length !== PALETTE_BYTES + GLYPHS * TILE_BYTES) {
    throw new Error("kana resource has an unexpected compressed extent");
  }
  const replay = encode_general(decoded, tokens);
  if (!replay.equals(encoded)) throw new Error("kana compression replay differs");
  mkdirSync(directory, { recursive: true });
  const imagePath = resolve(directory, "kana.4bpp.png");
  const planPath = resolve(directory, "stream.json");
  writeFileSync(imagePath, tile_png(decoded.subarray(PALETTE_BYTES), 4, ATLAS_COLUMNS, bgr555(decoded.subarray(0, PALETTE_BYTES)))[0]);
  const plan: GlyphPlan = {
    format: 1,
    kind: "golden-sun-kana-glyph-bank",
    codec: "golden-sun-general-lz",
    resource_id: "01c",
    address: hex(RESOURCE_ADDRESS),
    size: hex(RESOURCE_SIZE, 1),
    end: hex(RESOURCE_END),
    decoded_size: decoded.length,
    palette_entries: 16,
    glyph_width: 8,
    glyph_height: 8,
    glyphs: GLYPHS,
    atlas_columns: ATLAS_COLUMNS,
    source: "kana.4bpp.png",
    tokens,
  };
  writeFileSync(planPath, `${JSON.stringify(plan)}\n`);
  if (!build_resource_01c(planPath)[0].equals(encoded)) throw new Error("exported kana resource differs");
  return plan;
}

export function export_resource_01c(romPath: string, directory: string): GlyphPlan {
  validateExportDestination(romPath, directory);
  const rom = readFileSync(romPath);
  return replaceDirectory(directory, (staged) => writeResource01c(rom, staged));
}

export function verify_resource_01c(romPath: string, planPath: string): void {
  const rom = readFileSync(romPath);
  if (rom.length !== ROM_SIZE) throw new Error("kana verifier requires the 8 MiB canonical ROM");
  validateDirectoryBounds(rom);
  const [built] = build_resource_01c(planPath);
  const original = rom.subarray(RESOURCE_ADDRESS - ROM_BASE, RESOURCE_END - ROM_BASE);
  if (!built.equals(original)) throw new Error("kana resource differs from ROM");
  console.log(`identical=true glyphs=${GLYPHS} source_bytes=${built.length}`);
}

export function self_test(): void {
  const palette = Buffer.alloc(PALETTE_BYTES);
  for (let index = 0; index < 16; index++) palette.writeUInt16LE(index | index << 5 | index << 10, index * 2);
  const colors = bgr555(palette);
  if (colors.length !== 16 || colors[15][0] !== 120 || colors[15][1] !== 120 || colors[15][2] !== 120) {
    throw new Error("kana palette self-test failed");
  }
  let rejected = false;
  try { validateTokens([["l", 1, "ignored"]]); } catch { rejected = true; }
  if (!rejected) throw new Error("kana token strictness self-test failed");
  const temporary = mkdtempSync("/private/tmp/kana-resource-self-test-");
  try {
    const reject = (action: () => void): void => {
      let failed = false;
      try { action(); } catch { failed = true; }
      if (!failed) throw new Error("kana adversarial self-test failed");
    };
    const duplicate = join(temporary, "duplicate.json");
    writeFileSync(duplicate, "{\"format\":0,\"format\":1}\n");
    reject(() => jsonDocument(duplicate, "test document"));
    const hidden = join(temporary, "hidden.png");
    const tiles = Buffer.alloc(GLYPHS * TILE_BYTES);
    writeFileSync(hidden, Buffer.concat([tile_png(tiles, 4, ATLAS_COLUMNS, bgr555(Buffer.alloc(PALETTE_BYTES)))[0], Buffer.from("hidden")]));
    reject(() => readKanaImage(hidden));
    const destination = join(temporary, "destination");
    mkdirSync(destination);
    reject(() => validateExportDestination(join(destination, "rom.gba"), destination));
    const physicalRom = join(destination, "physical.gba");
    const romLink = join(temporary, "rom-link.gba");
    writeFileSync(physicalRom, "rom");
    symlinkSync(physicalRom, romLink);
    reject(() => validateExportDestination(romLink, destination));
    const unrelated = join(temporary, "unrelated");
    mkdirSync(unrelated);
    writeFileSync(join(unrelated, "keep"), "old");
    reject(() => validateExportDestination(join(temporary, "rom.gba"), unrelated));
    writeFileSync(join(destination, "stale"), "old");
    reject(() => replaceDirectory(destination, (staged) => {
      mkdirSync(staged);
      writeFileSync(join(staged, "partial"), "new");
      throw new Error("synthetic failure");
    }));
    if (!existsSync(join(destination, "stale")) || existsSync(join(destination, "partial"))) {
      throw new Error("failed kana export changed its destination");
    }
    replaceDirectory(destination, (staged) => {
      mkdirSync(staged);
      writeFileSync(join(staged, "current"), "new");
    });
    if (existsSync(join(destination, "stale")) || !existsSync(join(destination, "current"))) {
      throw new Error("kana export did not replace its whole destination");
    }
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok");
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { self_test(); return; }
  if (args.length === 1 && (args[0] === "-h" || args[0] === "--help")) {
    console.log("usage: resource_01c.ts export ROM --directory DIR | verify ROM --plan PLAN | --self-test");
    return;
  }
  if (args.length === 4 && args[0] === "export" && args[2] === "--directory") {
    const plan = export_resource_01c(args[1], args[3]);
    console.log(`glyphs=${plan.glyphs} source_bytes=${RESOURCE_SIZE}`);
    return;
  }
  if (args.length === 4 && args[0] === "verify" && args[2] === "--plan") {
    verify_resource_01c(args[1], args[3]);
    return;
  }
  throw new Error("usage: resource_01c.ts export ROM --directory DIR | verify ROM --plan PLAN | --self-test");
}

if (import.meta.main) main(Bun.argv.slice(2));
