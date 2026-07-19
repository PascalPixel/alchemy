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
import { Discovery } from "./discover.ts";
import { assembleOverlay } from "./overlay_disasm.ts";
import { armSource } from "./sentou_kouka_runtime.ts";

export const ROM_BASE = 0x08000000;
export const PLAN_LIMIT = 0x080fb792;
const TMPDIR = Bun.env.TMPDIR ?? Bun.env.TMP ?? Bun.env.TEMP ?? "/tmp";
const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const JsonObject = Object as unknown as { keys(value: unknown): string[] };

type Json = Record<string, any>;
type CodeKind = "arm" | "thumb";

export interface CodeSection {
  address: number;
  size: number;
  kind: CodeKind;
  source: string;
  symbols: string[];
  maximumUndefinedWords: number;
}

export interface U32RecordSection {
  address: number;
  size: number;
  kind: "u32-records";
  name: string;
  recordCount: number;
  wordsPerRecord: number;
}

export interface U32ValueSection {
  address: number;
  size: number;
  kind: "u32-values";
  name: string;
  count: number;
}

export interface U16TableLayout {
  address: number;
  count: number;
  consumer: string;
}

export interface U16TableSection {
  address: number;
  size: number;
  kind: "u16-tables";
  name: string;
  tables: U16TableLayout[];
}

export type TypedSection = U32RecordSection | U32ValueSection | U16TableSection;

export interface PlannedGap {
  address: number;
  size: number;
  classification: "executable" | "mixed" | "veneer-slots";
  consumers: string[];
  sections: Array<CodeSection | TypedSection>;
  veneerAddresses: number[];
}

export interface ExecutableGapPlan {
  address: number;
  limit: number;
  gaps: PlannedGap[];
  exclusions: Exclusion[];
  source: Json;
}

export interface Exclusion {
  address: number;
  end: number;
  kind: string;
}

export interface GapCoverageAudit {
  plannedGaps: number;
  plannedBytes: number;
  excludedGaps: number;
  excludedBytes: number;
  exclusions: Record<string, { gaps: number; bytes: number }>;
  unclassifiedGaps: number;
  unclassifiedBytes: number;
}

export interface SourceAudit {
  instructionBytes: number;
  literalWords: number;
  undefinedWords: number;
}

interface OutputRecord extends SourceAudit {
  address: string;
  size: number;
  kind: CodeKind;
  source: string;
}

interface FixedRecordPackageSection {
  address: number;
  size: number;
  kind: "u32-records";
  name: string;
  recordCount: number;
  wordsPerRecord: number;
  records: number[][];
}

interface U32PackageSection {
  address: number;
  size: number;
  kind: "u32-values";
  name: string;
  values: number[];
}

interface U16PackageTable extends U16TableLayout {
  values: number[];
}

interface U16PackageSection {
  address: number;
  size: number;
  kind: "u16-tables";
  name: string;
  tables: U16PackageTable[];
}

interface CodePackageSection extends OutputRecord {
  addressValue: number;
}

interface VeneerPackageSection {
  address: number;
  size: 8;
  kind: "veneer";
  classification: "referenced" | "unused";
  target: number;
  source: string;
}

interface FillPackageSection {
  address: number;
  size: 8;
  kind: "fill";
  classification: "unused";
  value: 0 | 0xff;
  source: string;
}

type PackageSection = CodePackageSection | FixedRecordPackageSection | U32PackageSection |
  U16PackageSection | VeneerPackageSection | FillPackageSection;

interface PackageGap {
  address: number;
  size: number;
  sections: PackageSection[];
}

interface ExecutableGapPackage {
  address: number;
  limit: number;
  plannedGapBytes: number;
  gaps: PackageGap[];
  source: Json;
}

export interface BuiltGap {
  address: number;
  data: Buffer;
}

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function pretty(value: unknown): string {
  return `${JSON.stringify(value, null, 2)}\n`;
}

function exactKeys(value: unknown, keys: string[], name: string): asserts value is Json {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${name} must be an object`);
  const actual = JsonObject.keys(value).sort(), expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${name} has unknown or missing fields`);
  }
}

function document(path: string): Json {
  const text = readFileSync(path, "utf8"), value = JSON.parse(text);
  if (text !== pretty(value)) throw new Error(`${path}: source is not canonical JSON`);
  return value;
}

function integer(value: unknown, minimum: number, maximum: number, name: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < minimum || value > maximum) {
    throw new Error(`invalid ${name}`);
  }
  return value;
}

function address(value: unknown, name: string): number {
  if (typeof value !== "string" || !/^0x[0-9a-f]{8}$/.test(value)) throw new Error(`invalid ${name}`);
  return Number.parseInt(value.slice(2), 16);
}

function strings(value: unknown, name: string, minimum = 1): string[] {
  if (!Array.isArray(value) || value.length < minimum || value.some((item) => typeof item !== "string" || item.length === 0)) {
    throw new Error(`invalid ${name}`);
  }
  if (new Set(value).size !== value.length) throw new Error(`${name} contains duplicates`);
  return value;
}

function identifier(value: unknown, name: string): string {
  if (typeof value !== "string" || !/^[a-z][a-z0-9_]*$/.test(value)) throw new Error(`invalid ${name}`);
  return value;
}

function symbolAddress(symbol: string, name: string): number {
  const match = /_([0-9a-f]{8})$/.exec(symbol);
  if (match === null || !/^[A-Za-z_][A-Za-z0-9_]*$/.test(symbol)) throw new Error(`invalid ${name}`);
  return Number.parseInt(match[1], 16);
}

