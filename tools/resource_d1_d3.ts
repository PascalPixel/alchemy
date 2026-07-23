#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { canonicalJson, isCanonicalJsonText } from "./canonical_json.ts";
import {
  existsSync,
  lstatSync,
  mkdirSync,
  mkdtempSync,
  readFileSync,
  readdirSync,
  realpathSync,
  renameSync,
  rmSync,
  symlinkSync,
  writeFileSync,
} from "node:fs";
import { basename, dirname, isAbsolute, join, relative, resolve } from "node:path";
import { tmpdir } from "node:os";
import {
  decode_palette_trace,
  encode_palette,
  type PaletteGroup,
} from "./extract_resource.ts";
import { palette_rgba_image, tile_png } from "./export_asset.ts";
import { gba_graphics, gba_palette_rgba, indexed_png, type Rgb } from "./import_asset.ts";
import {
  build_alignment_tail,
  inspect_alignment_tail,
  parse_alignment_tail,
  self_test_alignment_tail,
  type AlignmentTail,
} from "./alignment_tail.ts";

const ROM_BASE = 0x08000000;
const ROM_SIZE = 0x00800000;
const RESOURCE_TABLE = 0x08320000;

export const D1_D3_ADDRESS = 0x084101d0;
export const D1_D3_END = 0x08411474;
export const D1_D3_BOUNDARY_SIZE = D1_D3_END - D1_D3_ADDRESS;

interface Spec {
  id: number;
  address: number;
  boundarySize: number;
  sourceSize: number;
  source: string;
}

const D1: Spec = {
  id: 0x0d1,
  address: 0x084101d0,
  boundarySize: 0x10a8,
  sourceSize: 0x10a5,
  source: "stream.json",
};

const D2: Spec = {
  id: 0x0d2,
  address: 0x08411278,
  boundarySize: 0x01a8,
  sourceSize: 0x01a8,
  source: "idou_d2.json",
};

const D3: Spec = {
  id: 0x0d3,
  address: 0x08411420,
  boundarySize: 0x0054,
  sourceSize: 0x0054,
  source: "idou_d3.json",
};

const SPECS = [D1, D2, D3] as const;
const D1_PALETTE_SIZE = 0x80;
const D1_DECODED_SIZE = 0x2a00;
const D1_COLUMNS = 14;
const D1_WIDTH = D1_COLUMNS * 8;
const D1_HEIGHT = 96;

interface IndexEntry {
  id: string;
  address: string;
  resource_boundary_size: string;
  source_size: string;
  source: string;
}

interface IndexDocument {
  format: 1;
  kind: "golden-sun-d1-d3-package";
  address: string;
  end: string;
  boundary_size: string;
  resources: IndexEntry[];
}

interface StreamPlan {
  format: 1;
  kind: "golden-sun-d1-background";
  resource_id: "0x0d1";
  address: string;
  resource_boundary_size: string;
  source_size: string;
  consumer: {
    function: "Func_080d41a4";
    loader: "Func_080e0524";
    decompressor: "Func_08005340";
    palette_bytes: string;
    graphics_bpp: 8;
  };
  palette: {
    source: "iro.rgba.png";
    size: string;
  };
  stream: {
    codec: "palette-lz";
    tag: 1;
    decoded_size: string;
    tokens: PaletteGroup[];
  };
  image: {
    source: "haikei.8bpp.png";
    encoding: "gba-8bpp-linear-tiles";
    width: number;
    height: number;
    columns: number;
  };
  suffix: {
    size: 3;
    policy: "fallback";
  } | AlignmentTail;
}

interface MotionPlan {
  format: 1;
  kind: "golden-sun-motion-path";
  resource_id: string;
  address: string;
  size: string;
  consumer: {
    function: string;
    initial_frame: number;
    delta_first_frame: number;
    delta_last_frame: number;
    initial_encoding: "be-s16-pair";
    delta_encoding: "s8-pair";
    boundary_delta_pairs: number;
  };
  initial: [number, number];
  deltas: [number, number][];
  boundary_deltas: [number, number][];
}

export interface BuiltD1D3Resource {
  id: number;
  address: number;
  boundarySize: number;
  data: Buffer;
  sources: string[];
}

function hexadecimal(value: number, width = 8): string {
  return `0x${value.toString(16).padStart(width, "0")}`;
}

