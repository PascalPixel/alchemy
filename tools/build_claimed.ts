#!/usr/bin/env bun
// Tool role: entrypoint; invoked by STATUS.md, package.json, tools/build_full.ts (+1 more).
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, dirname, extname, isAbsolute, join, relative, resolve } from "node:path";
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

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;

interface RunResult {
  stdout: string;
  stderr: string;
}

interface Compiled {
  object: string;
  definedNames: string[];
  undefinedNames: string[];
}

interface Options {
  target: DecompTargetId;
  rom: string;
  jobs: number;
  output: string;
  sourceOnly: boolean;
}

function stem(path: string): string {
  return basename(path, extname(path));
}

export function moduleEnd(
  names: string[],
  symbols: Map<string, [number, number]>,
): number {
  if (names.length === 0) throw new Error("C module has no functions");
  return Math.max(...names.map((name) => {
    const symbol = symbols.get(name);
    if (symbol === undefined || symbol[1] <= 0) throw new Error(`invalid module symbol ${name}`);
    return symbol[0] + symbol[1];
  }));
}

export function selfTest(): void {
  const symbols = new Map<string, [number, number]>([
    ["Func_0801c0c8", [0x0801c0c8, 2]],
    ["Func_0801c0cc", [0x0801c0cc, 2]],
    ["Func_0801c0d0", [0x0801c0d0, 2]],
    ["Func_0801c0d4", [0x0801c0d4, 2]],
    ["Func_0801c0d8", [0x0801c0d8, 2]],
  ]);
  if (moduleEnd([...symbols.keys()], symbols) !== 0x0801c0da) {
    throw new Error("multi-function C module range self-test failed");
  }
  console.log("self-test=ok");
}

function rooted(path: string): string {
  return isAbsolute(path) ? path : resolve(ROOT, path);
}

async function run(command: string[]): Promise<RunResult> {
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
  return { stdout, stderr };
}

async function compileSource(
  source: string,
  objectDir: string,
  compiler: CompilerTarget,
): Promise<Compiled> {
  const name = stem(source);
  const assembly = join(objectDir, `${name}.s`);
  const object = join(objectDir, `${name}.o`);
  await run(compilerCommandForTargetSource(
    compiler,
    source,
    ...cflagsForTargetSource(compiler, source),
    "-S", "-o", assembly, source,
  ));
  await run([
    "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
    "-o", object, assembly,
  ]);
  const defined = (await run([
    "arm-none-eabi-nm", "-g", "--defined-only", object,
  ])).stdout.split(/\r?\n/).filter(Boolean).map((line) => line.trim().split(/\s+/).at(-1)!);
  const expected = `Func_${name}`;
  if (!defined.includes(expected) || defined.some((symbol) => !/^Func_[0-9a-f]{8}$/.test(symbol))) {
    throw new Error(`${basename(source)}: expected ${expected} and address-named functions, found ${JSON.stringify(defined)}`);
  }
  const undefinedNames = (await run(["arm-none-eabi-nm", "-u", object])).stdout
    .split(/\r?\n/).filter(Boolean).map((line) => line.trim().split(/\s+/).at(-1)!);
  for (const external of undefinedNames) {
    if (externalSymbol(external) === null) {
      throw new Error(`${basename(source)}: unsupported external ${external}`);
    }
  }
  return { object, definedNames: defined, undefinedNames };
}

async function mapLimit<T, U>(items: T[], limit: number, action: (item: T) => Promise<U>): Promise<U[]> {
  const results = new Array<U>(items.length);
  let cursor = 0;
  async function worker(): Promise<void> {
    for (;;) {
      const index = cursor++;
      if (index >= items.length) return;
      results[index] = await action(items[index]);
    }
  }
  await Promise.all(Array.from({ length: Math.min(limit, items.length) }, () => worker()));
  return results;
}

function usage(): void {
  console.log(
    "usage: build_claimed.ts [-h] [--target gs1-en|gs2-en] [--source-only] " +
    "[--jobs JOBS] [--output OUTPUT] [rom]",
  );
}

