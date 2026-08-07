#!/usr/bin/env bun
// List semantic sources that an exact source at the same address has superseded.
//
// `build_semantic.ts` throws `duplicates exact source` when a semantic source
// and a canonical exact source share an address, and `verify` runs
// `build:semantic`, so every conversion that supersedes a semantic source
// breaks verification until that file is deleted. Noncanonical match scaffolds
// do not supersede the semantic owner. The error names one pair per run; this
// names all of them at once.
//
// Tracked-tree only: it compares file names, reads nothing from `out/`, and
// needs neither the ROM nor the toolchain, so it runs on any branch.
//
//   bun tools/semantic/semantic_superseded.ts             # print the paths, one per line
//   bun tools/semantic/semantic_superseded.ts --check     # exit 1 if any exist
//   bun tools/semantic/semantic_superseded.ts --self-test

import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
import { canonicalCSource } from "../lib/full_c_progress.ts";

const ROOT = resolve(dirname(new URL(import.meta.url).pathname), "../..");

// Each pair is (semantic directory, the exact directory that supersedes it).
// A semantic source is superseded when the exact directory holds the same file
// name -- both source trees name a file after the address it owns, so equal
// names mean equal addresses. `fromDisk` additionally requires canonical C.
// Main-image and overlay sources share one flat directory each since the
// exact/semantic tree consolidation, so this is a single pair now.
const SOURCE_PAIRS: readonly (readonly [string, string])[] = [
  ["semantic", "exact"],
];

export function supersededSources(
  listing: (directory: string) => readonly string[],
  present: (path: string) => boolean,
): string[] {
  const found: string[] = [];
  for (const [semantic, exact] of SOURCE_PAIRS) {
    for (const name of listing(semantic)) {
      if (!name.endsWith(".c")) continue;
      if (present(`${exact}/${name}`)) found.push(`${semantic}/${name}`);
    }
  }
  return found.sort();
}

function fromDisk(): string[] {
  return supersededSources(
    (directory) => {
      const path = join(ROOT, directory);
      return existsSync(path) ? readdirSync(path) : [];
    },
    (path) => {
      const exact = join(ROOT, path);
      return existsSync(exact) && canonicalCSource(readFileSync(exact, "utf8"));
    },
  );
}

function selfTest(): void {
  const listing = (directory: string): string[] =>
    directory === "semantic"
      ? ["resource_3aa_c_02000230.c", "resource_3aa_c_02000400.c", "notes.md", "08006d50.c"]
      : [];
  const present = (path: string): boolean =>
    path === "exact/resource_3aa_c_02000230.c" || path === "exact/08006d50.c";
  const found = supersededSources(listing, present);
  const expected = ["semantic/08006d50.c", "semantic/resource_3aa_c_02000230.c"];
  if (JSON.stringify(found) !== JSON.stringify(expected)) {
    throw new Error(`superseded self-test failed: ${JSON.stringify(found)}`);
  }
  // A semantic source with no exact counterpart is not superseded, and a
  // non-source file in the directory is never reported.
  if (supersededSources(listing, () => false).length !== 0) {
    throw new Error("superseded self-test reported a source with no exact counterpart");
  }
  console.log("self-test=ok");
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) {
    selfTest();
    return;
  }
  const found = fromDisk();
  for (const path of found) console.log(path);
  if (argv.includes("--check")) {
    console.log(`superseded=${found.length}`);
    if (found.length > 0) process.exitCode = 1;
  }
}

if (import.meta.main) main();
