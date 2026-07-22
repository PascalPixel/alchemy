// Tool role: both; imported by tools/archive_asset.ts, tools/battle_effect_data.ts, tools/build_assets.ts (+12 more); invoked by package.json.
import { mkdirSync, readFileSync, realpathSync, writeFileSync } from "node:fs";
import { dirname, resolve } from "node:path";

export const ROM_BASE = 0x08000000;
export const TABLE = 0x08320000;

export class DecodeError extends Error {}

export type GeneralToken = ["l", number] | ["c", number, number];
export type PaletteOperation = ["l"] | ["e"] | ["c", number, number];
export type PaletteGroup = ["z"] | ["g", PaletteOperation[]];

export class LsbBits {
  data: Uint8Array;
  cursor: number;
  end: number;
  value = 0;
  count = 0;

  constructor(data: Uint8Array, cursor: number, end: number) {
    this.data = data; this.cursor = cursor; this.end = end;
    if (cursor & 1) { this.value = this.byte(); this.count = 8; }
    this.fill();
  }

  need(size: number): void {
    if (this.cursor + size > this.end) throw new DecodeError("compressed input ended before terminator");
  }

  byte(): number { this.need(1); return this.data[this.cursor++]; }

  fill(): void {
    this.need(2);
    const word = this.data[this.cursor] | this.data[this.cursor + 1] << 8;
    this.value = (this.value | word << this.count) >>> 0;
    this.cursor += 2; this.count += 16;
  }

  get(count: number): number {
    while (this.count < count) this.fill();
    const value = this.value & (2 ** count - 1);
    this.value >>>= count; this.count -= count;
    return value;
  }
}

function bitLength(value: number): number {
  if (value === 0) return 0;
  return Math.floor(Math.log2(value)) + 1;
}

export function append_copy(output: number[], distance: number, length: number, maximum: number): void {
  if (distance < 1 || distance > output.length)
    throw new DecodeError(`invalid back-reference distance at output offset 0x${output.length.toString(16)}`);
  if (length < 1 || output.length + length > maximum) throw new DecodeError("decoded output crossed configured bound");
  for (let index = 0; index < length; index++) output.push(output[output.length - distance]);
}

function decodeLength(bits: LsbBits): number | null {
  if (!bits.get(1)) return 2;
  if (!bits.get(1)) return 3;
  if (!bits.get(1)) return 4;
  if (!bits.get(1)) return 5;
  if (!bits.get(1)) return bits.get(1) ? 7 : 6;
  const short = bits.get(2);
  if (short) return short + 7;
  const long = bits.get(7);
  return long === 0 ? null : long + 10;
}

export function decode_general_trace(data: Uint8Array, start: number, end: number, maximum: number): [Buffer, number, GeneralToken[]] {
  if (start >= end || data[start] !== 0) throw new DecodeError("general stream is missing its kind-zero header");
  const bits = new LsbBits(data, start + 1, end), output: number[] = [], tokens: GeneralToken[] = [];
  while (true) {
    if (bits.get(1)) {
      if (output.length >= maximum) throw new DecodeError("decoded output crossed configured bound");
      output.push(bits.get(8));
      const last = tokens.at(-1);
      if (last?.[0] === "l") last[1]++;
      else tokens.push(["l", 1]);
      continue;
    }
    const length = decodeLength(bits);
    if (length === null) return [Buffer.from(output), bits.cursor, tokens];
    let distance: number;
    if (bits.get(1)) distance = bits.get(5) + 1;
    else {
      const window = output.length - 33;
      const width = window >= 0 && window < 2048 ? bitLength(window) : 12;
      distance = bits.get(width) + 33;
    }
    tokens.push(["c", length, distance]); append_copy(output, distance, length, maximum);
  }
}

export function decode_general(data: Uint8Array, start: number, end: number, maximum: number): [Buffer, number] {
  const [output, cursor] = decode_general_trace(data, start, end, maximum); return [output, cursor];
}

