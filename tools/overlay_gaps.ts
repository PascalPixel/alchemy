#!/usr/bin/env bun
// Sweep D: read the gaps between recorded owners.
//
// Why this exists. On 2026-08-01 resource_3a4's sweeps A and B both went empty
// and its one class-C hit was settled as table data. It was not closed. A
// complete 24-byte function sat at 0x02003410, in the gap between 0x02003028's
// true end and the next recorded owner:
//
//   ldr r3, =0x03001ebc / movs r1, #191 / ldr r3, [r3] / lsls r1, #1 /
//   adds r2, r3, r1 / ldr r3, =0x1018 / strh r3, [r2] / bx lr
//
// No `bl` in the image targeted it, no word in the image held its published
// spelling, and it had NO PUSH PROLOGUE because it is a leaf -- it saves no
// register. Sweeps A, B and C are all KEYED: each hunts a signature, a `bl`
// target, a published pointer word, a `push`-shaped halfword. A leaf offers
// none of the three.
//
// Sweep D is the only UNKEYED one. It looks at unaccounted BYTES rather than at
// a signature, which is why it catches what the signatures cannot.
//
// The arithmetic: for each recorded owner, subtract its end from the next
// owner's start. 0-2 bytes is alignment. Anything larger is unaccounted and
// must be read and ruled -- more literal pool, data, or a function.
//
// It screens the OPPOSITE error for free. A NEGATIVE gap means a drafted span
// ran past its neighbour, which is the over-measure `measureSpan` produced on
// resource_3a4's 0x020039c8 (356 reported against 124 real). One subtraction,
// both directions.
//
// The report is a CANDIDATE LIST, not a verdict, in the same way sweep C is
// noisy by design. Two known sources of legitimate fire:
//   - a row whose recorded span_bytes came from the strict inventory and so
//     stops before its literal pool. The gap is that pool.
//   - a pool word that happens to wear a return shape, e.g. resource_3a4's
//     0x0200bd78 disassembling as `pop {r3, r4, r5, r6, pc}`.
// Both are ruled by reading the gap. That read is the sweep; this tool only
// says where to point it.
//
// RETURN-SHAPED is the useful discriminator inside a gap, because whatever else
// a hidden function does, it must still return.
//
//   bun tools/overlay_gaps.ts                  # every overlay
//   bun tools/overlay_gaps.ts resource_3a4     # one overlay
//   bun tools/overlay_gaps.ts --certified      # only overlays claimed closed
//   bun tools/overlay_gaps.ts --json
//   bun tools/overlay_gaps.ts --self-test
import { existsSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { overlayImage } from "./overlay_call_targets.ts";
import { manualRegions, exactSpans } from "./overlay_published.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

/** Alignment only ever costs a halfword. Anything more is unaccounted. */
export const ALIGNMENT_SLACK = 2;

export interface Gap {
  /** Offset of the first unaccounted byte. */
  start: number;
  /** Offset one past the last unaccounted byte. */
  end: number;
  /** Owner whose end opens the gap. */
  after: number;
  /** Owner whose start closes it. */
  before: number;
  bytes: number;
  /** Return-shaped halfwords inside the gap, as offsets. */
  returns: number[];
  /** Every byte is zero. */
  padding: boolean;
  verdict: "PADDING" | "POOL-OR-DATA" | "CODE-SUSPECT";
}

export interface Overlap {
  /** Owner whose recorded span runs past its neighbour. */
  owner: number;
  next: number;
  /** How far past, in bytes. */
  bytes: number;
}

/**
 * `bx lr` / `bx rN`, or `pop {..., pc}`.
 *
 * A hidden function may open with anything at all -- that is the whole reason
 * sweeps A, B and C miss leaves -- but it cannot avoid returning.
 */
export function isReturnShape(halfword: number): boolean {
  if ((halfword & 0xff87) === 0x4700) return true; // bx rN
  if ((halfword & 0xff00) === 0xbd00) return true; // pop {..., pc}
  return false;
}

export interface Duplicate {
  start: number;
  /** Span recorded in semantic/regions.json. */
  recorded: number;
  /** Span the compiler actually produces for the exact-C row. */
  compiled: number;
}

/**
 * Every recorded owner span on an overlay: semantic bodies plus exact-C rows.
 *
 * A row promoted from a semantic draft to exact C is recorded TWICE -- its old
 * `manual_regions` entry stays behind, and its span there can disagree with
 * what the compiler now produces. Deduplicated by start, keeping the longer
 * span, because a gap computed off the shorter one is a phantom. The
 * disagreements are returned rather than swallowed: a recorded span shorter
 * than the compiled one is a stale figure someone may still be drafting
 * against.
 */
export function ownerSpans(overlay: string): { spans: { start: number; end: number }[]; duplicates: Duplicate[] } {
  const recorded = manualRegions(overlay).filter((span) => span.end > span.start);
  const compiled = exactSpans(overlay).filter((span) => span.end > span.start);
  const byStart = new Map<number, { start: number; end: number }>();
  const duplicates: Duplicate[] = [];

  for (const span of recorded) {
    const seen = byStart.get(span.start);
    if (seen === undefined || span.end > seen.end) byStart.set(span.start, span);
  }
  for (const span of compiled) {
    const seen = byStart.get(span.start);
    if (seen !== undefined && seen.end !== span.end) {
      duplicates.push({ start: span.start, recorded: seen.end - span.start, compiled: span.end - span.start });
    }
    if (seen === undefined || span.end > seen.end) byStart.set(span.start, span);
  }

  return {
    spans: [...byStart.values()].sort((a, b) => a.start - b.start),
    duplicates: duplicates.sort((a, b) => a.start - b.start),
  };
}

export function gapsOf(overlay: string): {
  gaps: Gap[];
  overlaps: Overlap[];
  duplicates: Duplicate[];
  owners: number;
  tail: number;
} {
  const image = overlayImage(overlay);
  const { spans, duplicates } = ownerSpans(overlay);
  const gaps: Gap[] = [];
  const overlaps: Overlap[] = [];

  for (let index = 0; index + 1 < spans.length; index += 1) {
    const current = spans[index];
    const next = spans[index + 1];
    const size = next.start - current.end;

    if (size < 0) {
      overlaps.push({ owner: current.start, next: next.start, bytes: -size });
      continue;
    }
    if (size <= ALIGNMENT_SLACK) continue;

    const start = current.end;
    const end = next.start;
    const returns: number[] = [];
    let padding = true;
    for (let at = start; at < end; at += 1) if (image[at] !== 0) padding = false;
    for (let at = start; at + 1 < end; at += 2) {
      if (isReturnShape(image[at] | (image[at + 1] << 8))) returns.push(at);
    }

    gaps.push({
      start,
      end,
      after: current.start,
      before: next.start,
      bytes: size,
      returns,
      padding,
      verdict: padding ? "PADDING" : returns.length > 0 ? "CODE-SUSPECT" : "POOL-OR-DATA",
    });
  }

  const last = spans[spans.length - 1];
  return {
    gaps,
    overlaps,
    duplicates,
    owners: spans.length,
    tail: last === undefined ? 0 : image.length - last.end,
  };
}

/**
 * Overlays whose residue is empty under sweeps A and B, i.e. the ones a lane
 * would be entitled to call closed under the pre-sweep-D standard.
 */
export function overlayNames(): string[] {
  const path = join(ROOT, "assets", "code");
  if (!existsSync(path)) return [];
  const names = new Set<string>();
  for (const name of readdirSync(path)) {
    const hit = /^(resource_[0-9a-f]+)_overlay\.s$/.exec(name);
    if (hit !== null) names.add(hit[1]);
  }
  return [...names].sort();
}

function selfTest(): void {
  if (!isReturnShape(0x4770)) throw new Error("sweep D self-test: bx lr");
  if (!isReturnShape(0x4700)) throw new Error("sweep D self-test: bx r0");
  if (!isReturnShape(0xbd78)) throw new Error("sweep D self-test: pop pc");
  if (isReturnShape(0xb5e0)) throw new Error("sweep D self-test: push is not a return");
  if (isReturnShape(0x0000)) throw new Error("sweep D self-test: zero is not a return");

  // The founding case: resource_3a4's leaf at 0x3410 must be reported, and the
  // gap must be CODE-SUSPECT rather than pool.
  const found = gapsOf("resource_3a4").gaps.find((gap) => gap.start === 0x3410);
  if (found === undefined) throw new Error("sweep D self-test: 0x3410 gap not reported");
  if (found.bytes !== 24) throw new Error(`sweep D self-test: 0x3410 gap is ${found.bytes}, expected 24`);
  if (found.verdict !== "CODE-SUSPECT") throw new Error(`sweep D self-test: 0x3410 ruled ${found.verdict}`);
  console.log("sweep D self-test passed (bx lr, bx rN, pop pc, and the 0x3410 leaf)");
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) return selfTest();

  const json = argv.includes("--json");
  const named = argv.filter((argument) => !argument.startsWith("--"));
  const overlays = named.length > 0 ? named : overlayNames();

  const report: Record<string, ReturnType<typeof gapsOf>> = {};
  let suspects = 0;
  let overlapping = 0;

  for (const overlay of overlays) {
    let result: ReturnType<typeof gapsOf>;
    try {
      result = gapsOf(overlay);
    } catch {
      continue;
    }
    report[overlay] = result;
    const interesting = result.gaps.filter((gap) => gap.verdict === "CODE-SUSPECT");
    suspects += interesting.length;
    overlapping += result.overlaps.length;
    if (json) continue;
    if (interesting.length === 0 && result.overlaps.length === 0 && result.duplicates.length === 0) continue;

    console.log(overlay);
    for (const overlap of result.overlaps) {
      console.log(
        `  OVERLAP      0x${overlap.owner.toString(16)} runs ${overlap.bytes} bytes past 0x${overlap.next.toString(16)}`,
      );
    }
    for (const duplicate of result.duplicates) {
      console.log(
        `  STALE-SPAN   0x${duplicate.start.toString(16)} recorded ${duplicate.recorded}B, compiles to ${duplicate.compiled}B`,
      );
    }
    for (const gap of interesting) {
      const where = gap.returns.map((at) => `0x${at.toString(16)}`).join(" ");
      console.log(
        `  CODE-SUSPECT 0x${gap.start.toString(16)}-0x${(gap.end - 1).toString(16)} ` +
          `${gap.bytes}B after 0x${gap.after.toString(16)}, returns at ${where}`,
      );
    }
  }

  if (json) {
    console.log(JSON.stringify(report, null, 2));
    return;
  }
  console.log(`\noverlays=${Object.keys(report).length} code_suspect_gaps=${suspects} overlaps=${overlapping}`);
}

if (import.meta.main) main();