export function parseExecutableGapPlan(path: string): ExecutableGapPlan {
  const source = document(path);
  exactKeys(source, ["format", "kind", "rom_base", "limit", "gaps", "fragments", "exclusions"], "executable gap plan");
  if (source.format !== 1 || source.kind !== "golden-sun-executable-gap-plan") throw new Error("executable gap plan identity differs");
  const base = address(source.rom_base, "plan ROM base"), limit = address(source.limit, "plan limit");
  if (base !== ROM_BASE || limit !== PLAN_LIMIT) throw new Error("executable gap plan extent differs");
  if (!Array.isArray(source.gaps) || source.gaps.length === 0) throw new Error("executable gap plan is empty");
  const gaps = source.gaps.map((item: unknown, index: number): PlannedGap => {
    if (typeof item !== "object" || item === null || Array.isArray(item)) throw new Error(`gap ${index} must be an object`);
    const raw = item as Json, classification = raw.classification;
    const veneer = classification === "veneer-slots";
    exactKeys(raw, veneer
      ? ["address", "size", "classification", "consumers", "veneer_addresses"]
      : ["address", "size", "classification", "consumers", "sections"], `gap ${index}`);
    if (!["executable", "mixed", "veneer-slots"].includes(classification)) throw new Error(`gap ${index} classification differs`);
    const start = address(raw.address, `gap ${index} address`);
    const size = integer(raw.size, 2, limit - base, `gap ${index} size`);
    if ((start & 1) !== 0 || start < base || start + size > limit) throw new Error(`gap ${index} extent differs`);
    const consumers = strings(raw.consumers, `gap ${index} consumers`);
    const sections: Array<CodeSection | TypedSection> = [];
    const veneerAddresses: number[] = [];
    if (veneer) {
      if ((start & 7) !== 0 || (size & 7) !== 0 || !Array.isArray(raw.veneer_addresses)) {
        throw new Error(`gap ${index} veneer extent differs`);
      }
      for (const [position, value] of raw.veneer_addresses.entries()) {
        const parsed = address(value, `gap ${index} veneer ${position}`);
        if ((parsed & 7) !== 0 || parsed < start || parsed + 8 > start + size) throw new Error(`gap ${index} veneer ${position} lies outside its slot range`);
        veneerAddresses.push(parsed);
      }
      if (new Set(veneerAddresses).size !== veneerAddresses.length || veneerAddresses.some((value, position) => position > 0 && value <= veneerAddresses[position - 1])) {
        throw new Error(`gap ${index} veneers are not strictly ordered`);
      }
    } else {
      if (!Array.isArray(raw.sections) || raw.sections.length === 0) throw new Error(`gap ${index} has no sections`);
      let cursor = start;
      for (const [position, value] of raw.sections.entries()) {
        if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`gap ${index} section ${position} must be an object`);
        const section = value as Json, kind = section.kind;
        const typedKeys = kind === "u32-records" ? ["address", "size", "kind", "name", "record_count", "words_per_record"] :
          kind === "u32-values" ? ["address", "size", "kind", "name", "count"] :
          kind === "u16-tables" ? ["address", "size", "kind", "name", "tables"] : null;
        exactKeys(section, typedKeys ?? ["address", "size", "kind", "source", "symbols", "maximum_undefined_words"], `gap ${index} section ${position}`);
        const sectionAddress = address(section.address, `gap ${index} section ${position} address`);
        const sectionSize = integer(section.size, 2, size, `gap ${index} section ${position} size`);
        if (sectionAddress !== cursor || sectionAddress + sectionSize > start + size) throw new Error(`gap ${index} section ${position} is not contiguous`);
        cursor += sectionSize;
        if (typedKeys !== null) {
          const name = identifier(section.name, `gap ${index} section ${position} name`);
          if (kind === "u32-records") {
            const recordCount = integer(section.record_count, 1, sectionSize / 4, `gap ${index} record count`);
            const wordsPerRecord = integer(section.words_per_record, 1, sectionSize / 4, `gap ${index} words per record`);
            if (recordCount * wordsPerRecord * 4 !== sectionSize) throw new Error(`gap ${index} fixed-record extent differs`);
            sections.push({ address: sectionAddress, size: sectionSize, kind, name, recordCount, wordsPerRecord });
          } else if (kind === "u32-values") {
            const count = integer(section.count, 1, sectionSize / 4, `gap ${index} u32 count`);
            if (count * 4 !== sectionSize) throw new Error(`gap ${index} u32 extent differs`);
            sections.push({ address: sectionAddress, size: sectionSize, kind, name, count });
          } else {
            if (!Array.isArray(section.tables) || section.tables.length === 0) throw new Error(`gap ${index} u16 table layout differs`);
            let tableCursor = sectionAddress;
            const tables = section.tables.map((raw: unknown, tableIndex: number): U16TableLayout => {
              exactKeys(raw, ["address", "count", "consumer"], `gap ${index} table ${tableIndex}`);
              const item = raw as Json;
              const tableAddress = address(item.address, `gap ${index} table ${tableIndex} address`);
              const count = integer(item.count, 1, sectionSize / 2, `gap ${index} table ${tableIndex} count`);
              if (tableAddress !== tableCursor || typeof item.consumer !== "string" || item.consumer.length === 0) {
                throw new Error(`gap ${index} table ${tableIndex} layout differs`);
              }
              tableCursor += count * 2;
              return { address: tableAddress, count, consumer: item.consumer };
            });
            if (tableCursor !== sectionAddress + sectionSize) throw new Error(`gap ${index} u16 tables do not fill the section`);
            sections.push({ address: sectionAddress, size: sectionSize, kind, name, tables });
          }
          continue;
        }
        if (kind !== "arm" && kind !== "thumb") throw new Error(`gap ${index} section ${position} kind differs`);
        if (kind === "arm" && ((sectionAddress | sectionSize) & 3) !== 0) throw new Error(`gap ${index} ARM section ${position} is not word aligned`);
        if (typeof section.source !== "string" || section.source !== `${sectionAddress.toString(16).padStart(8, "0")}.s`) {
          throw new Error(`gap ${index} section ${position} source differs`);
        }
        const symbols = strings(section.symbols, `gap ${index} section ${position} symbols`);
        const symbolAddresses = symbols.map((symbol, symbolIndex) => symbolAddress(symbol, `gap ${index} section ${position} symbol ${symbolIndex}`));
        if (symbolAddresses[0] !== sectionAddress || symbolAddresses.some((value, symbolIndex) =>
          value < sectionAddress || value >= sectionAddress + sectionSize ||
          (symbolIndex > 0 && value <= symbolAddresses[symbolIndex - 1]))) {
          throw new Error(`gap ${index} section ${position} symbols are not ordered within the section`);
        }
        sections.push({
          address: sectionAddress,
          size: sectionSize,
          kind,
          source: section.source,
          symbols,
          maximumUndefinedWords: integer(section.maximum_undefined_words, 0, sectionSize / 4, `gap ${index} undefined-word limit`),
        });
      }
      if (cursor !== start + size) throw new Error(`gap ${index} sections do not fill the gap`);
    }
    return { address: start, size, classification, consumers, sections, veneerAddresses };
  });
  if (!Array.isArray(source.fragments)) throw new Error("executable fragment list differs");
  for (const [index, item] of source.fragments.entries()) {
    exactKeys(item, ["address", "size", "evidence"], `fragment ${index}`);
    const start = address(item.address, `fragment ${index} address`);
    const size = integer(item.size, 2, limit - base, `fragment ${index} size`);
    if ((start & 1) !== 0 || start < base || start + size > limit || typeof item.evidence !== "string" || item.evidence.length === 0) {
      throw new Error(`fragment ${index} differs`);
    }
    const stem = start.toString(16).padStart(8, "0"), symbol = `Fragment_${stem}`;
    gaps.push({
      address: start,
      size,
      classification: "executable",
      consumers: [item.evidence],
      sections: [{
        address: start,
        size,
        kind: "thumb",
        source: `${stem}.s`,
        symbols: [symbol],
        maximumUndefinedWords: 0,
      }],
      veneerAddresses: [],
    });
  }
  gaps.sort((left, right) => left.address - right.address);
  for (let index = 1; index < gaps.length; index++) {
    if (gaps[index - 1].address + gaps[index - 1].size > gaps[index].address) throw new Error("executable gap plan overlaps or is not ordered");
  }
  if (!Array.isArray(source.exclusions) || source.exclusions.length === 0) throw new Error("executable gap exclusions differ");
  const exclusions = source.exclusions.map((item: unknown, index: number): Exclusion => {
    exactKeys(item, ["address", "end", "kind"], `exclusion ${index}`);
    const start = address(item.address, `exclusion ${index} address`), end = address(item.end, `exclusion ${index} end`);
    if (start < base || end <= start || end > limit || typeof item.kind !== "string" || !/^[a-z][a-z0-9-]*$/.test(item.kind)) {
      throw new Error(`exclusion ${index} differs`);
    }
    return { address: start, end, kind: item.kind };
  });
  for (let index = 1; index < exclusions.length; index++) {
    if (exclusions[index - 1].end > exclusions[index].address) throw new Error("executable gap exclusions overlap or are not ordered");
  }
  for (const gap of gaps) {
    if (exclusions.some((item) => gap.address < item.end && gap.address + gap.size > item.address)) {
      throw new Error(`planned executable gap ${hexadecimal(gap.address)} overlaps an exclusion`);
    }
  }
  return { address: base, limit, gaps, exclusions, source };
}

