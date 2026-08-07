#!/usr/bin/env bun
// Gate for SANCTUM.md, the sealed-owner ledger.
//
// A ledger nobody checks decays into folklore. Two failures matter and both are
// silent without this: an entry naming an owner that no longer exists, and an
// entry for an owner that has since gone exact. The second is the dangerous
// one -- it tells the next agent to skip work that is already done, or worse,
// to leave a solved owner unadopted.
//
//   bun tools/check/check_sanctum.ts            # gate the ledger
//   bun tools/check/check_sanctum.ts --queue    # owners with the compiler axis spent
//   bun tools/check/check_sanctum.ts --self-test
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
const LEDGER = join(ROOT, "SANCTUM.md");
const ENTRY = /^- `([A-Za-z0-9_]+)` floor=(\d+)hw axes=([a-z,]+) — (.+)$/;
const REQUIRED_AXES = ["compiler", "shape"] as const;

export type SealedEntry = { owner: string; floor: number; axes: string[]; reason: string };

// Only the `## Sealed` section is enforced. Prose elsewhere in the ledger names
// owners freely -- the queue section lists dozens -- and parsing those as
// entries would fail the gate on documentation.
export function parseSealed(markdown: string): SealedEntry[] {
  const start = markdown.indexOf("\n## Sealed");
  if (start < 0) throw new Error("SANCTUM.md has no '## Sealed' section");
  const rest = markdown.slice(start + 1);
  const end = rest.indexOf("\n## ", 1);
  const section = end < 0 ? rest : rest.slice(0, end);
  const entries: SealedEntry[] = [];
  for (const line of section.split("\n")) {
    if (!line.startsWith("- ")) continue;
    const match = ENTRY.exec(line.trim());
    if (!match) throw new Error(`malformed sanctum entry: ${line.trim()}`);
    entries.push({
      owner: match[1]!,
      floor: Number(match[2]),
      axes: match[3]!.split(","),
      reason: match[4]!,
    });
  }
  return entries;
}

function stems(directory: string): Set<string> {
  const path = join(ROOT, directory);
  if (!existsSync(path)) return new Set();
  return new Set(
    readdirSync(path).filter((name) => name.endsWith(".c")).map((name) => name.slice(0, -2)),
  );
}

export function violations(entries: readonly SealedEntry[], exact: Set<string>, semantic: Set<string>): string[] {
  const problems: string[] = [];
  const seen = new Set<string>();
  for (const entry of entries) {
    if (seen.has(entry.owner)) problems.push(`${entry.owner}: listed twice`);
    seen.add(entry.owner);
    if (exact.has(entry.owner)) {
      problems.push(`${entry.owner}: is byte-exact now; remove it from the ledger`);
    } else if (!semantic.has(entry.owner)) {
      problems.push(`${entry.owner}: names no owner under semantic/`);
    }
    for (const axis of REQUIRED_AXES) {
      if (!entry.axes.includes(axis)) problems.push(`${entry.owner}: ${axis} axis not exhausted; not sealable`);
    }
    if (entry.floor < 1) problems.push(`${entry.owner}: floor=${entry.floor}hw would mean exact`);
  }
  return problems;
}

// Owners whose compiler axis is spent but whose shape axis has never run. These
// are not sealed and must not be: they are the queue that feeds shape_sweep.
function queue(): { owner: string; floor: number }[] {
  const exact = stems("exact");
  const best = new Map<string, number>();
  const root = join(ROOT, "out", "modesweep");
  if (!existsSync(root)) return [];
  for (const directory of readdirSync(root)) {
    const path = join(root, directory, "floor.json");
    if (!existsSync(path)) continue;
    let record: Record<string, unknown>;
    try {
      record = JSON.parse(readFileSync(path, "utf8")) as Record<string, unknown>;
    } catch {
      continue;
    }
    if (record.bounded_search_complete !== true || record.exact === true) continue;
    const owner = typeof record.stem === "string" ? record.stem : null;
    const floor = record.irreducible_floor_halfwords;
    if (!owner || typeof floor !== "number" || floor < 1 || exact.has(owner)) continue;
    const previous = best.get(owner);
    if (previous === undefined || floor < previous) best.set(owner, floor);
  }
  return [...best].map(([owner, floor]) => ({ owner, floor })).sort((a, b) => a.floor - b.floor);
}

function selfTest(): void {
  const good = "\n## Sealed\n\n- `08011568` floor=2hw axes=compiler,shape — scheduler tie the reference wins\n\n## Next\n";
  const parsed = parseSealed(good);
  if (parsed.length !== 1 || parsed[0]?.owner !== "08011568" || parsed[0]?.floor !== 2) {
    throw new Error("parseSealed did not read a well-formed entry");
  }
  // Prose after the section must not be parsed as entries.
  if (parseSealed(good + "\n- `08099999` some prose bullet\n").length !== 1) {
    throw new Error("parseSealed read beyond its own section");
  }
  const entries = parseSealed(good);
  if (violations(entries, new Set(["08011568"]), new Set()).length === 0) {
    throw new Error("a sealed owner that is now exact must be a violation");
  }
  if (violations(entries, new Set(), new Set()).length === 0) {
    throw new Error("a sealed owner with no semantic source must be a violation");
  }
  if (violations(entries, new Set(), new Set(["08011568"])).length !== 0) {
    throw new Error("a well-formed sealed owner must pass");
  }
  const oneAxis = parseSealed("\n## Sealed\n\n- `08011568` floor=2hw axes=compiler — only flags tried\n");
  if (violations(oneAxis, new Set(), new Set(["08011568"])).length === 0) {
    throw new Error("sealing on one axis must be refused");
  }
  let threw = false;
  try {
    parseSealed("\n## Sealed\n\n- `08011568` missing the rest\n");
  } catch {
    threw = true;
  }
  if (!threw) throw new Error("a malformed entry must throw");
  console.log("check_sanctum self-test ok");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  if (args.includes("--queue")) {
    const pending = queue();
    for (const row of pending) console.log(`  ${row.owner} floor=${row.floor}hw`);
    console.log(`queue=${pending.length} owners with the compiler axis spent and the shape axis unrun`);
    return;
  }
  const entries = parseSealed(readFileSync(LEDGER, "utf8"));
  const problems = violations(entries, stems("exact"), stems("semantic"));
  if (problems.length > 0) {
    for (const problem of problems) console.error(`error: ${problem}`);
    process.exit(1);
  }
  console.log(`sanctum ok: ${entries.length} sealed owner(s)`);
}

if (import.meta.main) main();
