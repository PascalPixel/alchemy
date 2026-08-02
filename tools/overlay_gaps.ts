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
//   bun tools/overlay_gaps.ts --ranked         # owner-shaped gaps, best yield first
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
  /** `bx rN / nop` entries of the compiler's `call_via` bank. */
  callVia: number;
  /**
   * lr-SAVING prologues in the tail that are not inside a veneer entry.
   *
   * The lr test is sweep C's discriminator and it applies here for the same
   * reason: a prologue you can return through has to save lr, so a pool word
   * or table entry wearing a `push` cannot be one.
   */
  prologues: number[];
  /**
   * RETURN-shaped halfwords in the tail outside the veneer and `call_via`
   * banks. This is the field that makes VENEER-AND-DATA mean something.
   */
  returns: number[];
  verdict: "EMPTY" | "VENEER-AND-DATA" | "RETURN-SUSPECT" | "PROLOGUE-SUSPECT";
}

export interface Overlap {
  /** Owner whose recorded span runs past its neighbour. */
  owner: number;
  next: number;
  /** How far past, in bytes. */
  bytes: number;
}

export interface RankedGap {
  overlay: string;
  gap: Gap;
}

/**
 * Put the safest high-yield semantic-C candidates first.
 *
 * Exactly one reachable return is not proof of one owner, but it is the best
 * cheap scheduling signal available: the recent 3bd:13f8, 378:088c,
 * 378:187c, 383:36f8 and 399:19e8 closures all have this shape. Multi-return
 * gaps remain visible after the one-return cohort rather than being discarded.
 */