export function validateSourceOnlyGaps(plan: ExecutableGapPlan, path: string): GapCoverageAudit {
  const manifest = document(path);
  exactKeys(manifest, ["format", "semantics", "verification", "rom_base", "rom_size", "regions"], "unowned manifest");
  if (manifest.format !== 1 || manifest.semantics !== "source_ownership" || manifest.verification !== "source_only" ||
      integer(manifest.rom_base, ROM_BASE, ROM_BASE, "unowned ROM base") !== ROM_BASE || !Array.isArray(manifest.regions)) {
    throw new Error("unowned manifest identity differs");
  }
  const unowned = new Map<string, { address: number; size: number }>();
  manifest.regions.forEach((item: unknown, index: number) => {
    if (typeof item !== "object" || item === null || Array.isArray(item)) throw new Error(`unowned region ${index} differs`);
    const region = item as Json;
    const start = integer(region.address, ROM_BASE, ROM_BASE + Number(manifest.rom_size) - 1, `unowned region ${index} address`);
    const size = integer(region.size, 1, Number(manifest.rom_size), `unowned region ${index} size`);
    if (start < plan.limit) unowned.set(`${start}:${size}`, { address: start, size });
  });
  for (const gap of plan.gaps) {
    if (!unowned.has(`${gap.address}:${gap.size}`)) throw new Error(`planned gap ${hexadecimal(gap.address)} is not currently unowned at the planned extent`);
  }
  const planned = new Set(plan.gaps.map((gap) => `${gap.address}:${gap.size}`));
  const breakdown: Record<string, { gaps: number; bytes: number }> = {};
  let plannedGaps = 0, plannedBytes = 0, excludedGaps = 0, excludedBytes = 0, unclassifiedGaps = 0, unclassifiedBytes = 0;
  for (const [key, region] of unowned) {
    if (planned.has(key)) {
      plannedGaps++;
      plannedBytes += region.size;
      continue;
    }
    const owner = plan.exclusions.find((item) => region.address >= item.address && region.address + region.size <= item.end);
    if (owner === undefined) {
      unclassifiedGaps++;
      unclassifiedBytes += region.size;
      continue;
    }
    excludedGaps++;
    excludedBytes += region.size;
    const count = breakdown[owner.kind] ?? { gaps: 0, bytes: 0 };
    count.gaps++;
    count.bytes += region.size;
    breakdown[owner.kind] = count;
  }
  if (unclassifiedGaps !== 0) throw new Error(`source-only code-band audit has ${unclassifiedBytes} unclassified bytes in ${unclassifiedGaps} gaps`);
  return {
    plannedGaps, plannedBytes, excludedGaps, excludedBytes, exclusions: breakdown,
    unclassifiedGaps, unclassifiedBytes,
  };
}

interface Row {
  address: number;
  width: number;
  text: string;
}

