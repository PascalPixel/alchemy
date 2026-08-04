#!/usr/bin/env bun
// Classify the main image's unconverted reconstruction assembly.
//
// The remaining main-image work is not one pile. Some of it was never C -- the
// IWRAM-relocated ARM runtime, the linker's inter-mode veneers, the BIOS `svc`
// wrappers, and a set of routines that return through `ip` rather than `lr`.
// Counting those toward a conversion target makes the target unreachable by
// construction and sends whoever takes it at code that has no C form.
//
// This reports bytes per class and lists the convertible owners smallest-first,
// which is the order that has produced the highest first-probe rate. Byte
// counts come from the tracked executable inventory, attributed to an owner
// through the `asm/<stem>.s` reference each interval names, so this reads only
// tracked evidence and needs neither the ROM nor the toolchain.
//
//   bun tools/main_image_classes.ts            # per-class totals
//   bun tools/main_image_classes.ts --list N   # N smallest convertible owners
//   bun tools/main_image_classes.ts --self-test

import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join, resolve } from "node:path";

const ROOT = resolve(dirname(new URL(import.meta.url).pathname), "..");

export type OwnerClass =
  | "convertible-thumb"
  | "retained-asm"
  | "arm-runtime"
  | "returns-via-ip"
  | "linker-veneer"
  | "bios-svc";

// Order matters: the structural classes are tested before the fallback, and a
// veneer is only a veneer if it is not already something more specific.
export function classifyOwner(assembly: string): OwnerClass {
  const instructions = assembly.split("\n").filter((line) => /^\t[a-z]/.test(line));
  if (/^\s*\.arm\b/m.test(assembly)) return "arm-runtime";
  if (/\b(svc|swi)\b/.test(assembly)) return "bios-svc";
  // `mov ip, lr` paired with `bx ip` is a hand-rolled convention: the return
  // address is parked in a scratch register so an intervening `bl` can use lr.
  // No C compiler emits it, because C has no way to ask for it.
  if (/\bmov\s+ip,\s*lr\b/.test(assembly) && /\bbx\s+ip\b/.test(assembly)) return "returns-via-ip";
  if (instructions.length <= 3 && /\bbx\s+r/.test(assembly)) return "linker-veneer";
  return "convertible-thumb";
}

// Retention per owner, read from the tracked `asm/classification.json` that
// `build_asm` classifies against. Structural shape alone cannot tell a
// convertible row from a retained one: a `keep_structured_asm` owner is
// ordinary Thumb and looks exactly like a candidate. Counting those as
// convertible overstated the front and prioritized rows that are deliberately
// staying assembly.
//
// Only the default classification -- `c_candidate` -- is work. Everything the
// config names explicitly is a decision already taken.
export function retentionByStem(config: {
  structural?: { retention: string; files?: string[] }[];
  groups?: { retention: string; files?: string[] }[];
}): Map<string, string> {
  const retention = new Map<string, string>();
  for (const rule of [...(config.structural ?? []), ...(config.groups ?? [])]) {
    for (const stem of rule.files ?? []) retention.set(stem, rule.retention);
  }
  return retention;
}

// Bytes attributed to each owner, from the intervals that name its assembly.
export function ownerBytes(intervals: readonly { start: number; end: number; evidence?: string }[]): Map<string, number> {
  const bytes = new Map<string, number>();
  for (const interval of intervals) {
    const match = /asm\/(?:executable_gaps\/)?([0-9a-f]{8})\.s/.exec(interval.evidence ?? "");
    if (!match) continue;
    bytes.set(match[1], (bytes.get(match[1]) ?? 0) + (interval.end - interval.start));
  }
  return bytes;
}

function sourceStems(directory: string): Set<string> {
  if (!existsSync(directory)) return new Set();
  return new Set(readdirSync(directory)
    .filter((name) => /^08[0-9a-f]{6}\.c$/i.test(name))
    .map((name) => name.slice(0, 8).toLowerCase()));
}

function semanticCoverage(): {
  stems: Set<string>;
  executable: { start: number; end: number }[];
  nonC: { start: number; end: number }[];
} {
  const document = JSON.parse(readFileSync(join(ROOT, "semantic/main-regions.json"), "utf8")) as {
    main_owners: { executable_ranges: { address: string; size: number }[] }[];
    non_c_ranges?: { address: string; size: number }[];
  };
  const ranges = (items: { address: string; size: number }[]) => items.map((item) => ({
    start: Number.parseInt(item.address, 16),
    end: Number.parseInt(item.address, 16) + item.size,
  }));
  return {
    stems: sourceStems(join(ROOT, "semantic")),
    executable: ranges(document.main_owners.flatMap((owner) => owner.executable_ranges)),
    nonC: ranges(document.non_c_ranges ?? []),
  };
}

