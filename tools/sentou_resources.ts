#!/usr/bin/env bun
import { mkdirSync, readFileSync, realpathSync, writeFileSync } from "node:fs";
import { dirname, join, relative, resolve } from "node:path";
import {
  decode_general_prefill_trace,
  decode_palette_trace,
  encode_general_prefill,
  encode_palette,
  type GeneralToken,
  type PaletteGroup,
} from "./extract_resource.ts";
import { byte_png, palette_rgba_image, tile_png } from "./export_asset.ts";
import { gba_graphics, gba_palette_rgba, indexed_png, type Rgb } from "./import_asset.ts";

const ROM_BASE = 0x08000000;
const RESOURCE_TABLE = 0x08320000;
const PREFILL = 0x1000;

type Codec = "general-lz-prefill" | "palette-lz";
type Presentation = "koma-4bpp" | "haikei-8bpp" | "naiyou";

interface Spec {
  id: number;
  address: number;
  boundarySize: number;
  sourceSize: number;
  decodedSize: number;
  prefixPalette: number;
  codec: Codec;
  presentation: Presentation;
  displayPaletteBack?: number;
}

interface Plan {
  format: 1;
  kind: "golden-sun-sentou-resource";
  resource_id: string;
  address: string;
  resource_boundary_size: string;
  source_size: string;
  prefix_palette: null | {
    size: string;
    source: "iro.rgba.png";
  };
  stream: {
    codec: Codec;
    tag: 0 | 1;
    prefill: string;
    decoded_size: string;
    tokens: GeneralToken[] | PaletteGroup[];
  };
  image: {
    encoding: Presentation;
    source: "koma.4bpp.png" | "haikei.8bpp.png" | "naiyou.png";
    canvas_size: string;
    columns: number;
  };
}

interface IndexEntry {
  id: string;
  address: string;
  size: string;
  resource_boundary_size: string;
  source: string;
}

interface IndexDocument {
  format: 1;
  kind: "golden-sun-sentou-series";
  resources: IndexEntry[];
}

function p(
  id: number,
  address: number,
  boundarySize: number,
  sourceSize: number,
  decodedSize: number,
  prefixPalette = 0x80,
): Spec {
  return { id, address, boundarySize, sourceSize, decodedSize, prefixPalette, codec: "palette-lz", presentation: "koma-4bpp" };
}

function g(
  id: number,
  address: number,
  boundarySize: number,
  sourceSize: number,
  decodedSize: number,
  prefixPalette = 0,
  presentation: Presentation = prefixPalette ? "koma-4bpp" : "naiyou",
): Spec {
  return { id, address, boundarySize, sourceSize, decodedSize, prefixPalette, codec: "general-lz-prefill", presentation };
}