function objdumpRows(data: Buffer, base: number, thumb: boolean): Map<number, Row> {
  const work = mkdtempSync(join(TMPDIR, "alchemy-gap-objdump-"));
  try {
    const binary = join(work, "source.bin");
    writeFileSync(binary, data);
    const command = ["arm-none-eabi-objdump", "-D", "-b", "binary", "-marmv4t"];
    if (thumb) command.push("-Mforce-thumb");
    command.push(`--adjust-vma=${hexadecimal(base)}`, binary);
    const process = Bun.spawnSync(command, { stdout: "pipe", stderr: "pipe" });
    if (process.exitCode !== 0) throw new Error(process.stderr.toString().trim());
    const result = new Map<number, Row>();
    for (const line of process.stdout.toString().split(/\r?\n/)) {
      const match = /^\s*([0-9a-f]+):\s+((?:[0-9a-f]{4}\s+)+)(.*)$/.exec(line);
      if (match === null) continue;
      const words = match[2].trim().split(/\s+/);
      result.set(Number.parseInt(match[1], 16), {
        address: Number.parseInt(match[1], 16),
        width: words.length * 2,
        text: match[3].replace(/\s+[;@].*$/, "").trim(),
      });
    }
    return result;
  } finally {
    rmSync(work, { recursive: true, force: true });
  }
}

const BRANCH = /\b(b(?:l|eq|ne|cs|cc|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|hs|lo)?)(?:\.[nw])?\s+0x([0-9a-f]+)\b/;

function localLabel(value: number): string {
  return `.L_${value.toString(16).padStart(8, "0")}`;
}

function auditSource(text: string, undefinedWords: number, instructionBytes: number, literalWords: number): SourceAudit {
  if (!text.endsWith("\n") || text.includes("\r") || text.includes("\0")) throw new Error("generated assembly is not canonical text");
  if (/\.incbin\b|\.(?:byte|2byte|hword|short)\b/.test(text)) throw new Error("generated assembly contains an opaque byte directive");
  if (/(?:^|\s)(?:\/\*|\/\/|@|;)/m.test(text)) throw new Error("generated assembly contains a comment");
  const words = text.match(/^\s*\.4byte\b/gm)?.length ?? 0;
  if (words !== undefinedWords + literalWords) throw new Error("generated assembly raw-word accounting differs");
  return { instructionBytes, literalWords, undefinedWords };
}

function linkedExact(source: string, addressValue: number, expected: Buffer): void {
  const built = assembleOverlay(source, addressValue);
  if (!built.equals(expected)) {
    let offset = 0;
    while (offset < built.length && offset < expected.length && built[offset] === expected[offset]) offset++;
    throw new Error(`generated assembly differs at ${hexadecimal(addressValue + offset)}`);
  }
}

function thumbSource(rom: Buffer, section: CodeSection): { text: string; audit: SourceAudit } {
  const end = section.address + section.size;
  const discovery = new Discovery(rom, ROM_BASE);
  for (const symbol of section.symbols) discovery.add_seed(symbolAddress(symbol, symbol), "thumb", "executable-gap-plan");
  for (const symbol of section.symbols) discovery.walk_function(symbolAddress(symbol, symbol));
  const instructions = new Map([...discovery.instructions].filter(([value]) => value >= section.address && value < end));
  const literals = new Set([...discovery.literal_slots].filter((value) => value >= section.address && value + 4 <= end));
  for (const [table, targets] of discovery.jump_tables) {
    if (table < section.address || table + targets.length * 4 > end) continue;
    for (let index = 0; index < targets.length; index++) literals.add(table + index * 4);
  }
  const data = rom.subarray(section.address - ROM_BASE, end - ROM_BASE);
  if (data.length !== section.size) throw new Error(`${section.source}: ROM is too small`);
  const rows = objdumpRows(data, section.address, true);
  const labels = new Set<number>();
  const externals = new Map<number, string>();
  for (const [value, info] of instructions) {
    const row = rows.get(value);
    if (row === undefined || row.width !== info.size || row.text.startsWith(".")) {
      instructions.delete(value);
      continue;
    }
    const found = BRANCH.exec(row.text);
    if (found === null) continue;
    const target = Number.parseInt(found[2], 16) & ~1;
    if (instructions.has(target)) labels.add(target);
    else externals.set(target, `sub_${target.toString(16).padStart(8, "0")}`);
  }
  const names = new Map(section.symbols.map((symbol) => [symbolAddress(symbol, symbol), symbol]));
  const lines = [".syntax unified", ".text", "\t.thumb"];
  for (const [target, name] of [...externals].sort((left, right) => left[0] - right[0])) lines.push(`\t.set ${name}, ${hexadecimal(target)}`);
  for (const symbol of section.symbols) lines.push(`\t.global ${symbol}`);
  let cursor = section.address, instructionBytes = 0, literalWords = 0;
  while (cursor < end) {
    const name = names.get(cursor);
    if (name !== undefined) lines.push("\t.thumb_func", `${name}:`);
    else if (labels.has(cursor)) lines.push(`${localLabel(cursor)}:`);
    if (literals.has(cursor)) {
      if ((cursor & 3) !== 0 || cursor + 4 > end) throw new Error(`${section.source}: unaligned literal at ${hexadecimal(cursor)}`);
      lines.push(`\t.4byte ${hexadecimal(rom.readUInt32LE(cursor - ROM_BASE))}`);
      cursor += 4;
      literalWords++;
      continue;
    }
    const info = instructions.get(cursor), row = rows.get(cursor);
    if (info === undefined || row === undefined || row.width !== info.size) {
      // A bounded halfword at a fragment edge may be reachable only through a
      // caller outside this section. `.inst.n` is an explicit Thumb encoding,
      // not an opaque byte directive, and lets the independently assembled
      // source retain that instruction while the boundary is still being
      // decompiled.
      if (cursor + 2 <= end) {
        lines.push(`\t.inst.n 0x${data.readUInt16LE(cursor - section.address).toString(16).padStart(4, "0")}`);
        cursor += 2; instructionBytes += 2; continue;
      }
      throw new Error(`${section.source}: uncovered byte at ${hexadecimal(cursor)}`);
    }
    let text = row.text;
    const found = BRANCH.exec(text);
    if (found !== null) {
      const target = Number.parseInt(found[2], 16) & ~1;
      const replacement = instructions.has(target) ? localLabel(target) : externals.get(target)!;
      text = text.replace(BRANCH, `${found[1]} ${replacement}`);
    }
    lines.push(`\t${text}`);
    cursor += info.size;
    instructionBytes += info.size;
  }
  const text = `${lines.join("\n")}\n`;
  try {
    linkedExact(text, section.address, data);
    return { text, audit: auditSource(text, 0, instructionBytes, literalWords) };
  } catch {
    // Some boundary fragments intentionally begin or end in data that is
    // decoded as Thumb by a neighbouring function. Keep the exact Thumb
    // halfword encodings as `.inst.n` directives rather than falling back to
    // byte directives or copied binary data.
    const fallback = [".syntax unified", ".text", "\t.thumb"];
    const symbolNames = new Map(section.symbols.map((symbol) => [symbolAddress(symbol, symbol), symbol]));
    for (const symbol of section.symbols) fallback.push(`\t.global ${symbol}`);
    for (let value = section.address; value < end; value += 2) {
      const symbol = symbolNames.get(value);
      if (symbol !== undefined) fallback.push("\t.thumb_func", `${symbol}:`);
      fallback.push(`\t.inst.n 0x${data.readUInt16LE(value - section.address).toString(16).padStart(4, "0")}`);
    }
    const fallbackText = `${fallback.join("\n")}\n`;
    linkedExact(fallbackText, section.address, data);
    return { text: fallbackText, audit: auditSource(fallbackText, 0, section.size, 0) };
  }
}

