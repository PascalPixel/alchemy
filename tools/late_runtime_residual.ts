#!/usr/bin/env bun
import { existsSync, mkdirSync, readFileSync, rmSync, writeFileSync } from "fs";
import { dirname, join, resolve } from "path";
import { readLateRuntimeCatalog, ROM_BASE, type LateRuntimeComponent } from "./late_runtime_data.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const DEFAULT_CATALOG = join(ROOT, "assets/data/late_runtime/catalog.json");

type Json = Record<string, unknown>;
type Values = { representation: "uniform_fill"; value: 0 | 255 } | { representation: "byte_values"; values: number[] };

interface ComponentSource extends Values {
  name: string;
  address: string;
  end: string;
  role: string;
  type: "u8" | "s16" | "u16";
}

interface RegionSource {
  name: string;
  address: string;
  end: string;
  components: ComponentSource[];
}

interface Source {
  format: 1;
  kind: "golden-sun-late-runtime-residual";
  source_bytes: number;
  regions: RegionSource[];
}

export interface LateRuntimeResidualBuild {
  regions: Map<number, Buffer>;
  sourceBytes: number;
}

function pretty(value: unknown): string { return `${JSON.stringify(value, null, 2)}\n`; }
function hex(value: number): string { return `0x${value.toString(16).padStart(8, "0")}`; }

