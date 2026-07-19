#!/usr/bin/env bun
import { mkdirSync, readFileSync, realpathSync, writeFileSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
import { decode_palette_trace, encode_palette, type PaletteGroup } from "./extract_resource.ts";
import { palette_rgba_image, tile_png } from "./export_asset.ts";
import { gba_graphics, gba_palette_rgba, type Rgb } from "./import_asset.ts";

const ROM_BASE = 0x08000000;
const RESOURCE_TABLE = 0x08320000;

const FIXED = {
  2: { address: 0x08320fa0, size: 0x10 },
  0x13: { address: 0x08320fb0, size: 0x2000 },
  0x14: { address: 0x08322fb0, size: 0x8c0 },
  0x18: { address: 0x083337a0, size: 0x828, decodedSize: 0x9600 },
} as const;

interface StampPlan {
  format: 1;
  kind: "golden-sun-build-stamp";
  digits: 12;
  trailer: number[];
}

interface ScreenPlan {
  format: 1;
  codec: "golden-sun-tagged-palette-screen";
  resource_id: "18";
  address: string;
  container_size: string;
  decoded_size: string;
  palette_entries: 224;
  tag: 1;
  width: 240;
  height: 160;
  consumer: {
    function: "Func_080f2b70";
    palette_bytes: string;
    stream_offset: string;
    tile_depth: 8;
  };
  tokens: PaletteGroup[];
}

function number(value: string | number): number {
  return typeof value === "number" ? value : Number(value);
}

function json(path: string): any {
  return JSON.parse(readFileSync(path, "utf8"));
}

function same(left: string, right: string): boolean {
  try { return realpathSync(left) === realpathSync(right); }
  catch { return resolve(left) === resolve(right); }
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || args[index + 1] === undefined) throw new Error(`${name} is required`);
  return args[index + 1];
}

function pointer(rom: Buffer, id: number): number {
  const offset = RESOURCE_TABLE - ROM_BASE + id * 4;
  if (offset < 0 || offset + 4 > rom.length) throw new Error("resource pointer is outside the ROM");
  return rom.readUInt32LE(offset);
}

function checkedRange(rom: Buffer, id: keyof typeof FIXED): Buffer {
  const fixed = FIXED[id];
  if (pointer(rom, Number(id)) !== fixed.address) throw new Error(`resource ${Number(id).toString(16)} address differs`);
  const start = fixed.address - ROM_BASE;
  const data = rom.subarray(start, start + fixed.size);
  if (data.length !== fixed.size) throw new Error("resource range is outside the ROM");
  return data;
}

function bgr555(data: Uint8Array): Rgb[] {
  if (data.length % 2) throw new Error("palette has an odd byte count");
  const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
  return Array.from({ length: data.length / 2 }, (_, index) => {
    const value = view.getUint16(index * 2, true);
    if (value & 0x8000) throw new Error("palette contains a high-bit color");
    return [(value & 31) << 3, (value >>> 5 & 31) << 3, (value >>> 10 & 31) << 3];
  });
}

function fontPalette(rom: Buffer): Rgb[] {
  const start = 0x0800777c - ROM_BASE;
  const values = Buffer.from(rom.subarray(start, start + 32));
  [0x4180, 0x3960, 0x3140, 0x2920, 0x49a0, 0x51c0, 0x59e0]
    .forEach((value, index) => values.writeUInt16LE(value, (index + 4) * 2));
  return bgr555(values);
}

function stampPaths(root: string): [string, string] {
  const directory = join(root, "data/resource_2");
  return [join(directory, "build_stamp.txt"), join(directory, "layout.json")];
}

