#!/usr/bin/env bun
import { existsSync, mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, dirname, extname, isAbsolute, join, relative, resolve } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;
const EXTERNAL = /^(Func|Data|Value)_([0-9a-f]{8})$/;

interface Options {
  rom: string;
  output: string;
  source?: string;
  sourceOnly: boolean;
}

interface AlignmentSource {
  format: 1;
  kind: "thumb-function-alignment";
  width: 2;
  value: 0;
  addresses: string[];
}

interface BuiltRegion {
  address: number;
  runAddress: number;
  data: Buffer;
}

interface LayoutRegion {
  source: string;
  address?: string | number;
  run_address: string | number;
}

interface Classification {
  kind: string;
  origin: string;
  retention: string;
  confidence: string;
  evidence: string[];
}

interface ClassificationRule extends Classification {
  expected_files: number;
  expected_bytes: number;
  files?: string[];
}

interface ClassificationConfig {
  format: number;
  default: Classification;
  structural: ClassificationRule[];
  groups: ClassificationRule[];
}

interface ClassificationCount {
  files: number;
  bytes: number;
}

function stem(path: string): string {
  return basename(path, extname(path));
}

function rooted(path: string): string {
  return isAbsolute(path) ? path : resolve(ROOT, path);
}

function assemblySources(directory: string): string[] {
  const result: string[] = [];
  for (const entry of readdirSync(directory, { withFileTypes: true })) {
    if (entry.name.startsWith(".")) continue;
    const path = join(directory, entry.name);
    if (entry.isDirectory()) result.push(...assemblySources(path));
    else if (entry.isFile() && entry.name.endsWith(".s")) result.push(path);
  }
  return result.sort();
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

function integer(value: string | number, name: string): number {
  const result = typeof value === "number" ? value : Number(value);
  if (!Number.isInteger(result) || result < 0 || result > 0xffffffff) {
    throw new Error(`${name}: invalid address`);
  }
  return result;
}

function loadLayout(): Map<string, { address: number; runAddress: number }> {
  const path = join(ROOT, "asm/manifest.json");
  if (!existsSync(path)) return new Map();
  const document = JSON.parse(readFileSync(path, "utf8"));
  if (document.format !== 1 || !Array.isArray(document.regions)) {
    throw new Error("asm/manifest.json: unsupported format");
  }
  const result = new Map<string, { address: number; runAddress: number }>();
  for (const item of document.regions as LayoutRegion[]) {
    if (typeof item.source !== "string" || result.has(item.source)) {
      throw new Error("asm/manifest.json: invalid or duplicate source");
    }
    const inferred = Number.parseInt(stem(item.source), 16);
    const address = item.address === undefined ? inferred : integer(item.address, item.source);
    if (address !== inferred) throw new Error(`${item.source}: load address differs from filename`);
    result.set(item.source, { address, runAddress: integer(item.run_address, item.source) });
  }
  return result;
}

function loadClassification(path: string): ClassificationConfig {
  const config = JSON.parse(readFileSync(path, "utf8")) as ClassificationConfig;
  if (config.format !== 1) throw new Error(`unsupported assembly classification format: ${config.format}`);
  return config;
}

function loadAlignments(path: string): Array<{ address: number; data: Buffer }> {
  const source = JSON.parse(readFileSync(path, "utf8")) as AlignmentSource;
  if (source.format !== 1 || source.kind !== "thumb-function-alignment" ||
      source.width !== 2 || source.value !== 0 || !Array.isArray(source.addresses)) {
    throw new Error("unsupported alignment source");
  }
  const found = new Set<number>();
  return source.addresses.map((text, index) => {
    if (!/^0x080[0-9a-f]{5}$/.test(text)) throw new Error(`alignment ${index}: invalid address`);
    const address = Number(text);
    if ((address & 3) !== 2 || found.has(address)) throw new Error(`alignment ${index}: invalid boundary`);
    found.add(address);
    return { address, data: Buffer.alloc(2) };
  }).sort((left, right) => left.address - right.address);
}

function explicitClassifications(config: ClassificationConfig): Map<string, ClassificationRule> {
  const result = new Map<string, ClassificationRule>();
  for (const group of config.groups) {
    for (const name of group.files ?? []) {
      if (!/^[0-9a-f]{8}$/.test(name)) throw new Error(`invalid classified assembly stem: ${name}`);
      if (result.has(name)) throw new Error(`duplicate assembly classification: ${name}`);
      result.set(name, group);
    }
  }
  return result;
}

function longCallVeneer(data: Buffer): boolean {
  return data.length === 8 &&
    data.readUInt16LE(0) === 0x4c00 &&
    data.readUInt16LE(2) === 0x4720 &&
    (data.readUInt32LE(4) & 1) !== 0;
}

function alignmentPadding(data: Buffer): boolean {
  return data.length === 2 && data.readUInt16LE(0) === 0;
}

function classify(
  name: string,
  data: Buffer,
  config: ClassificationConfig,
  explicit: Map<string, ClassificationRule>,
): Classification {
  const fixed = explicit.get(name);
  if (fixed !== undefined) return fixed;
  if (longCallVeneer(data)) {
    const veneer = config.structural.find((rule) => rule.kind === "linker_veneer");
    if (veneer === undefined) throw new Error("missing linker veneer classification");
    return veneer;
  }
  if (alignmentPadding(data)) {
    const padding = config.structural.find((rule) => rule.kind === "alignment_padding");
    if (padding === undefined) throw new Error("missing alignment padding classification");
    return padding;
  }
  return config.default;
}

function validateClassificationCounts(
  config: ClassificationConfig,
  counts: Map<string, ClassificationCount>,
): void {
  for (const rule of [...config.structural, ...config.groups]) {
    const count = counts.get(rule.kind) ?? { files: 0, bytes: 0 };
    if (count.files !== rule.expected_files || count.bytes !== rule.expected_bytes) {
      throw new Error(
        `${rule.kind}: expected ${rule.expected_files} files/${rule.expected_bytes} bytes, ` +
        `got ${count.files} files/${count.bytes} bytes`,
      );
    }
  }
}

async function buildRegion(source: string, outputDir: string, runAddress?: number): Promise<BuiltRegion> {
  const name = stem(source);
  const address = Number.parseInt(name, 16);
  const linkedAddress = runAddress ?? address;
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
  const formatted = linkedAddress.toString(16).padStart(8, "0");
  await run([
    "arm-none-eabi-ld", `-Ttext=0x${formatted}`, "-e", `0x${formatted}`,
    "-o", elf, ...objects,
  ]);
  await run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary]);
  return { address, runAddress: linkedAddress, data: readFileSync(binary) };
}