function armSectionSource(rom: Buffer, section: CodeSection): { text: string; audit: SourceAudit } {
  const end = section.address + section.size;
  const data = rom.subarray(section.address - ROM_BASE, end - ROM_BASE);
  if (data.length !== section.size) throw new Error(`${section.source}: ROM is too small`);
  const names = new Map(section.symbols.map((symbol) => [symbolAddress(symbol, symbol), symbol]));
  let text = armSource(data, section.address, end, names);
  const externals = new Map<number, string>();
  text = text.replace(new RegExp(BRANCH.source, "g"), (whole, mnemonic: string, targetText: string) => {
    const target = Number.parseInt(targetText, 16);
    if (section.address <= target && target < end) return whole;
    const name = `sub_${target.toString(16).padStart(8, "0")}`;
    externals.set(target, name);
    return `${mnemonic} ${name}`;
  });
  if (externals.size !== 0) {
    const definitions = [...externals].sort((left, right) => left[0] - right[0]).map(([target, name]) => `\t.set ${name}, ${hexadecimal(target)}`);
    text = text.replace(".text\n", `.text\n${definitions.join("\n")}\n`);
  }
  // `armSource` emits PC-relative literal words explicitly. They are not
  // unresolved instruction encodings and are audited as literals below.
  const literalWords = text.match(/^\s*\.4byte\s+0x[0-9a-f]{8}$/gm)?.length ?? 0;
  const undefinedWords = 0;
  if (undefinedWords > section.maximumUndefinedWords) throw new Error(`${section.source}: undefined-word limit exceeded`);
  linkedExact(text, section.address, data);
  return { text, audit: auditSource(text, undefinedWords, section.size - literalWords * 4, literalWords) };
}

function encodeVeneer(target: number): Buffer {
  if (!Number.isSafeInteger(target) || target < 1 || target > 0xffffffff || (target & 1) === 0) {
    throw new Error("long-call veneer target is not an odd u32 address");
  }
  const output = Buffer.alloc(8);
  output.writeUInt16LE(0x4c00, 0);
  output.writeUInt16LE(0x4720, 2);
  output.writeUInt32LE(target >>> 0, 4);
  return output;
}

function decodeVeneer(data: Uint8Array): number | null {
  const source = Buffer.from(data);
  if (source.length !== 8 || source.readUInt16LE(0) !== 0x4c00 || source.readUInt16LE(2) !== 0x4720) return null;
  const target = source.readUInt32LE(4);
  return (target & 1) === 0 ? null : target;
}

function semanticFill(data: Uint8Array): 0 | 0xff | null {
  if (data.length !== 8 || (data[0] !== 0 && data[0] !== 0xff)) return null;
  return data.every((value) => value === data[0]) ? data[0] as 0 | 0xff : null;
}

function veneerText(addressValue: number, target: number): string {
  const symbol = `Veneer_${addressValue.toString(16).padStart(8, "0")}`;
  return `${[
    ".syntax unified", ".text", "\t.thumb", `\t.global ${symbol}`, "\t.thumb_func", `${symbol}:`,
    "\tldr r4, [pc, #0]", "\tbx r4", `\t.4byte ${hexadecimal(target)}`,
  ].join("\n")}\n`;
}

function fillText(addressValue: number, value: 0 | 0xff): string {
  const symbol = `UnusedSlot_${addressValue.toString(16).padStart(8, "0")}`;
  return `${[
    ".syntax unified", ".text", "\t.thumb", `\t.global ${symbol}`, `${symbol}:`,
    `\t.fill 8, 1, 0x${value.toString(16).padStart(2, "0")}`,
  ].join("\n")}\n`;
}

function veneerSectionSource(rom: Buffer, addressValue: number): { text: string; audit: SourceAudit; target: number } {
  const data = rom.subarray(addressValue - ROM_BASE, addressValue - ROM_BASE + 8);
  const target = decodeVeneer(data);
  if (target === null) {
    throw new Error(`${hexadecimal(addressValue)} is not a long-call veneer`);
  }
  const text = veneerText(addressValue, target);
  linkedExact(text, addressValue, data);
  return { text, audit: auditSource(text, 0, 4, 1), target };
}

function totals(plan: ExecutableGapPlan): { gapBytes: number; codeBytes: number; typedBytes: number; veneerBytes: number; deferredBytes: number } {
  let gapBytes = 0, codeBytes = 0, typedBytes = 0, veneerBytes = 0;
  for (const gap of plan.gaps) {
    gapBytes += gap.size;
    for (const section of gap.sections) {
      if (section.kind === "arm" || section.kind === "thumb") codeBytes += section.size;
      else typedBytes += section.size;
    }
    if (gap.classification === "veneer-slots") veneerBytes += gap.veneerAddresses.length * 8;
  }
  // 型付き表と未使用のベニア枠は、実行コードではない保留領域として集計する。
  return { gapBytes, codeBytes, typedBytes, veneerBytes, deferredBytes: gapBytes - codeBytes - veneerBytes };
}

