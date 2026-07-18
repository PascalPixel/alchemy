import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { deflateSync } from "./zlib.ts";
import { chunk } from "./export_asset.ts";
import { prune_files } from "./generated_files.ts";
import { indexed_png } from "./import_asset.ts";

export const ROM_BASE = 0x08000000;
export const PREFIX: Record<number, string> = {
  1: "00", 2: "010", 3: "011", 4: "100", 5: "101", 6: "110",
  7: "11100", 8: "11101", 9: "11110", 10: "1111100", 11: "1111101",
  12: "1111110", 13: "111111100", 14: "111111101", 15: "111111110",
};

export interface F0Plan {
  format?: number;
  codec?: string;
  images: number;
  entries: Array<number | null>;
}

export class Bits {
  data: Uint8Array;
  offset = 0;

  constructor(data: Uint8Array) { this.data = data; }

  get(): number {
    if (this.offset >= this.data.length * 8)
      throw new Error("F0 image stream ended before its terminator");
    return (this.data[this.offset >>> 3] >>> (this.offset++ & 7)) & 1;
  }
}

export function selection(bits: Bits): number | null {
  if (bits.get() === 0) return bits.get() === 0 ? 1 : (bits.get() === 0 ? 2 : 3);
  if (bits.get() === 0) return bits.get() === 0 ? 4 : 5;
  if (bits.get() === 0) return 6;
  if (bits.get() === 0) return bits.get() === 0 ? 7 : 8;
  if (bits.get() === 0) return 9;
  if (bits.get() === 0) return bits.get() === 0 ? 10 : 11;
  if (bits.get() === 0) return 12;
  if (bits.get() === 0) return bits.get() === 0 ? 13 : 14;
  if (bits.get() === 0) return 15;
  return null;
}

export function decode_pixels(data: Uint8Array): [Buffer, number] {
  const bits = new Bits(data);
  const moveToFront = Array.from({ length: 16 }, (_, index) => index);
  const pixels: number[] = [];
  while (true) {
    while (bits.get() === 0) pixels.push(moveToFront[0]);
    const index = selection(bits);
    if (index === null) return [Buffer.from(pixels), bits.offset];
    const [value] = moveToFront.splice(index, 1);
    moveToFront.unshift(value);
    pixels.push(value);
  }
}

export function encode_pixels(pixels: Uint8Array): Buffer {
  const moveToFront = Array.from({ length: 16 }, (_, index) => index);
  const bits: number[] = [];
  for (const value of pixels) {
    if (value < 0 || value >= 16) throw new Error("F0 image contains a non-4bpp pixel");
    if (value === moveToFront[0]) { bits.push(0); continue; }
    const index = moveToFront.indexOf(value);
    bits.push(1, ...Array.from(PREFIX[index], Number));
    const [selected] = moveToFront.splice(index, 1);
    moveToFront.unshift(selected);
  }
  bits.push(...Array(10).fill(1));
  bits.push(...Array((8 - bits.length % 8) % 8).fill(1));
  const encoded = Buffer.alloc(bits.length / 8);
  bits.forEach((value, index) => { encoded[index >>> 3] |= value << (index & 7); });
  return encoded;
}

function pngHeader(width: number, height: number, depth: number, colorType: number): Buffer {
  const header = Buffer.alloc(13);
  header.writeUInt32BE(width, 0); header.writeUInt32BE(height, 4);
  header.set([depth, colorType, 0, 0, 0], 8);
  return header;
}

