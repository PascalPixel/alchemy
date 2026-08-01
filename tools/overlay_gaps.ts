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
import { manualRegions, exactSpans, isPrologueShape, savesLinkRegister } from "./overlay_published.ts";

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

export interface Tail {
  /** First byte past the last recorded owner. */
  start: number;
  end: number;
  bytes: number;
  /** Eight-byte `ldr rN,[pc,#0] / bx rN / .word` interworking veneers found. */
  veneers: number;
  /**
   * lr-SAVING prologues in the tail that are not inside a veneer entry.
   *
   * The lr test is sweep C's discriminator and it applies here for the same
   * reason: a prologue you can return through has to save lr, so a pool word
   * or table entry wearing a `push` cannot be one.
   */
  prologues: number[];
  verdict: "EMPTY" | "VENEER-AND-DATA" | "PROLOGUE-SUSPECT";
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

/**
 * The whole of sweep D, as a pure function over an image and a span list.
 *
 * Kept separate from `gapsOf` so the self-test can exercise it on a SYNTHETIC
 * image. A self-test pinned to live tree state is a self-test that fails the
 * moment the work it describes gets done -- this one did exactly that when
 * resource_3a4's 0x3410 was drafted, which is the right alarm from the wrong
 * design.
 */
export function gapsBetween(
  image: Uint8Array,
  spans: { start: number; end: number }[],
): { gaps: Gap[]; overlaps: Overlap[] } {
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

  return { gaps, overlaps };
}

/**
 * Rule the bytes past the last recorded owner.
 *
 * `gapsOf` used to COMPUTE a tail byte-count and print nothing about it, which
 * is the same defect this file was written to fix in sweep B: scan a region,
 * then decline to say anything about it. Isaac caught it, and the cost was
 * real -- resource_3c9's tail held two owners with genuine push prologues, and
 * four overlays stood certified on a sweep that never looked past their last
 * owner.
 *
 * A tail is USUALLY legitimate: the import-veneer bank and the overlay's data
 * tables live there. So this classifies rather than accuses.
 */
export function ruleTail(image: Uint8Array, from: number): Tail {
  const bytes = image.length - from;
  if (bytes <= 0) return { start: from, end: image.length, bytes: 0, veneers: 0, prologues: [], verdict: "EMPTY" };

  // An interworking veneer is `ldr rN,[pc,#0] / bx rN / .word target`, eight
  // bytes. Mark them so a `push`-shaped byte inside one is not read as code.
  const covered = new Uint8Array(bytes);
  let veneers = 0;
  for (let at = from; at + 8 <= image.length; at += 4) {
    const first = image[at] | (image[at + 1] << 8);
    const second = image[at + 2] | (image[at + 3] << 8);
    if ((first & 0xf800) === 0x4800 && (second & 0xff87) === 0x4700) {
      veneers += 1;
      for (let k = at; k < at + 8 && k < image.length; k += 1) covered[k - from] = 1;
    }
  }

  const prologues: number[] = [];
  for (let at = from; at + 1 < image.length; at += 2) {
    if (covered[at - from]) continue;
    const halfword = image[at] | (image[at + 1] << 8);
    if (!isPrologueShape(halfword) || !savesLinkRegister(halfword)) continue;
    prologues.push(at);
  }

  return {
    start: from,
    end: image.length,
    bytes,
    veneers,
    prologues,
    verdict: prologues.length > 0 ? "PROLOGUE-SUSPECT" : "VENEER-AND-DATA",
  };
}

export function gapsOf(overlay: string): {
  gaps: Gap[];
  overlaps: Overlap[];
  duplicates: Duplicate[];
  owners: number;
  tail: Tail;
} {
  const image = overlayImage(overlay);
  const { spans, duplicates } = ownerSpans(overlay);
  const { gaps, overlaps } = gapsBetween(image, spans);
  const last = spans[spans.length - 1];
  return {
    gaps,
    overlaps,
    duplicates,
    owners: spans.length,
    tail: ruleTail(image, last === undefined ? 0 : last.end),
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

  // The founding case, rebuilt synthetically so drafting the real row cannot
  // break it: an owner ending at 0x10, then 24 unaccounted bytes carrying
  // resource_3a4's actual 0x02003410 leaf, then the next owner.
  const image = new Uint8Array(0x40);
  const leaf = [0x4b03, 0x21bf, 0x681b, 0x0049, 0x185a, 0x4b02, 0x8013, 0x4770];
  leaf.forEach((halfword, index) => {
    image[0x10 + index * 2] = halfword & 0xff;
    image[0x10 + index * 2 + 1] = halfword >> 8;
  });
  const { gaps, overlaps } = gapsBetween(image, [
    { start: 0, end: 0x10 },
    { start: 0x28, end: 0x40 },
  ]);
  if (gaps.length !== 1) throw new Error(`sweep D self-test: expected one gap, got ${gaps.length}`);
  if (gaps[0].start !== 0x10 || gaps[0].bytes !== 24)
    throw new Error(`sweep D self-test: gap is 0x${gaps[0].start.toString(16)}/${gaps[0].bytes}B`);
  if (gaps[0].verdict !== "CODE-SUSPECT")
    throw new Error(`sweep D self-test: leaf ruled ${gaps[0].verdict}`);
  // Its return is at the gap's END, which is what tells a real hidden function
  // from a span that merely stopped two bytes before its own `bx r0`.
  if (gaps[0].returns.length !== 1 || gaps[0].returns[0] !== 0x1e)
    throw new Error("sweep D self-test: the leaf's return must be at the gap's end");

  // A two-byte undercount: the gap opens ON the previous owner's return, then
  // runs through its pool. Same verdict, opposite signature.
  const undercount = new Uint8Array(0x20);
  undercount[0x10] = 0x00;
  undercount[0x11] = 0x47; // bx r0 at the gap's FIRST halfword
  const short = gapsBetween(undercount, [
    { start: 0, end: 0x10 },
    { start: 0x1c, end: 0x20 },
  ]);
  if (short.gaps.length !== 1 || short.gaps[0].returns[0] !== short.gaps[0].start)
    throw new Error("sweep D self-test: undercount signature not reproduced");

  // A negative gap is an over-measure, the opposite error, and must be caught.
  const past = gapsBetween(new Uint8Array(0x20), [
    { start: 0, end: 0x18 },
    { start: 0x10, end: 0x20 },
  ]);
  if (past.overlaps.length !== 1 || past.overlaps[0].bytes !== 8)
    throw new Error("sweep D self-test: over-measure not caught");

  // The tail must be RULED, not merely counted. A tail of pure veneers reads
  // as VENEER-AND-DATA; the same tail with one lr-saving push outside a veneer
  // reads as PROLOGUE-SUSPECT. Synthetic, so no overlay's real spans matter.
  const tailImage = new Uint8Array(0x20);
  for (let v = 0x10; v < 0x20; v += 8) {
    tailImage[v] = 0x00; tailImage[v + 1] = 0x48;       // ldr r0, [pc, #0]
    tailImage[v + 2] = 0x00; tailImage[v + 3] = 0x47;   // bx r0
  }
  const clean = ruleTail(tailImage, 0x10);
  if (clean.veneers !== 2) throw new Error(`sweep D self-test: expected 2 veneers, got ${clean.veneers}`);
  if (clean.verdict !== "VENEER-AND-DATA") throw new Error(`sweep D self-test: clean tail ruled ${clean.verdict}`);
  // A `push {r5, r6, lr}` in the tail's data, clear of any veneer entry.
  const dirty = new Uint8Array(0x28);
  dirty.set(tailImage);
  dirty[0x20] = 0x60; dirty[0x21] = 0xb5;
  const flagged = ruleTail(dirty, 0x10);
  if (flagged.verdict !== "PROLOGUE-SUSPECT") throw new Error(`sweep D self-test: dirty tail ruled ${flagged.verdict}`);
  if (flagged.prologues[0] !== 0x20) throw new Error("sweep D self-test: wrong prologue offset");
  // A bare `push {r5, r6}` saves no lr and cannot be returned through.
  const spill = new Uint8Array(0x28);
  spill[0x20] = 0x60; spill[0x21] = 0xb4;
  if (ruleTail(spill, 0x10).verdict !== "VENEER-AND-DATA")
    throw new Error("sweep D self-test: a non-lr push must not flag a tail");

  console.log("sweep D self-test passed (return shapes, leaf, undercount, over-measure, tail ruling)");
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
  let tails = 0;

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
    if (result.tail.verdict === "PROLOGUE-SUSPECT") tails += 1;
    if (json) continue;
    if (
      interesting.length === 0 &&
      result.overlaps.length === 0 &&
      result.duplicates.length === 0 &&
      result.tail.verdict !== "PROLOGUE-SUSPECT"
    )
      continue;

    console.log(overlay);
    if (result.tail.verdict === "PROLOGUE-SUSPECT") {
      const where = result.tail.prologues.slice(0, 8).map((at) => `0x${at.toString(16)}`).join(" ");
      console.log(
        `  TAIL         0x${result.tail.start.toString(16)}-0x${(result.tail.end - 1).toString(16)} ` +
          `${result.tail.bytes}B, ${result.tail.veneers} veneers, ` +
          `${result.tail.prologues.length} lr-saving prologues at ${where}`,
      );
    }
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
    if (Object.keys(report).length === 0) process.exitCode = 1;
    return;
  }
  console.log(
    `\noverlays=${Object.keys(report).length} code_suspect_gaps=${suspects} ` +
      `overlaps=${overlapping} prologue_suspect_tails=${tails}`,
  );
  // SWEEPING NOTHING IS NOT PASSING. A mistyped overlay name used to print
  // `overlays=0 code_suspect_gaps=0 overlaps=0 prologue_suspect_tails=0` and
  // exit 0, which is indistinguishable from a clean overlay -- a lane gating a
  // certification loop on this tool would have recorded a pass for an overlay
  // it never opened. Sweep A/B/C already refuses an unknown name; sweep D did
  // not, and the gap was found by the liveness control of resource_380's
  // certification rather than by reading this file. Same blind spot as sweep
  // B's push requirement, sweep D's tail counting, `ownerOf` accepting an
  // inventory row, and `veneer_resolve`'s 0x1000 cap: the fault lives in what
  // the tool ACCEPTS, not in what it scans.
  if (Object.keys(report).length === 0) {
    console.log(
      "NOTHING SWEPT — this is a FAILURE, not a pass.\n" +
        "  No overlay matched. Check the name against assets/code/*_overlay.s.",
    );
    process.exitCode = 1;
  }
}

if (import.meta.main) main();