export function put(bits: number[], value: number, count: number): void {
  for (let index = 0; index < count; index++) bits.push(value >>> index & 1);
}

function encodeLength(bits: number[], length: number): void {
  put(bits, 0, 1);
  if (length === 2) put(bits, 0, 1);
  else if (length === 3) { put(bits, 1, 1); put(bits, 0, 1); }
  else if (length === 4) { put(bits, 3, 2); put(bits, 0, 1); }
  else if (length === 5) { put(bits, 7, 3); put(bits, 0, 1); }
  else if (length === 6 || length === 7) { put(bits, 15, 4); put(bits, 0, 1); put(bits, length - 6, 1); }
  else if (length >= 8 && length <= 10) { put(bits, 31, 5); put(bits, length - 7, 2); }
  else if (length >= 11 && length <= 137) { put(bits, 31, 5); put(bits, 0, 2); put(bits, length - 10, 7); }
  else throw new DecodeError(`unencodable copy length: ${length}`);
}

function finishBits(bits: number[], header: number): Buffer {
  put(bits, 0, 1); put(bits, 31, 5); put(bits, 0, 2); put(bits, 0, 7);
  const packed: number[] = Array(header).fill(0);
  for (let offset = 0; offset < bits.length; offset += 8) {
    let byte = 0;
    for (let index = 0; index < Math.min(8, bits.length - offset); index++) byte |= bits[offset + index] << index;
    packed.push(byte);
  }
  return Buffer.from(packed);
}

function encodeGeneralInner(decoded: Uint8Array, tokens: GeneralToken[], prefill: number, header: number): Buffer {
  const bits: number[] = [], replay: number[] = Array(prefill).fill(0);
  let cursor = 0;
  for (const token of tokens) {
    if (token[0] === "l") {
      const count = Number(token[1]);
      if (count < 1 || cursor + count > decoded.length) throw new DecodeError("literal run crossed decoded input");
      for (let index = 0; index < count; index++) {
        put(bits, 1, 1); const value = decoded[cursor++]; put(bits, value, 8); replay.push(value);
      }
      continue;
    }
    if (token.length !== 3 || token[0] !== "c") throw new DecodeError("invalid general-stream token");
    const length = Number(token[1]), distance = Number(token[2]);
    encodeLength(bits, length);
    if (distance < 1 || distance > replay.length) throw new DecodeError("copy distance crossed replay prefix");
    if (distance <= 32) { put(bits, 1, 1); put(bits, distance - 1, 5); }
    else {
      put(bits, 0, 1);
      const window = replay.length - prefill - 33;
      const width = window >= 0 && window < 2048 ? bitLength(window) : 12;
      if (distance - 33 >= 2 ** width) throw new DecodeError("copy distance exceeds position-dependent width");
      put(bits, distance - 33, width);
    }
    for (let index = 0; index < length; index++) replay.push(replay[replay.length - distance]);
    cursor += length;
  }
  if (cursor !== decoded.length || !Buffer.from(replay.slice(prefill)).equals(Buffer.from(decoded)))
    throw new DecodeError("token plan does not reconstruct decoded input");
  return finishBits(bits, header);
}

export function encode_general(decoded: Uint8Array, tokens: GeneralToken[]): Buffer {
  return encodeGeneralInner(decoded, tokens, 0, 1);
}

