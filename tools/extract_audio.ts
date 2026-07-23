#!/usr/bin/env bun
// Tool role: entrypoint; invoked by assets/audio/README.md.
import { build_sequence, extract_sequence } from "./music_sequence.ts";
import { wavFromSignedPcm } from "./audio_wave.ts";
import { canonicalJson } from "./canonical_json.ts";

interface SequenceIndexEntry {
  name: string;
  address: string;
  size: string;
  source: string;
}

interface SequenceFailure {
  name: string;
  header: string;
  reason: string;
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || index + 1 >= args.length) throw new Error(`${name} is required`);
  return args[index + 1];
}

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function sourceJson(value: unknown, depth = 0): string {
  if (value === null || typeof value !== "object") return JSON.stringify(value);
  const indentation = "  ".repeat(depth);
  const inner = "  ".repeat(depth + 1);
  if (Array.isArray(value)) {
    if (value.length === 0) return "[]";
    if (value.every((item) => item === null || typeof item !== "object")) {
      return JSON.stringify(value);
    }
    return `[\n${value.map((item) => inner + sourceJson(item, depth + 1)).join(",\n")}\n${indentation}]`;
  }
  const entries = Object.entries(value);
  if (entries.length === 0) return "{}";
  return `{\n${entries.map(([key, item]) =>
    `${inner}${JSON.stringify(key)}: ${sourceJson(item, depth + 1)}`
  ).join(",\n")}\n${indentation}}`;
}

function soundStart(rom: Uint8Array, header: number): number | undefined {
  const offset = header - 0x08000000;
  if (offset < 0 || offset + 8 > rom.length) throw new Error("sound header lies outside ROM");
  const tracks = rom[offset];
  if (tracks === 0) return undefined;
  if (tracks > 16 || offset + 8 + tracks * 4 > rom.length) {
    throw new Error("sound header has an invalid track count");
  }
  const view = new DataView(rom.buffer, rom.byteOffset, rom.byteLength);
  let start = 0xffffffff;
  for (let index = 0; index < tracks; index++) {
    start = Math.min(start, view.getUint32(offset + 8 + index * 4, true));
  }
  return start;
}

export async function extractSequences(
  rom: Uint8Array,
  symbols: Record<string, string | number>,
  output: string,
): Promise<[SequenceIndexEntry[], SequenceFailure[]]> {
  await Bun.$`mkdir -p ${output}`.quiet();
  const sequences: SequenceIndexEntry[] = [];
  const failures: SequenceFailure[] = [];
  for (const [name, rawHeader] of Object.entries(symbols)) {
    const header = Number(rawHeader);
    try {
      const start = soundStart(rom, header);
      if (start === undefined) continue;
      const source = extract_sequence(rom, start, header, name);
      const [built] = build_sequence(source);
      const expected = Buffer.from(rom.subarray(
        start - 0x08000000,
        start - 0x08000000 + built.length,
      ));
      if (!built.equals(expected)) throw new Error("sequence round trip differs from ROM");
      const filename = `${name}.json`;
      await Bun.write(`${output}/${filename}`, sourceJson(source) + "\n");
      sequences.push({
        name,
        address: hexadecimal(start),
        size: hexadecimal(built.length),
        source: filename,
      });
    } catch (error) {
      failures.push({
        name,
        header: hexadecimal(header),
        reason: error instanceof Error ? error.message : String(error),
      });
    }
  }
  sequences.sort((left, right) => Number(left.address) - Number(right.address));
  for (let index = 1; index < sequences.length; index++) {
    const previous = sequences[index - 1];
    const current = sequences[index];
    if (Number(previous.address) + Number(previous.size) > Number(current.address)) {
      throw new Error(`${previous.name} overlaps ${current.name}`);
    }
  }
  return [sequences, failures];
}

async function main(args: string[]): Promise<void> {
  if (!args[1] || !["extract-sequences", "extract-waves"].includes(args[0])) {
    console.log("usage: extract_audio.ts extract-sequences ROM --table TABLE -o DIRECTORY");
    console.log("       extract_audio.ts extract-waves ROM --tone-start ADDRESS --tone-end ADDRESS --arena-start ADDRESS --arena-end ADDRESS -o DIRECTORY");
    return;
  }
  const rom = new Uint8Array(await Bun.file(args[1]).arrayBuffer());
  if (args[0] === "extract-waves") {
    const toneStart = Number(option(args, "--tone-start"));
    const toneEnd = Number(option(args, "--tone-end"));
    const arenaStart = Number(option(args, "--arena-start"));
    const arenaEnd = Number(option(args, "--arena-end"));
    const output = option(args, "-o");
    const view = new DataView(rom.buffer, rom.byteOffset, rom.byteLength);
    const pointers = new Set<number>();
    for (let address = toneStart; address + 4 <= toneEnd; address += 4) {
      const pointer = view.getUint32(address - 0x08000000, true);
      if (pointer >= arenaStart && pointer < arenaEnd) pointers.add(pointer);
    }
    const addresses = [...pointers].sort((left, right) => left - right);
    await Bun.$`mkdir -p ${output}`.quiet();
    const waves: Record<string, unknown>[] = [];
    for (const [pointerIndex, address] of addresses.entries()) {
      const offset = address - 0x08000000;
      const flags = view.getUint32(offset, true);
      const frequency = view.getUint32(offset + 4, true);
      const loopStart = view.getUint32(offset + 8, true);
      const sampleCount = view.getUint32(offset + 12, true) + 1;
      const next = addresses[pointerIndex + 1] ?? arenaEnd;
      const padding = rom.subarray(offset + 16 + sampleCount, next - 0x08000000);
      if (sampleCount <= 1 || ![0, 0x40000000].includes(flags) || padding.length > 3 ||
          padding.some((value) => value !== 0)) continue;
      const name = `wave_${String(waves.length).padStart(2, "0")}`;
      const filename = `${name}.pcm8.wav`;
      const samples = rom.subarray(offset + 16, offset + 16 + sampleCount);
      await Bun.write(`${output}/${filename}`, wavFromSignedPcm(samples, Math.round(frequency / 1024)));
      waves.push({
        name,
        address: hexadecimal(address),
        size: hexadecimal(next - address),
        frequency,
        loop_start: flags === 0 ? null : loopStart,
        source: filename,
      });
    }
    const index = { format: 1, engine: "smsh-pcm-wave-series", waves };
    await Bun.write(`${output}/index.json`, canonicalJson(index) + "\n");
    console.log(`waves=${waves.length} bytes=${waves.reduce((sum, wave) => sum + Number(wave.size), 0)}`);
    return;
  }
  const table = await Bun.file(option(args, "--table")).json();
  if (table.format !== 1 || typeof table.symbols !== "object") {
    throw new Error("unsupported sound table");
  }
  const output = option(args, "-o");
  const [sequences, failures] = await extractSequences(rom, table.symbols, output);
  const index = {
    format: 1,
    engine: "smsh-sequence-series",
    sequences,
    failures,
  };
  await Bun.write(`${output}/index.json`, canonicalJson(index) + "\n");
  const bytes = sequences.reduce((total, sequence) => total + Number(sequence.size), 0);
  console.log(`sequences=${sequences.length} bytes=${bytes} failures=${failures.length}`);
}

if (import.meta.main) await main(Bun.argv.slice(2));
