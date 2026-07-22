// Tool role: both; imported by tools/build_assets.ts, tools/chiiki_map_resources.ts, tools/tokushu_map_resources.ts; invoked by package.json.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { join } from "node:path";
import { deflateSync } from "./zlib.ts";
import { byte_png, chunk } from "./export_asset.ts";
import { decode_palette_trace, encode_palette, type PaletteGroup } from "./extract_resource.ts";
import { indexed_png } from "./import_asset.ts";

export const ROM_BASE = 0x08000000;
export const PLANE = 0x4000;
export const FILES = ["value_low.png", "value_high.png", "attribute_a.png", "attribute_b.png"] as const;

interface Kind1Plan {
  format: number;
  codec: string;
  decoded_size?: number;
  encoded_size?: number;
  tokens: PaletteGroup[];
  lookahead?: string;
}

export function mask_png(mask: Uint8Array): Buffer {
  if (mask.length !== PLANE || mask.some((value) => value !== 0 && value !== 1))
    throw new Error("sentinel mask must contain 128x128 binary pixels");
  const rows = Buffer.alloc(128 * 17);
  let cursor = 0;
  for (let y = 0; y < 128; y++) {
    rows[cursor++] = 0;
    for (let x = 0; x < 128; x += 8) {
      let value = 0;
      for (let bit = 0; bit < 8; bit++) value |= mask[y * 128 + x + bit] << (7 - bit);
      rows[cursor++] = value;
    }
  }
  const header = Buffer.alloc(13);
  header.writeUInt32BE(128, 0); header.writeUInt32BE(128, 4);
  header.set([1, 3, 0, 0, 0], 8);
  return Buffer.concat([
    Buffer.from("89504e470d0a1a0a", "hex"), chunk(Buffer.from("IHDR"), header),
    chunk(Buffer.from("PLTE"), Buffer.from("000000ffffff", "hex")),
    chunk(Buffer.from("IDAT"), deflateSync(rows, { level: 9 })), chunk(Buffer.from("IEND"), Buffer.alloc(0)),
  ]);
}

export function transform(decoded: Uint8Array): [[Buffer, Buffer, Buffer, Buffer], Buffer] {
  if (decoded.length !== 4 * PLANE)
    throw new Error("kind-1 map input must contain three 128x128 planes");
  const planeA = decoded.subarray(0, PLANE);
  const planeB = decoded.subarray(PLANE, 2 * PLANE);
  const values = new DataView(decoded.buffer, decoded.byteOffset + 2 * PLANE, decoded.byteLength - 2 * PLANE);
  const planes = [Buffer.alloc(PLANE), Buffer.alloc(PLANE), Buffer.alloc(PLANE), Buffer.alloc(PLANE)] as [Buffer, Buffer, Buffer, Buffer];
  const mask = Buffer.alloc(PLANE);
  let ordinal = 0;
  for (let index = 0; index < PLANE; index++) {
    let value = values.getUint16(index * 2, true);
    if ((value & 0xfff) === 0xfff) {
      mask[index] = 1;
      if (ordinal < 0xfff) value = (value & 0xf000) | ordinal++;
    }
    planes[0][index] = value & 0xff;
    planes[1][index] = value >>> 8;
    planes[2][index] = planeA[index];
    planes[3][index] = planeB[index];
  }
  return [planes, mask];
}

export function inverse(planes: readonly ArrayLike<number>[], mask: ArrayLike<number>): Buffer {
  if (planes.length !== 4 || planes.some((plane) => plane.length !== PLANE))
    throw new Error("map source must contain four 128x128 byte planes");
  if (mask.length !== PLANE || Array.from(mask).some((value) => value !== 0 && value !== 1))
    throw new Error("map sentinel mask is invalid");
  const values = Buffer.alloc(2 * PLANE);
  let ordinal = 0;
  for (let index = 0; index < PLANE; index++) {
    let value = planes[0][index] | (planes[1][index] << 8);
    if (mask[index]) {
      const expected = ordinal < 0xfff ? (value & 0xf000) | ordinal : value;
      if (value !== expected) throw new Error("sentinel mask disagrees with transformed value");
      value = (value & 0xf000) | 0xfff;
      if (ordinal < 0xfff) ordinal++;
    } else if ((value & 0xfff) === 0xfff)
      throw new Error("unmasked source value collides with sentinel form");
    values.writeUInt16LE(value, index * 2);
  }
  return Buffer.concat([Buffer.from(planes[2]), Buffer.from(planes[3]), values]);
}