function survey(): {
  classes: Map<OwnerClass, { owners: number; bytes: number }>;
  convertible: { stem: string; bytes: number }[];
  totalNotC: number;
} {
  const inventory = JSON.parse(readFileSync(join(ROOT, "metrics/gs1-en-executable.json"), "utf8")) as {
    main: { intervals: { start: number; end: number; evidence?: string }[] };
  };
  const bytes = ownerBytes(inventory.main.intervals);
  const retention = retentionByStem(
    JSON.parse(readFileSync(join(ROOT, "asm/classification.json"), "utf8")),
  );
  const semantic = semanticCoverage();
  const classes = new Map<OwnerClass, { owners: number; bytes: number }>();
  const convertible: { stem: string; bytes: number }[] = [];
  for (const [stem, owned] of bytes) {
    if (existsSync(join(ROOT, "exact", `${stem}.c`))) continue;
    if (semantic.stems.has(stem)) continue;
    const address = Number.parseInt(stem, 16);
    const overlaps = (range: { start: number; end: number }) =>
      address < range.end && range.start < address + owned;
    if (semantic.executable.some(overlaps)) continue;
    const source = [
      join(ROOT, "asm", `${stem}.s`),
      join(ROOT, "asm", "executable_gaps", `${stem}.s`),
    ].find(existsSync);
    if (source === undefined) continue;
    const assembly = readFileSync(source, "utf8");
    const shape = classifyOwner(assembly);
    // The structural classes are findings about the code and outrank the
    // config; a retained row that is also ARM runtime is still ARM runtime.
    // Retention only overrides the convertible fallback.
    const held = retention.get(stem);
    const alignmentOnly = /^\s*\.2byte\s+0\s*$/m.test(assembly) &&
      !/^\s*[a-z][a-z0-9.]*\s/im.test(assembly);
    const explicitlyNonC = semantic.nonC.some(overlaps) || alignmentOnly;
    const kind: OwnerClass = shape === "convertible-thumb" &&
        (explicitlyNonC || (held !== undefined && held !== "c_candidate"))
      ? "retained-asm"
      : shape;
    const row = classes.get(kind) ?? { owners: 0, bytes: 0 };
    row.owners += 1;
    row.bytes += owned;
    classes.set(kind, row);
    if (kind === "convertible-thumb" && owned > 0) convertible.push({ stem, bytes: owned });
  }
  convertible.sort((a, b) => a.bytes - b.bytes || a.stem.localeCompare(b.stem));
  const coverage = JSON.parse(readFileSync(join(ROOT, "metrics/gs1-en-coverage-map.json"), "utf8")) as {
    main: { executable_bytes: number; exact_c_bytes: number; semantic_c_bytes: number };
  };
  if (coverage.main.executable_bytes !== inventory.main.executable_bytes)
    throw new Error("coverage map and executable inventory disagree; run bun run coverage");
  const totalNotC = coverage.main.executable_bytes - coverage.main.exact_c_bytes -
    coverage.main.semantic_c_bytes;
  return { classes, convertible, totalNotC };
}

function selfTest(): void {
  const cases: [string, OwnerClass][] = [
    ["\t.arm\n\tmov r0, #1\n", "arm-runtime"],
    ["\t.thumb\n\tmovs r0, #1\n\tsvc 25\n\tbx lr\n", "bios-svc"],
    ["\t.thumb\n\tmov ip, lr\n\tbl Func_1\n\tbx ip\n", "returns-via-ip"],
    ["\t.thumb\n\tldr r4, [pc, #0]\n\tbx r4\n", "linker-veneer"],
    ["\t.thumb\n\tpush {lr}\n\tmovs r3, #255\n\tsubs r3, #1\n\tbx lr\n", "convertible-thumb"],
  ];
  for (const [assembly, expected] of cases) {
    const actual = classifyOwner(assembly);
    if (actual !== expected) throw new Error(`classify self-test failed: expected ${expected}, got ${actual}`);
  }
  // A veneer that also returns through ip is the more specific class, and a
  // four-instruction body is past the veneer length even with a `bx`.
  if (classifyOwner("\t.thumb\n\tmov ip, lr\n\tbx ip\n") !== "returns-via-ip") {
    throw new Error("classify self-test failed: ip convention must outrank veneer length");
  }
  const bytes = ownerBytes([
    { start: 0, end: 10, evidence: "asm/08000770.s:runtime" },
    { start: 10, end: 30, evidence: "asm/08000770.s:runtime" },
    { start: 30, end: 40, evidence: "no owner here" },
  ]);
  if (bytes.get("08000770") !== 30 || bytes.size !== 1) {
    throw new Error(`ownerBytes self-test failed: ${JSON.stringify([...bytes])}`);
  }
  console.log("self-test=ok");
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) {
    selfTest();
    return;
  }
  const { classes, convertible, totalNotC } = survey();
  const rows = [...classes].sort((a, b) => b[1].bytes - a[1].bytes);
  let notC = 0;
  for (const [kind, row] of rows) {
    if (kind !== "convertible-thumb") notC += row.bytes;
    console.log(`${kind.padEnd(18)} ${String(row.owners).padStart(4)} owners  ${String(row.bytes).padStart(6)} bytes`);
  }
  if (!classes.has("convertible-thumb"))
    console.log(`${"convertible-thumb".padEnd(18)} ${String(0).padStart(4)} owners  ${String(0).padStart(6)} bytes`);
  const unattributed = totalNotC - notC;
  if (unattributed < 0) throw new Error(`classified retained bytes exceed the audited complement by ${-unattributed}`);
  if (unattributed > 0)
    console.log(`${"retained-unattributed".padEnd(18)} ${String(0).padStart(4)} owners  ${String(unattributed).padStart(6)} bytes`);
  console.log(`not-c-total        ${String(totalNotC).padStart(11)} bytes`);
  const listIndex = argv.indexOf("--list");
  if (listIndex >= 0) {
    const limit = Number(argv[listIndex + 1] ?? 20);
    for (const row of convertible.slice(0, limit)) console.log(`  ${row.stem}  ${row.bytes}`);
  }
}

if (import.meta.main) main();
