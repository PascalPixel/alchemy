#!/usr/bin/env bun

export const ROM_BASE = 0x08000000;
export const ROM_SIZE = 0x00800000;
export const AUDIO_ADDRESS = 0x080fb792;
export const RESOURCE_ADDRESS = 0x08320000;

type JsonObject = Record<string, unknown>;

interface Interval {
  address: number;
  end: number;
  size: number;
}

interface BandReport {
  name: "main" | "audio" | "resources";
  address: string;
  end: string;
  unowned_bytes: number;
  gaps: number;
}

interface MissingResource {
  id: string;
  name: string;
  address: string;
  end: string;
  size: number;
}

export interface ResidualAudit {
  format: 1;
  kind: "alchemy-source-residual-audit";
  verification: "source_only";
  rom_base: string;
  rom_size: number;
  source_regions: number;
  source_bytes: number;
  unowned_bytes: number;
  gaps: number;
  project_completion_status: "audit_pending";
  project_completion_percent: null;
  byte_reconstruction_bytes: number;
  byte_reconstruction_remaining_bytes: number;
  byte_reconstruction_percent: number;
  total_decompilation_bytes: number;
  total_decompilation_remaining_bytes: number;
  total_decompilation_percent: number;
  total_decompilation_semantics: "deprecated_alias_for_byte_reconstruction";
  bands: BandReport[];
  missing_resources: MissingResource[];
  missing_resource_bytes: number;
}

interface Inputs {
  full: unknown;
  fallback: unknown;
  claimed: unknown;
  assembly: unknown;
  assets: unknown;
  directory: unknown;
}

interface Options {
  fullReport: string;
  fallbackManifest?: string;
  claimedManifest: string;
  asmManifest: string;
  assetManifest: string;
  resourceDirectory: string;
  json: boolean;
}

function object(value: unknown, label: string): JsonObject {
  if (typeof value !== "object" || value === null || Array.isArray(value)) {
    throw new Error(`${label} must be an object`);
  }
  return value as JsonObject;
}

function integer(value: unknown, label: string, minimum = 0, maximum = 0xffffffff): number {
  const parsed = typeof value === "string" ? Number(value) : value;
  if (!Number.isSafeInteger(parsed) || Number(parsed) < minimum || Number(parsed) > maximum) {
    throw new Error(`${label} must be an integer`);
  }
  return Number(parsed);
}

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function resourceName(index: number, count: number): string {
  const width = Math.max(3, (count - 1).toString(16).length);
  return `resource_${index.toString(16).padStart(width, "0")}`;
}

function regions(value: unknown, label: string, base: number, size: number): Interval[] {
  const manifest = object(value, label);
  if (manifest.format !== 1 || !Array.isArray(manifest.regions)) {
    throw new Error(`${label} has an unsupported format`);
  }
  if (manifest.rom_base !== undefined && integer(manifest.rom_base, `${label} ROM base`) !== base) {
    throw new Error(`${label} ROM base differs`);
  }
  if (manifest.rom_size !== undefined && integer(manifest.rom_size, `${label} ROM size`) !== size) {
    throw new Error(`${label} ROM size differs`);
  }
  if (manifest.verification !== undefined && manifest.verification !== "source_only") {
    throw new Error(`${label} is not source-only`);
  }
  return manifest.regions.map((raw, index) => {
    const region = object(raw, `${label} region ${index}`);
    const address = integer(region.address, `${label} region ${index} address`);
    const bytes = integer(region.size, `${label} region ${index} size`, 1);
    const end = address + bytes;
    if (!Number.isSafeInteger(end) || address < base || end > base + size) {
      throw new Error(`${label} region ${index} lies outside the ROM`);
    }
    if (region.end !== undefined && integer(region.end, `${label} region ${index} end`) !== end) {
      throw new Error(`${label} region ${index} end differs`);
    }
    return { address, end, size: bytes };
  });
}

