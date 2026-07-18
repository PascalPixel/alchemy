import { readFileSync, writeFileSync } from "node:fs";
import {
  decode_general_trace, decode_palette_trace, encode_general, encode_palette,
} from "./extract_resource.ts";
import { export_tilemap, import_tilemap } from "./tilemap.ts";

type Json = Record<string, any>;
type PlanFactory = (
  decoded: Uint8Array, encoded: Uint8Array, tokens: any[], used: number, extra?: Json,
) => Json;

function hex(data: Uint8Array): string { return Buffer.from(data).toString("hex"); }
function compact(document: unknown): string { return `${JSON.stringify(document)}\n`; }
function pretty(document: unknown): string { return `${JSON.stringify(document, null, 2)}\n`; }

function u16s(data: Uint8Array): number[] {
  if (data.length % 2) throw new Error("partial u16");
  const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
  return Array.from({ length: data.length / 2 }, (_, index) => view.getUint16(index * 2, true));
}

function packU16(words: readonly number[]): Buffer {
  const output = Buffer.alloc(words.length * 2);
  words.forEach((word, index) => output.writeUInt16LE(word, index * 2));
  return output;
}

export function general_plan(
  decoded: Uint8Array, encoded: Uint8Array, tokens: any[], used: number, extra?: Json,
): Json {
  const replay = encode_general(decoded, tokens);
  if (!Buffer.from(encoded.subarray(0, replay.length)).equals(replay) || used > encoded.length)
    throw new Error("general-LZ token replay differs from component span");
  return {
    format: 1, codec: "golden-sun-general-lz", decoded_size: decoded.length,
    encoded_size: encoded.length, tokens, lookahead: hex(encoded.subarray(replay.length)), ...(extra ?? {}),
  };
}

export function tagged_palette_plan(
  decoded: Uint8Array, encoded: Uint8Array, tokens: any[], used: number, extra?: Json,
): Json {
  const replay = Buffer.concat([Buffer.from([1]), encode_palette(decoded, tokens)]);
  if (!Buffer.from(encoded.subarray(0, replay.length)).equals(replay) || used > encoded.length)
    throw new Error("tagged palette-LZ token replay differs from component span");
  return {
    format: 1, codec: "golden-sun-tagged-palette-lz", tag: 1,
    decoded_size: decoded.length, encoded_size: encoded.length, tokens,
    lookahead: hex(encoded.subarray(replay.length)), ...(extra ?? {}),
  };
}

export function decode_component(encoded: Uint8Array): [Buffer, number, any[], PlanFactory] {
  if (encoded.length && encoded[0] === 0) {
    const [decoded, used, tokens] = decode_general_trace(encoded, 0, encoded.length, 0x200000);
    return [decoded, used, tokens, general_plan];
  }
  if (encoded.length && encoded[0] === 1) {
    const [decoded, used, tokens] = decode_palette_trace(encoded, 1, encoded.length, 0x200000);
    return [decoded, used, tokens, tagged_palette_plan];
  }
  throw new Error("unsupported map-component compression tag");
}

export function encode_plan(decoded: Uint8Array, plan: Json): Buffer {
  if (plan.format !== 1 || !["golden-sun-general-lz", "golden-sun-tagged-palette-lz"].includes(plan.codec))
    throw new Error("unsupported map-component LZ plan");
  if (decoded.length !== Number(plan.decoded_size))
    throw new Error("map-component decoded size differs from plan");
  let encoded: Buffer;
  if (plan.codec === "golden-sun-general-lz") encoded = encode_general(decoded, plan.tokens);
  else {
    if (Number(plan.tag ?? -1) !== 1) throw new Error("tagged palette-LZ plan is missing tag 1");
    encoded = Buffer.concat([Buffer.from([1]), encode_palette(decoded, plan.tokens)]);
  }
  return Buffer.concat([encoded, Buffer.from(plan.lookahead ?? "", "hex")]);
}

