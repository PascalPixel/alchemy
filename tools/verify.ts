#!/usr/bin/env bun
// Tool role: library; imported by tools/permute_m2c.ts, tools/permute_v1.ts.
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, dirname, extname, join } from "node:path";
import {
  cflagsForTargetSource,
  compilerCommandForTarget,
  externalSymbol,
  externalSymbolAssembly,
  type CompilerTarget,
} from "./alchemy_gcc.ts";
import { DEFAULT_TARGET, decompTarget, parseDecompTarget } from "./decomp_targets.ts";

export const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
export const ROM_BASE = 0x08000000;

function text(value: Uint8Array): string {
  return Buffer.from(value).toString("utf8");
}

function run(command: string[], quiet = true): string {
  const result = Bun.spawnSync(command, {
    cwd: ROOT,
    stdout: "pipe",
    stderr: "pipe",
  });
  if (result.exitCode !== 0) {
    const detail = (text(result.stderr) || text(result.stdout)).trim();
    if (quiet) throw new Error(`${basename(command[0])} failed`);
    throw new Error(`${basename(command[0])} failed: ${detail}`);
  }
  return text(result.stdout);
}

function stem(path: string): string {
  return basename(path, extname(path));
}

export function verify(
  source: string,
  rom: Uint8Array,
  outputDir: string,
  details: true,
  compiler?: CompilerTarget,
): [Buffer, Buffer, number];
export function verify(
  source: string,
  rom: Uint8Array,
  outputDir: string,
  details?: false,
  compiler?: CompilerTarget,
): [boolean, number];
export function verify(
  source: string,
  rom: Uint8Array,
  outputDir: string,
  details = false,
  compiler: CompilerTarget = "gs1",
): [Buffer, Buffer, number] | [boolean, number] {
  const name = stem(source);
  const address = Number.parseInt(name, 16);
  const symbol = `Func_${address.toString(16).padStart(8, "0")}`;
  const assembly = join(outputDir, `${name}.s`);
  const object = join(outputDir, `${name}.o`);
  const symbolsSource = join(outputDir, `${name}.symbols.s`);
  const symbolsObject = join(outputDir, `${name}.symbols.o`);
  const elf = join(outputDir, `${name}.elf`);
  const binary = join(outputDir, `${name}.bin`);
  run(compilerCommandForTarget(
    compiler,
    ...cflagsForTargetSource(compiler, source),
    "-S", "-o", assembly, source,
  ));
  run([
    "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
    "-o", object, assembly,
  ]);
  const names: string[] = [];
  for (const line of run(["arm-none-eabi-nm", "-u", object]).split(/\r?\n/).filter(Boolean)) {
    const external = line.trim().split(/\s+/).at(-1)!;
    if (externalSymbol(external) === null) throw new Error(`unsupported external symbol: ${external}`);
    names.push(external);
  }
  writeFileSync(symbolsSource, ".syntax unified\n.thumb\n" + names.map(externalSymbolAssembly).join(""));
  run([
    "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
    "-o", symbolsObject, symbolsSource,
  ]);
  run([
    "arm-none-eabi-ld", `-Ttext=0x${address.toString(16).padStart(8, "0")}`,
    "-e", symbol, "-o", elf, object, symbolsObject,
  ]);
  run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary]);
  const symbols = run(["arm-none-eabi-nm", "-S", elf]);
  const row = symbols.split(/\r?\n/).find((line) => line.endsWith(` ${symbol}`));
  if (row === undefined) throw new Error(`missing linked symbol: ${symbol}`);
  const size = Number.parseInt(row.trim().split(/\s+/)[1], 16);
  const actual = readFileSync(binary).subarray(0, size);
  const expected = Buffer.from(rom).subarray(address - ROM_BASE, address - ROM_BASE + size);
  if (details) return [actual, expected, size];
  return [actual.equals(expected), size];
}

function main(): void {
  const arguments_ = Bun.argv.slice(2);
  if (arguments_.some((argument) => argument === "-h" || argument === "--help")) {
    console.log("usage: verify.ts [-h] [--target gs1-en|gs2-en]");
    return;
  }
  let targetId = DEFAULT_TARGET;
  for (let index = 0; index < arguments_.length; index++) {
    const argument = arguments_[index];
    if (argument === "--target") {
      const value = arguments_[++index];
      if (value === undefined) throw new Error("--target requires a value");
      targetId = parseDecompTarget(value);
    } else if (argument.startsWith("--target=")) {
      targetId = parseDecompTarget(argument.slice(9));
    } else {
      throw new Error(`unrecognized argument: ${argument}`);
    }
  }
  const target = decompTarget(targetId);
  const rom = readFileSync(join(ROOT, target.rom));
  if (rom.length !== target.romSize) {
    throw new Error(`${target.id} ROM must contain exactly ${target.romSize} bytes`);
  }
  const outputDir = join(ROOT, target.outputDir, "verify");
  mkdirSync(outputDir, { recursive: true });
  const sourceDirectory = join(ROOT, target.sourceDir);
  const sources = readdirSync(sourceDirectory, { withFileTypes: true })
    .filter((entry) => entry.isFile() && entry.name.endsWith(".c"))
    .map((entry) => join(sourceDirectory, entry.name)).sort();
  const failures: string[] = [];
  let total = 0;
  for (const source of sources) {
    const [matched, size] = verify(source, rom, outputDir, false, target.compiler);
    total += size;
    if (!matched) failures.push(basename(source));
  }
  console.log(`verified=${sources.length - failures.length} failures=${failures.length} bytes=${total}`);
  if (failures.length !== 0) process.exitCode = 1;
}

if (import.meta.main) main();