function mark(mask: Uint8Array, source: Interval[], label: string, base: number): void {
  for (let index = 0; index < source.length; index++) {
    const region = source[index];
    const start = region.address - base;
    const end = region.end - base;
    if (mask.subarray(start, end).some((value) => value !== 0)) {
      throw new Error(`${label} region ${index} overlaps another source`);
    }
    mask.fill(1, start, end);
  }
}

export function unownedRegions(mask: Uint8Array, base = ROM_BASE): Interval[] {
  const result: Interval[] = [];
  let start = -1;
  for (let index = 0; index <= mask.length; index++) {
    const value = index === mask.length ? 1 : mask[index];
    if (value !== 0 && value !== 1) throw new Error(`coverage byte ${index} differs`);
    if (value === 0 && start < 0) start = index;
    else if (value === 1 && start >= 0) {
      result.push({ address: base + start, end: base + index, size: index - start });
      start = -1;
    }
  }
  return result;
}

function compareFallback(value: unknown, expected: Interval[], base: number, size: number): void {
  const actual = regions(value, "fallback manifest", base, size);
  if (actual.length !== expected.length || actual.some((region, index) =>
    region.address !== expected[index].address || region.size !== expected[index].size)) {
    throw new Error("fallback manifest differs from source ownership");
  }
}

function band(name: BandReport["name"], address: number, end: number, gaps: Interval[]): BandReport {
  let bytes = 0;
  let count = 0;
  for (const gap of gaps) {
    const overlap = Math.max(0, Math.min(end, gap.end) - Math.max(address, gap.address));
    if (overlap === 0) continue;
    bytes += overlap;
    count++;
  }
  return {
    name,
    address: hexadecimal(address),
    end: hexadecimal(end),
    unowned_bytes: bytes,
    gaps: count,
  };
}

function resourcePointers(value: unknown, base: number, size: number): Array<{ index: number; address: number }> {
  const directory = object(value, "resource directory");
  if (directory.format !== 1 || directory.kind !== "golden-sun-resource-directory") {
    throw new Error("resource directory has an unsupported format");
  }
  const count = integer(directory.slot_count, "resource directory slot count", 2, 0x10000);
  if (!Array.isArray(directory.slots) || directory.slots.length !== count) {
    throw new Error("resource directory slot count differs");
  }
  const pointers: Array<{ index: number; address: number }> = [];
  const seen = new Set<number>();
  directory.slots.forEach((slot, index) => {
    if (typeof slot !== "string") throw new Error(`resource directory slot ${index} differs`);
    if (!slot.startsWith("resource:")) return;
    const raw = slot.slice("resource:".length);
    if (!/^0x[0-9a-f]{8}$/.test(raw)) throw new Error(`resource directory slot ${index} is not canonical`);
    const address = integer(raw, `resource directory slot ${index} address`, base, base + size - 1);
    if (seen.has(address)) throw new Error(`resource directory pointer ${hexadecimal(address)} is duplicated`);
    seen.add(address);
    pointers.push({ index, address });
  });
  return pointers;
}

function missingResources(mask: Uint8Array, value: unknown, base: number): MissingResource[] {
  const directory = object(value, "resource directory");
  const count = integer(directory.slot_count, "resource directory slot count", 2, 0x10000);
  const pointers = resourcePointers(value, base, mask.length);
  const result: MissingResource[] = [];
  for (let position = 0; position < pointers.length; position++) {
    const current = pointers[position];
    const next = pointers.slice(position + 1).find((item) => item.address > current.address);
    const end = next?.address ?? base + mask.length;
    if (end <= current.address) continue;
    const source = mask.subarray(current.address - base, end - base);
    if (source.length === 0 || source.some((value) => value !== 0)) continue;
    const name = resourceName(current.index, count);
    result.push({
      id: `0x${current.index.toString(16).padStart(name.length - "resource_".length, "0")}`,
      name,
      address: hexadecimal(current.address),
      end: hexadecimal(end),
      size: end - current.address,
    });
  }
  return result;
}

