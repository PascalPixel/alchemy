#!/usr/bin/env bun
import {
  mkdirSync,
  readFileSync,
  realpathSync,
  writeFileSync,
} from "node:fs";
import { basename, dirname, join, resolve } from "node:path";

import {
  decode_general_trace,
  encode_general,
  type GeneralToken,
} from "./extract_resource.ts";
import {
  assemble_overlay,
  build_overlay_source,
} from "./overlay_disasm.ts";

export const ROM_BASE = 0x08000000;
export const ROM_END = 0x08800000;
export const DIRECTORY_ADDRESS = 0x08320000;
export const RESOURCE_ID = 0x03ce;
export const RESOURCE_ADDRESS = 0x087fcd20;
export const DECODED_ADDRESS = 0x02000000;
export const STREAM_SIZE = 0x0799;
export const DECODED_SIZE = 0x168c;
export const PADDING_ADDRESS = RESOURCE_ADDRESS + STREAM_SIZE;
export const PADDING_SIZE = 3;
export const FILL_ADDRESS = PADDING_ADDRESS + PADDING_SIZE;

interface StreamPlan {
  format: 1;
  codec: "golden-sun-general-lz";
  decoded_size: string;
  tokens: GeneralToken[];
}

interface BattleSelection {
  table: string;
  record_index: number;
  record_address: string;
  resource_id: string;
  group: number;
  variant: number;
  effect_id: number;
  consumers: string[];
}

interface PaddingSource {
  address: string;
  size: number;
  alignment: number;
  policy: "fallback";
}

interface FillSource {
  address: string;
  end: string;
  value: 0;
}

export interface Resource3ceSource {
  format: 1;
  kind: "golden-sun-final-battle-overlay";
  resource_id: "0x3ce";
  address: string;
  decoded_address: string;
  stream_size: string;
  decoded_size: string;
  overlay: "overlay.s";
  compression_plan: "stream.lz.json";
  selection: BattleSelection;
  padding: PaddingSource;
  zero_fill: FillSource;
}

export interface Resource3ceBuild {
  stream_address: number;
  stream: Buffer;
  fill_address: number;
  fill: Buffer;
  fallback_address: number;
  fallback_size: number;
}

function hexadecimal(value: number, width = 8): string {
  return `0x${value.toString(16).padStart(width, "0")}`;
}

function integer(value: unknown, label: string): number {
  const parsed = typeof value === "string" ? Number(value) : value;
  if (!Number.isSafeInteger(parsed)) throw new Error(`${label} must be an integer`);
  return parsed as number;
}

function tokenInteger(value: unknown, label: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value)) {
    throw new Error(`${label} must be a numeric integer`);
  }
  return value;
}

function exactKeys(value: object, keys: string[], label: string): void {
  const actual = Object.keys(value).sort().join(",");
  const expected = [...keys].sort().join(",");
  if (actual !== expected) throw new Error(`${label} has unknown or missing fields`);
}

function layoutDocument(): Resource3ceSource {
  return {
    format: 1,
    kind: "golden-sun-final-battle-overlay",
    resource_id: "0x3ce",
    address: hexadecimal(RESOURCE_ADDRESS),
    decoded_address: hexadecimal(DECODED_ADDRESS),
    stream_size: hexadecimal(STREAM_SIZE, 3),
    decoded_size: hexadecimal(DECODED_SIZE, 4),
    overlay: "overlay.s",
    compression_plan: "stream.lz.json",
    selection: {
      table: "0x0809f1a8",
      record_index: 200,
      record_address: "0x0809f7e8",
      resource_id: "0x3ce",
      group: 70,
      variant: 1,
      effect_id: 0,
      consumers: ["Func_0808a8e4", "Func_0808ab48", "Func_0808ab74"],
    },
    padding: {
      address: hexadecimal(PADDING_ADDRESS),
      size: PADDING_SIZE,
      alignment: 4,
      policy: "fallback",
    },
    zero_fill: {
      address: hexadecimal(FILL_ADDRESS),
      end: hexadecimal(ROM_END),
      value: 0,
    },
  };
}

function overlaySource(decoded: Uint8Array): string {
  return build_overlay_source(decoded, DECODED_ADDRESS)
    .split("\n")
    .map((line) => line.replace(/[ \t]+@.*$/, ""))
    .join("\n");
}

