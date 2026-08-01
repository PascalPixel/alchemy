#!/usr/bin/env bun
// Enumerate the overlay `mov pc, rN` dispatch sites against the BUILT IMAGE.
//
// Why this is a tool and not a shell loop: the first version of this census
// was a five-line `for r in "…"; do set -- $r; …` that reported 0 for every
// row, because zsh does not word-split an unquoted `$r` -- `overlay_show` ran
// on its defaults and the grep found nothing. A dead run and a clean run
// printed the same text (HANDOVER §5b5). The census also has to be independent
// of label positions in the `.s`: mapping sites to owners that way lands on the
// neighbouring owner and produced three rows that carry no dispatch at all.
//
// So: bytes from `assembleOverlay`, instruction boundaries from the inventory's
// `instruction_offsets`, and nothing inferred from source text. A site is a
// halfword matching the Thumb high-register form `mov pc, rN`,
// `(hw & 0xff87) === 0x4687` -- Rd = pc means H1 = 1 and the low three bits are
// 7. Cross-checked against `overlay_show` on five rows; all five agree.
//
// `--all` additionally scans every even offset, inventoried or not. That upper
// bound is where the "60 sites across 29 overlays" headline came from: 35 of
// the 60 are instructions inside an inventoried owner and 25 are in code with
// no inventory row. Uninventoried is not the same as absent -- `3b9:1a6e` is in
// that 25 and sits inside `3b9:1a4c`, which is closed byte-exact.
//
// It prints a header line ALWAYS, including the zero case, so "no sites" and
// "the scan did not run" cannot be confused for one another.
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { assembleOverlay, OVERLAY_BASE } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const MOV_PC_MASK = 0xff87;
const MOV_PC_FORM = 0x4687;

interface FunctionRow {
  id: string;
  overlay: string;
  offset: number;
  span_bytes: number;
  instruction_offsets: number[];
}

export function isMovPc(halfword: number): boolean {
  return (halfword & MOV_PC_MASK) === MOV_PC_FORM;
}

function selfTest(): void {
  // `mov pc, r3` is 0x469f and `mov pc, r8` is 0x46c7; `bx r3` (0x4718) and a
  // plain `mov r3, r8` (0x4643) must not match, or the census silently inflates.
  const expect = (halfword: number, want: boolean): void => {
    if (isMovPc(halfword) !== want) throw new Error(`isMovPc(0x${halfword.toString(16)}) !== ${want}`);
  };
  expect(0x469f, true);
  expect(0x46c7, true);
  expect(0x4687, true);
  expect(0x4718, false);
  expect(0x4643, false);
  expect(0x0000, false);
  console.log("self-test=ok tool=overlay_dispatch_sites");
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) { selfTest(); return; }
  const all = argv.includes("--all");
  const inventoryPath = join(ROOT, "out/decomp/overlays.json");
  if (!existsSync(inventoryPath)) {
    throw new Error(`missing ${inventoryPath}; run tools/overlay_inventory.ts first`);
  }
  const inventory = JSON.parse(readFileSync(inventoryPath, "utf8")) as { functions: FunctionRow[] };
  const byOverlay = new Map<string, FunctionRow[]>();
  for (const row of inventory.functions) {
    const list = byOverlay.get(row.overlay);
    if (list === undefined) byOverlay.set(row.overlay, [row]); else list.push(row);
  }

  const owners = new Map<string, { id: string; span: number; sites: number }>();
  const overlays = new Set<string>();
  const outside: string[] = [];
  let inventoried = 0;
  let scanned = 0;

  for (const name of readdirSync(join(ROOT, "assets/code")).sort()) {
    const match = /^(resource_[0-9a-f]+)_overlay\.s$/.exec(name);
    if (match === null) continue;
    const overlay = match[1];
    const rows = byOverlay.get(overlay) ?? [];
    let image: Buffer;
    try { image = assembleOverlay(join(ROOT, "assets/code", name), OVERLAY_BASE); }
    catch (error) { console.log(`skip overlay=${overlay} reason=${(error as Error).message}`); continue; }
    scanned++;

    const instructionSites = new Set<number>();
    const known = new Set<number>();
    for (const row of rows) {
      for (const step of row.instruction_offsets) {
        const at = row.offset + step;
        known.add(at);
        if (at + 1 < image.length && isMovPc(image.readUInt16LE(at))) instructionSites.add(at);
      }
    }
    for (const at of instructionSites) {
      overlays.add(overlay);
      inventoried++;
      let best: FunctionRow | null = null;
      for (const row of rows) {
        if (row.offset > at || at >= row.offset + row.span_bytes) continue;
        if (!row.instruction_offsets.includes(at - row.offset)) continue;
        if (best === null || row.span_bytes < best.span_bytes) best = row;
      }
      if (best === null) continue;
      const entry = owners.get(best.id) ?? { id: best.id, span: best.span_bytes, sites: 0 };
      entry.sites++;
      owners.set(best.id, entry);
    }
    if (!all) continue;
    for (let at = 0; at + 1 < image.length; at += 2) {
      if (!isMovPc(image.readUInt16LE(at))) continue;
      overlays.add(overlay);
      if (!known.has(at)) outside.push(`${overlay}:${at.toString(16)}`);
    }
  }

  const rows = [...owners.values()].sort((a, b) => a.span - b.span || a.id.localeCompare(b.id));
  const ownerBytes = rows.reduce((sum, row) => sum + row.span, 0);
  console.log(
    `dispatch_sites overlays_scanned=${scanned} sites_in_inventoried_owners=${inventoried}` +
    ` owners=${rows.length} owner_bytes=${ownerBytes} overlays_with_sites=${overlays.size}` +
    (all ? ` sites_outside_inventory=${outside.length}` : " (pass --all for the uninventoried count)"),
  );
  for (const row of rows) console.log(`${row.id}\tspan=${row.span}\tsites=${row.sites}`);
  if (all) for (const site of outside) console.log(`uninventoried\t${site}`);
}

if (import.meta.main) main();