export function auditResiduals(inputs: Inputs): ResidualAudit {
  const full = object(inputs.full, "full report");
  if (full.format !== 1 || full.verification !== "source_only" || full.byte_identical !== false || full.output !== null) {
    throw new Error("full report is not a source-only build");
  }
  const base = integer(full.rom_base, "full report ROM base");
  const size = integer(full.rom_size, "full report ROM size", 1);
  if (base !== ROM_BASE || size !== ROM_SIZE) throw new Error("full report target differs");
  const claimed = regions(inputs.claimed, "claimed manifest", base, size);
  const assembly = regions(inputs.assembly, "assembly manifest", base, size);
  const assets = regions(inputs.assets, "asset manifest", base, size);
  const mask = new Uint8Array(size);
  mark(mask, claimed, "claimed manifest", base);
  mark(mask, assembly, "assembly manifest", base);
  mark(mask, assets, "asset manifest", base);
  const gaps = unownedRegions(mask, base);
  compareFallback(inputs.fallback, gaps, base, size);
  const sourceBytes = mask.reduce((sum, value) => sum + value, 0);
  const unownedBytes = gaps.reduce((sum, region) => sum + region.size, 0);
  const codeBytes = claimed.reduce((sum, region) => sum + region.size, 0);
  const asmBytes = assembly.reduce((sum, region) => sum + region.size, 0);
  const assetBytes = assets.reduce((sum, region) => sum + region.size, 0);
  const sourceRegions = claimed.length + assembly.length + assets.length;
  const expected = [
    ["code_regions", claimed.length],
    ["code_bytes", codeBytes],
    ["asm_regions", assembly.length],
    ["asm_bytes", asmBytes],
    ["asset_regions", assets.length],
    ["asset_bytes", assetBytes],
    ["source_regions", sourceRegions],
    ["source_bytes", sourceBytes],
    ["unowned_bytes", unownedBytes],
    ["unowned_regions", gaps.length],
    ["rom_fallback_bytes", unownedBytes],
    ["fallback_regions", gaps.length],
  ] as const;
  for (const [key, value] of expected) {
    if (integer(full[key], `full report ${key}`) !== value) throw new Error(`full report ${key} differs`);
  }
  if (sourceBytes + unownedBytes !== size) throw new Error("source ownership does not span the ROM");
  const asmDebtBytes = integer(full.asm_c_debt_bytes, "full report asm_c_debt_bytes");
  const retainedAsmBytes = integer(full.asm_retained_structural_bytes, "full report asm_retained_structural_bytes");
  if (asmDebtBytes + retainedAsmBytes !== asmBytes) throw new Error("assembly accounting differs");
  const byteReconstructionBytes = codeBytes + assetBytes + retainedAsmBytes;
  const byteReconstructionRemainingBytes = asmDebtBytes + unownedBytes;
  const byteReconstructionPercent = Number((byteReconstructionBytes * 100 / size).toFixed(2));
  if (byteReconstructionBytes + byteReconstructionRemainingBytes !== size ||
      integer(full.byte_reconstruction_bytes, "full report byte_reconstruction_bytes") !== byteReconstructionBytes ||
      integer(full.byte_reconstruction_remaining_bytes, "full report byte_reconstruction_remaining_bytes") !== byteReconstructionRemainingBytes ||
      full.byte_reconstruction_percent !== byteReconstructionPercent) {
    throw new Error("byte reconstruction accounting differs");
  }
  if (integer(full.total_decompilation_bytes, "full report total_decompilation_bytes") !== byteReconstructionBytes ||
      integer(full.total_decompilation_remaining_bytes, "full report total_decompilation_remaining_bytes") !== byteReconstructionRemainingBytes ||
      full.total_decompilation_percent !== byteReconstructionPercent ||
      full.total_decompilation_semantics !== "deprecated_alias_for_byte_reconstruction") {
    throw new Error("deprecated byte reconstruction aliases differ");
  }
  const projectCompletion = object(full.project_completion, "full report project_completion");
  if (projectCompletion.status !== "audit_pending" || projectCompletion.percent !== null ||
      projectCompletion.scoring !== "withheld_until_all_dimensions_are_audited") {
    throw new Error("project completion must remain unscored while its audit is pending");
  }
  const resources = missingResources(mask, inputs.directory, base);
  const bands = [
    band("main", base, AUDIO_ADDRESS, gaps),
    band("audio", AUDIO_ADDRESS, RESOURCE_ADDRESS, gaps),
    band("resources", RESOURCE_ADDRESS, base + size, gaps),
  ];
  if (bands.reduce((sum, item) => sum + item.unowned_bytes, 0) !== unownedBytes) {
    throw new Error("residual bands do not span the ROM");
  }
  return {
    format: 1,
    kind: "alchemy-source-residual-audit",
    verification: "source_only",
    rom_base: hexadecimal(base),
    rom_size: size,
    source_regions: sourceRegions,
    source_bytes: sourceBytes,
    unowned_bytes: unownedBytes,
    gaps: gaps.length,
    project_completion_status: "audit_pending",
    project_completion_percent: null,
    byte_reconstruction_bytes: byteReconstructionBytes,
    byte_reconstruction_remaining_bytes: byteReconstructionRemainingBytes,
    byte_reconstruction_percent: byteReconstructionPercent,
    total_decompilation_bytes: byteReconstructionBytes,
    total_decompilation_remaining_bytes: byteReconstructionRemainingBytes,
    total_decompilation_percent: byteReconstructionPercent,
    total_decompilation_semantics: "deprecated_alias_for_byte_reconstruction",
    bands,
    missing_resources: resources,
    missing_resource_bytes: resources.reduce((sum, resource) => sum + resource.size, 0),
  };
}

