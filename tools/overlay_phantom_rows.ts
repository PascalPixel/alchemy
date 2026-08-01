#!/usr/bin/env bun
// Find inventory rows that are not functions at all.
//
// The overlay discovery pass seeded entries from `bl` targets decoded WITHOUT
// the overlay rule (`target = stored + 2`). Every such seed is off by two, and
// two bytes into a Thumb stream is a different instruction — so some rows in
// `out/decomp/overlays.json` describe addresses that sit in the middle of a
// body. `resource_379:00dc` is the worked example: listed as a 2,524-byte
// contained row, actually an `ldr r1,[pc,#944]` belonging to the owner at
// 0x0074, whose true `bl` (`f000 f839`, storing 0x072) points there.
//
// Recognising one takes three tests, and the third is the whole tool:
//
//   * the row is NOT a prologue;
//   * some `bl`'s pre-rule decode lands exactly on the row's offset; and
//   * that SAME call site's correct target is a real prologue.
//
// The two decodes differ by `site + 2`, not by a constant, so the real target
// can only come from the site that produced the stale one. Without the third
// test any `bl` whose stale decode happens to land on an interior row marks it
// phantom — which reported 76 phantoms in `resource_379` alone, more rows than
// the overlay has functions. With it, `resource_379` reports exactly one.
//
// Measured project-wide: 470 phantom rows, and **none of them are strict-queue
// rows** — every one is non-prologue or `contained_by` something, which the
// strict filter already excludes. The conversion queue's byte counts are sound.
//
//   bun tools/overlay_phantom_rows.ts              # every overlay
//   bun tools/overlay_phantom_rows.ts resource_379
//   bun tools/overlay_phantom_rows.ts --json
//   bun tools/overlay_phantom_rows.ts --self-test
//
// Rows are reported, never rewritten. The queue is measured from the inventory,
// so silently editing it would invalidate published counts.
import { readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { overlayImage, storedDisplacement, targetOffset } from "./overlay_call_targets.ts";
import { reachesReturn } from "./overlay_published.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface Row {
  overlay: string;
  offset: number;
  span_bytes: number;
  starts_with_prologue: boolean;
  contained_by?: unknown[];
}

export interface Phantom {
  overlay: string;
  offset: number;
  span_bytes: number;
  real_target: number;
  sites: number;
  /**
   * True when this row opens with ordinary work and reaches a `bx lr` — the
   * shape of a LEAF. A phantom verdict on such a row is not wrong, but it is
   * not safe to act on unread either, so it is MARKED rather than dropped.
   */
  leaf_shaped: boolean;
}

/**
 * The pre-rule decode of a BL pair: the displacement plus the branch's own pc,
 * which is what a stock disassembler prints and what the discovery pass
 * believed. Kept explicit so the off-by-two is visible rather than implied.
 */
export function preRuleTarget(site: number, high: number, low: number): number | null {
  const displacement = storedDisplacement(high, low);
  return displacement === null ? null : site + 4 + displacement;
}

export function phantomRows(rows: Row[], image: Uint8Array, overlay: string): Phantom[] {
  const prologues = new Set(rows.filter((row) => row.starts_with_prologue).map((row) => row.offset));
  // Pair each stale address with the address the SAME call site really names.
  // The two differ by `site + 2`, not by a constant — the pre-rule decode is
  // `site + 4 + displacement` while the true target is `displacement + 2` — so
  // the real target can only come from the site that produced the stale one.
  const stale = new Map<number, { real: number; sites: number }>();
  for (let at = 0; at + 3 < image.length; at += 2) {
    const high = image[at] | (image[at + 1] << 8);
    const low = image[at + 2] | (image[at + 3] << 8);
    const believed = preRuleTarget(at, high, low);
    const real = targetOffset(high, low);
    if (believed === null || real === null) continue;
    if (believed < 0 || believed >= image.length) continue;
    if (real < 0 || real >= image.length) continue;
    if (believed === real) continue;
    const existing = stale.get(believed);
    // Only count a stale address once per real target; two sites disagreeing
    // about the real target means this address is a coincidence, not a seed.
    if (existing !== undefined && existing.real !== real) {
      stale.set(believed, { real: -1, sites: existing.sites + 1 });
      continue;
    }
    stale.set(believed, { real, sites: (existing?.sites ?? 0) + 1 });
  }
  const phantoms: Phantom[] = [];
  for (const row of rows) {
    // A row the inventory itself calls a function start is not a phantom.
    //
    // PROLOGUE-KEYED, AND KNOWINGLY SO. A leaf opens with ordinary work, so it
    // is not in `prologues` and is not skipped here — meaning a genuine leaf
    // CAN be reported as a phantom. The fix is not to skip leaf-shaped rows:
    // most of them are interior fragments of the discovery walk, and three of
    // the five in the tree today are overlapping walks from different starts to
    // one common end (`resource_397` 0x16e/0x17c/0x18e all end at 0x1b4), so at
    // most one of the three is a function at all. Skipping them would hide real
    // phantoms to protect rows that are mostly not functions.
    //
    // So they are MARKED, not dropped. Two of the five open with the documented
    // leaf entry shapes — `ldr rN,[pc]` (0x4b0a) and `movs rN,#imm` (0x2000) —
    // and those deserve a human read before anyone acts on the verdict.
    if (row.starts_with_prologue || prologues.has(row.offset)) continue;
    const seed = stale.get(row.offset);
    if (seed === undefined || seed.real < 0) continue;
    // The decisive test: the same call site's CORRECT target must be a real
    // function start. Without this, any `bl` whose stale decode happens to land
    // on an interior row marks it phantom — which on `resource_379` alone
    // produced 76 false positives, more rows than the overlay has functions.
    //
    // This one stays prologue-keyed on MEASURED grounds, not by default: a leaf
    // target reaches a return, but so does an interior row of an enclosing
    // function, so relaxing it to a return test would re-admit exactly the 76.
    // Recorded here so the next reader does not have to rediscover the reason.
    if (!prologues.has(seed.real)) continue;
    phantoms.push({
      overlay,
      offset: row.offset,
      span_bytes: row.span_bytes,
      real_target: seed.real,
      sites: seed.sites,
      leaf_shaped: reachesReturn(image, row.offset),
    });
  }
  return phantoms;
}