export function build_resource_2(root: string): Buffer {
  const [source, planPath] = stampPaths(root);
  const stamp = readFileSync(source, "utf8").trimEnd();
  const plan = json(planPath) as StampPlan;
  if (plan.format !== 1 || plan.kind !== "golden-sun-build-stamp" || plan.digits !== 12 || !/^\d{12}$/.test(stamp))
    throw new Error("invalid build-stamp source");
  if (!Array.isArray(plan.trailer) || plan.trailer.length !== 4 ||
      plan.trailer.some((value) => !Number.isInteger(value) || value < 0 || value > 255))
    throw new Error("invalid build-stamp trailer");
  return Buffer.concat([Buffer.from(stamp, "ascii"), Buffer.from(plan.trailer)]);
}

export function build_resource_13(root: string): Buffer {
  const [tiles, , report] = gba_graphics(readFileSync(join(root, "graphics/resource_13/font.4bpp.png")), 4);
  if (report.width !== 128 || report.height !== 128 || tiles.length !== FIXED[0x13].size)
    throw new Error("font source must be a 16 by 16 grid of 8 by 8 glyphs");
  return tiles;
}

export function build_resource_14(root: string): Buffer {
  const [words, report] = gba_palette_rgba(readFileSync(join(root, "graphics/resource_14/words.rgba.png")));
  if (report.width !== 16 || report.height !== 70 || words.length !== FIXED[0x14].size)
    throw new Error("resource 14 word image must contain 1120 BGR555-compatible values");
  return words;
}

export function build_resource_18_full(root: string): Buffer {
  const directory = join(root, "graphics/resource_18");
  const plan = json(join(directory, "screen.lz.json")) as ScreenPlan;
  if (plan.format !== 1 || plan.codec !== "golden-sun-tagged-palette-screen" ||
      plan.resource_id !== "18" || number(plan.address) !== FIXED[0x18].address ||
      number(plan.container_size) !== FIXED[0x18].size || number(plan.decoded_size) !== FIXED[0x18].decodedSize ||
      plan.palette_entries !== 224 || plan.tag !== 1 || plan.width !== 240 || plan.height !== 160 ||
      plan.consumer.function !== "Func_080f2b70" || number(plan.consumer.palette_bytes) !== 0x1c0 ||
      number(plan.consumer.stream_offset) !== 0x1c0 || plan.consumer.tile_depth !== 8)
    throw new Error("resource 18 screen plan differs from its consumer layout");
  const [tiles, palette, report] = gba_graphics(readFileSync(join(directory, "screen.8bpp.png")), 8);
  if (report.width !== 240 || report.height !== 160 || tiles.length !== FIXED[0x18].decodedSize || palette.length !== 0x1c0)
    throw new Error("resource 18 source must be a 240 by 160 screen with 224 colors");
  const built = Buffer.concat([palette, Buffer.from([1]), encode_palette(tiles, plan.tokens)]);
  if (built.length !== FIXED[0x18].size) throw new Error("resource 18 encoded size differs");
  return built;
}

export function build_resource_18(root: string): Buffer {
  return build_resource_18_full(root);
}

export function build_simple_resource(id: 2 | 0x13 | 0x14 | 0x18, root: string): Buffer {
  if (id === 2) return build_resource_2(root);
  if (id === 0x13) return build_resource_13(root);
  if (id === 0x14) return build_resource_14(root);
  return build_resource_18(root);
}