export function decode_general_prefill_trace(data: Uint8Array, start: number, end: number, maximum: number, prefill: number, header = 1): [Buffer, number, GeneralToken[]] {
  if (start >= end) throw new DecodeError("general stream is empty");
  if (header && data[start] !== 0) throw new DecodeError("general stream is missing its kind-zero header");
  const bits = new LsbBits(data, start + header, end), output: number[] = Array(prefill).fill(0), tokens: GeneralToken[] = [];
  while (true) {
    if (bits.get(1)) {
      if (output.length - prefill >= maximum) throw new DecodeError("decoded output crossed configured bound");
      output.push(bits.get(8)); const last = tokens.at(-1);
      if (last?.[0] === "l") last[1]++; else tokens.push(["l", 1]);
      continue;
    }
    const length = decodeLength(bits);
    if (length === null) return [Buffer.from(output.slice(prefill)), bits.cursor, tokens];
    let distance: number;
    if (bits.get(1)) distance = bits.get(5) + 1;
    else {
      const window = output.length - prefill - 33;
      const width = window >= 0 && window < 2048 ? bitLength(window) : 12;
      distance = bits.get(width) + 33;
    }
    tokens.push(["c", length, distance]); append_copy(output, distance, length, prefill + maximum);
  }
}

export function encode_general_prefill(decoded: Uint8Array, tokens: GeneralToken[], prefill: number, header = 1): Buffer {
  return encodeGeneralInner(decoded, tokens, prefill, header);
}

export function decode_palette_trace(data: Uint8Array, start: number, end: number, maximum: number): [Buffer, number, PaletteGroup[]] {
  let cursor = start;
  const output: number[] = [], groups: PaletteGroup[] = [];
  const take = (size: number): Uint8Array => {
    if (cursor + size > end) throw new DecodeError("compressed input ended before terminator");
    const result = data.subarray(cursor, cursor + size); cursor += size; return result;
  };
  while (true) {
    const flags = take(1)[0];
    if (flags === 0) {
      const block = take(8);
      if (output.length + 8 > maximum) throw new DecodeError("decoded output crossed configured bound");
      output.push(...block); groups.push(["z"]); continue;
    }
    const operations: PaletteOperation[] = []; groups.push(["g", operations]);
    for (let bit = 7; bit >= 0; bit--) {
      if (!(flags & 1 << bit)) {
        if (output.length >= maximum) throw new DecodeError("decoded output crossed configured bound");
        output.push(take(1)[0]); operations.push(["l"]); continue;
      }
      const pair = take(2), first = pair[0], second = pair[1];
      const distance = (first & 0xf0) << 4 | second;
      let length = first & 0x0f;
      if (length === 0) {
        if (distance === 0) { operations.push(["e"]); return [Buffer.from(output), cursor, groups]; }
        length = take(1)[0] + 17;
      } else length++;
      operations.push(["c", length, distance]); append_copy(output, distance, length, maximum);
    }
  }
}

export function decode_palette(data: Uint8Array, start: number, end: number, maximum: number): [Buffer, number] {
  const [output, cursor] = decode_palette_trace(data, start, end, maximum); return [output, cursor];
}

export function encode_palette(decoded: Uint8Array, groups: PaletteGroup[]): Buffer {
  const output: number[] = [], encoded: number[] = [];
  let cursor = 0, ended = false;
  for (const group of groups) {
    if (ended) throw new DecodeError("palette plan contains data after terminator");
    if (group[0] === "z") {
      if (cursor + 8 > decoded.length) throw new DecodeError("palette literal block crossed decoded input");
      encoded.push(0, ...decoded.subarray(cursor, cursor + 8)); output.push(...decoded.subarray(cursor, cursor + 8)); cursor += 8; continue;
    }
    if (group.length !== 2 || group[0] !== "g" || group[1].length === 0) throw new DecodeError("invalid palette token group");
    const operations = group[1];
    if (operations.length > 8) throw new DecodeError("palette group exceeds eight operations");
    let flags = 0; const payload: number[] = [];
    for (const [index, token] of operations.entries()) {
      if (token[0] === "l") {
        if (cursor >= decoded.length) throw new DecodeError("palette literal crossed decoded input");
        payload.push(decoded[cursor]); output.push(decoded[cursor++]); continue;
      }
      flags |= 1 << (7 - index);
      if (token[0] === "e") {
        payload.push(0, 0); ended = true;
        if (index + 1 !== operations.length) throw new DecodeError("palette terminator is not group-final");
        continue;
      }
      if (token.length !== 3 || token[0] !== "c") throw new DecodeError("invalid palette copy token");
      const length = Number(token[1]), distance = Number(token[2]);
      if (distance < 1 || distance > output.length || distance > 0xfff) throw new DecodeError("palette copy distance is invalid");
      if (length >= 2 && length <= 16) payload.push((distance >>> 8) << 4 | length - 1, distance & 0xff);
      else if (length >= 17 && length <= 272) payload.push((distance >>> 8) << 4, distance & 0xff, length - 17);
      else throw new DecodeError("palette copy length is invalid");
      for (let count = 0; count < length; count++) output.push(output[output.length - distance]);
      cursor += length;
    }
    encoded.push(flags, ...payload);
  }
  if (!ended) throw new DecodeError("palette plan lacks a terminator");
  if (cursor !== decoded.length || !Buffer.from(output).equals(Buffer.from(decoded)))
    throw new DecodeError("palette token plan does not reconstruct input");
  return Buffer.from(encoded);
}