function testInputs(): Inputs {
  const gapList = [
    { address: ROM_BASE + 2, size: 2 },
    { address: AUDIO_ADDRESS + 1, size: 3 },
    { address: 0x08320020, size: 16 },
  ];
  const claimedRegions = [
    { address: ROM_BASE, size: 2 },
    { address: ROM_BASE + 4, size: AUDIO_ADDRESS + 1 - (ROM_BASE + 4) },
  ];
  const assemblyRegions = [
    { address: AUDIO_ADDRESS + 4, size: 0x08320020 - (AUDIO_ADDRESS + 4) },
  ];
  const assetRegions = [
    { address: 0x08320030, size: ROM_BASE + ROM_SIZE - 0x08320030 },
  ];
  const sourceRegions = claimedRegions.length + assemblyRegions.length + assetRegions.length;
  const unownedBytes = gapList.reduce((sum, item) => sum + item.size, 0);
  const codeBytes = claimedRegions.reduce((sum, item) => sum + item.size, 0);
  const asmBytes = assemblyRegions.reduce((sum, item) => sum + item.size, 0);
  const assetBytes = assetRegions.reduce((sum, item) => sum + item.size, 0);
  const byteReconstructionBytes = codeBytes + assetBytes;
  const byteReconstructionRemainingBytes = asmBytes + unownedBytes;
  const byteReconstructionPercent = Number((byteReconstructionBytes * 100 / ROM_SIZE).toFixed(2));
  return {
    full: {
      format: 1,
      rom_base: ROM_BASE,
      rom_size: ROM_SIZE,
      code_regions: claimedRegions.length,
      code_bytes: codeBytes,
      asm_regions: assemblyRegions.length,
      asm_bytes: asmBytes,
      asm_c_debt_bytes: asmBytes,
      asm_retained_structural_bytes: 0,
      asset_regions: assetRegions.length,
      asset_bytes: assetBytes,
      source_regions: sourceRegions,
      source_bytes: ROM_SIZE - unownedBytes,
      unowned_bytes: unownedBytes,
      unowned_regions: gapList.length,
      project_completion: {
        status: "audit_pending",
        percent: null,
        scoring: "withheld_until_all_dimensions_are_audited",
      },
      byte_reconstruction_bytes: byteReconstructionBytes,
      byte_reconstruction_remaining_bytes: byteReconstructionRemainingBytes,
      byte_reconstruction_percent: byteReconstructionPercent,
      total_decompilation_bytes: byteReconstructionBytes,
      total_decompilation_remaining_bytes: byteReconstructionRemainingBytes,
      total_decompilation_percent: byteReconstructionPercent,
      total_decompilation_semantics: "deprecated_alias_for_byte_reconstruction",
      rom_fallback_bytes: unownedBytes,
      fallback_regions: gapList.length,
      verification: "source_only",
      byte_identical: false,
      output: null,
    },
    fallback: { format: 1, rom_base: ROM_BASE, rom_size: ROM_SIZE, regions: gapList },
    claimed: { format: 1, rom_base: ROM_BASE, rom_size: ROM_SIZE, verification: "source_only", regions: claimedRegions },
    assembly: { format: 1, rom_base: ROM_BASE, verification: "source_only", regions: assemblyRegions },
    assets: { format: 1, rom_base: ROM_BASE, rom_size: ROM_SIZE, regions: assetRegions },
    directory: {
      format: 1,
      kind: "golden-sun-resource-directory",
      address: "0x08320000",
      slot_count: 4,
      slots: ["rom-base", "directory-self", "resource:0x08320020", "resource:0x08320030"],
    },
  };
}