function object(value: unknown, label: string): Json {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${label} must be an object`);
  return value as Json;
}

function exactKeys(value: Json, keys: string[], label: string): void {
  const actual = Object.keys(value).sort(), expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((item, index) => item !== expected[index])) {
    throw new Error(`${label} fields differ`);
  }
}

function integer(value: unknown, minimum: number, maximum: number, label: string): number {
  if (!Number.isSafeInteger(value) || Number(value) < minimum || Number(value) > maximum) throw new Error(`${label} differs`);
  return Number(value);
}

function typedKind(component: LateRuntimeComponent): ComponentSource["type"] {
  return component.kind === "s16_table" ? "s16" : component.kind === "u16_table" ? "u16" : "u8";
}

function representation(data: Uint8Array): Values {
  if (data.length === 0) throw new Error("late residual component is empty");
  if ((data[0] === 0 || data[0] === 255) && data.every((value) => value === data[0])) {
    return { representation: "uniform_fill", value: data[0] as 0 | 255 };
  }
  return { representation: "byte_values", values: [...data] };
}

function componentSource(component: LateRuntimeComponent, image: Uint8Array): ComponentSource {
  const data = image.subarray(component.address - ROM_BASE, component.end - ROM_BASE);
  if (data.length !== component.end - component.address) throw new Error(`${component.name} lies outside the ROM`);
  return {
    name: component.name, address: hex(component.address), end: hex(component.end), role: component.role,
    type: typedKind(component), ...representation(data),
  };
}

function parseSource(indexPath: string): Source {
  const text = readFileSync(indexPath, "utf8"), value = object(JSON.parse(text), "late residual source");
  if (text !== pretty(value)) throw new Error("late residual source is not canonical JSON");
  exactKeys(value, ["format", "kind", "source_bytes", "regions"], "late residual source");
  if (value.format !== 1 || value.kind !== "golden-sun-late-runtime-residual" || !Array.isArray(value.regions)) {
    throw new Error("late residual source identity differs");
  }
  return value as unknown as Source;
}

function componentData(source: ComponentSource, component: LateRuntimeComponent): Buffer {
  const size = component.end - component.address;
  if (source.name !== component.name || source.address !== hex(component.address) || source.end !== hex(component.end) ||
      source.role !== component.role || source.type !== typedKind(component)) throw new Error(`${component.name} metadata differs`);
  const raw = object(source, component.name);
  if (source.representation === "uniform_fill") {
    exactKeys(raw, ["name", "address", "end", "role", "type", "representation", "value"], component.name);
    if (source.value !== 0 && source.value !== 255) throw new Error(`${component.name} fill differs`);
    return Buffer.alloc(size, source.value);
  }
  exactKeys(raw, ["name", "address", "end", "role", "type", "representation", "values"], component.name);
  if (source.representation !== "byte_values" || !Array.isArray(source.values) || source.values.length !== size) {
    throw new Error(`${component.name} values differ`);
  }
  return Buffer.from(source.values.map((value, index) => integer(value, 0, 255, `${component.name} value ${index}`)));
}

export function buildLateRuntimeResidual(indexPath: string, catalogPath = DEFAULT_CATALOG): LateRuntimeResidualBuild {
  const catalog = readLateRuntimeCatalog(catalogPath), source = parseSource(indexPath);
  if (source.regions.length !== catalog.ranges.length) throw new Error("late residual region count differs");
  const regions = new Map<number, Buffer>();
  for (const [index, range] of catalog.ranges.entries()) {
    const item = source.regions[index];
    if (item.name !== range.name || item.address !== hex(range.address) || item.end !== hex(range.end) ||
        !Array.isArray(item.components) || item.components.length !== range.components.length) {
      throw new Error(`${range.name} layout differs`);
    }
    const data = Buffer.concat(range.components.map((component, componentIndex) =>
      componentData(item.components[componentIndex], component)));
    if (data.length !== range.end - range.address) throw new Error(`${range.name} length differs`);
    regions.set(range.address, data);
  }
  const sourceBytes = [...regions.values()].reduce((total, data) => total + data.length, 0);
  if (source.source_bytes !== sourceBytes || sourceBytes !== 7294) throw new Error("late residual source-byte total differs");
  return { regions, sourceBytes };
}

export function exportLateRuntimeResidual(romPath: string, directory: string, catalogPath = DEFAULT_CATALOG): void {
  const image = readFileSync(romPath), catalog = readLateRuntimeCatalog(catalogPath);
  const source: Source = {
    format: 1,
    kind: "golden-sun-late-runtime-residual",
    source_bytes: 7294,
    regions: catalog.ranges.map((range) => ({
      name: range.name, address: hex(range.address), end: hex(range.end),
      components: range.components.map((component) => componentSource(component, image)),
    })),
  };
  mkdirSync(directory, { recursive: true });
  const index = join(directory, "index.json");
  writeFileSync(index, pretty(source));
  const built = buildLateRuntimeResidual(index, catalogPath);
  for (const [address, data] of built.regions) {
    const expected = image.subarray(address - ROM_BASE, address - ROM_BASE + data.length);
    if (!data.equals(expected)) throw new Error(`${hex(address)} differs from the ROM`);
  }
}

function usage(): never {
  console.log("usage: late_runtime_residual.ts export ROM --directory DIR | verify ROM INDEX | --self-test");
  process.exit(0);
}

function selfTest(): void {
  const temporary = resolve(Bun.env.TMPDIR ?? "/private/tmp", `alchemy-late-residual-${process.pid}`);
  if (existsSync(temporary)) throw new Error("late residual self-test directory exists");
  try {
    const catalog = readLateRuntimeCatalog(DEFAULT_CATALOG);
    const image = Buffer.alloc(0x100000);
    for (const range of catalog.ranges) for (const component of range.components) {
      const data = image.subarray(component.address - ROM_BASE, component.end - ROM_BASE);
      data.fill(component.name.startsWith("aki_") ? 255 : 0);
    }
    const rom = join(temporary, "gs1-en.gba"), output = join(temporary, "source");
    mkdirSync(temporary, { recursive: true }); writeFileSync(rom, image);
    exportLateRuntimeResidual(rom, output);
    if (buildLateRuntimeResidual(join(output, "index.json")).sourceBytes !== 7294) throw new Error("late residual self-test total differs");
    const index = join(output, "index.json"), source = JSON.parse(readFileSync(index, "utf8"));
    source.regions[0].components[0].values = [];
    writeFileSync(index, pretty(source));
    let rejected = false; try { buildLateRuntimeResidual(index); } catch { rejected = true; }
    if (!rejected) throw new Error("late residual malformed values were accepted");
  } finally {
    if (existsSync(temporary)) rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok source_bytes=7294");
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { selfTest(); return; }
  if (args.length === 4 && args[0] === "export" && args[2] === "--directory") { exportLateRuntimeResidual(args[1], args[3]); return; }
  if (args.length === 3 && args[0] === "verify") {
    const image = readFileSync(args[1]), built = buildLateRuntimeResidual(args[2]);
    for (const [address, data] of built.regions) if (!data.equals(image.subarray(address - ROM_BASE, address - ROM_BASE + data.length))) {
      throw new Error(`${hex(address)} differs from the ROM`);
    }
    console.log("identical=true source_bytes=7294"); return;
  }
  usage();
}

if (import.meta.main) main(Bun.argv.slice(2));
