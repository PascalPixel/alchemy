#!/usr/bin/env bun
import {
  mkdirSync,
  mkdtempSync,
  readFileSync,
  realpathSync,
  renameSync,
  rmSync,
  writeFileSync,
} from "fs";
import { basename, dirname, isAbsolute, join, relative, resolve } from "path";
import { chunk } from "./export_asset.ts";
import { indexed_png } from "./import_asset.ts";
import { deflateSync } from "./zlib.ts";

export const GBA_HEADER_ADDRESS = 0x08000000;
export const GBA_HEADER_SIZE = 0xc0;
export const GBA_LOGO_ADDRESS = GBA_HEADER_ADDRESS + 0x04;
export const GBA_LOGO_SIZE = 0x9c;
export const GBA_FIXED_ADDRESS = GBA_HEADER_ADDRESS + 0xb2;
export const GBA_FIXED_SIZE = 0x0a;
export const GBA_RESERVED_END_ADDRESS = GBA_HEADER_ADDRESS + 0xbe;
export const GBA_RESERVED_END_SIZE = 0x02;
export const GBA_LOGO_WIDTH = 104;
export const GBA_LOGO_HEIGHT = 16;

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const DEFAULT_SOURCE = resolve(ROOT, "assets/data/gba_header.json");
const TMPDIR = Bun.env.TMPDIR ?? Bun.env.TMP ?? Bun.env.TEMP ?? "/tmp";
const LOGO_SHA256 = "08a0153cfd6b0ea54b938f7d209933fa849da0d56f5a34c481060c9ff2fad818";
const CODEWORDS = [
  "1", "0110", "01010", "0100", "00010", "011110", "010110", "000110",
  "00110", "011111", "010111", "000111", "0010", "01110", "00111", "0000",
] as const;

type JsonObject = Record<string, unknown>;

export interface GbaHeaderStandardFields {
  logo: {
    codec: "gba-bios-huffman-logo";
    source: string;
    width: 104;
    height: 16;
    bpp: 1;
  };
  fixed_value: "0x96";
  unit_code: "0x00";
  device_type: "0x00";
  reserved_zero_ranges: Array<{ address: string; size: number }>;
}

export interface GbaHeaderEdition {
  entry_branch: {
    instruction_set: "arm";
    operation: "b";
    target: string;
  };
  title: {
    text: string;
    padding: "nul";
    field_bytes: 12;
  };
  game_code: string;
  maker_code: string;
  software_version: number;
  complement_checksum: "derived";
}

export interface GbaHeaderSource {
  format: 2;
  kind: "gba-cartridge-header-standard-fields";
  address: "0x08000000";
  standard: GbaHeaderStandardFields;
  edition: GbaHeaderEdition | null;
  unresolved_fields: Array<{ name: string; address: string; size: number }>;
}

const EXPECTED_RESERVED = [
  { address: "0x080000b5", size: 7 },
  { address: "0x080000be", size: 2 },
];

const EXPECTED_UNRESOLVED = [
  { name: "entry_branch", address: "0x08000000", size: 4 },
  { name: "title", address: "0x080000a0", size: 12 },
  { name: "game_code", address: "0x080000ac", size: 4 },
  { name: "maker_code", address: "0x080000b0", size: 2 },
  { name: "software_version", address: "0x080000bc", size: 1 },
  { name: "complement_checksum", address: "0x080000bd", size: 1 },
];

function pretty(value: unknown): string {
  return `${JSON.stringify(value, null, 2)}\n`;
}

function object(value: unknown, label: string): JsonObject {
  if (typeof value !== "object" || value === null || Array.isArray(value)) {
    throw new Error(`${label} must be an object`);
  }
  return value as JsonObject;
}

function exactKeys(value: JsonObject, expected: string[], label: string): void {
  const actual = Object.keys(value).sort();
  const wanted = [...expected].sort();
  if (actual.length !== wanted.length || actual.some((key, index) => key !== wanted[index])) {
    throw new Error(`${label} fields differ`);
  }
}

