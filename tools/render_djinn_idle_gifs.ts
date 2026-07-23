#!/usr/bin/env bun
// Tool role: entrypoint; invoked by assets/README.md.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { indexed_png } from "./import_asset.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const OUTPUT_DIRECTORY = join(ROOT, "assets", "readme");
const SCALE = 2;
const DELAY_CENTISECONDS = 30;

// The archive indices interleave the two battle viewpoints. The last even
// frame in each resource is a distinct action pose, so it is not in the idle.
const LOOPS = [
  { resource: "101", frames: [0, 2, 4] },
  { resource: "102", frames: [0, 2, 4, 6] },
  { resource: "103", frames: [0, 2, 4] },
  { resource: "104", frames: [0, 2, 4] },
] as const;

function little_u16(value: number): Buffer {
  const result = Buffer.alloc(2);
  result.writeUInt16LE(value);
  return result;
}

function scale_indices(source: Uint8Array, width: number, height: number, scale: number): Buffer {
  const output = Buffer.alloc(width * height * scale * scale);
  const scaledWidth = width * scale;
  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      const value = source[y * width + x];
      for (let dy = 0; dy < scale; dy++) {
        const row = (y * scale + dy) * scaledWidth + x * scale;
        output.fill(value, row, row + scale);
      }
    }
  }
  return output;
}

function gif_lzw(indices: Uint8Array): Buffer {
  if (indices.length === 0) throw new Error("GIF frame is empty");
  const minimumCodeSize = 8;
  const clearCode = 1 << minimumCodeSize;
  const endCode = clearCode + 1;
  let nextCode = endCode + 1;
  let codeSize = minimumCodeSize + 1;
  let bits = 0;
  let bitCount = 0;
  const bytes: number[] = [];
  const dictionary = new Map<string, number>();

  const emit = (code: number): void => {
    bits |= code << bitCount;
    bitCount += codeSize;
    while (bitCount >= 8) {
      bytes.push(bits & 0xff);
      bits >>>= 8;
      bitCount -= 8;
    }
  };

  const reset = (): void => {
    dictionary.clear();
    nextCode = endCode + 1;
    codeSize = minimumCodeSize + 1;
  };

  emit(clearCode);
  let prefix = indices[0];
  for (let index = 1; index < indices.length; index++) {
    const value = indices[index];
    const key = `${prefix},${value}`;
    const code = dictionary.get(key);
    if (code !== undefined) {
      prefix = code;
      continue;
    }
    emit(prefix);
    if (nextCode < 4096) {
      dictionary.set(key, nextCode++);
      // The decoder adds this dictionary entry after it reads our next code,
      // so that code still uses the previous width.
      if (nextCode > 1 << codeSize && codeSize < 12) codeSize++;
    } else {
      emit(clearCode);
      reset();
    }
    prefix = value;
  }
  emit(prefix);
  emit(endCode);
  if (bitCount > 0) bytes.push(bits & 0xff);

  const blocks: Buffer[] = [Buffer.from([minimumCodeSize])];
  for (let offset = 0; offset < bytes.length; offset += 255) {
    const block = Buffer.from(bytes.slice(offset, offset + 255));
    blocks.push(Buffer.from([block.length]), block);
  }
  blocks.push(Buffer.from([0]));
  return Buffer.concat(blocks);
}

function make_gif(frames: readonly Buffer[], width: number, height: number, palette: readonly number[][]): Buffer {
  if (palette.length > 256) throw new Error("GIF palette has more than 256 colors");
  const colorTable = Buffer.alloc(256 * 3);
  palette.forEach((color, index) => colorTable.set(color, index * 3));
  const scaledWidth = width * SCALE;
  const scaledHeight = height * SCALE;
  const logicalScreen = Buffer.concat([
    little_u16(scaledWidth), little_u16(scaledHeight),
    Buffer.from([0xf7, 0, 0]),
  ]);
  const loopForever = Buffer.from([
    0x21, 0xff, 0x0b, ...Buffer.from("NETSCAPE2.0", "ascii"),
    0x03, 0x01, 0x00, 0x00, 0x00,
  ]);
  const encodedFrames = frames.map((frame) => Buffer.concat([
    Buffer.from([0x21, 0xf9, 0x04, 0x09]),
    little_u16(DELAY_CENTISECONDS),
    Buffer.from([0, 0]),
    Buffer.from([0x2c]),
    little_u16(0), little_u16(0),
    little_u16(scaledWidth), little_u16(scaledHeight),
    Buffer.from([0]),
    gif_lzw(scale_indices(frame, width, height, SCALE)),
  ]));
  return Buffer.concat([
    Buffer.from("GIF89a", "ascii"), logicalScreen, colorTable, loopForever,
    ...encodedFrames, Buffer.from([0x3b]),
  ]);
}

function render(resource: string, frameNumbers: readonly number[]): [string, Buffer] {
  const frames: Buffer[] = [];
  let width = 0;
  let height = 0;
  let palette: number[][] = [];
  for (const frameNumber of frameNumbers) {
    const name = `frame_${frameNumber.toString().padStart(2, "0")}.png`;
    const path = join(ROOT, "assets", "graphics", `resource_${resource}`, `images_${name}`);
    const [frameWidth, frameHeight, indices, colors] = indexed_png(readFileSync(path));
    if (frames.length === 0) {
      width = frameWidth;
      height = frameHeight;
      palette = colors;
    } else if (frameWidth !== width || frameHeight !== height) {
      throw new Error(`${path}: frame dimensions differ`);
    } else if (JSON.stringify(colors) !== JSON.stringify(palette)) {
      throw new Error(`${path}: frame palette differs`);
    }
    frames.push(Buffer.from(indices));
  }
  const pingPong = [...frames, ...frames.slice(1, -1).reverse()];
  return [join(OUTPUT_DIRECTORY, `djinn_${resource}_idle.gif`), make_gif(pingPong, width, height, palette)];
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: render_djinn_idle_gifs.ts [--check]");
    return;
  }
  const check = args.includes("--check");
  if (args.some((argument) => argument !== "--check")) throw new Error("unknown argument");
  mkdirSync(OUTPUT_DIRECTORY, { recursive: true });
  for (const loop of LOOPS) {
    const [path, rendered] = render(loop.resource, loop.frames);
    if (check) {
      if (!readFileSync(path).equals(rendered)) throw new Error(`${path}: generated GIF differs`);
      console.log(`verified=${path}`);
    } else {
      writeFileSync(path, rendered);
      console.log(`wrote=${path}`);
    }
  }
}

if (import.meta.main) main(process.argv.slice(2));