export type ResourceKind = "general" | "palette";

export function decode(data: Uint8Array, start: number, end: number, maximum: number, kind: "auto" | ResourceKind): [ResourceKind, Buffer, number] {
  if (kind !== "auto") {
    const [output, cursor] = kind === "general" ? decode_general(data, start, end, maximum) : decode_palette(data, start, end, maximum);
    return [kind, output, cursor];
  }
  const valid: [ResourceKind, Buffer, number][] = [], errors: string[] = [];
  for (const name of ["general", "palette"] as const) {
    try {
      const [output, cursor] = name === "general" ? decode_general(data, start, end, maximum) : decode_palette(data, start, end, maximum);
      valid.push([name, output, cursor]);
    } catch (error) {
      if (error instanceof DecodeError) errors.push(`${name}: ${error.message}`); else throw error;
    }
  }
  if (valid.length === 1) return valid[0];
  if (valid.length === 0) throw new DecodeError(`no decoder accepted stream (${errors.join("; ")})`);
  throw new DecodeError("stream is ambiguous; specify --format general or palette");
}

export function synthetic_general(): Buffer {
  const bits: number[] = [];
  for (const value of Buffer.from("AB")) { put(bits, 1, 1); put(bits, value, 8); }
  put(bits, 0, 1); put(bits, 0, 1); put(bits, 1, 1); put(bits, 1, 5);
  put(bits, 0, 1); put(bits, 0x1f, 5); put(bits, 0, 2); put(bits, 0, 7);
  const packed: number[] = [0];
  for (let offset = 0; offset < bits.length; offset += 8) {
    let value = 0;
    for (let index = 0; index < Math.min(8, bits.length - offset); index++) value |= bits[offset + index] << index;
    packed.push(value);
  }
  packed.push(0, 0); return Buffer.from(packed);
}

export function self_test(): void {
  const general = synthetic_general();
  let [output, cursor, tokens] = decode_general_trace(general, 0, general.length, 4);
  if (!output.equals(Buffer.from("ABAB")) || cursor > general.length) throw new Error("general decoder self-test failed");
  if (!general.subarray(0, encode_general(output, tokens).length).equals(encode_general(output, tokens)))
    throw new Error("general encoder self-test failed");
  const palette = Buffer.from([0x30, 65, 66, 0x01, 0x02, 0, 0]);
  let groups: PaletteGroup[];
  [output, cursor, groups] = decode_palette_trace(palette, 0, palette.length, 4);
  if (!output.equals(Buffer.from("ABAB")) || cursor !== palette.length) throw new Error("palette decoder self-test failed");
  if (!encode_palette(output, groups).equals(palette)) throw new Error("palette encoder self-test failed");
  for (const [decoder, stream] of [[decode_general, general.subarray(0, -2)], [decode_palette, palette.subarray(0, -1)]] as const) {
    let accepted = true; try { decoder(stream, 0, stream.length, 4); } catch (error) { if (error instanceof DecodeError) accepted = false; else throw error; }
    if (accepted) throw new Error("truncated stream was accepted");
  }
  console.log("self-test=ok");
}