export function export_header(container: Uint8Array, source: string): number[] {
  const header = Buffer.from(container.subarray(0, 0x3c));
  if (header.length !== 0x3c) throw new Error("map container is shorter than its 0x3c header");
  const records = Array.from({ length: 3 }, (_, index) =>
    Array.from({ length: 4 }, (__, field) => header.readUInt16LE(0x0c + 8 * index + field * 2)));
  const componentOffsets = Array.from({ length: 6 }, (_, index) => `0x${header.readUInt32LE(0x24 + index * 4).toString(16)}`);
  const document = { format: 1, parameters: Array.from(header.subarray(0, 0x0c)), records, component_offsets: componentOffsets };
  writeFileSync(source, pretty(document));
  if (!build_header(source).equals(header)) throw new Error("container header does not round-trip");
  return componentOffsets.map(Number);
}

export function build_header(source: string, offsetsCheck?: Map<number, number> | Record<number, number>): Buffer {
  const document = JSON.parse(readFileSync(source, "utf8")) as Json;
  if (document.format !== 1) throw new Error("unsupported map container header source");
  const parameters = document.parameters as number[];
  const records = document.records as number[][];
  const offsets = (document.component_offsets as string[]).map(Number);
  if (parameters.length !== 12 || parameters.some((value) => !Number.isInteger(Number(value)) || Number(value) < 0 || Number(value) > 0xff))
    throw new Error("container header requires twelve parameter bytes");
  if (records.length !== 3 || records.some((record) => record.length !== 4 || record.some((value) => !Number.isInteger(Number(value)) || Number(value) < 0 || Number(value) > 0xffff)))
    throw new Error("container header requires three four-u16 records");
  if (offsets.length !== 6 || offsets.some((value) => !Number.isInteger(value) || value < 0 || value > 0xffffffff))
    throw new Error("container header requires six u32 offsets");
  if (offsetsCheck) for (let slot = 0; slot < 6; slot++) {
    const expected = offsetsCheck instanceof Map ? offsetsCheck.get(slot) ?? 0 : Number(offsetsCheck[slot] ?? 0);
    if (offsets[slot] !== expected) throw new Error(`header offset ${slot} differs from the claimed component span`);
  }
  const output = Buffer.alloc(0x3c);
  output.set(parameters.map(Number), 0);
  records.forEach((record, index) => record.forEach((value, field) => output.writeUInt16LE(Number(value), 0x0c + index * 8 + field * 2)));
  offsets.forEach((value, index) => output.writeUInt32LE(value, 0x24 + index * 4));
  return output;
}

export function decode_metatiles(decoded: Uint8Array): [number, number[]] {
  if (!decoded.length || ![0, 1, 2].includes(decoded[0]) || (decoded.length - 1) % 8)
    throw new Error("invalid 2x2-metatile component");
  const mode = decoded[0];
  const count = (decoded.length - 1) / 2;
  if (mode === 0 || mode === 2) {
    const entries = u16s(decoded.subarray(1));
    if (mode === 2) {
      let previous = 0;
      entries.forEach((value, index) => { entries[index] = value ^ previous; previous = entries[index]; });
    }
    return [mode, entries];
  }
  let previous = 0;
  const entries: number[] = [];
  for (let index = 0; index < count; index++) {
    let value = (decoded[1 + index] << 8) | decoded[1 + count + index];
    value ^= previous;
    previous = value;
    entries.push(value);
  }
  return [mode, entries];
}

export function encode_metatiles(entries: readonly number[], mode: number): Buffer {
  if (![0, 1, 2].includes(mode) || !entries.length || entries.length % 4)
    throw new Error("metatiles require mode 0/1/2 and groups of four u16 entries");
  if (entries.some((value) => !Number.isInteger(value) || value < 0 || value > 0xffff))
    throw new Error("metatile entry is outside u16");
  if (mode === 0 || mode === 2) {
    const transformed: number[] = [];
    let previous = 0;
    for (const value of entries) { transformed.push(mode === 2 ? value ^ previous : value); previous = value; }
    return Buffer.concat([Buffer.from([mode]), packU16(transformed)]);
  }
  const planar: number[] = [];
  let previous = 0;
  for (const value of entries) { planar.push(value ^ previous); previous = value; }
  return Buffer.concat([Buffer.from([mode]), Buffer.from(planar.map((value) => value >>> 8)), Buffer.from(planar.map((value) => value & 0xff))]);
}

