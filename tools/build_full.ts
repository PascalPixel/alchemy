#!/usr/bin/env bun
import { existsSync, mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { dirname, extname, isAbsolute, join, resolve } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;

interface Options {
  rom: string;
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

function rooted(path: string): string {
  return isAbsolute(path) ? path : resolve(ROOT, path);
}

function usage(): void {
  console.log(
    "usage: build_full.ts [-h] [-o OUTPUT] [--claimed-output CLAIMED_OUTPUT] " +
    "[--asm-output ASM_OUTPUT] [--asset-manifest ASSET_MANIFEST] " +
    "[--asset-output ASSET_OUTPUT] [--jobs JOBS] [rom]",
  );
}

function parseArgs(argv: string[]): Options {
  const options: Options = {
    rom: "gs1-en.gba",
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
  const args = parseArgs(Bun.argv.slice(2));
  const romPath = resolve(process.cwd(), args.rom);
  const rom = readFileSync(romPath);
  const claimed = rooted(args.claimedOutput);
  await run([
    process.execPath, "tools/build_claimed.ts", romPath,
    "--jobs", String(args.jobs), "--output", claimed,
  ]);
  const manifest = readJson(join(claimed, "manifest.json"));
  const image = readFileSync(join(claimed, "claimed.bin"));
  const imageBase = Number(manifest.image_base);
  const rebuilt = Buffer.from(rom);
  const claimedMask = Buffer.alloc(rom.length);
  for (const region of manifest.regions as Region[]) {
    const address = Number(region.address);
    const size = Number(region.size);
    const source = image.subarray(address - imageBase, address - imageBase + size);
    const expected = rom.subarray(address - ROM_BASE, address - ROM_BASE + size);
    if (!source.equals(expected)) {
      throw new Error(`source mismatch at 0x${address.toString(16).padStart(8, "0")} (0x${size.toString(16)} bytes)`);
    }
    const start = address - ROM_BASE;
    source.copy(rebuilt, start);
    claimedMask.fill(1, start, start + size);
  }

  const asmRegions: Region[] = [];
  const asmDirectory = join(ROOT, "asm");
  if (existsSync(asmDirectory) && readdirSync(asmDirectory).some((name) => name.endsWith(".s"))) {
    const asmOutput = rooted(args.asmOutput);
    await run([process.execPath, "tools/build_asm.ts", romPath, "--output", asmOutput]);
    const asmManifest = readJson(join(asmOutput, "manifest.json"));
    for (const region of asmManifest.regions as Region[]) {
      const address = Number(region.address);
      const size = Number(region.size);
      const start = address - ROM_BASE;
      const end = start + size;
      if (claimedMask.subarray(start, end).some((value) => value !== 0)) {
        throw new Error(`assembly overlaps another source at 0x${address.toString(16).padStart(8, "0")}`);
      }
      const source = readFileSync(region.output);
      if (!source.equals(rom.subarray(start, end))) {
        throw new Error(`assembly mismatch at 0x${address.toString(16).padStart(8, "0")} (0x${size.toString(16)} bytes)`);
      }
      source.copy(rebuilt, start);
      claimedMask.fill(1, start, end);
      asmRegions.push(region);
    }
  }

  const assetRegions: Region[] = [];
  const assetManifestPath = rooted(args.assetManifest);
  if (existsSync(assetManifestPath)) {
    const assetOutput = rooted(args.assetOutput);
    await run([
      process.execPath, "tools/build_assets.ts", romPath,
      "--manifest", assetManifestPath, "--output", assetOutput,
    ]);
    const assetManifest = readJson(join(assetOutput, "manifest.json"));
    for (const region of assetManifest.regions as Region[]) {
      const address = Number(region.address);
      const size = Number(region.size);
      const start = address - ROM_BASE;
      const end = start + size;
      if (claimedMask.subarray(start, end).some((value) => value !== 0)) {
        throw new Error(`asset overlaps another source at 0x${address.toString(16).padStart(8, "0")}`);
      }
      const source = readFileSync(region.output);
      const expected = rom.subarray(start, end);
      if (!source.equals(expected)) {
        throw new Error(`asset mismatch at 0x${address.toString(16).padStart(8, "0")} (0x${size.toString(16)} bytes)`);
      }
      source.copy(rebuilt, start);
      claimedMask.fill(1, start, end);
      assetRegions.push(region);
    }
  }
  if (!rebuilt.equals(rom)) throw new Error("full rebuild differs from private ROM");
  const output = rooted(args.output);
  mkdirSync(dirname(output), { recursive: true });
  writeFileSync(output, rebuilt);
  const sourceBytes = claimedMask.reduce((sum, value) => sum + value, 0);
  const report = {
    format: 1,
    rom_base: ROM_BASE,
    rom_size: rom.length,
    code_regions: manifest.regions.length,
    asm_regions: asmRegions.length,
    asset_regions: assetRegions.length,
    source_regions: manifest.regions.length + asmRegions.length + assetRegions.length,
    source_bytes: sourceBytes,
    rom_fallback_bytes: rom.length - sourceBytes,
    byte_identical: true,
    output: args.output,
  };
  const suffix = extname(output);
  const reportPath = suffix ? output.slice(0, -suffix.length) + ".json" : output + ".json";
  writeFileSync(reportPath, JSON.stringify(report, null, 2) + "\n");
  console.log(
    `identical=True regions=${report.source_regions} code=${report.code_regions} ` +
    `asm=${report.asm_regions} assets=${report.asset_regions} source_bytes=${report.source_bytes} ` +
    `rom_fallback_bytes=${report.rom_fallback_bytes}`,
  );
}

if (import.meta.main) await main();