function canonicalAddress(value: unknown, label: string): string {
  if (typeof value !== "string" || !/^0x[0-9a-f]{8}$/.test(value)) {
    throw new Error(`${label} is not a canonical address`);
  }
  return value;
}

function parseRecord(
  value: unknown,
  label: string,
  withName: boolean,
): { name?: string; address: string; size: number } {
  const record = object(value, label);
  exactKeys(record, withName ? ["name", "address", "size"] : ["address", "size"], label);
  if (withName && typeof record.name !== "string") throw new Error(`${label} name differs`);
  const address = canonicalAddress(record.address, `${label} address`);
  if (!Number.isSafeInteger(record.size) || Number(record.size) <= 0) throw new Error(`${label} size differs`);
  return { ...(withName ? { name: String(record.name) } : {}), address, size: Number(record.size) };
}

function sameRecords(
  actual: Array<{ name?: string; address: string; size: number }>,
  expected: Array<{ name?: string; address: string; size: number }>,
): boolean {
  return actual.length === expected.length && actual.every((record, index) =>
    record.name === expected[index].name && record.address === expected[index].address &&
    record.size === expected[index].size);
}

function parseAscii(value: unknown, pattern: RegExp, label: string): string {
  if (typeof value !== "string" || !pattern.test(value)) throw new Error(`${label} differs`);
  return value;
}

function titleBytes(title: GbaHeaderEdition["title"]): Buffer {
  const text = Buffer.from(title.text, "ascii");
  if (text.length !== title.text.length || text.length > 12 ||
      text.some((value) => value < 0x20 || value > 0x7e)) {
    throw new Error("GBA title must contain at most twelve printable ASCII bytes");
  }
  const output = Buffer.alloc(12);
  text.copy(output);
  return output;
}

function parseEdition(value: unknown): GbaHeaderEdition {
  const edition = object(value, "GBA header edition");
  exactKeys(edition, [
    "entry_branch", "title", "game_code", "maker_code", "software_version", "complement_checksum",
  ], "GBA header edition");
  const entry = object(edition.entry_branch, "GBA entry branch");
  exactKeys(entry, ["instruction_set", "operation", "target"], "GBA entry branch");
  if (entry.instruction_set !== "arm" || entry.operation !== "b") throw new Error("GBA entry branch differs");
  const target = canonicalAddress(entry.target, "GBA entry target");
  encodeArmBranch(GBA_HEADER_ADDRESS, Number(target));
  const title = object(edition.title, "GBA title");
  exactKeys(title, ["text", "padding", "field_bytes"], "GBA title");
  if (typeof title.text !== "string" || title.padding !== "nul" || title.field_bytes !== 12) {
    throw new Error("GBA title encoding differs");
  }
  const parsedTitle = { text: title.text, padding: "nul" as const, field_bytes: 12 as const };
  titleBytes(parsedTitle);
  const gameCode = parseAscii(edition.game_code, /^[A-Z0-9]{4}$/, "GBA game code");
  const makerCode = parseAscii(edition.maker_code, /^[A-Z0-9]{2}$/, "GBA maker code");
  if (!Number.isSafeInteger(edition.software_version) || Number(edition.software_version) < 0 ||
      Number(edition.software_version) > 0xff) throw new Error("GBA software version differs");
  if (edition.complement_checksum !== "derived") throw new Error("GBA checksum policy differs");
  return {
    entry_branch: { instruction_set: "arm", operation: "b", target },
    title: parsedTitle,
    game_code: gameCode,
    maker_code: makerCode,
    software_version: Number(edition.software_version),
    complement_checksum: "derived",
  };
}

