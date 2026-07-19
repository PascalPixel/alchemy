#!/usr/bin/env bun
import {
  existsSync,
  mkdirSync,
  mkdtempSync,
  readFileSync,
  renameSync,
  rmSync,
  writeFileSync,
} from "fs";
import { dirname, join, resolve } from "path";

export const ROM_BASE = 0x08000000;
const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const TMPDIR = Bun.env.TMPDIR ?? Bun.env.TMP ?? Bun.env.TEMP ?? "/tmp";

type JsonObject = Record<string, unknown>;
export type SlotClassification = "existing_veneer" | "live_missing" | "unknown";

export interface VeneerIslandLayout {
  address: number;
  end: number;
  slotSize: 8;
  counts: Record<SlotClassification, number>;
  slots: Map<number, SlotClassification>;
  source: JsonObject;
}

export interface VeneerSlot {
  address: number;
  classification: SlotClassification;
  kind: "veneer";
  target: number;
  ultimateTarget: number;
  source: string;
}

export interface FillSlot {
  address: number;
  classification: "unknown";
  kind: "fill";
  value: 0 | 0xff;
  size: 8;
  source: string;
}

export type IslandSlot = VeneerSlot | FillSlot;

export interface VeneerIslandPackage {
  address: number;
  end: number;
  slotSize: 8;
  counts: { live_missing: number; unknown: number };
  slots: IslandSlot[];
  source: JsonObject;
}

function object(value: unknown, name: string): JsonObject {
  if (typeof value !== "object" || value === null || Array.isArray(value)) {
    throw new Error(`${name} must be an object`);
  }
  return value as JsonObject;
}