function idText(value: number): string {
  return `0x${value.toString(16).padStart(3, "0")}`;
}

function exactKeys(value: Record<string, unknown>, keys: readonly string[], label: string): void {
  const actual = Object.keys(value).sort();
  const expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${label} has unexpected fields`);
  }
}

function integer(value: unknown, label: string): number {
  const result = typeof value === "string" ? Number(value) : value;
  if (!Number.isSafeInteger(result)) throw new Error(`${label} must be an integer`);
  return result as number;
}

function canonicalHex(value: unknown, expected: number, width: number, label: string): void {
  if (value !== hexadecimal(expected, width)) throw new Error(`${label} differs from the audited extent`);
}

function jsonDocument(path: string, pretty: boolean, label: string): unknown {
  const text = readFileSync(path, "utf8");
  const value: unknown = JSON.parse(text);
    if (!isCanonicalJsonText(text, value)) throw new Error(`${label} is not canonical JSON`);
  return value;
}

function exactBytes(actual: Uint8Array, canonical: Uint8Array, label: string): void {
  if (!Buffer.from(actual).equals(Buffer.from(canonical))) throw new Error(`${label} is not canonical`);
}

function contained(directory: string, path: string): boolean {
  const position = relative(directory, path);
  return position === "" || (position !== ".." && !position.startsWith("../") &&
    !position.startsWith("..\\") && !isAbsolute(position));
}

function physical(path: string): string {
  const absolute = resolve(path);
  let ancestor = absolute;
  while (!existsSync(ancestor)) {
    const parent = dirname(ancestor);
    if (parent === ancestor) return absolute;
    ancestor = parent;
  }
  return resolve(realpathSync(ancestor), relative(ancestor, absolute));
}

function child(root: string, name: string): string {
  const canonicalRoot = realpathSync(root);
  const path = realpathSync(resolve(canonicalRoot, name));
  if (!contained(canonicalRoot, path)) throw new Error("D1-D3 source escaped its package");
  return path;
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

function tokenInteger(value: unknown, label: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value)) throw new Error(`${label} must be an integer`);
  return value;
}

function validatePaletteTokens(value: unknown[], decodedSize: number): asserts value is PaletteGroup[] {
  let output = 0;
  let ended = false;
  value.forEach((group, groupPosition) => {
    if (!Array.isArray(group) || ended) throw new Error(`palette group ${groupPosition} is invalid`);
    if (group[0] === "z") {
      if (group.length !== 1 || output + 8 > decodedSize) throw new Error(`palette group ${groupPosition} is invalid`);
      output += 8;
      return;
    }
    if (group[0] !== "g" || group.length !== 2 || !Array.isArray(group[1]) ||
        group[1].length < 1 || group[1].length > 8) {
      throw new Error(`palette group ${groupPosition} is invalid`);
    }
    const operations = group[1];
    const terminal = Array.isArray(operations.at(-1)) && operations.at(-1)?.[0] === "e";
    if ((!terminal && operations.length !== 8) ||
        !operations.some((token) => Array.isArray(token) && (token[0] === "c" || token[0] === "e"))) {
      throw new Error(`palette group ${groupPosition} is not canonical`);
    }
    operations.forEach((token, tokenPosition) => {
      if (!Array.isArray(token)) throw new Error(`palette token ${groupPosition}:${tokenPosition} is invalid`);
      if (token[0] === "l") {
        if (token.length !== 1 || output >= decodedSize) throw new Error(`palette token ${groupPosition}:${tokenPosition} is invalid`);
        output++;
        return;
      }
      if (token[0] === "e") {
        if (token.length !== 1 || tokenPosition + 1 !== group[1].length) {
          throw new Error(`palette token ${groupPosition}:${tokenPosition} is invalid`);
        }
        ended = true;
        return;
      }
      if (token[0] !== "c" || token.length !== 3) throw new Error(`palette token ${groupPosition}:${tokenPosition} is invalid`);
      const length = tokenInteger(token[1], `palette token ${groupPosition}:${tokenPosition} length`);
      const distance = tokenInteger(token[2], `palette token ${groupPosition}:${tokenPosition} distance`);
      if (length < 2 || length > 272 || distance < 1 || distance > output || output + length > decodedSize) {
        throw new Error(`palette token ${groupPosition}:${tokenPosition} crossed its replay bounds`);
      }
      output += length;
    });
  });
  if (!ended || output !== decodedSize) throw new Error("palette tokens do not cover the decoded background");
}

function parseIndex(value: unknown): IndexDocument {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error("D1-D3 index must be an object");
  const index = value as IndexDocument;
  exactKeys(index as unknown as Record<string, unknown>, [
    "format", "kind", "address", "end", "boundary_size", "resources",
  ], "D1-D3 index");
  if (index.format !== 1 || index.kind !== "golden-sun-d1-d3-package" || !Array.isArray(index.resources) ||
      index.resources.length !== SPECS.length) {
    throw new Error("unsupported D1-D3 index");
  }
  canonicalHex(index.address, D1_D3_ADDRESS, 8, "D1-D3 index address");
  canonicalHex(index.end, D1_D3_END, 8, "D1-D3 index end");
  canonicalHex(index.boundary_size, D1_D3_BOUNDARY_SIZE, 4, "D1-D3 index boundary size");
  index.resources.forEach((entry, position) => {
    if (typeof entry !== "object" || entry === null || Array.isArray(entry)) throw new Error("D1-D3 index entry must be an object");
    exactKeys(entry as unknown as Record<string, unknown>, [
      "id", "address", "resource_boundary_size", "source_size", "source",
    ], "D1-D3 index entry");
    const spec = SPECS[position];
    if (entry.id !== idText(spec.id) || entry.address !== hexadecimal(spec.address) ||
        entry.resource_boundary_size !== hexadecimal(spec.boundarySize, 4) ||
        entry.source_size !== hexadecimal(spec.sourceSize, 4) || entry.source !== spec.source) {
      throw new Error("D1-D3 index entry differs from the audited resource directory");
    }
  });
  return index;
}

function parseStream(value: unknown): StreamPlan {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error("D1 stream plan must be an object");
  const plan = value as StreamPlan;
  exactKeys(plan as unknown as Record<string, unknown>, [
    "format", "kind", "resource_id", "address", "resource_boundary_size", "source_size",
    "consumer", "palette", "stream", "image", "suffix",
  ], "D1 stream plan");
  if (plan.format !== 1 || plan.kind !== "golden-sun-d1-background" || plan.resource_id !== "0x0d1") {
    throw new Error("unsupported D1 stream plan");
  }
  canonicalHex(plan.address, D1.address, 8, "D1 address");
  canonicalHex(plan.resource_boundary_size, D1.boundarySize, 4, "D1 boundary size");
  canonicalHex(plan.source_size, D1.sourceSize, 4, "D1 source size");
  if (typeof plan.consumer !== "object" || plan.consumer === null || Array.isArray(plan.consumer)) throw new Error("D1 consumer must be an object");
  exactKeys(plan.consumer as unknown as Record<string, unknown>, [
    "function", "loader", "decompressor", "palette_bytes", "graphics_bpp",
  ], "D1 consumer");
  if (plan.consumer.function !== "Func_080d41a4" || plan.consumer.loader !== "Func_080e0524" ||
      plan.consumer.decompressor !== "Func_08005340" ||
      plan.consumer.palette_bytes !== hexadecimal(D1_PALETTE_SIZE, 2) || plan.consumer.graphics_bpp !== 8) {
    throw new Error("D1 consumer framing differs from reconstructed code");
  }
  if (typeof plan.palette !== "object" || plan.palette === null || Array.isArray(plan.palette)) throw new Error("D1 palette must be an object");
  exactKeys(plan.palette as unknown as Record<string, unknown>, ["source", "size"], "D1 palette");
  if (plan.palette.source !== "iro.rgba.png" || plan.palette.size !== hexadecimal(D1_PALETTE_SIZE, 2)) {
    throw new Error("D1 palette framing differs");
  }
  if (typeof plan.stream !== "object" || plan.stream === null || Array.isArray(plan.stream)) throw new Error("D1 stream must be an object");
  exactKeys(plan.stream as unknown as Record<string, unknown>, ["codec", "tag", "decoded_size", "tokens"], "D1 stream");
  if (plan.stream.codec !== "palette-lz" || plan.stream.tag !== 1 ||
      plan.stream.decoded_size !== hexadecimal(D1_DECODED_SIZE, 4) || !Array.isArray(plan.stream.tokens)) {
    throw new Error("D1 compression framing differs");
  }
  validatePaletteTokens(plan.stream.tokens, D1_DECODED_SIZE);
  if (typeof plan.image !== "object" || plan.image === null || Array.isArray(plan.image)) throw new Error("D1 image must be an object");
  exactKeys(plan.image as unknown as Record<string, unknown>, [
    "source", "encoding", "width", "height", "columns",
  ], "D1 image");
  if (plan.image.source !== "haikei.8bpp.png" || plan.image.encoding !== "gba-8bpp-linear-tiles" ||
      plan.image.width !== D1_WIDTH || plan.image.height !== D1_HEIGHT || plan.image.columns !== D1_COLUMNS) {
    throw new Error("D1 image framing differs");
  }
  if (typeof plan.suffix !== "object" || plan.suffix === null || Array.isArray(plan.suffix)) throw new Error("D1 suffix must be an object");
  if ("policy" in plan.suffix) {
    exactKeys(plan.suffix as unknown as Record<string, unknown>, ["size", "policy"], "D1 suffix");
    if (plan.suffix.size !== D1.boundarySize - D1.sourceSize || plan.suffix.policy !== "fallback") {
      throw new Error("D1 suffix policy differs");
    }
  } else {
    plan.suffix = parse_alignment_tail(plan.suffix, D1.boundarySize - D1.sourceSize, 3, "D1 suffix");
  }
  return plan;
}

function motionShape(spec: Spec): {
  consumer: string;
  initialFrame: number;
  firstFrame: number;
  lastFrame: number;
  deltas: number;
  boundaryDeltas: number;
} {
  if (spec.id === D2.id) {
    return { consumer: "Func_080dc968", initialFrame: 0, firstFrame: 1, lastFrame: 209, deltas: 209, boundaryDeltas: 1 };
  }
  if (spec.id === D3.id) {
    return { consumer: "Func_080e89ec", initialFrame: 18, firstFrame: 19, lastFrame: 58, deltas: 40, boundaryDeltas: 0 };
  }
  throw new Error("resource is not a motion path");
}

function coordinatePair(value: unknown, label: string, minimum: number, maximum: number): [number, number] {
  if (!Array.isArray(value) || value.length !== 2) throw new Error(`${label} must be a coordinate pair`);
  const x = tokenInteger(value[0], `${label} X`);
  const y = tokenInteger(value[1], `${label} Y`);
  if (x < minimum || x > maximum || y < minimum || y > maximum) throw new Error(`${label} is outside its encoded range`);
  return [x, y];
}

function parseMotion(value: unknown, spec: Spec): MotionPlan {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error("motion path must be an object");
  const plan = value as MotionPlan;
  exactKeys(plan as unknown as Record<string, unknown>, [
    "format", "kind", "resource_id", "address", "size", "consumer", "initial", "deltas", "boundary_deltas",
  ], "motion path");
  if (plan.format !== 1 || plan.kind !== "golden-sun-motion-path" || plan.resource_id !== idText(spec.id)) {
    throw new Error("unsupported motion path");
  }
  canonicalHex(plan.address, spec.address, 8, "motion-path address");
  canonicalHex(plan.size, spec.sourceSize, 4, "motion-path size");
  if (typeof plan.consumer !== "object" || plan.consumer === null || Array.isArray(plan.consumer)) {
    throw new Error("motion-path consumer must be an object");
  }
  exactKeys(plan.consumer as unknown as Record<string, unknown>, [
    "function", "initial_frame", "delta_first_frame", "delta_last_frame",
    "initial_encoding", "delta_encoding", "boundary_delta_pairs",
  ], "motion-path consumer");
  const shape = motionShape(spec);
  if (plan.consumer.function !== shape.consumer || plan.consumer.initial_frame !== shape.initialFrame ||
      plan.consumer.delta_first_frame !== shape.firstFrame || plan.consumer.delta_last_frame !== shape.lastFrame ||
      plan.consumer.initial_encoding !== "be-s16-pair" || plan.consumer.delta_encoding !== "s8-pair" ||
      plan.consumer.boundary_delta_pairs !== shape.boundaryDeltas) {
    throw new Error("motion-path consumer shape differs from reconstructed code");
  }
  coordinatePair(plan.initial, "motion-path initial coordinate", -0x8000, 0x7fff);
  if (!Array.isArray(plan.deltas) || plan.deltas.length !== shape.deltas ||
      !Array.isArray(plan.boundary_deltas) || plan.boundary_deltas.length !== shape.boundaryDeltas) {
    throw new Error("motion-path record count differs from reconstructed code");
  }
  plan.deltas.forEach((pair, index) => coordinatePair(pair, `motion-path delta ${index}`, -0x80, 0x7f));
  plan.boundary_deltas.forEach((pair, index) => coordinatePair(pair, `motion-path boundary delta ${index}`, -0x80, 0x7f));
  if (4 + (plan.deltas.length + plan.boundary_deltas.length) * 2 !== spec.sourceSize) {
    throw new Error("motion-path records do not fill their resource boundary");
  }
  return plan;
}

function buildStream(path: string): [Buffer, string[]] {
  const plan = parseStream(jsonDocument(path, false, "D1 stream plan"));
  const root = dirname(path);
  const planPrefix = basename(path).replace(/stream\.json$/, "");
  const palettePath = child(root, planPrefix + plan.palette.source);
  const imagePath = child(root, planPrefix + plan.image.source);
  const paletteImage = readFileSync(palettePath);
  const palette = gba_palette_rgba(paletteImage)[0];
  if (palette.length !== D1_PALETTE_SIZE) throw new Error("D1 palette has the wrong size");
  exactBytes(paletteImage, palette_rgba_image(palette, 16)[0], "D1 palette image");
  const image = readFileSync(imagePath);
  const [decoded, imagePalette, report] = gba_graphics(image, 8);
  if (decoded.length !== D1_DECODED_SIZE || report.width !== D1_WIDTH || report.height !== D1_HEIGHT ||
      report.palette_entries !== D1_PALETTE_SIZE / 2 || !imagePalette.equals(palette)) {
    throw new Error("D1 background image differs from its consumer-framed layout");
  }
  exactBytes(image, tile_png(decoded, 8, D1_COLUMNS, indexed_png(image)[3])[0], "D1 background image");
  const encoded = Buffer.concat([palette, Buffer.from([1]), encode_palette(decoded, plan.stream.tokens)]);
  if (encoded.length !== D1.sourceSize) throw new Error("D1 source differs from its audited encoded size");
  const data = "policy" in plan.suffix
    ? encoded
    : Buffer.concat([encoded, build_alignment_tail(plan.suffix)]);
  if (data.length !== D1.sourceSize && data.length !== D1.boundarySize) throw new Error("D1 built size differs");
  return [data, [path, palettePath, imagePath]];
}

function buildMotion(path: string, spec: Spec): Buffer {
  const plan = parseMotion(jsonDocument(path, true, `${idText(spec.id)} motion path`), spec);
  const output = Buffer.alloc(spec.sourceSize);
  output.writeInt16BE(plan.initial[0], 0);
  output.writeInt16BE(plan.initial[1], 2);
  let cursor = 4;
  for (const [x, y] of [...plan.deltas, ...plan.boundary_deltas]) {
    output.writeInt8(x, cursor++);
    output.writeInt8(y, cursor++);
  }
  if (cursor !== output.length) throw new Error("motion-path encoder did not fill its resource");
  return output;
}

export function build_resource_d1_d3(indexPath: string): BuiltD1D3Resource[] {
  const index = parseIndex(jsonDocument(indexPath, true, "D1-D3 index"));
  const root = dirname(indexPath);
  const prefix = basename(indexPath).replace(/index\.json$/, "");
  return index.resources.map((entry, position) => {
    const spec = SPECS[position];
    const source = child(root, prefix + entry.source);
    let data: Buffer;
    let sources: string[];
    if (spec.id === D1.id) [data, sources] = buildStream(source);
    else {
      data = buildMotion(source, spec);
      sources = [source];
    }
    if (data.length !== spec.sourceSize && data.length !== spec.boundarySize) throw new Error(`${idText(spec.id)} built size differs`);
    return { id: spec.id, address: spec.address, boundarySize: spec.boundarySize, data, sources: [indexPath, ...sources] };
  });
}

function resourcePointer(rom: Buffer, id: number): number {
  const offset = RESOURCE_TABLE - ROM_BASE + id * 4;
  if (offset < 0 || offset + 4 > rom.length) throw new Error("resource directory lies outside the ROM");
  return rom.readUInt32LE(offset);
}

function checkedBoundary(rom: Buffer, spec: Spec): Buffer {
  const start = resourcePointer(rom, spec.id);
  const end = resourcePointer(rom, spec.id + 1);
  if (start !== spec.address || end !== spec.address + spec.boundarySize) {
    throw new Error(`${idText(spec.id)} resource-directory boundary differs`);
  }
  const data = rom.subarray(start - ROM_BASE, end - ROM_BASE);
  if (data.length !== spec.boundarySize) throw new Error(`${idText(spec.id)} lies outside the ROM`);
  return data;
}

function exportStream(original: Buffer, root: string): void {
  if (original[D1_PALETTE_SIZE] !== 1) throw new Error("D1 stream lacks its tag-1 header");
  const palette = original.subarray(0, D1_PALETTE_SIZE);
  const [decoded, cursor, tokens] = decode_palette_trace(
    original,
    D1_PALETTE_SIZE + 1,
    original.length,
    D1_DECODED_SIZE,
  );
  if (decoded.length !== D1_DECODED_SIZE || cursor !== D1.sourceSize) {
    throw new Error("D1 stream termination differs from its audited source extent");
  }
  writeFileSync(join(root, "iro.rgba.png"), palette_rgba_image(palette, 16)[0]);
  writeFileSync(join(root, "haikei.8bpp.png"), tile_png(decoded, 8, D1_COLUMNS, paletteColors(palette))[0]);
  const plan: StreamPlan = {
    format: 1,
    kind: "golden-sun-d1-background",
    resource_id: "0x0d1",
    address: hexadecimal(D1.address),
    resource_boundary_size: hexadecimal(D1.boundarySize, 4),
    source_size: hexadecimal(D1.sourceSize, 4),
    consumer: {
      function: "Func_080d41a4",
      loader: "Func_080e0524",
      decompressor: "Func_08005340",
      palette_bytes: hexadecimal(D1_PALETTE_SIZE, 2),
      graphics_bpp: 8,
    },
    palette: { source: "iro.rgba.png", size: hexadecimal(D1_PALETTE_SIZE, 2) },
    stream: { codec: "palette-lz", tag: 1, decoded_size: hexadecimal(D1_DECODED_SIZE, 4), tokens },
    image: {
      source: "haikei.8bpp.png",
      encoding: "gba-8bpp-linear-tiles",
      width: D1_WIDTH,
      height: D1_HEIGHT,
      columns: D1_COLUMNS,
    },
    suffix: inspect_alignment_tail(original.subarray(D1.sourceSize), 3),
  };
  writeFileSync(join(root, D1.source), `${JSON.stringify(plan)}\n`);
  const [rebuilt] = buildStream(join(root, D1.source));
  if (!rebuilt.equals(original)) throw new Error("D1 export does not round-trip");
}

function signed(byte: number): number {
  return byte > 0x7f ? byte - 0x100 : byte;
}

function exportMotion(original: Buffer, root: string, spec: Spec): void {
  const shape = motionShape(spec);
  const pairs: [number, number][] = [];
  for (let cursor = 4; cursor < original.length; cursor += 2) pairs.push([signed(original[cursor]), signed(original[cursor + 1])]);
  const plan: MotionPlan = {
    format: 1,
    kind: "golden-sun-motion-path",
    resource_id: idText(spec.id),
    address: hexadecimal(spec.address),
    size: hexadecimal(spec.sourceSize, 4),
    consumer: {
      function: shape.consumer,
      initial_frame: shape.initialFrame,
      delta_first_frame: shape.firstFrame,
      delta_last_frame: shape.lastFrame,
      initial_encoding: "be-s16-pair",
      delta_encoding: "s8-pair",
      boundary_delta_pairs: shape.boundaryDeltas,
    },
    initial: [original.readInt16BE(0), original.readInt16BE(2)],
    deltas: pairs.slice(0, shape.deltas),
    boundary_deltas: pairs.slice(shape.deltas),
  };
  writeFileSync(join(root, spec.source), `${canonicalJson(plan)}\n`);
  if (!buildMotion(join(root, spec.source), spec).equals(original)) throw new Error(`${idText(spec.id)} export does not round-trip`);
}

function expectedIndex(): IndexDocument {
  return {
    format: 1,
    kind: "golden-sun-d1-d3-package",
    address: hexadecimal(D1_D3_ADDRESS),
    end: hexadecimal(D1_D3_END),
    boundary_size: hexadecimal(D1_D3_BOUNDARY_SIZE, 4),
    resources: SPECS.map((spec) => ({
      id: idText(spec.id),
      address: hexadecimal(spec.address),
      resource_boundary_size: hexadecimal(spec.boundarySize, 4),
      source_size: hexadecimal(spec.sourceSize, 4),
      source: spec.source,
    })),
  };
}

function writePackage(rom: Buffer, directory: string): void {
  mkdirSync(directory, { recursive: true });
  exportStream(checkedBoundary(rom, D1), directory);
  exportMotion(checkedBoundary(rom, D2), directory, D2);
  exportMotion(checkedBoundary(rom, D3), directory, D3);
  writeFileSync(join(directory, "index.json"), `${canonicalJson(expectedIndex())}\n`);
}

function verifyBuilt(rom: Buffer, indexPath: string): { claimedBytes: number; boundaryBytes: number } {
  const built = build_resource_d1_d3(indexPath);
  let claimedBytes = 0;
  let boundaryBytes = 0;
  built.forEach((resource, position) => {
    const spec = SPECS[position];
    const original = checkedBoundary(rom, spec);
    if (resource.id !== spec.id || resource.address !== spec.address || resource.boundarySize !== spec.boundarySize ||
        !resource.data.equals(original.subarray(0, resource.data.length))) {
      throw new Error(`${idText(spec.id)} reconstructed source differs from ROM`);
    }
    claimedBytes += resource.data.length;
    boundaryBytes += resource.boundarySize;
  });
  return { claimedBytes, boundaryBytes };
}

function validateExportDestination(romPath: string, directory: string): void {
  const lexicalRom = resolve(romPath);
  const lexicalDestination = resolve(directory);
  if (lexicalRom === lexicalDestination || contained(lexicalDestination, lexicalRom)) {
    throw new Error("D1-D3 export directory must not contain its input ROM path");
  }
  const rom = physical(romPath);
  const destination = physical(directory);
  if (rom === destination || contained(destination, rom)) {
    throw new Error("D1-D3 export directory must not contain its input ROM");
  }
  if (!existsSync(directory)) return;
  const stat = lstatSync(directory);
  if (!stat.isDirectory() || stat.isSymbolicLink()) throw new Error("D1-D3 export destination must be a real directory");
  if (readdirSync(directory).length === 0) return;
  const marker = join(directory, "index.json");
  if (!existsSync(marker) || !lstatSync(marker).isFile()) {
    throw new Error("refusing to replace a directory that is not a D1-D3 package");
  }
  try {
    parseIndex(jsonDocument(marker, true, "D1-D3 index"));
  } catch {
    throw new Error("refusing to replace a directory that is not a canonical D1-D3 package");
  }
}

export function export_resource_d1_d3(romPath: string, directory: string): void {
  validateExportDestination(romPath, directory);
  const rom = readFileSync(romPath);
  if (rom.length !== ROM_SIZE) throw new Error("canonical ROM must be exactly 8 MiB");
  const destination = resolve(directory);
  const parent = dirname(destination);
  mkdirSync(parent, { recursive: true });
  const staging = mkdtempSync(join(parent, `.${basename(destination)}.tmp-`));
  const backup = `${staging}.old`;
  let movedOld = false;
  let installed = false;
  try {
    writePackage(rom, staging);
    verifyBuilt(rom, join(staging, "index.json"));
    if (existsSync(destination)) {
      renameSync(destination, backup);
      movedOld = true;
    }
    renameSync(staging, destination);
    installed = true;
    if (movedOld) rmSync(backup, { recursive: true, force: true });
  } catch (error) {
    if (movedOld && !installed && !existsSync(destination) && existsSync(backup)) renameSync(backup, destination);
    throw error;
  } finally {
    if (existsSync(staging)) rmSync(staging, { recursive: true, force: true });
    if (installed && existsSync(backup)) rmSync(backup, { recursive: true, force: true });
  }
}

export function verify_resource_d1_d3(romPath: string, indexPath: string): void {
  const rom = readFileSync(romPath);
  if (rom.length !== ROM_SIZE) throw new Error("canonical ROM must be exactly 8 MiB");
  const report = verifyBuilt(rom, indexPath);
  console.log(
    `identical=true resources=3 claimed_bytes=${report.claimedBytes} ` +
    `boundary_bytes=${report.boundaryBytes} suffix_fallback=${report.boundaryBytes - report.claimedBytes}`,
  );
}

function rejects(action: () => void): boolean {
  try {
    action();
    return false;
  } catch {
    return true;
  }
}

export function selfTest(): void {
  self_test_alignment_tail();
  if (SPECS.reduce((sum, spec) => sum + spec.sourceSize, 0) !== 4769 ||
      SPECS.reduce((sum, spec) => sum + spec.boundarySize, 0) !== D1_D3_BOUNDARY_SIZE ||
      D1.address + D1.boundarySize !== D2.address || D2.address + D2.boundarySize !== D3.address ||
      D3.address + D3.boundarySize !== D1_D3_END) {
    throw new Error("D1-D3 catalogue self-test failed");
  }
  const motion: MotionPlan = {
    format: 1,
    kind: "golden-sun-motion-path",
    resource_id: "0x0d3",
    address: hexadecimal(D3.address),
    size: hexadecimal(D3.sourceSize, 4),
    consumer: {
      function: "Func_080e89ec",
      initial_frame: 18,
      delta_first_frame: 19,
      delta_last_frame: 58,
      initial_encoding: "be-s16-pair",
      delta_encoding: "s8-pair",
      boundary_delta_pairs: 0,
    },
    initial: [-123, 456],
    deltas: Array.from({ length: 40 }, (_, index) => [index - 20, 20 - index] as [number, number]),
    boundary_deltas: [],
  };
  parseMotion(motion, D3);
  if (!rejects(() => parseMotion({ ...motion, deltas: motion.deltas.slice(1) }, D3)) ||
      !rejects(() => parseMotion({ ...motion, extra: 1 } as unknown, D3)) ||
      !rejects(() => validatePaletteTokens([["g", [["l", 1]]]] as unknown[], 1)) ||
      !rejects(() => validatePaletteTokens([["g", [["l"]]], ["g", [["e"]]]] as unknown[], 1)) ||
      !rejects(() => validatePaletteTokens([
        ["g", Array.from({ length: 8 }, () => ["l"])], ["g", [["e"]]],
      ] as unknown[], 8))) {
    throw new Error("D1-D3 adversarial source self-test failed");
  }
  const temporary = mkdtempSync(join(tmpdir(), "alchemy-d1-d3-test-"));
  try {
    const rom = join(temporary, "game.gba");
    writeFileSync(rom, Buffer.alloc(1));
    const wrong = join(temporary, "wrong");
    mkdirSync(wrong);
    writeFileSync(join(wrong, "note.txt"), "x");
    const canonical = join(temporary, "canonical");
    mkdirSync(canonical);
    writeFileSync(join(canonical, "index.json"), `${canonicalJson(expectedIndex())}\n`);
    const linkedRom = join(canonical, "game.gba");
    symlinkSync(rom, linkedRom);
    if (!rejects(() => validateExportDestination(rom, temporary)) ||
        !rejects(() => validateExportDestination(rom, rom)) ||
        !rejects(() => validateExportDestination(rom, wrong)) ||
        !rejects(() => validateExportDestination(linkedRom, canonical))) {
      throw new Error("unsafe D1-D3 export destination was accepted");
    }
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok");
}

function option(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : undefined;
}

function usage(): void {
  console.log(
    "usage: resource_d1_d3.ts export ROM --directory DIR | " +
    "verify ROM --index INDEX | --self-test",
  );
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    usage();
    return;
  }
  if (args.includes("--self-test")) {
    selfTest();
    if (args.length === 1) return;
    args = args.filter((argument) => argument !== "--self-test");
  }
  const command = args[0];
  const rom = args[1];
  if (command === "export" && rom !== undefined) {
    const directory = option(args, "--directory");
    if (directory === undefined) throw new Error("--directory is required");
    export_resource_d1_d3(rom, directory);
    verify_resource_d1_d3(rom, join(directory, "index.json"));
    return;
  }
  if (command === "verify" && rom !== undefined) {
    const index = option(args, "--index");
    if (index === undefined) throw new Error("--index is required");
    verify_resource_d1_d3(rom, index);
    return;
  }
  usage();
  throw new Error("unsupported D1-D3 command");
}

if (import.meta.main) main(Bun.argv.slice(2));