export function parseGbaHeaderSource(value: unknown): GbaHeaderSource {
  const source = object(value, "GBA header source");
  exactKeys(source, ["format", "kind", "address", "standard", "edition", "unresolved_fields"], "GBA header source");
  if (source.format !== 2 || source.kind !== "gba-cartridge-header-standard-fields" ||
      source.address !== "0x08000000") throw new Error("unsupported GBA header source");
  const standard = object(source.standard, "GBA header standard fields");
  exactKeys(standard, ["logo", "fixed_value", "unit_code", "device_type", "reserved_zero_ranges"],
    "GBA header standard fields");
  const logo = object(standard.logo, "GBA header logo");
  exactKeys(logo, ["codec", "source", "width", "height", "bpp"], "GBA header logo");
  if (logo.codec !== "gba-bios-huffman-logo" || typeof logo.source !== "string" || logo.source.length === 0 ||
      logo.width !== GBA_LOGO_WIDTH || logo.height !== GBA_LOGO_HEIGHT || logo.bpp !== 1 ||
      standard.fixed_value !== "0x96" || standard.unit_code !== "0x00" || standard.device_type !== "0x00") {
    throw new Error("GBA header standard values differ");
  }
  if (!Array.isArray(standard.reserved_zero_ranges)) throw new Error("GBA header reserved ranges differ");
  const reserved = standard.reserved_zero_ranges.map((record, index) =>
    parseRecord(record, `GBA header reserved range ${index}`, false));
  if (!sameRecords(reserved, EXPECTED_RESERVED)) throw new Error("GBA header reserved ranges differ");
  if (!Array.isArray(source.unresolved_fields)) throw new Error("GBA header unresolved fields differ");
  const unresolved = source.unresolved_fields.map((record, index) =>
    parseRecord(record, `GBA header unresolved field ${index}`, true));
  if (source.edition === null) {
    if (!sameRecords(unresolved, EXPECTED_UNRESOLVED)) throw new Error("GBA header unresolved fields differ");
  } else {
    parseEdition(source.edition);
    if (unresolved.length !== 0) throw new Error("resolved GBA header still names unresolved fields");
  }
  return value as GbaHeaderSource;
}

export function readGbaHeaderSource(path: string): GbaHeaderSource {
  const text = readFileSync(path, "utf8");
  const value = JSON.parse(text);
  if (text !== pretty(value)) throw new Error(`${path}: source is not canonical JSON`);
  return parseGbaHeaderSource(value);
}

function pngHeader(width: number, height: number): Buffer {
  const header = Buffer.alloc(13);
  header.writeUInt32BE(width, 0);
  header.writeUInt32BE(height, 4);
  header.set([1, 3, 0, 0, 0], 8);
  return header;
}

export function logoPng(pixels: Uint8Array): Buffer {
  if (pixels.length !== GBA_LOGO_WIDTH * GBA_LOGO_HEIGHT ||
      pixels.some((pixel) => pixel !== 0 && pixel !== 1)) throw new Error("GBA logo pixels must be 104x16 1bpp");
  const rows = Buffer.alloc(GBA_LOGO_HEIGHT * (GBA_LOGO_WIDTH / 8 + 1));
  let cursor = 0;
  for (let y = 0; y < GBA_LOGO_HEIGHT; y++) {
    rows[cursor++] = 0;
    for (let x = 0; x < GBA_LOGO_WIDTH; x += 8) {
      let value = 0;
      for (let bit = 0; bit < 8; bit++) value |= pixels[y * GBA_LOGO_WIDTH + x + bit] << (7 - bit);
      rows[cursor++] = value;
    }
  }
  return Buffer.concat([
    Buffer.from("89504e470d0a1a0a", "hex"),
    chunk(Buffer.from("IHDR"), pngHeader(GBA_LOGO_WIDTH, GBA_LOGO_HEIGHT)),
    chunk(Buffer.from("PLTE"), Buffer.from("ffffff000000", "hex")),
    chunk(Buffer.from("IDAT"), deflateSync(rows, { level: 9 })),
    chunk(Buffer.from("IEND"), Buffer.alloc(0)),
  ]);
}

