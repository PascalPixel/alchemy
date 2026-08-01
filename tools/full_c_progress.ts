#!/usr/bin/env bun
// Canonical Full-C Byte Share metric.
//
// The denominator is an audited union of executable intervals, not a ROM-size,
// file-size, function-count, or discovery-count proxy. The numerator is the
// union of byte-identical C-owned intervals from the normal claimed build and
// verified code-overlay placeholders. Every address space is unioned independently.
import {
  createHash,
} from "node:crypto";
import {
  existsSync,
  mkdtempSync,
  readFileSync,
  readdirSync,
  rmSync,
  writeFileSync,
} from "node:fs";
import { basename, dirname, join } from "node:path";
import { tmpdir } from "node:os";
import {
  DEFAULT_TARGET,
  decompTarget,
  parseDecompTarget,
  type DecompTargetId,
} from "./decomp_targets.ts";
import { canonicalJson } from "./canonical_json.ts";
import { targetOffset } from "./overlay_call_targets.ts";
import { publishedOffset } from "./overlay_published.ts";
import { assembleOverlay, overlayCSources } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const OVERLAY_BASE = 0x02000000;
const SUBJECT = /\[C ([0-9]{1,3}(?:,[0-9]{3})*)\/([0-9]{1,3}(?:,[0-9]{3})*) bytes\]$/;

export type IntervalKind =
  | "thumb"
  | "arm"
  | "veneer"
  | "literal_pool"
  | "executable_alignment"
  | "structural";

export interface Interval {
  start: number;
  end: number;
  kind: IntervalKind;
  evidence: string;
}

export interface NamespaceInventory {
  id: string;
  decoded_bytes?: number;
  executable_bytes: number;
  excluded_bytes?: number;
  audit: "complete" | "incomplete";
  intervals: Interval[];
  evidence: string[];
}

export interface ExecutableInventory {
  format: 1;
  metric: "full-c-byte-share";
  target: DecompTargetId;
  derivation: string;
  audit: "complete" | "incomplete";
  total_union_bytes: number;
  main: NamespaceInventory;
  overlays: NamespaceInventory[];
  caveats?: string[];
}

export interface ProgressReport {
  format: 1;
  metric: "full-c-byte-share";
  target: DecompTargetId;
  full_c_bytes: number;
  executable_bytes: number;
  remaining_bytes: number;
  percent: number;
  main: { full_c_bytes: number; executable_bytes: number };
  overlays: { full_c_bytes: number; executable_bytes: number };
  audit: "complete";
}

interface OwnedSpan {
  namespace: string;
  start: number;
  end: number;
  owner: string;
}

