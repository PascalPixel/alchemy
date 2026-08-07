import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname } from "node:path";
import { byte_png, rgba_image } from "./export_asset.ts";
import { decode_palette_trace, encode_palette, type PaletteGroup } from "./extract_resource.ts";
import { indexed_png, rgba_png } from "./import_asset.ts";
import {
  build_alignment_tail,
  inspect_alignment_tail,
  parse_alignment_tail,
  self_test_alignment_tail,
  type AlignmentTail,
} from "./alignment_tail.ts";

export const ROM_BASE = 0x08000000;

export type PixelFormat = "rgba" | "indexed8";

export interface ArchiveStream {
  decoded_size: number;
  encoded_size: number;
  tokens: PaletteGroup[];
  lookahead?: string;
}

export interface ArchivePlan {
  format: number;
  codec: string;
  chunk_width: number;
  chunk_height: number;
  columns: number;
  pixel_format?: PixelFormat;
  offset_width?: number;
  stream_alignment?: number;
  streams: ArchiveStream[];
  alignment_tail?: AlignmentTail;
}

export function number(text: string): number { return Number(text); }

export function pixel_size(pixel_format: string): number {
  if (pixel_format === "rgba") return 4;
  if (pixel_format === "indexed8") return 1;
  throw new Error("unsupported archive pixel format");
}

export function make_atlas(chunks: readonly Uint8Array[], chunk_width: number, chunk_height: number, columns: number, pixel_format: PixelFormat): Buffer {
  const depth = pixel_size(pixel_format), unit = chunk_width * chunk_height * depth;
  if (chunks.length === 0 || chunks.some((chunk) => chunk.length !== unit))
    throw new Error("archive chunks have inconsistent RGBA dimensions");
  const rows = Math.ceil(chunks.length / columns);
  const padded = [...chunks.map((chunk) => Buffer.from(chunk))];
  while (padded.length < rows * columns) padded.push(Buffer.alloc(unit));
  const width = columns * chunk_width, atlas = Buffer.alloc(width * rows * chunk_height * depth);
  padded.forEach((chunk, index) => {
    const left = index % columns * chunk_width, top = Math.floor(index / columns) * chunk_height;
    for (let y = 0; y < chunk_height; y++) {
      const source = y * chunk_width * depth, target = ((top + y) * width + left) * depth;
      chunk.copy(atlas, target, source, source + chunk_width * depth);
    }
  });
  return (pixel_format === "rgba" ? rgba_image(atlas, width) : byte_png(atlas, width))[0];
}

export function read_atlas(data: Uint8Array, count: number, chunk_width: number, chunk_height: number, columns: number, pixel_format: PixelFormat): Buffer[] {
  const depth = pixel_size(pixel_format);
  let width: number, height: number, pixels: Buffer;
  if (pixel_format === "rgba") [width, height, pixels] = rgba_png(data);
  else {
    const decoded = indexed_png(data); width = decoded[0]; height = decoded[1]; pixels = Buffer.from(decoded[2]);
  }
  const rows = Math.ceil(count / columns);
  if (width !== columns * chunk_width || height !== rows * chunk_height)
    throw new Error("archive atlas dimensions differ from its plan");
  const chunks: Buffer[] = [];
  for (let index = 0; index < count; index++) {
    const left = index % columns * chunk_width, top = Math.floor(index / columns) * chunk_height;
    const chunk: Buffer[] = [];
    for (let y = 0; y < chunk_height; y++) {
      const start = ((top + y) * width + left) * depth;
      chunk.push(pixels.subarray(start, start + chunk_width * depth));
    }
    chunks.push(Buffer.concat(chunk));
  }
  return chunks;
}

export function build_archive(atlas: Uint8Array, plan: ArchivePlan): Buffer {
  const streams = plan.streams;
  const chunks = read_atlas(atlas, streams.length, Number(plan.chunk_width), Number(plan.chunk_height), Number(plan.columns), plan.pixel_format ?? "rgba");
  const encoded: Buffer[] = [];
  chunks.forEach((chunk, index) => {
    const stream = streams[index];
    if (chunk.length !== Number(stream.decoded_size)) throw new Error("archive chunk size differs from its plan");
    const payload = Buffer.concat([encode_palette(chunk, stream.tokens), Buffer.from(stream.lookahead ?? "", "hex")]);
    if (payload.length !== Number(stream.encoded_size)) throw new Error("archive stream size differs from its plan");
    encoded.push(payload);
  });
  const offsetWidth = Number(plan.offset_width ?? 4), alignment = Number(plan.stream_alignment ?? 1);
  let offset = encoded.length * offsetWidth;
  const offsets: number[] = [], slots: Buffer[] = [];
  for (const payload of encoded) {
    offsets.push(offset);
    const padding = (alignment - payload.length % alignment) % alignment;
    const slot = Buffer.concat([payload, Buffer.alloc(padding)]);
    slots.push(slot); offset += slot.length;
  }
  if (![2, 4].includes(offsetWidth) || offsets.some((value) => value >= 2 ** (offsetWidth * 8)))
    throw new Error("archive offsets do not fit their configured width");
  const table = Buffer.alloc(offsets.length * offsetWidth);
  offsets.forEach((value, index) => offsetWidth === 2 ? table.writeUInt16LE(value, index * 2) : table.writeUInt32LE(value, index * 4));
  const archive = Buffer.concat([table, ...slots]);
  if (plan.alignment_tail === undefined) return archive;
  return Buffer.concat([
    archive,
    build_alignment_tail(parse_alignment_tail(plan.alignment_tail, plan.alignment_tail.size, 3, "archive alignment tail")),
  ]);
}