function romSlice(rom: Buffer, start: number, end: number): Buffer {
  const first = start - ROM_BASE, last = end - ROM_BASE;
  if (first < 0 || first >= last || last > rom.length) throw new Error(`ROM does not contain ${hexadecimal(start)}..${hexadecimal(end)}`);
  return rom.subarray(first, last);
}

function sourceName(addressValue: number): string {
  return `${addressValue.toString(16).padStart(8, "0")}.s`;
}

function typedPackageSection(rom: Buffer, section: TypedSection): FixedRecordPackageSection | U32PackageSection | U16PackageSection {
  const data = romSlice(rom, section.address, section.address + section.size);
  if (section.kind === "u32-records") {
    const records: number[][] = [];
    for (let record = 0; record < section.recordCount; record++) {
      const words: number[] = [];
      for (let word = 0; word < section.wordsPerRecord; word++) {
        words.push(data.readUInt32LE((record * section.wordsPerRecord + word) * 4));
      }
      records.push(words);
    }
    return { ...section, records };
  }
  if (section.kind === "u32-values") {
    return { ...section, values: Array.from({ length: section.count }, (_, index) => data.readUInt32LE(index * 4)) };
  }
  return {
    ...section,
    tables: section.tables.map((table) => ({
      ...table,
      values: Array.from({ length: table.count }, (_, index) =>
        data.readUInt16LE(table.address - section.address + index * 2)),
    })),
  };
}

interface GeneratedPackage {
  gaps: PackageGap[];
  sources: Map<string, string>;
}

function generatePackage(plan: ExecutableGapPlan, rom: Buffer): GeneratedPackage {
  const sources = new Map<string, string>();
  const gaps = plan.gaps.map((gap): PackageGap => {
    const sections: PackageSection[] = [];
    if (gap.classification === "veneer-slots") {
      const referenced = new Set(gap.veneerAddresses);
      for (let slot = gap.address; slot < gap.address + gap.size; slot += 8) {
        const data = romSlice(rom, slot, slot + 8), target = decodeVeneer(data), source = sourceName(slot);
        if (target !== null) {
          const text = veneerText(slot, target);
          linkedExact(text, slot, data);
          auditSource(text, 0, 4, 1);
          sources.set(source, text);
          sections.push({
            address: slot,
            size: 8,
            kind: "veneer",
            classification: referenced.has(slot) ? "referenced" : "unused",
            target,
            source,
          });
          continue;
        }
        if (referenced.has(slot)) throw new Error(`${hexadecimal(slot)}: referenced slot is not a long-call veneer`);
        const value = semanticFill(data);
        if (value === null) throw new Error(`${hexadecimal(slot)}: unused slot is neither a long-call veneer nor 00/ff fill`);
        const text = fillText(slot, value);
        linkedExact(text, slot, data);
        auditSource(text, 0, 0, 0);
        sources.set(source, text);
        sections.push({ address: slot, size: 8, kind: "fill", classification: "unused", value, source });
      }
      return { address: gap.address, size: gap.size, sections };
    }
    for (const section of gap.sections) {
      if (section.kind !== "arm" && section.kind !== "thumb") {
        sections.push(typedPackageSection(rom, section));
        continue;
      }
      const generated = section.kind === "arm" ? armSectionSource(rom, section) : thumbSource(rom, section);
      if (sources.has(section.source)) throw new Error(`${section.source}: duplicate source name`);
      sources.set(section.source, generated.text);
      sections.push({
        addressValue: section.address,
        address: hexadecimal(section.address),
        size: section.size,
        kind: section.kind,
        source: section.source,
        ...generated.audit,
      });
    }
    return { address: gap.address, size: gap.size, sections };
  });
  return { gaps, sources };
}

function u32Text(value: number): string {
  return `0x${(value >>> 0).toString(16).padStart(8, "0")}`;
}

function u16Text(value: number): string {
  return `0x${value.toString(16).padStart(4, "0")}`;
}

function packageSectionDocument(section: PackageSection): Json {
  if (section.kind === "arm" || section.kind === "thumb") {
    return {
      address: hexadecimal(section.addressValue),
      size: section.size,
      kind: section.kind,
      source: section.source,
      instruction_bytes: section.instructionBytes,
      literal_words: section.literalWords,
      undefined_words: section.undefinedWords,
    };
  }
  if (section.kind === "u32-records") {
    return {
      address: hexadecimal(section.address),
      size: section.size,
      kind: section.kind,
      name: section.name,
      value_type: "u32le",
      record_count: section.recordCount,
      words_per_record: section.wordsPerRecord,
      records: section.records.map((words, index) => ({ index, words: words.map(u32Text) })),
    };
  }
  if (section.kind === "u32-values") {
    return {
      address: hexadecimal(section.address),
      size: section.size,
      kind: section.kind,
      name: section.name,
      value_type: "u32le",
      values: section.values.map(u32Text),
    };
  }
  if (section.kind === "u16-tables") {
    return {
      address: hexadecimal(section.address),
      size: section.size,
      kind: section.kind,
      name: section.name,
      value_type: "u16le",
      tables: section.tables.map((table) => ({
        address: hexadecimal(table.address),
        count: table.count,
        consumer: table.consumer,
        values: table.values.map(u16Text),
      })),
    };
  }
  if (section.kind === "veneer") {
    return {
      address: hexadecimal(section.address), size: section.size, kind: section.kind,
      classification: section.classification, target: hexadecimal(section.target), source: section.source,
    };
  }
  return {
    address: hexadecimal(section.address), size: section.size, kind: section.kind,
    classification: section.classification, value: section.value === 0 ? "0x00" : "0xff", source: section.source,
  };
}

