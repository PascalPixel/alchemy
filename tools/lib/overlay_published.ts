#!/usr/bin/env bun
// Find overlay functions that are installed by DATA rather than called by code.
//
// Why this exists. On 2026-08-01 `resource_3af` was declared closed after the
// two standard checks both returned zero: a whole-image `bl` sweep resolved
// with the +2 rule, and `tools/overlay/overlay_unindexed.ts`. Ten owners worth about
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
// Reported alongside it are the other sweeps a closure needs:
//
//   A  `bl` targets that land on a prologue and are not owned.
//   B  published pointers to prologues that are not owned   (this file's point)
//   B  leaf: published pointers whose target is NOT prologue-shaped
//   C  every prologue-shaped halfword that is not an owner start.
//
// THE `B leaf` CLASS WAS ADDED 2026-08-01 AND IS THE MORE IMPORTANT HALF.
// Until then this sweep required a `push` at the resolved target, so it found
// a published pointer, resolved it correctly, and then threw the answer away
// because the target did not open with a push. A LEAF NEVER DOES: it saves no
// register and returns with `bx lr`. The blind spot was never in what this
// sweep SCANS, it is in what this sweep ACCEPTS. Removing the gate surfaced
// 310 targets across 83 of 96 overlays, including two on resource_3b9 and two
// on resource_3a4 — both of which had been certified closed at residue 0.
//
// The prologue-confirmed count is UNCHANGED at 475 by that edit, which is the
// invariant proving the new class was added rather than the old one disturbed.
// Both lists must be empty to close an overlay.
//
// The typical leaf is four bytes of overlay boilerplate published from the
// overlay's own header table, and nearly every overlay has a pair:
//
//   movs r0, #0          / bx lr        return 0
//   ldr  r0, =0x0200939c / bx lr        return &Data_...
//
// This sweep still cannot see a function that NOTHING publishes and nothing
// calls — resource_3a4's 0x02003410 is the standing example. That half belongs
// to `tools/overlay/overlay_gaps.ts` (sweep D), which reads unaccounted bytes instead
// of hunting a signature. The two tools cover different halves; run both.
//
// C is NOISY BY DESIGN and must be CLASSIFIED, not zeroed. Two things make it
// fire legitimately: high-register spill pushes a few bytes inside a prologue
// (`push {r5, r6}` after `push {r5, r6, lr}`), and literal pool words that
// happen to look like a push. The cheap discriminator for the second is that a
// prologue you can return through must save `lr` — `resource_3af`'s pool word
// 0x0200b4bd decodes as `push {r0, r2, r3, r4, r5, r7}` and cannot be one.
//
//   bun tools/lib/overlay_published.ts                 # every overlay
//   bun tools/lib/overlay_published.ts resource_3af    # one overlay
//   bun tools/lib/overlay_published.ts --json
//   bun tools/lib/overlay_published.ts --self-test
import { existsSync, readdirSync, readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { classify, overlayImage, OVERLAY_BASE, BASE_SHIFT, targetOffset } from "./overlay_call_targets.ts";
import { overlayCSpans } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));

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
  /**
   * bl-reached targets with no owner that are NOT prologue-shaped: LEAVES.
   *
   * THE SAME DEFECT AS `publishedLeaf`, IN SWEEP A, TEN LINES ABOVE WHERE IT
   * WAS FIXED IN SWEEP B (found 2026-08-01). The call loop resolved every
   * `bl` correctly and then dropped any target that did not open with a `push`,
   * so a leaf called from inside its own overlay was found, discarded, and
   * never appeared in residue. A fix applied to one branch and not its sibling.
   *
   * Proven on `resource_3a5` `0x1c78`: FOUR call sites, a real 12-byte setter,
   * reported by `overlay_call_targets` as kind `unknown` and by this sweep as
   * nothing at all. Sweep A counted its callers and declined to call it a
   * function.
   *
   * Kept separate from `called` so the prologue-confirmed count stays a
   * regression invariant, exactly as `publishedLeaf` is kept separate from
   * `published`. BOTH must be empty to close an overlay.
   */
  calledLeaf: number[];
  /**
   * bl-reached targets with no owner that do not reach a return: NOT CODE.
   *
   * Relaxing the gate admits data along with leaves, so the leaf class earns
   * itself the same way the published one does -- a bounded scan for `bx lr`.
   * Reported rather than dropped, because quietly declining to say what was
   * seen is the fault being fixed. They do not count towards residue.
   */
  calledData: number[];
  /** published prologues with no owner. */
  published: number[];
  /**
   * Published pointers with no owner whose target is NOT prologue-shaped.
   *
   * These are LEAF functions. Until 2026-08-01 the published path required a
   * `push` at the target, which a leaf never has -- it saves no register and
   * returns with `bx lr`. The pointer was resolved correctly and then thrown
   * away by that one check, so the blind spot was never in what this sweep
   * SCANS, it is in what this sweep ACCEPTS.
   *
   * Kept separate from `published` rather than merged into it, so the
   * prologue-confirmed count stays a stable regression invariant. BOTH must be
   * empty to close an overlay.
   */
  publishedLeaf: number[];
  /**
   * Published pointers with no owner that are NOT code at all.
   *
   * The relaxed published path of 2026-08-01 admits any non-prologue target,
   * and a data table whose words happen to be odd is admitted with it. Five
   * such rows appeared tree-wide: four in resource_3c8 starting two bytes
   * apart -- which is one table, not four functions -- and one in
   * resource_3b7.
   *
   * The discriminator is one bounded scan: a published target that does not
   * reach a `bx lr` within RETURN_WINDOW bytes is data. They are REPORTED
   * rather than dropped, because an instrument that quietly declines to say
   * what it saw is the exact fault this sweep was relaxed to fix. They do
   * not count towards residue: there is no owner here to draft.
   */
  publishedData: number[];
  /** prologue-shaped halfwords with no owner, each classified. */
  shaped: { offset: number; halfword: number; owner: number | null; delta: number | null; verdict: string }[];
}

