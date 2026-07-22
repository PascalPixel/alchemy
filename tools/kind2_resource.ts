// Tool role: both; imported by tools/build_assets.ts, tools/kind2_resource_series.ts; invoked by package.json.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname } from "node:path";
import { tile_png } from "./export_asset.ts";
import { indexed_png } from "./import_asset.ts";

export const ROM_BASE = 0x08000000;
export type Kind2Token = number | [number, number];
export interface Kind2Plan {
  format: number;
  codec: string;
  decoded_size?: number;
  encoded_size?: number;
  tokens: Kind2Token[];
  lookahead?: string;
  layout?: unknown;
}

export class BitReader {
  data: Uint8Array;
  bit: number;

  constructor(data: Uint8Array, bit = 8) {
    this.data = data;
    this.bit = bit;
  }

  get(): number {
    if (this.bit >= this.data.length * 8)
      throw new Error("tag-2 stream ended before its terminator");
    const value = (this.data[this.bit >>> 3] >>> (this.bit & 7)) & 1;
    this.bit++;
    return value;
  }

  take(count: number): number {
    let value = 0;
    for (let index = 0; index < count; index++) value |= this.get() << index;
    return value >>> 0;
  }
}

export class BitWriter {
  bits: number[] = [];

  put(value: number | boolean): void {
    this.bits.push(Number(value));
  }

  write(value: number, count: number): void {
    for (let index = 0; index < count; index++) this.bits.push((value >>> index) & 1);
  }

  finish(): Buffer {
    const output = Buffer.alloc(Math.ceil(this.bits.length / 8));
    this.bits.forEach((value, index) => { output[index >>> 3] |= value << (index & 7); });
    return output;
  }
}

export function literal_width(bits: BitReader): number {
  if (bits.get()) return 2;
  return bits.get() ? 3 : 4;
}

export function match_length(bits: BitReader): number | null {
  if (!bits.get()) return 2;
  if (!bits.get()) return 3;
  if (!bits.get()) return 4;
  if (!bits.get()) return 5;
  if (!bits.get()) return bits.get() ? 7 : 6;
  let value = bits.take(2);
  if (value) return value + 7;
  value = bits.take(7);
  return value === 0 ? null : value + 10;
}

export function distance_width(produced: number): number {
  if (produced < 33 || produced >= 2081) return 12;
  const value = produced - 33;
  return value === 0 ? 0 : 32 - Math.clz32(value);
}

export function decode_kind2(data: Uint8Array): [Buffer, number, number, Kind2Token[]] {
  if (!data.length || data[0] !== 2) throw new Error("tag-2 resource must begin with byte 2");
  const bits = new BitReader(data);
  const moveToFront = Array.from({ length: 16 }, (_, index) => index);
  const output: number[] = [];
  const tokens: Kind2Token[] = [];
  const select = (index: number): number => {
    if (index < 0 || index >= moveToFront.length) throw new Error("tag-2 MTF index is outside its table");
    const [value] = moveToFront.splice(index, 1);
    moveToFront.unshift(value);
    return value;
  };
  while (true) {
    if (bits.get()) {
      const width = literal_width(bits);
      const low = select(bits.take(width));
      const high = select(bits.take(width));
      output.push(low | (high << 4));
      tokens.push(width);
      continue;
    }
    const length = match_length(bits);
    if (length === null) return [Buffer.from(output), Math.ceil(bits.bit / 8), bits.bit, tokens];
    const distance = bits.get() ? bits.take(5) + 1 : bits.take(distance_width(output.length)) + 33;
    if (distance > output.length) throw new Error("tag-2 copy precedes decoded output");
    tokens.push([distance, length]);
    for (let index = 0; index < length; index++) output.push(output[output.length - distance]);
  }
}

export function put_length(bits: BitWriter, length: number): void {
  if (length === 2) bits.put(0);
  else if (length === 3) { bits.write(1, 1); bits.put(0); }
  else if (length === 4) { bits.write(3, 2); bits.put(0); }
  else if (length === 5) { bits.write(7, 3); bits.put(0); }
  else if (length === 6 || length === 7) { bits.write(15, 4); bits.put(0); bits.put(length - 6); }
  else if (length >= 8 && length <= 10) { bits.write(31, 5); bits.write(length - 7, 2); }
  else if (length >= 11 && length <= 137) {
    bits.write(31, 5); bits.write(0, 2); bits.write(length - 10, 7);
  } else throw new Error("tag-2 copy length is outside 2..137");
}