export function rankGaps(items: RankedGap[]): RankedGap[] {
  return [...items].sort((left, right) => {
    const leftTier = left.gap.returns.length === 1 ? 0 : 1;
    const rightTier = right.gap.returns.length === 1 ? 0 : 1;
    if (leftTier !== rightTier) return leftTier - rightTier;
    if (left.gap.bytes !== right.gap.bytes) return right.gap.bytes - left.gap.bytes;
    const overlayOrder = left.overlay.localeCompare(right.overlay);
    return overlayOrder !== 0 ? overlayOrder : left.gap.start - right.gap.start;
  });
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

/**
 * The two banks that legitimately contain return shapes, masked off so the
 * return-shape discriminator means something in the regions that hold them.
 *
 * A VENEER is `ldr rN,[pc,#0] / bx rN / .word target` -- and the register test
 * is load-bearing, not decoration. The old predicate accepted any `bx rM` after
 * any `ldr rN`, which makes `ldr r0,=table / bx lr` -- a published
 * getter stub, a REAL FUNCTION -- indistinguishable from a veneer. A veneer
 * branches to the register it just loaded; a leaf returns through lr. Pin by
 * register agreement, and resource_3cb's 0x30 and resource_3ce's 0x30, 0x3c and
 * 0x44 stay visible instead of being masked as structure. Measured separately
 * before the alignment fix below: the register tightening changes the tail
 * veneer count on 0 of 96 overlays, so it removed a hazard rather than moved a
 * number. The alignment fix changes it on 6, from zero to between 32 and 72.
 *
 * A CALL_VIA entry is `bx rN / nop`, the compiler's dispatch bank.
 */
export function maskBanks(
  image: Uint8Array,
  from: number,
  to: number,
): { covered: Uint8Array; veneers: number; callVia: number } {
  const covered = new Uint8Array(Math.max(0, to - from));
  let veneers = 0;
  let callVia = 0;

  // ALIGN TO THE IMAGE, NOT TO THE REGION. The old scan stepped `at = from;
  // at += 4`, so whenever a region began at an offset 2 mod 4 -- which is
  // whatever the last owner happened to end on -- every four-byte veneer entry
  // was read straddling its own boundary and none matched. It was invisible
  // while the verdict keyed on prologues only; the moment returns began to
  // count, resource_37b reported 54 of them at a perfect stride of 8, which is
  // a veneer bank announcing itself. A run at a fixed stride is a table, and a
  // table the instrument cannot see is the instrument's fault first.
  const aligned = from + ((4 - (from % 4)) % 4);
  for (let at = aligned; at + 8 <= to; at += 4) {
    const first = image[at] | (image[at + 1] << 8);
    const second = image[at + 2] | (image[at + 3] << 8);
    if ((first & 0xf800) !== 0x4800 || (second & 0xff87) !== 0x4700) continue;
    if (((second >> 3) & 0xf) !== ((first >> 8) & 7)) continue; // branches elsewhere: not a veneer
    veneers += 1;
    for (let k = at; k < at + 8 && k < to; k += 1) covered[k - from] = 1;
  }
  for (let at = from; at + 4 <= to; at += 2) {
    if (covered[at - from]) continue;
    const first = image[at] | (image[at + 1] << 8);
    const second = image[at + 2] | (image[at + 3] << 8);
    if ((first & 0xff87) !== 0x4700 || second !== 0x46c0) continue;
    callVia += 1;
    for (let k = at; k < at + 4 && k < to; k += 1) covered[k - from] = 1;
  }

  return { covered, veneers, callVia };
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

  // `maskBanks` applies to every gap, not just the head. The head is where an
  // overlay's veneer/pointer header usually lives, but it is not the only
  // place: resource_3bf's interior 0x5588-0x57ec gap is EXACTLY 69 fixed
  // eight-byte veneers followed by all 15 four-byte `call_via` entries. The
  // old head-only exception reported their 84 structural `bx` instructions as
  // 84 function returns and ranked 612 bytes of compiler/linker structure as
  // semantic-C debt.
  //
  // Masking does not make a mixed gap disappear. `maskBanks` proves only the
  // exact `ldr rN,[pc,#0] / bx rN / .word` and `bx rN / nop` entries; every
  // return-shaped halfword outside those entries remains visible. Tree-wide at
  // introduction, only two code-suspect gaps changed: resource_3bf loses all
  // 84 structural returns, while resource_3a7 keeps its one ordinary return
  // after 31 veneers are removed. The founding leaf still has one uncovered
  // `bx lr`, so its discriminator is unchanged.
  const rule = (start: number, end: number, after: number, before: number, mask: boolean): void => {
    const returns: number[] = [];
    let padding = true;
    const covered = mask ? maskBanks(image, start, end).covered : undefined;
    for (let at = start; at < end; at += 1) if (image[at] !== 0) padding = false;
    for (let at = start; at + 1 < end; at += 2) {
      if (covered !== undefined && covered[at - start]) continue;
      if (isReturnShape(image[at] | (image[at + 1] << 8))) returns.push(at);
    }
    gaps.push({
      start,
      end,
      after,
      before,
      bytes: end - start,
      returns,
      padding,
      verdict: padding ? "PADDING" : returns.length > 0 ? "CODE-SUSPECT" : "POOL-OR-DATA",
    });
  };

  // THE HEAD. This loop used to start by pairing owner 0 with owner 1, so the
  // region from image start to the FIRST recorded owner was never read -- the
  // same defect as the unswept tail, at the other end, in the same function.
  // Most overlays put their first owner at 0x30 and those 48 bytes are the
  // pointer/veneer header, which rules POOL-OR-DATA and stays quiet. Twenty-five
  // overlays have a larger head, and resource_3bd has a whole function at
  // 0x0030: `push {r5, lr}`, a body, a `bl`, and `pop {r5} / pop {r1} / bx r1`
  // at 0x0066. Sweep C could see it and its own output said so in a way nobody
  // read -- `C shaped 0x2000030 b520 nearest owner 0x? +null`, where the `+null`
  // is not a formatting wart but the statement that NO OWNER PRECEDES IT.
  //
  // `after` is -1 because no owner opens the head. Consumers formatting an owner
  // address must handle it; that is the point of encoding it rather than
  // pretending owner 0 sits at offset 0.
  const first = spans[0];
  if (first !== undefined && first.start > ALIGNMENT_SLACK) rule(0, first.start, -1, first.start, true);

  for (let index = 0; index + 1 < spans.length; index += 1) {
    const current = spans[index];
    const next = spans[index + 1];
    const size = next.start - current.end;

    if (size < 0) {
      overlaps.push({ owner: current.start, next: next.start, bytes: -size });
      continue;
    }
    // ALIGNMENT PADDING IS ZERO. A sub-slack remainder used to be skipped on
    // its SIZE alone, which quietly assumed that anything two bytes or smaller
    // is padding. It is not: an UNDERCOUNTED SPAN that stops between its own
    // `pop {r0}` and its own `bx r0` leaves a two-byte remainder holding a
    // RETURN SHAPE, and sweep D discarded it without looking (found 2026-08-01,
    // by auditing the sealed overlays after `resource_398` showed that a
    // clean owners+head+tail sum silently omits every sub-slack byte).
    //
    // Measured before changing the rule, because a guard that cries wolf gets
    // switched off: **376 sub-slack remainders tree-wide, exactly 2 of them
    // non-zero, and both are `bx r0`** -- `resource_3a4` 0x2d56 after owner
    // 0x2d08, and `resource_3bc` 0x3b7e after owner 0x3b40. No noise at all.
    //
    // Sweep E reaches the same two owners from the other side, by its "recorded
    // span contains NO return shape" check. Two instruments with unrelated
    // failure modes landing on one answer is evidence; this one is cheaper,
    // because it needs no owner-start set and no `bl` resolution.
    let padded = true;
    for (let at = current.end; at < next.start; at += 1) if (image[at] !== 0) padded = false;
    if (size <= ALIGNMENT_SLACK && padded) continue;

    rule(current.end, next.start, current.start, next.start, true);
  }

  return { gaps, overlaps };
}

/**
 * Rule the bytes past the last recorded owner.
 *
 * `gapsOf` used to COMPUTE a tail byte-count and print nothing about it, which
 * is the same defect this file was written to fix in sweep B: scan a region,
 * then decline to say anything about it. resource_3c9's tail held two owners
 * with genuine push prologues, and
 * four overlays stood certified on a sweep that never looked past their last
 * owner.
 *
 * A tail is USUALLY legitimate: the import-veneer bank and the overlay's data
 * tables live there. So this classifies rather than accuses.
 *
 * THE PROLOGUE GATE WAS STILL HERE (fixed 2026-08-01). This function was
 * written to close sweep B's blind spot at the other end of the image, and it
 * shipped carrying the exact defect it was answering: it decided
 * `prologues.length > 0 ? PROLOGUE-SUSPECT : VENEER-AND-DATA`, on `push`
 * prologues ALONE. A leaf saves no register, so every leaf in every tail on the
 * tree was filed VENEER-AND-DATA. Three were: resource_395's 0x1838 and
 * 0x1858 -- 14 bytes each, byte-identical, one `stmia` writing DMA3 SAD/DAD/CNT
 * and differing in exactly one pool word -- and resource_3cd's 0x07b8. None is
 * `bl`-reached, none is published, none has a `push`: invisible to all four
 * sweeps at once, which this file had recorded as a hypothetical.
 *
 * THE REPLACEMENT ARGUMENT is structural: **a Thumb function cannot
 * avoid returning.** So count RETURN shapes outside the two banks that
 * legitimately contain them. Zero uncovered returns is not "no push found", it
 * is a proof that no function of any shape -- leaf, stub, or ordinary -- lives
 * in those bytes. That is what VENEER-AND-DATA now asserts.
 *
 * Two banks must be masked or the discriminator drowns:
 *   - the interworking veneer, `ldr rN,[pc,#0] / bx rN / .word`;
 *   - the compiler's `call_via` bank, a run of `bx rN / nop` for each register.
 *     `overlay_call_targets` tags its entry `call_via`; on resource_398 the
 *     `bx lr` at 0x0904 carries FIFTEEN call sites. An isolated `bx lr` in a
 *     tail is not automatically a find, and 29 of the 36 uncovered returns on
 *     the tree are this bank.
 */
export function ruleTail(image: Uint8Array, from: number): Tail {
  const bytes = image.length - from;
  if (bytes <= 0)
    return { start: from, end: image.length, bytes: 0, veneers: 0, callVia: 0, prologues: [], returns: [], verdict: "EMPTY" };

  const { covered, veneers, callVia } = maskBanks(image, from, image.length);

  const prologues: number[] = [];
  const returns: number[] = [];
  for (let at = from; at + 1 < image.length; at += 2) {
    if (covered[at - from]) continue;
    const halfword = image[at] | (image[at + 1] << 8);
    if (isReturnShape(halfword)) returns.push(at);
    if (!isPrologueShape(halfword) || !savesLinkRegister(halfword)) continue;
    prologues.push(at);
  }

  return {
    start: from,
    end: image.length,
    bytes,
    veneers,
    callVia,
    prologues,
    returns,
    verdict:
      prologues.length > 0 ? "PROLOGUE-SUSPECT" : returns.length > 0 ? "RETURN-SUSPECT" : "VENEER-AND-DATA",
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
 * Overlays whose residue is empty under sweeps A and B, i.e. the ones a pass
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

  const ranked = rankGaps([
    { overlay: "resource_b", gap: { start: 0x20, end: 0x120, after: 0, before: 0x120, bytes: 256, returns: [0x40, 0x80], padding: false, verdict: "CODE-SUSPECT" } },
    { overlay: "resource_c", gap: { start: 0x30, end: 0xb0, after: 0, before: 0xb0, bytes: 128, returns: [0x90], padding: false, verdict: "CODE-SUSPECT" } },
    { overlay: "resource_a", gap: { start: 0x10, end: 0xd0, after: 0, before: 0xd0, bytes: 192, returns: [0xa0], padding: false, verdict: "CODE-SUSPECT" } },
  ]);
  if (ranked.map((item) => item.overlay).join(",") !== "resource_a,resource_c,resource_b")
    throw new Error("sweep D self-test: ranked candidates must prefer one-return gaps, then byte yield");

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

  // A SUB-SLACK remainder is skipped when it is zero and REPORTED when it is
  // not, in both directions, because the whole hazard is a size test standing
  // in for a content test.
  const padding2 = new Uint8Array(0x20);
  if (gapsBetween(padding2, [{ start: 0, end: 0x10 }, { start: 0x12, end: 0x20 }]).gaps.length !== 0)
    throw new Error("sweep D self-test: a zero 2-byte remainder is alignment and must stay quiet");
  const live2 = new Uint8Array(0x20);
  live2[0x10] = 0x00; live2[0x11] = 0x47; // `bx r0` in a two-byte remainder
  const caught = gapsBetween(live2, [{ start: 0, end: 0x10 }, { start: 0x12, end: 0x20 }]);
  if (caught.gaps.length !== 1 || caught.gaps[0].verdict !== "CODE-SUSPECT")
    throw new Error("sweep D self-test: a RETURN in a two-byte remainder is an undercounted span, not padding");
  if (caught.gaps[0].returns.length !== 1 || caught.gaps[0].returns[0] !== 0x10)
    throw new Error("sweep D self-test: the remainder's return must be located");

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

  // THE LEAF IN THE TAIL. The whole point of the RETURN-SUSPECT class: a
  // function with no `push` at all, which the prologue-only verdict filed as
  // VENEER-AND-DATA. resource_3cd's 0x07b8, verbatim.
  const tailLeaf = new Uint8Array(0x30);
  tailLeaf.set(tailImage);
  [0x4b02, 0x681b, 0x2201, 0x3335, 0x701a, 0x4770].forEach((halfword, index) => {
    tailLeaf[0x20 + index * 2] = halfword & 0xff;
    tailLeaf[0x20 + index * 2 + 1] = halfword >> 8;
  });
  const leafTail = ruleTail(tailLeaf, 0x10);
  if (leafTail.prologues.length !== 0)
    throw new Error("sweep D self-test: the tail leaf must have NO prologue -- that is why it was missed");
  if (leafTail.verdict !== "RETURN-SUSPECT")
    throw new Error(`sweep D self-test: tail leaf ruled ${leafTail.verdict}, not RETURN-SUSPECT`);
  if (leafTail.returns.length !== 1 || leafTail.returns[0] !== 0x2a)
    throw new Error("sweep D self-test: the tail leaf's `bx lr` must be the flagged return");

  // ...and the OTHER direction, or the new class is just noise: the `call_via`
  // bank is a legitimate run of `bx rN / nop` and must NOT flag a tail. Without
  // this mask, 29 of the tree's 36 uncovered returns are this bank and the
  // verdict means nothing.
  const bank = new Uint8Array(0x30);
  for (let index = 0; index < 8; index += 1) {
    bank[0x10 + index * 4] = index << 3;
    bank[0x10 + index * 4 + 1] = 0x47;   // bx rN
    bank[0x10 + index * 4 + 2] = 0xc0;
    bank[0x10 + index * 4 + 3] = 0x46;   // nop
  }
  const banked = ruleTail(bank, 0x10);
  if (banked.callVia !== 8) throw new Error(`sweep D self-test: expected 8 call_via entries, got ${banked.callVia}`);
  if (banked.verdict !== "VENEER-AND-DATA")
    throw new Error(`sweep D self-test: the call_via bank ruled ${banked.verdict} -- the mask is not working`);

  // The same structural bank can live BETWEEN owners. It must be quiet there
  // too, while an ordinary return beside a veneer must remain visible. This is
  // the exact failure direction exposed by resource_3bf:5588-57ec.
  const interiorImage = new Uint8Array(0x40);
  interiorImage.set(bank.subarray(0x10, 0x30), 0x10);
  const interiorBank = gapsBetween(interiorImage, [
    { start: 0, end: 0x10 },
    { start: 0x30, end: 0x40 },
  ]);
  if (interiorBank.gaps.length !== 1 || interiorBank.gaps[0].verdict !== "POOL-OR-DATA")
    throw new Error("sweep D self-test: an interior call_via bank is structural, not semantic-C debt");

  const mixed = new Uint8Array(0x40);
  mixed[0x10] = 0x00; mixed[0x11] = 0x48; // ldr r0, [pc, #0]
  mixed[0x12] = 0x00; mixed[0x13] = 0x47; // bx r0: covered veneer return
  mixed[0x18] = 0x00; mixed[0x19] = 0x48; // ldr r0, [pc, #0]
  mixed[0x1a] = 0x70; mixed[0x1b] = 0x47; // bx lr: uncovered getter return
  const mixedGap = gapsBetween(mixed, [
    { start: 0, end: 0x10 },
    { start: 0x30, end: 0x40 },
  ]);
  if (mixedGap.gaps.length !== 1 || mixedGap.gaps[0].verdict !== "CODE-SUSPECT")
    throw new Error("sweep D self-test: masking an interior veneer must not hide an adjacent getter leaf");
  if (mixedGap.gaps[0].returns.length !== 1 || mixedGap.gaps[0].returns[0] !== 0x1a)
    throw new Error("sweep D self-test: the mixed interior gap must retain only its ordinary return");

  // THE REGISTER TEST inside `maskBanks`, both directions. `ldr r0 / bx r0` is
  // structure and must be masked; `ldr r0 / bx lr` is a getter stub, a
  // real function, and must NOT be. Same four bytes to any predicate that does
  // not compare the registers.
  const veneer = new Uint8Array(8);
  veneer[0] = 0x00; veneer[1] = 0x48; veneer[2] = 0x00; veneer[3] = 0x47;
  if (maskBanks(veneer, 0, 8).veneers !== 1) throw new Error("sweep D self-test: `ldr r0 / bx r0` is a veneer");
  const getter = new Uint8Array(8);
  getter[0] = 0x00; getter[1] = 0x48; getter[2] = 0x70; getter[3] = 0x47;
  if (maskBanks(getter, 0, 8).veneers !== 0)
    throw new Error("sweep D self-test: `ldr r0 / bx lr` is a LEAF GETTER, not a veneer");

  // THE HEAD. resource_3bd's 0x0030 function, reduced to its epilogue: a region
  // before the FIRST owner must be ruled, and a return shape in it is
  // CODE-SUSPECT. The loop used to begin at owner 0, so this region did not
  // exist as far as sweep D was concerned.
  const headed = new Uint8Array(0x40);
  headed[0x20] = 0x08; headed[0x21] = 0x47; // bx r1, inside the head
  const withHead = gapsBetween(headed, [{ start: 0x30, end: 0x40 }]);
  if (withHead.gaps.length !== 1) throw new Error("sweep D self-test: the head was not ruled");
  if (withHead.gaps[0].start !== 0 || withHead.gaps[0].end !== 0x30)
    throw new Error("sweep D self-test: the head must run from image start to the first owner");
  if (withHead.gaps[0].after !== -1)
    throw new Error("sweep D self-test: the head has no owner before it and must say so");
  if (withHead.gaps[0].verdict !== "CODE-SUSPECT")
    throw new Error(`sweep D self-test: a return in the head ruled ${withHead.gaps[0].verdict}`);
  // A first owner at offset 0 leaves no head, and an all-zero head is PADDING,
  // not a finding -- the new region must not invent work.
  if (gapsBetween(new Uint8Array(0x20), [{ start: 0, end: 0x20 }]).gaps.length !== 0)
    throw new Error("sweep D self-test: an owner at offset 0 must leave no head");
  const blank = gapsBetween(new Uint8Array(0x40), [{ start: 0x30, end: 0x40 }]);
  if (blank.gaps.length !== 1 || blank.gaps[0].verdict !== "PADDING")
    throw new Error("sweep D self-test: an all-zero head must rule PADDING");

  // The failure DIRECTION is pinned by running the tool, because the defect
  // lived in `main` and in no function above: a name matching no overlay must
  // exit non-zero, and a real overlay must exit zero. A predicate that rots
  // into always-failing is as useless as one that rots into always-passing, so
  // both directions are asserted.
  const self = Bun.fileURLToPath(import.meta.url);
  const run = (name: string) =>
    Bun.spawnSync(["bun", self, name], { stdout: "pipe", stderr: "pipe" }).exitCode;
  // No overlay is NAMED as the passing fixture: it is whichever overlay is
  // first in the tree, because sweep D exits 0 for any overlay it can actually
  // sweep, whatever it finds there. Naming one pins the test to a tree state
  // that source changes move every cycle.
  if (run("resource_ffffff") === 0)
    throw new Error("sweep D self-test: an empty sweep must NOT exit 0");
  const anyOverlay = overlayNames()[0];
  if (anyOverlay !== undefined && run(anyOverlay) !== 0)
    throw new Error(`sweep D self-test: a real overlay (${anyOverlay}) must exit 0`);

  console.log(
    "sweep D self-test passed (return shapes, leaf, undercount, over-measure, tail ruling,\n" +
      "  tail leaf, call_via/interior-bank mask, head region, empty-sweep refusal)",
  );
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) return selfTest();

  const json = argv.includes("--json");
  const ranked = argv.includes("--ranked");
  if (json && ranked) throw new Error("choose either --json or --ranked");
  const named = argv.filter((argument) => !argument.startsWith("--"));
  const overlays = named.length > 0 ? named : overlayNames();

  const report: Record<string, ReturnType<typeof gapsOf>> = {};
  let suspects = 0;
  let overlapping = 0;
  let tails = 0;
  let returnTails = 0;

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
    if (result.tail.verdict === "RETURN-SUSPECT") returnTails += 1;
    if (json || ranked) continue;
    if (
      interesting.length === 0 &&
      result.overlaps.length === 0 &&
      result.duplicates.length === 0 &&
      result.tail.verdict !== "PROLOGUE-SUSPECT" &&
      result.tail.verdict !== "RETURN-SUSPECT"
    )
      continue;

    console.log(overlay);
    if (result.tail.verdict === "PROLOGUE-SUSPECT" || result.tail.verdict === "RETURN-SUSPECT") {
      const at = (offsets: number[]) => offsets.slice(0, 8).map((offset) => `0x${offset.toString(16)}`).join(" ");
      console.log(
        `  TAIL         0x${result.tail.start.toString(16)}-0x${(result.tail.end - 1).toString(16)} ` +
          `${result.tail.bytes}B, ${result.tail.veneers} veneers, ${result.tail.callVia} call_via`,
      );
      // BOTH signals, always. A single-verdict line let the stronger one shadow
      // the weaker: resource_395's tail reads PROLOGUE-SUSPECT on five pushes,
      // and printing only those hid the returns at 0x1846 and 0x1866 -- the two
      // leaves this whole class was added to surface. The tiering is fine for a
      // one-word verdict and useless as a report.
      if (result.tail.prologues.length > 0)
        console.log(`               ${result.tail.prologues.length} lr-saving prologues at ${at(result.tail.prologues)}`);
      if (result.tail.returns.length > 0)
        console.log(`               ${result.tail.returns.length} returns outside the banks at ${at(result.tail.returns)}`);
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
  if (ranked) {
    const candidates = rankGaps(Object.entries(report).flatMap(([overlay, result]) =>
      result.gaps
        .filter((gap) => gap.verdict === "CODE-SUSPECT")
        .map((gap) => ({ overlay, gap })),
    ));
    for (const { overlay, gap } of candidates) {
      const shape = gap.returns.length === 1 ? "ONE-RETURN" : `${gap.returns.length}-RETURN`;
      console.log(
        `${gap.bytes.toString().padStart(6)}B  ${shape.padEnd(10)}  ${overlay} ` +
          `0x${gap.start.toString(16)}-0x${gap.end.toString(16)}`,
      );
    }
    console.log(`\nranked_code_suspect_gaps=${candidates.length}`);
  }
  console.log(
    `\noverlays=${Object.keys(report).length} code_suspect_gaps=${suspects} ` +
      `overlaps=${overlapping} prologue_suspect_tails=${tails} return_suspect_tails=${returnTails}`,
  );
  // SWEEPING NOTHING IS NOT PASSING. A mistyped overlay name used to print
  // `overlays=0 code_suspect_gaps=0 overlaps=0 prologue_suspect_tails=0` and
  // exit 0, which is indistinguishable from a clean overlay -- an automated
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
        "  No code overlay matched. Check the name against assets/code/*_overlay.s.",
    );
    process.exitCode = 1;
  }
}

if (import.meta.main) main();