export function image_png(pixels: Uint8Array, palette: Uint8Array): Buffer {
  if (pixels.length !== 32 * 32 || palette.length !== 32)
    throw new Error("F0 images must be 32x32 with 16 colors");
  const colors = Buffer.alloc(48);
  for (let index = 0; index < 16; index++) {
    const value = palette[index * 2] | (palette[index * 2 + 1] << 8);
    if (value & 0x8000) throw new Error("F0 palette uses an unsupported high bit");
    colors[index * 3] = (value & 31) << 3;
    colors[index * 3 + 1] = ((value >>> 5) & 31) << 3;
    colors[index * 3 + 2] = ((value >>> 10) & 31) << 3;
  }
  const rows = Buffer.alloc(32 * 17);
  let cursor = 0;
  for (let y = 0; y < 32; y++) {
    rows[cursor++] = 0;
    for (let x = 0; x < 32; x += 2)
      rows[cursor++] = (pixels[y * 32 + x] << 4) | pixels[y * 32 + x + 1];
  }
  return Buffer.concat([
    Buffer.from("89504e470d0a1a0a", "hex"), chunk(Buffer.from("IHDR"), pngHeader(32, 32, 4, 3)),
    chunk(Buffer.from("PLTE"), colors), chunk(Buffer.from("IDAT"), deflateSync(rows, { level: 9 })),
    chunk(Buffer.from("IEND"), Buffer.alloc(0)),
  ]);
}

export function read_image(data: Uint8Array): [Buffer, Buffer] {
  const [width, height, pixels, colors] = indexed_png(data);
  if (width !== 32 || height !== 32 || colors.length !== 16)
    throw new Error("F0 source PNG must be 32x32 with 16 colors");
  const palette = Buffer.alloc(32);
  colors.forEach(([red, green, blue], index) => {
    if ((red & 7) || (green & 7) || (blue & 7))
      throw new Error("F0 palette channels must be multiples of eight");
    palette.writeUInt16LE((red >>> 3) | ((green >>> 3) << 5) | ((blue >>> 3) << 10), index * 2);
  });
  return [Buffer.from(pixels), palette];
}

export function preview_png(directory: string, count: number, columns = 8, scale = 2): Buffer {
  const rows = Math.ceil(count / columns);
  const width = columns * 32 * scale;
  const height = rows * 32 * scale;
  const output = Buffer.alloc(width * height * 3, 255);
  for (let index = 0; index < count; index++) {
    const [pixels, palette] = read_image(readFileSync(join(directory, `image_${index.toString().padStart(2, "0")}.png`)));
    const colors: number[][] = [];
    for (let colorIndex = 0; colorIndex < 16; colorIndex++) {
      const value = palette.readUInt16LE(colorIndex * 2);
      colors.push([value & 31, (value >>> 5) & 31, (value >>> 10) & 31].map((channel) => (channel << 3) | (channel >>> 2)));
    }
    const left = (index % columns) * 32 * scale;
    const top = Math.floor(index / columns) * 32 * scale;
    for (let y = 0; y < 32; y++) for (let x = 0; x < 32; x++) {
      const color = colors[pixels[y * 32 + x]];
      for (let sy = 0; sy < scale; sy++) for (let sx = 0; sx < scale; sx++) {
        const target = (((top + y * scale + sy) * width + left + x * scale + sx) * 3);
        output.set(color, target);
      }
    }
  }
  const scanlines = Buffer.alloc(height * (1 + width * 3));
  for (let y = 0; y < height; y++) output.copy(scanlines, y * (1 + width * 3) + 1, y * width * 3, (y + 1) * width * 3);
  return Buffer.concat([
    Buffer.from("89504e470d0a1a0a", "hex"), chunk(Buffer.from("IHDR"), pngHeader(width, height, 8, 2)),
    chunk(Buffer.from("IDAT"), deflateSync(scanlines, { level: 9 })), chunk(Buffer.from("IEND"), Buffer.alloc(0)),
  ]);
}

export function package_image(image: Uint8Array): Buffer {
  const [pixels, palette] = read_image(image);
  const data = Buffer.concat([palette, encode_pixels(pixels)]);
  return Buffer.concat([data, Buffer.alloc((4 - data.length % 4) % 4)]);
}