function tiledLogoBits(pixels: readonly number[]): Buffer {
  const decoded = Buffer.alloc(GBA_LOGO_WIDTH * GBA_LOGO_HEIGHT / 8);
  const tilesWide = GBA_LOGO_WIDTH / 8;
  for (let y = 0; y < GBA_LOGO_HEIGHT; y++) {
    for (let x = 0; x < GBA_LOGO_WIDTH; x++) {
      const tile = Math.floor(y / 8) * tilesWide + Math.floor(x / 8);
      const bit = tile * 64 + (y & 7) * 8 + (x & 7);
      decoded[bit >> 3] |= pixels[y * GBA_LOGO_WIDTH + x] << (bit & 7);
    }
  }
  return decoded;
}

function additionDeltas(decoded: Buffer): Buffer {
  if (decoded.length % 2 !== 0) throw new Error("GBA logo bitmap must contain whole halfwords");
  const output = Buffer.alloc(decoded.length + 4);
  output.writeUInt32LE(decoded.length << 8 | 0x82, 0);
  let previous = 0;
  for (let offset = 0; offset < decoded.length; offset += 2) {
    const current = decoded.readUInt16LE(offset);
    output.writeUInt16LE((current - previous) & 0xffff, offset + 4);
    previous = current;
  }
  return output;
}

function huffmanLogo(data: Uint8Array): Buffer {
  const output = Buffer.alloc(GBA_LOGO_SIZE);
  let position = 0;
  for (const value of data) {
    for (const nibble of [value & 15, value >>> 4]) {
      for (const character of CODEWORDS[nibble]) {
        if (position >= 0x4ce) throw new Error("GBA logo exceeds its fixed Huffman field");
        if (character === "1") {
          const wordOffset = Math.floor(position / 32) * 4;
          const shift = 31 - position % 32;
          const current = output.readUInt32LE(wordOffset);
          output.writeUInt32LE((current | (1 << shift)) >>> 0, wordOffset);
        }
        position++;
      }
    }
  }
  output[0x98] = 0x21;
  output[0x99] = 0xd4;
  return output;
}

export function encodeGbaLogo(image: Uint8Array): Buffer {
  const encodedImage = Buffer.from(image);
  const [width, height, pixels, palette] = indexed_png(encodedImage);
  if (width !== GBA_LOGO_WIDTH || height !== GBA_LOGO_HEIGHT ||
      !Bun.deepEquals(palette, [[255, 255, 255], [0, 0, 0]], true) ||
      !encodedImage.equals(logoPng(Uint8Array.from(pixels)))) {
    throw new Error("GBA logo source must be the canonical 104x16 monochrome PNG");
  }
  const output = huffmanLogo(additionDeltas(tiledLogoBits(pixels)));
  const hash = new Bun.CryptoHasher("sha256").update(output).digest("hex");
  if (hash !== LOGO_SHA256) throw new Error("GBA logo source does not encode the standard firmware logo");
  return output;
}

export function encodeArmBranch(address: number, target: number): Buffer {
  if (!Number.isSafeInteger(address) || !Number.isSafeInteger(target) || address < 0 || target < 0 ||
      address > 0xfffffffc || target > 0xfffffffc || (address & 3) !== 0 || (target & 3) !== 0) {
    throw new Error("ARM branch addresses must be aligned u32 values");
  }
  const words = (target - (address + 8)) / 4;
  if (!Number.isInteger(words) || words < -0x800000 || words > 0x7fffff) {
    throw new Error("ARM branch target is out of range");
  }
  const output = Buffer.alloc(4);
  output.writeUInt32LE((0xea000000 | (words & 0x00ffffff)) >>> 0);
  return output;
}

