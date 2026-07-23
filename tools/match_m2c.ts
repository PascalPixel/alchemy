#!/usr/bin/env bun
// Tool role: both; imported by tools/decomp_diagnose.ts, tools/decomp_module.ts, tools/decomp_queue.ts (+7 more); invoked by PLAYBOOK.md.
import { canonicalJson } from "./canonical_json.ts";
import {
  existsSync,
  mkdirSync,
  readFileSync,
  readdirSync,
  writeFileSync,
} from "node:fs";
import { basename, dirname, extname, join } from "node:path";
import {
  cflagsForTargetSource,
  compilerCommandForTargetSource,
  externalSymbol,
  externalSymbolAssembly,
  type CompilerTarget,
} from "./alchemy_gcc.ts";
import {
  DEFAULT_TARGET,
  decompTarget,
  parseDecompTarget,
  type DecompTargetId,
} from "./decomp_targets.ts";

export const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
export const ROM_BASE = 0x08000000;
export const M2C_PREAMBLE = `typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

`;

const REJECT = ["M2C_ERROR", "M2C_BITFIELD", "M2C_MEMSET", "M2C_MEMCPY"];

interface Options {
  target: DecompTargetId;
  rom: string;
  drafts: string;
  jobs: number;
}

export interface Verification {
  actual: Buffer;
  expected: Buffer;
  size: number;
}

type JsonRecord = Record<string, unknown>;

class CalledProcessError extends Error {}
class ValueError extends Error {}
class StopIteration extends Error {}

function parseHex(value: string): number {
  if (!/^[0-9a-f]+$/i.test(value)) throw new ValueError(`invalid hexadecimal value: ${value}`);
  return Number.parseInt(value, 16);
}

async function run(command: string[], cwd = ROOT): Promise<string> {
  const child = Bun.spawn(command, { cwd, stdout: "pipe", stderr: "pipe" });
  const stdout = new Response(child.stdout).text();
  const stderr = new Response(child.stderr).text();
  const [code, output, error] = await Promise.all([child.exited, stdout, stderr]);
  if (code !== 0) {
    const detail = (error || output).trim();
    throw new CalledProcessError(`${basename(command[0])} failed${detail ? `: ${detail}` : ""}`);
  }
  return output;
}

function sourceStem(path: string): string {
  return basename(path, extname(path));
}

export async function verifyCandidate(
  source: string,
  rom: Uint8Array,
  outputDirectory: string,
  extraCompilerFlags: readonly string[] = [],
  imageBase = ROM_BASE,
  compiler: CompilerTarget = "gs1",
): Promise<Verification> {
  const stem = sourceStem(source);
  const address = parseHex(stem);
  const symbol = `Func_${address.toString(16).padStart(8, "0")}`;
  const assembly = join(outputDirectory, `${stem}.s`);
  const object = join(outputDirectory, `${stem}.o`);
  const symbolsSource = join(outputDirectory, `${stem}.symbols.s`);
  const symbolsObject = join(outputDirectory, `${stem}.symbols.o`);
  const elf = join(outputDirectory, `${stem}.elf`);
  const binary = join(outputDirectory, `${stem}.bin`);
  await run(compilerCommandForTargetSource(
    compiler,
    source,
    ...cflagsForTargetSource(compiler, source),
    ...extraCompilerFlags,
    "-S", "-o", assembly, source,
  ));
  await run([
    "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
    "-o", object, assembly,
  ]);
  const names: string[] = [];
  const undefinedSymbols = await run(["arm-none-eabi-nm", "-u", object]);
  for (const line of undefinedSymbols.split(/\r?\n/).filter(Boolean)) {
    const external = line.trim().split(/\s+/).at(-1)!;
    if (externalSymbol(external) === null) throw new ValueError(`unsupported external symbol: ${external}`);
    names.push(external);
  }
  writeFileSync(symbolsSource, ".syntax unified\n.thumb\n" + names.map(externalSymbolAssembly).join(""));
  await run([
    "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
    "-o", symbolsObject, symbolsSource,
  ]);
  await run([
    "arm-none-eabi-ld", `-Ttext=0x${address.toString(16).padStart(8, "0")}`,
    "-e", symbol, "-o", elf, object, symbolsObject,
  ]);
  await run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary]);
  const symbols = await run(["arm-none-eabi-nm", "-S", elf]);
  const row = symbols.split(/\r?\n/).find((line) => line.endsWith(` ${symbol}`));
  if (row === undefined) throw new StopIteration(`missing linked symbol: ${symbol}`);
  const size = parseHex(row.trim().split(/\s+/)[1]);
  const actual = readFileSync(binary).subarray(0, size);
  const expected = Buffer.from(rom).subarray(address - imageBase, address - imageBase + size);
  return { actual, expected, size };
}

async function parallelMap<T, R>(items: T[], jobs: number, operation: (item: T) => Promise<R>): Promise<R[]> {
  const results = new Array<R>(items.length);
  let next = 0;
  async function worker(): Promise<void> {
    while (true) {
      const index = next++;
      if (index >= items.length) return;
      results[index] = await operation(items[index]);
    }
  }
  await Promise.all(Array.from({ length: Math.min(jobs, items.length) }, worker));
  return results;
}

function usage(): void {
  console.log("usage: match_m2c.ts [-h] [--target gs1-en|gs2-en] [--jobs JOBS] rom drafts");
}