export function export_metatiles(encoded: Uint8Array, source: string, planPath: string): [number, number] {
  const [decoded, used, tokens, makePlan] = decode_component(encoded);
  const [mode, entries] = decode_metatiles(decoded);
  writeFileSync(source, export_tilemap(packU16(entries), 4));
  const plan = makePlan(decoded, encoded, tokens, used, {
    component: "map-metatiles-2x2", transform_mode: mode, metatiles: entries.length / 4,
  });
  writeFileSync(planPath, compact(plan));
  if (!build_metatiles(source, planPath).equals(Buffer.from(encoded)))
    throw new Error("metatile component does not round-trip");
  return [entries.length / 4, mode];
}

export function build_metatiles(source: string, planPath: string): Buffer {
  const plan = JSON.parse(readFileSync(planPath, "utf8")) as Json;
  const raw = import_tilemap(readFileSync(source, "utf8"));
  const decoded = encode_metatiles(u16s(raw), Number(plan.transform_mode));
  return encode_plan(decoded, plan);
}

export function export_descriptors(encoded: Uint8Array, source: string, planPath: string): number {
  const [decoded, used, tokens, makePlan] = decode_component(encoded);
  if (decoded.length % 4) throw new Error("descriptor component is not a sequence of four-byte records");
  const records = Array.from({ length: decoded.length / 4 }, (_, index) => Array.from(decoded.subarray(index * 4, index * 4 + 4)));
  writeFileSync(source, pretty({ format: 1, record_size: 4, records }));
  const plan = makePlan(decoded, encoded, tokens, used, { component: "map-descriptors-4byte", records: records.length });
  writeFileSync(planPath, compact(plan));
  if (!build_descriptors(source, planPath).equals(Buffer.from(encoded)))
    throw new Error("descriptor component does not round-trip");
  return records.length;
}

export function build_descriptors(source: string, planPath: string): Buffer {
  const document = JSON.parse(readFileSync(source, "utf8")) as Json;
  if (document.format !== 1 || document.record_size !== 4) throw new Error("unsupported map descriptor source");
  const records = document.records ?? [];
  if (records.some((record: any[]) => record.length !== 4 || record.some((value) => !Number.isInteger(Number(value)) || Number(value) < 0 || Number(value) > 255)))
    throw new Error("map descriptors must contain four byte values");
  const decoded = Buffer.from(records.flat().map(Number));
  return encode_plan(decoded, JSON.parse(readFileSync(planPath, "utf8")));
}

export interface AnimationQueue { header: string; commands: number[][] }

export function decode_queues(decoded: Uint8Array): AnimationQueue[] {
  if (decoded.length % 2) throw new Error("animation queue stream has a partial u16");
  const words = u16s(decoded);
  const queues: AnimationQueue[] = [];
  let cursor = 0;
  while (cursor < words.length && words[cursor] !== 0xffff) {
    const header = words[cursor];
    if ((header >>> 8) !== 0xfd) throw new Error("animation queue is missing its FDxx header");
    cursor++;
    const commands: number[][] = [];
    while (cursor < words.length && words[cursor] !== 0xfe00) {
      if (cursor + 1 >= words.length) throw new Error("animation queue has a partial command pair");
      commands.push([words[cursor], words[cursor + 1]]); cursor += 2;
    }
    if (cursor >= words.length) throw new Error("animation queue is missing FE00");
    cursor++;
    queues.push({ header: `0x${header.toString(16).padStart(4, "0")}`, commands });
  }
  if (cursor !== words.length - 1 || words[cursor] !== 0xffff)
    throw new Error("animation queue stream is missing final FFFF");
  return queues;
}