export function export_simple_resources(romPath: string, root: string): void {
  const rom = readFileSync(romPath);
  const stamp = checkedRange(rom, 2);
  const [stampSource, stampPlan] = stampPaths(root);
  mkdirSync(dirname(stampSource), { recursive: true });
  writeFileSync(stampSource, `${stamp.subarray(0, 12).toString("ascii")}\n`);
  writeFileSync(stampPlan, `${JSON.stringify({
    format: 1, kind: "golden-sun-build-stamp", digits: 12, trailer: [...stamp.subarray(12)],
  })}\n`);
  if (!build_resource_2(root).equals(stamp)) throw new Error("resource 2 round trip differs");

  const font = checkedRange(rom, 0x13);
  const fontPath = join(root, "graphics/resource_13/font.4bpp.png");
  mkdirSync(dirname(fontPath), { recursive: true });
  writeFileSync(fontPath, tile_png(font, 4, 16, fontPalette(rom))[0]);
  if (!build_resource_13(root).equals(font)) throw new Error("resource 13 round trip differs");

  const words = checkedRange(rom, 0x14);
  const wordsPath = join(root, "graphics/resource_14/words.rgba.png");
  mkdirSync(dirname(wordsPath), { recursive: true });
  writeFileSync(wordsPath, palette_rgba_image(words, 16)[0]);
  if (!build_resource_14(root).equals(words)) throw new Error("resource 14 round trip differs");

  const screen = checkedRange(rom, 0x18);
  const start = FIXED[0x18].address - ROM_BASE;
  const palette = screen.subarray(0, 0x1c0);
  if (screen[0x1c0] !== 1) throw new Error("resource 18 lacks its tag-1 screen stream");
  const [decoded, cursor, tokens] = decode_palette_trace(rom, start + 0x1c1, start + screen.length, 0x9600);
  if (decoded.length !== FIXED[0x18].decodedSize || cursor !== start + screen.length)
    throw new Error("resource 18 screen stream differs from its consumer bounds");
  const screenPath = join(root, "graphics/resource_18/screen.8bpp.png");
  const streamPath = join(root, "graphics/resource_18/screen.lz.json");
  mkdirSync(dirname(screenPath), { recursive: true });
  writeFileSync(screenPath, tile_png(decoded, 8, 30, bgr555(palette))[0]);
  writeFileSync(streamPath, `${JSON.stringify({
    format: 1,
    codec: "golden-sun-tagged-palette-screen",
    resource_id: "18",
    address: "0x083337a0",
    container_size: "0x828",
    decoded_size: "0x9600",
    palette_entries: 224,
    tag: 1,
    width: 240,
    height: 160,
    consumer: {
      function: "Func_080f2b70", palette_bytes: "0x1c0", stream_offset: "0x1c0", tile_depth: 8,
    },
    tokens,
  })}\n`);
  if (!build_resource_18(root).equals(screen)) throw new Error("resource 18 screen round trip differs");
  console.log("resources=4 unique_bytes=0x30f8 screen_18=240x160 decoded_18=0x9600");
}

export function verify_simple_resources(romPath: string, root: string): void {
  const rom = readFileSync(romPath);
  for (const id of [2, 0x13, 0x14] as const) {
    if (!build_simple_resource(id, root).equals(checkedRange(rom, id)))
      throw new Error(`resource ${id.toString(16)} verification differs`);
  }
  if (!build_resource_18(root).equals(checkedRange(rom, 0x18))) throw new Error("resource 18 verification differs");
  console.log("identical=true resources=4 unique_bytes=0x30f8");
}

export function self_test(): void {
  const digits = "012345678901";
  if (!/^\d{12}$/.test(digits) || /^\d{12}$/.test(`${digits}2`)) throw new Error("build-stamp validation self-test failed");
  const decoded = Buffer.from([1, 2, 1, 2]);
  const tokens: PaletteGroup[] = [["g", [["l"], ["l"], ["c", 2, 2], ["e"]]]];
  const encoded = encode_palette(decoded, tokens);
  if (!decode_palette_trace(encoded, 0, encoded.length, decoded.length)[0].equals(decoded))
    throw new Error("tagged screen codec self-test failed");
  console.log("self-test=ok");
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: simple_resources.ts [--self-test] {export|verify} ROM --directory ASSETS");
    return;
  }
  if (args.includes("--self-test")) {
    self_test();
    args = args.filter((argument) => argument !== "--self-test");
    if (!args.length) return;
  }
  if (!["export", "verify"].includes(args[0]) || !args[1]) throw new Error("export or verify and a ROM are required");
  const root = option(args, "--directory");
  if (same(args[1], root)) throw new Error("refusing to overwrite the input ROM");
  if (args[0] === "export") export_simple_resources(args[1], root);
  else verify_simple_resources(args[1], root);
}

if (import.meta.main) main(Bun.argv.slice(2));