export function decodeArmBranch(data: Uint8Array, address = GBA_HEADER_ADDRESS): number {
  if (data.length !== 4) throw new Error("GBA entry branch must contain one ARM instruction");
  const instruction = Buffer.from(data).readUInt32LE();
  if (((instruction & 0xff000000) >>> 0) !== 0xea000000) {
    throw new Error("GBA entry instruction is not an unconditional ARM B");
  }
  let displacement = instruction & 0x00ffffff;
  if ((displacement & 0x00800000) !== 0) displacement -= 0x01000000;
  const target = address + 8 + displacement * 4;
  if (!Number.isSafeInteger(target) || target < 0 || target > 0xfffffffc || (target & 3) !== 0) {
    throw new Error("GBA entry target is invalid");
  }
  return target;
}

export function gbaComplementChecksum(header: Uint8Array): number {
  if (header.length < 0xbd) throw new Error("GBA header is too short for its complement checksum");
  let checksum = -0x19;
  for (let offset = 0xa0; offset <= 0xbc; offset++) checksum -= header[offset];
  return checksum & 0xff;
}

function completeEdition(source: GbaHeaderSource): GbaHeaderEdition {
  if (source.edition === null) throw new Error("GBA header edition is unresolved");
  return parseEdition(source.edition);
}

function logoSourcePath(source: GbaHeaderSource): string {
  const path = resolve(ROOT, source.standard.logo.source);
  const position = relative(ROOT, path);
  if (position === ".." || position.startsWith("../") || position.startsWith("..\\") || isAbsolute(position)) {
    throw new Error("GBA logo source escaped the repository");
  }
  return path;
}

export function buildGbaHeader(sourceValue: unknown, logoImage: Uint8Array): Buffer {
  const source = parseGbaHeaderSource(sourceValue);
  const edition = completeEdition(source);
  const output = Buffer.alloc(GBA_HEADER_SIZE);
  encodeArmBranch(GBA_HEADER_ADDRESS, Number(edition.entry_branch.target)).copy(output, 0x00);
  encodeGbaLogo(logoImage).copy(output, 0x04);
  titleBytes(edition.title).copy(output, 0xa0);
  Buffer.from(edition.game_code, "ascii").copy(output, 0xac);
  Buffer.from(edition.maker_code, "ascii").copy(output, 0xb0);
  output[0xb2] = 0x96;
  output[0xb3] = 0x00;
  output[0xb4] = 0x00;
  output.fill(0, 0xb5, 0xbc);
  output[0xbc] = edition.software_version;
  output[0xbd] = gbaComplementChecksum(output);
  output.fill(0, 0xbe, 0xc0);
  return output;
}

export function buildGbaHeaderComponent(
  value: unknown,
  logoImage: Uint8Array,
  address: number,
  size: number,
): Buffer {
  const source = parseGbaHeaderSource(value);
  if (address === GBA_LOGO_ADDRESS && size === GBA_LOGO_SIZE) return encodeGbaLogo(logoImage);
  if (address === GBA_FIXED_ADDRESS && size === GBA_FIXED_SIZE) {
    return Buffer.from([0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]);
  }
  if (address === GBA_RESERVED_END_ADDRESS && size === GBA_RESERVED_END_SIZE) return Buffer.alloc(2);
  if (source.edition !== null && Number.isSafeInteger(address) && Number.isSafeInteger(size) && size > 0 &&
      address >= GBA_HEADER_ADDRESS && address + size <= GBA_HEADER_ADDRESS + GBA_HEADER_SIZE) {
    const header = buildGbaHeader(source, logoImage);
    const offset = address - GBA_HEADER_ADDRESS;
    return header.subarray(offset, offset + size);
  }
  throw new Error("GBA header component is not independently determined");
}