export function self_test(): void {
  self_test_alignment_tail();
  console.log("self-test=ok");
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name); if (index < 0 || index + 1 >= args.length) throw new Error(`${name} is required`); return args[index + 1];
}

function integer(args: string[], name: string, fallback?: number): number {
  if (!args.includes(name)) { if (fallback !== undefined) return fallback; throw new Error(`${name} is required`); }
  const result = Number(option(args, name)); if (!Number.isInteger(result)) throw new Error(`${name} must be an integer`); return result;
}

function main(args: string[]): void {
  if (args.includes("--self-test")) {
    self_test();
    args = args.filter((argument) => argument !== "--self-test");
    if (!args.length) return;
  }
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: archive_asset.ts [--self-test] ROM --address ADDRESS --input-end END --chunk-width WIDTH --chunk-height HEIGHT --columns COLUMNS --plan PLAN --atlas ATLAS [--source-end END] [options]");
    return;
  }
  const romPath = args[0]; if (!romPath) throw new Error("ROM is required");
  const address = integer(args, "--address"), inputEnd = integer(args, "--input-end");
  const sourceEnd = integer(args, "--source-end", inputEnd);
  const chunkWidth = integer(args, "--chunk-width"), chunkHeight = integer(args, "--chunk-height"), columns = integer(args, "--columns");
  const pixelFormat = (args.includes("--pixel-format") ? option(args, "--pixel-format") : "rgba") as PixelFormat;
  if (!["rgba", "indexed8"].includes(pixelFormat)) throw new Error("unsupported archive pixel format");
  const offsetWidth = integer(args, "--offset-width", 4), streamAlignment = integer(args, "--stream-alignment", 1);
  if (![2, 4].includes(offsetWidth)) throw new Error("offset width must be 2 or 4");
  const planPath = option(args, "--plan"), atlasPath = option(args, "--atlas");
  if (sourceEnd < address || sourceEnd > inputEnd) throw new Error("archive source end lies outside its boundary");
  const rom = readFileSync(romPath), start = address - ROM_BASE, end = sourceEnd - ROM_BASE, archive = rom.subarray(start, end);
  const tail = rom.subarray(end, inputEnd - ROM_BASE);
  if (tail.length > 3) throw new Error("archive alignment tail exceeds its bounded extent");
  if (archive.length < 4) throw new Error("archive is empty");
  if (streamAlignment <= 0) throw new Error("stream alignment must be positive");
  const first = offsetWidth === 2 ? archive.readUInt16LE(0) : archive.readUInt32LE(0);
  if (first === 0 || first % offsetWidth !== 0 || first > archive.length) throw new Error("archive has no valid offset table");
  const count = first / offsetWidth, offsets: number[] = [];
  for (let index = 0; index < count; index++) offsets.push(offsetWidth === 2 ? archive.readUInt16LE(index * 2) : archive.readUInt32LE(index * 4));
  if (offsets[0] !== first || offsets.some((value, index) => index > 0 && offsets[index - 1] > value))
    throw new Error("archive offsets are not monotonic");
  const chunks: Buffer[] = [], streams: ArchiveStream[] = [];
  const maximum = chunkWidth * chunkHeight * pixel_size(pixelFormat);
  offsets.forEach((offset, index) => {
    const streamEnd = index + 1 < count ? offsets[index + 1] : archive.length;
    const [decoded, cursor, tokens] = decode_palette_trace(archive, offset, streamEnd, maximum);
    if (decoded.length !== maximum) throw new Error("archive stream has an unexpected decoded size");
    const replay = encode_palette(decoded, tokens), original = archive.subarray(offset, cursor);
    if (!original.subarray(0, replay.length).equals(replay)) throw new Error("archive token replay differs");
    const lookahead = original.subarray(replay.length), padding = archive.subarray(cursor, streamEnd);
    if (padding.length !== (streamAlignment - original.length % streamAlignment) % streamAlignment || padding.some((byte) => byte !== 0))
      throw new Error("archive stream padding differs from its alignment");
    chunks.push(decoded); streams.push({ decoded_size: decoded.length, encoded_size: original.length, tokens, lookahead: lookahead.toString("hex") });
  });
  const plan: ArchivePlan = { format: 1, codec: "golden-sun-offset-palette-lz", chunk_width: chunkWidth, chunk_height: chunkHeight,
    columns, pixel_format: pixelFormat, offset_width: offsetWidth, stream_alignment: streamAlignment, streams,
    ...(tail.length ? { alignment_tail: inspect_alignment_tail(tail, 3) } : {}) };
  const atlas = make_atlas(chunks, chunkWidth, chunkHeight, columns, pixelFormat);
  if (!build_archive(atlas, plan).equals(rom.subarray(start, inputEnd - ROM_BASE))) throw new Error("rebuilt archive differs");
  mkdirSync(dirname(planPath), { recursive: true }); mkdirSync(dirname(atlasPath), { recursive: true });
  writeFileSync(planPath, JSON.stringify(plan) + "\n"); writeFileSync(atlasPath, atlas);
  console.log(`streams=${count} decoded=${chunks.reduce((sum, chunk) => sum + chunk.length, 0)} encoded=${archive.length} alignment=${tail.length}`);
}

if (import.meta.main) main(process.argv.slice(2));