function mustReject(inputs: Inputs, pattern: RegExp): void {
  try {
    auditResiduals(inputs);
  } catch (error) {
    if (pattern.test((error as Error).message)) return;
    throw error;
  }
  throw new Error(`audit accepted invalid input for ${pattern}`);
}

export function selfTest(): void {
  const inputs = testInputs();
  const report = auditResiduals(inputs);
  if (report.unowned_bytes !== 21 || report.gaps !== 3 || report.source_bytes !== ROM_SIZE - 21 ||
      report.project_completion_status !== "audit_pending" || report.project_completion_percent !== null ||
      report.byte_reconstruction_bytes + report.byte_reconstruction_remaining_bytes !== ROM_SIZE ||
      report.bands[0].unowned_bytes !== 2 || report.bands[0].gaps !== 1 ||
      report.bands[1].unowned_bytes !== 3 || report.bands[1].gaps !== 1 ||
      report.bands[2].unowned_bytes !== 16 || report.bands[2].gaps !== 1 ||
      report.missing_resources.length !== 1 || report.missing_resources[0].name !== "resource_002" ||
      report.missing_resources[0].size !== 16 || report.missing_resource_bytes !== 16) {
    throw new Error("residual audit self-test differs");
  }
  const overlap = structuredClone(inputs);
  (overlap.assembly as JsonObject).regions = [
    ...((overlap.assembly as JsonObject).regions as unknown[]),
    { address: ROM_BASE, size: 1 },
  ];
  (overlap.full as JsonObject).asm_regions = 2;
  (overlap.full as JsonObject).source_regions = 5;
  mustReject(overlap, /overlaps/);
  const fallback = structuredClone(inputs);
  (((fallback.fallback as JsonObject).regions as JsonObject[])[0]).size = 1;
  mustReject(fallback, /fallback manifest differs/);
  const verified = structuredClone(inputs);
  (verified.full as JsonObject).verification = "rom";
  mustReject(verified, /not a source-only build/);
  const duplicate = structuredClone(inputs);
  (duplicate.directory as JsonObject).slots = [
    "rom-base", "directory-self", "resource:0x08320020", "resource:0x08320020",
  ];
  mustReject(duplicate, /duplicated/);
  console.log("self-test=ok");
}