function decodeTitle(field: Uint8Array): GbaHeaderEdition["title"] {
  if (field.length !== 12) throw new Error("GBA title field size differs");
  const bytes = Buffer.from(field);
  const terminator = bytes.indexOf(0);
  const length = terminator < 0 ? bytes.length : terminator;
  if (terminator >= 0 && bytes.subarray(terminator).some(Boolean)) throw new Error("GBA title has data after NUL padding");
  const textBytes = bytes.subarray(0, length);
  if (textBytes.some((value) => value < 0x20 || value > 0x7e)) throw new Error("GBA title is not printable ASCII");
  return { text: textBytes.toString("ascii"), padding: "nul", field_bytes: 12 };
}

function decodeCode(field: Uint8Array, pattern: RegExp, label: string): string {
  const value = Buffer.from(field).toString("ascii");
  if (!pattern.test(value)) throw new Error(`${label} is not canonical ASCII`);
  return value;
}

function standardFields(source: GbaHeaderSource): GbaHeaderStandardFields {
  return structuredClone(source.standard);
}

export function deriveGbaHeaderSource(header: Uint8Array, templateValue: unknown): GbaHeaderSource {
  if (header.length !== GBA_HEADER_SIZE) throw new Error("GBA header extent differs");
  const data = Buffer.from(header);
  const template = parseGbaHeaderSource(templateValue);
  const logo = encodeGbaLogo(readFileSync(logoSourcePath(template)));
  if (!data.subarray(0x04, 0xa0).equals(logo)) throw new Error("GBA firmware logo differs");
  if (data[0xb2] !== 0x96 || data[0xb3] !== 0 || data[0xb4] !== 0 ||
      data.subarray(0xb5, 0xbc).some(Boolean) || data.subarray(0xbe, 0xc0).some(Boolean)) {
    throw new Error("GBA fixed or reserved fields differ");
  }
  const target = decodeArmBranch(data.subarray(0, 4));
  const edition: GbaHeaderEdition = {
    entry_branch: {
      instruction_set: "arm",
      operation: "b",
      target: `0x${target.toString(16).padStart(8, "0")}`,
    },
    title: decodeTitle(data.subarray(0xa0, 0xac)),
    game_code: decodeCode(data.subarray(0xac, 0xb0), /^[A-Z0-9]{4}$/, "GBA game code"),
    maker_code: decodeCode(data.subarray(0xb0, 0xb2), /^[A-Z0-9]{2}$/, "GBA maker code"),
    software_version: data[0xbc],
    complement_checksum: "derived",
  };
  if (data[0xbd] !== gbaComplementChecksum(data)) throw new Error("GBA complement checksum differs");
  const source: GbaHeaderSource = {
    format: 2,
    kind: "gba-cartridge-header-standard-fields",
    address: "0x08000000",
    standard: standardFields(template),
    edition,
    unresolved_fields: [],
  };
  parseGbaHeaderSource(source);
  const rebuilt = buildGbaHeader(source, readFileSync(logoSourcePath(source)));
  if (!rebuilt.equals(data)) throw new Error("derived GBA header source does not round-trip");
  return source;
}

function samePath(left: string, right: string): boolean {
  try { return realpathSync(left) === realpathSync(right); }
  catch { return resolve(left) === resolve(right); }
}

export function exportGbaHeader(
  rom: Buffer,
  outputPath: string,
  templatePath = DEFAULT_SOURCE,
  inputPath?: string,
): GbaHeaderSource {
  if (rom.length < GBA_HEADER_SIZE) throw new Error("ROM is too small for a GBA header");
  if (inputPath !== undefined && samePath(inputPath, outputPath)) throw new Error("refusing to overwrite the input ROM");
  const template = readGbaHeaderSource(templatePath);
  const header = rom.subarray(0, GBA_HEADER_SIZE);
  const source = deriveGbaHeaderSource(header, template);
  const destination = resolve(outputPath);
  const parent = dirname(destination);
  mkdirSync(parent, { recursive: true });
  const transaction = mkdtempSync(join(parent, ".gba-header-export-"));
  const staged = join(transaction, basename(destination));
  try {
    writeFileSync(staged, pretty(source));
    const stagedSource = readGbaHeaderSource(staged);
    const rebuilt = buildGbaHeader(stagedSource, readFileSync(logoSourcePath(stagedSource)));
    if (!rebuilt.equals(header)) throw new Error("staged GBA header source does not round-trip");
    renameSync(staged, destination);
  } finally {
    rmSync(transaction, { recursive: true, force: true });
  }
  return source;
}