export function read_plane(path: string): Buffer {
  const [width, height, pixels] = indexed_png(readFileSync(path));
  if (width !== 128 || height !== 128 || pixels.length !== PLANE)
    throw new Error(`${path}: expected one 128x128 byte plane`);
  return Buffer.from(pixels);
}

export function build_grid(plan: Kind1Plan, directory: string): Buffer {
  if (plan.format !== 1 || plan.codec !== "golden-sun-kind1-grid")
    throw new Error("unsupported kind-1 grid plan");
  const planes = FILES.map((name) => read_plane(join(directory, name)));
  const [, , mask] = indexed_png(readFileSync(join(directory, "sentinels.png")));
  const decoded = inverse(planes, mask);
  const encoded = Buffer.concat([Buffer.from([1]), encode_palette(decoded, plan.tokens)]);
  return Buffer.concat([encoded, Buffer.from(plan.lookahead ?? "", "hex")]);
}

export function export_grid(data: Uint8Array, directory: string): [number, number] {
  if (!data.length || data[0] !== 1) throw new Error("kind-1 map grid must begin with byte 1");
  const [decoded, , tokens] = decode_palette_trace(data, 1, data.length, 0x10000);
  const [planes, mask] = transform(decoded);
  mkdirSync(directory, { recursive: true });
  FILES.forEach((name, index) => writeFileSync(join(directory, name), byte_png(planes[index], 128)[0]));
  writeFileSync(join(directory, "sentinels.png"), mask_png(mask));
  const encoded = Buffer.concat([Buffer.from([1]), encode_palette(decoded, tokens)]);
  if (!Buffer.from(data.subarray(0, encoded.length)).equals(encoded))
    throw new Error("kind-1 token replay differs");
  const plan: Kind1Plan = {
    format: 1, codec: "golden-sun-kind1-grid", decoded_size: decoded.length,
    encoded_size: data.length, tokens, lookahead: Buffer.from(data.subarray(encoded.length)).toString("hex"),
  };
  writeFileSync(join(directory, "grid.kind1.json"), `${JSON.stringify(plan)}\n`);
  if (!build_grid(plan, directory).equals(Buffer.from(data)))
    throw new Error("exported kind-1 grid does not round-trip");
  return [tokens.length, mask.reduce((sum, value) => sum + value, 0)];
}

export function self_test(): void {
  const decoded = Buffer.alloc(4 * PLANE);
  [0x0fff, 0x2fff, 0x1234, 0x4fff].forEach((value, index) => decoded.writeUInt16LE(value, 2 * PLANE + index * 2));
  const [planes, mask] = transform(decoded);
  if (!inverse(planes, mask).equals(decoded) || mask.reduce((sum, value) => sum + value, 0) !== 3)
    throw new Error("kind-1 map transform self-test failed");
  console.log("self-test=ok");
}

function value(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : undefined;
}

function positional(args: string[]): string | undefined {
  const takesValue = new Set(["--address", "--size", "--directory"]);
  for (let index = 0; index < args.length; index++) {
    if (takesValue.has(args[index])) { index++; continue; }
    if (!args[index].startsWith("-")) return args[index];
  }
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: kind1_map_grid.ts [--self-test] [ROM --address N --size N --directory DIR]");
    return;
  }
  if (args.includes("--self-test")) {
    self_test();
    if (!positional(args.filter((item) => item !== "--self-test"))) return;
  }
  const romPath = positional(args);
  const addressText = value(args, "--address");
  const sizeText = value(args, "--size");
  const directory = value(args, "--directory");
  if (!romPath || !addressText || !sizeText || !directory)
    throw new Error("rom, --address, --size, and --directory are required");
  const rom = readFileSync(romPath);
  const address = Number(addressText);
  const size = Number(sizeText);
  const start = address - ROM_BASE;
  const data = rom.subarray(start, start + size);
  if (data.length !== size) throw new Error("kind-1 grid range is outside the ROM");
  const [tokens, sentinels] = export_grid(data, directory);
  console.log(`tokens=${tokens} sentinels=${sentinels} encoded=0x${data.length.toString(16)}`);
}

if (import.meta.main) main(process.argv.slice(2));