function parseArgs(argv: string[]): Options {
  let targetId: DecompTargetId = DEFAULT_TARGET;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--target") {
      const value = argv[++index];
      if (value === undefined) throw new Error("--target requires a value");
      targetId = parseDecompTarget(value);
    } else if (argument.startsWith("--target=")) {
      targetId = parseDecompTarget(argument.slice(9));
    }
  }
  const target = decompTarget(targetId);
  const options: Options = {
    target: targetId,
    rom: target.rom,
    jobs: Math.min(16, navigator.hardwareConcurrency || 1),
    output: join(target.outputDir, "claimed"),
    sourceOnly: false,
  };
  let positional = false;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    } else if (argument === "--source-only") {
      options.sourceOnly = true;
    } else if (argument === "--target") {
      index++;
    } else if (argument.startsWith("--target=")) {
      continue;
    } else if (argument === "--jobs" || argument === "--output") {
      const value = argv[++index];
      if (value === undefined) throw new Error(`${argument} requires a value`);
      if (argument === "--jobs") options.jobs = Number.parseInt(value, 10);
      else options.output = value;
    } else if (argument.startsWith("--jobs=")) {
      options.jobs = Number.parseInt(argument.slice(7), 10);
    } else if (argument.startsWith("--output=")) {
      options.output = argument.slice(9);
    } else if (!argument.startsWith("-") && !positional) {
      options.rom = argument;
      positional = true;
    } else {
      throw new Error(`unrecognized argument: ${argument}`);
    }
  }
  if (!Number.isInteger(options.jobs) || options.jobs < 1) throw new Error("jobs must be positive");
  if (options.sourceOnly && positional) throw new Error("--source-only does not accept a ROM");
  return options;
}

