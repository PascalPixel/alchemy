#!/usr/bin/env bun
// Run the bounded compiler explorer across a semantically verified cohort and
// report configurations that are exact for more than one member or contribute
// an irreducible improvement to multiple members without worsening any member
// in which they compile.
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

interface SharedImprovement {
  ids: string[];
  flags: string[];
  remove_flags: string[];
  compiler_family: string;
  improved_stems: string[];
  unchanged_stems: string[];
  score_changes: {
    stem: string;
    baseline_halfwords: number;
    result_halfwords: number;
    removed_halfwords: number;
  }[];
  total_halfwords_removed: number;
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

export function sharedNonRegressingImprovements(
  reports: readonly ExplorerReport[],
): SharedImprovement[] {
  const baselines = new Map<string, number>();
  const configs = new Map<string, {
    config: ExplorerResult["config"];
    scores: Map<string, number>;
  }>();
  for (const report of reports) {
    for (const result of report.results) {
      if (!result.compiled || result.evidence === undefined) continue;
      if (result.config.ids.length === 0 &&
          result.config.compiler_family === "routed" &&
          result.config.flags.length === 0 &&
          result.config.remove_flags.length === 0) {
        baselines.set(report.stem, result.evidence.differing_halfwords);
      }
      const key = canonicalJson(result.config);
      const row = configs.get(key) ?? { config: result.config, scores: new Map() };
      row.scores.set(report.stem, result.evidence.differing_halfwords);
      configs.set(key, row);
    }
  }
  if (baselines.size !== reports.length) {
    throw new Error("mode cohort: every report must contain a compiled routed baseline");
  }

  const found: SharedImprovement[] = [];
  for (const { config, scores } of configs.values()) {
    const improved: string[] = [];
    const unchanged: string[] = [];
    const scoreChanges: SharedImprovement["score_changes"] = [];
    let removed = 0;
    let worsened = false;
    for (const report of reports) {
      const baseline = baselines.get(report.stem)!;
      const score = scores.get(report.stem);
      // A configuration that fails to compile one cohort member is not shared.
      if (score === undefined || score > baseline) {
        worsened = true;
        break;
      }
      if (score < baseline) {
        // Do not credit an additive superset with unrelated improvements from
        // its members. A stem counts only when this configuration beats every
        // compiled proper subset from the same compiler family.
        const properSubsetScores = [...configs.values()]
          .filter((candidate) =>
            candidate.config.compiler_family === config.compiler_family &&
            candidate.config.ids.length < config.ids.length &&
            candidate.config.ids.every((id) => config.ids.includes(id)))
          .map((candidate) => candidate.scores.get(report.stem))
          .filter((candidateScore): candidateScore is number => candidateScore !== undefined);
        const bestProperSubset = Math.min(baseline, ...properSubsetScores);
        if (score < bestProperSubset) {
          improved.push(report.stem);
          const removedHere = baseline - score;
          removed += removedHere;
          scoreChanges.push({
            stem: report.stem,
            baseline_halfwords: baseline,
            result_halfwords: score,
            removed_halfwords: removedHere,
          });
        } else {
          unchanged.push(report.stem);
        }
      } else {
        unchanged.push(report.stem);
      }
    }
    if (!worsened && improved.length > 1) {
      found.push({
        ...config,
        improved_stems: improved.sort(),
        unchanged_stems: unchanged.sort(),
        score_changes: scoreChanges.sort((left, right) => left.stem.localeCompare(right.stem)),
        total_halfwords_removed: removed,
      });
    }
  }
  return found.sort((left, right) =>
    right.improved_stems.length - left.improved_stems.length ||
    right.total_halfwords_removed - left.total_halfwords_removed ||
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
        { config: { ids: ["shared"], flags: ["-fshared"], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: false, differing_halfwords: 1 } },
        { config: { ids: ["left"], flags: ["-fleft"], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: false, differing_halfwords: 1 } },
        { config: { ids: ["left", "right"], flags: ["-fleft", "-fright"], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: false, differing_halfwords: 1 } },
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
        { config: { ids: ["shared"], flags: ["-fshared"], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: false, differing_halfwords: 2 } },
        { config: { ids: ["right"], flags: ["-fright"], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: false, differing_halfwords: 2 } },
        { config: { ids: ["left", "right"], flags: ["-fleft", "-fright"], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: false, differing_halfwords: 2 } },
        { config: { ids: [], flags: [], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: false, differing_halfwords: 3 } },
      ],
    },
  ];
  const shared = sharedExactConfigurations(reports);
  if (shared.length !== 1 ||
      shared[0].exact_stems.join(",") !== "08000000,08000010" ||
      shared[0].flags.join(" ") !== "-fno-gcse") {
    throw new Error("mode cohort self-test: shared exact aggregation differs");
  }
  const improvements = sharedNonRegressingImprovements(reports);
  const sharedImprovement = improvements.find((row) => row.ids.join(",") === "shared");
  if (improvements.length !== 2 ||
      sharedImprovement === undefined ||
      sharedImprovement.improved_stems.join(",") !== "08000000,08000010" ||
      sharedImprovement.total_halfwords_removed !== 2 ||
      improvements.some((row) => row.ids.join(",") === "left,right")) {
    throw new Error("mode cohort self-test: shared improvement aggregation differs");
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
  const improvements = sharedNonRegressingImprovements(reports);
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
    format: 2,
    members: reports.map((report) => ({
      source: report.source,
      source_sha256: report.source_sha256,
      reference_sha256: report.reference_sha256,
      compiler_signature: report.compiler_signature,
      bounded_search_complete: report.planning.bounded_search_complete,
    })),
    stems: reports.map((report) => report.stem),
    shared_exact_configurations: shared,
    shared_nonregressing_improvements: improvements,
    auto_promote: false,
  };
  const reportPath = join(output, "report.json");
  writeFileSync(reportPath, canonicalJson(summary) + "\n");
  console.log(`cohort=${reports.length} shared_exact=${shared.length} shared_improvements=${improvements.length}`);
  for (const row of shared) {
    console.log(`${row.exact_stems.length}  ${row.flags.join(" ") || "(routed default)"}  ${row.exact_stems.join(",")}`);
  }
  for (const row of improvements) {
    console.log(
      `improves=${row.improved_stems.length} removed=${row.total_halfwords_removed}hw  ` +
      `${row.flags.join(" ") || row.ids.join("+") || "(routed default)"}  ` +
      `${row.improved_stems.join(",")}`,
    );
  }
  console.log(`report=${reportPath}`);
}

if (import.meta.main) await main();
