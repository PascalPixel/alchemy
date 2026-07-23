#!/usr/bin/env bun
// Tool role: both; imported by tools/scan_data_v2.ts, tools/scan_decomp.ts; invoked by package.json.
import type { ComparisonReport, MatchSpan } from "./compare_roms.ts";
import { canonicalJson } from "./canonical_json.ts";

interface BuiltRegion {
  address: number | string;
  size: number;
  source: string;
  kind?: string;
}

interface BuildManifest {
  regions: BuiltRegion[];
}

export interface RegionMatch {
  source: string;
  kind?: string;
  reference_address: string;
  candidate_address: string;
  size: number;
  covered_bytes: number;
  ratio: number;
  supporting_spans?: number;
  relocation_deltas?: number[];
  maximum_occurrences?: number;
  confidence?: "high" | "medium" | "ambiguous";
}

export interface RegionComparison {
  format: 1;
  reference: string;
  candidate: string;
  mode: "exact" | "thumb_relocated";
  minimum_ratio: number;
  region_count: number;
  matched_region_count: number;
  fully_covered_count: number;
  covered_bytes: number;
  regions: RegionMatch[];
}

interface NumericSpan {
  reference: number;
  candidate: number;
  end: number;
  maximumOccurrences: number;
}

function address(value: number | string, label: string): number {
  const result = typeof value === "number" ? value : Number(value);
  if (!Number.isSafeInteger(result) || result < 0 || result > 0xffffffff) {
    throw new Error(`${label}: invalid address`);
  }
  return result;
}

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function spans(report: ComparisonReport, exact: boolean): { mode: "exact" | "thumb_relocated"; spans: MatchSpan[] } {
  if (!exact && report.thumb_relocated !== undefined) {
    if (report.thumb_relocated.longest_spans.length !== report.thumb_relocated.span_count) {
      throw new Error("comparison report omits Thumb spans; regenerate it with a sufficient --limit");
    }
    return { mode: "thumb_relocated", spans: report.thumb_relocated.longest_spans };
  }
  if (report.relocated.longest_spans.length !== report.relocated.span_count) {
    throw new Error("comparison report omits exact spans; regenerate it with a sufficient --limit");
  }
  return { mode: "exact", spans: report.relocated.longest_spans };
}

function numericSpans(source: MatchSpan[]): NumericSpan[] {
  return source.map((span, index) => {
    const reference = address(span.reference_address, `span ${index}`);
    const candidate = address(span.candidate_address, `span ${index}`);
    if (!Number.isSafeInteger(span.size) || span.size <= 0) {
      throw new Error(`span ${index}: invalid size`);
    }
    return {
      reference,
      candidate,
      end: reference + span.size,
      maximumOccurrences: span.maximum_occurrences ?? 1,
    };
  }).sort((left, right) => left.reference - right.reference || left.end - right.end);
}

function regionCoverage(
  start: number,
  size: number,
  source: NumericSpan[],
): {
  covered: number;
  candidate: number;
  supportingSpans: number;
  deltas: number[];
  maximumOccurrences: number;
} {
  const end = start + size;
  const intervals: Array<[number, number]> = [];
  let bestSize = 0;
  let candidate = 0;
  let maximumOccurrences = 1;
  const deltas = new Set<number>();
  let supportingSpans = 0;
  for (const span of source) {
    if (span.end <= start) continue;
    if (span.reference >= end) break;
    const left = Math.max(start, span.reference);
    const right = Math.min(end, span.end);
    if (right <= left) continue;
    intervals.push([left, right]);
    supportingSpans++;
    maximumOccurrences = Math.max(maximumOccurrences, span.maximumOccurrences);
    deltas.add(span.candidate - span.reference);
    if (right - left > bestSize) {
      bestSize = right - left;
      candidate = span.candidate + start - span.reference;
    }
  }
  intervals.sort((left, right) => left[0] - right[0] || left[1] - right[1]);
  let covered = 0;
  let currentEnd = -1;
  for (const [left, right] of intervals) {
    if (left > currentEnd) {
      covered += right - left;
      currentEnd = right;
    } else if (right > currentEnd) {
      covered += right - currentEnd;
      currentEnd = right;
    }
  }
  return {
    covered,
    candidate,
    supportingSpans,
    deltas: [...deltas].sort((left, right) => left - right),
    maximumOccurrences,
  };
}

