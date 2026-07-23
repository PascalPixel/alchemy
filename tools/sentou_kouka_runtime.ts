#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts, tools/executable_gap_sources.ts, tools/relocated_arm_helpers.ts; invoked by package.json.
import { canonicalJson, isCanonicalJsonText } from "./canonical_json.ts";
import {
  existsSync,
  mkdirSync,
  mkdtempSync,
  readFileSync,
  renameSync,
  rmSync,
  writeFileSync,
} from "node:fs";
import { basename, dirname, join, resolve } from "node:path";

export const ROM_BASE = 0x08000000;
export const SENTOU_KOUKA_ADDRESS = 0x080eda78;
export const SENTOU_KOUKA_END = 0x080f0254;
export const SENTOU_KOUKA_SIZE = SENTOU_KOUKA_END - SENTOU_KOUKA_ADDRESS;

const KEISU_ADDRESS = 0x080eda78;
const GOUSEI_ADDRESS = 0x080edaf0;
const HYOU_A_ADDRESS = 0x080ede48;
const KANSUU_ADDRESS = 0x080ee2b4;
const HYOU_B_ADDRESS = 0x080ee910;
const GOUSEI_IRO_ADDRESS = 0x080eefa4;
const BIT_MASK_ADDRESS = 0x080ef014;
const ZERO_FILL_ADDRESS = 0x080ef054;
const ZERO_FILL_SPLIT = 0x080ef800;
const TENKAI_ADDRESS = 0x080f0000;

const CALLBACK_COUNT = 407;
const JsonObject = Object as unknown as { keys(value: unknown): string[] };
const TMPDIR = Bun.env.TMPDIR ?? Bun.env.TMP ?? Bun.env.TEMP ?? "/tmp";

type Json = Record<string, any>;
type TableType = "u8" | "s8" | "u16" | "s16" | "u32" | "s32";

interface TableSpec {
  name: string;
  address: number;
  end: number;
  type: TableType;
}

const HYOU_A_BOUNDARIES = [
  0x080ede48, 0x080ede5c, 0x080ede84, 0x080ede96, 0x080ede9f, 0x080edea5,
  0x080edeab, 0x080edeb2, 0x080edebe, 0x080edeca, 0x080eded0, 0x080eded6,
  0x080ededc, 0x080edee8, 0x080edefc, 0x080edf04, 0x080edf58, 0x080edf5e,
  0x080edf64, 0x080edf70, 0x080edf76, 0x080edf7b, 0x080edf7f, 0x080edf83,
  0x080edf88, 0x080edf90, 0x080edfb1, 0x080edfd2, 0x080ee016, 0x080ee037,
  0x080ee058, 0x080ee05c, 0x080ee060, 0x080ee064, 0x080ee06a, 0x080ee070,
  0x080ee07c, 0x080ee088, 0x080ee090, 0x080ee096, 0x080ee09c, 0x080ee09f,
  0x080ee0a2, 0x080ee0aa, 0x080ee0b0, 0x080ee0b3, 0x080ee0b6, 0x080ee0c4,
  0x080ee0d6, 0x080ee0e8, 0x080ee10c, 0x080ee11a, 0x080ee128, 0x080ee134,
  0x080ee158, 0x080ee15a, 0x080ee163, 0x080ee16c, 0x080ee174, 0x080ee177,
  0x080ee17a, 0x080ee17e, 0x080ee184, 0x080ee188, 0x080ee18a, 0x080ee18e,
  0x080ee194, 0x080ee19a, 0x080ee1a0, 0x080ee1ac, 0x080ee1b4, 0x080ee1c4,
  0x080ee1ca, 0x080ee1d3, 0x080ee1f5, 0x080ee1fb, 0x080ee207, 0x080ee214,
  0x080ee244, 0x080ee250, 0x080ee25e, 0x080ee262, 0x080ee294, 0x080ee29a,
  0x080ee2a9, 0x080ee2ae, 0x080ee2b4,
] as const;

const HYOU_B_BOUNDARIES = [
  0x080ee910, 0x080ee916, 0x080ee920, 0x080ee925, 0x080ee92a, 0x080ee930,
  0x080ee934, 0x080ee93e, 0x080ee943, 0x080ee948, 0x080ee952, 0x080ee958,
  0x080ee966, 0x080ee974, 0x080ee9f8, 0x080eea08, 0x080eea20, 0x080eea2c,
  0x080eea38, 0x080eea41, 0x080eea44, 0x080eea4a, 0x080eea50, 0x080eea56,
  0x080eea62, 0x080eea88, 0x080eea91, 0x080eea99, 0x080eeaa2, 0x080eeab2,
  0x080eeab8, 0x080eeabb, 0x080eeac3, 0x080eeacc, 0x080eeadc, 0x080eeae2,
  0x080eeaec, 0x080eeafa, 0x080eeb40, 0x080eeb48, 0x080eeb4b, 0x080eeb4e,
  0x080eeb54, 0x080eeb58, 0x080eeb5e, 0x080eeb61, 0x080eeb71, 0x080eeb79,
  0x080eeb80, 0x080eeb88, 0x080eeb96, 0x080eebae, 0x080eebb6, 0x080eebd6,
  0x080eebe2, 0x080eebe6, 0x080eebe9, 0x080eebec, 0x080eec28, 0x080eec2f,
  0x080eec36, 0x080eec3d, 0x080eec44, 0x080eec52, 0x080eec5a, 0x080eec5f,
  0x080eec63, 0x080eec68, 0x080eec70, 0x080eec74, 0x080eec7d, 0x080eec86,
  0x080eec98, 0x080eeca1, 0x080eecb2, 0x080eecf2, 0x080eecf7, 0x080eecfc,
  0x080eecff, 0x080eed0e, 0x080eed1e, 0x080eed3e, 0x080eed7e, 0x080eed90,
  0x080eed9a, 0x080eeda0, 0x080eeda3, 0x080eeda6, 0x080eedac, 0x080eedb2,
  0x080eedb8, 0x080eedbe, 0x080eedca, 0x080eedf4, 0x080eedfb, 0x080eee02,
  0x080eee10, 0x080eee17, 0x080eee1e, 0x080eee2a, 0x080eee36, 0x080eee3e,
  0x080eee46, 0x080eee4e, 0x080eee56, 0x080eee5e, 0x080eee66, 0x080eee76,
  0x080eeea0, 0x080eeebc, 0x080eeeca, 0x080eeed8, 0x080eeee1, 0x080eeeea,
  0x080eeef8, 0x080eef06, 0x080eef0c, 0x080eef12, 0x080eef18, 0x080eef28,
  0x080eef30, 0x080eef3e, 0x080eef4a, 0x080eef50, 0x080eef56, 0x080eef5f,
  0x080eef88, 0x080eef96, 0x080eefa4,
] as const;

