#!/usr/bin/env bun
import {
  copyFileSync,
  existsSync,
  mkdirSync,
  readFileSync,
  readdirSync,
  rmSync,
  writeFileSync,
} from "node:fs";
import { basename, dirname, join } from "node:path";
import {
  cflagsForSource,
  compilerCommand,
  externalSymbol,
  externalSymbolAssembly,
} from "./alchemy_gcc.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const HELPER = /inline_fn|^(static|inline)\b/m;

interface Options {
  directory: string;
  apply: boolean;
  selfTest: boolean;
}

function outputText(value: Uint8Array): string {
  return Buffer.from(value).toString("utf8");
}

function run(command: string[], cwd = ROOT): { code: number; stdout: string; stderr: string } {
  const process = Bun.spawnSync(command, { cwd, stdout: "pipe", stderr: "pipe" });
  return {
    code: process.exitCode,
    stdout: outputText(process.stdout),
    stderr: outputText(process.stderr),
  };
}

function hexadecimal(value: number): string {
  return value.toString(16).padStart(8, "0");
}

function commandError(result: ReturnType<typeof run>): string {
  return (result.stderr || result.stdout).trim();
}

function linkedBytes(stem: string, source: string, scratch: string, kind: "asm" | "c"): Buffer {
  const address = Number.parseInt(stem, 16);
  if (!Number.isSafeInteger(address) || !/^08[0-9a-f]{6}$/.test(stem)) throw new Error("invalid source address");
  const prefix = join(scratch, `${stem}.${kind}probe`);
  const listing = `${prefix}.s`, object = `${prefix}.o`;
  if (kind === "c") {
    const compiled = run(compilerCommand(...cflagsForSource(source), "-S", "-o", listing, source));
    if (compiled.code !== 0) throw new Error(`compiler failed: ${commandError(compiled)}`);
  } else {
    copyFileSync(source, listing);
  }
  const assembled = run([
    "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
    "-o", object, listing,
  ]);
  if (assembled.code !== 0) throw new Error(`assembler failed: ${commandError(assembled)}`);
  const undefinedResult = run(["arm-none-eabi-nm", "-u", object]);
  if (undefinedResult.code !== 0) throw new Error(`nm failed: ${commandError(undefinedResult)}`);
  const names = undefinedResult.stdout.split(/\r?\n/).filter(Boolean)
    .map((line) => line.trim().split(/\s+/).at(-1)!);
  for (const name of names) {
    if (externalSymbol(name) === null) throw new Error(`unsupported external symbol ${name}`);
  }
  const symbolsSource = `${prefix}.symbols.s`, symbolsObject = `${prefix}.symbols.o`;
  writeFileSync(symbolsSource, ".syntax unified\n.thumb\n" + names.map(externalSymbolAssembly).join(""));
  const symbolsAssembled = run([
    "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
    "-o", symbolsObject, symbolsSource,
  ]);
  if (symbolsAssembled.code !== 0)
    throw new Error(`symbol assembler failed: ${commandError(symbolsAssembled)}`);
  const elf = `${prefix}.elf`, binary = `${prefix}.bin`, formatted = hexadecimal(address);
  const linked = run([
    "arm-none-eabi-ld", `-Ttext=0x${formatted}`, "-e", `Func_${formatted}`,
    "-o", elf, object, symbolsObject,
  ]);
  if (linked.code !== 0) throw new Error(`linker failed: ${commandError(linked)}`);
  const copied = run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary]);
  if (copied.code !== 0) throw new Error(`objcopy failed: ${commandError(copied)}`);
  const bytes = readFileSync(binary);
  if (kind === "asm") return bytes;
  const symbolResult = run(["arm-none-eabi-nm", "-S", "--defined-only", elf]);
  if (symbolResult.code !== 0) throw new Error(`nm failed: ${commandError(symbolResult)}`);
  const extent = linkedFunctionExtent(symbolResult.stdout, `Func_${formatted}`, address, bytes.length);
  return bytes.subarray(0, extent);
}

export function mismatch(left: Uint8Array, right: Uint8Array): { offset: number; left?: number; right?: number } | null {
  const shared = Math.min(left.length, right.length);
  for (let offset = 0; offset < shared; offset++) {
    if (left[offset] !== right[offset]) return { offset, left: left[offset], right: right[offset] };
  }
  return left.length === right.length ? null : { offset: shared, left: left[shared], right: right[shared] };
}

