#!/usr/bin/env bun
// Emit a DERIVED overlay inventory covering the semantic owners that the
// discovered inventory does not list, so the compiler-mode search can run on
// them.
//
// out/decomp/overlays.json is provenance: overlay_inventory.ts seeds it from
// control flow inside the stream, so a function whose only callers live in the
// main image or in an external pointer table is never discovered. 1102 of the
// 1337 semantic owners -- 82% of the corpus -- have no row there, and
// overlay_mode_cohort.ts refuses any candidate it cannot find in an inventory.
// The effect is that the repository's own compiler-mode search has never been
// able to look at the bulk of its own corpus.
//
// This does NOT touch the discovered inventory. It writes a separate file to
// pass via `overlay_mode_cohort.ts --inventory`, built from the same span
// source overlay_adopt.ts already trusts for these owners (alchemist's
// resolveSpan, which reads the registered span, not a guess). Keeping it
// separate is the point: a derived row is a convenience for searching, and
// must never be mistaken for evidence that a function was discovered.
//
// Ids are emitted in both the bare and 4-digit-zero-padded forms, because
// overlay_mode_cohort.ts requires at least four hex digits in the id it is
// given while the rest of the tooling writes the bare form. Emitting one form
// only makes every short-offset owner unreachable again, which is exactly the
// failure this tool exists to remove.
import { readdirSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { resolveSpan } from "./alchemist.ts";
import { OVERLAY_BASE } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface Row {
  id: string;
  overlay: string;
  entry: number;
  offset: number;
  span_bytes: number;
}

export function derivedRows(owners: Array<{ overlay: string; offset: number; span: number }>): Row[] {
  const rows: Row[] = [];
  for (const { overlay, offset, span } of owners) {
    const base = { overlay, entry: OVERLAY_BASE + offset, offset, span_bytes: span };
    const forms = new Set([offset.toString(16), offset.toString(16).padStart(4, "0")]);
    for (const form of forms) rows.push({ id: `${overlay}:${form}`, ...base });
  }
  return rows;
}

function collect(): Array<{ overlay: string; offset: number; span: number }> {
  const owners: Array<{ overlay: string; offset: number; span: number }> = [];
  for (const name of readdirSync(join(ROOT, "semantic")).sort()) {
    const matched = /^(resource_[0-9a-f]+)_c_([0-9a-f]{8})\.c$/i.exec(name);
    if (matched === null) continue;
    const overlay = matched[1];
    const offset = Number.parseInt(matched[2], 16) - OVERLAY_BASE;
    const span = resolveSpan(overlay, offset);
    if (span === undefined) continue; // no registered span: nothing to derive from
    owners.push({ overlay, offset, span });
  }
  return owners;
}

function selfTest(): void {
  const rows = derivedRows([{ overlay: "resource_3af", offset: 0xbf0, span: 56 }]);
  const ids = rows.map((row) => row.id).sort();
  if (ids.length !== 2 || ids[0] !== "resource_3af:0bf0" || ids[1] !== "resource_3af:bf0") {
    throw new Error(`expected both id forms, got ${JSON.stringify(ids)}`);
  }
  if (rows.some((row) => row.entry !== OVERLAY_BASE + 0xbf0 || row.span_bytes !== 56)) {
    throw new Error("derived row lost its entry or span");
  }
  // A 4-or-more digit offset must not be duplicated.
  const wide = derivedRows([{ overlay: "resource_3b1", offset: 0x48ac, span: 60 }]);
  if (wide.length !== 1 || wide[0].id !== "resource_3b1:48ac") {
    throw new Error(`wide offset should emit one id, got ${JSON.stringify(wide.map((r) => r.id))}`);
  }
  console.log("self-test=ok tool=derived_overlay_inventory");
}

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const outputFlag = Bun.argv.indexOf("--output");
  const output = outputFlag === -1 ? join(ROOT, "out", "derived-overlays.json") : Bun.argv[outputFlag + 1];
  const owners = collect();
  const rows = derivedRows(owners);
  writeFileSync(output, JSON.stringify({ functions: rows }));
  console.log(`derived=${output} owners=${owners.length} rows=${rows.length}`);
  console.log("pass to overlay_mode_cohort.ts --inventory; never merge into out/decomp/overlays.json");
}

if (import.meta.main) await main();