const GOUSEI_LABELS = new Map<number, string>([
  [0x080edaf0, "SentouKouka_YomiGyaku"],
  [0x080edaf8, "SentouKouka_YomiJun"],
  [0x080edb00, "SentouKouka_NuriJun"],
  [0x080edb10, "SentouKouka_NuriGyaku"],
  [0x080edb20, "SentouKouka_Nuri8Jun"],
  [0x080edb84, "SentouKouka_Nuri8Gyaku"],
  [0x080edbe8, "SentouKouka_HikakuJun"],
  [0x080edbf8, "SentouKouka_HikakuGyaku"],
  [0x080edc08, "SentouKouka_Hikaku4Jun"],
  [0x080edc48, "SentouKouka_Hikaku4Gyaku"],
  [0x080edc88, "SentouKouka_KasanJun"],
  [0x080edca0, "SentouKouka_KasanGyaku"],
  [0x080edcb8, "SentouKouka_Mask"],
  [0x080edcc4, "SentouKouka_Gousei"],
]);

const GOUSEI_IRO_LABELS = new Map<number, string>([
  [0x080eefa4, "SentouKouka_IroJun"],
  [0x080eefdc, "SentouKouka_IroGyaku"],
]);

function hexadecimal(value: number, width = 8): string {
  return `0x${value.toString(16).padStart(width, "0")}`;
}

function pretty(value: unknown): string {
  return `${canonicalJson(value)}\n`;
}

