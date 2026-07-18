#!/usr/bin/env bun
import type { ComparisonReport } from "./compare_roms.ts";
import { compareRegions, type RegionMatch } from "./compare_regions.ts";

interface Region {
  address: number | string;
  size: number;
  source: string;
  kind?: string;
}

interface Manifest {
  regions: Region[];
}

interface Evidence {
  candidate: string;
  candidate_address: string;
  mode: "exact" | "thumb_relocated";
  ratio: number;
  covered_bytes: number;
  supporting_spans: number;
  relocation_deltas: number[];
  maximum_occurrences: number;
  confidence: "high" | "medium" | "ambiguous";
  cross_title: boolean;
}

export interface WorkItem {
  priority: "A" | "B" | "C" | "D" | "E";
  score: number;
  source: string;
  kind?: string;
  reference_address: string;
  size: number;
  area: "code" | "data";
  entropy: number;
  dominant_byte_ratio: number;
  best_ratio: number;
  exact_supporters: number;
  normalized_supporters: number;
  cross_title_supporters: number;
  ambiguous: boolean;
  evidence: Evidence[];
}

export interface WorkQueue {
  format: 1;
  reference: string;
  code_end: string;
  near_ratio: number;
  report_count: number;
  region_count: number;
  priorities: Record<"A" | "B" | "C" | "D" | "E", number>;
  items: WorkItem[];
}

function numeric(value: number | string, label: string): number {
  const result = typeof value === "number" ? value : Number(value);
  if (!Number.isSafeInteger(result) || result < 0 || result > 0xffffffff) {
    throw new Error(`${label}: invalid address`);
  }
  return result;
}

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function leaf(path: string): string {
  return path.replaceAll("\\", "/").split("/").at(-1) ?? path;
}

function entropy(source: Uint8Array): { entropy: number; dominant: number } {
  if (source.length === 0) return { entropy: 0, dominant: 0 };
  const counts = new Uint32Array(256);
  for (const value of source) counts[value]++;
  let result = 0;
  let maximum = 0;
  for (const count of counts) {
    if (count === 0) continue;
    const ratio = count / source.length;
    result -= ratio * Math.log2(ratio);
    maximum = Math.max(maximum, count);
  }
  return { entropy: result, dominant: maximum / source.length };
}

function evidence(
  report: ComparisonReport,
  match: RegionMatch,
  mode: Evidence["mode"],
): Evidence {
  return {
    candidate: report.candidate,
    candidate_address: match.candidate_address,
    mode,
    ratio: match.ratio,
    covered_bytes: match.covered_bytes,
    supporting_spans: match.supporting_spans ?? 1,
    relocation_deltas: match.relocation_deltas ?? [],
    maximum_occurrences: match.maximum_occurrences ?? 1,
    confidence: match.confidence ?? "high",
    cross_title: leaf(report.candidate).toLowerCase().startsWith("gs2-"),
  };
}

export function buildWorkQueue(
  referenceName: string,
  reference: Uint8Array,
  manifest: Manifest,
  reports: ComparisonReport[],
  codeEnd = 0x08100000,
  nearRatio = 0.99,
  base = 0x08000000,
): WorkQueue {
  if (!Number.isFinite(nearRatio) || nearRatio <= 0 || nearRatio > 1) {
    throw new Error("near ratio must be greater than zero and at most one");
  }
  if (!Number.isSafeInteger(codeEnd) || codeEnd <= base || codeEnd > base + reference.length) {
    throw new Error("code end is outside the reference ROM");
  }
  const bySource = new Map<string, Evidence[]>();
  for (const report of reports) {
    const exact = compareRegions(report, manifest, 0, true);
    for (const match of exact.regions) {
      const item = evidence(report, match, "exact");
      bySource.set(match.source, [...(bySource.get(match.source) ?? []), item]);
    }
    if (report.thumb_relocated === undefined) continue;
    const normalized = compareRegions(report, manifest, 0, false);
    for (const match of normalized.regions) {
      const item = evidence(report, match, "thumb_relocated");
      bySource.set(match.source, [...(bySource.get(match.source) ?? []), item]);
    }
  }
  const items: WorkItem[] = [];
  for (const region of manifest.regions) {
    const start = numeric(region.address, region.source);
    if (!Number.isSafeInteger(region.size) || region.size <= 0 ||
        start < base || start + region.size > base + reference.length) {
      throw new Error(`${region.source}: invalid range`);
    }
    const allEvidence = bySource.get(region.source) ?? [];
    if (allEvidence.length === 0) continue;
    allEvidence.sort((left, right) =>
      right.ratio - left.ratio ||
      Number(right.mode === "exact") - Number(left.mode === "exact") ||
      left.candidate.localeCompare(right.candidate)
    );
    const exact = allEvidence.filter((item) => item.mode === "exact" && item.ratio >= nearRatio);
    const normalized = allEvidence.filter((item) => item.mode === "thumb_relocated" && item.ratio >= nearRatio);
    const best = allEvidence[0];
    const ambiguous = best.confidence === "ambiguous" || best.maximum_occurrences > 1;
    const metrics = entropy(reference.subarray(start - base, start - base + region.size));
    const area = start < codeEnd ? "code" : "data";
    let priority: WorkItem["priority"];
    if (area === "code" && exact.length > 0 && !ambiguous) priority = "A";
    else if (area === "code" && normalized.length > 0 && !ambiguous) priority = "B";
    else if (area === "data" && exact.length > 0 && metrics.entropy >= 4 &&
        metrics.dominant < 0.5 && region.size >= 64 && !ambiguous) priority = "C";
    else if (best.ratio >= 0.5 && !ambiguous) priority = "D";
    else priority = "E";
    const supporters = new Set(allEvidence.filter((item) => item.ratio >= nearRatio)
      .map((item) => item.candidate)).size;
    const crossTitle = new Set(allEvidence.filter((item) => item.ratio >= nearRatio && item.cross_title)
      .map((item) => item.candidate)).size;
    const score = best.ratio * 100 + Math.log2(region.size + 1) * 2 + supporters * 3 + crossTitle * 6 +
      (exact.length > 0 ? 8 : 0) - (ambiguous ? 20 : 0);
    items.push({
      priority,
      score: Number(score.toFixed(3)),
      source: region.source,
      ...(region.kind === undefined ? {} : { kind: region.kind }),
      reference_address: hexadecimal(start),
      size: region.size,
      area,
      entropy: Number(metrics.entropy.toFixed(4)),
      dominant_byte_ratio: Number(metrics.dominant.toFixed(6)),
      best_ratio: best.ratio,
      exact_supporters: new Set(exact.map((item) => item.candidate)).size,
      normalized_supporters: new Set(normalized.map((item) => item.candidate)).size,
      cross_title_supporters: crossTitle,
      ambiguous,
      evidence: allEvidence,
    });
  }
  const order = { A: 0, B: 1, C: 2, D: 3, E: 4 } as const;
  items.sort((left, right) => order[left.priority] - order[right.priority] ||
    right.score - left.score || right.size - left.size ||
    left.reference_address.localeCompare(right.reference_address));
  const priorities = { A: 0, B: 0, C: 0, D: 0, E: 0 };
  for (const item of items) priorities[item.priority]++;
  return {
    format: 1,
    reference: leaf(referenceName),
    code_end: hexadecimal(codeEnd),
    near_ratio: nearRatio,
    report_count: reports.length,
    region_count: manifest.regions.length,
    priorities,
    items,
  };
}