/**
 * How far a real leaf is allowed to run before it returns.
 *
 * 301 of the 306 rows the relaxed gate admitted reach a `bx lr` inside 128
 * bytes; the five that never do are data. The window is deliberately generous
 * -- the longest body in the cohort is the 54-byte record integrator -- so
 * that widening it further would not change the verdict on anything measured.
 */
export const RETURN_WINDOW = 128;

/** `bx lr`, the only return a leaf has. */
const BX_LR = 0x4770;

/**
 * True when `offset` reaches a `bx lr` within RETURN_WINDOW bytes.
 *
 * This is the whole discriminator between a leaf and a table of odd words.
 * It scans halfwords, so it does not care where instruction boundaries fall
 * -- data that happens to contain 0x4770 will pass, and that is the safe
 * direction to be wrong in: a false leaf is read by a human and ruled, a
 * false table is silently lost.
 */
export function reachesReturn(image: Uint8Array, offset: number, window = RETURN_WINDOW): boolean {
  const end = Math.min(image.length - 1, offset + window);
  for (let at = offset; at < end; at += 2) {
    if ((image[at] | (image[at + 1] << 8)) === BX_LR) return true;
  }
  return false;
}

/** Owners are whatever the tree already claims: a C file, or a manual region. */
export function ownerSet(overlay: string): Set<number> {
  const owners = new Set<number>();
  const pattern = new RegExp(`^${overlay}_c_0*([0-9a-f]+)\\.c$`);
  for (const directory of ["exact", "semantic"]) {
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
 * exact/ carry no span here — their extent is whatever the compiler
 * produces — so a candidate sitting behind an exact row cannot be ruled inside
 * a body by this data alone, and is reported as needing the owner's
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

/**
 * Real, compiled spans for this overlay's exact-C rows.
 *
 * These are not recorded in the tree -- an exact row's extent is whatever the
 * compiler produces -- so this tool used to have no way to tell an address
 * inside such a row from an address after it, and reported UNRULED. That
 * ambiguity is what hid the data-installed callbacks in the first place, so
 * closing it is worth the compile. `overlayCSpans` omits any row that fails to
 * build rather than guessing, and the omission simply leaves the old UNRULED
 * behaviour in place for that row, which is the safe direction.
 */
export function exactSpans(overlay: string): { start: number; end: number }[] {
  const source = join(ROOT, "assets", "code", `${overlay}_overlay.s`);
  if (!existsSync(source)) return [];
  return overlayCSpans(source);
}

/** Exact-C row start offsets, used only to name the rows a span is missing for. */
export function exactStarts(overlay: string): Set<number> {
  const starts = new Set<number>();
  const path = join(ROOT, "exact");
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
  const compiled = exactSpans(overlay);
  const measuredExactStart = new Set(compiled.map((span) => span.start));
  /**
   * Inside a body the tree measured, or inside an exact-C row this tool has now
   * compiled. Both are real spans; the second used to be invisible.
   */
  const insideRecordedBody = (offset: number): boolean =>
    bodies.some((body) => offset > body.start && offset < body.end) ||
    compiled.some((span) => offset > span.start && offset < span.end);
  /**
   * Behind an exact-C row whose span is STILL unknown -- that is, one
   * `overlayCSpans` could not compile. A row it did compile is now decided by
   * `insideRecordedBody` above, either way, so it is no longer UNRULED.
   */
  const behindExactRow = (offset: number): boolean => {
    let nearest: number | null = null;
    for (const start of sorted) {
      if (start <= offset) nearest = start;
      else break;
    }
    return nearest !== null && exact.has(nearest) && !measuredExactStart.has(nearest);
  };

  const called: number[] = [];
  const calledLeaf: number[] = [];
  const calledData: number[] = [];
  const seenCall = new Set<number>();
  for (let at = 0; at + 4 <= image.length; at += 2) {
    const target = targetOffset(image[at] | (image[at + 1] << 8), image[at + 2] | (image[at + 3] << 8));
    if (target === null || target < 0 || target >= image.length) continue;
    const kind = classify(image, target, new Set()).kind;
    // A veneer target is the import bank and is structure, not residue. It was
    // already excluded before, as part of the `!== "prologue"` drop; keeping it
    // excluded explicitly is the difference between a rule and a side effect.
    if (kind === "veneer" || kind === "call_via") continue;
    if (owners.has(target) || seenCall.has(target)) continue;
    if (insideRecordedBody(target)) continue;
    seenCall.add(target);
    // Same tri-class as the published path below, for the same reason: the
    // blind spot was never in what this sweep SCANS, it is in what it ACCEPTS.
    if (kind === "prologue") called.push(target);
    else if (reachesReturn(image, target)) calledLeaf.push(target);
    else calledData.push(target);
  }

  const published: number[] = [];
  const publishedLeaf: number[] = [];
  const publishedData: number[] = [];
  const seenPublish = new Set<number>();
  for (let at = 0; at + 4 <= image.length; at += 4) {
    const word =
      (image[at] | (image[at + 1] << 8) | (image[at + 2] << 16) | (image[at + 3] << 24)) >>> 0;
    const offset = publishedOffset(word, image.length);
    if (offset === null) continue;
    if (owners.has(offset) || seenPublish.has(offset)) continue;
    if (insideRecordedBody(offset)) continue;
    seenPublish.add(offset);
    // A prologue-shaped target is the long-standing case. A target that is NOT
    // prologue-shaped used to be dropped here, and that is precisely how a leaf
    // hid: something published its address, and this sweep resolved it and then
    // refused it for not opening with a `push`.
    if (isPrologueShape(image[offset] | (image[offset + 1] << 8))) published.push(offset);
    // ...but relaxing the gate admits data as well as leaves, so the leaf
    // class now has to earn itself: a real leaf returns, and it returns soon.
    else if (reachesReturn(image, offset)) publishedLeaf.push(offset);
    else publishedData.push(offset);
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

  return { overlay, called, calledLeaf, calledData, published, publishedLeaf, publishedData, shaped };
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
  // Exact-C spans, the limitation this tool shipped with. resource_380's exact
  // row at 0x02000390 compiles to 868 bytes and therefore ends exactly at
  // 0x020006f4, where a published callback begins. Before the spans were
  // readable the tool could not tell those two apart and reported UNRULED, so
  // this asserts BOTH halves of the fix: the span is produced at all, and it
  // decides 0x020006f4 to be outside rather than inside.
  const spans = exactSpans("resource_380");
  const row = spans.find((span) => span.start === 0x390);
  if (row === undefined) throw new Error("resource_380's exact row at 0x02000390 must compile to a span");
  if (row.end !== 0x6f4) throw new Error(`exact row 0x02000390 must end at 0x020006f4, got 0x${row.end.toString(16)}`);
  if (spans.some((span) => 0x6f4 > span.start && 0x6f4 < span.end)) {
    throw new Error("0x020006f4 is a row start, not an interior address");
  }
  // A row that fails to compile must be omitted, never guessed at, so a start
  // with no span has to stay absent rather than appear with a zero length.
  if (spans.some((span) => span.end <= span.start)) throw new Error("an empty span is a guess, not a measurement");
  // The verdict string is UPPERCASE. An earlier draft of this check compared
  // against "unruled" and therefore matched nothing and passed vacuously --
  // which is the whole failure mode this tool exists to catch, reproduced
  // inside its own self-test.
  const unruled = sweep("resource_380").shaped.filter((row) => row.verdict.startsWith("UNRULED"));
  if (unruled.length !== 0) {
    throw new Error(`resource_380 must have no UNRULED rows left, got ${unruled.length}`);
  }
  // The bounded-window discriminator, asserted on SYNTHETIC bytes rather than
  // on today's residue -- a check pinned to the rows currently in the tree
  // rots the moment they are drafted.
  const getter = new Uint8Array(64);
  getter[0] = 0x00; getter[1] = 0x48;            /* ldr r0, [pc, #0] */
  getter[2] = 0x70; getter[3] = 0x47;            /* bx lr            */
  if (!reachesReturn(getter, 0)) throw new Error("a getter returns at +2 and must read as a leaf");
  const table = new Uint8Array(64);              /* no 0x4770 anywhere */
  if (reachesReturn(table, 0)) throw new Error("a table with no return must read as data");
  // The window has to BE a window. A return past it does not rescue the row,
  // which is the difference between this check and 'is there a bx lr later'.
  const far = new Uint8Array(64);
  far[40] = 0x70; far[41] = 0x47;
  if (reachesReturn(far, 0, 16)) throw new Error("a return past the window must not count");
  if (!reachesReturn(far, 0, 64)) throw new Error("a return inside the window must count");
  // And it must not run off the end of the image looking for one.
  const short = new Uint8Array(4);
  short[2] = 0x70; short[3] = 0x47;
  if (!reachesReturn(short, 0, 1024)) throw new Error("a return at the last halfword must count");
  if (reachesReturn(new Uint8Array(4), 0, 1024)) throw new Error("running off the end must not invent a return");

  // SWEEP A'S TRI-CLASS, asserted the way sweep B's is: the prologue-confirmed
  // count is the regression invariant, and the leaf class must be ADDED rather
  // than the old one disturbed. Measured on the tree at the moment of the edit:
  // `A called` 224 and `B published` 464 both UNCHANGED, residue 1597 -> 1607,
  // and the difference is exactly the ten `A leaf` rows. Asserted here on
  // synthetic bytes so it does not rot as those ten get drafted.
  //
  // The three shapes a bl target can wear, at 0x20 in a synthetic image:
  //   a `push {lr}` prologue  -> called
  //   `ldr r0,=X / bx lr`     -> calledLeaf  (a leaf: no push, returns)
  //   a table with no return  -> calledData  (not code)
  const kindOf = (opening: number[]): string => {
    const image = new Uint8Array(0x60);
    opening.forEach((halfword, index) => {
      image[0x20 + index * 2] = halfword & 0xff;
      image[0x20 + index * 2 + 1] = halfword >> 8;
    });
    if (classify(image, 0x20, new Set()).kind === "prologue") return "called";
    return reachesReturn(image, 0x20) ? "calledLeaf" : "calledData";
  };
  if (kindOf([0xb500, 0x4770]) !== "called")
    throw new Error("sweep A: a push prologue must stay in `called` -- that count is the invariant");
  if (kindOf([0x4800, 0x4770]) !== "calledLeaf")
    throw new Error("sweep A: `ldr r0,=X / bx lr` is a LEAF and must not be dropped");
  if (kindOf([0x0000, 0x0000]) !== "calledData")
    throw new Error("sweep A: bytes that never return are data, and must be reported as such");
  // And the direction that matters most: the leaf must not be silently absent.
  // Before 2026-08-01 this shape produced NOTHING at all -- not a leaf, not
  // data, not a complaint. A tri-class that can still return nothing is the
  // same defect wearing three names.
  for (const shape of [[0xb500, 0x4770], [0x4800, 0x4770], [0x0000, 0x0000]])
    if (!["called", "calledLeaf", "calledData"].includes(kindOf(shape)))
      throw new Error("sweep A: every resolved bl target must land in exactly one class");

  console.log("self-test=ok");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  // A NAME IS REJECTED BY EXISTENCE, NOT BY SHAPE (fixed 2026-08-01).
  // This filter used to be `/^resource_[0-9a-f]+$/` with a fall-back to every
  // overlay when nothing matched, so `resource_zzz` -- a typo, a stale name, a
  // name from another game -- silently swept all 96 and exited 0. It could not
  // fake a certification, because the fall-back prints residue=1602 rather than
  // 0, but `overlays=1` was the only line in the output telling you your name
  // had matched anything. HANDOVER's claim that "sweep A/B/C already refused an
  // unknown name" was false as measured; only a WELL-FORMED absent name threw.
  // Seventh sighting of the fault living in what a tool ACCEPTS.
  const known = new Set(overlayNames());
  const requested = args.filter((argument) => !argument.startsWith("--"));
  for (const name of requested)
    if (!known.has(name)) {
      console.log(`NOTHING SWEPT — no overlay named ${name}. This is a FAILURE, not a pass.`);
      process.exit(1);
    }
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
      result.calledLeaf.length === 0 &&
      result.calledData.length === 0 &&
      result.published.length === 0 &&
      result.publishedLeaf.length === 0 &&
      result.publishedData.length === 0 &&
      unexplained.length === 0 &&
      unruled.length === 0
    )
      continue;
    residue +=
      result.called.length +
      result.calledLeaf.length +
      result.published.length +
      result.publishedLeaf.length +
      unexplained.length;
    console.log(result.overlay);
    for (const offset of result.called)
      console.log(`  A called    0x${(OVERLAY_BASE + offset).toString(16)}`);
    for (const offset of result.calledLeaf)
      console.log(`  A leaf      0x${(OVERLAY_BASE + offset).toString(16)}  bl-reached, no push prologue`);
    for (const offset of result.calledData)
      console.log(
        `  A data      0x${(OVERLAY_BASE + offset).toString(16)}  ` +
          `bl-reached, no push prologue, NO RETURN within ${RETURN_WINDOW} bytes -- RULED DATA, not residue`,
      );
    for (const offset of result.published)
      console.log(`  B published 0x${(OVERLAY_BASE + offset).toString(16)}`);
    for (const offset of result.publishedLeaf)
      console.log(`  B leaf      0x${(OVERLAY_BASE + offset).toString(16)}  published, no push prologue`);
    for (const offset of result.publishedData)
      console.log(
        `  B data      0x${(OVERLAY_BASE + offset).toString(16)}  ` +
          `published, no push prologue, NO RETURN within ${RETURN_WINDOW} bytes -- RULED DATA, not residue`,
      );
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