export function encode_queues(queues: AnimationQueue[]): Buffer {
  const words: number[] = [];
  for (const queue of queues) {
    const header = Number(queue.header);
    if ((header >>> 8) !== 0xfd) throw new Error("animation queue header must be FDxx");
    words.push(header);
    for (const command of queue.commands) {
      if (command.length !== 2 || command.some((value) => !Number.isInteger(Number(value)) || Number(value) < 0 || Number(value) > 0xffff))
        throw new Error("animation command must contain two u16 values");
      words.push(...command.map(Number));
    }
    words.push(0xfe00);
  }
  words.push(0xffff);
  return packU16(words);
}

export function word_list_document(decoded: Uint8Array): Json {
  if (decoded.length % 2) throw new Error("component word stream has a partial u16");
  return { format: 1, word_size: 2, words: u16s(decoded).map((word) => `0x${word.toString(16).padStart(4, "0")}`) };
}

export function encode_word_list(document: Json): Buffer {
  const words = document.words.map(Number);
  if (words.some((word: number) => !Number.isInteger(word) || word < 0 || word > 0xffff))
    throw new Error("component word is outside u16");
  return packU16(words);
}

export function export_queues(encoded: Uint8Array, source: string, planPath: string): [number | string, number] {
  const [decoded, used, tokens, makePlan] = decode_component(encoded);
  let document: Json, report: Json, counts: [number | string, number];
  try {
    const queues = decode_queues(decoded);
    document = { format: 1, terminators: ["0xfe00", "0xffff"], queues };
    const commands = queues.reduce((sum, queue) => sum + queue.commands.length, 0);
    report = { component: "map-animation-queues", queues: queues.length, commands };
    counts = [queues.length, commands];
  } catch (error) {
    if (!(error instanceof Error)) throw error;
    document = word_list_document(decoded);
    report = { component: "map-animation-words", words: document.words.length };
    counts = ["raw", document.words.length];
  }
  writeFileSync(source, pretty(document));
  writeFileSync(planPath, compact(makePlan(decoded, encoded, tokens, used, report)));
  if (!build_queues(source, planPath).equals(Buffer.from(encoded)))
    throw new Error("animation queue component does not round-trip");
  return counts;
}

export function build_queues(source: string, planPath: string): Buffer {
  const document = JSON.parse(readFileSync(source, "utf8")) as Json;
  if (document.format !== 1) throw new Error("unsupported animation queue source");
  const decoded = "queues" in document ? encode_queues(document.queues) : encode_word_list(document);
  return encode_plan(decoded, JSON.parse(readFileSync(planPath, "utf8")));
}

export interface BlendCommand { op: string; value?: string; duration?: number; target_pair?: number }

export function decode_blend_commands(decoded: Uint8Array): BlendCommand[] {
  if (decoded.length % 2) throw new Error("blend animation stream has a partial u16");
  const words = u16s(decoded);
  const commands: BlendCommand[] = [];
  let cursor = 0;
  while (cursor < words.length) {
    const word = words[cursor++];
    if (word === 0xffff) commands.push({ op: "reset" });
    else if ((word >>> 8) === 0xfe)
      commands.push(word === 0xfeff ? { op: "stop" } : { op: "jump", target_pair: word & 0xff });
    else if ((word & 0xf000) === 0x3000)
      commands.push({ op: "set_blend_control", value: `0x${word.toString(16).padStart(4, "0")}` });
    else {
      if (cursor >= words.length) throw new Error("blend value is missing its duration");
      commands.push({ op: "write_blend_value", value: `0x${word.toString(16).padStart(4, "0")}`, duration: words[cursor++] });
    }
  }
  return commands;
}