function privateOutput(path: string): boolean {
  const normalized = path.replaceAll("\\", "/");
  return normalized.startsWith("out/") || normalized.includes("/out/") ||
    normalized.startsWith("/tmp/") || normalized.startsWith("/private/tmp/");
}

function option(args: string[], name: string, fallback?: string): string {
  const index = args.indexOf(name);
  if (index < 0) {
    if (fallback !== undefined) return fallback;
    throw new Error(`${name} is required`);
  }
  const value = args[index + 1];
  if (value === undefined) throw new Error(`${name} requires a value`);
  return value;
}

function positional(args: string[]): string[] {
  const values = new Set(["-o", "--code-end", "--near-ratio"]);
  const result: string[] = [];
  for (let index = 0; index < args.length; index++) {
    if (values.has(args[index])) index++;
    else if (!args[index].startsWith("-")) result.push(args[index]);
  }
  return result;
}

export function selfTest(): void {
  const reference = new Uint8Array(256);
  for (let index = 0; index < reference.length; index++) reference[index] = index;
  const manifest: Manifest = {
    regions: [{ address: 0x08000040, size: 64, source: "rom-fallback/08000040", kind: "rom_fallback" }],
  };
  const report = {
    format: 1,
    reference: "gs1-en.gba",
    candidate: "gs2-en.gba",
    reference_size: 256,
    candidate_size: 256,
    same_offset: { compared_bytes: 256, matching_bytes: 0, ratio: 0, run_count: 0, longest_runs: [] },
    relocated: {
      block_size: 64,
      step: 4,
      span_count: 1,
      matched_bytes: 64,
      longest_spans: [{
        reference_address: "0x08000040",
        candidate_address: "0x08000080",
        size: 64,
        maximum_occurrences: 1,
      }],
    },
  } satisfies ComparisonReport;
  const queue = buildWorkQueue("/private/gs1-en.gba", reference, manifest, [report], 0x080000c0);
  if (queue.items.length !== 1 || queue.items[0].priority !== "A" ||
      queue.items[0].cross_title_supporters !== 1 || queue.reference !== "gs1-en.gba") {
    throw new Error("decompilation queue self-test failed");
  }
  console.log("self-test=ok");
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    selfTest();
    return;
  }
  const inputs = positional(args);
  if (inputs.length < 3) {
    console.log(
      "usage: scan_decomp.ts REFERENCE MANIFEST COMPARISON... -o OUT " +
      "[--code-end ADDRESS] [--near-ratio RATIO]",
    );
    return;
  }
  const output = option(args, "-o");
  if (!privateOutput(output)) throw new Error("decompilation queues must stay under out/ or a temporary directory");
  const reference = new Uint8Array(await Bun.file(inputs[0]).arrayBuffer());
  const manifest = await Bun.file(inputs[1]).json() as Manifest;
  const reports = await Promise.all(inputs.slice(2).map(async (path) =>
    await Bun.file(path).json() as ComparisonReport
  ));
  const queue = buildWorkQueue(
    inputs[0],
    reference,
    manifest,
    reports,
    Number(option(args, "--code-end", "0x08100000")),
    Number(option(args, "--near-ratio", "0.99")),
  );
  await Bun.write(output, JSON.stringify(queue, null, 2) + "\n");
  console.log(`items=${queue.items.length} A=${queue.priorities.A} B=${queue.priorities.B} ` +
    `C=${queue.priorities.C} D=${queue.priorities.D} E=${queue.priorities.E}`);
}

if (import.meta.main) await main(Bun.argv.slice(2));