async function main(): Promise<void> {
  const args = parseArgs(Bun.argv.slice(2));
  const target = decompTarget(args.target);
  const rom = args.sourceOnly ? null : readFileSync(resolve(process.cwd(), args.rom));
  if (rom !== null && rom.length !== target.romSize) {
    throw new Error(`${target.id} ROM must contain exactly ${target.romSize} bytes`);
  }
  const sourceDirectory = rooted(target.sourceDir);
  const sources = readdirSync(sourceDirectory, { withFileTypes: true })
    .filter((entry) => entry.isFile() && entry.name.endsWith(".c"))
    .map((entry) => join(sourceDirectory, entry.name)).sort();
  if (sources.length === 0) throw new Error("no reconstructed sources");
  const addresses = sources.map((source) => Number.parseInt(stem(source), 16));
  if (new Set(addresses).size !== addresses.length) throw new Error("duplicate source address");
  const limit = ROM_BASE + target.romSize;
  if (addresses.some((address) => address < ROM_BASE || address >= limit)) {
    throw new Error("source address outside ROM");
  }

  const output = rooted(args.output);
  const objectDir = join(output, "obj");
  mkdirSync(objectDir, { recursive: true });
  const compiled = await mapLimit(
    sources,
    args.jobs,
    (source) => compileSource(source, objectDir, target.compiler),
  );
  const objects = compiled.map((item) => item.object);
  const definitions = compiled.flatMap((item) => item.definedNames);
  const defined = new Set(definitions);
  if (defined.size !== definitions.length) throw new Error("duplicate function definition across C modules");
  const undefinedNames = [...new Set(compiled.flatMap((item) => item.undefinedNames)
    .filter((name) => !defined.has(name)))].sort();
  const symbolsSource = join(output, "externals.s");
  const symbolsObject = join(output, "externals.o");
  writeFileSync(symbolsSource, ".syntax unified\n.thumb\n" + undefinedNames.map(externalSymbolAssembly).join(""));
  await run([
    "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
    "-o", symbolsObject, symbolsSource,
  ]);

  const linker = join(output, "claimed.ld");
  writeFileSync(linker,
    `OUTPUT_ARCH(arm)\nENTRY(Func_${stem(sources[0])})\nSECTIONS\n{\n` +
    sources.map((source, index) =>
      `  .func_${stem(source)} 0x${stem(source)} : { ${relative(ROOT, objects[index])}(.text) }\n`,
    ).join("") +
    "  /DISCARD/ : { *(.comment) *(.note*) }\n}\n",
  );
  const elf = join(output, "claimed.elf");
  const binary = join(output, "claimed.bin");
  await run(["arm-none-eabi-ld", "-T", linker, "-o", elf, symbolsObject]);
  await run(["arm-none-eabi-objcopy", "-O", "binary", elf, binary]);

  const symbols = new Map<string, [number, number]>();
  for (const line of (await run([
    "arm-none-eabi-nm", "-S", "--defined-only", elf,
  ])).stdout.split(/\r?\n/)) {
    const fields = line.trim().split(/\s+/);
    if (fields.length === 4 && defined.has(fields[3])) {
      symbols.set(fields[3], [Number.parseInt(fields[0], 16), Number.parseInt(fields[1], 16)]);
    }
  }
  if (symbols.size !== defined.size) {
    const missing = [...defined].filter((name) => !symbols.has(name)).sort();
    throw new Error(`missing linked functions: ${JSON.stringify(missing)}`);
  }

  const image = readFileSync(binary);
  const imageBase = Math.min(...[...symbols.values()].map(([address]) => address));
  const manifest: Array<Record<string, unknown>> = [];
  const failures: string[] = [];
  let total = 0;
  let previousEnd = 0;
  for (let sourceIndex = 0; sourceIndex < sources.length; sourceIndex++) {
    const source = sources[sourceIndex];
    const name = `Func_${stem(source)}`;
    const [address] = symbols.get(name)!;
    if (address !== Number.parseInt(stem(source), 16)) {
      failures.push(`${basename(source)}: linked at 0x${address.toString(16).padStart(8, "0")}`);
      continue;
    }
    const moduleSymbols = compiled[sourceIndex].definedNames.sort((left, right) =>
      symbols.get(left)![0] - symbols.get(right)![0]
    );
    const end = moduleEnd(moduleSymbols, symbols);
    const size = end - address;
    if (address < previousEnd) failures.push(`${basename(source)}: overlaps previous function`);
    previousEnd = Math.max(previousEnd, end);
    const offset = address - imageBase;
    const actual = image.subarray(offset, offset + size);
    if (end > limit) failures.push(`${basename(source)}: linked extent outside ROM`);
    if (rom !== null) {
      const expected = rom.subarray(address - ROM_BASE, end - ROM_BASE);
      if (!actual.equals(expected)) failures.push(`${basename(source)}: linked bytes differ`);
    }
    total += size;
    for (const symbol of moduleSymbols) {
      const expectedAddress = Number.parseInt(symbol.slice(5), 16);
      if (symbols.get(symbol)![0] !== expectedAddress || expectedAddress < address || expectedAddress >= end) {
        failures.push(`${basename(source)}: invalid module symbol ${symbol}`);
      }
    }
    manifest.push({
      source: relative(ROOT, source), symbol: name, symbols: moduleSymbols, address, size, end,
    });
  }
  writeFileSync(join(output, "manifest.json"), JSON.stringify({
    format: 1,
    target: target.id,
    compiler: target.compiler,
    rom_base: ROM_BASE,
    rom_size: target.romSize,
    verification: args.sourceOnly ? "source_only" : "rom",
    image_base: imageBase,
    image_size: image.length,
    claimed_bytes: total,
    regions: manifest,
  }, null, 2) + "\n");

  console.log(`linked=${manifest.length} failures=${failures.length} claimed_bytes=${total} image_bytes=${image.length}`);
  if (failures.length !== 0) {
    failures.forEach((failure) => console.log(failure));
    process.exitCode = 1;
  }
}

if (import.meta.main) {
  if (Bun.argv.includes("--self-test")) selfTest();
  else await main();
}