function exactKeys(value: unknown, keys: string[], name: string): asserts value is Json {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${name} must be an object`);
  const actual = JsonObject.keys(value).sort(), expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index]))
    throw new Error(`${name} has unknown or missing fields`);
}

function document(path: string): Json {
  const text = readFileSync(path, "utf8"), value = JSON.parse(text);
  if (!isCanonicalJsonText(text, value)) throw new Error(`${path}: source is not canonical JSON`);
  return value;
}

function integer(value: unknown, minimum: number, maximum: number, name: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < minimum || value > maximum)
    throw new Error(`invalid ${name}`);
  return value;
}

function array(value: unknown, count: number, name: string): unknown[] {
  if (!Array.isArray(value) || value.length !== count) throw new Error(`${name} requires ${count} entries`);
  return value;
}

function tableSpecs(boundaries: readonly number[], prefix: string): TableSpec[] {
  return Array.from({ length: boundaries.length - 1 }, (_, index) => ({
    name: `${prefix}_${index.toString().padStart(3, "0")}`,
    address: boundaries[index],
    end: boundaries[index + 1],
    type: prefix === "hyou_a" && index === 0 ? "u16" : "u8",
  }));
}

const HYOU_A_SPECS = tableSpecs(HYOU_A_BOUNDARIES, "hyou_a");
const HYOU_B_SPECS = tableSpecs(HYOU_B_BOUNDARIES, "hyou_b");

function typeWidth(type: TableType): number {
  return type === "u8" || type === "s8" ? 1 : type === "u16" || type === "s16" ? 2 : 4;
}

function typeRange(type: TableType): [number, number] {
  if (type === "u8") return [0, 0xff];
  if (type === "s8") return [-0x80, 0x7f];
  if (type === "u16") return [0, 0xffff];
  if (type === "s16") return [-0x8000, 0x7fff];
  if (type === "u32") return [0, 0xffffffff];
  return [-0x80000000, 0x7fffffff];
}

function typedValues(data: Buffer, offset: number, count: number, type: TableType): number[] {
  const values: number[] = [], width = typeWidth(type);
  for (let index = 0; index < count; index++) {
    const at = offset + index * width;
    if (type === "u8") values.push(data.readUInt8(at));
    else if (type === "s8") values.push(data.readInt8(at));
    else if (type === "u16") values.push(data.readUInt16LE(at));
    else if (type === "s16") values.push(data.readInt16LE(at));
    else if (type === "u32") values.push(data.readUInt32LE(at));
    else values.push(data.readInt32LE(at));
  }
  return values;
}

function encodeValues(values: unknown, type: TableType, count: number, name: string): Buffer {
  const input = array(values, count, name), width = typeWidth(type), output = Buffer.alloc(count * width);
  const [minimum, maximum] = typeRange(type);
  input.forEach((value, index) => {
    const parsed = integer(value, minimum, maximum, `${name} value ${index}`), at = index * width;
    if (type === "u8") output.writeUInt8(parsed, at);
    else if (type === "s8") output.writeInt8(parsed, at);
    else if (type === "u16") output.writeUInt16LE(parsed, at);
    else if (type === "s16") output.writeInt16LE(parsed, at);
    else if (type === "u32") output.writeUInt32LE(parsed, at);
    else output.writeInt32LE(parsed, at);
  });
  return output;
}

function parseTableDocument(path: string, specs: TableSpec[], address: number, end: number): Buffer {
  const source = document(path);
  exactKeys(source, ["format", "address", "size", "tables"], "effect table source");
  if (source.format !== 1 || source.address !== hexadecimal(address) || source.size !== end - address)
    throw new Error("effect table extent differs");
  const tables = array(source.tables, specs.length, "effect tables"), output: Buffer[] = [];
  tables.forEach((table, index) => {
    exactKeys(table, ["name", "address", "type", "values"], `effect table ${index}`);
    const spec = specs[index], width = typeWidth(spec.type), count = (spec.end - spec.address) / width;
    if (!Number.isInteger(count) || table.name !== spec.name || table.address !== hexadecimal(spec.address) || table.type !== spec.type)
      throw new Error(`effect table ${index} identity differs`);
    output.push(encodeValues(table.values, spec.type, count, spec.name));
  });
  const built = Buffer.concat(output);
  if (built.length !== end - address) throw new Error("effect table output size differs");
  return built;
}

function exportTableDocument(data: Buffer, specs: TableSpec[], address: number, end: number): Json {
  return {
    format: 1,
    address: hexadecimal(address),
    size: end - address,
    tables: specs.map((spec) => {
      const width = typeWidth(spec.type), offset = spec.address - address;
      return {
        name: spec.name,
        address: hexadecimal(spec.address),
        type: spec.type,
        values: typedValues(data, offset, (spec.end - spec.address) / width, spec.type),
      };
    }),
  };
}

function buildKeisu(path: string): Buffer {
  const source = document(path);
  exactKeys(source, ["format", "address", "size", "unit_q16", "negative_offsets", "coordinate_pairs", "scales_q16"], "effect coefficients");
  if (source.format !== 1 || source.address !== hexadecimal(KEISU_ADDRESS) || source.size !== GOUSEI_ADDRESS - KEISU_ADDRESS)
    throw new Error("effect coefficient extent differs");
  const unit = encodeValues(source.unit_q16, "s32", 4, "unit q16");
  const negative = encodeValues(source.negative_offsets, "s32", 4, "negative offsets");
  const pairs = Buffer.alloc(24);
  array(source.coordinate_pairs, 6, "coordinate pairs").forEach((pair, index) => {
    const values = array(pair, 2, `coordinate pair ${index}`);
    pairs.writeInt16LE(integer(values[0], -0x8000, 0x7fff, `coordinate pair ${index} x`), index * 4);
    pairs.writeInt16LE(integer(values[1], -0x8000, 0x7fff, `coordinate pair ${index} y`), index * 4 + 2);
  });
  exactKeys(source.scales_q16, ["base", "enhanced", "normal_a", "normal_b", "high"], "effect q16 scales");
  const scales = Buffer.concat([
    encodeValues(source.scales_q16.base, "s32", 4, "base scales"),
    encodeValues(source.scales_q16.enhanced, "s32", 2, "enhanced scales"),
    encodeValues(source.scales_q16.normal_a, "s32", 4, "normal A scales"),
    encodeValues(source.scales_q16.normal_b, "s32", 4, "normal B scales"),
    encodeValues(source.scales_q16.high, "s32", 2, "high scales"),
  ]);
  const output = Buffer.concat([unit, negative, pairs, scales]);
  if (output.length !== GOUSEI_ADDRESS - KEISU_ADDRESS) throw new Error("effect coefficient size differs");
  return output;
}

function exportKeisu(data: Buffer): Json {
  const words = (offset: number, count: number) => typedValues(data, offset, count, "s32");
  return {
    format: 1,
    address: hexadecimal(KEISU_ADDRESS),
    size: GOUSEI_ADDRESS - KEISU_ADDRESS,
    unit_q16: words(0, 4),
    negative_offsets: words(16, 4),
    coordinate_pairs: Array.from({ length: 6 }, (_, index) => [data.readInt16LE(32 + index * 4), data.readInt16LE(34 + index * 4)]),
    scales_q16: {
      base: words(56, 4),
      enhanced: words(72, 2),
      normal_a: words(80, 4),
      normal_b: words(96, 4),
      high: words(112, 2),
    },
  };
}

function callbackSymbol(word: number): string {
  if (word === 0 || (word & 1) === 0) throw new Error("callback is not a Thumb pointer");
  const address = word & ~1;
  if (address < 0x08000000 || address >= SENTOU_KOUKA_ADDRESS || (address & 1) !== 0) throw new Error("callback target is outside reconstructed code");
  return `Func_${address.toString(16).padStart(8, "0")}`;
}

function callbackWord(value: unknown, index: number): number {
  if (value === null) return 0;
  if (typeof value !== "string") throw new Error(`callback ${index} must be symbolic or null`);
  const match = /^Func_([0-9a-f]{8})$/.exec(value);
  if (!match) throw new Error(`callback ${index} symbol differs`);
  const address = Number.parseInt(match[1], 16);
  if (address < 0x08000000 || address >= SENTOU_KOUKA_ADDRESS || (address & 1) !== 0)
    throw new Error(`callback ${index} target differs`);
  return address | 1;
}

function buildCallbacks(path: string): Buffer {
  const source = document(path);
  exactKeys(source, ["format", "address", "size", "mode", "index_bias", "entries"], "effect callbacks");
  if (source.format !== 1 || source.address !== hexadecimal(KANSUU_ADDRESS) || source.size !== HYOU_B_ADDRESS - KANSUU_ADDRESS ||
      source.mode !== "thumb" || source.index_bias !== -1)
    throw new Error("effect callback directory identity differs");
  const entries = array(source.entries, CALLBACK_COUNT, "effect callback directory"), output = Buffer.alloc(CALLBACK_COUNT * 4);
  entries.forEach((entry, index) => output.writeUInt32LE(callbackWord(entry, index), index * 4));
  const nonnull = entries.filter((entry) => entry !== null) as string[], unique = new Set(nonnull);
  if (nonnull.length !== 162 || entries.length - nonnull.length !== 245 || unique.size !== 143 || nonnull.length - unique.size !== 19)
    throw new Error("effect callback null or alias structure differs");
  return output;
}

function exportCallbacks(data: Buffer): Json {
  const entries = Array.from({ length: CALLBACK_COUNT }, (_, index) => {
    const word = data.readUInt32LE(index * 4);
    return word === 0 ? null : callbackSymbol(word);
  });
  return {
    format: 1,
    address: hexadecimal(KANSUU_ADDRESS),
    size: HYOU_B_ADDRESS - KANSUU_ADDRESS,
    mode: "thumb",
    index_bias: -1,
    entries,
  };
}

function buildMasks(path: string): Buffer {
  const source = document(path);
  exactKeys(source, ["format", "address", "size", "type", "repeat", "values"], "effect bit masks");
  if (source.format !== 1 || source.address !== hexadecimal(BIT_MASK_ADDRESS) || source.size !== ZERO_FILL_ADDRESS - BIT_MASK_ADDRESS ||
      source.type !== "u16" || source.repeat !== 2)
    throw new Error("effect bit mask identity differs");
  const values = array(source.values, 16, "effect bit masks").map((value, index) => integer(value, 0, 0xffff, `effect bit mask ${index}`));
  for (let index = 0; index < 8; index++) if (values[index] !== 1 << index) throw new Error("effect low-bit mask order differs");
  for (let index = 0; index < 8; index++) {
    const expected = (12 - Math.floor(index / 2)) << 8 | 1 + index % 2;
    if (values[index + 8] !== expected) throw new Error("effect packed-bit mask order differs");
  }
  const single = encodeValues(values, "u16", 16, "effect bit masks");
  return Buffer.concat([single, single]);
}

function exportMasks(data: Buffer): Json {
  if (!data.subarray(0, 32).equals(data.subarray(32, 64))) throw new Error("effect bit mask copies differ");
  return {
    format: 1,
    address: hexadecimal(BIT_MASK_ADDRESS),
    size: ZERO_FILL_ADDRESS - BIT_MASK_ADDRESS,
    type: "u16",
    repeat: 2,
    values: typedValues(data, 0, 16, "u16"),
  };
}

interface InstructionRow {
  address: number;
  word: string;
  text: string;
}

function objdumpRows(data: Buffer, base: number, thumb = false): InstructionRow[] {
  const work = mkdtempSync(join(TMPDIR, "sentou-kouka-objdump-"));
  try {
    const binary = join(work, "source.bin");
    writeFileSync(binary, data);
    const command = ["arm-none-eabi-objdump", "-D", "-b", "binary", "-marmv4t"];
    if (thumb) command.push("-Mforce-thumb");
    command.push(`--adjust-vma=${hexadecimal(base)}`, binary);
    const process = Bun.spawnSync(command, { stdout: "pipe", stderr: "pipe" });
    if (process.exitCode !== 0) throw new Error(process.stderr.toString().trim());
    const rows: InstructionRow[] = [];
    for (const line of process.stdout.toString().split(/\r?\n/)) {
      const match = thumb
        ? /^\s*([0-9a-f]+):\s+[0-9a-f]{4}\s+(.+)$/.exec(line)
        : /^\s*([0-9a-f]+):\s+[0-9a-f]{8}\s+(.+)$/.exec(line);
      if (!match) continue;
      rows.push({
        address: Number.parseInt(match[1], 16),
        word: thumb ? "" : line.match(/^\s*[0-9a-f]+:\s+([0-9a-f]{8})\s+/)![1],
        text: match[2].replace(/\s+@.*$/, "").trim(),
      });
    }
    return rows;
  } finally {
    rmSync(work, { recursive: true, force: true });
  }
}

const BRANCH = /\b(b(?:l|eq|ne|cs|cc|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|hs|lo)?)\s+0x([0-9a-f]+)\b/;
const ARM_PC_LITERAL = /^ldr[a-z]*\s+(?:r\d+|sp|lr|ip),\s*\[pc,\s*#(-?\d+)\]/;

function localLabel(address: number): string {
  return `.L_${address.toString(16).padStart(8, "0")}`;
}

function renderRows(rows: InstructionRow[], base: number, end: number, named: ReadonlyMap<number, string>): string[] {
  const byAddress = new Map(rows.map((row) => [row.address, row]));
  for (let address = base; address < end; address += 4) if (!byAddress.has(address)) throw new Error(`missing ARM row at ${hexadecimal(address)}`);
  const branchTargets = new Set<number>();
  const literalWords = new Set<number>();
  for (const row of rows) {
    const match = BRANCH.exec(row.text);
    if (match) {
      const target = Number.parseInt(match[2], 16);
      if (base <= target && target < end) branchTargets.add(target);
    }
    const literal = ARM_PC_LITERAL.exec(row.text);
    if (literal !== null) {
      const target = row.address + 8 + Number(literal[1]);
      if ((target & 3) === 0 && base <= target && target + 4 <= end) literalWords.add(target);
    }
  }
  const output: string[] = [];
  for (let address = base; address < end; address += 4) {
    const name = named.get(address);
    if (name) output.push(`${name}:`);
    if (branchTargets.has(address) && !name) output.push(`${localLabel(address)}:`);
    let text = byAddress.get(address)!.text;
    const undefinedWord = /^@ <UNDEFINED> instruction: (0x[0-9a-f]{8})$/.exec(text);
    if (literalWords.has(address)) text = `.4byte 0x${byAddress.get(address)!.word}`;
    else if (undefinedWord) text = `.4byte ${undefinedWord[1]}`;
    else text = text.replace(/^\.word\s+(0x[0-9a-f]{8})$/, ".4byte $1");
    // ARMv4T reserves PC writeback forms of halfword and signed loads. GNU
    // objdump prints a mnemonic for them, but GAS correctly refuses to emit
    // that architecturally-invalid spelling. Retain the explicitly bounded
    // instruction word so the reconstructed helper stays byte exact.
    if (/^ldr(?:h|sh|sb)[a-z]*\s+r\d+,\s*\[pc\],/.test(text) ||
        /^tst[a-z]*\s+r\d+,\s+r\d+,\s+lsl r\d+$/.test(text)) {
      text = `.4byte 0x${byAddress.get(address)!.word}`;
    }
    text = text.replace(BRANCH, (whole, mnemonic: string, targetText: string) => {
      const target = Number.parseInt(targetText, 16);
      const namedTarget = named.get(target);
      if (namedTarget) return `${mnemonic} ${namedTarget}`;
      if (target < base || target >= end) return whole;
      return `${mnemonic} ${localLabel(target)}`;
    });
    output.push(`\t${text}`);
  }
  return output;
}

export function armSource(data: Buffer, base: number, end: number, names: ReadonlyMap<number, string>): string {
  const rows = objdumpRows(data, base);
  if (rows.length !== data.length / 4) throw new Error("ARM fragment disassembly is incomplete");
  return `${[
    ".syntax unified",
    ".text",
    "\t.arm",
    ...[...names.values()].map((name) => `\t.global ${name}`),
    ...renderRows(rows, base, end, names),
  ].join("\n")}\n`;
}

function tenkaiSource(data: Buffer): string {
  if (data.length !== SENTOU_KOUKA_END - TENKAI_ADDRESS) throw new Error("packed runtime extent differs");
  if (data.readUInt32LE(4) !== 0x080f03f1 || data.readUInt32LE(0xf0) !== 0x80808080 || data.readUInt32LE(0xf4) !== 0x080f0124)
    throw new Error("packed runtime anchors differ");
  if (data.subarray(0x104, 0x124).some(Boolean)) throw new Error("packed runtime dispatch workspace is not zero");
  const cases = [0x080f0144, 0x080f0160, 0x080f0200, 0x080f01b0, 0x080f0144, 0x080f0188, 0x080f0230, 0x080f01d8];
  cases.forEach((address, index) => {
    if (data.readUInt32LE(0x124 + index * 4) !== address) throw new Error("packed runtime dispatch source differs");
  });
  const rows = objdumpRows(data.subarray(8), TENKAI_ADDRESS + 8), named = new Map<number, string>([
    [0x080f0008, "SentouKouka_SekiWa"],
    [0x080f0024, "SentouKouka_Tenkai"],
    [0x080f00f8, "SentouKouka_TenkaiYomi"],
    [0x080f0144, ".L_case_2"],
    [0x080f0160, ".L_case_4_plus"],
    [0x080f0188, ".L_case_4_minus"],
    [0x080f01b0, ".L_case_6_plus"],
    [0x080f01d8, ".L_case_6_minus"],
    [0x080f0200, ".L_case_8"],
    [0x080f0230, ".L_case_10"],
  ]);
  const firstRows = rows.filter((row) => 0x080f0008 <= row.address && row.address < 0x080f00f0);
  const dispatchRows = rows.filter((row) => 0x080f00f8 <= row.address && row.address < 0x080f0104);
  const caseRows = rows.filter((row) => 0x080f0144 <= row.address && row.address < SENTOU_KOUKA_END);
  const output = [
    ".syntax unified",
    ".text",
    "\t.set Func_080f03f0, 0x080f03f0",
    "\t.thumb",
    "\t.global SentouKouka_Tobikomi",
    "\t.thumb_func",
    "SentouKouka_Tobikomi:",
    "\tldr r4, [pc, #0]",
    "\tbx r4",
    "\t.4byte Func_080f03f0 + 1",
    "\t.arm",
    "\t.global SentouKouka_SekiWa",
    "\t.global SentouKouka_Tenkai",
    "\t.global SentouKouka_TenkaiYomi",
    ...renderRows(firstRows, 0x080f0008, 0x080f00f0, named),
    ".L_bias:",
    "\t.4byte 0x80808080",
    ".L_dispatch_base:",
    "\t.4byte .L_dispatch_source",
    ...renderRows(dispatchRows, 0x080f00f8, 0x080f0104, named),
    ".L_dispatch_runtime:",
    "\t.space 32, 0",
    ".L_dispatch_source:",
    ...cases.map((address) => `\t.4byte ${named.get(address)}`),
    ...renderRows(caseRows, 0x080f0144, SENTOU_KOUKA_END, named),
  ];
  return `${output.join("\n")}\n`;
}

function validateAssembly(
  text: string,
  name: string,
  required: readonly string[],
  allowData: boolean,
  allowRawWords = false,
): void {
  if (!text.endsWith("\n") || text.includes("\r") || text.includes("\0")) throw new Error(`${name} is not canonical text`);
  if (/[;@]|\/\*|\/\//.test(text)) throw new Error(`${name} contains a comment`);
  const directive = /^\.(?:syntax unified|text)$|^\.(?:arm|thumb|thumb_func)$|^\.global [A-Za-z_][A-Za-z0-9_]*$/;
  for (const line of text.split("\n")) {
    const trimmed = line.trim();
    if (!trimmed || /^[A-Za-z_.][A-Za-z0-9_.]*:$/.test(trimmed)) continue;
    if (trimmed.includes(":")) throw new Error(`${name} contains a compound source line`);
    if (!trimmed.startsWith(".")) {
      if (!line.startsWith("\t")) throw new Error(`${name} contains noncanonical instruction spacing`);
      continue;
    }
    const data = allowData && (/^\.set Func_080f03f0, 0x080f03f0$/.test(trimmed) ||
      (allowRawWords && /^\.4byte 0x[0-9a-f]{8}$/.test(trimmed)) ||
      /^\.4byte (?:Func_080f03f0 \+ 1|\.L_[A-Za-z0-9_]+|0x80808080)$/.test(trimmed) ||
      /^\.space 32, 0$/.test(trimmed));
    if (!directive.test(trimmed) && !data) throw new Error(`${name} contains an unsupported directive`);
  }
  if (/\bb(?:l|eq|ne|cs|cc|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|hs|lo)?\s+0x[0-9a-f]+\b/.test(text))
    throw new Error(`${name} contains a numeric branch target`);
  for (const symbol of required) {
    const escaped = symbol.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
    const count = text.match(new RegExp(`^${escaped}:$`, "gm"))?.length ?? 0;
    if (count !== 1) throw new Error(`${name} symbol ${symbol} differs`);
  }
}

export function assembleAt(
  sourcePath: string,
  base: number,
  expectedSize: number,
  required: readonly string[],
  allowData = false,
  allowRawWords = false,
): Buffer {
  const text = readFileSync(sourcePath, "utf8");
  validateAssembly(text, sourcePath, required, allowData, allowRawWords);
  const work = mkdtempSync(join(TMPDIR, "sentou-kouka-assemble-"));
  try {
    const object = join(work, "source.o"), elf = join(work, "source.elf"), binary = join(work, "source.bin");
    const assembled = Bun.spawnSync(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", object, sourcePath], { stdout: "pipe", stderr: "pipe" });
    if (assembled.exitCode !== 0) throw new Error(assembled.stderr.toString().trim());
    const linked = Bun.spawnSync(["arm-none-eabi-ld", `-Ttext=${hexadecimal(base)}`, "-o", elf, object], { stdout: "pipe", stderr: "pipe" });
    if (linked.exitCode !== 0) throw new Error(linked.stderr.toString().trim());
    const copied = Bun.spawnSync(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary], { stdout: "pipe", stderr: "pipe" });
    if (copied.exitCode !== 0) throw new Error(copied.stderr.toString().trim());
    const output = readFileSync(binary);
    if (output.length !== expectedSize) throw new Error(`${sourcePath}: assembled size differs`);
    return output;
  } finally {
    rmSync(work, { recursive: true, force: true });
  }
}

function parseIndex(indexPath: string): Json {
  const index = document(indexPath);
  exactKeys(index, ["format", "kind", "address", "end", "size", "sources", "layout", "consumers", "zero_fill"], "battle effect runtime index");
  if (index.format !== 1 || index.kind !== "golden-sun-sentou-kouka-runtime" || index.address !== hexadecimal(SENTOU_KOUKA_ADDRESS) ||
      index.end !== hexadecimal(SENTOU_KOUKA_END) || index.size !== SENTOU_KOUKA_SIZE)
    throw new Error("battle effect runtime index extent differs");
  exactKeys(index.sources, ["keisu", "gousei", "hyou_a", "kansuu", "hyou_b", "gousei_iro", "bit_mask", "tenkai"], "battle effect runtime sources");
  const sources = {
    keisu: "keisu.json", gousei: "gousei.s", hyou_a: "hyou_a.json", kansuu: "kansuu_hyou.json",
    hyou_b: "hyou_b.json", gousei_iro: "gousei_iro.s", bit_mask: "bit_mask.json", tenkai: "tenkai.s",
  };
  for (const [key, value] of Object.entries(sources)) if (index.sources[key] !== value) throw new Error(`battle effect runtime ${key} source differs`);
  exactKeys(index.layout, ["keisu", "gousei", "hyou_a", "kansuu", "hyou_b", "gousei_iro", "bit_mask", "tenkai"], "battle effect runtime layout");
  const extents: Record<string, [number, number]> = {
    keisu: [KEISU_ADDRESS, GOUSEI_ADDRESS], gousei: [GOUSEI_ADDRESS, HYOU_A_ADDRESS], hyou_a: [HYOU_A_ADDRESS, KANSUU_ADDRESS],
    kansuu: [KANSUU_ADDRESS, HYOU_B_ADDRESS], hyou_b: [HYOU_B_ADDRESS, GOUSEI_IRO_ADDRESS],
    gousei_iro: [GOUSEI_IRO_ADDRESS, BIT_MASK_ADDRESS], bit_mask: [BIT_MASK_ADDRESS, ZERO_FILL_ADDRESS],
    tenkai: [TENKAI_ADDRESS, SENTOU_KOUKA_END],
  };
  for (const [key, [start, end]] of Object.entries(extents)) {
    exactKeys(index.layout[key], ["address", "size"], `battle effect runtime ${key} layout`);
    if (index.layout[key].address !== hexadecimal(start) || index.layout[key].size !== end - start)
      throw new Error(`battle effect runtime ${key} layout differs`);
  }
  exactKeys(index.consumers, ["fragment_composer", "callback_lookup", "packed_decoder_copy"], "battle effect runtime consumers");
  exactKeys(index.consumers.fragment_composer, ["symbol", "copy_sizes_words"], "fragment composer consumer");
  const copySizes = [2, 4, 25, 16, 6, 3, 97, 14];
  if (index.consumers.fragment_composer.symbol !== "Func_080ed408" || !Array.isArray(index.consumers.fragment_composer.copy_sizes_words) ||
      index.consumers.fragment_composer.copy_sizes_words.length !== copySizes.length ||
      index.consumers.fragment_composer.copy_sizes_words.some((value: unknown, position: number) => value !== copySizes[position]))
    throw new Error("fragment composer consumer differs");
  exactKeys(index.consumers.callback_lookup, ["symbol", "slots", "index_bias", "stride"], "callback lookup consumer");
  if (index.consumers.callback_lookup.symbol !== "Func_080d6660" || index.consumers.callback_lookup.slots !== CALLBACK_COUNT ||
      index.consumers.callback_lookup.index_bias !== -1 || index.consumers.callback_lookup.stride !== 4)
    throw new Error("callback lookup consumer differs");
  exactKeys(index.consumers.packed_decoder_copy, ["symbol", "source", "size"], "packed decoder copy consumer");
  if (index.consumers.packed_decoder_copy.symbol !== "Func_080f02b0" || index.consumers.packed_decoder_copy.source !== hexadecimal(0x080f0024) ||
      index.consumers.packed_decoder_copy.size !== 560)
    throw new Error("packed decoder copy consumer differs");
  exactKeys(index.zero_fill, ["value", "spans"], "battle effect runtime zero fill");
  const spans = [[ZERO_FILL_ADDRESS, ZERO_FILL_SPLIT], [ZERO_FILL_SPLIT, TENKAI_ADDRESS]];
  if (index.zero_fill.value !== 0 || !Array.isArray(index.zero_fill.spans) || index.zero_fill.spans.length !== spans.length)
    throw new Error("battle effect runtime zero fill differs");
  index.zero_fill.spans.forEach((span: unknown, position: number) => {
    exactKeys(span, ["address", "end", "size"], `battle effect runtime zero span ${position}`);
    const [start, end] = spans[position];
    if (span.address !== hexadecimal(start) || span.end !== hexadecimal(end) || span.size !== end - start)
      throw new Error(`battle effect runtime zero span ${position} differs`);
  });
  return index;
}

export function build_sentou_kouka_runtime(indexPath: string): Buffer {
  const index = parseIndex(indexPath), directory = dirname(indexPath);
  const prefix = basename(indexPath).replace(/index\.json$/, "");
  const gouseiLabels = [...GOUSEI_LABELS.values()], colorLabels = [...GOUSEI_IRO_LABELS.values()];
  const output = Buffer.concat([
    buildKeisu(join(directory, prefix + index.sources.keisu)),
    assembleAt(join(directory, prefix + index.sources.gousei), GOUSEI_ADDRESS, HYOU_A_ADDRESS - GOUSEI_ADDRESS, gouseiLabels),
    parseTableDocument(join(directory, prefix + index.sources.hyou_a), HYOU_A_SPECS, HYOU_A_ADDRESS, KANSUU_ADDRESS),
    buildCallbacks(join(directory, prefix + index.sources.kansuu)),
    parseTableDocument(join(directory, prefix + index.sources.hyou_b), HYOU_B_SPECS, HYOU_B_ADDRESS, GOUSEI_IRO_ADDRESS),
    assembleAt(join(directory, prefix + index.sources.gousei_iro), GOUSEI_IRO_ADDRESS, BIT_MASK_ADDRESS - GOUSEI_IRO_ADDRESS, colorLabels),
    buildMasks(join(directory, prefix + index.sources.bit_mask)),
    Buffer.alloc(TENKAI_ADDRESS - ZERO_FILL_ADDRESS),
    assembleAt(join(directory, prefix + index.sources.tenkai), TENKAI_ADDRESS, SENTOU_KOUKA_END - TENKAI_ADDRESS,
      ["SentouKouka_Tobikomi", "SentouKouka_SekiWa", "SentouKouka_Tenkai", "SentouKouka_TenkaiYomi"], true),
  ]);
  if (output.length !== SENTOU_KOUKA_SIZE) throw new Error("battle effect runtime output size differs");
  return output;
}

function writePackage(rom: Buffer, directory: string): void {
  const start = SENTOU_KOUKA_ADDRESS - ROM_BASE, end = SENTOU_KOUKA_END - ROM_BASE;
  if (start < 0 || end > rom.length) throw new Error("ROM is too small for battle effect runtime");
  const data = rom.subarray(start, end), slice = (address: number, sectionEnd: number) => data.subarray(address - SENTOU_KOUKA_ADDRESS, sectionEnd - SENTOU_KOUKA_ADDRESS);
  const zero = slice(ZERO_FILL_ADDRESS, TENKAI_ADDRESS);
  if (zero.some(Boolean)) throw new Error("battle effect runtime zero fill is not structural");
  mkdirSync(directory, { recursive: true });
  writeFileSync(join(directory, "keisu.json"), pretty(exportKeisu(slice(KEISU_ADDRESS, GOUSEI_ADDRESS))));
  writeFileSync(join(directory, "gousei.s"), armSource(slice(GOUSEI_ADDRESS, HYOU_A_ADDRESS), GOUSEI_ADDRESS, HYOU_A_ADDRESS, GOUSEI_LABELS));
  writeFileSync(join(directory, "hyou_a.json"), pretty(exportTableDocument(slice(HYOU_A_ADDRESS, KANSUU_ADDRESS), HYOU_A_SPECS, HYOU_A_ADDRESS, KANSUU_ADDRESS)));
  writeFileSync(join(directory, "kansuu_hyou.json"), pretty(exportCallbacks(slice(KANSUU_ADDRESS, HYOU_B_ADDRESS))));
  writeFileSync(join(directory, "hyou_b.json"), pretty(exportTableDocument(slice(HYOU_B_ADDRESS, GOUSEI_IRO_ADDRESS), HYOU_B_SPECS, HYOU_B_ADDRESS, GOUSEI_IRO_ADDRESS)));
  writeFileSync(join(directory, "gousei_iro.s"), armSource(slice(GOUSEI_IRO_ADDRESS, BIT_MASK_ADDRESS), GOUSEI_IRO_ADDRESS, BIT_MASK_ADDRESS, GOUSEI_IRO_LABELS));
  writeFileSync(join(directory, "bit_mask.json"), pretty(exportMasks(slice(BIT_MASK_ADDRESS, ZERO_FILL_ADDRESS))));
  writeFileSync(join(directory, "tenkai.s"), tenkaiSource(slice(TENKAI_ADDRESS, SENTOU_KOUKA_END)));
  const layout = (address: number, sectionEnd: number) => ({ address: hexadecimal(address), size: sectionEnd - address });
  writeFileSync(join(directory, "index.json"), pretty({
    format: 1,
    kind: "golden-sun-sentou-kouka-runtime",
    address: hexadecimal(SENTOU_KOUKA_ADDRESS),
    end: hexadecimal(SENTOU_KOUKA_END),
    size: SENTOU_KOUKA_SIZE,
    sources: {
      keisu: "keisu.json", gousei: "gousei.s", hyou_a: "hyou_a.json", kansuu: "kansuu_hyou.json",
      hyou_b: "hyou_b.json", gousei_iro: "gousei_iro.s", bit_mask: "bit_mask.json", tenkai: "tenkai.s",
    },
    layout: {
      keisu: layout(KEISU_ADDRESS, GOUSEI_ADDRESS),
      gousei: layout(GOUSEI_ADDRESS, HYOU_A_ADDRESS),
      hyou_a: layout(HYOU_A_ADDRESS, KANSUU_ADDRESS),
      kansuu: layout(KANSUU_ADDRESS, HYOU_B_ADDRESS),
      hyou_b: layout(HYOU_B_ADDRESS, GOUSEI_IRO_ADDRESS),
      gousei_iro: layout(GOUSEI_IRO_ADDRESS, BIT_MASK_ADDRESS),
      bit_mask: layout(BIT_MASK_ADDRESS, ZERO_FILL_ADDRESS),
      tenkai: layout(TENKAI_ADDRESS, SENTOU_KOUKA_END),
    },
    consumers: {
      fragment_composer: { symbol: "Func_080ed408", copy_sizes_words: [2, 4, 25, 16, 6, 3, 97, 14] },
      callback_lookup: { symbol: "Func_080d6660", slots: CALLBACK_COUNT, index_bias: -1, stride: 4 },
      packed_decoder_copy: { symbol: "Func_080f02b0", source: hexadecimal(0x080f0024), size: 560 },
    },
    zero_fill: {
      value: 0,
      spans: [
        { address: hexadecimal(ZERO_FILL_ADDRESS), end: hexadecimal(ZERO_FILL_SPLIT), size: ZERO_FILL_SPLIT - ZERO_FILL_ADDRESS },
        { address: hexadecimal(ZERO_FILL_SPLIT), end: hexadecimal(TENKAI_ADDRESS), size: TENKAI_ADDRESS - ZERO_FILL_SPLIT },
      ],
    },
  }));
  const rebuilt = build_sentou_kouka_runtime(join(directory, "index.json"));
  if (!rebuilt.equals(data)) throw new Error("exported battle effect runtime does not round-trip");
}

export function export_sentou_kouka_runtime(rom: Buffer, directory: string): void {
  const destination = resolve(directory), parent = dirname(destination);
  if (destination === parent) throw new Error("battle effect runtime export requires a dedicated directory");
  if (existsSync(destination)) parseIndex(join(destination, "index.json"));
  mkdirSync(parent, { recursive: true });
  const transaction = mkdtempSync(join(parent, ".sentou-kouka-export-"));
  const staged = join(transaction, "new"), previous = join(transaction, "previous");
  let installed = false;
  try {
    writePackage(rom, staged);
    if (existsSync(destination)) renameSync(destination, previous);
    try {
      renameSync(staged, destination);
      installed = true;
    } catch (error) {
      if (existsSync(previous)) renameSync(previous, destination);
      throw error;
    }
  } finally {
    if (!installed && existsSync(previous) && !existsSync(destination)) renameSync(previous, destination);
    rmSync(transaction, { recursive: true, force: true });
  }
}

export function verify_sentou_kouka_runtime(rom: Buffer, indexPath: string): void {
  const expected = rom.subarray(SENTOU_KOUKA_ADDRESS - ROM_BASE, SENTOU_KOUKA_END - ROM_BASE);
  const built = build_sentou_kouka_runtime(indexPath);
  if (!built.equals(expected)) {
    let offset = 0;
    while (offset < built.length && built[offset] === expected[offset]) offset++;
    throw new Error(`battle effect runtime differs at ${hexadecimal(SENTOU_KOUKA_ADDRESS + offset)}`);
  }
}

export function selfTest(): void {
  const temporary = mkdtempSync(join(TMPDIR, "sentou-kouka-self-test-"));
  let rejected = 0;
  const reject = (action: () => void): void => { try { action(); } catch { rejected++; } };
  try {
    reject(() => exactKeys({ format: 1, extra: true }, ["format"], "test source"));
    reject(() => encodeValues([256], "u8", 1, "test u8"));
    reject(() => encodeValues([1.5], "s32", 1, "test s32"));
    reject(() => callbackWord("Func_080c91dd", 0));
    reject(() => callbackWord("sub_080c91dc", 0));
    reject(() => callbackWord("Func_080edaf0", 0));
    reject(() => validateAssembly(".syntax unified\n.incbin \"x.bin\"\n", "test assembly", [], true));
    reject(() => validateAssembly(".syntax unified\n.byte 1\n", "test assembly", [], true));
    reject(() => validateAssembly(".syntax unified\n.long 1\n", "test assembly", [], true));
    reject(() => validateAssembly(".syntax unified\n.4byte 0x12345678\n", "test assembly", [], true));
    reject(() => validateAssembly(".syntax unified\nHidden: .long 1\n", "test assembly", [], true));
    reject(() => validateAssembly(".syntax unified\n@ 注釈\n", "test assembly", [], true));
    reject(() => validateAssembly(".syntax unified\n\tb 0x08000000\n", "test assembly", [], true));
    reject(() => validateAssembly(".syntax unified\n", "test assembly", ["Missing"], true));
    const duplicate = join(temporary, "duplicate.json");
    writeFileSync(duplicate, "{\"format\":0,\"format\":1}\n");
    reject(() => document(duplicate));
    if (rejected !== 15) throw new Error("adversarial source validation failed");
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok");
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { selfTest(); return; }
  if (args[0] === "export" && args.length === 4 && args[2] === "--directory") {
    export_sentou_kouka_runtime(readFileSync(args[1]), args[3]);
  } else if (args[0] === "verify" && args.length === 3) {
    verify_sentou_kouka_runtime(readFileSync(args[1]), args[2]);
    console.log(`address=${hexadecimal(SENTOU_KOUKA_ADDRESS)} bytes=${SENTOU_KOUKA_SIZE} exact=true`);
  } else {
    throw new Error("usage: sentou_kouka_runtime.ts export ROM --directory DIR | verify ROM INDEX | --self-test");
  }
}

if (import.meta.main) main(process.argv.slice(2));