export function compareRegions(
  report: ComparisonReport,
  manifest: BuildManifest,
  minimumRatio = 0,
  exact = false,
  kind?: string,
): RegionComparison {
  if (!Number.isFinite(minimumRatio) || minimumRatio < 0 || minimumRatio > 1) {
    throw new Error("minimum ratio must be between zero and one");
  }
  if (!Array.isArray(manifest.regions)) throw new Error("build manifest has no regions");
  const selected = spans(report, exact);
  const matches = numericSpans(selected.spans);
  const regions: RegionMatch[] = [];
  let regionCount = 0;
  for (const region of manifest.regions) {
    if (kind !== undefined && region.kind !== kind) continue;
    const start = address(region.address, region.source);
    if (!Number.isSafeInteger(region.size) || region.size <= 0) {
      throw new Error(`${region.source}: invalid size`);
    }
    regionCount++;
    const overlap = regionCoverage(start, region.size, matches);
    const ratio = overlap.covered / region.size;
    if (overlap.covered === 0 || ratio < minimumRatio) continue;
    regions.push({
      source: region.source,
      ...(region.kind === undefined ? {} : { kind: region.kind }),
      reference_address: hexadecimal(start),
      candidate_address: hexadecimal(overlap.candidate),
      size: region.size,
      covered_bytes: overlap.covered,
      ratio,
      supporting_spans: overlap.supportingSpans,
      relocation_deltas: overlap.deltas,
      maximum_occurrences: overlap.maximumOccurrences,
      confidence: overlap.deltas.length === 1 && overlap.maximumOccurrences === 1
        ? "high"
        : overlap.deltas.length === 1 ? "medium" : "ambiguous",
    });
  }
  regions.sort((left, right) => right.ratio - left.ratio || right.size - left.size ||
    left.reference_address.localeCompare(right.reference_address));
  return {
    format: 1,
    reference: report.reference,
    candidate: report.candidate,
    mode: selected.mode,
    minimum_ratio: minimumRatio,
    region_count: regionCount,
    matched_region_count: regions.length,
    fully_covered_count: regions.filter((region) => region.covered_bytes === region.size).length,
    covered_bytes: regions.reduce((sum, region) => sum + region.covered_bytes, 0),
    regions,
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
  const result: string[] = [];
  for (let index = 0; index < args.length; index++) {
    const argument = args[index];
    if (["-o", "--minimum-ratio", "--kind"].includes(argument)) {
      index++;
    } else if (!argument.startsWith("-")) {
      result.push(argument);
    }
  }
  return result;
}

export function selfTest(): void {
  const report = {
    format: 1,
    reference: "reference.gba",
    candidate: "candidate.gba",
    reference_size: 128,
    candidate_size: 128,
    same_offset: { compared_bytes: 128, matching_bytes: 0, ratio: 0, run_count: 0, longest_runs: [] },
    relocated: {
      block_size: 16,
      step: 4,
      span_count: 1,
      matched_bytes: 32,
      longest_spans: [{ reference_address: "0x08000010", candidate_address: "0x08001010", size: 32 }],
    },
    thumb_relocated: {
      block_size: 16,
      step: 4,
      span_count: 2,
      matched_bytes: 48,
      exact_matched_bytes: 32,
      improvement_bytes: 16,
      normalization: {
        reference_long_calls: 0,
        reference_rom_pointer_literals: 0,
        candidate_long_calls: 0,
        candidate_rom_pointer_literals: 0,
      },
      longest_spans: [
        { reference_address: "0x08000008", candidate_address: "0x08001008", size: 24 },
        { reference_address: "0x08000018", candidate_address: "0x08001018", size: 32 },
      ],
    },
  } satisfies ComparisonReport;
  const manifest: BuildManifest = {
    regions: [
      { address: "0x08000000", size: 32, source: "asm/one.s", kind: "compiler_output" },
      { address: "0x08000020", size: 16, source: "asm/two.s", kind: "compiler_output" },
      { address: "0x08000040", size: 16, source: "asm/data.s", kind: "probable_data" },
    ],
  };
  const result = compareRegions(report, manifest, 0.75, false, "compiler_output");
  if (result.mode !== "thumb_relocated" || result.region_count !== 2 ||
      result.matched_region_count !== 2 || result.fully_covered_count !== 1 ||
      result.covered_bytes !== 40 || result.regions[0].source !== "asm/two.s" ||
      result.regions[0].candidate_address !== "0x08001020" ||
      result.regions[1].covered_bytes !== 24 ||
      result.regions[1].candidate_address !== "0x08001000") {
    throw new Error("region comparison self-test failed");
  }
  console.log("self-test=ok");
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    selfTest();
    return;
  }
  const inputs = positional(args);
  if (inputs.length !== 2) {
    console.log("usage: compare_regions.ts COMPARISON MANIFEST -o OUT [--minimum-ratio RATIO] [--kind KIND] [--exact]");
    return;
  }
  const output = option(args, "-o");
  if (!privateOutput(output)) throw new Error("region comparison reports must stay under out/ or a temporary directory");
  const report = await Bun.file(inputs[0]).json() as ComparisonReport;
  const manifest = await Bun.file(inputs[1]).json() as BuildManifest;
  const result = compareRegions(
    report,
    manifest,
    Number(option(args, "--minimum-ratio", "0")),
    args.includes("--exact"),
    args.includes("--kind") ? option(args, "--kind") : undefined,
  );
  await Bun.write(output, canonicalJson(result) + "\n");
  console.log(`regions=${result.matched_region_count}/${result.region_count} ` +
    `full=${result.fully_covered_count} covered_bytes=${result.covered_bytes} mode=${result.mode}`);
}

if (import.meta.main) await main(Bun.argv.slice(2));
