#!/usr/bin/env bun
// Tool role: library; imported by tools/build_assets.ts.
import { mkdirSync, readFileSync, writeFileSync } from "fs";
import { dirname, join } from "path";

const BASE = 0x08000000;
type Json = Record<string, unknown>;
export interface ByteValueRegion { address: number; data: Buffer; }

function pretty(value: unknown): string { return `${JSON.stringify(value, null, 2)}\n`; }
function hex(value: number): string { return `0x${value.toString(16).padStart(8, "0")}`; }
function fail(message: string): never { throw new Error(message); }
function object(value: unknown, name: string): Json {
  if (typeof value !== "object" || value === null || Array.isArray(value)) fail(`${name} differs`);
  return value as Json;
}
function number(value: unknown, name: string): number {
  if (!Number.isSafeInteger(value) || Number(value) < 0 || Number(value) > 0xffffffff) fail(`${name} differs`);
  return Number(value);
}
function parseAddress(value: unknown, name: string): number {
  if (typeof value !== "string" || !/^0x08[0-7][0-9a-f]{5}$/.test(value)) fail(`${name} differs`);
  return Number(value);
}

export function buildByteValueRegions(path: string): ByteValueRegion[] {
  const text = readFileSync(path, "utf8"), value = object(JSON.parse(text), "byte-value source");
  if (text !== pretty(value) || value.format !== 1 || value.kind !== "golden-sun-byte-value-regions" || !Array.isArray(value.regions)) {
    fail("byte-value source identity differs");
  }
  let previous = BASE - 1;
  return value.regions.map((raw, index) => {
    const item = object(raw, `region ${index}`);
    if (Object.keys(item).sort().join(",") !== "address,name,representation,values") fail(`region ${index} fields differ`);
    if (typeof item.name !== "string" || !/^[a-z0-9_]+$/.test(item.name) || item.representation !== "byte_values" || !Array.isArray(item.values)) fail(`region ${index} differs`);
    const address = parseAddress(item.address, `region ${index} address`);
    if (address <= previous || item.values.length === 0) fail(`region ${index} ordering differs`);
    const data = Buffer.from(item.values.map((entry, offset) => number(entry, `region ${index} byte ${offset}`)));
    previous = address + data.length - 1;
    return { address, data };
  });
}

export function exportByteValueRegions(romPath: string, directory: string, regions: Array<{ name: string; address: number; size: number }>): void {
  const rom = readFileSync(romPath);
  const source = { format: 1, kind: "golden-sun-byte-value-regions", regions: regions.map((region) => {
    const data = rom.subarray(region.address - BASE, region.address - BASE + region.size);
    if (data.length !== region.size) fail(`${region.name} lies outside ROM`);
    return { name: region.name, address: hex(region.address), representation: "byte_values", values: [...data] };
  }) };
  mkdirSync(directory, { recursive: true });
  const output = join(directory, "index.json"); writeFileSync(output, pretty(source));
  const built = buildByteValueRegions(output);
  for (const [index, region] of built.entries()) if (!region.data.equals(rom.subarray(region.address - BASE, region.address - BASE + region.data.length))) fail(`region ${index} differs`);
}

if (import.meta.main) {
  const [command, rom, directory, ...specs] = Bun.argv.slice(2);
  if (command === "export-unowned" && rom && directory && specs.length === 1) {
    const audit = object(JSON.parse(readFileSync(specs[0], "utf8")), "unowned manifest");
    if (!Array.isArray(audit.regions)) fail("unowned manifest regions differ");
    exportByteValueRegions(rom, directory, audit.regions.map((raw, index) => {
      const item = object(raw, `unowned region ${index}`);
      return { name: `residual_${index.toString().padStart(3, "0")}`, address: number(item.address, `unowned region ${index} address`), size: number(item.size, `unowned region ${index} size`) };
    }));
    process.exit(0);
  }
  if (command !== "export" || !rom || !directory || specs.length === 0) fail("usage: byte_value_regions.ts export ROM DIRECTORY name:address:size [...]");
  exportByteValueRegions(rom, directory, specs.map((spec) => {
    const [name, address, size] = spec.split(":");
    if (!name || !address || !size) fail("region spec differs");
    return { name, address: Number(address), size: Number(size) };
  }));
}
