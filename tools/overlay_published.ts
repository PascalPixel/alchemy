#!/usr/bin/env bun
// Find overlay functions that are installed by DATA rather than called by code.
//
// Why this exists. On 2026-08-01 `resource_3af` was declared closed after the
// two standard checks both returned zero: a whole-image `bl` sweep resolved
// with the +2 rule, and `tools/overlay_unindexed.ts`. Ten owners worth about
// 3,064 bytes were still sitting in it.
//
// Neither check could see them, and not by accident. A data-installed callback
// is NEVER the target of a `bl` — its address is written into a script record
// as a word, with the Thumb bit set and the overlay's 0x8000 link base already
// applied. A call-graph walk is structurally blind to that whole class.
//
// The sweep: for every 4-aligned word in the image, if the Thumb bit is set,
// resolve `(word & ~1) - 0x8000` and check whether the halfword there is a
// `push` prologue (0xb4xx / 0xb5xx). If it is, something published a pointer to
// a function at that offset.
//
// Reported alongside it are the other two sweeps a closure needs:
//
//   A  `bl` targets that land on a prologue and are not owned.
//   B  published pointers to prologues that are not owned   (this file's point)
//   C  every prologue-shaped halfword that is not an owner start.
//
// C is NOISY BY DESIGN and must be CLASSIFIED, not zeroed. Two things make it
// fire legitimately: high-register spill pushes a few bytes inside a prologue
// (`push {r5, r6}` after `push {r5, r6, lr}`), and literal pool words that
// happen to look like a push. The cheap discriminator for the second is that a
// prologue you can return through must save `lr` — `resource_3af`'s pool word
// 0x0200b4bd decodes as `push {r0, r2, r3, r4, r5, r7}` and cannot be one.
//
//   bun tools/overlay_published.ts                 # every overlay
//   bun tools/overlay_published.ts resource_3af    # one overlay
//   bun tools/overlay_published.ts --json
//   bun tools/overlay_published.ts --self-test
import { existsSync, readdirSync, readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { classify, overlayImage, OVERLAY_BASE, BASE_SHIFT, targetOffset } from "./overlay_call_targets.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

/** A `push {regs}` (0xb4xx) or `push {regs, lr}` (0xb5xx) opening. */
export function isPrologueShape(halfword: number): boolean {
  return (halfword & 0xfe00) === 0xb400;
}

/** A prologue that can be returned through has to save `lr`. */
export function savesLinkRegister(halfword: number): boolean {
  return (halfword & 0xff00) === 0xb500;
}

/**
 * Resolve a pool/table word to the image offset it publishes, or null.
 *
 * The Thumb bit must be set — a published *function* pointer is odd — and the
 * overlay's link base subtracted, exactly as `overlay_call_targets` does for
 * jump-table entries.
 */
export function publishedOffset(word: number, imageLength: number): number | null {
  if ((word & 1) === 0) return null;
  const offset = (word & ~1) - (OVERLAY_BASE + BASE_SHIFT);
  if (offset < 0 || offset + 2 > imageLength) return null;
  return offset;
}

export interface OverlayResidue {
  overlay: string;
  /** bl-reached prologues with no owner. */
  called: number[];
  /** published prologues with no owner. */
  published: number[];
  /** prologue-shaped halfwords with no owner, each classified. */
  shaped: { offset: number; halfword: number; owner: number | null; delta: number | null; verdict: string }[];
}

/** Owners are whatever the tree already claims: a C file, or a manual region. */
export function ownerSet(overlay: string): Set<number> {
  const owners = new Set<number>();
  const pattern = new RegExp(`^${overlay}_c_0*([0-9a-f]+)\\.c$`);
  for (const directory of ["assets/code", "semantic/overlays"]) {
    const path = join(ROOT, directory);
    if (!existsSync(path)) continue;
    for (const name of readdirSync(path)) {
      const hit = pattern.exec(name);
      if (hit !== null) owners.add(Number.parseInt(hit[1], 16) - OVERLAY_BASE);
    }
  }
  for (const region of manualRegions(overlay)) owners.add(region.start);
  return owners;
}

/**
 * Recorded semantic bodies, as [start, end) offsets.
 *
 * These are the only spans the tree states outright. Exact-C rows under
 * assets/code carry no span here — their extent is whatever the compiler
 * produces — so a candidate sitting behind an exact row cannot be ruled inside
 * a body by this data alone, and is reported as needing the owning lane's
 * check rather than silently dropped.
 */
export function manualRegions(overlay: string): { start: number; end: number }[] {
  const path = join(ROOT, "semantic", "regions.json");
  if (!existsSync(path)) return [];
  const document = JSON.parse(readFileSync(path, "utf8")) as {
    manual_regions: { overlay?: string; entry?: string; span_bytes?: number }[];
  };
  const spans: { start: number; end: number }[] = [];
  for (const region of document.manual_regions) {
    if (region.overlay !== overlay || region.entry === undefined) continue;
    const start = Number.parseInt(region.entry, 16) - OVERLAY_BASE;
    spans.push({ start, end: start + (region.span_bytes ?? 0) });
  }
  return spans.sort((a, b) => a.start - b.start);
}

/** Exact-C rows, whose spans are not recorded anywhere this tool can read. */
export function exactStarts(overlay: string): Set<number> {
  const starts = new Set<number>();
  const path = join(ROOT, "assets", "code");
  const pattern = new RegExp(`^${overlay}_c_0*([0-9a-f]+)\\.c$`);
  if (!existsSync(path)) return starts;
  for (const name of readdirSync(path)) {
    const hit = pattern.exec(name);
    if (hit !== null) starts.add(Number.parseInt(hit[1], 16) - OVERLAY_BASE);
  }
  return starts;
}

export function sweep(overlay: string): OverlayResidue {
  const image = overlayImage(overlay);
  const owners = ownerSet(overlay);
  const sorted = [...owners].sort((a, b) => a - b);
  const bodies = manualRegions(overlay);
  const exact = exactStarts(overlay);
  /** Inside a body the tree explicitly measured — a pool word or a spill. */
  const insideRecordedBody = (offset: number): boolean =>
    bodies.some((body) => offset > body.start && offset < body.end);
  /** Behind an exact-C row, whose real extent this tool cannot read. */
  const behindExactRow = (offset: number): boolean => {
    let nearest: number | null = null;
    for (const start of sorted) {
      if (start <= offset) nearest = start;
      else break;
    }
    return nearest !== null && exact.has(nearest);
  };

  const called: number[] = [];
  const seenCall = new Set<number>();
  for (let at = 0; at + 4 <= image.length; at += 2) {
    const target = targetOffset(image[at] | (image[at + 1] << 8), image[at + 2] | (image[at + 3] << 8));
    if (target === null || target < 0 || target >= image.length) continue;
    if (classify(image, target, new Set()).kind !== "prologue") continue;
    if (owners.has(target) || seenCall.has(target)) continue;
    if (insideRecordedBody(target)) continue;
    seenCall.add(target);
    called.push(target);
  }

  const published: number[] = [];
  const seenPublish = new Set<number>();
  for (let at = 0; at + 4 <= image.length; at += 4) {
    const word =
      (image[at] | (image[at + 1] << 8) | (image[at + 2] << 16) | (image[at + 3] << 24)) >>> 0;
    const offset = publishedOffset(word, image.length);
    if (offset === null) continue;
    if (!isPrologueShape(image[offset] | (image[offset + 1] << 8))) continue;
    if (owners.has(offset) || seenPublish.has(offset)) continue;
    if (insideRecordedBody(offset)) continue;
    seenPublish.add(offset);
    published.push(offset);
  }

  const shaped: OverlayResidue["shaped"] = [];
  for (let at = 0; at + 2 <= image.length; at += 2) {
    const halfword = image[at] | (image[at + 1] << 8);
    if (!isPrologueShape(halfword) || owners.has(at)) continue;
    if (insideRecordedBody(at)) continue;
    let owner: number | null = null;
    for (const candidate of sorted) {
      if (candidate <= at) owner = candidate;
      else break;
    }
    const delta = owner === null ? null : at - owner;
    // A spill push sits within the prologue of the owner it belongs to. Twelve
    // bytes covers the widest spill seen so far (fp/sl then r9/r8, two pairs).
    const verdict =
      delta !== null && delta <= 12
        ? "spill inside an owner's prologue"
        : !savesLinkRegister(halfword)
          ? "pool word wearing a push (no lr, cannot be returned through)"
          : behindExactRow(at)
            ? "UNRULED — saves lr, but sits behind an exact-C row whose span this tool cannot read"
            : "UNEXPLAINED — saves lr, outside every recorded body";
    shaped.push({ offset: at, halfword, owner, delta, verdict });
  }

  return { overlay, called, published, shaped };
}

function overlayNames(): string[] {
  const path = join(ROOT, "assets", "code");
  return readdirSync(path)
    .filter((name) => name.endsWith("_overlay.s"))
    .map((name) => name.replace(/_overlay\.s$/, ""))
    .sort();
}

function selfTest(): void {
  if (!isPrologueShape(0xb500)) throw new Error("push {lr} is a prologue shape");
  if (!isPrologueShape(0xb5e0)) throw new Error("push {r5,r6,r7,lr} is a prologue shape");
  if (!isPrologueShape(0xb460)) throw new Error("a bare push is a prologue shape");
  if (isPrologueShape(0xb082)) throw new Error("sub sp,#n must not read as a push");
  if (isPrologueShape(0xbc60)) throw new Error("pop must not read as a push");
  // The lr discriminator, taken from the real trap: resource_3af's pool word
  // 0x0200b4bd decodes as `push {r0, r2, r3, r4, r5, r7}` at 0x02001654 and was
  // mistaken for the next function's prologue, under-measuring 0x02001444 by 48
  // bytes. No lr, so nothing can return through it.
  if (savesLinkRegister(0xb4bd)) throw new Error("a push without lr must not claim to save it");
  if (!savesLinkRegister(0xb560)) throw new Error("push {r5, r6, lr} saves lr");
  // Published-pointer resolution: an overlay function at image 0x3c88 is
  // published as 0x0200bc89 — offset + OVERLAY_BASE + 0x8000, Thumb bit set.
  if (publishedOffset(0x0200bc89, 0x5b2c) !== 0x3c88) throw new Error("published resolution is wrong");
  // An even word is data, not a function pointer.
  if (publishedOffset(0x0200bc88, 0x5b2c) !== null) throw new Error("an even word is not a function pointer");
  // Out of range must be rejected rather than clamped.
  if (publishedOffset(0x0200bc89, 0x100) !== null) throw new Error("out-of-range must be rejected");
  if (publishedOffset(0x00000001, 0x5b2c) !== null) throw new Error("a word below the link base must be rejected");
  console.log("self-test=ok");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const requested = args.filter((argument) => /^resource_[0-9a-f]+$/.test(argument));
  const overlays = requested.length > 0 ? requested : overlayNames();
  const results = overlays.map(sweep);
  if (args.includes("--json")) {
    console.log(JSON.stringify(results, null, 2));
    return;
  }
  let residue = 0;
  for (const result of results) {
    const unexplained = result.shaped.filter((row) => row.verdict.startsWith("UNEXPLAINED"));
    const unruled = result.shaped.filter((row) => row.verdict.startsWith("UNRULED"));
    if (
      result.called.length === 0 &&
      result.published.length === 0 &&
      unexplained.length === 0 &&
      unruled.length === 0
    )
      continue;
    residue += result.called.length + result.published.length + unexplained.length;
    console.log(result.overlay);
    for (const offset of result.called)
      console.log(`  A called    0x${(OVERLAY_BASE + offset).toString(16)}`);
    for (const offset of result.published)
      console.log(`  B published 0x${(OVERLAY_BASE + offset).toString(16)}`);
    for (const row of [...unexplained, ...unruled])
      console.log(
        `  C ${row.verdict.startsWith("UNRULED") ? "unruled " : "shaped  "}  ` +
          `0x${(OVERLAY_BASE + row.offset).toString(16)}  ${row.halfword.toString(16)}  ` +
          `nearest owner 0x${row.owner === null ? "?" : (OVERLAY_BASE + row.owner).toString(16)} +${row.delta}`,
      );
  }
  console.log(`\noverlays=${results.length} residue=${residue}`);
}

if (import.meta.main) main();