function usage(): void {
  console.log("usage: build_asm.ts [-h] [--source-only] [--output OUTPUT] [--source SOURCE] [rom]");
}

function parseArgs(argv: string[]): Options {
  const options: Options = { rom: "gs1-en.gba", output: "out/asm", sourceOnly: false };
  let positional = false;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    } else if (argument === "--source-only") {
      options.sourceOnly = true;
    } else if (argument === "--output") {
      const value = argv[++index];
      if (value === undefined) throw new Error("--output requires a value");
      options.output = value;
    } else if (argument.startsWith("--output=")) {
      options.output = argument.slice(9);
    } else if (argument === "--source") {
      const value = argv[++index];
      if (value === undefined) throw new Error("--source requires a value");
      options.source = value;
    } else if (argument.startsWith("--source=")) {
      options.source = argument.slice(9);
    } else if (!argument.startsWith("-") && !positional) {
      options.rom = argument;
      positional = true;
    } else {
      throw new Error(`unrecognized argument: ${argument}`);
    }
  }
  if (options.sourceOnly && positional) throw new Error("--source-only does not accept a ROM");
  return options;
}

async function main(): Promise<void> {
  const args = parseArgs(Bun.argv.slice(2));
  const rom = args.sourceOnly ? null : readFileSync(resolve(process.cwd(), args.rom));
  const output = rooted(args.output);
  mkdirSync(output, { recursive: true });
  let sources = assemblySources(join(ROOT, "asm"));
  const stems = new Set<string>();
  for (const source of sources) {
    const name = stem(source);
    if (stems.has(name)) throw new Error(`duplicate assembly source stem: ${name}`);
    stems.add(name);
  }
  if (args.source !== undefined) {
    const selected = rooted(args.source);
    sources = sources.filter((source) => source === selected);
    if (sources.length === 0) throw new Error(`${args.source}: assembly source not found`);
  }
  if (sources.length === 0) throw new Error("no reconstructed assembly sources");
  const layout = loadLayout();
  const classificationPath = join(ROOT, "asm/classification.json");
  const classification = loadClassification(classificationPath);
  const explicit = explicitClassifications(classification);
  const found = new Set<string>();
  const counts = new Map<string, ClassificationCount>();
  const sourceNames = new Set(sources.map((source) => relative(ROOT, source)));
  if (args.source === undefined) {
    for (const source of layout.keys()) {
      if (!sourceNames.has(source)) throw new Error(`${source}: layout source not found`);
    }
  }
  const regions: Array<Record<string, string | number>> = [];
  for (const source of sources) {
    const sourceName = relative(ROOT, source);
    const placement = layout.get(sourceName);
    const { address, runAddress, data } = await buildRegion(source, output, placement?.runAddress);
    const limit = rom === null ? ROM_BASE + 0x00800000 : ROM_BASE + rom.length;
    if (address < ROM_BASE || address >= limit || data.length === 0 || address + data.length > limit) {
      throw new Error(`${basename(source)}: region outside ROM`);
    }
    if (rom !== null) {
      const expected = rom.subarray(address - ROM_BASE, address - ROM_BASE + data.length);
      if (!data.equals(expected)) throw new Error(`${basename(source)}: assembled bytes differ`);
    }
    const name = stem(source);
    const category = classify(name, data, classification, explicit);
    const count = counts.get(category.kind) ?? { files: 0, bytes: 0 };
    count.files++;
    count.bytes += data.length;
    counts.set(category.kind, count);
    found.add(name);
    regions.push({
      address,
      run_address: runAddress,
      size: data.length,
      source: sourceName,
      output: join(output, `${name}.bin`),
      kind: category.kind,
      origin: category.origin,
      retention: category.retention,
      confidence: category.confidence,
      evidence: category.evidence.join(","),
    });
  }
  if (args.source === undefined) {
    const alignmentPath = join(ROOT, "asm/alignment.json");
    const category = classification.structural.find((item) => item.kind === "alignment_padding");
    if (category === undefined) throw new Error("missing alignment padding classification");
    for (const { address, data } of loadAlignments(alignmentPath)) {
      const name = address.toString(16).padStart(8, "0");
      const outputPath = join(output, `${name}.bin`);
      if (rom !== null) {
        const expected = rom.subarray(address - ROM_BASE, address - ROM_BASE + data.length);
        if (!data.equals(expected)) throw new Error(`${name}: alignment bytes differ`);
      }
      writeFileSync(outputPath, data);
      const count = counts.get(category.kind) ?? { files: 0, bytes: 0 };
      count.files++;
      count.bytes += data.length;
      counts.set(category.kind, count);
      regions.push({
        address,
        run_address: address,
        size: data.length,
        source: relative(ROOT, alignmentPath),
        output: outputPath,
        kind: category.kind,
        origin: category.origin,
        retention: category.retention,
        confidence: category.confidence,
        evidence: category.evidence.join(","),
      });
    }
  }
  regions.sort((left, right) => Number(left.address) - Number(right.address));
  let previousEnd = 0;
  for (const region of regions) {
    const address = Number(region.address);
    if (address < previousEnd) throw new Error(`overlapping assembly region at 0x${address.toString(16)}`);
    previousEnd = address + Number(region.size);
  }
  if (args.source === undefined) {
    for (const name of explicit.keys()) {
      if (!found.has(name)) throw new Error(`classified assembly source is missing: ${name}.s`);
    }
    validateClassificationCounts(classification, counts);
  }
  writeFileSync(join(output, "manifest.json"), JSON.stringify({
    format: 1,
    rom_base: ROM_BASE,
    verification: args.sourceOnly ? "source_only" : "rom",
    classification: relative(ROOT, classificationPath),
    regions,
  }, null, 2) + "\n");
  const bytes = regions.reduce((sum, region) => sum + Number(region.size), 0);
  console.log(`regions=${regions.length} bytes=${bytes}`);
  console.log([...counts.entries()].sort(([left], [right]) => left.localeCompare(right))
    .map(([kind, count]) => `${kind}=${count.files}/${count.bytes}`)
    .join(" "));
}

if (import.meta.main) await main();
