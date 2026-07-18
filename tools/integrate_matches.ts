#!/usr/bin/env bun
import {
  copyFileSync,
  existsSync,
  mkdirSync,
  readFileSync,
  readdirSync,
  rmSync,
} from "node:fs";
import { basename, dirname, join } from "node:path";
import { CFLAGS, compilerCommand } from "./alchemy_gcc.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const HELPER = /inline_fn|^(static|inline)\b/m;

interface Options {
  directory: string;
  apply: boolean;
}

function outputText(value: Uint8Array): string {
  return Buffer.from(value).toString("utf8");
}

function run(command: string[], cwd = ROOT): { code: number; stdout: string } {
  const process = Bun.spawnSync(command, { cwd, stdout: "pipe", stderr: "pipe" });
  return { code: process.exitCode, stdout: outputText(process.stdout) };
}

function textSize(object: string): number | null {
  const listing = run(["arm-none-eabi-objdump", "-h", object]).stdout;
  const found = /\.text\s+([0-9a-f]+)/.exec(listing);
  return found === null ? null : Number.parseInt(found[1], 16);
}

function asmExtent(stem: string, scratch: string): number | null {
  const source = join(ROOT, "asm", `${stem}.s`);
  if (!existsSync(source)) return null;
  const listing = join(scratch, `${stem}.asmprobe.s`);
  const object = join(scratch, `${stem}.asmprobe.o`);
  copyFileSync(source, listing);
  const assembled = run([
    "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
    "-o", object, listing,
  ]);
  return assembled.code === 0 ? textSize(object) : null;
}

function cExtent(stem: string, candidate: string, scratch: string): number | null {
  const listing = join(scratch, `${stem}.cprobe.s`);
  const object = join(scratch, `${stem}.cprobe.o`);
  const compiled = run(compilerCommand(...CFLAGS, "-S", "-o", listing, candidate));
  if (compiled.code !== 0) return null;
  const assembled = run([
    "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
    "-o", object, listing,
  ]);
  return assembled.code === 0 ? textSize(object) : null;
}

function usage(): void {
  console.log("usage: integrate_matches.ts [-h] [--apply] directory");
}

function parseArguments(arguments_: string[]): Options {
  let directory: string | undefined;
  let apply = false;
  for (const argument of arguments_) {
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    }
    if (argument === "--apply") {
      apply = true;
      continue;
    }
    if (argument.startsWith("-")) throw new Error(`unrecognized argument: ${argument}`);
    if (directory !== undefined) throw new Error(`unrecognized argument: ${argument}`);
    directory = argument;
  }
  if (directory === undefined) throw new Error("the following arguments are required: directory");
  return { directory, apply };
}

function main(): void {
  const options = parseArguments(Bun.argv.slice(2));
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
    const wanted = asmExtent(stem, scratch);
    const got = cExtent(stem, candidate, scratch);
    if (wanted === null || got === null) {
      rejected.push([stem, `could not measure (asm=${wanted === null ? "None" : wanted} c=${got === null ? "None" : got})`]);
    } else if (wanted !== got) {
      rejected.push([stem, `INCOMPLETE: asm=${wanted}B c=${got}B`]);
    } else {
      accepted.push([stem, wanted]);
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
