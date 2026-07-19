#!/usr/bin/env bun
import { readFileSync } from "fs";
import { dirname, resolve } from "path";
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
const LOGO_SHA256 = "08a0153cfd6b0ea54b938f7d209933fa849da0d56f5a34c481060c9ff2fad818";
const CODEWORDS = [
  "1", "0110", "01010", "0100", "00010", "011110", "010110", "000110",
  "00110", "011111", "010111", "000111", "0010", "01110", "00111", "0000",
] as const;

type JsonObject = Record<string, unknown>;

export interface GbaHeaderSource {
  format: 1;
  kind: "gba-cartridge-header-standard-fields";
  address: "0x08000000";
  standard: {
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
  };
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

function parseRecord(value: unknown, label: string, withName: boolean): { name?: string; address: string; size: number } {
  const record = object(value, label);
  exactKeys(record, withName ? ["name", "address", "size"] : ["address", "size"], label);
  if (withName && typeof record.name !== "string") throw new Error(`${label} name differs`);
  if (typeof record.address !== "string" || !/^0x[0-9a-f]{8}$/.test(record.address)) {
    throw new Error(`${label} address is not canonical`);
  }
  if (!Number.isSafeInteger(record.size) || Number(record.size) <= 0) throw new Error(`${label} size differs`);
  return { ...(withName ? { name: String(record.name) } : {}), address: record.address, size: Number(record.size) };
}

function sameRecords(
  actual: Array<{ name?: string; address: string; size: number }>,
  expected: Array<{ name?: string; address: string; size: number }>,
): boolean {
  return actual.length === expected.length && actual.every((record, index) =>
    record.name === expected[index].name && record.address === expected[index].address && record.size === expected[index].size);
}

export function parseGbaHeaderSource(value: unknown): GbaHeaderSource {
  const source = object(value, "GBA header source");
  exactKeys(source, ["format", "kind", "address", "standard", "unresolved_fields"], "GBA header source");
  if (source.format !== 1 || source.kind !== "gba-cartridge-header-standard-fields" ||
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
  if (!sameRecords(unresolved, EXPECTED_UNRESOLVED)) throw new Error("GBA header unresolved fields differ");
  return value as GbaHeaderSource;
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

export function buildGbaHeaderComponent(
  value: unknown,
  logoImage: Uint8Array,
  address: number,
  size: number,
): Buffer {
  parseGbaHeaderSource(value);
  if (address === GBA_LOGO_ADDRESS && size === GBA_LOGO_SIZE) return encodeGbaLogo(logoImage);
  if (address === GBA_FIXED_ADDRESS && size === GBA_FIXED_SIZE) {
    return Buffer.from([0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]);
  }
  if (address === GBA_RESERVED_END_ADDRESS && size === GBA_RESERVED_END_SIZE) return Buffer.alloc(2);
  throw new Error("GBA header component is not independently determined");
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

export function gbaComplementChecksum(header: Uint8Array): number {
  if (header.length < 0xbd) throw new Error("GBA header is too short for its complement checksum");
  let checksum = -0x19;
  for (let offset = 0xa0; offset <= 0xbc; offset++) checksum -= header[offset];
  return checksum & 0xff;
}

function selfTest(): void {
  const sourcePath = resolve(ROOT, "assets/data/gba_header.json");
  const source = parseGbaHeaderSource(JSON.parse(readFileSync(sourcePath, "utf8")));
  const logoPath = resolve(ROOT, source.standard.logo.source);
  const logo = buildGbaHeaderComponent(source, readFileSync(logoPath), GBA_LOGO_ADDRESS, GBA_LOGO_SIZE);
  if (logo.length !== GBA_LOGO_SIZE) throw new Error("GBA logo self-test failed");
  const fixed = buildGbaHeaderComponent(source, Buffer.alloc(0), GBA_FIXED_ADDRESS, GBA_FIXED_SIZE);
  if (fixed.toString("hex") !== "96000000000000000000") throw new Error("GBA fixed-field self-test failed");
  if (buildGbaHeaderComponent(source, Buffer.alloc(0), GBA_RESERVED_END_ADDRESS, GBA_RESERVED_END_SIZE).some(Boolean)) {
    throw new Error("GBA reserved-field self-test failed");
  }
  if (encodeArmBranch(0x08000000, 0x080000c0).toString("hex") !== "2e0000ea") {
    throw new Error("ARM branch self-test failed");
  }
  if (gbaComplementChecksum(Buffer.alloc(GBA_HEADER_SIZE)) !== 0xe7) throw new Error("GBA checksum self-test failed");
  const reject = (callback: () => unknown): void => {
    try { callback(); } catch { return; }
    throw new Error("invalid GBA header source was accepted");
  };
  reject(() => buildGbaHeaderComponent(source, Buffer.alloc(0), GBA_HEADER_ADDRESS, 4));
  reject(() => parseGbaHeaderSource({ ...source, standard: { ...source.standard, fixed_value: "0x00" } }));
  reject(() => encodeArmBranch(0x08000002, 0x080000c0));
}

if (import.meta.main) {
  if (!process.argv.includes("--self-test")) throw new Error("usage: gba_header.ts --self-test");
  selfTest();
  console.log("gba header tests passed");
}
