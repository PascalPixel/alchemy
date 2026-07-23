// Tool role: both; imported by tools/build_assets.ts, tools/early_runtime_data.ts, tools/sentou_gamen_data.ts (+4 more); invoked by package.json.
import { mkdirSync, readFileSync, statSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { deflateSync } from "./zlib.ts";
import { chunk } from "./export_asset.ts";
import { prune_files } from "./generated_files.ts";
import { indexed_png } from "./import_asset.ts";

export interface SpritePlan {
  format: number;
  codec: string;
  width: number;
  height: number;
  images: number;
  palette_offset: number;
  palette_entries: number;
  stream_order: number[];
}

export type Rgb = [number, number, number];

export function decode_stream(data: Uint8Array, offset: number, pixels: number): [Buffer, number] {
  const output = Buffer.alloc(pixels);
  let cursor = offset;
  let position = 0;
  while (cursor < data.length) {
    const value = data[cursor++];
    if (value === 0) {
      if (position !== pixels) throw new Error("sprite stream ended before its image boundary");
      return [output, cursor];
    }
    if (value <= 0xdf) {
      if (position >= pixels) throw new Error("sprite literal crossed its image boundary");
      output[position++] = value;
    } else {
      position += value - 0xdf;
      if (position > pixels) throw new Error("sprite skip crossed its image boundary");
    }
  }
  throw new Error("sprite stream has no terminator");
}

export function encode_stream(pixels: Uint8Array): Buffer {
  const output: number[] = [];
  let cursor = 0;
  while (cursor < pixels.length) {
    const value = pixels[cursor];
    if (value) {
      if (value > 0xdf) throw new Error("sprite pixel exceeds the literal range");
      output.push(value); cursor++; continue;
    }
    let end = cursor + 1;
    while (end < pixels.length && pixels[end] === 0) end++;
    let remaining = end - cursor;
    while (remaining) {
      const count = Math.min(remaining, 32);
      output.push(0xdf + count); remaining -= count;
    }
    cursor = end;
  }
  output.push(0);
  return Buffer.from(output);
}

function pngHeader(width: number, height: number, depth: number, colorType: number): Buffer {
  const header = Buffer.alloc(13);
  header.writeUInt32BE(width, 0); header.writeUInt32BE(height, 4);
  header.set([depth, colorType, 0, 0, 0], 8);
  return header;
}

export function png(pixels: Uint8Array, width: number, height: number, palette: Rgb[]): Buffer {
  if (pixels.length !== width * height || palette.length < 1 || palette.length > 256)
    throw new Error("invalid indexed sprite image");
  const colors = Buffer.from(palette.flat());
  const rows = Buffer.alloc(height * (width + 1));
  for (let y = 0; y < height; y++) Buffer.from(pixels).copy(rows, y * (width + 1) + 1, y * width, (y + 1) * width);
  return Buffer.concat([
    Buffer.from("89504e470d0a1a0a", "hex"), chunk(Buffer.from("IHDR"), pngHeader(width, height, 8, 3)),
    chunk(Buffer.from("PLTE"), colors), chunk(Buffer.from("tRNS"), Buffer.concat([Buffer.from([0]), Buffer.alloc(palette.length - 1, 0xff)])),
    chunk(Buffer.from("IDAT"), deflateSync(rows, { level: 9 })), chunk(Buffer.from("IEND"), Buffer.alloc(0)),
  ]);
}

export function read_palette(path: string, offset: number, entries: number): Rgb[] {
  const [, , pixels, colors] = indexed_png(readFileSync(path));
  if (offset + entries > pixels.length) throw new Error("shared palette PNG is too small");
  return Array.from({ length: entries }, (_, index) => [...colors[pixels[offset + index]]] as Rgb);
}

export function read_image(path: string, width: number, height: number, palette: Rgb[]): Buffer {
  const [gotWidth, gotHeight, pixels, colors] = indexed_png(readFileSync(path));
  if (gotWidth !== width || gotHeight !== height) throw new Error(`${path}: expected ${width}x${height}`);
  if (JSON.stringify(colors) !== JSON.stringify(palette))
    throw new Error(`${path}: palette differs from the proven shared palette`);
  if (pixels.some((value) => value > 0xdf)) throw new Error(`${path}: pixel exceeds the archive literal range`);
  return Buffer.from(pixels);
}

export function build_archive(plan: SpritePlan, directory: string, palettePath: string): Buffer {
  if (plan.format !== 1 || plan.codec !== "zero-skip-sprite-archive")
    throw new Error("unsupported zero-skip sprite plan");
  const width = Number(plan.width), height = Number(plan.height), count = Number(plan.images);
  const order = plan.stream_order.map(Number);
  if (JSON.stringify([...order].sort((a, b) => a - b)) !== JSON.stringify(Array.from({ length: count }, (_, index) => index)))
    throw new Error("sprite stream order is not a permutation");
  const palette = read_palette(palettePath, Number(plan.palette_offset), Number(plan.palette_entries));
  const images = Array.from({ length: count }, (_, index) =>
    read_image(`${directory}_images_frame_${index.toString().padStart(2, "0")}.png`, width, height, palette));
  const streams = images.map(encode_stream);
  const headerSize = (count + 1) * 4;
  const offsets = Array<number>(count);
  let cursor = headerSize;
  for (const index of order) { offsets[index] = cursor; cursor += streams[index].length; }
  const header = Buffer.alloc(headerSize);
  offsets.forEach((offset, index) => header.writeUInt32LE(offset, index * 4));
  return Buffer.concat([header, ...order.map((index) => streams[index])]);
}

export function preview(images: Uint8Array[], width: number, height: number, palette: Rgb[], columns = 4, scale = 2): Buffer {
  const rows = Math.ceil(images.length / columns);
  const outWidth = columns * width * scale;
  const outHeight = rows * height * scale;
  const output = Buffer.alloc(outWidth * outHeight * 3, Buffer.from([32, 32, 32]));
  images.forEach((pixels, index) => {
    const left = (index % columns) * width * scale;
    const top = Math.floor(index / columns) * height * scale;
    for (let y = 0; y < height; y++) for (let x = 0; x < width; x++) {
      const value = pixels[y * width + x];
      const color = value ? palette[value] : [32, 32, 32];
      for (let sy = 0; sy < scale; sy++) for (let sx = 0; sx < scale; sx++)
        output.set(color, ((top + y * scale + sy) * outWidth + left + x * scale + sx) * 3);
    }
  });
  const scanlines = Buffer.alloc(outHeight * (outWidth * 3 + 1));
  for (let y = 0; y < outHeight; y++) output.copy(scanlines, y * (outWidth * 3 + 1) + 1, y * outWidth * 3, (y + 1) * outWidth * 3);
  return Buffer.concat([
    Buffer.from("89504e470d0a1a0a", "hex"), chunk(Buffer.from("IHDR"), pngHeader(outWidth, outHeight, 8, 2)),
    chunk(Buffer.from("IDAT"), deflateSync(scanlines, { level: 9 })), chunk(Buffer.from("IEND"), Buffer.alloc(0)),
  ]);
}

export function export_archive(
  decoded: Uint8Array, directory: string, planPath: string, palettePath: string,
  previewPath?: string, width = 64, height = 64,
): number {
  const entries: number[] = [];
  const view = new DataView(decoded.buffer, decoded.byteOffset, decoded.byteLength);
  for (let offset = 0; offset < Math.min(decoded.length, 256 * 4); offset += 4) {
    const value = view.getUint32(offset, true); entries.push(value); if (value === 0) break;
  }
  if (!entries.length || entries.at(-1) !== 0) throw new Error("archive pointer table lacks its null terminator");
  const offsets = entries.slice(0, -1);
  const first = entries.length * 4;
  if (!offsets.length || Math.min(...offsets) !== first)
    throw new Error("archive streams do not follow its pointer table");
  if (new Set(offsets).size !== offsets.length || offsets.some((value) => value < first || value >= decoded.length))
    throw new Error("archive contains an invalid sprite pointer");
  const images: Buffer[] = [];
  const ends = new Map<number, number>();
  for (const offset of offsets) {
    const [image, end] = decode_stream(decoded, offset, width * height);
    images.push(image); ends.set(offset, end);
  }
  const order = Array.from({ length: offsets.length }, (_, index) => index).sort((a, b) => offsets[a] - offsets[b]);
  order.forEach((index, position) => {
    const expected = position + 1 < order.length ? offsets[order[position + 1]] : decoded.length;
    if (ends.get(offsets[index]) !== expected) throw new Error("sprite streams are not contiguous");
  });
  const paletteOffset = 16, paletteEntries = 224;
  const palette = read_palette(palettePath, paletteOffset, paletteEntries);
  mkdirSync(directory, { recursive: true });
  images.forEach((image, index) => writeFileSync(`${directory}_images_frame_${index.toString().padStart(2, "0")}.png`, png(image, width, height, palette)));
  const plan: SpritePlan = {
    format: 1, codec: "zero-skip-sprite-archive", width, height, images: images.length,
    palette_offset: paletteOffset, palette_entries: paletteEntries, stream_order: order,
  };
  mkdirSync(dirname(planPath), { recursive: true });
  writeFileSync(planPath, `${JSON.stringify(plan)}\n`);
  if (!build_archive(plan, directory, palettePath).equals(Buffer.from(decoded)))
    throw new Error("exported sprite archive does not round-trip");
  prune_files(directory, "images_frame_*.png", Array.from({ length: images.length }, (_, index) => `images_frame_${index.toString().padStart(2, "0")}.png`));
  if (previewPath) { mkdirSync(dirname(previewPath), { recursive: true }); writeFileSync(previewPath, preview(images, width, height, palette)); }
  return images.length;
}

export function self_test(): void {
  const unit = [...Array(65).fill(0), 1, 2, 3, ...Array(31).fill(0)];
  const source = Buffer.from(Array(8).fill(unit).flat());
  const encoded = encode_stream(source);
  const [decoded, end] = decode_stream(encoded, 0, source.length);
  if (!decoded.equals(source) || end !== encoded.length) throw new Error("zero-skip stream round-trip failed");
  console.log("self-test=ok");
}

function value(args: string[], name: string): string | undefined {
  const index = args.indexOf(name); return index >= 0 ? args[index + 1] : undefined;
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: skip_sprite_archive.ts [--self-test] [--decoded FILE --images DIR --plan FILE --palette FILE] [--preview FILE] [--width N] [--height N]");
    return;
  }
  if (args.includes("--self-test")) {
    self_test();
    if (!value(args, "--decoded")) return;
  }
  const decodedPath = value(args, "--decoded"), images = value(args, "--images");
  const plan = value(args, "--plan"), palette = value(args, "--palette");
  if (!decodedPath || !images || !plan || !palette)
    throw new Error("--decoded, --images, --plan, and --palette are required");
  const count = export_archive(
    readFileSync(decodedPath), images, plan, palette, value(args, "--preview"),
    Number(value(args, "--width") ?? 64), Number(value(args, "--height") ?? 64),
  );
  console.log(`images=${count} decoded=${statSync(decodedPath).size}`);
}

if (import.meta.main) main(process.argv.slice(2));