export const SENTOU_RESOURCES: readonly Spec[] = [
  p(0x03f, 0x083c3574, 0x0784, 0x0781, 0x4b00, 0x20),
  g(0x042, 0x083c5678, 0x2e20, 0x2e1d, 0x9600, 0x1c0, "haikei-8bpp"),
  p(0x044, 0x083cb4bc, 0x0848, 0x0845, 0x6d2a),
  p(0x045, 0x083cbd04, 0x1200, 0x1200, 0x5300),
  p(0x04b, 0x083cdb04, 0x06c4, 0x06c2, 0x7080),
  p(0x04c, 0x083ce1c8, 0x02e8, 0x02e5, 0x7080),
  p(0x04d, 0x083ce4b0, 0x057c, 0x057c, 0x7080),
  p(0x04e, 0x083cea2c, 0x06e8, 0x06e8, 0x7080),
  p(0x04f, 0x083cf114, 0x0700, 0x06ff, 0x7080),
  p(0x050, 0x083cf814, 0x03ec, 0x03e9, 0x7080),
  p(0x052, 0x083cffd0, 0x08a0, 0x089f, 0x5500),
  p(0x056, 0x083d1ecc, 0x0458, 0x0458, 0x10e0),
  p(0x058, 0x083d3390, 0x0fc8, 0x0fc7, 0x3840, 0),
  g(0x05c, 0x083d6010, 0x01b4, 0x01b4, 0x0300),
  p(0x05d, 0x083d61c4, 0x0398, 0x0395, 0x1200),
  p(0x05e, 0x083d655c, 0x1d88, 0x1d86, 0x564c),
  g(0x05f, 0x083d82e4, 0x0278, 0x0277, 0x0480),
  g(0x065, 0x083dc418, 0x043c, 0x043b, 0x08b0),
  g(0x067, 0x083dc968, 0x019c, 0x019c, 0x035c),
  g(0x069, 0x083dd0e4, 0x04a0, 0x049d, 0x0a00),
  p(0x06a, 0x083dd584, 0x0d14, 0x0d12, 0x1deb),
  p(0x06c, 0x083de7d8, 0x0840, 0x083e, 0x1b00, 0),
  p(0x071, 0x083e1ec8, 0x082c, 0x082a, 0x63c0),
  p(0x072, 0x083e26f4, 0x03f0, 0x03f0, 0x3180),
  g(0x074, 0x083e2d80, 0x01bc, 0x01bb, 0x0302),
  g(0x075, 0x083e2f3c, 0x0940, 0x0940, 0x0c08),
  g(0x076, 0x083e387c, 0x0418, 0x0417, 0x0604),
  p(0x07a, 0x083e45bc, 0x03ac, 0x03ab, 0x0800),
  p(0x084, 0x083e9d70, 0x0dcc, 0x0dcb, 0x2d25),
  { ...g(0x08d, 0x083ee404, 0x035c, 0x035b, 0x0640, 0, "koma-4bpp"), displayPaletteBack: 0x80 },
  g(0x08e, 0x083ee760, 0x028c, 0x0289, 0x0320, 0x80),
  p(0x090, 0x083eed90, 0x025c, 0x025a, 0x0320),
  p(0x096, 0x083f088c, 0x15c4, 0x15c2, 0x5100),
  p(0x098, 0x083f28bc, 0x03c4, 0x03c4, 0x0b40),
  p(0x099, 0x083f2c80, 0x0a14, 0x0a12, 0x2d00),
  p(0x09a, 0x083f3694, 0x0384, 0x0381, 0x0810),
  p(0x09b, 0x083f3a18, 0x0cb0, 0x0cad, 0x2580),
  p(0x09c, 0x083f46c8, 0x0b48, 0x0b45, 0x1a40),
  g(0x09d, 0x083f5210, 0x0198, 0x0196, 0x02b8),
  p(0x09e, 0x083f53a8, 0x088c, 0x088b, 0x1b00),
  g(0x0aa, 0x083f7ec4, 0x0298, 0x0297, 0x039c, 0x80),
  p(0x0ab, 0x083f815c, 0x1500, 0x1500, 0x5e01),
  p(0x0ac, 0x083f965c, 0x049c, 0x049b, 0x603e),
  p(0x0ae, 0x083fb484, 0x03ec, 0x03ea, 0x6680),
  p(0x0b0, 0x083fcb58, 0x0c8c, 0x0c8b, 0x3000),
  p(0x0b3, 0x083fe348, 0x0a34, 0x0a33, 0x1901),
  p(0x0b5, 0x083ff978, 0x07d8, 0x07d5, 0x5100),
  p(0x0b6, 0x08400150, 0x0ad0, 0x0ace, 0x5100),
  p(0x0b7, 0x08400c20, 0x0b3c, 0x0b3b, 0x2580),
  g(0x0ba, 0x08403324, 0x0064, 0x0061, 0x0101),
  { ...g(0x0bb, 0x08403408, 0x02ec, 0x02eb, 0x0600, 0, "koma-4bpp"), displayPaletteBack: 0x80 },
  g(0x0be, 0x08403fc8, 0x033c, 0x0339, 0x0400, 0x80),
  g(0x0bf, 0x08404304, 0x1018, 0x1016, 0x239d, 0x80),
  p(0x0c0, 0x0840531c, 0x166c, 0x166c, 0x38dc),
  { ...p(0x0c4, 0x0840914c, 0x0f94, 0x0f94, 0x2d00, 0), displayPaletteBack: 0x80 },
  p(0x0c5, 0x0840a0e0, 0x037c, 0x037b, 0x1073, 0),
  p(0x0c6, 0x0840a45c, 0x0430, 0x042f, 0x0d80, 0),
  p(0x0c7, 0x0840a88c, 0x10cc, 0x10cc, 0x2514),
  p(0x0c9, 0x0840b9dc, 0x04e8, 0x04e8, 0x104e),
  p(0x0ce, 0x0840cf98, 0x1b34, 0x1b32, 0x3956),
] as const;