function usage(): void {
  console.log(
    "usage: audit_residuals.ts [--full-report FILE] [--fallback-manifest FILE] " +
    "[--claimed-manifest FILE] [--asm-manifest FILE] [--asset-manifest FILE] " +
    "[--resource-directory FILE] [--json] | --self-test",
  );
}

function parseArgs(argv: string[]): Options {
  const options: Options = {
    fullReport: "out/full/rebuilt.json",
    claimedManifest: "out/full/claimed/manifest.json",
    asmManifest: "out/full/asm/manifest.json",
    assetManifest: "out/full/assets/manifest.json",
    resourceDirectory: "assets/data/resource_directory.json",
    json: false,
  };
  const keys: Record<string, keyof Options> = {
    "--full-report": "fullReport",
    "--fallback-manifest": "fallbackManifest",
    "--claimed-manifest": "claimedManifest",
    "--asm-manifest": "asmManifest",
    "--asset-manifest": "assetManifest",
    "--resource-directory": "resourceDirectory",
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--json") {
      options.json = true;
      continue;
    }
    const equal = argument.indexOf("=");
    const name = equal < 0 ? argument : argument.slice(0, equal);
    if (!(name in keys)) throw new Error(`unrecognized argument: ${argument}`);
    const value = equal < 0 ? argv[++index] : argument.slice(equal + 1);
    if (value === undefined || value.length === 0) throw new Error(`${name} requires a value`);
    options[keys[name]] = value as never;
  }
  return options;
}

async function readJson(path: string, label: string): Promise<unknown> {
  const file = Bun.file(path);
  if (!(await file.exists())) throw new Error(`${label} does not exist: ${path}`);
  try {
    return await file.json();
  } catch {
    throw new Error(`${label} is not valid JSON: ${path}`);
  }
}

function printReport(report: ResidualAudit): void {
  console.log(
    `source_only=True source_regions=${report.source_regions} source_bytes=${report.source_bytes} ` +
    `unowned_bytes=${report.unowned_bytes} gaps=${report.gaps} ` +
    `project_completion=${report.project_completion_status} ` +
    `byte_reconstruction=${report.byte_reconstruction_percent.toFixed(2)}%`,
  );
  for (const item of report.bands) {
    console.log(
      `band=${item.name} address=${item.address} end=${item.end} ` +
      `unowned_bytes=${item.unowned_bytes} gaps=${item.gaps}`,
    );
  }
  console.log(`missing_resources=${report.missing_resources.length} bytes=${report.missing_resource_bytes}`);
  for (const resource of report.missing_resources) {
    console.log(
      `${resource.name} address=${resource.address} end=${resource.end} bytes=${resource.size}`,
    );
  }
}

async function main(argv: string[]): Promise<void> {
  if (argv.includes("-h") || argv.includes("--help")) {
    usage();
    return;
  }
  if (argv.includes("--self-test")) {
    if (argv.length !== 1) throw new Error("--self-test does not accept other arguments");
    selfTest();
    return;
  }
  const options = parseArgs(argv);
  const full = await readJson(options.fullReport, "full report") as JsonObject;
  const fallbackPath = options.fallbackManifest ?? String(full.fallback_manifest ?? "");
  if (fallbackPath.length === 0) throw new Error("full report has no fallback manifest");
  const report = auditResiduals({
    full,
    fallback: await readJson(fallbackPath, "fallback manifest"),
    claimed: await readJson(options.claimedManifest, "claimed manifest"),
    assembly: await readJson(options.asmManifest, "assembly manifest"),
    assets: await readJson(options.assetManifest, "asset manifest"),
    directory: await readJson(options.resourceDirectory, "resource directory"),
  });
  if (options.json) console.log(JSON.stringify(report, null, 2));
  else printReport(report);
}

if (import.meta.main) await main(Bun.argv.slice(2));
