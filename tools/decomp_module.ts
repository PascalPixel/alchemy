#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md, tools/decomp_overnight.ts.
// Compile adjacent drafts as one translation unit. This exposes shared literal
// pools and cross-function register/code-generation effects that per-function
// matching cannot represent. Installation is guarded by exact whole-module bytes
// and exact symbol addresses.
import { existsSync, mkdirSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { cflagsForSource, compilerCommand, externalSymbol, externalSymbolAssembly } from "./alchemy_gcc.ts";
import { M2C_PREAMBLE } from "./match_m2c.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;

async function run(command: string[]): Promise<string> {
  const child = Bun.spawn(command, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  const [stdout, stderr, code] = await Promise.all([
    new Response(child.stdout).text(), new Response(child.stderr).text(), child.exited,
  ]);
  if (code !== 0) throw new Error(`${basename(command[0])} failed: ${(stderr || stdout).trim()}`);
  return stdout;
}

interface Region { address: number; size: number; source: string; retention: string }

function regionsFor(stems: string[]): Region[] {
  const path = [join(ROOT, "out/full/asm/manifest.json"), join(ROOT, "out/asm/manifest.json")].find(existsSync);
  if (!path) throw new Error("missing assembly manifest; run tools/build_asm.ts first");
  const document = JSON.parse(readFileSync(path, "utf8")) as { regions: Region[] };
  const byStem = new Map(document.regions.map((region) => [basename(region.source, ".s"), region]));
  const regions = stems.map((stem) => {
    const region = byStem.get(stem);
    if (!region) throw new Error(`assembly manifest has no region ${stem}`);
    if (region.retention !== "c_candidate") throw new Error(`${stem} is retained structural assembly`);
    return region;
  });
  for (let index = 1; index < regions.length; index++) {
    const gap = regions[index].address - (regions[index - 1].address + regions[index - 1].size);
    if (gap < 0 || gap > 2) {
      throw new Error(`${stems[index - 1]} and ${stems[index]} are not contiguous`);
    }
  }
  return regions;
}

async function main(): Promise<void> {
  const arguments_ = Bun.argv.slice(2);
  const install = arguments_.includes("--install");
  const draftsIndex = arguments_.indexOf("--drafts");
  const drafts = draftsIndex >= 0 ? arguments_[draftsIndex + 1] : join(ROOT, "out/decomp/candidates");
  const stems = arguments_.filter((argument, index) => /^[0-9a-f]{8}$/.test(argument) &&
    !(draftsIndex >= 0 && index === draftsIndex + 1));
  if (stems.length < 2) throw new Error("usage: decomp_module.ts [--drafts DIR] [--install] STEM STEM [STEM ...]");
  const sorted = [...stems].sort();
  if (sorted.some((stem, index) => stem !== stems[index])) throw new Error("module stems must be in address order");
  const regions = regionsFor(stems);
  const sources = stems.map((stem) => {
    const path = join(drafts, `${stem}.c`);
    if (!existsSync(path)) throw new Error(`missing candidate ${path}`);
    return readFileSync(path, "utf8");
  });
  const headerLines = new Set<string>();
  const bodies: string[] = [];
  sources.forEach((raw, index) => {
    const source = index === 0 && !raw.startsWith("typedef") ? M2C_PREAMBLE + raw : raw;
    const definition = new RegExp(`^.*\\bFunc_${stems[index]}\\s*\\([^;]*\\)\\s*\\{`, "m").exec(source);
    if (definition === null || definition.index === undefined) throw new Error(`cannot find definition Func_${stems[index]}`);
    for (const line of source.slice(0, definition.index).split(/\r?\n/)) if (line.trim() !== "") headerLines.add(line);
    bodies.push(source.slice(definition.index));
  });
  const combined = [...headerLines].join("\n") + "\n\n" + bodies.join("\n");
  const scratch = join(ROOT, "out/decomp/modules", stems.join("-"));
  mkdirSync(scratch, { recursive: true });
  const prefix = join(scratch, stems[0]);
  const cFile = `${prefix}.c`, assembly = `${prefix}.s`, object = `${prefix}.o`;
  const symbolsSource = `${prefix}.symbols.s`, symbolsObject = `${prefix}.symbols.o`;
  const elf = `${prefix}.elf`, binary = `${prefix}.bin`;
  writeFileSync(cFile, combined);
  await run(compilerCommand(...cflagsForSource(cFile), "-S", "-o", assembly, cFile));
  await run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", object, assembly]);
  const undefinedNames = (await run(["arm-none-eabi-nm", "-u", object])).split(/\r?\n/).filter(Boolean)
    .map((line) => line.trim().split(/\s+/).at(-1)!);
  for (const name of undefinedNames) if (externalSymbol(name) === null) throw new Error(`unsupported external ${name}`);
  writeFileSync(symbolsSource, ".syntax unified\n.thumb\n" + undefinedNames.map(externalSymbolAssembly).join(""));
  await run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", symbolsObject, symbolsSource]);
  await run(["arm-none-eabi-ld", `-Ttext=0x${stems[0]}`, "-e", `Func_${stems[0]}`, "-o", elf, object, symbolsObject]);
  await run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary]);
  const symbolRows = await run(["arm-none-eabi-nm", "-S", "--defined-only", elf]);
  const addresses = new Map<string, number>();
  for (const line of symbolRows.split(/\r?\n/)) {
    const fields = line.trim().split(/\s+/);
    if (fields.length === 4 && /^Func_[0-9a-f]{8}$/.test(fields[3])) addresses.set(fields[3], Number.parseInt(fields[0], 16));
  }
  const misplaced = stems.filter((stem) => addresses.get(`Func_${stem}`) !== Number.parseInt(stem, 16));
  const actual = readFileSync(binary);
  const expectedSize = regions.at(-1)!.address + regions.at(-1)!.size - regions[0].address;
  const rom = readFileSync(join(ROOT, "gs1-en.gba"));
  const expected = rom.subarray(regions[0].address - ROM_BASE, regions[0].address - ROM_BASE + expectedSize);
  let mismatches = Math.abs(actual.length - expected.length);
  for (let index = 0; index < Math.min(actual.length, expected.length); index++) if (actual[index] !== expected[index]) mismatches++;
  const exact = misplaced.length === 0 && actual.length === expected.length && actual.equals(expected);
  console.log(`module=${stems.join(",")} bytes=${actual.length}/${expected.length} mismatches=${mismatches} misplaced=${misplaced.length} exact=${exact}`);
  if (install && exact) {
    writeFileSync(join(ROOT, "src", `${stems[0]}.c`), combined);
    for (const stem of stems) rmSync(join(ROOT, "asm", `${stem}.s`), { force: true });
    console.log(`installed src/${stems[0]}.c with ${stems.length} functions`);
  } else if (install) process.exitCode = 1;
}

if (import.meta.main) await main();