export function build_archive(plan: F0Plan, directory: string): Buffer {
  const count = Number(plan.images);
  const packages = Array.from({ length: count }, (_, index) =>
    package_image(readFileSync(join(directory, `image_${index.toString().padStart(2, "0")}.png`))));
  let offset = plan.entries.length * 2;
  const offsets: number[] = [];
  for (const payload of packages) { offsets.push(offset); offset += payload.length; }
  const table = plan.entries.map((image) => image === null ? 0 : offsets[Number(image)]);
  if (table.some((value) => value >= 0x10000)) throw new Error("F0 archive offset exceeds 16 bits");
  const header = Buffer.alloc(table.length * 2);
  table.forEach((value, index) => header.writeUInt16LE(value, index * 2));
  return Buffer.concat([header, ...packages]);
}

function value(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : undefined;
}

function positional(args: string[]): string | undefined {
  const takesValue = new Set(["--address", "--input-end", "--plan", "--images", "--preview"]);
  for (let index = 0; index < args.length; index++) {
    if (takesValue.has(args[index])) { index++; continue; }
    if (!args[index].startsWith("-")) return args[index];
  }
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: f0_archive.ts ROM --address N --input-end N --plan FILE --images DIR [--preview FILE]");
    return;
  }
  const romPath = positional(args);
  const addressText = value(args, "--address");
  const endText = value(args, "--input-end");
  const planPath = value(args, "--plan");
  const images = value(args, "--images");
  const preview = value(args, "--preview");
  if (!romPath || !addressText || !endText || !planPath || !images)
    throw new Error("rom, --address, --input-end, --plan, and --images are required");
  const rom = readFileSync(romPath);
  const archive = rom.subarray(Number(addressText) - ROM_BASE, Number(endText) - ROM_BASE);
  const first = archive.readUInt16LE(0);
  if (!first || first % 2 || first > archive.length) throw new Error("F0 archive has no valid offset table");
  const entries = Array.from({ length: first / 2 }, (_, index) => archive.readUInt16LE(index * 2));
  const unique = Array.from(new Set(entries.filter(Boolean))).sort((a, b) => a - b);
  if (!unique.length || unique[0] !== first) throw new Error("F0 first image does not follow its offset table");
  mkdirSync(images, { recursive: true });
  const indexByOffset = new Map(unique.map((offset, index) => [offset, index]));
  unique.forEach((offset, index) => {
    const bound = index + 1 < unique.length ? unique[index + 1] : archive.length;
    const palette = archive.subarray(offset, offset + 32);
    const [pixels] = decode_pixels(archive.subarray(offset + 32, bound));
    if (pixels.length !== 32 * 32) throw new Error("F0 image does not decode to 32x32");
    const data = Buffer.concat([palette, encode_pixels(pixels)]);
    const rebuilt = Buffer.concat([data, Buffer.alloc((4 - data.length % 4) % 4)]);
    if (!rebuilt.equals(archive.subarray(offset, bound))) throw new Error("F0 image package does not re-encode exactly");
    writeFileSync(join(images, `image_${index.toString().padStart(2, "0")}.png`), image_png(pixels, palette));
  });
  const plan: F0Plan = {
    format: 1, codec: "golden-sun-mtf4-archive", images: unique.length,
    entries: entries.map((offset) => offset === 0 ? null : indexByOffset.get(offset)!),
  };
  if (!build_archive(plan, images).equals(archive)) throw new Error("rebuilt F0 archive differs");
  prune_files(images, "image_*.png", Array.from({ length: unique.length }, (_, index) => `image_${index.toString().padStart(2, "0")}.png`));
  mkdirSync(dirname(planPath), { recursive: true });
  writeFileSync(planPath, `${JSON.stringify(plan)}\n`);
  if (preview) { mkdirSync(dirname(preview), { recursive: true }); writeFileSync(preview, preview_png(images, unique.length)); }
  console.log(`entries=${entries.length} images=${unique.length} encoded=${archive.length}`);
}

if (import.meta.main) main(process.argv.slice(2));
