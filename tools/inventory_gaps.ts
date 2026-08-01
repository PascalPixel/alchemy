#!/usr/bin/env bun
// Report every span that is OWNED-OR-OWNABLE but sits outside the audited
// executable intervals.
//
// Why this exists. Twice on 2026-08-01 the executable audit was found to be
// missing a span, and both times it was discovered by ADOPTING into the hole:
// `full_c_progress` threw `C span is outside audited executable intervals`
// after the adoption had already been applied. That discovery method only
// fires when someone happens to seal that exact row, so the holes it finds are
// the ones we tripped over rather than the ones that exist.
//
// The four sources below are deliberately different in kind. Sources 1-3 are
// what the tree already OWNS; they answer "is the audit consistent with our
// own bookkeeping". Source 4 is the uninventoried population -- real code with
// no owner label and no C source -- and it is the ONLY one that can find a
// hole before it bites, because a hole under an unowned span is invisible to
// every consistency check until someone converts that row.
//
// Reports addresses and byte counts, never ROM bytes.
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { basename, dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const OVERLAY_BASE = 0x02000000;

export interface Interval { start: number; end: number }
export interface Ownable {
  namespace: string;
  owner: string;
  start: number;
  end: number;
  /** false when the span has no C owner yet -- the pre-emptive class. */
  owned: boolean;
}

/** Merge overlapping/adjacent intervals so containment is a single test. */
export function unionOf(intervals: readonly Interval[]): Interval[] {
  const sorted = [...intervals].sort((a, b) => a.start - b.start || a.end - b.end);
  const out: Interval[] = [];
  for (const interval of sorted) {
    const last = out[out.length - 1];
    if (last !== undefined && interval.start <= last.end) {
      last.end = Math.max(last.end, interval.end);
    } else out.push({ start: interval.start, end: interval.end });
  }
  return out;
}

function contained(span: Ownable, audited: readonly Interval[]): boolean {
  return audited.some((interval) => interval.start <= span.start && span.end <= interval.end);
}

/**
 * The whole check, as a pure function so it can be self-tested on synthetic
 * input rather than on whatever the repository happens to contain today.
 */
export function findGaps(
  audited: ReadonlyMap<string, readonly Interval[]>,
  ownables: readonly Ownable[],
): Ownable[] {
  const unions = new Map<string, Interval[]>();
  for (const [namespace, intervals] of audited) unions.set(namespace, unionOf(intervals));
  const gaps: Ownable[] = [];
  for (const span of ownables) {
    if (span.end <= span.start) throw new Error(`${span.owner}: empty span`);
    const union = unions.get(span.namespace);
    if (union === undefined || !contained(span, union)) gaps.push(span);
  }
  return gaps.sort((a, b) =>
    a.namespace.localeCompare(b.namespace) || a.start - b.start);
}

function readJson(path: string): unknown {
  return JSON.parse(readFileSync(path, "utf8"));
}

function auditedIntervals(inventoryPath: string): Map<string, Interval[]> {
  const inventory = readJson(inventoryPath) as {
    main: { intervals: Interval[] };
    overlays: Array<{ id: string; intervals: Interval[] }>;
  };
  const audited = new Map<string, Interval[]>();
  audited.set("main", inventory.main.intervals);
  for (const overlay of inventory.overlays) audited.set(overlay.id, overlay.intervals);
  return audited;
}

function collectOwnables(): Ownable[] {
  const ownables: Ownable[] = [];

  // 1. main: every assembled manifest row.
  const manifestPath = [join(ROOT, "out/full/asm/manifest.json"), join(ROOT, "out/asm/manifest.json")]
    .find(existsSync);
  if (manifestPath !== undefined) {
    const manifest = readJson(manifestPath) as {
      regions?: Array<{ address: number; size: number; source: string }>;
    };
    for (const region of manifest.regions ?? []) {
      if (region.size <= 0) continue;
      ownables.push({
        namespace: "main",
        owner: `manifest:${basename(region.source)}`,
        start: region.address,
        end: region.address + region.size,
        owned: true,
      });
    }
  }

  // 2. main: every registered semantic owner range.
  const regionsPath = join(ROOT, "semantic/main-regions.json");
  if (existsSync(regionsPath)) {
    const document = readJson(regionsPath) as {
      main_owners?: Array<{ entry: string; executable_ranges?: Array<{ address: string; size: number }> }>;
    };
    for (const owner of document.main_owners ?? []) {
      for (const range of owner.executable_ranges ?? []) {
        const start = Number.parseInt(range.address, 16);
        ownables.push({
          namespace: "main",
          owner: `semantic:${owner.entry}`,
          start,
          end: start + range.size,
          owned: true,
        });
      }
    }
  }

  // 3. overlays: every owner label the overlay assembly names.
  const code = join(ROOT, "assets/code");
  if (existsSync(code)) {
    for (const name of readdirSync(code).sort()) {
      if (!name.endsWith("_overlay.s")) continue;
      const namespace = name.slice(0, -"_overlay.s".length);
      const text = readFileSync(join(code, name), "utf8");
      for (const match of text.matchAll(/^(AlchemyC|AlchemyAsm)_([0-9a-f]{8}):/gm)) {
        const start = Number.parseInt(match[2], 16);
        ownables.push({
          namespace,
          owner: `${match[1]}:${match[2]}`,
          start,
          // A label alone does not give a length; two bytes is enough to
          // detect an owner whose ENTRY is outside the audit.
          end: start + 2,
          owned: true,
        });
      }
    }
  }

  // 4. overlays: the uninventoried population -- ownable, not yet owned.
  const shapes = join(ROOT, "out/decomp/overlay-shapes.json");
  if (existsSync(shapes)) {
    const document = readJson(shapes) as {
      all?: Array<{ overlay: string; offset: number; codeBytes: number }>;
    };
    for (const entry of document.all ?? []) {
      const start = OVERLAY_BASE + entry.offset;
      ownables.push({
        namespace: entry.overlay,
        owner: `unindexed:${start.toString(16)}`,
        start,
        end: start + entry.codeBytes,
        owned: false,
      });
    }
  }

  return ownables;
}

function selfTest(): void {
  // Synthetic, so the test does not rot when the repository's own spans move.
  const audited = new Map<string, Interval[]>([
    ["main", [{ start: 0x100, end: 0x200 }, { start: 0x200, end: 0x280 }]],
    ["overlay_a", [{ start: 0x1000, end: 0x1100 }]],
  ]);

  if (JSON.stringify(unionOf(audited.get("main")!)) !== JSON.stringify([{ start: 0x100, end: 0x280 }])) {
    throw new Error("unionOf must merge adjacent intervals");
  }

  const ownables: Ownable[] = [
    { namespace: "main", owner: "covered", start: 0x110, end: 0x120, owned: true },
    { namespace: "main", owner: "spans-the-join", start: 0x1f0, end: 0x210, owned: true },
    { namespace: "main", owner: "past-the-end", start: 0x270, end: 0x290, owned: true },
    { namespace: "overlay_a", owner: "unowned-hole", start: 0x1200, end: 0x1240, owned: false },
    { namespace: "overlay_b", owner: "missing-namespace", start: 0, end: 4, owned: true },
  ];
  const gaps = findGaps(audited, ownables);
  const names = gaps.map((gap) => gap.owner).sort();
  const expected = ["missing-namespace", "past-the-end", "unowned-hole"];
  if (JSON.stringify(names) !== JSON.stringify(expected)) {
    throw new Error(`self-test gap set mismatch: ${names.join(",")}`);
  }
  // A span that merely crosses an interval JOIN is contained once unioned.
  if (gaps.some((gap) => gap.owner === "spans-the-join")) {
    throw new Error("adjacent intervals must union before the containment test");
  }
  // An unowned hole must be reported even though nothing owns it yet: that is
  // the entire point of source 4.
  if (!gaps.some((gap) => gap.owner === "unowned-hole" && !gap.owned)) {
    throw new Error("unowned spans must be reported");
  }
  let threw = false;
  try {
    findGaps(audited, [{ namespace: "main", owner: "empty", start: 8, end: 8, owned: true }]);
  } catch { threw = true; }
  if (!threw) throw new Error("an empty span must be rejected");
  console.log("inventory_gaps self-test ok");
}

function main(argv: string[]): void {
  if (argv.includes("--self-test")) return selfTest();
  const flag = argv.indexOf("--inventory");
  const inventoryPath = flag >= 0 ? argv[flag + 1] : join(ROOT, "metrics/gs1-en-executable.json");
  const gaps = findGaps(auditedIntervals(inventoryPath), collectOwnables());
  const owned = gaps.filter((gap) => gap.owned);
  const unowned = gaps.filter((gap) => !gap.owned);
  const bytes = (spans: Ownable[]) => spans.reduce((sum, gap) => sum + (gap.end - gap.start), 0);
  console.log(
    `inventory=${basename(inventoryPath)} gaps=${gaps.length} ` +
    `owned=${owned.length}/${bytes(owned)}B unowned=${unowned.length}/${bytes(unowned)}B`,
  );
  for (const gap of gaps) {
    console.log(
      `  ${gap.owned ? "OWNED  " : "OWNABLE"} ${gap.namespace} ${gap.owner} ` +
      `0x${gap.start.toString(16)}..0x${gap.end.toString(16)} (${gap.end - gap.start})`,
    );
  }
  if (gaps.length > 0) process.exitCode = 1;
}

if (import.meta.main) main(Bun.argv.slice(2));