function parseLayout(value: unknown): Resource3ceSource {
  if (typeof value !== "object" || value === null) throw new Error("resource 3ce layout must be an object");
  const source = value as Resource3ceSource;
  exactKeys(source, [
    "format", "kind", "resource_id", "address", "decoded_address", "stream_size",
    "decoded_size", "overlay", "compression_plan", "selection", "padding", "zero_fill",
  ], "resource 3ce layout");
  if (source.format !== 1 || source.kind !== "golden-sun-final-battle-overlay" ||
      source.resource_id !== "0x3ce" || source.address !== hexadecimal(RESOURCE_ADDRESS) ||
      source.decoded_address !== hexadecimal(DECODED_ADDRESS) ||
      source.stream_size !== hexadecimal(STREAM_SIZE, 3) ||
      source.decoded_size !== hexadecimal(DECODED_SIZE, 4) ||
      source.overlay !== "overlay.s" || source.compression_plan !== "stream.lz.json") {
    throw new Error("resource 3ce layout identity differs");
  }
  if (typeof source.selection !== "object" || source.selection === null) {
    throw new Error("resource 3ce selection must be an object");
  }
  exactKeys(source.selection, [
    "table", "record_index", "record_address", "resource_id", "group", "variant",
    "effect_id", "consumers",
  ], "resource 3ce selection");
  const selection = source.selection;
  const consumers = ["Func_0808a8e4", "Func_0808ab48", "Func_0808ab74"];
  if (selection.table !== "0x0809f1a8" || selection.record_index !== 200 ||
      selection.record_address !== "0x0809f7e8" || selection.resource_id !== "0x3ce" ||
      selection.group !== 70 || selection.variant !== 1 || selection.effect_id !== 0 ||
      !Array.isArray(selection.consumers) || selection.consumers.length !== consumers.length ||
      selection.consumers.some((consumer, index) => consumer !== consumers[index])) {
    throw new Error("resource 3ce selection differs");
  }
  if (typeof source.padding !== "object" || source.padding === null) {
    throw new Error("resource 3ce padding must be an object");
  }
  exactKeys(source.padding, ["address", "size", "alignment", "policy"], "resource 3ce padding");
  if (source.padding.address !== hexadecimal(PADDING_ADDRESS) ||
      source.padding.size !== PADDING_SIZE || source.padding.alignment !== 4 ||
      source.padding.policy !== "fallback") {
    throw new Error("resource 3ce padding must remain an unclaimed aligned gap");
  }
  if (typeof source.zero_fill !== "object" || source.zero_fill === null) {
    throw new Error("resource 3ce zero fill must be an object");
  }
  exactKeys(source.zero_fill, ["address", "end", "value"], "resource 3ce zero fill");
  if (source.zero_fill.address !== hexadecimal(FILL_ADDRESS) ||
      source.zero_fill.end !== hexadecimal(ROM_END) || source.zero_fill.value !== 0) {
    throw new Error("resource 3ce zero fill differs");
  }
  return source;
}

function parsePlan(value: unknown): StreamPlan {
  if (typeof value !== "object" || value === null) throw new Error("resource 3ce stream plan must be an object");
  const plan = value as StreamPlan;
  exactKeys(plan, ["format", "codec", "decoded_size", "tokens"], "resource 3ce stream plan");
  if (plan.format !== 1 || plan.codec !== "golden-sun-general-lz" ||
      plan.decoded_size !== hexadecimal(DECODED_SIZE, 4) || !Array.isArray(plan.tokens)) {
    throw new Error("resource 3ce stream plan differs");
  }
  let decoded = 0;
  const tokens = plan.tokens.map((token, index): GeneralToken => {
    if (!Array.isArray(token) || (token[0] !== "l" && token[0] !== "c")) {
      throw new Error(`resource 3ce token ${index} opcode differs`);
    }
    if (token[0] === "l") {
      if (token.length !== 2) throw new Error(`resource 3ce token ${index} literal tuple differs`);
      const count = tokenInteger(token[1], `resource 3ce token ${index} literal count`);
      if (count < 1 || decoded + count > DECODED_SIZE) {
        throw new Error(`resource 3ce token ${index} literal count is outside its range`);
      }
      decoded += count;
      return ["l", count];
    }
    if (token.length !== 3) throw new Error(`resource 3ce token ${index} copy tuple differs`);
    const length = tokenInteger(token[1], `resource 3ce token ${index} copy length`);
    const distance = tokenInteger(token[2], `resource 3ce token ${index} copy distance`);
    if (length < 2 || length > 137 || decoded + length > DECODED_SIZE) {
      throw new Error(`resource 3ce token ${index} copy length is outside its range`);
    }
    if (distance < 1 || distance > decoded) {
      throw new Error(`resource 3ce token ${index} copy distance is outside its replay prefix`);
    }
    decoded += length;
    return ["c", length, distance];
  });
  if (decoded !== DECODED_SIZE) throw new Error("resource 3ce token plan has the wrong decoded extent");
  return { ...plan, tokens };
}