export function encode_kind2(decoded: Uint8Array, plan: Kind2Plan): Buffer {
  if (plan.format !== 1 || plan.codec !== "golden-sun-kind2-lz")
    throw new Error("unsupported tag-2 plan");
  const bits = new BitWriter();
  const moveToFront = Array.from({ length: 16 }, (_, index) => index);
  let cursor = 0;
  const indexOf = (value: number): number => {
    const position = moveToFront.indexOf(value);
    const [selected] = moveToFront.splice(position, 1);
    moveToFront.unshift(selected);
    return position;
  };
  for (const token of plan.tokens) {
    if (typeof token === "number") {
      const width = token;
      if (![2, 3, 4].includes(width) || cursor >= decoded.length)
        throw new Error("invalid tag-2 literal token");
      bits.put(1);
      if (width === 2) bits.put(1);
      else { bits.put(0); bits.put(width === 3); }
      const value = decoded[cursor];
      const low = indexOf(value & 15);
      const high = indexOf(value >>> 4);
      if (low >= 2 ** width || high >= 2 ** width)
        throw new Error("literal does not fit its recorded MTF width");
      bits.write(low, width); bits.write(high, width);
      cursor++;
      continue;
    }
    if (!Array.isArray(token) || token.length !== 2 || token.some((value) => !Number.isInteger(value)))
      throw new Error("invalid tag-2 copy token");
    const [distance, length] = token;
    if (distance <= 0 || cursor + length > decoded.length)
      throw new Error("tag-2 copy is outside decoded data");
    for (let index = 0; index < length; index++)
      if (decoded[cursor + index] !== decoded[cursor + index - distance])
        throw new Error("tag-2 copy token differs from source pixels");
    bits.put(0); put_length(bits, length);
    if (distance <= 32) { bits.put(1); bits.write(distance - 1, 5); }
    else {
      const width = distance_width(cursor);
      const value = distance - 33;
      if (value < 0 || value >= 2 ** width) throw new Error("tag-2 long distance does not fit");
      bits.put(0); bits.write(value, width);
    }
    cursor += length;
  }
  if (cursor !== decoded.length) throw new Error("tag-2 tokens do not cover decoded data");
  bits.put(0); bits.write(31, 5); bits.write(0, 2); bits.write(0, 7);
  return Buffer.concat([Buffer.from([2]), bits.finish(), Buffer.from(plan.lookahead ?? "", "hex")]);
}

export function export_resource(
  data: Uint8Array, source: string, planPath: string, bpp: number, columns: number,
  palettePath?: string, layout?: unknown,
): [number, number, number] {
  const [decoded, used, bit, tokens] = decode_kind2(data);
  const colors = palettePath ? indexed_png(readFileSync(palettePath))[3] : undefined;
  const [image] = tile_png(decoded, bpp, columns, colors);
  const plan: Kind2Plan = {
    format: 1, codec: "golden-sun-kind2-lz", decoded_size: decoded.length,
    encoded_size: data.length, tokens, lookahead: Buffer.from(data.subarray(used)).toString("hex"),
  };
  if (layout !== undefined) plan.layout = layout;
  if (!encode_kind2(decoded, plan).equals(Buffer.from(data)))
    throw new Error("tag-2 resource does not re-encode exactly");
  mkdirSync(dirname(source), { recursive: true });
  writeFileSync(source, image);
  mkdirSync(dirname(planPath), { recursive: true });
  writeFileSync(planPath, `${JSON.stringify(plan)}\n`);
  return [decoded.length, tokens.length, bit];
}

export function self_test(): void {
  const prefix = Buffer.from(Array.from({ length: 32 }, (_, index) => (index * 37 + 11) & 255));
  const decoded = Buffer.concat([prefix, prefix.subarray(-16), prefix.subarray(-16)]);
  const plan: Kind2Plan = {
    format: 1, codec: "golden-sun-kind2-lz", tokens: [...Array(32).fill(4), [16, 32]], lookahead: "a55a",
  };
  const encoded = encode_kind2(decoded, plan);
  const [rebuilt, used, , tokens] = decode_kind2(encoded);
  if (!rebuilt.equals(decoded) || JSON.stringify(tokens) !== JSON.stringify(plan.tokens) ||
      !encoded.subarray(used).equals(Buffer.from("a55a", "hex")))
    throw new Error("tag-2 codec self-test failed");
  console.log("self-test=ok");
}

function value(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : undefined;
}

function positional(args: string[]): string | undefined {
  const takesValue = new Set(["--address", "--size", "--source", "--plan", "--bpp", "--columns", "--palette"]);
  for (let index = 0; index < args.length; index++) {
    if (takesValue.has(args[index])) { index++; continue; }
    if (!args[index].startsWith("-")) return args[index];
  }
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: kind2_resource.ts [--self-test] [ROM --address N --size N --source FILE --plan FILE] [--bpp {4,8}] [--columns N] [--palette FILE]");
    return;
  }
  if (args.includes("--self-test")) {
    self_test();
    if (!positional(args.filter((item) => item !== "--self-test"))) return;
  }
  const romPath = positional(args);
  const addressText = value(args, "--address");
  const sizeText = value(args, "--size");
  const source = value(args, "--source");
  const plan = value(args, "--plan");
  if (!romPath || !addressText || !sizeText || !source || !plan)
    throw new Error("rom, --address, --size, --source, and --plan are required");
  const address = Number(addressText);
  const size = Number(sizeText);
  const rom = readFileSync(romPath);
  const start = address - ROM_BASE;
  const data = rom.subarray(start, start + size);
  if (data.length !== size) throw new Error("resource range is outside the ROM");
  const [decoded, tokens, bit] = export_resource(
    data, source, plan, Number(value(args, "--bpp") ?? 4), Number(value(args, "--columns") ?? 16), value(args, "--palette"),
  );
  console.log(`decoded=0x${decoded.toString(16)} tokens=${tokens} consumed_bits=${bit}`);
}

if (import.meta.main) main(process.argv.slice(2));
