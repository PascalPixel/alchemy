#!/usr/bin/env bun
import {
  existsSync,
  lstatSync,
  mkdirSync,
  mkdtempSync,
  readFileSync,
  readdirSync,
  realpathSync,
  renameSync,
  rmSync,
  writeFileSync,
} from "fs";
import { basename, dirname, isAbsolute, join, relative, resolve, sep } from "path";
import { tmpdir } from "os";
import { buildWaveRecord } from "./audio_wave.ts";
import { publicationEntryReason, publicationPathReason } from "./check_publication.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ADAPTERS = [
  "gba-header-fields",
  "audio-wave",
  "relocated-arm",
  "linker-veneer",
  "resource-series",
  "early-data",
  "late-data",
  "executable-gap",
  "identified-residual",
  "resource-alignment",
  "packed-runtime",
  "additional-veneer-islands",
  "runtime-support-residual",
  "asset-boundary",
  "nonstandard-alignment",
] as const;

type Adapter = typeof ADAPTERS[number];
type Json = Record<string, unknown>;

interface CommonStep {
  id: string;
  adapter: Adapter;
  destination: string;
  ownership: ClosureOwnership[];
}

export interface ClosureOwnership {
  address: string;
  size: number;
  component: "asm" | "asset";
}

interface AudioWaveStep extends CommonStep {
  adapter: "audio-wave";
  tones: string;
  start: string;
  end: string;
}

interface HeaderStep extends CommonStep {
  adapter: "gba-header-fields";
  template: string;
}

interface RelocatedArmStep extends CommonStep {
  adapter: "relocated-arm";
  layout: string;
}

interface ResourceSeriesStep extends CommonStep {
  adapter: "resource-series";
  catalog: string;
}

interface VeneerStep extends CommonStep {
  adapter: "linker-veneer";
  layout: string;
}

interface EarlyDataStep extends CommonStep {
  adapter: "early-data";
}

interface LateDataStep extends CommonStep {
  adapter: "late-data";
  catalog: string;
}

interface CommandStep extends CommonStep {
  adapter: "identified-residual" | "resource-alignment" | "packed-runtime" |
    "additional-veneer-islands" | "runtime-support-residual" | "asset-boundary" | "nonstandard-alignment";
  tool: string;
  arguments: string[];
}

interface ExecutableGapStep extends CommonStep {
  adapter: "executable-gap";
  plan: string;
}

type ClosureStep = HeaderStep | AudioWaveStep | RelocatedArmStep | ResourceSeriesStep | VeneerStep | EarlyDataStep |
  LateDataStep | ExecutableGapStep | CommandStep;

export interface ClosurePlan {
  format: 1;
  kind: "golden-sun-byte-closure-plan";
  canonical_rom: "gs1-en.gba";
  steps: ClosureStep[];
}

export interface ClosureResult {
  source_bytes: number;
  unowned_bytes: number;
  unowned_regions: number;
}

