#!/usr/bin/env bun
// Tool role: library; imported by tools/permute_m2c.ts, tools/permute_v1.ts.
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, dirname, extname, join } from "node:path";
import { cflagsForSource, compilerCommand, externalSymbol, externalSymbolAssembly } from "./alchemy_gcc.ts";

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
): [Buffer, Buffer, number];
export function verify(
  source: string,
  rom: Uint8Array,
  outputDir: string,
  details?: false,
): [boolean, number];
export function verify(
  source: string,
  rom: Uint8Array,
  outputDir: string,
  details = false,
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
  run(compilerCommand(...cflagsForSource(source), "-S", "-o", assembly, source));
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
  if (Bun.argv.slice(2).some((argument) => argument === "-h" || argument === "--help")) {
    console.log("usage: verify.ts [-h]");
    return;
  }
  if (Bun.argv.length > 2) throw new Error(`unrecognized argument: ${Bun.argv[2]}`);
  const rom = readFileSync(join(ROOT, "roms", "gs1-en.gba"));
  const outputDir = join(ROOT, "out/verify");
  mkdirSync(outputDir, { recursive: true });
  const sources = readdirSync(join(ROOT, "src"), { withFileTypes: true })
    .filter((entry) => entry.isFile() && entry.name.endsWith(".c"))
    .map((entry) => join(ROOT, "src", entry.name)).sort();
  const failures: string[] = [];
  let total = 0;
  for (const source of sources) {
    const [matched, size] = verify(source, rom, outputDir);
    total += size;
    if (!matched) failures.push(basename(source));
  }
  console.log(`verified=${sources.length - failures.length} failures=${failures.length} bytes=${total}`);
  if (failures.length !== 0) process.exitCode = 1;
}

if (import.meta.main) main();
