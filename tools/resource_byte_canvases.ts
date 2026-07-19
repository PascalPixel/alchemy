#!/usr/bin/env bun
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "fs";
import { dirname, join } from "path";
import { byte_png } from "./export_asset.ts";
import { indexed_png } from "./import_asset.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;

interface Spec { id: string; address: number; size: number; source: string; }
interface Index { format: 1; kind: "golden-sun-resource-byte-canvas-series"; resources: Spec[]; }
export interface BuiltResource { id: string; address: number; data: Buffer; source: string; }

const RESOURCES: Spec[] = [
  ["049", 0x083cd090, 0x488], ["04a", 0x083cd518, 0x5ec], ["061", 0x083d85e0, 0x444], ["064", 0x083dc1f8, 0x220],
  ["066", 0x083dc854, 0x114], ["073", 0x083e2ae4, 0x29c], ["0a5", 0x083f5f4c, 0x7b0], ["0a6", 0x083f66fc, 0x518],
].map(([id, address, size]) => ({ id: String(id), address: Number(address), size: Number(size), source: `resource_${id}.png` }));

function pretty(value: unknown): string { return `${JSON.stringify(value, null, 2)}\n`; }
function hex(value: number): string { return `0x${value.toString(16).padStart(8, "0")}`; }
function width(_: number): number { return 32; }
function height(size: number): number { return Math.ceil(Math.ceil(size / width(size)) / 8) * 8; }

function parse(indexPath: string): Index {
  const text = readFileSync(indexPath, "utf8"), value = JSON.parse(text);
  if (text !== pretty(value) || typeof value !== "object" || value === null || Array.isArray(value)) throw new Error("resource canvas index differs");
  const source = value as Index;
  if (source.format !== 1 || source.kind !== "golden-sun-resource-byte-canvas-series" || !Array.isArray(source.resources) ||
      source.resources.length !== RESOURCES.length) throw new Error("resource canvas index identity differs");
  source.resources.forEach((item, index) => {
    const expected = RESOURCES[index];
    if (item.id !== expected.id || item.address !== expected.address || item.size !== expected.size || item.source !== expected.source) {
      throw new Error(`resource canvas ${index} layout differs`);
    }
  });
  return source;
}

export function buildResourceByteCanvases(indexPath: string): BuiltResource[] {
  const index = parse(indexPath), directory = dirname(indexPath);
  return index.resources.map((resource) => {
    const path = join(directory, resource.source), [imageWidth, imageHeight, pixels] = indexed_png(readFileSync(path));
    if (imageWidth !== width(resource.size) || imageHeight !== height(resource.size) || pixels.length !== imageWidth * imageHeight ||
        pixels.slice(resource.size).some(Boolean)) throw new Error(`resource ${resource.id} canvas differs`);
    const canvas = Buffer.from(pixels);
    if (!byte_png(canvas, imageWidth)[0].equals(readFileSync(path))) throw new Error(`resource ${resource.id} canvas is not canonical`);
    return { id: resource.id, address: resource.address, data: canvas.subarray(0, resource.size), source: resource.source };
  });
}

export function exportResourceByteCanvases(romPath: string, directory: string): void {
  const rom = readFileSync(romPath); mkdirSync(directory, { recursive: true });
  const index: Index = { format: 1, kind: "golden-sun-resource-byte-canvas-series", resources: RESOURCES };
  writeFileSync(join(directory, "index.json"), pretty(index));
  for (const resource of RESOURCES) {
    const data = rom.subarray(resource.address - ROM_BASE, resource.address - ROM_BASE + resource.size);
    if (data.length !== resource.size) throw new Error(`resource ${resource.id} lies outside the ROM`);
    const canvas = Buffer.alloc(width(resource.size) * height(resource.size)); data.copy(canvas);
    writeFileSync(join(directory, resource.source), byte_png(canvas, width(resource.size))[0]);
  }
  for (const resource of buildResourceByteCanvases(join(directory, "index.json"))) {
    const expected = rom.subarray(resource.address - ROM_BASE, resource.address - ROM_BASE + resource.data.length);
    if (!resource.data.equals(expected)) throw new Error(`resource ${resource.id} does not round-trip`);
  }
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") {
    if (RESOURCES.reduce((sum, resource) => sum + resource.size, 0) !== 8528 || RESOURCES.some((resource) => height(resource.size) % 8 !== 0)) throw new Error("resource canvas layout differs");
    console.log("self-test=ok resources=8 source_bytes=8528"); return;
  }
  if (args.length === 4 && args[0] === "export" && args[2] === "--directory") { exportResourceByteCanvases(args[1], args[3]); return; }
  console.log("usage: resource_byte_canvases.ts export ROM --directory DIR | --self-test");
}
if (import.meta.main) main(Bun.argv.slice(2));