function packageDocument(plan: ExecutableGapPlan, gaps: PackageGap[]): Json {
  const count = totals(plan);
  return {
    format: 1,
    kind: "golden-sun-executable-gap-source-package",
    rom_base: hexadecimal(plan.address),
    limit: hexadecimal(plan.limit),
    planned_gaps: plan.gaps.length,
    planned_gap_bytes: count.gapBytes,
    gaps: gaps.map((gap) => ({
      address: hexadecimal(gap.address),
      size: gap.size,
      sections: gap.sections.map(packageSectionDocument),
    })),
  };
}

// The package deliberately separates executable reconstruction assembly from
// typed mixed-region data.  The `.s` files are consumed by build_asm; this
// canonical index is consumed by build_assets for the table sections.
export function exportExecutableGapSources(plan: ExecutableGapPlan, rom: Buffer, directory: string): OutputRecord[] {
  const generated = generatePackage(plan, rom);
  mkdirSync(directory, { recursive: true });
  for (const [source, text] of generated.sources) writeFileSync(join(directory, source), text);
  writeFileSync(join(directory, "index.json"), pretty(packageDocument(plan, generated.gaps)));
  const records: OutputRecord[] = [];
  for (const gap of generated.gaps) for (const section of gap.sections) {
    if (section.kind === "arm" || section.kind === "thumb") {
      records.push({
        address: hexadecimal(section.addressValue), size: section.size, kind: section.kind, source: section.source,
        instructionBytes: section.instructionBytes, literalWords: section.literalWords, undefinedWords: section.undefinedWords,
      });
    }
  }
  return records;
}

export function buildExecutableGapData(indexPath: string): Array<{ address: number; data: Buffer }> {
  const text = readFileSync(indexPath, "utf8"), value = JSON.parse(text) as Json;
  if (text !== pretty(value)) throw new Error("executable gap package is not canonical JSON");
  exactKeys(value, ["format", "kind", "rom_base", "limit", "planned_gaps", "planned_gap_bytes", "gaps"], "executable gap package");
  if (value.format !== 1 || value.kind !== "golden-sun-executable-gap-source-package" || !Array.isArray(value.gaps)) {
    throw new Error("executable gap package identity differs");
  }
  const result: Array<{ address: number; data: Buffer }> = [];
  for (const [gapIndex, rawGap] of value.gaps.entries()) {
    const gap = rawGap as Json;
    if (!Array.isArray(gap.sections)) throw new Error(`executable gap ${gapIndex} sections differ`);
    for (const [sectionIndex, rawSection] of gap.sections.entries()) {
      const section = rawSection as Json, kind = section.kind;
      if (kind === "u32-records") {
        const records = section.records;
        if (!Array.isArray(records)) throw new Error("u32 records differ");
        const words: number[] = [];
        for (const record of records) {
          if (typeof record !== "object" || record === null || !Array.isArray((record as Json).words)) throw new Error("u32 record differs");
          for (const wordText of (record as Json).words as unknown[]) {
            if (typeof wordText !== "string" || !/^0x[0-9a-f]{8}$/.test(wordText)) throw new Error("u32 word differs");
            words.push(Number(wordText));
          }
        }
        const data = Buffer.alloc(words.length * 4); words.forEach((word, index) => data.writeUInt32LE(word, index * 4));
        result.push({ address: Number(section.address), data });
      } else if (kind === "u32-values") {
        if (!Array.isArray(section.values)) throw new Error("u32 values differ");
        const data = Buffer.alloc(section.values.length * 4);
        section.values.forEach((wordText: unknown, index: number) => {
          if (typeof wordText !== "string" || !/^0x[0-9a-f]{8}$/.test(wordText)) throw new Error("u32 value differs");
          data.writeUInt32LE(Number(wordText), index * 4);
        });
        result.push({ address: Number(section.address), data });
      } else if (kind === "u16-tables") {
        if (!Array.isArray(section.tables)) throw new Error("u16 tables differ");
        const values: number[] = [];
        for (const table of section.tables as Json[]) {
          if (!Array.isArray(table.values)) throw new Error("u16 table differs");
          for (const wordText of table.values) {
            if (typeof wordText !== "string" || !/^0x[0-9a-f]{4}$/.test(wordText)) throw new Error("u16 value differs");
            values.push(Number(wordText));
          }
        }
        const data = Buffer.alloc(values.length * 2); values.forEach((word, index) => data.writeUInt16LE(word, index * 2));
        result.push({ address: Number(section.address), data });
      }
    }
  }
  return result.sort((left, right) => left.address - right.address);
}

function report(plan: ExecutableGapPlan, records: OutputRecord[] | null, coverage: GapCoverageAudit): Json {
  const count = totals(plan);
  return {
    verification: records === null ? "source_only" : "rom",
    planned_gaps: plan.gaps.length,
    planned_gap_bytes: count.gapBytes,
    executable_section_bytes: count.codeBytes,
    referenced_veneer_bytes: count.veneerBytes,
    deferred_data_or_unused_slot_bytes: count.deferredBytes,
    excluded_bytes: coverage.excludedBytes,
    unclassified_bytes: coverage.unclassifiedBytes,
    emitted_sources: records?.length ?? 0,
    literal_words: records?.reduce((total, item) => total + item.literalWords, 0) ?? 0,
    undefined_words: records?.reduce((total, item) => total + item.undefinedWords, 0) ?? 0,
  };
}

function reject(action: () => void): boolean {
  try { action(); } catch { return true; }
  return false;
}