export function verifyGbaHeader(rom: Buffer, sourcePath: string): Buffer {
  if (rom.length < GBA_HEADER_SIZE) throw new Error("ROM is too small for a GBA header");
  const source = readGbaHeaderSource(sourcePath);
  const built = buildGbaHeader(source, readFileSync(logoSourcePath(source)));
  const expected = rom.subarray(0, GBA_HEADER_SIZE);
  if (!built.equals(expected)) {
    let offset = 0;
    while (offset < GBA_HEADER_SIZE && built[offset] === expected[offset]) offset++;
    throw new Error(`GBA header differs at 0x${(GBA_HEADER_ADDRESS + offset).toString(16).padStart(8, "0")}`);
  }
  return built;
}

function reject(callback: () => unknown): boolean {
  try { callback(); } catch { return true; }
  return false;
}

function syntheticSource(template: GbaHeaderSource): GbaHeaderSource {
  return {
    format: 2,
    kind: "gba-cartridge-header-standard-fields",
    address: "0x08000000",
    standard: standardFields(template),
    edition: {
      entry_branch: { instruction_set: "arm", operation: "b", target: "0x080003c0" },
      title: { text: "HEADER TEST", padding: "nul", field_bytes: 12 },
      game_code: "TST1",
      maker_code: "01",
      software_version: 7,
      complement_checksum: "derived",
    },
    unresolved_fields: [],
  };
}