function option(args: string[], name: string): string | undefined {
  const index = args.indexOf(name); return index >= 0 ? args[index + 1] : undefined;
}

function integer(text: string | undefined, label: string): number {
  if (text === undefined) throw new Error(`${label} is required`);
  const result = Number(text);
  if (!Number.isInteger(result)) throw new Error(`invalid integer: ${text}`);
  return result;
}

function same(a: string, b: string): boolean {
  try { return realpathSync(a) === realpathSync(b); } catch { return resolve(a) === resolve(b); }
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: extract_resource.ts [ROM] [--id ID | --address ADDRESS] [--format {auto,general,palette}] [-o OUTPUT | --verify-only]");
    return;
  }
  if (args.includes("--self-test")) {
    self_test(); args = args.filter((arg) => arg !== "--self-test"); if (args.length === 0) return;
  }
  const romPath = args.find((arg) => !arg.startsWith("-") && !args.some((previous, index) =>
    args[index + 1] === arg && ["--id", "--address", "--format", "--input-end", "--max-output", "-o", "--output"].includes(previous)));
  if (!romPath) throw new Error("ROM is required unless only --self-test is used");
  const idText = option(args, "--id"), addressText = option(args, "--address");
  if (idText === undefined && addressText === undefined) throw new Error("one of --id or --address is required");
  if (idText !== undefined && addressText !== undefined) throw new Error("--id and --address are mutually exclusive");
  const verify = args.includes("--verify-only"), outputPath = option(args, "-o") ?? option(args, "--output");
  if (verify && outputPath) throw new Error("--verify-only and --output are mutually exclusive");
  if (!verify && !outputPath) throw new Error("--output is required unless --verify-only is used");
  const maximum = option(args, "--max-output") === undefined ? 0x4000000 : integer(option(args, "--max-output"), "--max-output");
  if (maximum < 0) throw new Error("--max-output cannot be negative");
  const data = readFileSync(romPath), romEnd = ROM_BASE + data.length;
  let address: number;
  if (idText !== undefined) {
    const id = integer(idText, "--id"); if (id < 0) throw new Error("--id cannot be negative");
    const entry = TABLE - ROM_BASE + id * 4;
    if (entry < 0 || entry + 4 > data.length) throw new Error("resource ID lies outside the ROM pointer table");
    address = data.readUInt32LE(entry);
  } else address = integer(addressText, "--address");
  const inputEnd = option(args, "--input-end") === undefined ? romEnd : integer(option(args, "--input-end"), "--input-end");
  if (!(ROM_BASE <= address && address < inputEnd && inputEnd <= romEnd)) throw new Error("source/input bound is outside ROM or empty");
  const format = (option(args, "--format") ?? "auto") as "auto" | ResourceKind;
  if (!["auto", "general", "palette"].includes(format)) throw new Error("invalid resource format");
  const [kind, output, cursor] = decode(data, address - ROM_BASE, inputEnd - ROM_BASE, maximum, format);
  const endAddress = ROM_BASE + cursor;
  if (!verify && outputPath) {
    if (same(outputPath, romPath)) throw new Error("refusing to overwrite the input ROM");
    mkdirSync(dirname(outputPath), { recursive: true }); writeFileSync(outputPath, output);
  }
  console.log(`format=${kind} source=${address.toString(16).padStart(8, "0")} end=${endAddress.toString(16).padStart(8, "0")} consumed=0x${(endAddress - address).toString(16)} decoded=0x${output.length.toString(16)}`);
}

if (import.meta.main) main(process.argv.slice(2));
