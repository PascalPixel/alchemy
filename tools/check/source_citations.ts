#!/usr/bin/env bun
// A tool path cited in a reconstructed source must resolve, or be a recorded
// retirement.
//
// Reconstructed sources cite the tool that derived a fact. That is provenance,
// not decoration, so a citation naming a file that no longer exists breaks the
// trail. A restructure broke 802 of them across 14 tool paths and every gate
// stayed green: brokenDocPaths checks documents, and nothing checked source.
//
// Repointing a citation at a different surviving tool is forbidden and this
// cannot detect it -- it would assert a derivation that never happened. When a
// cited tool is deleted, record it in PROVENANCE.md instead.
//
//   bun tools/check/source_citations.ts
//   bun tools/check/source_citations.ts --self-test
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
const CITATION = /tools\/[a-z0-9_/]+\.ts/g;

export function retiredTools(provenance: string): Set<string> {
  const retired = new Set<string>();
  for (const [, path] of provenance.matchAll(/\|\s*`(tools\/[a-z0-9_/]+\.ts)`\s*\|/g)) retired.add(path);
  return retired;
}

export function brokenCitations(
  sources: readonly [string, string][],
  exists: (path: string) => boolean,
  retired: ReadonlySet<string>,
): string[] {
  const counts = new Map<string, number>();
  for (const [, text] of sources) {
    for (const path of text.match(CITATION) ?? []) {
      if (exists(path) || retired.has(path)) continue;
      counts.set(path, (counts.get(path) ?? 0) + 1);
    }
  }
  return [...counts]
    .sort((left, right) => right[1] - left[1])
    .map(([path, count]) =>
      `${path} is cited by ${count} source(s) but does not exist; ` +
      `repoint it to the tool's current path, or record its retirement in PROVENANCE.md`);
}

// The retirement table silences a broken citation, so each row must actually
// recover. Without this it is an unbounded permission slip: add a row and any
// citation passes, whether or not the commit it names still holds the file.
export function unrecoverableRetirements(provenance: string): string[] {
  const problems: string[] = [];
  for (const [, path, sha] of provenance.matchAll(/\|\s*`(tools\/[a-z0-9_/]+\.ts)`\s*\|\s*`([0-9a-f]{7,40})`/g)) {
    const found = Bun.spawnSync(["git", "cat-file", "-e", `${sha}:${path}`], { cwd: ROOT });
    if (found.exitCode !== 0) {
      problems.push(`PROVENANCE.md claims ${path} is recoverable at ${sha}, but it is not there`);
    }
  }
  return problems;
}

function selfTest(): void {
  const retired = retiredTools("| `tools/gone_fixture.ts` | `abc1234` | recover |");
  if (!retired.has("tools/gone_fixture.ts")) throw new Error("retirement table not parsed");
  const sources: [string, string][] = [
    ["a.c", "// derived by tools/live_fixture.ts"],
    ["b.c", "// derived by tools/gone_fixture.ts"],
    ["c.c", "// derived by tools/absent_fixture.ts"],
    ["d.c", "// derived by tools/absent_fixture.ts"],
  ];
  const problems = brokenCitations(sources, (path) => path === "tools/live_fixture.ts", retired);
  if (problems.length !== 1) throw new Error(`expected one problem, got ${problems.length}`);
  if (!problems[0]!.includes("tools/absent_fixture.ts")) throw new Error("wrong path reported");
  if (!problems[0]!.includes("cited by 2")) throw new Error("citations must be counted");
  if (brokenCitations(sources, () => true, new Set()).length !== 0) {
    throw new Error("resolvable citations must pass");
  }
  if (unrecoverableRetirements("| `tools/x.ts` | `0000000` | recover |").length !== 1) {
    throw new Error("a retirement row naming an unreachable commit must fail");
  }
  console.log("source_citations self-test ok");
}

function main(): void {
  if (Bun.argv.includes("--self-test")) return selfTest();
  // Recursive, and asm/ and include/ too. The first version read only the top
  // level of exact/ and semantic/ -- which is where the citations happen to be
  // today, and this gate exists precisely because "where nobody thought to
  // check" is the failure mode.
  const sources: [string, string][] = [];
  const walk = (directory: string, prefix: string): void => {
    if (!existsSync(directory)) return;
    for (const entry of readdirSync(directory, { withFileTypes: true })) {
      const relative = `${prefix}/${entry.name}`;
      if (entry.isDirectory()) walk(join(directory, entry.name), relative);
      else if (/\.(c|h|s|inc)$/.test(entry.name)) {
        sources.push([relative, readFileSync(join(directory, entry.name), "utf8")]);
      }
    }
  };
  for (const directory of ["exact", "semantic", "asm", "include", "assets/code"]) {
    walk(join(ROOT, directory), directory);
  }
  const provenance = readFileSync(join(ROOT, "PROVENANCE.md"), "utf8");
  const retired = retiredTools(provenance);
  const problems = [
    ...brokenCitations(sources, (path) => existsSync(join(ROOT, path)), retired),
    ...unrecoverableRetirements(provenance),
  ];
  if (problems.length > 0) {
    for (const problem of problems) console.error(`error: ${problem}`);
    process.exit(1);
  }
  console.log(`source citations ok: ${sources.length} sources, ${retired.size} recorded retirement(s)`);
}

if (import.meta.main) main();