function inventory(): Row[] {
  return (
    JSON.parse(readFileSync(join(ROOT, "out", "decomp", "overlays.json"), "utf8")) as { functions: Row[] }
  ).functions;
}

function selfTest(): void {
  // `f000 f839` at site 0 stores 0x072; the true target is 0x074 and the
  // pre-rule decode is 0x04 + 0x072 = 0x076 — the phantom, two bytes past.
  const high = 0xf000;
  const low = 0xf839;
  if (storedDisplacement(high, low) !== 0x072) throw new Error("stored displacement must be 0x072");
  if (preRuleTarget(0, high, low) !== 0x076) throw new Error("pre-rule decode must be 0x076");

  const image = new Uint8Array(0x200);
  image[0] = 0x00;
  image[1] = 0xf0;
  image[2] = 0x39;
  image[3] = 0xf8;
  const rows: Row[] = [
    { overlay: "t", offset: 0x074, span_bytes: 16, starts_with_prologue: true },
    { overlay: "t", offset: 0x076, span_bytes: 400, starts_with_prologue: false },
    { overlay: "t", offset: 0x100, span_bytes: 8, starts_with_prologue: false },
  ];
  // 0x074 must be a prologue for the phantom at 0x076 to be provable.
  image[0x074] = 0x00;
  image[0x075] = 0xb5;
  const found = phantomRows(rows, image, "t");
  if (found.length !== 1) throw new Error(`expected exactly one phantom, got ${found.length}`);
  if (found[0].offset !== 0x076) throw new Error("the phantom must be the mis-seeded row");
  if (found[0].real_target !== 0x074) throw new Error("the real target is two bytes back");
  // A non-prologue row nothing points at is an ordinary interior row.
  if (found.some((row) => row.offset === 0x100)) throw new Error("unreferenced rows are not phantoms");

  // The LEAF-SHAPED marker, both directions, on synthetic bytes. The image
  // above is all zeroes past the seed, so 0x076 reaches no `bx lr` and must NOT
  // be marked. Planting one within the window must flip it — and must NOT
  // change the verdict, because the marker annotates a phantom, it does not
  // withdraw one. A marker that silently dropped rows would hide real phantoms
  // to protect rows that are mostly interior fragments.
  if (found[0].leaf_shaped) throw new Error("a row with no return in range must not be marked leaf-shaped");
  const withReturn = new Uint8Array(image);
  withReturn[0x080] = 0x70;
  withReturn[0x081] = 0x47; // bx lr, inside the window from 0x076
  const marked = phantomRows(rows, withReturn, "t");
  if (marked.length !== 1) throw new Error("marking must not change how many phantoms are reported");
  if (!marked[0].leaf_shaped) throw new Error("a row reaching a return must be marked leaf-shaped");
  if (marked[0].offset !== 0x076) throw new Error("marking must not change which row is reported");

  console.log("self-test=ok (including the leaf-shaped marker, both directions)");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const only = args.find((argument) => /^resource_[0-9a-f]+$/.test(argument));
  const rows = inventory();
  const overlays = [...new Set(rows.map((row) => row.overlay))].filter(
    (overlay) => only === undefined || overlay === only,
  );
  const all: Phantom[] = [];
  for (const overlay of overlays) {
    let image: Uint8Array;
    try {
      image = overlayImage(overlay);
    } catch {
      continue; // no reconstruction assembly for this overlay yet
    }
    all.push(...phantomRows(rows.filter((row) => row.overlay === overlay), image, overlay));
  }
  if (args.includes("--json")) {
    console.log(JSON.stringify(all, null, 2));
    return;
  }
  for (const phantom of all.sort((left, right) => right.span_bytes - left.span_bytes)) {
    console.log(
      `${phantom.overlay}:${phantom.offset.toString(16).padStart(4, "0")}  ` +
        `${String(phantom.span_bytes).padStart(6)} bytes  ` +
        `real target 0x${phantom.real_target.toString(16)}  (${phantom.sites} stale site(s))` +
        (phantom.leaf_shaped ? "  LEAF-SHAPED — reaches a return; read it before acting" : ""),
    );
  }
  const bytes = all.reduce((sum, phantom) => sum + phantom.span_bytes, 0);
  console.log(`\nphantom_rows=${all.length} spanned_bytes=${bytes.toLocaleString()}`);
}

if (import.meta.main) main();
