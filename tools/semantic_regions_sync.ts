#!/usr/bin/env bun
// Record the span of every admitted overlay semantic owner in
// `semantic/regions.json`, so the published coverage map can size the semantic
// lane.
//
// `tools/coverage_map.ts` sizes an overlay semantic owner ONLY from a
// `manual_regions` entry here. That is deliberate: the map is a
// tracked-evidence-only tool and refuses to read the decoded-region inventory,
// which is build output. An owner with no entry is reported in
// `provenance.semantic_unresolved` and contributes ZERO to the published
// figure — which is why hundreds of converted overlay owners were invisible.
//
// This tool derives each span from the overlay inventory and writes it into the
// tracked file, but it does NOT trust the inventory blindly. Publishing a span
// the branch cannot substantiate is worse than leaving an owner unresolved, so
// every candidate must pass three checks before it is written:
//
//   1. the row exists in the strict inventory and has a positive span;
//   2. the span lies inside the assembled overlay image;
//   3. the span does not overlap another admitted owner's span — overlapping
//      spans are merged by the map's `normalize`, so an overstated span would
//      silently inflate the published number.
//
// Anything that fails is listed and left out. Hand-written entries are never
// modified: this only ever appends.
//
//   bun tools/semantic_regions_sync.ts            # report what would change
//   bun tools/semantic_regions_sync.ts --write    # append the entries
//   bun tools/semantic_regions_sync.ts --self-test
import { existsSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { assembleOverlay } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const REGIONS = join(ROOT, "semantic", "regions.json");
const SOURCE_NAME = /^(resource_[0-9a-f]+)_c_(0200[0-9a-f]{4})\.c$/i;

interface ManualRegion {
  overlay: string;
  entry: string;
  span_bytes: number;
  evidence: string;
}

interface InventoryRow {
  overlay: string;
  offset: number;
  span_bytes: number;
  starts_with_prologue: boolean;
  returns: number;
  structural_veneer: boolean;
  data_walk: boolean;
  contained_by: unknown[];
}

/**
 * Say WHY a strict row is missing, instead of only that it is.
 *
 * THE ADOPTION DOOR IS PROLOGUE-KEYED TOO (2026-08-01, mars). The strict row
 * set is filtered on `starts_with_prologue`, and the inventory that produces it
 * is seeded on `push`-shaped halfwords in the first place -- so a LEAF has no
 * row here and never will. Four real ones hit this on the day the sweeps were
 * taught to stop keying on prologues: resource_395's 0x1838 and 0x1858,
 * resource_3cd's 0x07b8, resource_3a5's 0x1c78.
 *
 * The old message, "no strict inventory row for this owner", is true and
 * useless: it reads as "your reconstruction is unsubstantiated" when the real
 * statement is "this tool structurally cannot see this class of owner". That
 * is the difference between a refusal and a silent decline, and this file
 * exists because publishing a span the branch cannot substantiate is worse
 * than leaving an owner unresolved -- which cuts both ways. Naming the cause
 * costs one image read.
 *
 * It does NOT admit the row. A hand-written `manual_regions` entry with a
 * ROM-measured span is the sanctioned path, and this tool never modifies those.
 */
export function diagnoseMissingRow(
  overlay: string,
  offset: number,
  images: Map<string, number>,
): string {
  const assembly = join(ROOT, "assets", "code", `${overlay}_overlay.s`);
  if (!existsSync(assembly)) return "no strict inventory row for this owner, and no reconstruction assembly to read";
  let image: Uint8Array;
  try {
    image = assembleOverlay(assembly);
  } catch {
    return "no strict inventory row for this owner, and the overlay image would not assemble";
  }
  if (offset < 0 || offset + 2 > image.length)
    return "no strict inventory row for this owner, and the offset lies outside the overlay image";
  const opening = image[offset] | (image[offset + 1] << 8);
  if ((opening & 0xfe00) === 0xb400)
    return "no strict inventory row for this owner, though it DOES open with a push -- check returns, containment, veneer and data_walk flags on the inventory row";
  let returns = false;
  for (let at = offset; at < Math.min(image.length - 1, offset + 128); at += 2) {
    if ((image[at] | (image[at + 1] << 8)) === 0x4770) returns = true;
  }
  if (!returns)
    return "no strict inventory row for this owner, and no `bx lr` within 128 bytes -- these bytes may not be a function at all";
  return (
    "LEAF-CONSISTENT: opens with no push and a bx lr follows within 128 bytes, so the strict inventory -- which " +
    "is seeded on push-shaped halfwords and filtered on starts_with_prologue -- structurally CANNOT hold this " +
    "owner. That is a limit of this tool, NOT a doubt about the reconstruction: record it as a hand-written " +
    "manual_regions entry with a span measured from the ROM, which this tool never modifies. " +
    "IT IS A DIAGNOSIS AND NOT A VERDICT -- the bx lr it found may belong to a LATER function, so a literal " +
    "pool word diagnoses the same way (resource_395 0x1848 is one, and it reads LEAF-CONSISTENT off its twin\'s " +
    "return downstream). The offset still has to be defended from the ROM."
  );
}

/** Reject a span that runs past the image or collides with a claimed one. */
export function spanIsSubstantiated(
  start: number,
  span: number,
  imageBytes: number,
  claimed: { start: number; end: number }[],
): { ok: true } | { ok: false; reason: string } {
  if (!Number.isInteger(span) || span <= 0) return { ok: false, reason: "no positive span" };
  if (start + span > imageBytes)
    return { ok: false, reason: `span ends at 0x${(start + span).toString(16)} past image 0x${imageBytes.toString(16)}` };
  for (const other of claimed) {
    if (start < other.end && other.start < start + span)
      return {
        ok: false,
        reason: `overlaps claimed 0x${other.start.toString(16)}-0x${other.end.toString(16)}`,
      };
  }
  return { ok: true };
}

function readRegions(): { format: number; manual_regions: ManualRegion[] } {
  if (!existsSync(REGIONS)) return { format: 1, manual_regions: [] };
  return JSON.parse(readFileSync(REGIONS, "utf8"));
}

function inventoryRows(): InventoryRow[] {
  const path = join(ROOT, "out", "decomp", "overlays.json");
  if (!existsSync(path))
    throw new Error("out/decomp/overlays.json is missing; run a build before syncing");
  return (JSON.parse(readFileSync(path, "utf8")) as { functions: InventoryRow[] }).functions;
}

export interface SyncResult {
  added: ManualRegion[];
  rejected: { source: string; reason: string }[];
  alreadyRecorded: number;
}

export function planSync(): SyncResult {
  const existing = readRegions();
  const recorded = new Set(
    existing.manual_regions.map((row) => `${row.overlay}@${Number.parseInt(row.entry, 16)}`),
  );
  const rows = new Map<string, InventoryRow>();
  for (const row of inventoryRows()) {
    if (!row.starts_with_prologue || row.returns <= 0) continue;
    if (row.structural_veneer || row.data_walk) continue;
    if ((row.contained_by ?? []).length > 0) continue;
    rows.set(`${row.overlay}@${row.offset}`, row);
  }
  // Spans already claimed, per overlay: hand-written entries plus anything this
  // run accepts. Both must be respected or the map's normalize would merge them.
  const claimed = new Map<string, { start: number; end: number }[]>();
  for (const row of existing.manual_regions) {
    const start = Number.parseInt(row.entry, 16) - 0x02000000;
    claimed.set(row.overlay, [
      ...(claimed.get(row.overlay) ?? []),
      { start, end: start + row.span_bytes },
    ]);
  }
  const images = new Map<string, number>();
  const added: ManualRegion[] = [];
  const rejected: { source: string; reason: string }[] = [];
  let alreadyRecorded = 0;

  const directory = join(ROOT, "semantic", "overlays");
  for (const name of existsSync(directory) ? readdirSync(directory).sort() : []) {
    const match = SOURCE_NAME.exec(name);
    if (match === null) continue;
    const overlay = match[1];
    const address = Number.parseInt(match[2], 16);
    if (recorded.has(`${overlay}@${address}`)) {
      alreadyRecorded++;
      continue;
    }
    const offset = address - 0x02000000;
    const row = rows.get(`${overlay}@${offset}`);
    if (row === undefined) {
      rejected.push({ source: name, reason: diagnoseMissingRow(overlay, offset, images) });
      continue;
    }
    if (!images.has(overlay)) {
      const assembly = join(ROOT, "assets", "code", `${overlay}_overlay.s`);
      images.set(overlay, existsSync(assembly) ? assembleOverlay(assembly).length : 0);
    }
    const verdict = spanIsSubstantiated(
      offset,
      row.span_bytes,
      images.get(overlay) ?? 0,
      claimed.get(overlay) ?? [],
    );
    if (!verdict.ok) {
      rejected.push({ source: name, reason: verdict.reason });
      continue;
    }
    claimed.set(overlay, [
      ...(claimed.get(overlay) ?? []),
      { start: offset, end: offset + row.span_bytes },
    ]);
    added.push({
      overlay,
      entry: `0x${address.toString(16).padStart(8, "0")}`,
      span_bytes: row.span_bytes,
      evidence:
        "admitted whole-owner semantic reconstruction; span is the strict overlay " +
        "inventory row (prologue-started, returning, not a veneer or data walk, " +
        "not contained by another row), checked to lie inside the assembled " +
        "overlay image and not to overlap another recorded owner",
    });
  }
  return { added, rejected, alreadyRecorded };
}

function selfTest(): void {
  const image = 0x1000;
  if (!spanIsSubstantiated(0x100, 0x40, image, []).ok) throw new Error("a clean span must pass");
  const past = spanIsSubstantiated(0xfe0, 0x40, image, []);
  if (past.ok || !past.reason.includes("past image")) throw new Error("overrun must be rejected");
  const overlap = spanIsSubstantiated(0x100, 0x40, image, [{ start: 0x120, end: 0x180 }]);
  if (overlap.ok || !overlap.reason.includes("overlaps")) throw new Error("overlap must be rejected");
  // Touching spans are adjacent, not overlapping.
  if (!spanIsSubstantiated(0x100, 0x20, image, [{ start: 0x120, end: 0x180 }]).ok)
    throw new Error("adjacent spans must be allowed");
  if (spanIsSubstantiated(0x100, 0, image, []).ok) throw new Error("zero span must be rejected");
  // The diagnosis, in every direction it can go, on real overlay bytes. The
  // offsets are pinned to SHAPES that will not change (a leaf, a push, a pool
  // word, an out-of-range offset), not to whether the rows are drafted.
  if (!diagnoseMissingRow("resource_395", 0x1838, new Map()).startsWith("LEAF-CONSISTENT"))
    throw new Error("a leaf must be diagnosed as one, not merely declined");
  if (!diagnoseMissingRow("resource_395", 0x12f4, new Map()).includes("DOES open with a push"))
    throw new Error("a push-opening owner must not be reported as a leaf");
  if (!diagnoseMissingRow("resource_395", 0x99999, new Map()).includes("outside the overlay image"))
    throw new Error("an out-of-range offset must say so rather than guess");
  // The over-claim is KNOWN and stated in the message rather than hidden: a
  // pool word diagnoses LEAF-CONSISTENT off a later function's return. Pinned
  // so nobody 'fixes' the wording back into a verdict.
  if (!diagnoseMissingRow("resource_395", 0x1848, new Map()).includes("DIAGNOSIS AND NOT A VERDICT"))
    throw new Error("the leaf diagnosis must carry its own limit");
  console.log("self-test=ok");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const { added, rejected, alreadyRecorded } = planSync();
  for (const item of rejected) console.log(`  REJECT ${item.source}: ${item.reason}`);
  const bytes = added.reduce((sum, row) => sum + row.span_bytes, 0);
  console.log(
    `already_recorded=${alreadyRecorded} addable=${added.length} bytes=${bytes} rejected=${rejected.length}`,
  );
  if (!args.includes("--write")) {
    console.log("(pass --write to append)");
    return;
  }
  const existing = readRegions();
  existing.manual_regions = [...existing.manual_regions, ...added].sort(
    (left, right) =>
      left.overlay.localeCompare(right.overlay) ||
      Number.parseInt(left.entry, 16) - Number.parseInt(right.entry, 16),
  );
  writeFileSync(REGIONS, `${JSON.stringify(existing, null, 2)}\n`);
  console.log(`wrote ${added.length} entries to semantic/regions.json`);
}

if (import.meta.main) main();