function selfTest(): void {
  const template = readGbaHeaderSource(DEFAULT_SOURCE);
  if (template.edition !== null) throw new Error("canonical GBA header template unexpectedly resolved");
  const logoImage = readFileSync(logoSourcePath(template));
  const source = syntheticSource(template);
  const built = buildGbaHeader(source, logoImage);
  if (built.length !== GBA_HEADER_SIZE || built.subarray(0, 4).toString("hex") !== "ee0000ea" ||
      decodeArmBranch(built.subarray(0, 4)) !== 0x080003c0 || built[0xbd] !== gbaComplementChecksum(built)) {
    throw new Error("complete GBA header build self-test failed");
  }
  if (!Bun.deepEquals(deriveGbaHeaderSource(built, template), source, true)) {
    throw new Error("GBA header semantic round-trip failed");
  }
  const temporary = mkdtempSync(join(TMPDIR, "gba-header-self-test-"));
  try {
    const output = join(temporary, "header.json");
    exportGbaHeader(built, output, DEFAULT_SOURCE);
    if (!buildGbaHeader(readGbaHeaderSource(output), logoImage).equals(built)) {
      throw new Error("exported GBA header differs");
    }
    verifyGbaHeader(built, output);
    const sentinel = Buffer.from("preserve\n");
    writeFileSync(output, sentinel);
    const corruptChecksum = Buffer.from(built);
    corruptChecksum[0xbd] ^= 1;
    if (!reject(() => exportGbaHeader(corruptChecksum, output, DEFAULT_SOURCE)) ||
        !readFileSync(output).equals(sentinel)) throw new Error("failed export replaced its destination");
    const corruptBranch = Buffer.from(built);
    corruptBranch[3] = 0xeb;
    if (!reject(() => deriveGbaHeaderSource(corruptBranch, template))) throw new Error("ARM BL entry was accepted");
    const corruptLogo = Buffer.from(built);
    corruptLogo[0x20] ^= 1;
    if (!reject(() => deriveGbaHeaderSource(corruptLogo, template))) throw new Error("nonstandard logo was accepted");
    const corruptFixed = Buffer.from(built);
    corruptFixed[0xb5] = 1;
    if (!reject(() => deriveGbaHeaderSource(corruptFixed, template))) throw new Error("reserved data was accepted");
    const corruptTitle = Buffer.from(built);
    corruptTitle[0xa2] = 0;
    corruptTitle[0xbd] = gbaComplementChecksum(corruptTitle);
    if (!reject(() => deriveGbaHeaderSource(corruptTitle, template))) throw new Error("embedded title NUL was accepted");
    const corruptCode = Buffer.from(built);
    corruptCode[0xac] = 0x61;
    corruptCode[0xbd] = gbaComplementChecksum(corruptCode);
    if (!reject(() => deriveGbaHeaderSource(corruptCode, template))) throw new Error("lowercase game code was accepted");
    const unknown = structuredClone(source) as unknown as JsonObject;
    (unknown.edition as JsonObject).raw_checksum = "0x00";
    if (!reject(() => parseGbaHeaderSource(unknown))) throw new Error("unknown edition field was accepted");
    const explicitChecksum = structuredClone(source);
    explicitChecksum.edition!.complement_checksum = "0x00" as "derived";
    if (!reject(() => parseGbaHeaderSource(explicitChecksum))) throw new Error("explicit checksum byte was accepted");
    const badVersion = structuredClone(source);
    badVersion.edition!.software_version = 256;
    if (!reject(() => parseGbaHeaderSource(badVersion))) throw new Error("oversized software version was accepted");
    if (!reject(() => buildGbaHeader(template, logoImage))) throw new Error("unresolved header built as complete");
    writeFileSync(output, JSON.stringify(source));
    if (!reject(() => readGbaHeaderSource(output))) throw new Error("noncanonical JSON was accepted");
    writeFileSync(join(temporary, "rom.gba"), built);
    if (!reject(() => exportGbaHeader(built, join(temporary, "rom.gba"), DEFAULT_SOURCE, join(temporary, "rom.gba")))) {
      throw new Error("input ROM overwrite was accepted");
    }
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok bytes=192 unresolved=0");
}

function option(args: string[], ...names: string[]): string {
  const index = args.findIndex((argument) => names.includes(argument));
  if (index < 0 || index + 1 >= args.length) throw new Error(`${names.at(-1)} is required`);
  return args[index + 1];
}

function usage(): never {
  console.log(
    "usage: gba_header.ts export ROM (--output FILE | --directory DIR) [--template FILE] | " +
    "build SOURCE --output FILE | verify ROM SOURCE | --self-test",
  );
  process.exit(0);
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { selfTest(); return; }
  if (args[0] === "export" && args[1] !== undefined) {
    const romPath = args[1];
    const output = args.includes("--directory") ? join(option(args, "--directory"), "gba_header.json") :
      option(args, "-o", "--output");
    const template = args.includes("--template") ? option(args, "--template") : DEFAULT_SOURCE;
    const source = exportGbaHeader(readFileSync(romPath), output, template, romPath);
    console.log(`target=${source.edition!.entry_branch.target} bytes=${GBA_HEADER_SIZE} exact=true`);
    return;
  }
  if (args[0] === "build" && args[1] !== undefined) {
    const source = readGbaHeaderSource(args[1]);
    const built = buildGbaHeader(source, readFileSync(logoSourcePath(source)));
    const output = option(args, "-o", "--output");
    mkdirSync(dirname(resolve(output)), { recursive: true });
    writeFileSync(output, built);
    console.log(`address=0x08000000 bytes=${built.length}`);
    return;
  }
  if (args[0] === "verify" && args.length === 3) {
    const built = verifyGbaHeader(readFileSync(args[1]), args[2]);
    console.log(`address=0x08000000 bytes=${built.length} exact=true`);
    return;
  }
  if (args.includes("-h") || args.includes("--help")) usage();
  usage();
}

if (import.meta.main) main(process.argv.slice(2));
