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

// Bytes attributed to each owner, from the intervals that name its assembly.
export function ownerBytes(intervals: readonly { start: number; end: number; evidence?: string }[]): Map<string, number> {
  const bytes = new Map<string, number>();
  for (const interval of intervals) {
    const match = /asm\/([0-9a-f]{8})\.s/.exec(interval.evidence ?? "");
    if (!match) continue;
    bytes.set(match[1], (bytes.get(match[1]) ?? 0) + (interval.end - interval.start));
  }
  return bytes;
}

function survey(): { classes: Map<OwnerClass, { owners: number; bytes: number }>; convertible: { stem: string; bytes: number }[] } {
  const inventory = JSON.parse(readFileSync(join(ROOT, "metrics/gs1-en-executable.json"), "utf8")) as {
    main: { intervals: { start: number; end: number; evidence?: string }[] };
  };
  const bytes = ownerBytes(inventory.main.intervals);
  const classes = new Map<OwnerClass, { owners: number; bytes: number }>();
  const convertible: { stem: string; bytes: number }[] = [];
  for (const name of readdirSync(join(ROOT, "asm"))) {
    if (!/^[0-9a-f]{8}\.s$/.test(name)) continue;
    const stem = name.slice(0, 8);
    if (existsSync(join(ROOT, "src", `${stem}.c`))) continue;
    const kind = classifyOwner(readFileSync(join(ROOT, "asm", name), "utf8"));
    const owned = bytes.get(stem) ?? 0;
    const row = classes.get(kind) ?? { owners: 0, bytes: 0 };
    row.owners += 1;
    row.bytes += owned;
    classes.set(kind, row);
    if (kind === "convertible-thumb" && owned > 0) convertible.push({ stem, bytes: owned });
  }
  convertible.sort((a, b) => a.bytes - b.bytes || a.stem.localeCompare(b.stem));
  return { classes, convertible };
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
  const { classes, convertible } = survey();
  const rows = [...classes].sort((a, b) => b[1].bytes - a[1].bytes);
  let notC = 0;
  for (const [kind, row] of rows) {
    if (kind !== "convertible-thumb") notC += row.bytes;
    console.log(`${kind.padEnd(18)} ${String(row.owners).padStart(4)} owners  ${String(row.bytes).padStart(6)} bytes`);
  }
  console.log(`not-c-total        ${String(notC).padStart(11)} bytes`);
  const listIndex = argv.indexOf("--list");
  if (listIndex >= 0) {
    const limit = Number(argv[listIndex + 1] ?? 20);
    for (const row of convertible.slice(0, limit)) console.log(`  ${row.stem}  ${row.bytes}`);
  }
}

if (import.meta.main) main();