export function linkedFunctionExtent(output: string, target: string, address: number, byteLength: number): number {
  const functions = output.split(/\r?\n/).filter(Boolean)
    .map((line) => line.trim().split(/\s+/))
    .filter((fields) => fields.length >= 4 && /^[Tt]$/.test(fields.at(-2)!) && /^Func_[0-9a-f]{8}$/.test(fields.at(-1)!))
    .map((fields) => ({
      address: Number.parseInt(fields[0], 16),
      size: Number.parseInt(fields[1], 16),
      name: fields.at(-1)!,
    }))
    .filter((entry) => entry.address >= address && entry.address < address + byteLength);
  if (!functions.some((entry) => entry.name === target && entry.address === address) ||
      functions.some((entry) => !Number.isSafeInteger(entry.address) ||
        !Number.isSafeInteger(entry.size) || entry.size <= 0)) {
    throw new Error("compiled function symbols differ");
  }
  const end = Math.max(...functions.map((entry) => entry.address + entry.size));
  if (end <= address || end - address > byteLength) throw new Error("compiled function extent differs");
  return end - address;
}

function selfTest(): void {
  if (mismatch(Buffer.from([1, 2, 3]), Buffer.from([1, 2, 3])) !== null)
    throw new Error("identical source bytes differ");
  const changed = mismatch(Buffer.from([1, 2, 3]), Buffer.from([1, 4, 3]));
  if (changed?.offset !== 1 || changed.left !== 2 || changed.right !== 4)
    throw new Error("source-byte mismatch offset differs");
  const short = mismatch(Buffer.from([1]), Buffer.from([1, 2]));
  if (short?.offset !== 1 || short.left !== undefined || short.right !== 2)
    throw new Error("source-byte extent mismatch differs");
  const symbols = [
    "08021360 00000030 T Func_08021360",
    "080770c1 A Func_080770c0",
  ].join("\n");
  if (linkedFunctionExtent(symbols, "Func_08021360", 0x08021360, 48) !== 48) {
    throw new Error("absolute external symbol changed compiled extent");
  }
  console.log("self-test=ok");
}

function usage(): void {
  console.log("usage: integrate_matches.ts [-h] [--apply] directory | --self-test");
}

function parseArguments(arguments_: string[]): Options {
  let directory: string | undefined;
  let apply = false;
  let selfTest = false;
  for (const argument of arguments_) {
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    }
    if (argument === "--apply") {
      apply = true;
      continue;
    }
    if (argument === "--self-test") {
      selfTest = true;
      continue;
    }
    if (argument.startsWith("-")) throw new Error(`unrecognized argument: ${argument}`);
    if (directory !== undefined) throw new Error(`unrecognized argument: ${argument}`);
    directory = argument;
  }
  if (selfTest) {
    if (directory !== undefined || apply || arguments_.length !== 1) throw new Error("--self-test takes no other arguments");
    return { directory: "", apply: false, selfTest: true };
  }
  if (directory === undefined) throw new Error("the following arguments are required: directory");
  return { directory, apply, selfTest: false };
}

function main(): void {
  const options = parseArguments(Bun.argv.slice(2));
  if (options.selfTest) {
    selfTest();
    return;
  }
  const scratch = join(options.directory, "gate");
  mkdirSync(scratch, { recursive: true });
  const accepted: Array<[string, number]> = [];
  const rejected: Array<[string, string]> = [];
  const candidates = readdirSync(options.directory)
    .filter((name) => /^src_.*\.c$/.test(name))
    .sort();
  for (const name of candidates) {
    const candidate = join(options.directory, name);
    const stem = basename(name, ".c").replace("src_", "");
    if (existsSync(join(ROOT, "src", `${stem}.c`))) continue;
    if (HELPER.test(readFileSync(candidate, "utf8"))) {
      rejected.push([stem, "carries an m2c helper"]);
      continue;
    }
    const source = join(ROOT, "asm", `${stem}.s`);
    if (!existsSync(source)) continue;
    try {
      const wanted = linkedBytes(stem, source, scratch, "asm");
      const got = linkedBytes(stem, candidate, scratch, "c");
      const difference = mismatch(wanted, got);
      if (difference !== null) {
        rejected.push([stem, `bytes differ at +0x${difference.offset.toString(16)} (asm=${wanted.length}B c=${got.length}B)`]);
      } else {
        accepted.push([stem, wanted.length]);
      }
    } catch (error) {
      rejected.push([stem, error instanceof Error ? error.message : String(error)]);
    }
  }
  for (const [stem, size] of accepted) {
    console.log(`accept ${stem} (${size}B)`);
    if (options.apply) {
      copyFileSync(join(options.directory, `src_${stem}.c`), join(ROOT, "src", `${stem}.c`));
      rmSync(join(ROOT, "asm", `${stem}.s`), { force: true });
    }
  }
  for (const [stem, reason] of rejected) console.log(`reject ${stem}: ${reason}`);
  console.log(`accepted=${accepted.length} rejected=${rejected.length}${options.apply ? " (applied)" : " (dry run)"}`);
}

if (import.meta.main) main();