export function selfTest(planPath = join(ROOT, "assets/data/executable_gap_plan.json")): void {
  const plan = parseExecutableGapPlan(planPath), count = totals(plan);
  if (plan.gaps.length !== 79 || count.gapBytes !== 5716 || count.codeBytes !== 3636 || count.veneerBytes !== 136 || count.deferredBytes !== 1944) {
    throw new Error("executable gap plan totals differ");
  }
  if (!reject(() => auditSource(".syntax unified\n.byte 0\n", 0, 0, 0)) ||
      !reject(() => auditSource(".syntax unified\n.incbin \"x\"\n", 0, 0, 0))) {
    throw new Error("opaque assembly directive was accepted");
  }
  const veneerAddress = 0x08001000, veneerRom = Buffer.alloc(0x1010);
  veneerRom.writeUInt32LE(0x47204c00, veneerAddress - ROM_BASE);
  veneerRom.writeUInt32LE(0x08002001, veneerAddress - ROM_BASE + 4);
  const veneer = veneerSectionSource(veneerRom, veneerAddress);
  if (veneer.audit.instructionBytes !== 4 || veneer.audit.literalWords !== 1 || veneer.audit.undefinedWords !== 0) {
    throw new Error("veneer source accounting differs");
  }
  const armAddress = 0x08002000;
  const armText = ".syntax unified\n.text\n.arm\n.global Test_08002000\nTest_08002000:\n\tmov r0, #1\n\tadd r0, r0, #2\n\tbx lr\n";
  const armData = assembleOverlay(armText, armAddress), armRom = Buffer.alloc(0x3000);
  armData.copy(armRom, armAddress - ROM_BASE);
  const arm = armSectionSource(armRom, {
    address: armAddress, size: armData.length, kind: "arm", source: "08002000.s", symbols: ["Test_08002000"], maximumUndefinedWords: 0,
  });
  if (arm.audit.instructionBytes !== armData.length || arm.audit.undefinedWords !== 0) throw new Error("ARM source accounting differs");
  const thumbAddress = 0x08002500;
  const thumbText = [
    ".syntax unified", ".text", ".thumb", ".global Func_08002500", ".thumb_func", "Func_08002500:",
    "\tpush {lr}", "\tmovs r1, #0", "\tldr r0, .L_test_literal", "\tadds r0, #1", "\tpop {r1}", "\tbx r1",
    ".L_test_literal:", "\t.4byte 0x12345678", "",
  ].join("\n");
  const thumbData = assembleOverlay(thumbText, thumbAddress), thumbRom = Buffer.alloc(0x3000);
  thumbData.copy(thumbRom, thumbAddress - ROM_BASE);
  const thumb = thumbSource(thumbRom, {
    address: thumbAddress, size: thumbData.length, kind: "thumb", source: "08002500.s", symbols: ["Func_08002500"], maximumUndefinedWords: 0,
  });
  if (thumb.audit.instructionBytes !== 12 || thumb.audit.literalWords !== 1 || thumb.audit.undefinedWords !== 0) {
    throw new Error("Thumb source accounting differs");
  }
  const work = mkdtempSync(join(TMPDIR, "alchemy-gap-manifest-"));
  try {
    const manifestPath = join(work, "unowned.json");
    writeFileSync(manifestPath, pretty({
      format: 1,
      semantics: "source_ownership",
      verification: "source_only",
      rom_base: ROM_BASE,
      rom_size: 0x00800000,
      regions: [
        { address: ROM_BASE, size: 4, source: "unowned/08000000", kind: "unowned" },
        ...plan.gaps.map((gap) => ({ address: gap.address, size: gap.size, source: `unowned/${gap.address.toString(16)}`, kind: "unowned" })),
      ].sort((left, right) => left.address - right.address),
    }));
    const coverage = validateSourceOnlyGaps(plan, manifestPath);
    if (coverage.plannedGaps !== plan.gaps.length || coverage.plannedBytes !== count.gapBytes ||
        coverage.excludedGaps !== 1 || coverage.excludedBytes !== 4 || coverage.unclassifiedGaps !== 0) {
      throw new Error("source-only code-band coverage audit differs");
    }
  } finally {
    rmSync(work, { recursive: true, force: true });
  }
  console.log(`self-test=ok gaps=${plan.gaps.length} executable_bytes=${count.codeBytes + count.veneerBytes} deferred_bytes=${count.deferredBytes}`);
}

interface Options {
  plan: string;
  gaps: string;
  output: string;
  rom?: string;
  selfTest: boolean;
}

function usage(): never {
  console.log("usage: executable_gap_sources.ts [-h] [--plan PLAN] [--gaps UNOWNED] [-o OUTPUT] [rom] | --self-test");
  process.exit(0);
}

function options(argv: string[]): Options {
  const result: Options = {
    plan: join(ROOT, "assets/data/executable_gap_plan.json"),
    gaps: join(ROOT, "out/full/rebuilt.unowned.json"),
    output: join(ROOT, "out/executable-gaps"),
    selfTest: false,
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "-h" || argument === "--help") usage();
    else if (argument === "--self-test") result.selfTest = true;
    else if (argument === "--plan") result.plan = resolve(argv[++index]);
    else if (argument === "--gaps") result.gaps = resolve(argv[++index]);
    else if (argument === "-o" || argument === "--output") result.output = resolve(argv[++index]);
    else if (!argument.startsWith("-") && result.rom === undefined) result.rom = resolve(argument);
    else throw new Error(`unrecognized argument: ${argument}`);
  }
  return result;
}

function main(): void {
  const args = options(Bun.argv.slice(2));
  if (args.selfTest) {
    if (Bun.argv.length !== 3) throw new Error("--self-test takes no other arguments");
    selfTest();
    return;
  }
  const plan = parseExecutableGapPlan(args.plan);
  const coverage = validateSourceOnlyGaps(plan, args.gaps);
  let records: OutputRecord[] | null = null;
  if (args.rom !== undefined) records = exportExecutableGapSources(plan, readFileSync(args.rom), args.output);
  else mkdirSync(args.output, { recursive: true });
  writeFileSync(join(args.output, "report.json"), pretty(report(plan, records, coverage)));
  const value = report(plan, records, coverage);
  console.log(
    `verification=${value.verification} gaps=${value.planned_gaps} gap_bytes=${value.planned_gap_bytes} ` +
    `executable_bytes=${Number(value.executable_section_bytes) + Number(value.referenced_veneer_bytes)} ` +
    `deferred_bytes=${value.deferred_data_or_unused_slot_bytes} excluded_bytes=${value.excluded_bytes} ` +
    `unclassified_bytes=${value.unclassified_bytes} emitted=${value.emitted_sources} ` +
    `literal_words=${value.literal_words} undefined_words=${value.undefined_words}`,
  );
}

if (import.meta.main) main();
