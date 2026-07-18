#!/usr/bin/env bun
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, dirname, extname, isAbsolute, join, relative, resolve } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;
const EXTERNAL = /^(Func|Data|Value)_([0-9a-f]{8})$/;

interface Options {
  rom: string;
  output: string;
}

interface BuiltRegion {
  address: number;
  data: Buffer;
}

function stem(path: string): string {
  return basename(path, extname(path));
}

function rooted(path: string): string {
  return isAbsolute(path) ? path : resolve(ROOT, path);
}

async function run(command: string[]): Promise<string> {
  const child = Bun.spawn(command, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  const [stdout, stderr, exitCode] = await Promise.all([
    new Response(child.stdout).text(),
    new Response(child.stderr).text(),
    child.exited,
  ]);
  if (exitCode !== 0) {
    const detail = (stderr || stdout).trim();
    throw new Error(`${basename(command[0])} failed: ${detail}`);
  }
  return stdout;
}

async function buildRegion(source: string, outputDir: string): Promise<BuiltRegion> {
  const name = stem(source);
  const address = Number.parseInt(name, 16);
  const object = join(outputDir, `${name}.o`);
  const elf = join(outputDir, `${name}.elf`);
  const binary = join(outputDir, `${name}.bin`);
  await run([
    "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
    "-o", object, source,
  ]);
  const undefinedNames = (await run(["arm-none-eabi-nm", "-u", object]))
    .split(/\r?\n/).filter(Boolean).map((line) => line.trim().split(/\s+/).at(-1)!);
  for (const external of undefinedNames) {
    if (!EXTERNAL.test(external)) {
      throw new Error(`${basename(source)}: unsupported external symbol ${external}`);
    }
  }
  const objects = [object];
  if (undefinedNames.length !== 0) {
    const symbolsSource = join(outputDir, `${name}.symbols.s`);
    const symbolsObject = join(outputDir, `${name}.symbols.o`);
    writeFileSync(symbolsSource, ".syntax unified\n.thumb\n" + undefinedNames.map((external) =>
      `.global ${external}\n${external.startsWith("Func_") ? ".thumb_func\n" : ""}` +
      `.set ${external}, 0x${external.slice(external.lastIndexOf("_") + 1)}\n`,
    ).join(""));
    await run([
      "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
      "-o", symbolsObject, symbolsSource,
    ]);
    objects.push(symbolsObject);
  }
  const formatted = address.toString(16).padStart(8, "0");
  await run([
    "arm-none-eabi-ld", `-Ttext=0x${formatted}`, "-e", `0x${formatted}`,
    "-o", elf, ...objects,
  ]);
  await run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary]);
  return { address, data: readFileSync(binary) };
}

function usage(): void {
  console.log("usage: build_asm.ts [-h] [--output OUTPUT] [rom]");
}

function parseArgs(argv: string[]): Options {
  const options: Options = { rom: "baserom.gba", output: "out/asm" };
  let positional = false;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    } else if (argument === "--output") {
      const value = argv[++index];
      if (value === undefined) throw new Error("--output requires a value");
      options.output = value;
    } else if (argument.startsWith("--output=")) {
      options.output = argument.slice(9);
    } else if (!argument.startsWith("-") && !positional) {
      options.rom = argument;
      positional = true;
    } else {
      throw new Error(`unrecognized argument: ${argument}`);
    }
  }
  return options;
}

async function main(): Promise<void> {
  const args = parseArgs(Bun.argv.slice(2));
  const rom = readFileSync(resolve(process.cwd(), args.rom));
  const output = rooted(args.output);
  mkdirSync(output, { recursive: true });
  const sources = readdirSync(join(ROOT, "asm"), { withFileTypes: true })
    .filter((entry) => entry.isFile() && entry.name.endsWith(".s"))
    .map((entry) => join(ROOT, "asm", entry.name)).sort();
  if (sources.length === 0) throw new Error("no reconstructed assembly sources");
  const regions: Array<Record<string, string | number>> = [];
  let previousEnd = 0;
  for (const source of sources) {
    const { address, data } = await buildRegion(source, output);
    if (address < ROM_BASE || address >= ROM_BASE + rom.length || data.length === 0) {
      throw new Error(`${basename(source)}: region outside ROM`);
    }
    if (address < previousEnd) throw new Error(`${basename(source)}: overlapping assembly region`);
    previousEnd = address + data.length;
    const expected = rom.subarray(address - ROM_BASE, address - ROM_BASE + data.length);
    if (!data.equals(expected)) throw new Error(`${basename(source)}: assembled bytes differ`);
    regions.push({
      address,
      size: data.length,
      source: relative(ROOT, source),
      output: join(output, `${stem(source)}.bin`),
    });
  }
  writeFileSync(join(output, "manifest.json"), JSON.stringify({
    format: 1, rom_base: ROM_BASE, regions,
  }, null, 2) + "\n");
  const bytes = regions.reduce((sum, region) => sum + Number(region.size), 0);
  console.log(`regions=${regions.length} bytes=${bytes}`);
}

if (import.meta.main) await main();