export function canonicalCSource(source: string): boolean {
  return ![
    /\bregister\b[^;\n]*\basm\s*\(/,
    /\b__asm__\b|\basm\s+volatile\b/,
    /\.incbin\b/,
    /\bM2C_ERROR\b/,
  ].some((pattern) => pattern.test(source));
}

function integer(value: unknown, label: string): number {
  if (!Number.isSafeInteger(value) || (value as number) < 0) {
    throw new Error(`${label} must be a non-negative safe integer`);
  }
  return value as number;
}

export function validateIntervals(intervals: readonly Interval[], label = "intervals"): void {
  for (const [index, interval] of intervals.entries()) {
    integer(interval.start, `${label}[${index}].start`);
    integer(interval.end, `${label}[${index}].end`);
    if (interval.end <= interval.start) throw new Error(`${label}[${index}] is empty or reversed`);
    if (!interval.kind || !interval.evidence) throw new Error(`${label}[${index}] lacks classification evidence`);
  }
}

export function overlapDiagnostics(intervals: readonly Interval[]): string[] {
  validateIntervals(intervals);
  const sorted = [...intervals].sort((a, b) => a.start - b.start || a.end - b.end);
  const diagnostics: string[] = [];
  let active: Interval | undefined;
  for (const interval of sorted) {
    if (active && interval.start < active.end &&
        !(interval.start === active.start && interval.end === active.end &&
          interval.kind === active.kind && interval.evidence === active.evidence)) {
      diagnostics.push(
        `0x${interval.start.toString(16)}..0x${Math.min(active.end, interval.end).toString(16)} ` +
        `overlaps ${active.kind} and ${interval.kind}`,
      );
    }
    if (!active || interval.end > active.end) active = interval;
  }
  return diagnostics;
}

export function unionIntervals(intervals: readonly Interval[]): Array<{ start: number; end: number }> {
  validateIntervals(intervals);
  const sorted = [...intervals].sort((a, b) => a.start - b.start || a.end - b.end);
  const result: Array<{ start: number; end: number }> = [];
  for (const interval of sorted) {
    const previous = result.at(-1);
    if (!previous || interval.start > previous.end) {
      result.push({ start: interval.start, end: interval.end });
    } else if (interval.end > previous.end) {
      previous.end = interval.end;
    }
  }
  return result;
}

export function intervalBytes(intervals: readonly Interval[]): number {
  return unionIntervals(intervals).reduce((sum, interval) => sum + interval.end - interval.start, 0);
}

function commas(value: number): string {
  return integer(value, "count").toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
}

function canonicalCount(value: string, label: string): number {
  const parsed = Number(value.replaceAll(",", ""));
  if (!Number.isSafeInteger(parsed) || parsed < 0 || commas(parsed) !== value) {
    throw new Error(`${label} is not canonically comma-separated`);
  }
  return parsed;
}

export function formatSubject(fullCBytes: number, executableBytes: number): string {
  if (fullCBytes > executableBytes) throw new Error("Full-C numerator exceeds executable denominator");
  return `[C ${commas(fullCBytes)}/${commas(executableBytes)} bytes]`;
}

export function parseSubject(subject: string): { fullCBytes: number; executableBytes: number } | undefined {
  const match = subject.match(SUBJECT);
  if (!match) return undefined;
  const fullCBytes = canonicalCount(match[1], "Full-C numerator");
  const executableBytes = canonicalCount(match[2], "executable denominator");
  if (fullCBytes > executableBytes) throw new Error("Full-C numerator exceeds executable denominator");
  return { fullCBytes, executableBytes };
}

export function roundHalfUpPercent(numerator: number, denominator: number): number {
  if (denominator <= 0) throw new Error("executable denominator must be positive");
  return Math.floor((numerator * 10000 + denominator / 2) / denominator) / 100;
}

function mergeClassified(intervals: readonly Interval[]): Interval[] {
  const unique = new Map<string, Interval>();
  for (const interval of intervals) {
    const key = `${interval.start}:${interval.end}:${interval.kind}`;
    if (!unique.has(key)) unique.set(key, interval);
  }
  const sorted = [...unique.values()].sort((a, b) =>
    a.start - b.start || a.end - b.end || a.kind.localeCompare(b.kind));
  const result: Interval[] = [];
  for (const interval of sorted) {
    const previous = result.at(-1);
    if (previous && previous.end === interval.start && previous.kind === interval.kind) {
      previous.end = interval.end;
      if (previous.evidence !== interval.evidence) {
        previous.evidence = `adjacent audited ${interval.kind} spans; see namespace evidence`;
      }
    } else {
      result.push({ ...interval });
    }
  }
  const diagnostics = overlapDiagnostics(result);
  if (diagnostics.length) throw new Error(diagnostics.join("\n"));
  return result;
}

function mainKind(kind: string): IntervalKind {
  if (kind === "alignment_padding") return "executable_alignment";
  if (kind === "literal_pool") return "literal_pool";
  if (kind.includes("veneer") || kind.includes("thunk")) return "veneer";
  if (kind.includes("arm") || kind === "gba_arm_entry") return "arm";
  if (kind === "compiler_output") return "thumb";
  return "structural";
}

function readJson(path: string): any {
  if (!existsSync(path)) throw new Error(`missing ${path}`);
  return JSON.parse(readFileSync(path, "utf8"));
}

function mainInventory(target: DecompTargetId): NamespaceInventory {
  const registry = decompTarget(target);
  const output = join(ROOT, registry.outputDir, "full");
  const asmPath = join(output, "asm", "manifest.json");
  const asmManifest = existsSync(asmPath) ? readJson(asmPath) : { regions: [] };
  const claimedManifest = readJson(join(output, "claimed", "manifest.json"));
  const intervals: Interval[] = [];
  for (const region of asmManifest.regions ?? []) {
    intervals.push({
      start: integer(region.address, "assembly region address"),
      end: integer(region.address, "assembly region address") + integer(region.size, "assembly region size"),
      kind: mainKind(String(region.kind ?? "structural")),
      evidence: `${region.source}:${region.kind ?? "classified-assembly"}`,
    });
  }
  for (const region of claimedManifest.regions ?? []) {
    intervals.push({
      start: integer(region.address, "C region address"),
      end: integer(region.address, "C region address") + integer(region.size, "C region size"),
      kind: "thumb",
      evidence: `${region.source}:byte-identical-claimed-C`,
    });
  }
  const diagnostics = overlapDiagnostics(intervals);
  if (diagnostics.length) throw new Error(`main executable ownership overlaps:\n${diagnostics.join("\n")}`);
  const merged = mergeClassified(intervals);
  return {
    id: "main",
    executable_bytes: intervalBytes(merged),
    audit: "complete",
    intervals: merged,
    evidence: [
      existsSync(asmPath) ? `${registry.outputDir}/full/asm/manifest.json` : "assembly classification unavailable",
      `${registry.outputDir}/full/claimed/manifest.json`,
      "byte-identical full build and tracked assembly classification",
    ],
  };
}

interface ListingRow {
  address: number;
  width: number;
  sourceLine: number;
  text: string;
}

function assemblerListing(source: string): { rows: Map<number, ListingRow>; decodedBytes: number; binary: Buffer } {
  const work = mkdtempSync(join(tmpdir(), "alchemy-full-c-overlay-"));
  try {
    const listing = join(work, "overlay.lst");
    const object = join(work, "overlay.o");
    const elf = join(work, "overlay.elf");
    const binary = join(work, "overlay.bin");
    const assembled = Bun.spawnSync([
      "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
      `-al=${listing}`, "-o", object, source,
    ], { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
    if (assembled.exitCode !== 0) throw new Error(assembled.stderr.toString().trim());
    const linked = Bun.spawnSync([
      "arm-none-eabi-ld", `-Ttext=0x${OVERLAY_BASE.toString(16)}`, "-o", elf, object,
    ], { stdout: "pipe", stderr: "pipe" });
    if (linked.exitCode !== 0) throw new Error(linked.stderr.toString().trim());
    const copied = Bun.spawnSync([
      "arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary,
    ], { stdout: "pipe", stderr: "pipe" });
    if (copied.exitCode !== 0) throw new Error(copied.stderr.toString().trim());
    const rows = new Map<number, ListingRow>();
    const pattern = /^\s*(\d+)\s+([0-9a-f]+)\s+([0-9a-f]{4,})\s+(.*)$/i;
    for (const line of readFileSync(listing, "utf8").split(/\r?\n/)) {
      const match = pattern.exec(line);
      if (!match) continue;
      const sourceLine = Number(match[1]);
      if (rows.has(sourceLine)) continue;
      rows.set(sourceLine, {
        sourceLine,
        address: OVERLAY_BASE + Number.parseInt(match[2], 16),
        width: match[3].length / 2,
        text: match[4].trim(),
      });
    }
    const placeholderImage = Buffer.from(readFileSync(binary));
    // Source lines tell us which bytes are canonical instructions, directives,
    // or verified C placeholders. Call-target evidence must instead come from
    // the image with those placeholders filled: scanning their assembler-zero
    // bytes makes a raw Thumb leaf disappear from the denominator as soon as
    // its only caller becomes exact C.
    const data = overlayCSources(source).length === 0
      ? placeholderImage
      : assembleOverlay(source);
    if (data.length !== placeholderImage.length) {
      throw new Error(`${source}: compiled code-overlay length differs from its canonical placeholder image`);
    }
    return { rows, decodedBytes: data.length, binary: data };
  } finally {
    rmSync(work, { recursive: true, force: true });
  }
}

function directiveValue(line: string): number | undefined {
  const match = /^\s*\.4byte\s+(0x[0-9a-f]+|\d+)\s*$/i.exec(line);
  return match ? Number.parseInt(match[1], 0) : undefined;
}

interface DirectiveSpan {
  start: number;
  end: number;
}

interface PublishedLeafEvidence {
  pointer: number;
  target: number;
}

const AUDITED_PUBLISHED_RAW_LEAVES = new Map<string, PublishedLeafEvidence[]>([
  ["resource_377", [{ pointer: 0x0200002c, target: 0x02000090 }]],
  ["resource_378", [{ pointer: 0x0200002c, target: 0x02000064 }]],
  ["resource_3a7", [{ pointer: 0x020021f8, target: 0x020004cc }]],
]);

/**
 * Recover leaf code written as raw halfwords only when two independent facts
 * agree: the overlay BL decoder resolves a branch to the first halfword, and
 * that bounded run of adjacent two-byte directives reaches `bx lr`. Neither a
 * BL-shaped word alone nor a return-shaped data run alone is sufficient.
 */
export function reachedDirectiveLeaves(
  binary: Uint8Array,
  callers: readonly Interval[],
  directives: readonly DirectiveSpan[],
): Interval[] {
  const starts = new Set<number>();
  for (const caller of callers) {
    if (caller.kind !== "thumb") continue;
    const from = Math.max(0, caller.start - OVERLAY_BASE);
    const to = Math.min(binary.length, caller.end - OVERLAY_BASE);
    for (let offset = from; offset + 4 <= to; offset += 2) {
      const target = targetOffset(
        binary[offset] | (binary[offset + 1] << 8),
        binary[offset + 2] | (binary[offset + 3] << 8),
      );
      if (target !== null && target >= 0 && target < binary.length) {
        starts.add(OVERLAY_BASE + target);
      }
    }
  }

  return directiveLeavesAtStarts(binary, starts, directives, "BL-reached");
}

/** Raw leaves whose Thumb entry address is installed in aligned overlay data. */
export function publishedDirectiveLeaves(
  binary: Uint8Array,
  directives: readonly DirectiveSpan[],
  publications: readonly PublishedLeafEvidence[],
): Interval[] {
  const directiveStarts = new Set(directives.map((span) => span.start));
  const starts = new Set<number>();
  for (const publication of publications) {
    const address = publication.pointer;
    const offset = address - OVERLAY_BASE;
    if (offset < 0 || offset + 4 > binary.length || offset % 4 !== 0) continue;
    if (!directiveStarts.has(address) || !directiveStarts.has(address + 2)) continue;
    const word = binary[offset] |
      (binary[offset + 1] << 8) |
      (binary[offset + 2] << 16) |
      (binary[offset + 3] << 24);
    const target = publishedOffset(word >>> 0, binary.length);
    if (target !== null && OVERLAY_BASE + target === publication.target) starts.add(publication.target);
  }
  return directiveLeavesAtStarts(binary, starts, directives, "published");
}

function directiveLeavesAtStarts(
  binary: Uint8Array,
  starts: ReadonlySet<number>,
  directives: readonly DirectiveSpan[],
  evidencePrefix: string,
): Interval[] {

  const byStart = new Map(directives.map((span) => [span.start, span]));
  const leaves: Interval[] = [];
  for (const start of starts) {
    if (!byStart.has(start)) continue;
    let cursor = start;
    const limit = Math.min(OVERLAY_BASE + binary.length, start + 128);
    const pools = new Set<number>();
    while (cursor < limit) {
      const span = byStart.get(cursor);
      if (!span || span.end !== cursor + 2) break;
      const offset = cursor - OVERLAY_BASE;
      const halfword = binary[offset] | (binary[offset + 1] << 8);
      if ((halfword & 0xf800) === 0x4800) {
        pools.add(((cursor + 4) & ~3) + ((halfword & 0xff) << 2));
      }
      cursor = span.end;
      if (halfword === 0x4770) {
        leaves.push({
          start,
          end: cursor,
          kind: "thumb",
          evidence: `${evidencePrefix} bounded raw-halfword leaf`,
        });
        for (const pool of pools) {
          if ([0, 2].every((byte) => byStart.has(pool + byte))) {
            leaves.push({
              start: pool,
              end: pool + 4,
              kind: "literal_pool",
              evidence: `literal pool referenced by ${evidencePrefix} raw-halfword leaf`,
            });
          }
        }
        break;
      }
    }
  }
  return leaves;
}

function overlayInventory(source: string, auditedCallers: readonly Interval[]): NamespaceInventory {
  const lines = readFileSync(source, "utf8").split(/\r?\n/);
  const listing = assemblerListing(source);
  const intervals: Interval[] = [];
  const directiveRows: Array<{ address: number; value: number }> = [];
  const halfwordDirectives: DirectiveSpan[] = [];
  const dataAddresses = new Set<number>();
  let inCPlaceholder = false;

  for (let index = 0; index < lines.length; index++) {
    const line = lines[index];
    const row = listing.rows.get(index + 1);
    if (/^\s*AlchemyC_[0-9a-f]{8}:\s*$/i.test(line)) {
      inCPlaceholder = true;
      continue;
    }
    if (inCPlaceholder && (/^\s*$/.test(line) || /^\s*\.L_[0-9a-z_.$]+:\s*$/i.test(line))) continue;
    const space = /^\s*\.space\s+(0x[0-9a-f]+|\d+)\s*$/i.exec(line);
    if (space && row && inCPlaceholder) {
      const size = Number.parseInt(space[1], 0);
      intervals.push({
        start: row.address,
        end: row.address + size,
        kind: "thumb",
        evidence: `${basename(source)}:${index + 1}:verified-C-placeholder`,
      });
      continue;
    }
    if (line.trim()) inCPlaceholder = false;
    if (!row) continue;

    const value = directiveValue(line);
    if (value !== undefined) {
      for (let byte = 0; byte < row.width; byte++) dataAddresses.add(row.address + byte);
      directiveRows.push({ address: row.address, value });
      for (let byte = 0; byte + 2 <= row.width; byte += 2) {
        halfwordDirectives.push({ start: row.address + byte, end: row.address + byte + 2 });
      }
      continue;
    }
    if (/^\s*\.(?:2byte|hword)\b/i.test(line) && row.width === 2) {
      halfwordDirectives.push({ start: row.address, end: row.address + 2 });
    }
    if (/^\s*\.(?:2byte|byte|hword|word|space)\b/i.test(line)) {
      for (let byte = 0; byte < row.width; byte++) dataAddresses.add(row.address + byte);
      continue;
    }
    if (/^\s*\./.test(line)) continue;
    if (/^\s*[a-z][a-z0-9.]*\s/i.test(line)) {
      intervals.push({
        start: row.address,
        end: row.address + row.width,
        kind: "thumb",
        evidence: `${basename(source)}:${index + 1}:canonical-instruction`,
      });
      const literal = /\bldr\w*\s+r\d+,\s*\[pc,\s*#(0x[0-9a-f]+|\d+)\]/i.exec(line);
      if (literal) {
        const address = ((row.address + 4) & ~3) + Number.parseInt(literal[1], 0);
        intervals.push({
          start: address,
          end: address + 4,
          kind: "literal_pool",
          evidence: `${basename(source)}:${index + 1}:pc-relative-literal`,
        });
      }
    }
  }

  const directives = new Map(directiveRows.map((row) => [row.address, row.value]));
  for (const row of directiveRows) {
    const code = directives.get(row.address + 4);
    const bank = row.value >>> 24;
    if ((row.value & 1) && [0x02, 0x08, 0x09].includes(bank) && code === 0x47204c00) {
      intervals.push({
        start: row.address,
        end: row.address + 8,
        kind: "veneer",
        evidence: `${basename(source)}:fixed-ldr-bx-veneer`,
      });
    }
  }

  const rawLeaves: Interval[] = [];
  const overlay = basename(source).replace(/_overlay\.s$/, "");
  for (const leaf of [
    ...reachedDirectiveLeaves(listing.binary, [...intervals, ...auditedCallers], halfwordDirectives),
    ...publishedDirectiveLeaves(
      listing.binary,
      halfwordDirectives,
      AUDITED_PUBLISHED_RAW_LEAVES.get(overlay) ?? [],
    ),
  ]
    .sort((left, right) => left.start - right.start || left.end - right.end)) {
    if (intervals.some((interval) => interval.start < leaf.end && leaf.start < interval.end)) continue;
    if (rawLeaves.some((accepted) => accepted.start < leaf.end && leaf.start < accepted.end)) continue;
    rawLeaves.push(leaf);
  }
  intervals.push(...rawLeaves);

  const veneers = intervals.filter((interval) => interval.kind === "veneer");
  const classified = intervals.filter((interval) =>
    interval.kind !== "literal_pool" ||
    (Array.from({ length: interval.end - interval.start }, (_, byte) => interval.start + byte)
      .every((address) => dataAddresses.has(address)) &&
      !veneers.some((veneer) => veneer.start <= interval.start && interval.end <= veneer.end)));
  let merged = mergeClassified(classified);
  const union = unionIntervals(merged);
  const alignment: Interval[] = [];
  for (let index = 1; index < union.length; index++) {
    const previous = union[index - 1];
    const current = union[index];
    if (current.start - previous.end !== 2) continue;
    const offset = previous.end - OVERLAY_BASE;
    if (offset >= 0 && listing.binary.readUInt16LE(offset) === 0) {
      alignment.push({
        start: previous.end,
        end: current.start,
        kind: "executable_alignment",
        evidence: `${basename(source)}:two-byte-zero-between-executable-spans`,
      });
    }
  }
  merged = mergeClassified([...merged, ...alignment]);
  const executableBytes = intervalBytes(merged);
  if (executableBytes > listing.decodedBytes) throw new Error(`${source}: executable bytes exceed decoded size`);
  return {
    id: basename(source).replace(/_overlay\.s$/, ""),
    decoded_bytes: listing.decodedBytes,
    executable_bytes: executableBytes,
    excluded_bytes: listing.decodedBytes - executableBytes,
    audit: "complete",
    intervals: merged,
    evidence: [
      `assets/code/${basename(source)}`,
      "assembler listing mapped to canonical instruction/directive source lines",
      "PC-relative literal targets and fixed ldr/bx veneers",
      "byte-identical decoded code-overlay round trip",
    ],
  };
}

function overlaySources(target: DecompTargetId): string[] {
  if (target !== "gs1-en") return [];
  const directory = join(ROOT, "assets", "code");
  return readdirSync(directory)
    .filter((name) => /^resource_.+_overlay\.s$/.test(name))
    .sort()
    .map((name) => join(directory, name));
}

export function deriveInventory(target: DecompTargetId): ExecutableInventory {
  const main = mainInventory(target);
  if (target === "gs2-en") {
    return {
      format: 1,
      metric: "full-c-byte-share",
      target,
      derivation: "full-c-progress-v1",
      audit: "incomplete",
      total_union_bytes: main.executable_bytes,
      main: { ...main, audit: "incomplete" },
      overlays: [],
      caveats: [
        "GS2-English is a compiler/bootstrap target and does not yet have a complete executable classification.",
      ],
    };
  }
  const overlays = overlaySources(target).map((source) => overlayInventory(source, []));
  const total = main.executable_bytes +
    overlays.reduce((sum, overlay) => sum + overlay.executable_bytes, 0);
  return {
    format: 1,
    metric: "full-c-byte-share",
    target,
    derivation: "full-c-progress-v1",
    audit: "complete",
    total_union_bytes: total,
    main,
    overlays,
  };
}

function inventoryPath(target: DecompTargetId): string {
  return join(ROOT, "metrics", `${target}-executable.json`);
}

function reportPath(target: DecompTargetId): string {
  return join(ROOT, "metrics", `${target}-progress.json`);
}

function validateInventory(inventory: ExecutableInventory): void {
  if (inventory.format !== 1 || inventory.metric !== "full-c-byte-share") {
    throw new Error("unsupported executable inventory format");
  }
  for (const namespace of [inventory.main, ...inventory.overlays]) {
    const diagnostics = overlapDiagnostics(namespace.intervals);
    if (diagnostics.length) throw new Error(`${namespace.id} overlaps:\n${diagnostics.join("\n")}`);
    const measured = intervalBytes(namespace.intervals);
    if (measured !== namespace.executable_bytes) {
      throw new Error(`${namespace.id} executable total is stale: ${namespace.executable_bytes} != ${measured}`);
    }
    if (namespace.decoded_bytes !== undefined) {
      if (namespace.excluded_bytes === undefined ||
          namespace.executable_bytes + namespace.excluded_bytes !== namespace.decoded_bytes) {
        throw new Error(`${namespace.id} decoded byte classification is incomplete`);
      }
    }
  }
  const total = inventory.main.executable_bytes +
    inventory.overlays.reduce((sum, overlay) => sum + overlay.executable_bytes, 0);
  if (total !== inventory.total_union_bytes) throw new Error("inventory total is stale");
}

function spansContained(spans: readonly OwnedSpan[], inventory: NamespaceInventory): number {
  const executable = unionIntervals(inventory.intervals);
  const ordered = [...spans].sort((a, b) => a.start - b.start || a.end - b.end);
  let previous: OwnedSpan | undefined;
  for (const span of ordered) {
    if (span.end <= span.start) throw new Error(`${span.owner}: empty C span`);
    if (previous && span.start < previous.end &&
        !(span.start === previous.start && span.end === previous.end)) {
      throw new Error(`${span.namespace}: overlapping C ownership: ${previous.owner} and ${span.owner}`);
    }
    previous = span;
    if (!executable.some((interval) => interval.start <= span.start && span.end <= interval.end)) {
      throw new Error(`${span.owner}: C span is outside audited executable intervals`);
    }
  }
  return unionIntervals(ordered.map((span) => ({
    start: span.start,
    end: span.end,
    kind: "thumb" as const,
    evidence: span.owner,
  }))).reduce((sum, span) => sum + span.end - span.start, 0);
}

function mainCSpans(target: DecompTargetId): OwnedSpan[] {
  const output = decompTarget(target).outputDir;
  const manifest = readJson(join(ROOT, output, "full", "claimed", "manifest.json"));
  if (!["exact", "source_only", "rom"].includes(manifest.verification) ||
      !Number.isSafeInteger(manifest.claimed_bytes)) {
    throw new Error("claimed build is not verified");
  }
  return (manifest.regions ?? []).flatMap((region: any) => {
    const source = String(region.source);
    const path = join(ROOT, source);
    if (!existsSync(path) || !canonicalCSource(readFileSync(path, "utf8"))) return [];
    return [{
      namespace: "main",
      start: integer(region.address, "C address"),
      end: integer(region.address, "C address") + integer(region.size, "C size"),
      owner: source,
    }];
  });
}

function overlayCSpans(source: string): OwnedSpan[] {
  const text = readFileSync(source, "utf8");
  const namespace = basename(source).replace(/_overlay\.s$/, "");
  const spans: OwnedSpan[] = [];
  let owner = "";
  let cursor = 0;
  let inPlaceholder = false;
  for (const line of text.split(/\r?\n/)) {
    const label = /^\s*AlchemyC_([0-9a-f]{8}):\s*$/i.exec(line);
    if (label) {
      owner = `AlchemyC_${label[1]}`;
      cursor = Number.parseInt(label[1], 16);
      inPlaceholder = true;
      continue;
    }
    if (inPlaceholder && (/^\s*$/.test(line) || /^\s*\.L_[0-9a-z_.$]+:\s*$/i.test(line))) continue;
    const space = /^\s*\.space\s+(0x[0-9a-f]+|\d+)\s*$/i.exec(line);
    if (inPlaceholder && space) {
      const size = Number.parseInt(space[1], 0);
      const cName = basename(source).replace(/overlay\.s$/, `c_${owner.slice(-8)}.c`);
      const cPath = join(dirname(source), cName);
      if (!existsSync(cPath) || !canonicalCSource(readFileSync(cPath, "utf8"))) {
        cursor += size;
        continue;
      }
      spans.push({
        namespace,
        start: cursor,
        end: cursor + size,
        owner: `${basename(source)}:${owner}`,
      });
      cursor += size;
      continue;
    }
    if (line.trim()) inPlaceholder = false;
  }
  return spans;
}

export function calculateProgress(
  inventory: ExecutableInventory,
  mainSpans: readonly OwnedSpan[],
  overlaySpans: ReadonlyMap<string, readonly OwnedSpan[]>,
): ProgressReport {
  validateInventory(inventory);
  if (inventory.audit !== "complete" || inventory.main.audit !== "complete" ||
      inventory.overlays.some((overlay) => overlay.audit !== "complete")) {
    throw new Error(`Full-C Byte Share withheld: ${inventory.target} executable audit is incomplete`);
  }
  const mainBytes = spansContained(mainSpans, inventory.main);
  let overlayBytes = 0;
  const expected = new Set(inventory.overlays.map((overlay) => overlay.id));
  for (const overlay of inventory.overlays) {
    overlayBytes += spansContained(overlaySpans.get(overlay.id) ?? [], overlay);
  }
  for (const id of overlaySpans.keys()) {
    if (!expected.has(id)) throw new Error(`C spans belong to un-inventoried code overlay ${id}`);
  }
  const fullCBytes = mainBytes + overlayBytes;
  const executableBytes = inventory.total_union_bytes;
  const remainingBytes = executableBytes - fullCBytes;
  if (fullCBytes < 0 || fullCBytes > executableBytes ||
      fullCBytes + remainingBytes !== executableBytes) {
    throw new Error("Full-C arithmetic invariant failed");
  }
  return {
    format: 1,
    metric: "full-c-byte-share",
    target: inventory.target,
    full_c_bytes: fullCBytes,
    executable_bytes: executableBytes,
    remaining_bytes: remainingBytes,
    percent: roundHalfUpPercent(fullCBytes, executableBytes),
    main: { full_c_bytes: mainBytes, executable_bytes: inventory.main.executable_bytes },
    overlays: {
      full_c_bytes: overlayBytes,
      executable_bytes: inventory.overlays.reduce((sum, item) => sum + item.executable_bytes, 0),
    },
    audit: "complete",
  };
}

export function currentProgress(target: DecompTargetId): ProgressReport {
  const inventory = readJson(inventoryPath(target)) as ExecutableInventory;
  if (inventory.target !== target) throw new Error("inventory target mismatch");
  const overlayMap = new Map<string, OwnedSpan[]>();
  for (const source of overlaySources(target)) {
    const id = basename(source).replace(/_overlay\.s$/, "");
    overlayMap.set(id, overlayCSpans(source));
  }
  return calculateProgress(inventory, mainCSpans(target), overlayMap);
}

function stableInventoryShape(inventory: ExecutableInventory): string {
  return createHash("sha256").update(canonicalJson(inventory)).digest("hex");
}

function formatReport(report: ProgressReport): string {
  return [
    `Full-C Byte Share: ${commas(report.full_c_bytes)} / ${commas(report.executable_bytes)} executable bytes (${report.percent.toFixed(2)}%)`,
    `Main image: ${commas(report.main.full_c_bytes)} / ${commas(report.main.executable_bytes)} executable bytes`,
    `Code overlays: ${commas(report.overlays.full_c_bytes)} / ${commas(report.overlays.executable_bytes)} executable bytes`,
  ].join("\n");
}

function equalUnion(left: NamespaceInventory, right: NamespaceInventory): boolean {
  return canonicalJson(unionIntervals(left.intervals)) === canonicalJson(unionIntervals(right.intervals)) &&
    left.executable_bytes === right.executable_bytes &&
    left.decoded_bytes === right.decoded_bytes &&
    left.excluded_bytes === right.excluded_bytes;
}

function checkCurrentInventory(target: DecompTargetId, tracked: ExecutableInventory): void {
  const derived = deriveInventory(target);
  validateInventory(derived);
  if (tracked.audit !== derived.audit || !equalUnion(tracked.main, derived.main) ||
      tracked.overlays.length !== derived.overlays.length) {
    throw new Error(`tracked ${target} executable inventory is stale`);
  }
  for (let index = 0; index < tracked.overlays.length; index++) {
    if (tracked.overlays[index].id !== derived.overlays[index].id ||
        !equalUnion(tracked.overlays[index], derived.overlays[index])) {
      throw new Error(`tracked ${target} code-overlay inventory is stale at ${tracked.overlays[index].id}`);
    }
  }
}

function expectReject(action: () => unknown, label: string): void {
  let rejected = false;
  try {
    action();
  } catch {
    rejected = true;
  }
  if (!rejected) throw new Error(`self-test failed to reject ${label}`);
}

function selfTest(): void {
  const item = (start: number, end: number, kind: IntervalKind = "thumb"): Interval => ({
    start, end, kind, evidence: "test",
  });
  if (intervalBytes([]) !== 0) throw new Error("empty interval union failed");
  if (intervalBytes([item(0, 2), item(2, 4)]) !== 4) throw new Error("adjacent interval union failed");
  if (intervalBytes([item(0, 8), item(2, 4)]) !== 8) throw new Error("nested interval union failed");
  if (intervalBytes([item(0, 4), item(2, 6)]) !== 6) throw new Error("partial interval union failed");
  if (overlapDiagnostics([item(0, 4), item(2, 6)]).length !== 1) throw new Error("overlap diagnostics failed");
  if (overlapDiagnostics([item(0, 4), item(0, 4)]).length !== 0) throw new Error("identical alias failed");
  if (formatSubject(123456, 1234567) !== "[C 123,456/1,234,567 bytes]") throw new Error("subject format failed");
  const parsed = parseSubject("decomp: x [C 123,456/1,234,567 bytes]");
  if (!parsed || parsed.fullCBytes !== 123456 || parsed.executableBytes !== 1234567) {
    throw new Error("subject parse failed");
  }
  for (const invalid of [
    "x [C 123456/1,234,567 bytes]",
    "x [C 123,456 /1,234,567 bytes]",
    "x [C 123,456/1,234,567]",
    "x [C 2/1 bytes]",
    "x [123 of 456]",
  ]) expectReject(() => {
    if (!parseSubject(invalid)) throw new Error("rejected");
  }, invalid);
  if (roundHalfUpPercent(1, 8) !== 12.5) throw new Error("round-half-up failed");

  const leafImage = new Uint8Array(0x20);
  leafImage.set([0x00, 0xf0, 0x07, 0xf8], 0); // stored displacement 0xe -> target +0x10
  leafImage.set([0x01, 0x20, 0x70, 0x47], 0x10); // movs r0,#1; bx lr
  const rawLeaf = [{ start: OVERLAY_BASE + 0x10, end: OVERLAY_BASE + 0x12 },
    { start: OVERLAY_BASE + 0x12, end: OVERLAY_BASE + 0x14 }];
  const caller = [item(OVERLAY_BASE, OVERLAY_BASE + 4)];
  const reached = reachedDirectiveLeaves(leafImage, caller, rawLeaf);
  if (reached.length !== 1 || reached[0].start !== OVERLAY_BASE + 0x10 ||
      reached[0].end !== OVERLAY_BASE + 0x14) {
    throw new Error("BL-reached directive leaf was not classified as Thumb");
  }
  const dataOnly = leafImage.slice();
  dataOnly.fill(0, 0, 4);
  if (reachedDirectiveLeaves(dataOnly, caller, rawLeaf).length !== 0) {
    throw new Error("a return-shaped raw-halfword data run without a BL must remain data");
  }
  const noReturn = leafImage.slice();
  noReturn[0x12] = 0;
  noReturn[0x13] = 0;
  if (reachedDirectiveLeaves(noReturn, caller, rawLeaf).length !== 0) {
    throw new Error("a reached directive run without bx lr must remain data");
  }
  if (reachedDirectiveLeaves(leafImage, [], rawLeaf).length !== 0) {
    throw new Error("a BL-shaped pair in unaudited data must not reach a raw leaf");
  }
  const poolBlShape = leafImage.slice();
  poolBlShape.fill(0, 0, 4);
  poolBlShape.set([0x00, 0xf0, 0x07, 0xf8], 4);
  if (reachedDirectiveLeaves(poolBlShape, caller, rawLeaf).length !== 0) {
    throw new Error("a BL-shaped pair in an adjacent literal pool must not reach a raw leaf");
  }
  const getterImage = leafImage.slice();
  getterImage.set([0x00, 0x48, 0x70, 0x47, 0x88, 0x98, 0x00, 0x02], 0x10);
  const rawGetter = Array.from({ length: 4 }, (_, index) => ({
    start: OVERLAY_BASE + 0x10 + index * 2,
    end: OVERLAY_BASE + 0x12 + index * 2,
  }));
  const getter = reachedDirectiveLeaves(getterImage, caller, rawGetter);
  if (getter.length !== 2 || getter[0].kind !== "thumb" || getter[0].end !== OVERLAY_BASE + 0x14 ||
      getter[1].kind !== "literal_pool" || getter[1].start !== OVERLAY_BASE + 0x14 ||
      getter[1].end !== OVERLAY_BASE + 0x18) {
    throw new Error("a reached raw getter must carry its referenced literal pool");
  }
  const publishedImage = leafImage.slice();
  publishedImage.fill(0, 0, 4);
  publishedImage.set([0x11, 0x80, 0x00, 0x02], 0); // Thumb pointer to overlay offset 0x10
  const publishedDirectives = [
    { start: OVERLAY_BASE, end: OVERLAY_BASE + 2 },
    { start: OVERLAY_BASE + 2, end: OVERLAY_BASE + 4 },
    ...rawLeaf,
  ];
  const publicationEvidence = [{ pointer: OVERLAY_BASE, target: OVERLAY_BASE + 0x10 }];
  const published = publishedDirectiveLeaves(publishedImage, publishedDirectives, publicationEvidence);
  if (published.length !== 1 || published[0].start !== OVERLAY_BASE + 0x10 ||
      published[0].end !== OVERLAY_BASE + 0x14) {
    throw new Error("a published raw leaf was not classified as Thumb");
  }
  const evenPublication = publishedImage.slice();
  evenPublication[0] &= 0xfe;
  if (publishedDirectiveLeaves(evenPublication, publishedDirectives, publicationEvidence).length !== 0) {
    throw new Error("an even data pointer must not publish a raw leaf");
  }
  const outOfRangePublication = publishedImage.slice();
  outOfRangePublication.set([0x01, 0x81, 0x00, 0x02], 0);
  if (publishedDirectiveLeaves(outOfRangePublication, publishedDirectives, publicationEvidence).length !== 0) {
    throw new Error("an out-of-range data pointer must not publish a raw leaf");
  }
  const publishedWithoutReturn = publishedImage.slice();
  publishedWithoutReturn.fill(0, 0x10, 0x14);
  if (publishedDirectiveLeaves(publishedWithoutReturn, publishedDirectives, publicationEvidence).length !== 0) {
    throw new Error("a published directive run without bx lr must remain data");
  }
  if (publishedDirectiveLeaves(publishedImage, rawLeaf, publicationEvidence).length !== 0) {
    throw new Error("a pointer-shaped instruction word must not publish a raw leaf");
  }
  const stalePublication = [{ pointer: OVERLAY_BASE, target: OVERLAY_BASE + 0x14 }];
  if (publishedDirectiveLeaves(publishedImage, publishedDirectives, stalePublication).length !== 0) {
    throw new Error("a changed publication target must require fresh audit evidence");
  }

  const inventory: ExecutableInventory = {
    format: 1,
    metric: "full-c-byte-share",
    target: "gs1-en",
    derivation: "test",
    audit: "complete",
    total_union_bytes: 18,
    main: {
      id: "main", executable_bytes: 10, audit: "complete",
      intervals: [item(0, 4, "arm"), item(4, 8), item(8, 9, "literal_pool"), item(9, 10, "executable_alignment")],
      evidence: ["test"],
    },
    overlays: [{
      id: "overlay-a", decoded_bytes: 12, executable_bytes: 8, excluded_bytes: 4,
      audit: "complete",
      intervals: [item(0x2000000, 0x2000008)],
      evidence: ["test"],
    }],
  };
  const report = calculateProgress(
    inventory,
    [{ namespace: "main", start: 0, end: 4, owner: "main-c" }],
    new Map([["overlay-a", [{ namespace: "overlay-a", start: 0x2000000, end: 0x2000002, owner: "overlay-c" }]]]),
  );
  if (report.full_c_bytes !== 6 || report.executable_bytes !== 18) throw new Error("progress calculation failed");
  expectReject(() => calculateProgress(inventory, [
    { namespace: "main", start: 0, end: 4, owner: "c" },
    { namespace: "main", start: 2, end: 6, owner: "asm-overlap" },
  ], new Map()), "C/assembly ownership overlap");
  const incomplete = structuredClone(inventory);
  incomplete.audit = "incomplete";
  expectReject(() => calculateProgress(incomplete, [], new Map()), "incomplete audit");
  const targetIsolation = structuredClone(inventory);
  targetIsolation.target = "gs2-en";
  if (targetIsolation.target === inventory.target) throw new Error("target isolation failed");
  console.log("self-test=ok metric=full-c-byte-share");
}

interface Options {
  target: DecompTargetId;
  check: boolean;
  subject: boolean;
  json: boolean;
  writeInventory: boolean;
  writeReport: boolean;
  selfTest: boolean;
}

function optionsOf(argv: string[]): Options {
  const options: Options = {
    target: DEFAULT_TARGET,
    check: false,
    subject: false,
    json: false,
    writeInventory: false,
    writeReport: false,
    selfTest: false,
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--target") options.target = parseDecompTarget(argv[++index]);
    else if (argument === "--check") options.check = true;
    else if (argument === "--subject") options.subject = true;
    else if (argument === "--json") options.json = true;
    else if (argument === "--write-inventory") options.writeInventory = true;
    else if (argument === "--write-report") options.writeReport = true;
    else if (argument === "--self-test") options.selfTest = true;
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: full_c_progress.ts [--target gs1-en|gs2-en] [--check|--subject|--json|--write-inventory|--write-report|--self-test]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  return options;
}

async function main(argv: string[]): Promise<void> {
  const options = optionsOf(argv);
  if (options.selfTest) return selfTest();
  if (options.writeInventory) {
    const inventory = deriveInventory(options.target);
    const path = inventoryPath(options.target);
    writeFileSync(path, canonicalJson(inventory));
    console.log(
      `inventory=${path.slice(ROOT.length + 1)} audit=${inventory.audit} ` +
      `executable_bytes=${inventory.total_union_bytes} sha256=${stableInventoryShape(inventory)}`,
    );
    return;
  }
  if (options.writeReport) {
    const report = currentProgress(options.target);
    const path = reportPath(options.target);
    writeFileSync(path, canonicalJson(report));
    console.log(`report=${path.slice(ROOT.length + 1)} ${formatSubject(report.full_c_bytes, report.executable_bytes)}`);
    return;
  }
  const inventory = readJson(inventoryPath(options.target)) as ExecutableInventory;
  validateInventory(inventory);
  if (options.check) checkCurrentInventory(options.target, inventory);
  const report = currentProgress(options.target);
  if (options.check) {
    const cached = readJson(reportPath(options.target));
    if (canonicalJson(cached) !== canonicalJson(report)) {
      throw new Error(`tracked ${options.target} Full-C report is stale`);
    }
  }
  if (options.subject) console.log(formatSubject(report.full_c_bytes, report.executable_bytes));
  else if (options.json) console.log(canonicalJson(report));
  else console.log(formatReport(report));
}

if (import.meta.main) await main(Bun.argv.slice(2));