function pathInside(directory: string, leaf: string): string {
  if (basename(leaf) !== leaf) throw new Error("resource 3ce source names must be local leaves");
  return join(directory, leaf);
}

function same(left: string, right: string): boolean {
  try {
    return realpathSync(left) === realpathSync(right);
  } catch {
    return resolve(left) === resolve(right);
  }
}

export function build_resource_3ce(layoutPath: string): Resource3ceBuild {
  const source = parseLayout(JSON.parse(readFileSync(layoutPath, "utf8")));
  const directory = dirname(layoutPath);
  const overlayPath = pathInside(directory, source.overlay);
  const planPath = pathInside(directory, source.compression_plan);
  const decoded = assemble_overlay(overlayPath, DECODED_ADDRESS);
  if (decoded.length !== DECODED_SIZE) throw new Error("resource 3ce overlay size differs");
  const plan = parsePlan(JSON.parse(readFileSync(planPath, "utf8")));
  const stream = encode_general(decoded, plan.tokens);
  if (stream.length !== STREAM_SIZE) throw new Error("resource 3ce stream size differs");
  return {
    stream_address: RESOURCE_ADDRESS,
    stream,
    fill_address: FILL_ADDRESS,
    fill: Buffer.alloc(ROM_END - FILL_ADDRESS),
    fallback_address: PADDING_ADDRESS,
    fallback_size: PADDING_SIZE,
  };
}

function romRange(rom: Uint8Array, start: number, end: number): Buffer {
  const first = start - ROM_BASE;
  const last = end - ROM_BASE;
  if (first < 0 || last < first || last > rom.length) throw new Error("resource 3ce range lies outside the ROM");
  return Buffer.from(rom.subarray(first, last));
}

function verifyDirectory(rom: Buffer): void {
  const entry = DIRECTORY_ADDRESS - ROM_BASE + RESOURCE_ID * 4;
  if (entry < 0 || entry + 4 > rom.length || rom.readUInt32LE(entry) !== RESOURCE_ADDRESS) {
    throw new Error("resource directory does not select resource 3ce");
  }
}

function verifySelection(rom: Buffer): void {
  const start = 0x0809f7e8 - ROM_BASE;
  if (start < 0 || start + 8 > rom.length || rom.readInt16LE(start) !== RESOURCE_ID ||
      rom.readInt8(start + 2) !== 70 || rom.readInt8(start + 3) !== 1 ||
      rom.readUInt16LE(start + 4) !== 0 || rom.readUInt16LE(start + 6) !== 0) {
    throw new Error("battle-effect descriptor does not select resource 3ce");
  }
}

export function verify_resource_3ce(rom: Uint8Array, layoutPath: string): Resource3ceBuild {
  const image = Buffer.from(rom);
  if (image.length !== ROM_END - ROM_BASE) throw new Error("resource 3ce verifier requires the 8 MiB canonical ROM");
  verifyDirectory(image);
  verifySelection(image);
  const built = build_resource_3ce(layoutPath);
  if (!built.stream.equals(romRange(image, built.stream_address, built.stream_address + built.stream.length))) {
    throw new Error("resource 3ce stream differs from ROM");
  }
  if (!built.fill.equals(romRange(image, built.fill_address, ROM_END))) {
    throw new Error("resource 3ce structural fill differs from ROM");
  }
  return built;
}

export function export_resource_3ce(rom: Uint8Array, directory: string): Resource3ceSource {
  const image = Buffer.from(rom);
  if (image.length !== ROM_END - ROM_BASE) throw new Error("resource 3ce exporter requires the 8 MiB canonical ROM");
  verifyDirectory(image);
  verifySelection(image);
  const start = RESOURCE_ADDRESS - ROM_BASE;
  const [decoded, used, tokens] = decode_general_trace(image, start, image.length, 0x100000);
  if (decoded.length !== DECODED_SIZE) throw new Error("resource 3ce decoded size differs");
  const stream = encode_general(decoded, tokens);
  if (stream.length !== STREAM_SIZE || !stream.equals(image.subarray(start, start + STREAM_SIZE))) {
    throw new Error("resource 3ce token replay differs from ROM");
  }
  if (used < start + STREAM_SIZE || used > FILL_ADDRESS - ROM_BASE) {
    throw new Error("resource 3ce decoder crossed its aligned allocation");
  }
  if (!image.subarray(FILL_ADDRESS - ROM_BASE).every((value) => value === 0)) {
    throw new Error("resource 3ce ROM tail is not structural zero fill");
  }
  const layout = layoutDocument();
  const plan: StreamPlan = {
    format: 1,
    codec: "golden-sun-general-lz",
    decoded_size: hexadecimal(DECODED_SIZE, 4),
    tokens,
  };
  mkdirSync(directory, { recursive: true });
  const layoutPath = join(directory, "layout.json");
  writeFileSync(layoutPath, `${JSON.stringify(layout, null, 2)}\n`);
  writeFileSync(join(directory, "stream.lz.json"), `${JSON.stringify(plan, null, 2)}\n`);
  writeFileSync(join(directory, "overlay.s"), overlaySource(decoded));
  verify_resource_3ce(image, layoutPath);
  return layout;
}