export function encode_blend_commands(commands: BlendCommand[]): Buffer {
  const words: number[] = [];
  for (const command of commands) {
    if (command.op === "reset") words.push(0xffff);
    else if (command.op === "stop") words.push(0xfeff);
    else if (command.op === "jump") {
      const target = Number(command.target_pair);
      if (!Number.isInteger(target) || target < 0 || target >= 0xff)
        throw new Error("blend jump target pair is outside 0..254");
      words.push(0xfe00 | target);
    } else if (command.op === "set_blend_control") {
      const value = Number(command.value);
      if (!Number.isInteger(value) || value < 0 || value > 0xffff || (value & 0xf000) !== 0x3000)
        throw new Error("blend-control value must be a 3xxx u16");
      words.push(value);
    } else if (command.op === "write_blend_value") {
      const value = Number(command.value), duration = Number(command.duration);
      if (!Number.isInteger(value) || value < 0 || value > 0xffff || value === 0xffff || (value >>> 8) === 0xfe || (value & 0xf000) === 0x3000)
        throw new Error("blend value collides with a control command");
      if (!Number.isInteger(duration) || duration < 0 || duration > 0xffff)
        throw new Error("blend duration is outside u16");
      words.push(value, duration);
    } else throw new Error(`unsupported blend animation operation: ${command.op}`);
  }
  return packU16(words);
}

export function export_blend_animation(encoded: Uint8Array, source: string, planPath: string): [number, string] {
  const [decoded, used, tokens, makePlan] = decode_component(encoded);
  let document: Json, report: Json, count: number;
  try {
    const commands = decode_blend_commands(decoded);
    document = { format: 1, word_size: 2, commands };
    report = { component: "map-blend-animation", commands: commands.length };
    count = commands.length;
  } catch (error) {
    if (!(error instanceof Error)) throw error;
    document = word_list_document(decoded);
    report = { component: "map-blend-words", words: document.words.length };
    count = document.words.length;
  }
  writeFileSync(source, pretty(document));
  const plan = makePlan(decoded, encoded, tokens, used, report);
  writeFileSync(planPath, compact(plan));
  if (!build_blend_animation(source, planPath).equals(Buffer.from(encoded)))
    throw new Error("blend animation component does not round-trip");
  return [count, plan.codec];
}

export function build_blend_animation(source: string, planPath: string): Buffer {
  const document = JSON.parse(readFileSync(source, "utf8")) as Json;
  if (document.format !== 1 || document.word_size !== 2) throw new Error("unsupported blend animation source");
  const decoded = "commands" in document ? encode_blend_commands(document.commands) : encode_word_list(document);
  return encode_plan(decoded, JSON.parse(readFileSync(planPath, "utf8")));
}

export function export_sparse(encoded: Uint8Array, source: string): number {
  const data = Buffer.from(encoded);
  const terminator = data.indexOf(Buffer.from([0xff, 0xff, 0xff]));
  if (terminator < 0 || terminator % 3 || data.subarray(terminator + 3).some((value) => value !== 0))
    throw new Error("invalid sparse-cell triple stream");
  const records = Array.from({ length: terminator / 3 }, (_, index) => Array.from(data.subarray(index * 3, index * 3 + 3)));
  writeFileSync(source, pretty({
    format: 1, record_size: 3, terminator: "ffffff",
    alignment_zeros: data.length - terminator - 3, records,
  }));
  if (!build_sparse(source).equals(data)) throw new Error("sparse-cell component does not round-trip");
  return records.length;
}

export function build_sparse(source: string): Buffer {
  const document = JSON.parse(readFileSync(source, "utf8")) as Json;
  if (document.format !== 1 || document.record_size !== 3 || document.terminator !== "ffffff")
    throw new Error("unsupported sparse-cell source");
  const records = document.records ?? [];
  if (records.some((record: any[]) => record.length !== 3 || record.some((value) => !Number.isInteger(Number(value)) || Number(value) < 0 || Number(value) > 255)))
    throw new Error("sparse-cell records must contain three byte values");
  const padding = Number(document.alignment_zeros);
  if (!Number.isInteger(padding) || padding < 0 || padding > 3)
    throw new Error("sparse-cell alignment is outside 0..3");
  return Buffer.concat([Buffer.from(records.flat().map(Number)), Buffer.from([0xff, 0xff, 0xff]), Buffer.alloc(padding)]);
}
