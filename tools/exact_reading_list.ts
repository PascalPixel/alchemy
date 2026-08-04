#!/usr/bin/env bun
// Which semantic reconstruction explains each overlay row
// you have not yet matched byte-exactly.
//
// Semantic reconstruction already explains *what the code does* for most
// overlay owners. When converting one of those rows, the expensive part — reading assembly
// until the behaviour is clear — is already done and sitting in
// `semantic/`. This prints the pairing directly:
// every strict-queue owner that has a semantic source but no exact source, with
// the path to read and the row's size.
//
//   bun tools/exact_reading_list.ts                  # every overlay, ranked
//   bun tools/exact_reading_list.ts resource_373     # one overlay
//   bun tools/exact_reading_list.ts --json
//   bun tools/exact_reading_list.ts --blocked        # only the rows held back
//   bun tools/exact_reading_list.ts --self-test
//
// Ranking is by bytes-with-a-reference descending, because that is the axis on
// which reading someone else's reconstruction saves the most time.
//
// Rows whose start address is not inside an audited executable interval are held
// back: they compile, they adopt, and they reproduce the ROM byte-identically,
// but `full_c_progress --write-report` cannot then write its report, so the
// conversion has to be backed out whole. Offering such a row costs a full
// conversion to discover. See `startsInAuditedSpan` below.
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { isConvertibleRow } from "./semantic_regions_sync.ts";

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

export interface Interval {
  start: number;
  end: number;
}

export interface Pairing {
  overlay: string;
  address: string;
  bytes: number;
  semanticSource: string;
  /** True when the report would reject this row; see `startsInAuditedSpan`. */
  blocked: boolean;
}

/**
 * Strict-queue rows only: the same filter semantic reconstruction uses.
 *
 * "The same filter" is now literally true — it delegates to the ONE definition
 * in `semantic_regions_sync`. It was a hand-copied duplicate, and a duplicate
 * would have stayed prologue-keyed after the sync gate stopped being so: the
 * fix-one-branch-not-its-twin defect that has bitten this project repeatedly.
 * Two copies of a rule drift; one definition cannot.
 *
 * The prologue requirement that lived here was the door on the CONVERSION
 * QUEUE, and `overlay_inventory` states it outright — a widened entry "never
 * becomes a queue row itself". So a leaf could be discovered, ruled, and
 * drafted, and still never be offered as work.
 */
export function isStrictRow(row: Row): boolean {
  return isConvertibleRow(row);
}

/**
 * Whether a row's *start* address lands inside one of its overlay's audited
 * executable intervals.
 *
 * The intervals in `metrics/gs1-en-executable.json` do not tile their overlay:
 * small unclassified gaps sit between them, and a row that begins in one of
 * those gaps cannot be attributed by the report writer. Only the start matters
 * — a row that begins inside an interval and runs past its end is fine — which
 * is why this is not a containment test.
 */
export function startsInAuditedSpan(address: number, intervals: Interval[]): boolean {
  return intervals.some((interval) => interval.start <= address && address < interval.end);
}

function auditedIntervals(): Map<string, Interval[]> {
  const executable = JSON.parse(
    readFileSync(join(ROOT, "metrics", "gs1-en-executable.json"), "utf8"),
  ) as { overlays: { id: string; intervals: Interval[] }[] };
  return new Map(executable.overlays.map((overlay) => [overlay.id, overlay.intervals]));
}

export function readingList(): Pairing[] {
  const inventory = JSON.parse(
    readFileSync(join(ROOT, "out", "decomp", "overlays.json"), "utf8"),
  ) as { functions: Row[] };
  const semantic = new Set(
    existsSync(join(ROOT, "semantic"))
      ? readdirSync(join(ROOT, "semantic"))
      : [],
  );
  const exact = new Set(readdirSync(join(ROOT, "exact")));
  const audited = auditedIntervals();
  const pairings: Pairing[] = [];
  for (const row of inventory.functions) {
    if (!isStrictRow(row)) continue;
    const start = 0x02000000 + row.offset;
    const address = start.toString(16).padStart(8, "0");
    const base = `${row.overlay}_c_${address}.c`;
    // Already byte-exact: no remaining conversion work here.
    if (exact.has(base)) continue;
    if (!semantic.has(base)) continue;
    const intervals = audited.get(row.overlay);
    if (intervals === undefined) {
      throw new Error(
        `${row.overlay} is in the inventory but not in metrics/gs1-en-executable.json; ` +
          `the executable map is stale and every row's blocked flag would be a guess`,
      );
    }
    pairings.push({
      overlay: row.overlay,
      address: `0x${address}`,
      bytes: row.span_bytes,
      semanticSource: `semantic/${base}`,
      blocked: !startsInAuditedSpan(start, intervals),
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
  // A LEAF MUST BE A QUEUE ROW. This assertion used to demand the opposite —
  // "a prologue-less seed is not an owner" — which was the conversion queue's
  // prologue-keyed door written down as a test. A leaf returns; that is what
  // makes it a function.
  if (!isStrictRow({ ...base, starts_with_prologue: false }))
    throw new Error("a returning leaf must be a queue row");

  // The two intervals below are adjacent-but-not-touching, which is exactly the
  // shape that produced the five backed-out conversions: a two-byte gap at
  // 0x02000110..0x02000112.
  const intervals = [
    { start: 0x02000100, end: 0x02000110 },
    { start: 0x02000112, end: 0x02000200 },
  ];
  if (!startsInAuditedSpan(0x02000100, intervals))
    throw new Error("an interval's first byte is inside it");
  if (!startsInAuditedSpan(0x0200010e, intervals))
    throw new Error("a byte before an interval's end is inside it");
  if (startsInAuditedSpan(0x02000110, intervals))
    throw new Error("an interval's end byte is past it, not inside it");
  if (startsInAuditedSpan(0x02000111, intervals))
    throw new Error("a row starting in the gap is blocked");
  if (!startsInAuditedSpan(0x02000112, intervals))
    throw new Error("the byte after the gap is inside the next interval");
  if (startsInAuditedSpan(0x02000200, intervals))
    throw new Error("a row starting past the last interval is blocked");
  if (startsInAuditedSpan(0x02000100, []))
    throw new Error("an overlay with no audited intervals blocks everything");

  console.log("self-test=ok");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const only = args.find((argument) => /^resource_[0-9a-f]+$/.test(argument));
  const wantBlocked = args.includes("--blocked");
  const all = readingList();
  let list = all.filter((item) => item.blocked === wantBlocked);
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
  const held = all.filter((item) => item.blocked);
  console.log(
    `\noverlays=${ranked.length} owners=${list.length} bytes_with_a_reference=${total.toLocaleString()}`,
  );
  if (!wantBlocked && held.length > 0) {
    const heldBytes = held.reduce((sum, item) => sum + item.bytes, 0);
    console.log(
      `held back (start address not in an audited span, --blocked to list): ` +
        `owners=${held.length} bytes=${heldBytes.toLocaleString()}`,
    );
  }
}

if (import.meta.main) main();
