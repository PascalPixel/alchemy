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
      rejected.push({ source: name, reason: "no strict inventory row for this owner" });
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
