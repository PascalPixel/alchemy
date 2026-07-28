#!/usr/bin/env bun
// Run the bounded compiler explorer across a semantically verified cohort and
// report only configurations that are exact for more than one member.
//
// This deliberately delegates compilation, caching, phased pair/triple policy,
// and floor classification to mode_sweep.ts. It is an aggregation layer, not
// another search implementation and never promotes a candidate.
import { createHash } from "node:crypto";
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join, resolve } from "node:path";
import { canonicalJson } from "./canonical_json.ts";
import { modeSweepOutputDirectory } from "./mode_sweep.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface ExplorerResult {
  config: {
    ids: string[];
    flags: string[];
    remove_flags: string[];
    compiler_family: string;
  };
  compiled: boolean;
  evidence?: { exact: boolean; differing_halfwords: number };
}

interface ExplorerReport {
  format: number;
  stem: string;
  source: string;
  source_sha256: string;
  reference_sha256: string;
  compiler_signature: string;
  policy: unknown;
  planning: { bounded_search_complete: boolean };
  results: ExplorerResult[];
}

interface SharedConfiguration {
  ids: string[];
  flags: string[];
  remove_flags: string[];
  compiler_family: string;
  exact_stems: string[];
}

export function sharedExactConfigurations(
  reports: readonly ExplorerReport[],
): SharedConfiguration[] {
  const grouped = new Map<string, SharedConfiguration>();
  for (const report of reports) {
    for (const result of report.results) {
      if (!result.compiled || !result.evidence?.exact) continue;
      const key = canonicalJson(result.config);
      const row = grouped.get(key) ?? {
        ids: result.config.ids,
        flags: result.config.flags,
        remove_flags: result.config.remove_flags,
        compiler_family: result.config.compiler_family,
        exact_stems: [],
      };
      if (!row.exact_stems.includes(report.stem)) row.exact_stems.push(report.stem);
      grouped.set(key, row);
    }
  }
  return [...grouped.values()]
    .filter((row) => row.exact_stems.length > 1)
    .map((row) => ({ ...row, exact_stems: row.exact_stems.sort() }))
    .sort((left, right) =>
      right.exact_stems.length - left.exact_stems.length ||
      left.ids.join("+").localeCompare(right.ids.join("+")));
}

function selfTest(): void {
  const reports: ExplorerReport[] = [
    {
      stem: "08000000",
      source: "a.c",
      results: [
        { config: { ids: ["no-gcse"], flags: ["-fno-gcse"], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: true, differing_halfwords: 0 } },
        { config: { ids: [], flags: [], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: false, differing_halfwords: 2 } },
      ],
    },
    {
      stem: "08000010",
      source: "b.c",
      results: [
        { config: { ids: ["no-gcse"], flags: ["-fno-gcse"], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: true, differing_halfwords: 0 } },
      ],
    },
  ];
  const shared = sharedExactConfigurations(reports);
  if (shared.length !== 1 ||
      shared[0].exact_stems.join(",") !== "08000000,08000010" ||
      shared[0].flags.join(" ") !== "-fno-gcse") {
    throw new Error("mode cohort self-test: shared exact aggregation differs");
  }
  console.log("mode cohort self-test passed");
}

async function main(): Promise<void> {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) {
    selfTest();
    return;
  }
  let jobs = Math.max(1, Math.min(8, (navigator.hardwareConcurrency || 8) - 2));
  let maxPairs = 256;
  let maxTriples = 64;
  const sources: string[] = [];
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--jobs") jobs = Number(argv[++index]);
    else if (argument === "--max-pairs") maxPairs = Number(argv[++index]);
    else if (argument === "--max-triples") maxTriples = Number(argv[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: mode_cohort.ts [--jobs N] [--max-pairs N] [--max-triples N] CANDIDATE.c [CANDIDATE.c ...]");
      return;
    } else sources.push(resolve(argument));
  }
  if (!Number.isInteger(jobs) || jobs < 1 ||
      !Number.isInteger(maxPairs) || maxPairs < 1 ||
      !Number.isInteger(maxTriples) || maxTriples < 1) {
    throw new Error("jobs and search bounds must be positive integers");
  }
  if (sources.length < 2) throw new Error("a cohort requires at least two candidates");

  const reports = new Array<ExplorerReport>(sources.length);
  let cursor = 0;
  async function worker(): Promise<void> {
    while (true) {
      const index = cursor++;
      if (index >= sources.length) return;
      const source = sources[index];
      const child = Bun.spawn([
        "bun", join(ROOT, "tools/mode_sweep.ts"), source,
        "--pairs", "--triples", "--jobs", "1", "--top", "1",
        "--max-pairs", String(maxPairs), "--max-triples", String(maxTriples),
      ], { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
      const [code, stdout, stderr] = await Promise.all([
        child.exited,
        new Response(child.stdout).text(),
        new Response(child.stderr).text(),
      ]);
      if (code !== 0) {
        throw new Error(`${basename(source)}: ${(stderr || stdout).trim().slice(0, 500)}`);
      }
      const stem = basename(source, ".c");
      reports[index] = JSON.parse(
        readFileSync(join(modeSweepOutputDirectory(source), "report.json"), "utf8"),
      ) as ExplorerReport;
    }
  }
  await Promise.all(Array.from({ length: Math.min(jobs, sources.length) }, worker));

  const shared = sharedExactConfigurations(reports);
  const digest = createHash("sha256")
    .update(canonicalJson(reports.map((report) => ({
      source: report.source,
      source_sha256: report.source_sha256,
      reference_sha256: report.reference_sha256,
      compiler_signature: report.compiler_signature,
      policy: report.policy,
      planning: report.planning,
    })).sort((left, right) => left.source.localeCompare(right.source))))
    .digest("hex").slice(0, 16);
  const output = join(ROOT, "out/modesweep/cohort", digest);
  mkdirSync(output, { recursive: true });
  const summary = {
    format: 1,
    members: reports.map((report) => ({
      source: report.source,
      source_sha256: report.source_sha256,
      reference_sha256: report.reference_sha256,
      compiler_signature: report.compiler_signature,
      bounded_search_complete: report.planning.bounded_search_complete,
    })),
    stems: reports.map((report) => report.stem),
    shared_exact_configurations: shared,
    auto_promote: false,
  };
  const reportPath = join(output, "report.json");
  writeFileSync(reportPath, canonicalJson(summary) + "\n");
  console.log(`cohort=${reports.length} shared_exact=${shared.length}`);
  for (const row of shared) {
    console.log(`${row.exact_stems.length}  ${row.flags.join(" ") || "(routed default)"}  ${row.exact_stems.join(",")}`);
  }
  console.log(`report=${reportPath}`);
}

if (import.meta.main) await main();