function hex(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function shortHex(value: number): string {
  return `0x${value.toString(16)}`;
}

function resourceId(value: number): string {
  return `0x${value.toString(16).padStart(3, "0")}`;
}

function integer(value: unknown, label: string): number {
  const result = typeof value === "string" ? Number(value) : value;
  if (!Number.isSafeInteger(result)) throw new Error(`${label} must be an integer`);
  return result as number;
}

function exactKeys(value: Record<string, unknown>, keys: readonly string[], label: string): void {
  const actual = Object.keys(value).sort();
  const expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${label} has unexpected fields`);
  }
}

function document(text: string, pretty: boolean): unknown {
  const value: unknown = JSON.parse(text);
  const canonical = `${JSON.stringify(value, null, pretty ? 2 : 0)}\n`;
  if (text !== canonical) throw new Error("sentou JSON is not canonical");
  return value;
}

function exactBytes(actual: Uint8Array, canonical: Uint8Array, label: string): void {
  if (!Buffer.from(actual).equals(Buffer.from(canonical))) throw new Error(`${label} is not canonical`);
}

function tokenInteger(value: unknown, label: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value)) throw new Error(`${label} must be an integer`);
  return value;
}

function validateGeneralTokens(value: unknown[], decodedSize: number): asserts value is GeneralToken[] {
  let output = 0;
  value.forEach((token, position) => {
    if (!Array.isArray(token) || (token[0] !== "l" && token[0] !== "c")) {
      throw new Error(`sentou general token ${position} has an invalid opcode`);
    }
    if (token[0] === "l") {
      if (token.length !== 2) throw new Error(`sentou general token ${position} has the wrong arity`);
      const length = tokenInteger(token[1], `sentou general token ${position} length`);
      if (length < 1 || output + length > decodedSize) throw new Error(`sentou general token ${position} crossed decoded output`);
      output += length;
      return;
    }
    if (token.length !== 3) throw new Error(`sentou general token ${position} has the wrong arity`);
    const length = tokenInteger(token[1], `sentou general token ${position} length`);
    const distance = tokenInteger(token[2], `sentou general token ${position} distance`);
    const window = output - 33;
    const width = window >= 0 && window < 2048 ? (window === 0 ? 0 : Math.floor(Math.log2(window)) + 1) : 12;
    if (length < 2 || length > 137 || distance < 1 || distance > PREFILL + output || output + length > decodedSize) {
      throw new Error(`sentou general token ${position} crossed its replay bounds`);
    }
    if (distance > 32 && distance - 33 >= 2 ** width) {
      throw new Error(`sentou general token ${position} exceeds its encoded distance width`);
    }
    output += length;
  });
  if (output !== decodedSize) throw new Error("sentou general tokens do not cover decoded output");
}

function validatePaletteTokens(value: unknown[], decodedSize: number): asserts value is PaletteGroup[] {
  let output = 0;
  let ended = false;
  value.forEach((group, groupPosition) => {
    if (!Array.isArray(group) || ended) throw new Error(`sentou palette group ${groupPosition} is invalid`);
    if (group[0] === "z") {
      if (group.length !== 1 || output + 8 > decodedSize) throw new Error(`sentou palette group ${groupPosition} is invalid`);
      output += 8;
      return;
    }
    if (group[0] !== "g" || group.length !== 2 || !Array.isArray(group[1]) || group[1].length < 1 || group[1].length > 8) {
      throw new Error(`sentou palette group ${groupPosition} is invalid`);
    }
    group[1].forEach((token, tokenPosition) => {
      if (!Array.isArray(token)) throw new Error(`sentou palette token ${groupPosition}:${tokenPosition} is invalid`);
      if (token[0] === "l") {
        if (token.length !== 1 || output >= decodedSize) throw new Error(`sentou palette token ${groupPosition}:${tokenPosition} is invalid`);
        output++;
        return;
      }
      if (token[0] === "e") {
        if (token.length !== 1 || tokenPosition + 1 !== group[1].length) {
          throw new Error(`sentou palette token ${groupPosition}:${tokenPosition} is invalid`);
        }
        ended = true;
        return;
      }
      if (token[0] !== "c" || token.length !== 3) throw new Error(`sentou palette token ${groupPosition}:${tokenPosition} is invalid`);
      const length = tokenInteger(token[1], `sentou palette token ${groupPosition}:${tokenPosition} length`);
      const distance = tokenInteger(token[2], `sentou palette token ${groupPosition}:${tokenPosition} distance`);
      if (length < 2 || length > 272 || distance < 1 || distance > output || distance > 0xfff || output + length > decodedSize) {
        throw new Error(`sentou palette token ${groupPosition}:${tokenPosition} crossed its replay bounds`);
      }
      output += length;
    });
    if (!ended && group[1].length !== 8) throw new Error(`sentou palette group ${groupPosition} is incomplete`);
  });
  if (!ended || output !== decodedSize) throw new Error("sentou palette tokens do not cover decoded output");
}

function child(parent: string, name: string): string {
  if (name.includes("/") || name.includes("\\") || name === "." || name === "..") {
    throw new Error("sentou source name must be a direct child");
  }
  const result = resolve(parent, name);
  const position = relative(parent, result);
  if (position.startsWith("../") || position === "..") throw new Error("sentou source escaped its directory");
  return result;
}

function directoryName(spec: Spec): string {
  return `resource_${spec.id.toString(16).padStart(3, "0")}`;
}

function planPath(directory: string, spec: Spec): string {
  return join(directory, directoryName(spec), "stream.json");
}

function imageName(presentation: Presentation): Plan["image"]["source"] {
  if (presentation === "koma-4bpp") return "koma.4bpp.png";
  if (presentation === "haikei-8bpp") return "haikei.8bpp.png";
  return "naiyou.png";
}

function canvasSize(spec: Spec): number {
  if (spec.presentation === "haikei-8bpp") return spec.decodedSize;
  if (spec.presentation === "koma-4bpp") return Math.ceil(spec.decodedSize / 0x3c0) * 0x3c0;
  return Math.ceil(spec.decodedSize / 0x200) * 0x200;
}

function paletteColors(data: Uint8Array): Rgb[] {
  const result: Rgb[] = [];
  const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
  for (let offset = 0; offset < data.length; offset += 2) {
    const value = view.getUint16(offset, true);
    result.push([(value & 31) << 3, (value >>> 5 & 31) << 3, (value >>> 10 & 31) << 3]);
  }
  return result;
}

function specFor(id: number, address: number): Spec {
  const result = SENTOU_RESOURCES.find((spec) => spec.id === id && spec.address === address);
  if (result === undefined) throw new Error(`unsupported sentou resource ${resourceId(id)} at ${hex(address)}`);
  return result;
}

function parsePlan(value: unknown): [Plan, Spec] {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error("sentou plan must be an object");
  const plan = value as Plan;
  exactKeys(plan as unknown as Record<string, unknown>, [
    "format", "kind", "resource_id", "address", "resource_boundary_size", "source_size",
    "prefix_palette", "stream", "image",
  ], "sentou plan");
  if (plan.format !== 1 || plan.kind !== "golden-sun-sentou-resource") throw new Error("unsupported sentou plan");
  if (!/^0x[0-9a-f]{3}$/.test(plan.resource_id)) throw new Error("sentou resource ID is not canonical");
  if (!/^0x[0-9a-f]{8}$/.test(plan.address)) throw new Error("sentou address is not canonical");
  const id = Number(plan.resource_id);
  const address = Number(plan.address);
  const spec = specFor(id, address);
  if (integer(plan.resource_boundary_size, "sentou resource boundary") !== spec.boundarySize ||
      integer(plan.source_size, "sentou source size") !== spec.sourceSize) {
    throw new Error("sentou source bounds differ from the audited resource");
  }
  if (typeof plan.stream !== "object" || plan.stream === null || Array.isArray(plan.stream)) {
    throw new Error("sentou stream must be an object");
  }
  exactKeys(plan.stream as unknown as Record<string, unknown>, ["codec", "tag", "prefill", "decoded_size", "tokens"], "sentou stream");
  if (plan.stream.codec !== spec.codec || plan.stream.tag !== (spec.codec === "palette-lz" ? 1 : 0) ||
      integer(plan.stream.prefill, "sentou stream prefill") !== (spec.codec === "general-lz-prefill" ? PREFILL : 0) ||
      integer(plan.stream.decoded_size, "sentou decoded size") !== spec.decodedSize || !Array.isArray(plan.stream.tokens)) {
    throw new Error("sentou stream layout differs from the audited resource");
  }
  if (spec.codec === "general-lz-prefill") validateGeneralTokens(plan.stream.tokens, spec.decodedSize);
  else validatePaletteTokens(plan.stream.tokens, spec.decodedSize);
  if (typeof plan.image !== "object" || plan.image === null || Array.isArray(plan.image)) {
    throw new Error("sentou image must be an object");
  }
  exactKeys(plan.image as unknown as Record<string, unknown>, ["encoding", "source", "canvas_size", "columns"], "sentou image");
  if (plan.image.encoding !== spec.presentation || plan.image.source !== imageName(spec.presentation) ||
      integer(plan.image.canvas_size, "sentou canvas size") !== canvasSize(spec) ||
      plan.image.columns !== (spec.presentation === "naiyou" ? 64 : 30)) {
    throw new Error("sentou presentation differs from the audited resource");
  }
  if (spec.prefixPalette === 0) {
    if (plan.prefix_palette !== null) throw new Error("sentou plan has an unexpected prefix palette");
  } else {
    if (typeof plan.prefix_palette !== "object" || plan.prefix_palette === null || Array.isArray(plan.prefix_palette)) {
      throw new Error("sentou plan lacks its prefix palette");
    }
    exactKeys(plan.prefix_palette as unknown as Record<string, unknown>, ["size", "source"], "sentou prefix palette");
    if (integer(plan.prefix_palette.size, "sentou prefix palette size") !== spec.prefixPalette ||
        plan.prefix_palette.source !== "iro.rgba.png") {
      throw new Error("sentou prefix palette differs from the audited resource");
    }
  }
  return [plan, spec];
}

function buildDecoded(plan: Plan, spec: Spec, directory: string): Buffer {
  const source = child(directory, plan.image.source);
  const image = readFileSync(source);
  let data: Buffer;
  if (spec.presentation === "naiyou") {
    const [width, , pixels] = indexed_png(image);
    if (width !== 64) throw new Error("sentou data image must be 64 bytes wide");
    data = Buffer.from(pixels);
    exactBytes(image, byte_png(data, 64)[0], "sentou data image");
  } else {
    const bpp = spec.presentation === "koma-4bpp" ? 4 : 8;
    const [pixels, , report] = gba_graphics(image, bpp);
    if (report.width !== 240) throw new Error("sentou graphics source must be 240 pixels wide");
    data = pixels;
    exactBytes(image, tile_png(data, bpp, 30, indexed_png(image)[3])[0], "sentou graphics source");
  }
  if (data.length !== canvasSize(spec) || data.subarray(spec.decodedSize).some((value) => value !== 0)) {
    throw new Error("sentou source canvas has nonzero data outside the decoded stream");
  }
  return data.subarray(0, spec.decodedSize);
}

export function build_sentou_resource(planFile: string): [Buffer, string[]] {
  const [plan, spec] = parsePlan(document(readFileSync(planFile, "utf8"), false));
  const directory = dirname(planFile);
  const decoded = buildDecoded(plan, spec, directory);
  const sources = [planFile, child(directory, plan.image.source)];
  let prefix = Buffer.alloc(0);
  if (plan.prefix_palette !== null) {
    const paletteFile = child(directory, plan.prefix_palette.source);
    const image = readFileSync(paletteFile);
    prefix = gba_palette_rgba(image)[0];
    if (prefix.length !== spec.prefixPalette) throw new Error("sentou prefix palette source has the wrong size");
    exactBytes(image, palette_rgba_image(prefix, 16)[0], "sentou prefix palette source");
    sources.push(paletteFile);
  }
  const encoded = spec.codec === "general-lz-prefill"
    ? encode_general_prefill(decoded, plan.stream.tokens as GeneralToken[], PREFILL, 1)
    : Buffer.concat([Buffer.from([1]), encode_palette(decoded, plan.stream.tokens as PaletteGroup[])]);
  const built = Buffer.concat([prefix, encoded]);
  if (built.length !== spec.sourceSize) throw new Error("sentou resource differs from its audited source size");
  return [built, sources];
}

function parseIndex(value: unknown): IndexDocument {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error("sentou index must be an object");
  const index = value as IndexDocument;
  exactKeys(index as unknown as Record<string, unknown>, ["format", "kind", "resources"], "sentou index");
  if (index.format !== 1 || index.kind !== "golden-sun-sentou-series" || !Array.isArray(index.resources) ||
      index.resources.length !== SENTOU_RESOURCES.length) {
    throw new Error("unsupported sentou series index");
  }
  index.resources.forEach((entry, position) => {
    if (typeof entry !== "object" || entry === null || Array.isArray(entry)) throw new Error("sentou index entry must be an object");
    exactKeys(entry as unknown as Record<string, unknown>, ["id", "address", "size", "resource_boundary_size", "source"], "sentou index entry");
    const spec = SENTOU_RESOURCES[position];
    const expectedSource = `${directoryName(spec)}/stream.json`;
    if (entry.id !== resourceId(spec.id) || entry.address !== hex(spec.address) ||
        integer(entry.size, "sentou index source size") !== spec.sourceSize ||
        integer(entry.resource_boundary_size, "sentou index boundary size") !== spec.boundarySize ||
        entry.source !== expectedSource) {
      throw new Error("sentou index entry differs from the audited series");
    }
  });
  return index;
}

export function build_sentou_series(indexFile: string): Array<{ address: number; data: Buffer; sources: string[] }> {
  const index = parseIndex(document(readFileSync(indexFile, "utf8"), true));
  const directory = dirname(indexFile);
  return index.resources.map((entry) => {
    const [data, sources] = build_sentou_resource(resolve(directory, entry.source));
    return { address: Number(entry.address), data, sources: [indexFile, ...sources] };
  });
}

function resourcePointer(rom: Buffer, id: number): number {
  return rom.readUInt32LE(RESOURCE_TABLE - ROM_BASE + id * 4);
}

function nextResourcePointer(rom: Buffer, id: number): number {
  const current = resourcePointer(rom, id);
  for (let next = id + 1; next < 1000; next++) {
    const pointer = resourcePointer(rom, next);
    if (pointer > current) return pointer;
  }
  throw new Error("sentou resource lacks a following directory pointer");
}

function checkedSource(rom: Buffer, spec: Spec): Buffer {
  const resource = resourcePointer(rom, spec.id);
  const next = nextResourcePointer(rom, spec.id);
  if (spec.address < resource || spec.address + spec.boundarySize !== next) {
    throw new Error(`sentou resource ${resourceId(spec.id)} has different directory bounds`);
  }
  const start = spec.address - ROM_BASE;
  const result = rom.subarray(start, start + spec.boundarySize);
  if (result.length !== spec.boundarySize) throw new Error("sentou resource lies outside the ROM");
  return result;
}

function writeImage(directory: string, spec: Spec, decoded: Buffer, palette: Buffer): void {
  const canvas = Buffer.alloc(canvasSize(spec));
  decoded.copy(canvas);
  let image: Buffer;
  if (spec.presentation === "naiyou") image = byte_png(canvas, 64)[0];
  else {
    const bpp = spec.presentation === "koma-4bpp" ? 4 : 8;
    const colors = palette.length ? paletteColors(palette).slice(0, bpp === 4 ? 16 : 256) : undefined;
    image = tile_png(canvas, bpp, 30, colors)[0];
  }
  writeFileSync(join(directory, imageName(spec.presentation)), image);
}

function exportOne(rom: Buffer, root: string, spec: Spec): IndexEntry {
  const original = checkedSource(rom, spec);
  const prefix = original.subarray(0, spec.prefixPalette);
  const streamStart = spec.prefixPalette + (spec.codec === "palette-lz" ? 1 : 0);
  let decoded: Buffer;
  let tokens: GeneralToken[] | PaletteGroup[];
  if (spec.codec === "general-lz-prefill") {
    [decoded, , tokens] = decode_general_prefill_trace(original, streamStart, original.length, spec.decodedSize, PREFILL, 1);
  } else {
    if (original[spec.prefixPalette] !== 1) throw new Error("sentou palette stream lacks tag 1");
    [decoded, , tokens] = decode_palette_trace(original, streamStart, original.length, spec.decodedSize);
  }
  if (decoded.length !== spec.decodedSize) throw new Error("sentou decoded size differs from its audited layout");
  const directory = join(root, directoryName(spec));
  mkdirSync(directory, { recursive: true });
  if (spec.prefixPalette) writeFileSync(join(directory, "iro.rgba.png"), palette_rgba_image(prefix, 16)[0]);
  const displayPalette = spec.displayPaletteBack
    ? rom.subarray(spec.address - ROM_BASE - spec.displayPaletteBack, spec.address - ROM_BASE)
    : prefix;
  writeImage(directory, spec, decoded, displayPalette);
  const plan: Plan = {
    format: 1,
    kind: "golden-sun-sentou-resource",
    resource_id: resourceId(spec.id),
    address: hex(spec.address),
    resource_boundary_size: shortHex(spec.boundarySize),
    source_size: shortHex(spec.sourceSize),
    prefix_palette: spec.prefixPalette ? { size: shortHex(spec.prefixPalette), source: "iro.rgba.png" } : null,
    stream: {
      codec: spec.codec,
      tag: spec.codec === "palette-lz" ? 1 : 0,
      prefill: shortHex(spec.codec === "general-lz-prefill" ? PREFILL : 0),
      decoded_size: shortHex(spec.decodedSize),
      tokens,
    },
    image: {
      encoding: spec.presentation,
      source: imageName(spec.presentation),
      canvas_size: shortHex(canvasSize(spec)),
      columns: spec.presentation === "naiyou" ? 64 : 30,
    },
  };
  const streamFile = join(directory, "stream.json");
  writeFileSync(streamFile, `${JSON.stringify(plan)}\n`);
  const [rebuilt] = build_sentou_resource(streamFile);
  if (!rebuilt.equals(original.subarray(0, spec.sourceSize))) throw new Error("sentou export does not round-trip");
  return {
    id: resourceId(spec.id),
    address: hex(spec.address),
    size: shortHex(spec.sourceSize),
    resource_boundary_size: shortHex(spec.boundarySize),
    source: `${directoryName(spec)}/stream.json`,
  };
}

export function export_sentou_resources(romFile: string, directory: string): void {
  const rom = readFileSync(romFile);
  mkdirSync(directory, { recursive: true });
  const index: IndexDocument = {
    format: 1,
    kind: "golden-sun-sentou-series",
    resources: SENTOU_RESOURCES.map((spec) => exportOne(rom, directory, spec)),
  };
  writeFileSync(join(directory, "index.json"), `${JSON.stringify(index, null, 2)}\n`);
  verify_sentou_resources(romFile, directory);
}

export function verify_sentou_resources(romFile: string, directory: string): void {
  const rom = readFileSync(romFile);
  const built = build_sentou_series(join(directory, "index.json"));
  let sourceBytes = 0;
  let boundaryBytes = 0;
  built.forEach((entry, position) => {
    const spec = SENTOU_RESOURCES[position];
    if (entry.address !== spec.address || !entry.data.equals(checkedSource(rom, spec).subarray(0, spec.sourceSize))) {
      throw new Error(`sentou resource ${resourceId(spec.id)} differs from ROM`);
    }
    sourceBytes += entry.data.length;
    boundaryBytes += spec.boundarySize;
  });
  console.log(`identical=true resources=${built.length} source_bytes=${sourceBytes} boundary_bytes=${boundaryBytes} suffix_fallback=${boundaryBytes - sourceBytes}`);
}

export function self_test(): void {
  if (SENTOU_RESOURCES.length !== 60 || SENTOU_RESOURCES.reduce((sum, spec) => sum + spec.sourceSize, 0) !== 137869 ||
      SENTOU_RESOURCES.reduce((sum, spec) => sum + spec.boundarySize, 0) !== 137956 ||
      canvasSize(specFor(0x042, 0x083c5678)) !== 0x9600 || canvasSize(specFor(0x044, 0x083cb4bc)) !== 0x7080) {
    throw new Error("sentou resource catalogue self-test failed");
  }
  const rejects = (action: () => void): boolean => { try { action(); return false; } catch { return true; } };
  if (!rejects(() => child("assets/graphics/sentou", "../private.png")) ||
      !rejects(() => document("{\"value\":1,\"value\":1}\n", false)) ||
      !rejects(() => document("{} \n", false)) ||
      !rejects(() => exactBytes(Buffer.concat([byte_png(Buffer.alloc(0x200), 64)[0], Buffer.from([0])]), byte_png(Buffer.alloc(0x200), 64)[0], "test image")) ||
      !rejects(() => validateGeneralTokens([["l", "1"]] as unknown[], 1)) ||
      !rejects(() => validateGeneralTokens([["x", 1]] as unknown[], 1)) ||
      !rejects(() => validateGeneralTokens([["l", 1, 2]] as unknown[], 1)) ||
      !rejects(() => validateGeneralTokens([["c", 2, PREFILL + 1]] as unknown[], 2)) ||
      !rejects(() => validateGeneralTokens([["l", 33], ["c", 2, 34]] as unknown[], 35)) ||
      !rejects(() => validateGeneralTokens([["l", 2]] as unknown[], 1)) ||
      !rejects(() => validatePaletteTokens([["g", [["e"], ["l"]]]] as unknown[], 0)) ||
      !rejects(() => validatePaletteTokens([["g", [["l"]]], ["g", [["e"]]]] as unknown[], 1)) ||
      !rejects(() => validatePaletteTokens([["g", [["c", 2, 1]]], ["g", [["e"]]]] as unknown[], 2))) {
    throw new Error("sentou adversarial plan self-test failed");
  }
  console.log("self-test=ok");
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || args[index + 1] === undefined) throw new Error(`${name} is required`);
  return args[index + 1];
}

function same(left: string, right: string): boolean {
  try { return realpathSync(left) === realpathSync(right); }
  catch { return resolve(left) === resolve(right); }
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: sentou_resources.ts [--self-test] {export|verify} ROM --directory DIRECTORY");
    return;
  }
  if (args.includes("--self-test")) {
    self_test();
    args = args.filter((argument) => argument !== "--self-test");
    if (!args.length) return;
  }
  const [command, romFile] = args.filter((argument, index) => !argument.startsWith("-") && args[index - 1] !== "--directory");
  if (!romFile || (command !== "export" && command !== "verify")) throw new Error("export or verify and a ROM are required");
  const directory = option(args, "--directory");
  if (same(romFile, directory)) throw new Error("refusing to overwrite the input ROM");
  if (command === "export") export_sentou_resources(romFile, directory);
  else verify_sentou_resources(romFile, directory);
}

if (import.meta.main) main(Bun.argv.slice(2));
