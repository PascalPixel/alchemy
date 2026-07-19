#!/usr/bin/env bun
import { existsSync, mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { dirname, extname, isAbsolute, join, resolve } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;
const ROM_SIZE = 0x00800000;

interface Options {
  rom: string;
  sourceOnly: boolean;
  output: string;
  claimedOutput: string;
  asmOutput: string;
  assetManifest: string;
  assetOutput: string;
  jobs: number;
}

interface Region {
  address: number;
  size: number;
  output: string;
}

interface AssemblyRegion extends Region {
  kind: string;
  retention: string;
}

export interface UnownedRegion {
  address: number;
  size: number;
  source: string;
  kind: "unowned";
}

export interface FallbackRegion {
  address: number;
  size: number;
  source: string;
  kind: "rom_fallback";
}

export interface AssemblySourceAccounting {
  cDebtRegions: number;
  cDebtBytes: number;
  retainedStructuralRegions: number;
  retainedStructuralBytes: number;
}

export interface TotalDecompilationProgress {
  bytes: number;
  remainingBytes: number;
  percent: number;
}

const ASM_C_DEBT_RETENTIONS = new Set([
  "c_candidate",
  "split_first",
  "merge_with_owner",
  "merge_with_function_owner",
  "merge_with_continuations",
]);

const ASM_RETAINED_RETENTIONS = new Set([
  "keep_asm",
  "keep_structured_asm",
  "adjacent_section_alignment",
]);

function rooted(path: string): string {
  return isAbsolute(path) ? path : resolve(ROOT, path);
}

function usage(): void {
  console.log(
    "usage: build_full.ts [-h] [-o OUTPUT] [--claimed-output CLAIMED_OUTPUT] " +
    "[--asm-output ASM_OUTPUT] [--asset-manifest ASSET_MANIFEST] " +
    "[--asset-output ASSET_OUTPUT] [--jobs JOBS] [--source-only] [rom] | --self-test",
  );
}

function hexadecimal(value: number): string {
  return value.toString(16).padStart(8, "0");
}

function groupedDecimal(value: number): string {
  const digits = String(value);
  const first = digits.length % 3 || 3;
  const groups = [digits.slice(0, first)];
  for (let index = first; index < digits.length; index += 3) {
    groups.push(digits.slice(index, index + 3));
  }
  return groups.join(",");
}

export function formatProgress(sourceBytes: number, romSize: number): string {
  if (!Number.isSafeInteger(sourceBytes) || !Number.isSafeInteger(romSize) ||
      sourceBytes < 0 || romSize <= 0 || sourceBytes > romSize) {
    throw new Error("invalid progress byte count");
  }
  return `[${groupedDecimal(sourceBytes)} of ${groupedDecimal(romSize)} bytes]`;
}

export function unownedRegions(mask: Uint8Array, base = ROM_BASE): UnownedRegion[] {
  if (!Number.isSafeInteger(base) || base < 0 || base + mask.length > 0x100000000) {
    throw new Error("invalid coverage base");
  }
  const regions: UnownedRegion[] = [];
  let start = -1;
  for (let index = 0; index <= mask.length; index++) {
    const value = index === mask.length ? 1 : mask[index];
    if (value !== 0 && value !== 1) throw new Error(`invalid coverage byte at ${index}`);
    if (value === 0 && start < 0) {
      start = index;
    } else if (value === 1 && start >= 0) {
      const address = base + start;
      regions.push({
        address,
        size: index - start,
        source: `unowned/${hexadecimal(address)}`,
        kind: "unowned",
      });
      start = -1;
    }
  }
  return regions;
}

export function uncoveredRegions(mask: Uint8Array, base = ROM_BASE): FallbackRegion[] {
  return unownedRegions(mask, base).map((region) => ({
    address: region.address,
    size: region.size,
    source: `rom-fallback/${hexadecimal(region.address)}`,
    kind: "rom_fallback",
  }));
}

export function assemblySourceAccounting(
  regions: Array<{ size: number; retention: string }>,
): AssemblySourceAccounting {
  const result: AssemblySourceAccounting = {
    cDebtRegions: 0,
    cDebtBytes: 0,
    retainedStructuralRegions: 0,
    retainedStructuralBytes: 0,
  };
  for (const region of regions) {
    if (!Number.isSafeInteger(region.size) || region.size <= 0) {
      throw new Error("invalid assembly accounting size");
    }
    if (ASM_C_DEBT_RETENTIONS.has(region.retention)) {
      result.cDebtRegions++;
      result.cDebtBytes += region.size;
    } else if (ASM_RETAINED_RETENTIONS.has(region.retention)) {
      result.retainedStructuralRegions++;
      result.retainedStructuralBytes += region.size;
    } else {
      throw new Error(`unsupported assembly retention: ${region.retention}`);
    }
  }
  return result;
}

export function totalDecompilationProgress(
  romSize: number,
  codeBytes: number,
  assetBytes: number,
  retainedAssemblyBytes: number,
  assemblyDebtBytes: number,
  unownedBytes: number,
): TotalDecompilationProgress {
  const values = [romSize, codeBytes, assetBytes, retainedAssemblyBytes, assemblyDebtBytes, unownedBytes];
  if (values.some((value, index) => !Number.isSafeInteger(value) || value < (index === 0 ? 1 : 0))) {
    throw new Error("invalid total decompilation count");
  }
  const bytes = codeBytes + assetBytes + retainedAssemblyBytes;
  const remainingBytes = assemblyDebtBytes + unownedBytes;
  if (bytes + remainingBytes !== romSize) throw new Error("total decompilation count differs");
  return { bytes, remainingBytes, percent: Number((bytes * 100 / romSize).toFixed(2)) };
}

export function selfTest(): void {
  const unowned = unownedRegions(Uint8Array.from([1, 0, 0, 1, 0]), 0x08000000);
  if (unowned.length !== 2 || unowned[0].address !== 0x08000001 ||
      unowned[0].size !== 2 || unowned[0].source !== "unowned/08000001" ||
      unowned[0].kind !== "unowned" || unowned[1].address !== 0x08000004 ||
      unowned[1].size !== 1) {
    throw new Error("unowned coverage self-test failed");
  }
  const fallback = uncoveredRegions(Uint8Array.from([1, 0, 0, 1, 0]), 0x08000000);
  if (fallback.length !== 2 || fallback[0].address !== 0x08000001 ||
      fallback[0].size !== 2 || fallback[0].source !== "rom-fallback/08000001" ||
      fallback[0].kind !== "rom_fallback" || fallback[1].address !== 0x08000004 ||
      fallback[1].size !== 1) {
    throw new Error("fallback coverage self-test failed");
  }
  const assembly = assemblySourceAccounting([
    { size: 12, retention: "c_candidate" },
    { size: 6, retention: "split_first" },
    { size: 8, retention: "keep_asm" },
    { size: 2, retention: "adjacent_section_alignment" },
  ]);
  if (assembly.cDebtRegions !== 2 || assembly.cDebtBytes !== 18 ||
      assembly.retainedStructuralRegions !== 2 || assembly.retainedStructuralBytes !== 10) {
    throw new Error("assembly source accounting self-test failed");
  }
  const progress = totalDecompilationProgress(100, 10, 50, 5, 25, 10);
  if (progress.bytes !== 65 || progress.remainingBytes !== 35 || progress.percent !== 65) {
    throw new Error("total decompilation progress self-test failed");
  }
  try {
    totalDecompilationProgress(100, 10, 50, 5, 25, 9);
    throw new Error("incomplete total decompilation count was accepted");
  } catch (error) {
    if ((error as Error).message === "incomplete total decompilation count was accepted") throw error;
  }
  try {
    assemblySourceAccounting([{ size: 4, retention: "unknown" }]);
    throw new Error("unknown assembly retention was accepted");
  } catch (error) {
    if ((error as Error).message === "unknown assembly retention was accepted") throw error;
  }
  if (formatProgress(1234567, 8388608) !== "[1,234,567 of 8,388,608 bytes]") {
    throw new Error("progress formatting self-test failed");
  }
  for (const [sourceBytes, romSize] of [[-1, 1], [2, 1], [0, 0], [1.5, 2]]) {
    try {
      formatProgress(sourceBytes, romSize);
      throw new Error("invalid progress was accepted");
    } catch (error) {
      if ((error as Error).message === "invalid progress was accepted") throw error;
    }
  }
  console.log("self-test=ok");
}

function parseArgs(argv: string[]): Options {
  const options: Options = {
    rom: "baserom.gba",
    sourceOnly: false,
    output: "out/full/rebuilt.gba",
    claimedOutput: "out/full/claimed",
    asmOutput: "out/full/asm",
    assetManifest: "assets/manifest.json",
    assetOutput: "out/full/assets",
    jobs: Math.min(16, navigator.hardwareConcurrency || 1),
  };
  const keys: Record<string, keyof Options> = {
    "-o": "output",
    "--output": "output",
    "--claimed-output": "claimedOutput",
    "--asm-output": "asmOutput",
    "--asset-manifest": "assetManifest",
    "--asset-output": "assetOutput",
    "--jobs": "jobs",
  };
  let positional = false;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    }
    if (argument === "--source-only") {
      options.sourceOnly = true;
      continue;
    }
    const equal = argument.indexOf("=");
    const option = equal >= 0 ? argument.slice(0, equal) : argument;
    if (option in keys) {
      const value = equal >= 0 ? argument.slice(equal + 1) : argv[++index];
      if (value === undefined) throw new Error(`${option} requires a value`);
      const key = keys[option];
      if (key === "jobs") options.jobs = Number.parseInt(value, 10);
      else options[key] = value as never;
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

async function run(command: string[]): Promise<void> {
  const child = Bun.spawn(command, { cwd: ROOT, stdout: "inherit", stderr: "inherit" });
  const exitCode = await child.exited;
  if (exitCode !== 0) throw new Error(`${command[1] ?? command[0]} failed with status ${exitCode}`);
}

function readJson(path: string): any {
  return JSON.parse(readFileSync(path, "utf8"));
}

async function main(): Promise<void> {
  if (Bun.argv.slice(2).includes("--self-test")) {
    selfTest();
    return;
  }
  const args = parseArgs(Bun.argv.slice(2));
  const romPath = resolve(process.cwd(), args.rom);
  const rom = args.sourceOnly ? null : readFileSync(romPath);
  const romSize = rom?.length ?? ROM_SIZE;
  const claimed = rooted(args.claimedOutput);
  const claimedCommand = [process.execPath, "tools/build_claimed.ts"];
  if (args.sourceOnly) claimedCommand.push("--source-only");
  else claimedCommand.push(romPath);
  claimedCommand.push("--jobs", String(args.jobs), "--output", claimed);
  await run(claimedCommand);
  const manifest = readJson(join(claimed, "manifest.json"));
  const image = readFileSync(join(claimed, "claimed.bin"));
  const imageBase = Number(manifest.image_base);
  const rebuilt = rom === null ? null : Buffer.from(rom);
  const claimedMask = Buffer.alloc(romSize);
  for (const region of manifest.regions as Region[]) {
    const address = Number(region.address);
    const size = Number(region.size);
    const start = address - ROM_BASE;
    const end = start + size;
    if (start < 0 || end > romSize || end <= start) throw new Error(`source outside image at 0x${hexadecimal(address)}`);
    const source = image.subarray(address - imageBase, address - imageBase + size);
    const expected = rom?.subarray(start, end);
    if (expected !== undefined && !source.equals(expected)) {
      throw new Error(`source mismatch at 0x${address.toString(16).padStart(8, "0")} (0x${size.toString(16)} bytes)`);
    }
    if (rebuilt !== null) source.copy(rebuilt, start);
    claimedMask.fill(1, start, start + size);
  }

  const asmRegions: AssemblyRegion[] = [];
  let asmAccounting = assemblySourceAccounting([]);
  const asmDirectory = join(ROOT, "asm");
  if (existsSync(asmDirectory) && readdirSync(asmDirectory).some((name) => name.endsWith(".s"))) {
    const asmOutput = rooted(args.asmOutput);
    const asmCommand = [process.execPath, "tools/build_asm.ts"];
    if (args.sourceOnly) asmCommand.push("--source-only");
    else asmCommand.push(romPath);
    asmCommand.push("--output", asmOutput);
    await run(asmCommand);
    const asmManifest = readJson(join(asmOutput, "manifest.json"));
    asmAccounting = assemblySourceAccounting(asmManifest.regions as AssemblyRegion[]);
    for (const region of asmManifest.regions as AssemblyRegion[]) {
      const address = Number(region.address);
      const size = Number(region.size);
      const start = address - ROM_BASE;
      const end = start + size;
      if (start < 0 || end > romSize || end <= start) throw new Error(`assembly outside image at 0x${hexadecimal(address)}`);
      if (claimedMask.subarray(start, end).some((value) => value !== 0)) {
        throw new Error(`assembly overlaps another source at 0x${address.toString(16).padStart(8, "0")}`);
      }
      const source = readFileSync(region.output);
      const expected = rom?.subarray(start, end);
      if (expected !== undefined && !source.equals(expected)) {
        throw new Error(`assembly mismatch at 0x${address.toString(16).padStart(8, "0")} (0x${size.toString(16)} bytes)`);
      }
      if (rebuilt !== null) source.copy(rebuilt, start);
      claimedMask.fill(1, start, end);
      asmRegions.push(region);
    }
  }

  const assetRegions: Region[] = [];
  const assetManifestPath = rooted(args.assetManifest);
  if (existsSync(assetManifestPath)) {
    const assetOutput = rooted(args.assetOutput);
    const assetCommand = [process.execPath, "tools/build_assets.ts"];
    if (args.sourceOnly) assetCommand.push("--source-only");
    else assetCommand.push(romPath);
    assetCommand.push("--manifest", assetManifestPath, "--output", assetOutput);
    await run(assetCommand);
    const assetManifest = readJson(join(assetOutput, "manifest.json"));
    for (const region of assetManifest.regions as Region[]) {
      const address = Number(region.address);
      const size = Number(region.size);
      const start = address - ROM_BASE;
      const end = start + size;
      if (start < 0 || end > romSize || end <= start) throw new Error(`asset outside image at 0x${hexadecimal(address)}`);
      if (claimedMask.subarray(start, end).some((value) => value !== 0)) {
        throw new Error(`asset overlaps another source at 0x${address.toString(16).padStart(8, "0")}`);
      }
      const source = readFileSync(region.output);
      const expected = rom?.subarray(start, end);
      if (expected !== undefined && !source.equals(expected)) {
        throw new Error(`asset mismatch at 0x${address.toString(16).padStart(8, "0")} (0x${size.toString(16)} bytes)`);
      }
      if (rebuilt !== null) source.copy(rebuilt, start);
      claimedMask.fill(1, start, end);
      assetRegions.push(region);
    }
  }
  if (rebuilt !== null && rom !== null && !rebuilt.equals(rom)) throw new Error("full rebuild differs from private ROM");
  const output = rooted(args.output);
  mkdirSync(dirname(output), { recursive: true });
  if (rebuilt !== null) writeFileSync(output, rebuilt);
  const sourceBytes = claimedMask.reduce((sum, value) => sum + value, 0);
  const codeBytes = Number(manifest.claimed_bytes);
  const asmBytes = asmRegions.reduce((sum, region) => sum + region.size, 0);
  const assetBytes = assetRegions.reduce((sum, region) => sum + region.size, 0);
  if (codeBytes + asmBytes + assetBytes !== sourceBytes) {
    throw new Error("source component count differs");
  }
  if (asmAccounting.cDebtBytes + asmAccounting.retainedStructuralBytes !== asmBytes) {
    throw new Error("assembly source accounting differs");
  }
  const unowned = unownedRegions(claimedMask);
  const unownedBytes = unowned.reduce((sum, region) => sum + region.size, 0);
  if (unownedBytes !== romSize - sourceBytes) throw new Error("unowned coverage count differs");
  const totalDecompilation = totalDecompilationProgress(
    romSize,
    codeBytes,
    assetBytes,
    asmAccounting.retainedStructuralBytes,
    asmAccounting.cDebtBytes,
    unownedBytes,
  );
  const fallback = uncoveredRegions(claimedMask);
  const suffix = extname(output);
  const reportBase = suffix ? output.slice(0, -suffix.length) : output;
  const unownedPath = reportBase + ".unowned.json";
  writeFileSync(unownedPath, JSON.stringify({
    format: 1,
    semantics: "source_ownership",
    verification: args.sourceOnly ? "source_only" : "rom",
    rom_base: ROM_BASE,
    rom_size: romSize,
    regions: unowned,
  }, null, 2) + "\n");
  const fallbackPath = reportBase + ".fallback.json";
  writeFileSync(fallbackPath, JSON.stringify({
    format: 1,
    semantics: args.sourceOnly ? "compatibility_alias_for_unowned_ranges" : "private_rom_fallback",
    rom_base: ROM_BASE,
    rom_size: romSize,
    regions: fallback,
  }, null, 2) + "\n");
  const report = {
    format: 1,
    rom_base: ROM_BASE,
    rom_size: romSize,
    code_regions: manifest.regions.length,
    code_bytes: codeBytes,
    asm_regions: asmRegions.length,
    asm_bytes: asmBytes,
    asm_c_debt_regions: asmAccounting.cDebtRegions,
    asm_c_debt_bytes: asmAccounting.cDebtBytes,
    asm_retained_structural_regions: asmAccounting.retainedStructuralRegions,
    asm_retained_structural_bytes: asmAccounting.retainedStructuralBytes,
    asset_regions: assetRegions.length,
    asset_bytes: assetBytes,
    source_regions: manifest.regions.length + asmRegions.length + assetRegions.length,
    source_bytes: sourceBytes,
    total_decompilation_bytes: totalDecompilation.bytes,
    total_decompilation_remaining_bytes: totalDecompilation.remainingBytes,
    total_decompilation_percent: totalDecompilation.percent,
    unowned_bytes: unownedBytes,
    unowned_regions: unowned.length,
    unowned_manifest: unownedPath,
    rom_fallback_bytes: unownedBytes,
    fallback_regions: fallback.length,
    fallback_manifest: fallbackPath,
    rom_fallback_applicable: !args.sourceOnly,
    verification: args.sourceOnly ? "source_only" : "rom",
    byte_identical: !args.sourceOnly,
    output: args.sourceOnly ? null : args.output,
  };
  const reportPath = reportBase + ".json";
  writeFileSync(reportPath, JSON.stringify(report, null, 2) + "\n");
  console.log(
    `${args.sourceOnly ? "source_only=True" : "identical=True"} regions=${report.source_regions} code=${report.code_regions} ` +
    `asm=${report.asm_regions} assets=${report.asset_regions} source_bytes=${report.source_bytes} ` +
    `unowned_bytes=${report.unowned_bytes} asm_c_debt_bytes=${report.asm_c_debt_bytes} ` +
    `asm_retained_structural_bytes=${report.asm_retained_structural_bytes} ` +
    `total_decompilation=${report.total_decompilation_percent.toFixed(2)}%` +
    (args.sourceOnly ? "" : ` rom_fallback_bytes=${report.rom_fallback_bytes}`),
  );
  console.log(formatProgress(report.source_bytes, report.rom_size));
}

if (import.meta.main) await main();