function object(value: unknown, label: string): Json {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${label} must be an object`);
  return value as Json;
}

function exactKeys(value: Json, expected: string[], label: string): void {
  const actual = Object.keys(value).sort(), wanted = [...expected].sort();
  if (actual.length !== wanted.length || actual.some((key, index) => key !== wanted[index])) {
    throw new Error(`${label} fields differ`);
  }
}

function canonicalRelative(value: unknown, label: string): string {
  if (typeof value !== "string" || value.length === 0 || isAbsolute(value) || value.includes("\\") ||
      value.split("/").some((part) => part === "" || part === "." || part === "..")) {
    throw new Error(`${label} must be a canonical relative path`);
  }
  return value;
}

function canonicalAddress(value: unknown, label: string): string {
  if (typeof value !== "string" || !/^0x[0-9a-f]{8}$/.test(value)) throw new Error(`${label} must be a canonical address`);
  return value;
}

function slug(value: unknown, label: string): string {
  if (typeof value !== "string" || !/^[a-z][a-z0-9-]*$/.test(value)) throw new Error(`${label} must be a slug`);
  return value;
}

function stringArray(value: unknown, label: string): string[] {
  if (!Array.isArray(value) || value.some((item) => typeof item !== "string")) throw new Error(`${label} must be strings`);
  return value as string[];
}

function ownership(value: unknown, label: string): ClosureOwnership[] {
  if (!Array.isArray(value) || value.length === 0) throw new Error(`${label} must be a nonempty array`);
  return value.map((raw, index) => {
    const claim = object(raw, `${label} ${index}`);
    exactKeys(claim, ["address", "size", "component"], `${label} ${index}`);
    const address = canonicalAddress(claim.address, `${label} ${index} address`);
    if (!Number.isSafeInteger(claim.size) || Number(claim.size) <= 0 ||
        (claim.component !== "asm" && claim.component !== "asset")) {
      throw new Error(`${label} ${index} extent or component differs`);
    }
    return { address, size: Number(claim.size), component: claim.component };
  });
}

export function parseClosurePlan(text: string): ClosurePlan {
  const parsed: unknown = JSON.parse(text);
  if (text !== `${JSON.stringify(parsed, null, 2)}\n`) throw new Error("closure plan must be canonical JSON");
  const plan = object(parsed, "closure plan");
  exactKeys(plan, ["format", "kind", "canonical_rom", "steps"], "closure plan");
  if (plan.format !== 1 || plan.kind !== "golden-sun-byte-closure-plan" || plan.canonical_rom !== "gs1-en.gba") {
    throw new Error("closure plan identity differs");
  }
  if (!Array.isArray(plan.steps) || plan.steps.length < ADAPTERS.length) throw new Error("closure plan steps differ");
  const ids = new Set<string>(), destinations = new Set<string>(), adapters = new Set<Adapter>();
  const steps = plan.steps.map((raw, index): ClosureStep => {
    const step = object(raw, `closure step ${index}`);
    const adapter = step.adapter;
    if (typeof adapter !== "string" || !(ADAPTERS as readonly string[]).includes(adapter)) {
      throw new Error(`closure step ${index} adapter differs`);
    }
    const id = slug(step.id, `closure step ${index} id`);
    const destination = canonicalRelative(step.destination, `closure step ${index} destination`);
    if (destination === ".git" || destination.startsWith(".git/") || destination === "out" ||
        destination.startsWith("out/") || destination === "work" || destination.startsWith("work/")) {
      throw new Error(`closure step ${index} destination is private or generated`);
    }
    if (ids.has(id)) throw new Error(`duplicate closure step ${id}`);
    if (destinations.has(destination)) throw new Error(`duplicate closure destination ${destination}`);
    ids.add(id); destinations.add(destination); adapters.add(adapter as Adapter);
    const plannedOwnership = ownership(step.ownership, `closure step ${index} ownership`);
    const common = { id, adapter: adapter as Adapter, destination, ownership: plannedOwnership };
    if (adapter === "gba-header-fields") {
      exactKeys(step, ["id", "adapter", "destination", "ownership", "template"], `closure step ${index}`);
      return { ...common, adapter, template: canonicalRelative(step.template, `closure step ${index} template`) };
    }
    if (adapter === "audio-wave") {
      exactKeys(step, ["id", "adapter", "destination", "ownership", "tones", "start", "end"], `closure step ${index}`);
      const start = canonicalAddress(step.start, `closure step ${index} start`);
      const end = canonicalAddress(step.end, `closure step ${index} end`);
      if (Number(start) >= Number(end)) throw new Error(`closure step ${index} wave extent differs`);
      return { ...common, adapter, tones: canonicalRelative(step.tones, `closure step ${index} tones`), start, end };
    }
    if (adapter === "relocated-arm") {
      exactKeys(step, ["id", "adapter", "destination", "ownership", "layout"], `closure step ${index}`);
      return { ...common, adapter, layout: canonicalRelative(step.layout, `closure step ${index} layout`) };
    }
    if (adapter === "resource-series") {
      exactKeys(step, ["id", "adapter", "destination", "ownership", "catalog"], `closure step ${index}`);
      return { ...common, adapter, catalog: canonicalRelative(step.catalog, `closure step ${index} catalog`) };
    }
    if (adapter === "linker-veneer") {
      exactKeys(step, ["id", "adapter", "destination", "ownership", "layout"], `closure step ${index}`);
      return { ...common, adapter, layout: canonicalRelative(step.layout, `closure step ${index} layout`) };
    }
    if (adapter === "early-data") {
      exactKeys(step, ["id", "adapter", "destination", "ownership"], `closure step ${index}`);
      return { ...common, adapter };
    }
    if (adapter === "late-data") {
      exactKeys(step, ["id", "adapter", "destination", "ownership", "catalog"], `closure step ${index}`);
      return { ...common, adapter, catalog: canonicalRelative(step.catalog, `closure step ${index} catalog`) };
    }
    if (adapter === "executable-gap") {
      exactKeys(step, ["id", "adapter", "destination", "ownership", "plan"], `closure step ${index}`);
      return { ...common, adapter, plan: canonicalRelative(step.plan, `closure step ${index} plan`) };
    }
    exactKeys(step, ["id", "adapter", "destination", "ownership", "tool", "arguments"], `closure step ${index}`);
    const tool = canonicalRelative(step.tool, `closure step ${index} tool`);
    if (!tool.startsWith("tools/") || !tool.endsWith(".ts")) throw new Error(`closure step ${index} tool must be TypeScript under tools`);
    const arguments_ = stringArray(step.arguments, `closure step ${index} arguments`);
    if (!arguments_.includes("{rom}") || !arguments_.includes("{output}")) {
      throw new Error(`closure step ${index} command requires {rom} and {output}`);
    }
    return { ...common, adapter, tool, arguments: arguments_ } as CommandStep;
  });
  for (const adapter of ADAPTERS) if (!adapters.has(adapter)) throw new Error(`closure plan lacks ${adapter}`);
  const allOwnership = steps.flatMap((step) => step.ownership.map((claim) => ({ step: step.id, ...claim })))
    .sort((left, right) => Number(left.address) - Number(right.address));
  for (let index = 1; index < allOwnership.length; index++) {
    if (Number(allOwnership[index].address) < Number(allOwnership[index - 1].address) + allOwnership[index - 1].size) {
      throw new Error(`closure ownership overlaps between ${allOwnership[index - 1].step} and ${allOwnership[index].step}`);
    }
  }
  const ordered = [...destinations].sort();
  for (let left = 0; left < ordered.length; left++) {
    for (let right = left + 1; right < ordered.length; right++) {
      if (ordered[right].startsWith(`${ordered[left]}/`)) throw new Error("closure destinations may not contain one another");
    }
  }
  return { format: 1, kind: "golden-sun-byte-closure-plan", canonical_rom: "gs1-en.gba", steps };
}

function inside(root: string, path: string, label: string): string {
  const absolute = resolve(root, path), relation = relative(root, absolute);
  if (relation === "" || relation === ".." || relation.startsWith(`..${sep}`) || isAbsolute(relation)) {
    throw new Error(`${label} escapes the repository`);
  }
  return absolute;
}

function regular(path: string, label: string): void {
  if (!existsSync(path) || !lstatSync(path).isFile()) throw new Error(`${label} is not a regular file`);
}

function commandFor(step: ClosureStep, root: string, rom: string, output: string, unowned: string): string[] {
  if (step.adapter === "gba-header-fields") return [
    process.execPath, inside(root, "tools/gba_header.ts", "GBA header tool"), "export", rom,
    "--directory", output, "--template", inside(root, step.template, "GBA header template"),
  ];
  if (step.adapter === "audio-wave") return [
    process.execPath, inside(root, "tools/probe_audio_waves.ts", "audio-wave tool"), rom,
    "--tones", inside(root, step.tones, "tone source"), "--start", step.start, "--end", step.end, "-o", output,
  ];
  if (step.adapter === "relocated-arm") return [
    process.execPath, inside(root, "tools/relocated_arm_helpers.ts", "ARM helper tool"),
    "export", rom, inside(root, step.layout, "ARM helper layout"), "--directory", output,
  ];
  if (step.adapter === "resource-series") return [
    process.execPath, inside(root, "tools/kind2_resource_series.ts", "resource-series tool"),
    "export", rom, "--catalog", inside(root, step.catalog, "resource catalog"), "--directory", output,
  ];
  if (step.adapter === "linker-veneer") return [
    process.execPath, inside(root, "tools/veneer_island.ts", "linker-veneer tool"),
    "export", rom, inside(root, step.layout, "linker-veneer layout"), "--directory", output,
  ];
  if (step.adapter === "early-data") return [
    process.execPath, inside(root, "tools/early_runtime_data.ts", "early-data tool"), "export", rom, "--directory", output,
  ];
  if (step.adapter === "late-data") return [
    process.execPath, inside(root, "tools/late_runtime_data.ts", "late-data tool"),
    "export", rom, "--catalog", inside(root, step.catalog, "late-data catalog"), "--directory", output,
  ];
  if (step.adapter === "executable-gap") return [
    process.execPath, inside(root, "tools/executable_gap_sources.ts", "executable-gap tool"),
    "--plan", inside(root, step.plan, "executable-gap plan"), "--gaps", unowned, "-o", output, rom,
  ];
  const values: Record<string, string> = { "{rom}": rom, "{output}": output, "{root}": root };
  const arguments_ = step.arguments.map((argument) => values[argument] ?? argument);
  return [process.execPath, inside(root, step.tool, `${step.adapter} tool`), ...arguments_];
}

async function run(command: string[], root: string): Promise<void> {
  regular(command[1], "closure tool");
  const child = Bun.spawn(command, { cwd: root, stdout: "inherit", stderr: "inherit" });
  const code = await child.exited;
  if (code !== 0) throw new Error(`${basename(command[1])} failed with status ${code}`);
}

function remove(path: string): void {
  rmSync(path, { recursive: true, force: true });
}

function restore(installed: Array<{ destination: string; backup: string; hadPrevious: boolean }>): void {
  for (const item of [...installed].reverse()) {
    remove(item.destination);
    if (item.hadPrevious && existsSync(item.backup)) {
      mkdirSync(dirname(item.destination), { recursive: true });
      renameSync(item.backup, item.destination);
    }
  }
}

function regularFiles(directory: string): string[] {
  const result: string[] = [];
  const visit = (path: string): void => {
    for (const entry of readdirSync(path, { withFileTypes: true })) {
      const child = join(path, entry.name);
      if (entry.isSymbolicLink()) throw new Error(`${child}: staged symbolic links are forbidden`);
      if (entry.isDirectory()) visit(child);
      else if (entry.isFile()) result.push(child);
      else throw new Error(`${child}: staged special files are forbidden`);
    }
  };
  if (!lstatSync(directory).isDirectory()) return [directory];
  visit(directory);
  return result.sort();
}

function verifyPublishableOutput(step: ClosureStep, output: string): void {
  const files = regularFiles(output);
  if (files.length === 0) throw new Error(`${step.id} exporter produced an empty staged output`);
  for (const file of files) {
    const suffix = relative(output, file).split(sep).join("/");
    const path = suffix === "" ? step.destination : `${step.destination}/${suffix}`;
    const reason = publicationPathReason(path) ?? publicationEntryReason(path, readFileSync(file));
    if (reason !== undefined) throw new Error(`${step.id} staged source ${path} is not publishable: ${reason}`);
  }
}

interface FullBuildResult extends ClosureResult {
  unowned: string;
  assembly: string;
  assets: string;
}

function integer(value: unknown, label: string): number {
  if (!Number.isSafeInteger(value) || Number(value) < 0) throw new Error(`${label} differs`);
  return Number(value);
}

function fullBuildResult(path: string, verification: "source_only" | "rom", requireZero: boolean): ClosureResult {
  regular(path, `${verification} build report`);
  const report = object(JSON.parse(readFileSync(path, "utf8")), `${verification} build report`);
  const sourceBytes = integer(report.source_bytes, "source byte count");
  const unownedBytes = integer(report.unowned_bytes, "unowned byte count");
  const unownedRegions = integer(report.unowned_regions, "unowned region count");
  if (report.verification !== verification || (verification === "source_only" ?
      report.rom_fallback_applicable !== false : report.byte_identical !== true)) {
    throw new Error(`${verification} build report semantics differ`);
  }
  if (requireZero && (unownedBytes !== 0 || unownedRegions !== 0)) {
    throw new Error(`byte closure remains incomplete: ${unownedBytes} bytes in ${unownedRegions} regions`);
  }
  return { source_bytes: sourceBytes, unowned_bytes: unownedBytes, unowned_regions: unownedRegions };
}

async function fullBuild(
  root: string,
  transaction: string,
  label: string,
  rom?: string,
  requireZero = false,
): Promise<FullBuildResult> {
  const directory = join(transaction, label), output = join(directory, "rebuilt.gba");
  const claimed = join(directory, "claimed"), assembly = join(directory, "asm"), assets = join(directory, "assets");
  mkdirSync(directory, { recursive: true });
  const command = [process.execPath, inside(root, "tools/build_full.ts", `${label} build tool`)];
  if (rom === undefined) command.push("--source-only");
  else command.push(rom);
  command.push("-o", output, "--claimed-output", claimed, "--asm-output", assembly, "--asset-output", assets);
  await run(command, root);
  const report = output.replace(/\.gba$/, ".json"), result = fullBuildResult(report, rom === undefined ? "source_only" : "rom", requireZero);
  const unowned = output.replace(/\.gba$/, ".unowned.json");
  regular(unowned, `${label} unowned manifest`);
  return { ...result, unowned, assembly: join(assembly, "manifest.json"), assets: join(assets, "manifest.json") };
}

interface NumericRegion { address: number; size: number }

function regions(path: string, label: string): NumericRegion[] {
  regular(path, label);
  const manifest = object(JSON.parse(readFileSync(path, "utf8")), label);
  if (!Array.isArray(manifest.regions)) throw new Error(`${label} has no regions`);
  return manifest.regions.map((raw, index) => {
    const region = object(raw, `${label} region ${index}`);
    return { address: integer(region.address, `${label} region address`), size: integer(region.size, `${label} region size`) };
  }).sort((left, right) => left.address - right.address);
}

function verifyPreflightInventory(plan: ClosurePlan, preflight: FullBuildResult): void {
  const merge = (items: NumericRegion[]): NumericRegion[] => {
    const result: NumericRegion[] = [];
    for (const item of [...items].sort((left, right) => left.address - right.address)) {
      const previous = result.at(-1);
      if (previous !== undefined && previous.address + previous.size === item.address) previous.size += item.size;
      else result.push({ ...item });
    }
    return result;
  };
  const actual = merge(regions(preflight.unowned, "preflight unowned manifest"));
  const planned = merge(plan.steps.flatMap((step) => step.ownership.map((claim) => ({
    address: Number(claim.address), size: claim.size,
  }))));
  if (actual.length !== planned.length || actual.some((region, index) =>
    region.address !== planned[index].address || region.size !== planned[index].size)) {
    throw new Error("closure ownership does not exactly equal the current source-only unowned mask");
  }
  const bytes = planned.reduce((sum, region) => sum + region.size, 0);
  if (bytes !== preflight.unowned_bytes || actual.length !== preflight.unowned_regions) {
    throw new Error("closure ownership totals differ from source-only coverage");
  }
}

function manifestDocument(path: string, label: string): Json[] {
  regular(path, label);
  const manifest = object(JSON.parse(readFileSync(path, "utf8")), label);
  if (!Array.isArray(manifest.regions)) throw new Error(`${label} has no regions`);
  return manifest.regions.map((item, index) => object(item, `${label} region ${index}`));
}

function sourceNames(region: Json, component: "asm" | "asset"): string[] {
  if (component === "asm") {
    if (typeof region.source !== "string") throw new Error("assembly ownership region has no source");
    return [region.source];
  }
  if (!Array.isArray(region.sources) || region.sources.some((source) => typeof source !== "string")) {
    throw new Error("asset ownership region has no sources");
  }
  return region.sources as string[];
}

function verifyInstalledOwnership(plan: ClosurePlan, exact: FullBuildResult, root: string): void {
  const manifests = {
    asm: manifestDocument(exact.assembly, "exact assembly manifest"),
    asset: manifestDocument(exact.assets, "exact asset manifest"),
  };
  const referenced = new Set<string>();
  for (const region of manifests.asm) for (const source of sourceNames(region, "asm")) referenced.add(resolve(root, source));
  for (const region of manifests.asset) for (const source of sourceNames(region, "asset")) referenced.add(resolve(root, source));
  for (const step of plan.steps) {
    const destination = inside(root, step.destination, `${step.id} destination`);
    for (const claim of step.ownership) {
      const address = Number(claim.address);
      const matches = manifests[claim.component].filter((region) =>
        Number(region.address) === address && Number(region.size) === claim.size);
      if (matches.length !== 1) throw new Error(`${step.id} ownership is not an exact ${claim.component} manifest region`);
      const used = sourceNames(matches[0], claim.component).some((source) => {
        const path = resolve(root, source), relation = relative(destination, path);
        return relation === "" || (!relation.startsWith(`..${sep}`) && relation !== ".." && !isAbsolute(relation));
      });
      if (!used) throw new Error(`${step.id} installed output is not used by its exact ownership region`);
    }
    for (const file of regularFiles(destination)) {
      if (!referenced.has(resolve(file))) throw new Error(`${step.id} installed source is not consumed by an exact build manifest: ${file}`);
    }
  }
}

function verifyAudioOutput(rom: Buffer, step: AudioWaveStep, output: string): void {
  const indexPath = join(output, "index.json");
  regular(indexPath, "audio-wave index");
  const index = object(JSON.parse(readFileSync(indexPath, "utf8")), "audio-wave index");
  if (!Array.isArray(index.waves) || index.waves.length === 0) throw new Error("audio-wave index has no waves");
  for (const [position, raw] of index.waves.entries()) {
    const wave = object(raw, `audio wave ${position}`);
    if (typeof wave.source !== "string") throw new Error(`audio wave ${position} has no source`);
    const address = Number(wave.address), size = Number(wave.size);
    if (!Number.isSafeInteger(address) || !Number.isSafeInteger(size) || size <= 0) throw new Error(`audio wave ${position} extent differs`);
    const [built] = buildWaveRecord(wave as unknown as Parameters<typeof buildWaveRecord>[0], readFileSync(join(output, wave.source)));
    const expected = rom.subarray(address - 0x08000000, address - 0x08000000 + size);
    if (built.length !== size || expected.length !== size || !built.equals(expected)) {
      throw new Error(`audio wave ${position} does not rebuild its canonical ROM extent`);
    }
  }
}

export async function closeByteGaps(romPath: string, planPath: string, root = ROOT): Promise<ClosureResult> {
  root = realpathSync(root);
  const rom = resolve(romPath), planFile = resolve(planPath);
  regular(rom, "canonical ROM"); regular(planFile, "closure plan");
  if (basename(rom) !== "gs1-en.gba") throw new Error("closure requires the canonical gs1-en.gba input");
  const plan = parseClosurePlan(readFileSync(planFile, "utf8"));
  const transactionParent = inside(root, "out", "transaction parent");
  mkdirSync(transactionParent, { recursive: true });
  const transaction = mkdtempSync(join(transactionParent, ".byte-closure-"));
  const staged: Array<{ step: ClosureStep; source: string; destination: string; backup: string }> = [];
  const installed: Array<{ destination: string; backup: string; hadPrevious: boolean }> = [];
  let committed = false;
  try {
    const preflight = await fullBuild(root, transaction, "preflight", undefined, false);
    verifyPreflightInventory(plan, preflight);
    const romImage = readFileSync(rom);
    for (const [index, step] of plan.steps.entries()) {
      const output = join(transaction, "stage", `${index}-${step.id}`);
      mkdirSync(dirname(output), { recursive: true });
      await run(commandFor(step, root, rom, output, preflight.unowned), root);
      if (!existsSync(output)) throw new Error(`${step.id} exporter produced no staged output`);
      verifyPublishableOutput(step, output);
      if (step.adapter === "audio-wave") verifyAudioOutput(romImage, step, output);
      staged.push({
        step,
        source: output,
        destination: inside(root, step.destination, `${step.id} destination`),
        backup: join(transaction, "backup", `${index}-${step.id}`),
      });
    }
    for (const item of staged) {
      mkdirSync(dirname(item.destination), { recursive: true });
      const hadPrevious = existsSync(item.destination);
      if (hadPrevious) {
        mkdirSync(dirname(item.backup), { recursive: true });
        renameSync(item.destination, item.backup);
      }
      try {
        renameSync(item.source, item.destination);
        installed.push({ destination: item.destination, backup: item.backup, hadPrevious });
      } catch (error) {
        if (hadPrevious && existsSync(item.backup)) renameSync(item.backup, item.destination);
        throw error;
      }
    }
    const exact = await fullBuild(root, transaction, "exact", rom, true);
    verifyInstalledOwnership(plan, exact, root);
    const result = await fullBuild(root, transaction, "source-only", undefined, true);
    if (result.source_bytes !== exact.source_bytes) throw new Error("exact and source-only ownership totals differ");
    committed = true;
    return result;
  } catch (error) {
    restore(installed);
    throw error;
  } finally {
    if (committed) {
      for (const item of installed) if (item.hadPrevious) remove(item.backup);
    }
    remove(transaction);
  }
}

function pretty(value: unknown): string {
  return `${JSON.stringify(value, null, 2)}\n`;
}

async function selfTest(): Promise<void> {
  const temporary = mkdtempSync(join(tmpdir(), "alchemy-byte-closure-"));
  try {
    const tools = join(temporary, "tools");
    mkdirSync(tools, { recursive: true });
    const exporter = [
      "import { mkdirSync, writeFileSync } from 'fs';",
      "const args = Bun.argv.slice(2);",
      "const flag = args.findIndex((value) => value === '-o' || value === '--directory');",
      "const output = flag >= 0 ? args[flag + 1] : args[1];",
      "if (!output) process.exit(2);",
      "mkdirSync(output, { recursive: true });",
      "writeFileSync(`${output}/source.json`, JSON.stringify(args) + '\\n');",
    ].join("\n");
    for (const name of [
      "gba_header.ts", "relocated_arm_helpers.ts", "kind2_resource_series.ts", "veneer_island.ts", "early_runtime_data.ts",
      "late_runtime_data.ts", "executable_gap_sources.ts", "slot.ts",
    ]) {
      writeFileSync(join(tools, name), exporter);
    }
    writeFileSync(join(tools, "probe_audio_waves.ts"), [
      "import { mkdirSync, writeFileSync } from 'fs';",
      "const args = Bun.argv.slice(2), output = args[args.indexOf('-o') + 1];",
      "mkdirSync(output, { recursive: true });",
      "const wav = Buffer.alloc(48); wav.write('RIFF'); wav.writeUInt32LE(40,4); wav.write('WAVE',8); wav.write('fmt ',12);",
      "wav.writeUInt32LE(16,16); wav.writeUInt16LE(1,20); wav.writeUInt16LE(1,22); wav.writeUInt32LE(4096,24); wav.writeUInt32LE(4096,28); wav.writeUInt16LE(1,32); wav.writeUInt16LE(8,34); wav.write('data',36); wav.writeUInt32LE(4,40); wav.set([129,130,131,132],44);",
      "writeFileSync(`${output}/wave.wav`, wav);",
      "writeFileSync(`${output}/index.json`, JSON.stringify({format:1,engine:'smsh-pcm-wave-series',address:'0x08001000',end:'0x08001014',waves:[{name:'wave',address:'0x08001000',size:'0x14',frequency:4194304,loop_start:null,source:'wave.wav',header:{control:'0x00000000',frequency:4194304,loop_start:0,sample_count:4},padding:{size:0,fill:0}}]}, null, 2) + '\\n');",
    ].join("\n"));
    writeFileSync(join(tools, "build_full.ts"), [
      "import { existsSync, mkdirSync, writeFileSync } from 'fs';",
      "import { dirname } from 'path';",
      "const args = Bun.argv.slice(2);",
      "const output = args[args.indexOf('-o') + 1];",
      "if (existsSync('fail-coverage') && output.includes('/exact/')) process.exit(9);",
      "const plan = JSON.parse(await Bun.file('plan.json').text());",
      "const claims = plan.steps.flatMap((step) => step.ownership.map((claim) => ({...claim,destination:step.destination})));",
      "const installed = plan.steps.every((step) => existsSync(step.destination));",
      "const sourceBytes = 4096 - (installed ? 0 : claims.reduce((sum, claim) => sum + claim.size, 0));",
      "const sourceOnly = args.includes('--source-only');",
      "mkdirSync(dirname(output), { recursive: true });",
      "writeFileSync(output.replace(/\\.gba$/, '.json'), JSON.stringify({verification:sourceOnly?'source_only':'rom',rom_fallback_applicable:sourceOnly?false:true,byte_identical:!sourceOnly,source_bytes:sourceBytes,unowned_bytes:installed?0:4096-sourceBytes,unowned_regions:installed?0:claims.length}) + '\\n');",
      "writeFileSync(output.replace(/\\.gba$/, '.unowned.json'), JSON.stringify({regions:installed?[]:claims.map((claim)=>({address:Number(claim.address),size:claim.size}))}) + '\\n');",
      "const asmDir = args[args.indexOf('--asm-output') + 1], assetDir = args[args.indexOf('--asset-output') + 1]; mkdirSync(asmDir,{recursive:true}); mkdirSync(assetDir,{recursive:true});",
      "writeFileSync(`${asmDir}/manifest.json`, JSON.stringify({regions:claims.filter((claim)=>claim.component==='asm').map((claim)=>({address:Number(claim.address),size:claim.size,source:`${claim.destination}/source.json`}))}) + '\\n');",
      "writeFileSync(`${assetDir}/manifest.json`, JSON.stringify({regions:claims.filter((claim)=>claim.component==='asset').map((claim)=>({address:Number(claim.address),size:claim.size,sources:claim.destination.endsWith('waves')?[`${claim.destination}/index.json`,`${claim.destination}/wave.wav`]:[`${claim.destination}/source.json`]}))}) + '\\n');",
    ].join("\n"));
    const rom = join(temporary, "gs1-en.gba"), planFile = join(temporary, "plan.json");
    const romImage = Buffer.alloc(0x3000);
    romImage.writeUInt32LE(0, 0x1000); romImage.writeUInt32LE(4194304, 0x1004);
    romImage.writeUInt32LE(0, 0x1008); romImage.writeUInt32LE(3, 0x100c); romImage.set([1, 2, 3, 4], 0x1010);
    writeFileSync(rom, romImage);
    const claim = (index: number, component: "asm" | "asset" = "asset"): ClosureOwnership[] => [{
      address: `0x${(0x08002000 + index * 8).toString(16).padStart(8, "0")}`, size: 4, component,
    }];
    const command = (id: string, adapter: CommandStep["adapter"], index: number, component: "asm" | "asset" = "asset"): CommandStep => ({
      id, adapter: adapter as CommandStep["adapter"], destination: `assets/${id}`,
      ownership: claim(index, component),
      tool: "tools/slot.ts", arguments: ["{rom}", "{output}", id],
    });
    const plan: ClosurePlan = {
      format: 1,
      kind: "golden-sun-byte-closure-plan",
      canonical_rom: "gs1-en.gba",
      steps: [
        { id: "header", adapter: "gba-header-fields", destination: "assets/header", ownership: claim(0), template: "header.json" },
        { id: "waves", adapter: "audio-wave", destination: "assets/waves", ownership: [{ address: "0x08001000", size: 20, component: "asset" }], tones: "tones.json", start: "0x08001000", end: "0x08001014" },
        { id: "arm", adapter: "relocated-arm", destination: "assets/arm", ownership: claim(1), layout: "arm.json" },
        { id: "veneers", adapter: "linker-veneer", destination: "assets/veneers", ownership: claim(2, "asm"), layout: "veneer.json" },
        { id: "resources", adapter: "resource-series", destination: "assets/resources", ownership: claim(3), catalog: "resources.json" },
        { id: "early", adapter: "early-data", destination: "assets/early", ownership: claim(4) },
        { id: "late", adapter: "late-data", destination: "assets/late", ownership: claim(5), catalog: "late.json" },
        { id: "executable", adapter: "executable-gap", destination: "assets/executable", ownership: claim(6, "asm"), plan: "executable.json" },
        command("residual", "identified-residual", 7),
        command("alignment", "resource-alignment", 8),
        command("packed", "packed-runtime", 9),
        command("more-veneers", "additional-veneer-islands", 10, "asm"),
        command("runtime-residual", "runtime-support-residual", 11),
        command("asset-boundary", "asset-boundary", 12),
        command("odd-alignment", "nonstandard-alignment", 13, "asm"),
      ],
    };
    writeFileSync(planFile, pretty(plan));
    const result = await closeByteGaps(rom, planFile, temporary);
    if (result.unowned_bytes !== 0 || !plan.steps.every((step) => existsSync(join(temporary, step.destination)))) {
      throw new Error("closure transaction did not install every synthetic export");
    }
    const old = readFileSync(join(temporary, "assets/header/source.json"));
    for (const step of plan.steps) if (step.id !== "header") remove(join(temporary, step.destination));
    writeFileSync(join(temporary, "fail-coverage"), "1");
    let rejected = false;
    try { await closeByteGaps(rom, planFile, temporary); } catch { rejected = true; }
    if (!rejected || !readFileSync(join(temporary, "assets/header/source.json")).equals(old)) {
      throw new Error("failed source-only coverage did not roll back the transaction");
    }
  } finally {
    remove(temporary);
  }
  console.log("self-test=ok adapters=15 transaction=atomic gates=rom-exact,source-only synthetic-only=true");
}

function usage(): void {
  console.log("usage: close_byte_gaps.ts ROM --plan FILE | --self-test");
}

async function main(args: string[]): Promise<void> {
  if (args.length === 1 && args[0] === "--self-test") { await selfTest(); return; }
  if (args.includes("-h") || args.includes("--help")) { usage(); return; }
  if (args.length !== 3 || args[1] !== "--plan") throw new Error("ROM and --plan FILE are required");
  const result = await closeByteGaps(args[0], args[2]);
  console.log(`source_only=True source_bytes=${result.source_bytes} unowned_bytes=0 gaps=0`);
}

if (import.meta.main) await main(Bun.argv.slice(2));