function exactKeys(value: JsonObject, keys: string[], name: string): void {
  const actual = Object.keys(value).sort(), expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${name} has unknown or missing fields`);
  }
}

function pretty(value: unknown): string {
  return `${JSON.stringify(value, null, 2)}\n`;
}

function canonicalDocument(path: string): JsonObject {
  const text = readFileSync(path, "utf8"), value = object(JSON.parse(text), path);
  if (text !== pretty(value)) throw new Error(`${path}: source is not canonical JSON`);
  return value;
}

function addressText(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function address(value: unknown, name: string): number {
  if (typeof value !== "string" || !/^0x[0-9a-f]{8}$/.test(value)) throw new Error(`invalid ${name}`);
  return Number.parseInt(value.slice(2), 16);
}

function count(value: unknown, name: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < 0) throw new Error(`invalid ${name}`);
  return value;
}

function stringArray(value: unknown, name: string): string[] {
  if (!Array.isArray(value) || value.some((item) => typeof item !== "string")) {
    throw new Error(`${name} must be a string array`);
  }
  return value as string[];
}

function parseSlotList(
  value: unknown,
  name: string,
  classification: SlotClassification,
  start: number,
  end: number,
  slotSize: number,
  slots: Map<number, SlotClassification>,
): number[] {
  const result = stringArray(value, name).map((item, index) => address(item, `${name} ${index}`));
  for (let index = 0; index < result.length; index++) {
    const slot = result[index];
    if (slot < start || slot >= end || (slot - start) % slotSize !== 0) throw new Error(`${name} has an invalid slot`);
    if (index !== 0 && result[index - 1] >= slot) throw new Error(`${name} is not strictly ordered`);
    if (slots.has(slot)) throw new Error(`${name} overlaps another classification`);
    slots.set(slot, classification);
  }
  return result;
}

export function parseVeneerIslandLayout(path: string): VeneerIslandLayout {
  const source = canonicalDocument(path);
  exactKeys(source, [
    "format", "kind", "instruction_set", "address", "end", "slot_size", "counts",
    "existing_veneer_slots", "live_missing_slots", "unknown_slots",
  ], "veneer island layout");
  if (source.format !== 1 || source.kind !== "golden-sun-thumb-long-call-veneer-island" ||
      source.instruction_set !== "thumb" || source.slot_size !== 8) {
    throw new Error("veneer island layout identity differs");
  }
  const start = address(source.address, "veneer island address"), end = address(source.end, "veneer island end");
  if (start < ROM_BASE || end <= start || (start & 7) !== 0 || (end - start) % 8 !== 0) {
    throw new Error("veneer island extent differs");
  }
  const countsSource = object(source.counts, "veneer island counts");
  exactKeys(countsSource, ["existing_veneer", "live_missing", "unknown"], "veneer island counts");
  const counts: Record<SlotClassification, number> = {
    existing_veneer: count(countsSource.existing_veneer, "existing veneer count"),
    live_missing: count(countsSource.live_missing, "live missing count"),
    unknown: count(countsSource.unknown, "unknown count"),
  };
  const slots = new Map<number, SlotClassification>();
  const existing = parseSlotList(source.existing_veneer_slots, "existing veneer slots", "existing_veneer", start, end, 8, slots);
  const live = parseSlotList(source.live_missing_slots, "live missing slots", "live_missing", start, end, 8, slots);
  const unknown = parseSlotList(source.unknown_slots, "unknown slots", "unknown", start, end, 8, slots);
  if (existing.length !== counts.existing_veneer || live.length !== counts.live_missing || unknown.length !== counts.unknown) {
    throw new Error("veneer island classification counts differ");
  }
  if (slots.size !== (end - start) / 8) throw new Error("veneer island classification does not cover every slot");
  for (let slot = start; slot < end; slot += 8) {
    if (!slots.has(slot)) throw new Error(`veneer island has an unclassified slot at ${addressText(slot)}`);
  }
  return { address: start, end, slotSize: 8, counts, slots, source };
}

export function encodeVeneer(target: number): Buffer {
  if (!Number.isSafeInteger(target) || target < 1 || target > 0xffffffff || (target & 1) === 0) {
    throw new Error("long-call veneer target is not an odd u32 address");
  }
  const output = Buffer.alloc(8);
  output.writeUInt16LE(0x4c00, 0);
  output.writeUInt16LE(0x4720, 2);
  output.writeUInt32LE(target >>> 0, 4);
  return output;
}

export function decodeVeneer(data: Uint8Array): number | null {
  const source = Buffer.from(data);
  if (source.length !== 8 || source.readUInt16LE(0) !== 0x4c00 || source.readUInt16LE(2) !== 0x4720) return null;
  const target = source.readUInt32LE(4);
  return (target & 1) === 0 ? null : target;
}

function romSlice(rom: Buffer, start: number, end: number): Buffer {
  const first = start - ROM_BASE, last = end - ROM_BASE;
  if (first < 0 || last > rom.length || first >= last) throw new Error(`ROM does not contain ${addressText(start)}..${addressText(end)}`);
  return rom.subarray(first, last);
}

export function resolveUltimateTarget(rom: Buffer, initialTarget: number): number {
  let target = initialTarget;
  const seen = new Set<number>();
  for (;;) {
    if ((target & 1) === 0) return target;
    const entry = target & ~1;
    const offset = entry - ROM_BASE;
    if (offset < 0 || offset + 8 > rom.length) return target;
    if (seen.has(entry)) throw new Error(`long-call veneer cycle at ${addressText(entry)}`);
    seen.add(entry);
    const next = decodeVeneer(rom.subarray(offset, offset + 8));
    if (next === null) return target;
    target = next;
  }
}

function semanticFill(data: Uint8Array): 0 | 0xff | null {
  if (data.length !== 8 || (data[0] !== 0 && data[0] !== 0xff)) return null;
  return data.every((value) => value === data[0]) ? data[0] as 0 | 0xff : null;
}

function sourceName(slot: number): string {
  return `${addressText(slot).slice(2)}.s`;
}

export function veneerSource(slot: number, target: number): string {
  const stem = addressText(slot).slice(2);
  return "@ 遠距離呼出し用モード間ベニア。固定のldr r4,[pc]; bx r4は\n" +
    "@ リンカが遠距離処理へ到達するために生成する。Cでは表現不能。\n" +
    ".syntax unified\n\t.thumb\n" +
    `\t.global Func_${stem}\n\t.thumb_func\nFunc_${stem}:\n` +
    "\tldr\tr4, [pc, #0]\n\tbx\tr4\n" +
    `\t.4byte ${addressText(target)}\n`;
}

export function fillSource(slot: number, value: 0 | 0xff): string {
  const stem = addressText(slot).slice(2);
  return "@ 未使用の遠距離呼出し枠。リンカ領域の消去値を明示する。\n" +
    ".syntax unified\n\t.thumb\n" +
    `\t.global Data_${stem}\nData_${stem}:\n` +
    `\t.fill 8, 1, 0x${value.toString(16).padStart(2, "0")}\n`;
}

function sourceForSlot(slot: IslandSlot): string {
  return slot.kind === "veneer" ? veneerSource(slot.address, slot.target) : fillSource(slot.address, slot.value);
}

export function inspectVeneerIsland(rom: Buffer, layout: VeneerIslandLayout): IslandSlot[] {
  romSlice(rom, layout.address, layout.end);
  const result: IslandSlot[] = [];
  const missing = [...layout.slots].filter(([, classification]) => classification !== "existing_veneer")
    .sort(([left], [right]) => left - right);
  for (const [slot, classification] of missing) {
    const data = romSlice(rom, slot, slot + 8);
    const target = decodeVeneer(data);
    if (target !== null) {
      result.push({
        address: slot,
        classification,
        kind: "veneer",
        target,
        ultimateTarget: resolveUltimateTarget(rom, target),
        source: sourceName(slot),
      });
      continue;
    }
    if (classification !== "unknown") {
      throw new Error(`${addressText(slot)}: live slot is not a long-call veneer`);
    }
    const value = semanticFill(data);
    if (value === null) throw new Error(`${addressText(slot)}: unresolved slot is neither a veneer nor semantic fill`);
    result.push({ address: slot, classification, kind: "fill", value, size: 8, source: sourceName(slot) });
  }
  return result;
}

function packageDocument(addressValue: number, end: number, slots: IslandSlot[]): JsonObject {
  const counts = { live_missing: 0, unknown: 0 };
  for (const slot of slots) {
    if (slot.classification === "existing_veneer") throw new Error("existing veneer entered the missing-slot package");
    counts[slot.classification]++;
  }
  return {
    format: 1,
    kind: "golden-sun-thumb-long-call-veneer-island-source",
    address: addressText(addressValue),
    end: addressText(end),
    slot_size: 8,
    counts,
    slots: slots.map((slot) => slot.kind === "veneer" ? {
      address: addressText(slot.address),
      classification: slot.classification,
      kind: slot.kind,
      target: addressText(slot.target),
      ultimate_target: addressText(slot.ultimateTarget),
      source: slot.source,
    } : {
      address: addressText(slot.address),
      classification: slot.classification,
      kind: slot.kind,
      value: `0x${slot.value.toString(16).padStart(2, "0")}`,
      size: slot.size,
      source: slot.source,
    }),
  };
}

export function parseVeneerIslandPackage(path: string): VeneerIslandPackage {
  const source = canonicalDocument(path);
  exactKeys(source, ["format", "kind", "address", "end", "slot_size", "counts", "slots"], "veneer island package");
  if (source.format !== 1 || source.kind !== "golden-sun-thumb-long-call-veneer-island-source" || source.slot_size !== 8 ||
      !Array.isArray(source.slots)) throw new Error("veneer island package identity differs");
  const start = address(source.address, "package address"), end = address(source.end, "package end");
  if (start < ROM_BASE || end <= start || (end - start) % 8 !== 0) {
    throw new Error("veneer island package extent differs");
  }
  const countsSource = object(source.counts, "veneer island package counts");
  exactKeys(countsSource, ["live_missing", "unknown"], "veneer island package counts");
  const counts = {
    live_missing: count(countsSource.live_missing, "package live missing count"),
    unknown: count(countsSource.unknown, "package unknown count"),
  };
  if (source.slots.length !== counts.live_missing + counts.unknown) throw new Error("veneer island package counts differ");
  let previousAddress = 0;
  const actualCounts = { live_missing: 0, unknown: 0 };
  const slots = source.slots.map((raw, index): IslandSlot => {
    const item = object(raw, `veneer island package slot ${index}`);
    const kind = item.kind;
    exactKeys(item, kind === "veneer" ?
      ["address", "classification", "kind", "target", "ultimate_target", "source"] :
      ["address", "classification", "kind", "value", "size", "source"], `veneer island package slot ${index}`);
    const slotAddress = address(item.address, `package slot ${index} address`);
    if (slotAddress < start || slotAddress >= end || (slotAddress - start) % 8 !== 0 ||
        (index !== 0 && slotAddress <= previousAddress) || item.source !== sourceName(slotAddress)) {
      throw new Error(`veneer island package slot ${index} boundary differs`);
    }
    previousAddress = slotAddress;
    if (item.classification !== "live_missing" && item.classification !== "unknown") {
      throw new Error(`veneer island package slot ${index} classification differs`);
    }
    actualCounts[item.classification]++;
    if (kind === "veneer") {
      const target = address(item.target, `package slot ${index} target`);
      const ultimateTarget = address(item.ultimate_target, `package slot ${index} ultimate target`);
      encodeVeneer(target);
      if ((ultimateTarget & 1) === 0) throw new Error(`package slot ${index} ultimate target is not Thumb`);
      return {
        address: slotAddress,
        classification: item.classification,
        kind,
        target,
        ultimateTarget,
        source: item.source as string,
      };
    }
    if (kind !== "fill" || item.classification !== "unknown" || item.size !== 8 ||
        (item.value !== "0x00" && item.value !== "0xff")) {
      throw new Error(`veneer island package slot ${index} fill differs`);
    }
    return {
      address: slotAddress,
      classification: "unknown",
      kind,
      value: item.value === "0x00" ? 0 : 0xff,
      size: 8,
      source: item.source as string,
    };
  });
  if (actualCounts.live_missing !== counts.live_missing || actualCounts.unknown !== counts.unknown) {
    throw new Error("veneer island package classifications differ from their counts");
  }
  return { address: start, end, slotSize: 8, counts, slots, source };
}

function packageSource(indexPath: string, name: string): string {
  const directory = resolve(dirname(indexPath)), path = resolve(directory, name);
  if (dirname(path) !== directory) throw new Error("veneer island source escaped its package");
  return path;
}

export function buildVeneerIsland(indexPath: string): Buffer {
  const source = parseVeneerIslandPackage(indexPath);
  const output: Buffer[] = [];
  for (const slot of source.slots) {
    const path = packageSource(indexPath, slot.source);
    if (readFileSync(path, "utf8") !== sourceForSlot(slot)) throw new Error(`${slot.source}: source is not canonical`);
    output.push(slot.kind === "veneer" ? encodeVeneer(slot.target) : Buffer.alloc(8, slot.value));
  }
  const data = Buffer.concat(output);
  if (data.length !== source.slots.length * 8) throw new Error("veneer island sparse build size differs");
  return data;
}

function verifySparseBytes(rom: Buffer, source: VeneerIslandPackage, built: Buffer, name: string): void {
  for (const [index, slot] of source.slots.entries()) {
    const actual = built.subarray(index * 8, index * 8 + 8), expected = romSlice(rom, slot.address, slot.address + 8);
    if (!actual.equals(expected)) throw new Error(`${name} differs at ${addressText(slot.address)}`);
  }
}

export function validateExistingVeneers(
  rom: Buffer,
  layout: VeneerIslandLayout,
  directory = resolve(ROOT, "asm"),
): void {
  for (const [slot, classification] of layout.slots) {
    if (classification !== "existing_veneer") continue;
    const target = trackedVeneerTarget(resolve(directory, sourceName(slot)), slot);
    if (!encodeVeneer(target).equals(romSlice(rom, slot, slot + 8))) {
      throw new Error(`${addressText(slot)}: tracked veneer differs from ROM`);
    }
    resolveUltimateTarget(rom, target);
  }
}

function writePackage(rom: Buffer, layout: VeneerIslandLayout, directory: string): void {
  const slots = inspectVeneerIsland(rom, layout);
  mkdirSync(directory, { recursive: true });
  writeFileSync(join(directory, "index.json"), pretty(packageDocument(layout.address, layout.end, slots)));
  for (const slot of slots) writeFileSync(join(directory, slot.source), sourceForSlot(slot));
  const indexPath = join(directory, "index.json"), source = parseVeneerIslandPackage(indexPath);
  verifySparseBytes(rom, source, buildVeneerIsland(indexPath), "exported veneer island");
}

export function exportVeneerIsland(
  rom: Buffer,
  layoutPath: string,
  directory: string,
  existingDirectory = resolve(ROOT, "asm"),
): void {
  const layout = parseVeneerIslandLayout(layoutPath);
  validateExistingVeneers(rom, layout, existingDirectory);
  const destination = resolve(directory), parent = dirname(destination);
  if (destination === parent) throw new Error("veneer island export requires a dedicated directory");
  if (existsSync(destination)) buildVeneerIsland(join(destination, "index.json"));
  mkdirSync(parent, { recursive: true });
  const transaction = mkdtempSync(join(parent, ".veneer-island-export-"));
  const staged = join(transaction, "new"), previous = join(transaction, "previous");
  let installed = false;
  try {
    writePackage(rom, layout, staged);
    if (existsSync(destination)) renameSync(destination, previous);
    try {
      renameSync(staged, destination);
      installed = true;
    } catch (error) {
      if (existsSync(previous)) renameSync(previous, destination);
      throw error;
    }
  } finally {
    if (!installed && existsSync(previous) && !existsSync(destination)) renameSync(previous, destination);
    rmSync(transaction, { recursive: true, force: true });
  }
}

function layoutFromPackage(source: VeneerIslandPackage): VeneerIslandLayout {
  const counts: Record<SlotClassification, number> = { existing_veneer: 0, live_missing: 0, unknown: 0 };
  const slots = new Map<number, SlotClassification>();
  for (const slot of source.slots) {
    slots.set(slot.address, slot.classification);
    counts[slot.classification]++;
  }
  return { address: source.address, end: source.end, slotSize: 8, counts, slots, source: {} };
}

export function verifyVeneerIsland(rom: Buffer, indexPath: string): Buffer {
  const source = parseVeneerIslandPackage(indexPath);
  const observed = inspectVeneerIsland(rom, layoutFromPackage(source));
  if (pretty(packageDocument(source.address, source.end, observed)) !== pretty(source.source)) {
    throw new Error("veneer island targets or classifications differ");
  }
  const built = buildVeneerIsland(indexPath);
  verifySparseBytes(rom, source, built, "veneer island");
  return built;
}

function reject(action: () => unknown): boolean {
  try { action(); } catch { return true; }
  return false;
}

function syntheticLayout(): VeneerIslandLayout {
  return {
    address: 0x08000020,
    end: 0x08000038,
    slotSize: 8,
    counts: { existing_veneer: 1, live_missing: 1, unknown: 1 },
    slots: new Map([
      [0x08000020, "existing_veneer"],
      [0x08000028, "live_missing"],
      [0x08000030, "unknown"],
    ]),
    source: {},
  };
}

function writeSyntheticLayout(path: string, layout: VeneerIslandLayout): void {
  const groups = (classification: SlotClassification): string[] => [...layout.slots]
    .filter(([, value]) => value === classification).map(([slot]) => addressText(slot));
  writeFileSync(path, pretty({
    format: 1,
    kind: "golden-sun-thumb-long-call-veneer-island",
    instruction_set: "thumb",
    address: addressText(layout.address),
    end: addressText(layout.end),
    slot_size: 8,
    counts: layout.counts,
    existing_veneer_slots: groups("existing_veneer"),
    live_missing_slots: groups("live_missing"),
    unknown_slots: groups("unknown"),
  }));
}

function trackedVeneerTarget(path: string, slot: number): number {
  const text = readFileSync(path, "utf8");
  const match = text.match(/\t\.4byte (0x[0-9a-f]{8})\n$/);
  if (match === null) throw new Error(`${path}: veneer target is not canonical`);
  const target = address(match[1], `${path} target`);
  if (text !== veneerSource(slot, target)) throw new Error(`${path}: veneer source is not canonical`);
  encodeVeneer(target);
  return target;
}

export function selfTest(layoutPath = resolve(ROOT, "assets/data/veneer_island.json")): void {
  const canonical = parseVeneerIslandLayout(layoutPath);
  if (canonical.address !== 0x08015000 || canonical.end !== 0x08015430 || canonical.slots.size !== 134 ||
      canonical.counts.existing_veneer !== 78 || canonical.counts.live_missing !== 21 || canonical.counts.unknown !== 35) {
    throw new Error("canonical veneer island classification differs");
  }
  for (const [slot, classification] of canonical.slots) {
    if (classification === "existing_veneer") trackedVeneerTarget(resolve(ROOT, "asm", sourceName(slot)), slot);
  }
  const temporary = mkdtempSync(join(TMPDIR, "veneer-island-self-test-"));
  try {
    const layout = syntheticLayout(), layoutFile = join(temporary, "layout.json"), output = join(temporary, "package");
    const existingDirectory = join(temporary, "asm");
    mkdirSync(existingDirectory);
    writeFileSync(join(existingDirectory, sourceName(0x08000020)), veneerSource(0x08000020, 0x08000061));
    writeSyntheticLayout(layoutFile, layout);
    const rom = Buffer.alloc(0x100);
    encodeVeneer(0x08000061).copy(rom, 0x20);
    encodeVeneer(0x08000081).copy(rom, 0x28);
    encodeVeneer(0x08000081).copy(rom, 0x60);
    exportVeneerIsland(rom, layoutFile, output, existingDirectory);
    const source = parseVeneerIslandPackage(join(output, "index.json"));
    if (source.slots.length !== 2 || source.counts.live_missing !== 1 || source.counts.unknown !== 1 ||
        source.slots[0].address !== 0x08000028 || source.slots[0].kind !== "veneer" ||
        source.slots[0].ultimateTarget !== 0x08000081 || source.slots[1].address !== 0x08000030 ||
        source.slots[1].kind !== "fill" || source.slots[1].value !== 0 ||
        existsSync(join(output, sourceName(0x08000020)))) {
      throw new Error("synthetic veneer extraction differs");
    }
    verifyVeneerIsland(rom, join(output, "index.json"));
    if (!buildVeneerIsland(join(output, "index.json")).equals(rom.subarray(0x28, 0x38))) {
      throw new Error("synthetic veneer build differs");
    }
    const liveCode = Buffer.from(rom);
    liveCode.fill(0, 0x28, 0x30);
    if (!reject(() => inspectVeneerIsland(liveCode, layout))) throw new Error("non-veneer live slot was accepted");
    const unknownCode = Buffer.from(rom);
    unknownCode[0x30] = 1;
    if (!reject(() => inspectVeneerIsland(unknownCode, layout))) throw new Error("non-fill unknown slot was accepted");
    const cycle = Buffer.from(rom);
    encodeVeneer(0x08000021).copy(cycle, 0x60);
    if (!reject(() => validateExistingVeneers(cycle, layout, existingDirectory))) throw new Error("veneer cycle was accepted");
    const changedExisting = Buffer.from(rom);
    encodeVeneer(0x08000081).copy(changedExisting, 0x20);
    if (!reject(() => validateExistingVeneers(changedExisting, layout, existingDirectory))) {
      throw new Error("changed existing veneer was accepted");
    }
    const firstSource = join(output, source.slots[0].source), original = readFileSync(firstSource, "utf8");
    writeFileSync(firstSource, `${original}\n`);
    if (!reject(() => buildVeneerIsland(join(output, "index.json")))) throw new Error("noncanonical veneer source was accepted");
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok existing=78 missing=56 live_missing=21 unknown=35 sparse_bytes=448");
}

function usage(): never {
  console.log("usage: veneer_island.ts export ROM LAYOUT --directory DIR | build INDEX -o OUTPUT | verify ROM INDEX | --self-test");
  process.exit(0);
}

function main(args: string[]): void {
  if (args.length === 1 && args[0] === "--self-test") { selfTest(); return; }
  if (args[0] === "export" && args.length === 5 && args[3] === "--directory") {
    exportVeneerIsland(readFileSync(args[1]), args[2], args[4]);
    return;
  }
  if (args[0] === "build" && args.length === 4 && (args[2] === "-o" || args[2] === "--output")) {
    const built = buildVeneerIsland(args[1]);
    writeFileSync(args[3], built);
    console.log(`address=${addressText(parseVeneerIslandPackage(args[1]).address)} bytes=${built.length}`);
    return;
  }
  if (args[0] === "verify" && args.length === 3) {
    const built = verifyVeneerIsland(readFileSync(args[1]), args[2]);
    console.log(`bytes=${built.length} exact=true`);
    return;
  }
  if (args.includes("-h") || args.includes("--help")) usage();
  usage();
}

if (import.meta.main) main(process.argv.slice(2));