export function self_test(): void {
  const source = layoutDocument();
  parseLayout(structuredClone(source));
  const mutations: Array<(value: any) => void> = [
    (value) => { value.extra = 1; },
    (value) => { value.padding.policy = "source"; },
    (value) => { value.padding.size = 4; },
    (value) => { value.zero_fill.value = 1; },
    (value) => { value.selection.record_index = 199; },
    (value) => { value.selection.consumers = [value.selection.consumers.join(",")]; },
  ];
  for (const mutate of mutations) {
    const value = structuredClone(source) as any;
    mutate(value);
    let accepted = true;
    try {
      parseLayout(value);
    } catch {
      accepted = false;
    }
    if (accepted) throw new Error("resource 3ce adversarial layout was accepted");
  }
  const plan: any = {
    format: 1,
    codec: "golden-sun-general-lz",
    decoded_size: hexadecimal(DECODED_SIZE, 4),
    tokens: [["l", DECODED_SIZE]],
  };
  parsePlan(plan);
  plan.lookahead = "00";
  let accepted = true;
  try {
    parsePlan(plan);
  } catch {
    accepted = false;
  }
  if (accepted) throw new Error("resource 3ce opaque lookahead was accepted");
  for (const invalid of [
    [["l", String(DECODED_SIZE)]],
    [["l", DECODED_SIZE, 0]],
    [["c", 2, 1]],
    [["x", DECODED_SIZE]],
  ]) {
    const malformed = { ...plan, tokens: invalid };
    delete malformed.lookahead;
    accepted = true;
    try {
      parsePlan(malformed);
    } catch {
      accepted = false;
    }
    if (accepted) throw new Error("resource 3ce malformed token plan was accepted");
  }
  console.log("self-test=ok");
}

function option(args: string[], ...names: string[]): string | undefined {
  const index = args.findIndex((argument) => names.includes(argument));
  return index >= 0 ? args[index + 1] : undefined;
}

function positional(args: string[]): string[] {
  const valued = new Set(["-o", "--output"]);
  return args.filter((argument, index) => !argument.startsWith("-") && !valued.has(args[index - 1]));
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    self_test();
    if (args.length === 1) return;
    args = args.filter((argument) => argument !== "--self-test");
  }
  const [command, input, source] = positional(args);
  if (command === undefined || args.includes("-h") || args.includes("--help")) {
    console.log("usage: resource_3ce.ts {export ROM DIRECTORY|verify ROM LAYOUT|build-stream LAYOUT|build-fill LAYOUT} [-o FILE] | --self-test");
    return;
  }
  if (command === "export") {
    if (input === undefined || source === undefined) throw new Error("export requires ROM and DIRECTORY");
    export_resource_3ce(new Uint8Array(await Bun.file(input).arrayBuffer()), source);
    console.log(`stream=0x${STREAM_SIZE.toString(16)} decoded=0x${DECODED_SIZE.toString(16)} fill=0x${(ROM_END - FILL_ADDRESS).toString(16)} fallback=${PADDING_SIZE}`);
    return;
  }
  if (command === "verify") {
    if (input === undefined || source === undefined) throw new Error("verify requires ROM and LAYOUT");
    const built = verify_resource_3ce(new Uint8Array(await Bun.file(input).arrayBuffer()), source);
    console.log(`identical=true claimed=${built.stream.length + built.fill.length} fallback=${built.fallback_size}`);
    return;
  }
  if (command === "build-stream" || command === "build-fill") {
    if (input === undefined) throw new Error(`${command} requires LAYOUT`);
    const output = option(args, "-o", "--output");
    if (output === undefined) throw new Error(`${command} requires --output`);
    if (same(input, output)) throw new Error("refusing to overwrite source layout");
    const built = build_resource_3ce(input);
    const result = command === "build-stream" ? built.stream : built.fill;
    mkdirSync(dirname(output), { recursive: true });
    writeFileSync(output, result);
    console.log(`bytes=${result.length}`);
    return;
  }
  throw new Error(`unknown command: ${command}`);
}

if (import.meta.main) await main(Bun.argv.slice(2));