function parseArguments(arguments_: string[]): Options {
  const positional: string[] = [];
  let jobs = Math.min(16, navigator.hardwareConcurrency || 1);
  let target: DecompTargetId = DEFAULT_TARGET;
  for (let index = 0; index < arguments_.length; index++) {
    const argument = arguments_[index];
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    }
    if (argument === "--jobs" || argument.startsWith("--jobs=")) {
      const value = argument.includes("=") ? argument.slice(argument.indexOf("=") + 1) : arguments_[++index];
      if (value === undefined) throw new Error("argument --jobs: expected one argument");
      jobs = Number.parseInt(value, 10);
      if (!Number.isInteger(jobs) || jobs < 1) throw new Error(`invalid int value: ${value}`);
      continue;
    }
    if (argument === "--target" || argument.startsWith("--target=")) {
      const value = argument.includes("=") ? argument.slice(argument.indexOf("=") + 1) : arguments_[++index];
      if (value === undefined) throw new Error("argument --target: expected one argument");
      target = parseDecompTarget(value);
      continue;
    }
    if (argument.startsWith("-")) throw new Error(`unrecognized argument: ${argument}`);
    positional.push(argument);
  }
  if (positional.length !== 2) throw new Error("the following arguments are required: rom, drafts");
  return { target, rom: positional[0], drafts: positional[1], jobs };
}

async function main(): Promise<void> {
  const options = parseArguments(Bun.argv.slice(2));
  const target = decompTarget(options.target);
  const rom = readFileSync(options.rom);
  if (rom.length !== target.romSize) {
    throw new Error(`${target.id} ROM must contain exactly ${target.romSize} bytes`);
  }
  const candidateDirectory = target.id === DEFAULT_TARGET
    ? join(ROOT, "work/matches/m2c")
    : join(ROOT, "work/matches", target.id, "m2c");
  const outputDirectory = target.id === DEFAULT_TARGET
    ? join(ROOT, "out/m2c")
    : join(ROOT, target.outputDir, "m2c");
  mkdirSync(candidateDirectory, { recursive: true });
  mkdirSync(outputDirectory, { recursive: true });
  const tracked = new Set(readdirSync(join(ROOT, target.sourceDir)).filter((name) => name.endsWith(".c")));
  const draftReport = join(options.drafts, "report.json");
  const spans = new Map<number, number>();
  if (existsSync(draftReport)) {
    const rows = JSON.parse(readFileSync(draftReport, "utf8")) as JsonRecord[];
    for (const row of rows) spans.set(Number(row.entry), Number(row.size));
  }
  const drafts = readdirSync(options.drafts)
    .filter((name) => name.endsWith(".c"))
    .sort()
    .filter((name) => !tracked.has(name))
    .map((name) => join(options.drafts, name))
    .filter((path) => {
      const source = readFileSync(path, "utf8");
      return !REJECT.some((marker) => source.includes(marker));
    });

  const results = await parallelMap(drafts, options.jobs, async (draft): Promise<JsonRecord> => {
    const name = basename(draft);
    const stem = sourceStem(draft);
    const entry = parseHex(stem);
    const candidate = join(candidateDirectory, name);
    const functionOutput = join(outputDirectory, stem);
    mkdirSync(functionOutput, { recursive: true });
    const source = readFileSync(draft, "utf8");
    const strategies: Array<string | null> = source.includes("M2C_UNK")
      ? ["void", "s32", "u32", "void *", "s16", "u16", "s8", "u8"]
      : [null];
    let best: {
      score: [number, number, number];
      replacement: string | null;
      body: string;
      size: number;
      prefix: number;
    } | null = null;
    for (const replacement of strategies) {
      const body = replacement === null ? source : source.replaceAll("M2C_UNK", replacement);
      writeFileSync(candidate, M2C_PREAMBLE + body);
      let verification: Verification;
      try {
        verification = await verifyCandidate(
          candidate,
          rom,
          functionOutput,
          [],
          ROM_BASE,
          target.compiler,
        );
      } catch {
        continue;
      }
      const { actual, expected, size } = verification;
      let mismatch = 0;
      for (let index = 0; index < Math.min(actual.length, expected.length); index++) {
        if (actual[index] !== expected[index]) mismatch++;
      }
      const span = spans.get(entry) ?? 0;
      mismatch += Math.max(0, span - actual.length);
      let prefix = 0;
      while (prefix < Math.min(actual.length, expected.length) && actual[prefix] === expected[prefix]) prefix++;
      const score: [number, number, number] = [mismatch, -prefix, Math.abs(size - span)];
      if (best === null || compareTuple(score, best.score) < 0) {
        best = { score, replacement, body, size, prefix };
      }
      if (actual.equals(expected)) {
        return { entry, matched: true, size, source: candidate, unknown_type: replacement };
      }
    }
    if (best === null) return { entry, matched: false };
    writeFileSync(candidate, M2C_PREAMBLE + best.body);
    return {
      entry,
      matched: false,
      size: best.size,
      mismatched_bytes: best.score[0],
      common_prefix: best.prefix,
      unknown_type: best.replacement,
    };
  });
  const reportPath = target.id === DEFAULT_TARGET
    ? join(ROOT, "work/matches/m2c.json")
    : join(ROOT, "work/matches", `${target.id}-m2c.json`);
  writeFileSync(reportPath, canonicalJson(results) + "\n");
  const matches = results.filter((item) => item.matched === true).length;
  console.log(`candidates=${results.length} matches=${matches} failures=${results.length - matches}`);
}

function compareTuple(left: [number, number, number], right: [number, number, number]): number {
  for (let index = 0; index < left.length; index++) {
    if (left[index] !== right[index]) return left[index] - right[index];
  }
  return 0;
}

if (import.meta.main) await main();
