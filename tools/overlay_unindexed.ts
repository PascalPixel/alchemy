#!/usr/bin/env bun
// Find overlay functions that exist in the ROM and in no index.
//
// The overlay inventory is incomplete. This scan finds what it misses, by
// REFERENCE rather than by structure: every `bl` in an overlay image is
// resolved with the overlay rule (`target = stored + 2`), and any target whose
// first halfword is a `push {…, lr}` is a function start. If that address has
// no inventory row, no semantic source and no exact source, and does not fall
// strictly inside a known row's span, then it is a real function nobody has.
//
// Measured when this was written: **422 such functions**, every one provably
// called. `resource_3b1` alone holds 44, including one called 179 times and
// another 69 times. 15 more sit strictly inside a known span — those are
// genuine interior functions, already covered by their owner, and are reported
// separately rather than counted.
//
// Why by reference and not by structure: a two-byte-gap sweep over the audit
// intervals found only 36 of the 422, because it can only see functions whose
// prologue happens to land in an interval gap. A transposition hunt then found
// one more by content that the gap sweep had missed. Being called is the
// property that actually proves a function exists.
//
// **This is the scan that must be used to check a completeness claim.** Of 68
// overlays declared "converted in full" against the inventory, 28 hold 204 of
// these. The claim was true about the queue and false about the overlay.
//
//   bun tools/overlay_unindexed.ts                    # every overlay, ranked
//   bun tools/overlay_unindexed.ts resource_3b1       # one overlay, with call counts
//   bun tools/overlay_unindexed.ts --json
//   bun tools/overlay_unindexed.ts --self-test
import { readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { overlayImage, targetOffset } from "./overlay_call_targets.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

/** `push {…}` is 0xb4xx and `push {…, lr}` is 0xb5xx — one mask covers both. */
export function isPrologue(halfword: number): boolean {
  return (halfword & 0xfe00) === 0xb400;
}

export interface Found {
  overlay: string;
  offset: number;
  calls: number;
  /** Strictly inside a known row's span: an interior function, not missing work. */
  interior: boolean;
}

interface Row {
  overlay: string;
  offset: number;
  span_bytes: number;
}

/**
 * Scan one image. `known` are inventory offsets, `spans` their extents, and
 * `converted` the addresses that already have a source of either kind.
 */
export function scanImage(
  image: Uint8Array,
  known: Set<number>,
  spans: Array<[number, number]>,
  converted: Set<number>,
  overlay = "",
): Found[] {
  const calls = new Map<number, number>();
  for (let at = 0; at + 3 < image.length; at += 2) {
    const target = targetOffset(image[at] | (image[at + 1] << 8), image[at + 2] | (image[at + 3] << 8));
    if (target === null || target < 0 || target + 1 >= image.length) continue;
    if (!isPrologue(image[target] | (image[target + 1] << 8))) continue;
    calls.set(target, (calls.get(target) ?? 0) + 1);
  }
  const found: Found[] = [];
  for (const [offset, count] of calls) {
    if (known.has(offset) || converted.has(offset)) continue;
    // `>` and `<` deliberately: a target exactly AT a span's start is that row,
    // and one at its end belongs to the next.
    const interior = spans.some(([low, high]) => offset > low && offset < high);
    found.push({ overlay, offset, calls: count, interior });
  }
  return found.sort((left, right) => right.calls - left.calls);
}

function inventory(): Row[] {
  return (
    JSON.parse(readFileSync(join(ROOT, "out", "decomp", "overlays.json"), "utf8")) as {
      functions: Row[];
    }
  ).functions;
}

export function scanAll(only?: string): Found[] {
  const rows = inventory();
  const semantic = new Set(readdirSync(join(ROOT, "semantic")));
  const exact = new Set(readdirSync(join(ROOT, "exact")));
  const byOverlay = new Map<string, Row[]>();
  for (const row of rows) byOverlay.set(row.overlay, [...(byOverlay.get(row.overlay) ?? []), row]);

  const found: Found[] = [];
  for (const [overlay, overlayRows] of byOverlay) {
    if (only !== undefined && overlay !== only) continue;
    let image: Uint8Array;
    try {
      image = overlayImage(overlay);
    } catch {
      continue; // no reconstruction assembly for this overlay
    }
    const converted = new Set<number>();
    for (let offset = 0; offset < image.length; offset += 2) {
      const address = (0x02000000 + offset).toString(16).padStart(8, "0");
      const name = `${overlay}_c_${address}.c`;
      if (semantic.has(name) || exact.has(name)) converted.add(offset);
    }
    found.push(
      ...scanImage(
        image,
        new Set(overlayRows.map((row) => row.offset)),
        overlayRows.map((row) => [row.offset, row.offset + (row.span_bytes ?? 0)] as [number, number]),
        converted,
        overlay,
      ),
    );
  }
  return found;
}

function selfTest(): void {
  if (!isPrologue(0xb5e0)) throw new Error("push {r5,r6,r7,lr} is a prologue");
  if (!isPrologue(0xb500)) throw new Error("push {lr} is a prologue");
  if (!isPrologue(0xb4f0)) throw new Error("a bare push is a prologue");
  if (isPrologue(0x4770)) throw new Error("bx lr is not a prologue");
  if (isPrologue(0xb084)) throw new Error("sub sp,#16 is not a prologue");

  // A `bl` at 0 storing 0x0e resolves to 0x10 under the +2 rule; put a prologue
  // there and the scan must find it, with the call counted.
  const image = new Uint8Array(0x40);
  image[0] = 0x00;
  image[1] = 0xf0;
  image[2] = 0x07;
  image[3] = 0xf8;
  if (targetOffset(0xf000, 0xf807) !== 0x10) throw new Error("the +2 rule must give 0x10");
  image[0x10] = 0xe0;
  image[0x11] = 0xb5;

  const bare = scanImage(image, new Set(), [], new Set());
  if (bare.length !== 1 || bare[0].offset !== 0x10) throw new Error("an unindexed callee must be found");
  if (bare[0].calls !== 1) throw new Error("the call must be counted");
  if (bare[0].interior) throw new Error("with no spans nothing is interior");

  if (scanImage(image, new Set([0x10]), [], new Set()).length !== 0)
    throw new Error("a known row must be dropped");
  if (scanImage(image, new Set(), [], new Set([0x10])).length !== 0)
    throw new Error("an already-converted address must be dropped");

  const inside = scanImage(image, new Set(), [[0x08, 0x20]], new Set());
  if (!inside[0].interior) throw new Error("a target inside a span is interior");
  const atStart = scanImage(image, new Set(), [[0x10, 0x20]], new Set());
  if (atStart[0].interior) throw new Error("a target AT a span start is that row, not interior");
  console.log("self-test=ok");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const only = args.find((argument) => /^resource_[0-9a-f]+$/.test(argument));
  const found = scanAll(only);
  const missing = found.filter((entry) => !entry.interior);
  const interior = found.filter((entry) => entry.interior);

  if (args.includes("--json")) {
    console.log(JSON.stringify(missing, null, 2));
    return;
  }
  if (only !== undefined) {
    for (const entry of missing) {
      console.log(
        `  0x${(0x02000000 + entry.offset).toString(16).padStart(8, "0")}  called ${entry.calls}x`,
      );
    }
  } else {
    const byOverlay = new Map<string, number>();
    for (const entry of missing) byOverlay.set(entry.overlay, (byOverlay.get(entry.overlay) ?? 0) + 1);
    for (const [overlay, count] of [...byOverlay].sort((left, right) => right[1] - left[1])) {
      console.log(`${overlay}  ${count}`);
    }
  }
  console.log(
    `\nunindexed_called_functions=${missing.length} interior_already_covered=${interior.length}`,
  );
}

if (import.meta.main) main();
