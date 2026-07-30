#!/usr/bin/env bun
// For the exact lane: which semantic reconstruction explains each overlay row
// you have not yet matched byte-exactly.
//
// Venus has already worked out *what the code does* for most overlay owners.
// When Mercury attacks one of those rows, the expensive part — reading assembly
// until the behaviour is clear — is already done and sitting in
// `semantic/overlays/`. This prints the pairing so nobody has to hunt for it:
// every strict-queue owner that has a semantic source but no exact source, with
// the path to read and the row's size.
//
//   bun tools/exact_reading_list.ts                  # every overlay, ranked
//   bun tools/exact_reading_list.ts resource_373     # one overlay
//   bun tools/exact_reading_list.ts --json
//   bun tools/exact_reading_list.ts --self-test
//
// Ranking is by bytes-with-a-reference descending, because that is the axis on
// which reading someone else's reconstruction saves the most time.
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface Row {
  overlay: string;
  offset: number;
  span_bytes: number;
  starts_with_prologue: boolean;
  returns: number;
  structural_veneer: boolean;
  data_walk: boolean;
  contained_by: unknown[];
}

export interface Pairing {
  overlay: string;
  address: string;
  bytes: number;
  semanticSource: string;
}

/** Strict-queue rows only: the same filter the semantic lane converts from. */
export function isStrictRow(row: Row): boolean {
  return (
    row.starts_with_prologue &&
    row.returns > 0 &&
    !row.structural_veneer &&
    !row.data_walk &&
    (row.contained_by ?? []).length === 0
  );
}

export function readingList(): Pairing[] {
  const inventory = JSON.parse(
    readFileSync(join(ROOT, "out", "decomp", "overlays.json"), "utf8"),
  ) as { functions: Row[] };
  const semantic = new Set(
    existsSync(join(ROOT, "semantic", "overlays"))
      ? readdirSync(join(ROOT, "semantic", "overlays"))
      : [],
  );
  const exact = new Set(readdirSync(join(ROOT, "assets", "code")));
  const pairings: Pairing[] = [];
  for (const row of inventory.functions) {
    if (!isStrictRow(row)) continue;
    const address = (0x02000000 + row.offset).toString(16).padStart(8, "0");
    const base = `${row.overlay}_c_${address}.c`;
    // Already byte-exact: nothing for the exact lane to do here.
    if (exact.has(base)) continue;
    if (!semantic.has(base)) continue;
    pairings.push({
      overlay: row.overlay,
      address: `0x${address}`,
      bytes: row.span_bytes,
      semanticSource: `semantic/overlays/${base}`,
    });
  }
  return pairings;
}

function selfTest(): void {
  const base = {
    overlay: "resource_000",
    offset: 0,
    span_bytes: 16,
    starts_with_prologue: true,
    returns: 1,
    structural_veneer: false,
    data_walk: false,
    contained_by: [] as unknown[],
  };
  if (!isStrictRow(base)) throw new Error("a plain owner must be strict");
  if (isStrictRow({ ...base, data_walk: true })) throw new Error("data walks are not owners");
  if (isStrictRow({ ...base, returns: 0 })) throw new Error("a non-returning row is not an owner");
  if (isStrictRow({ ...base, contained_by: ["x"] }))
    throw new Error("a contained row is a fragment, not an owner");
  if (isStrictRow({ ...base, starts_with_prologue: false }))
    throw new Error("a prologue-less seed is not an owner");
  console.log("self-test=ok");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const only = args.find((argument) => /^resource_[0-9a-f]+$/.test(argument));
  let list = readingList();
  if (only !== undefined) list = list.filter((item) => item.overlay === only);
  if (args.includes("--json")) {
    console.log(JSON.stringify(list, null, 2));
    return;
  }
  const byOverlay = new Map<string, Pairing[]>();
  for (const item of list) byOverlay.set(item.overlay, [...(byOverlay.get(item.overlay) ?? []), item]);
  const ranked = [...byOverlay.entries()].sort(
    (left, right) =>
      right[1].reduce((sum, item) => sum + item.bytes, 0) -
      left[1].reduce((sum, item) => sum + item.bytes, 0),
  );
  for (const [overlay, items] of ranked) {
    const bytes = items.reduce((sum, item) => sum + item.bytes, 0);
    console.log(`${overlay}  ${bytes.toLocaleString()} bytes across ${items.length} owners`);
    if (only !== undefined) {
      for (const item of items.sort((a, b) => a.bytes - b.bytes)) {
        console.log(`    ${item.address}  ${String(item.bytes).padStart(6)}  ${item.semanticSource}`);
      }
    }
  }
  const total = list.reduce((sum, item) => sum + item.bytes, 0);
  console.log(
    `\noverlays=${ranked.length} owners=${list.length} bytes_with_a_reference=${total.toLocaleString()}`,
  );
}

if (import.meta.main) main();
