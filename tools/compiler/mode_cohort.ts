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
import { canonicalJson } from "../lib/canonical_json.ts";
import { modeSweepOutputDirectory, pairConfigs } from "../lib/mode_sweep.ts";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));

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

interface MultiRegionImprovement extends SharedImprovement {
  regressed_stems: string[];
  total_halfwords_added: number;
}

interface SingleModeEffect {
  ids: string[];
  flags: string[];
  remove_flags: string[];
  compiler_family: string;
  improved_stems: string[];
  unchanged_stems: string[];
  regressed_stems: string[];
  total_halfwords_removed: number;
  total_halfwords_added: number;
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

export function irreducibleConfigurationImprovements(
  reports: readonly ExplorerReport[],
): MultiRegionImprovement[] {
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

  const found: MultiRegionImprovement[] = [];
  for (const { config, scores } of configs.values()) {
    const improved: string[] = [];
    const unchanged: string[] = [];
    const regressed: string[] = [];
    const scoreChanges: MultiRegionImprovement["score_changes"] = [];
    let removed = 0;
    let added = 0;
    let incomplete = false;
    for (const report of reports) {
      const baseline = baselines.get(report.stem)!;
      const score = scores.get(report.stem);
      if (score === undefined) {
        incomplete = true;
        break;
      }
      if (score < baseline) {
        const properSubsetScores = [...configs.values()]
          .filter((candidate) =>
            candidate.config.compiler_family === config.compiler_family &&
            candidate.config.ids.length < config.ids.length &&
            candidate.config.ids.every((id) => config.ids.includes(id)))
          .map((candidate) => candidate.scores.get(report.stem))
          .filter((candidateScore): candidateScore is number => candidateScore !== undefined);
        if (score < Math.min(baseline, ...properSubsetScores)) {
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
      } else if (score > baseline) {
        regressed.push(report.stem);
        added += score - baseline;
      } else {
        unchanged.push(report.stem);
      }
    }
    if (!incomplete && improved.length > 0) {
      found.push({
        ...config,
        improved_stems: improved.sort(),
        unchanged_stems: unchanged.sort(),
        regressed_stems: regressed.sort(),
        score_changes: scoreChanges.sort((left, right) => left.stem.localeCompare(right.stem)),
        total_halfwords_removed: removed,
        total_halfwords_added: added,
      });
    }
  }
  return found.sort((left, right) =>
    right.improved_stems.length - left.improved_stems.length ||
    right.total_halfwords_removed - left.total_halfwords_removed ||
    left.regressed_stems.length - right.regressed_stems.length ||
    left.ids.join("+").localeCompare(right.ids.join("+")));
}

export function multiRegionImprovements(
  reports: readonly ExplorerReport[],
): MultiRegionImprovement[] {
  return irreducibleConfigurationImprovements(reports)
    .filter((row) => row.improved_stems.length > 1);
}

export function singleModeEffects(
  reports: readonly ExplorerReport[],
): SingleModeEffect[] {
  const baselines = new Map<string, number>();
  const singles = new Map<string, {
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
      } else if (result.config.ids.length === 1) {
        const key = canonicalJson(result.config);
        const row = singles.get(key) ?? { config: result.config, scores: new Map() };
        row.scores.set(report.stem, result.evidence.differing_halfwords);
        singles.set(key, row);
      }
    }
  }
  if (baselines.size !== reports.length) {
    throw new Error("mode cohort: every report must contain a compiled routed baseline");
  }
  const effects: SingleModeEffect[] = [];
  for (const { config, scores } of singles.values()) {
    if (scores.size !== reports.length) continue;
    const improved: string[] = [];
    const unchanged: string[] = [];
    const regressed: string[] = [];
    let removed = 0;
    let added = 0;
    for (const report of reports) {
      const baseline = baselines.get(report.stem)!;
      const score = scores.get(report.stem)!;
      if (score < baseline) {
        improved.push(report.stem);
        removed += baseline - score;
      } else if (score > baseline) {
        regressed.push(report.stem);
        added += score - baseline;
      } else {
        unchanged.push(report.stem);
      }
    }
    effects.push({
      ...config,
      improved_stems: improved.sort(),
      unchanged_stems: unchanged.sort(),
      regressed_stems: regressed.sort(),
      total_halfwords_removed: removed,
      total_halfwords_added: added,
    });
  }
  return effects.sort((left, right) =>
    right.improved_stems.length - left.improved_stems.length ||
    right.total_halfwords_removed - left.total_halfwords_removed ||
    left.regressed_stems.length - right.regressed_stems.length ||
    left.ids.join("+").localeCompare(right.ids.join("+")));
}

function selfTest(): void {
  const reports: ExplorerReport[] = [
    {
      stem: "08000000",
        format: 4,
        source_sha256: "",
        reference_sha256: "",
        compiler_signature: "",
        policy: null,
        planning: { bounded_search_complete: true },
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
        format: 4,
        source_sha256: "",
        reference_sha256: "",
        compiler_signature: "",
        policy: null,
        planning: { bounded_search_complete: true },
      source: "b.c",
      results: [
        { config: { ids: ["no-gcse"], flags: ["-fno-gcse"], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: true, differing_halfwords: 0 } },
        { config: { ids: ["shared"], flags: ["-fshared"], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: false, differing_halfwords: 2 } },
        { config: { ids: ["right"], flags: ["-fright"], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: false, differing_halfwords: 2 } },
        { config: { ids: ["left"], flags: ["-fleft"], remove_flags: [], compiler_family: "routed" }, compiled: true,
          evidence: { exact: false, differing_halfwords: 3 } },
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
  const multi = multiRegionImprovements(reports);
  if (!multi.some((row) =>
    row.ids.join(",") === "shared" &&
    row.improved_stems.join(",") === "08000000,08000010") ||
      multi.some((row) => row.ids.join(",") === "left,right")) {
    throw new Error("mode cohort self-test: multi-region improvement aggregation differs");
  }
  const irreducible = irreducibleConfigurationImprovements(reports);
  if (!irreducible.some((row) => row.ids.join(",") === "left") ||
      irreducible.some((row) => row.ids.join(",") === "left,right")) {
    throw new Error("mode cohort self-test: irreducible configuration aggregation differs");
  }
  const singles = singleModeEffects(reports);
  if (!singles.some((row) =>
    row.ids.join(",") === "shared" &&
    row.improved_stems.join(",") === "08000000,08000010")) {
    throw new Error("mode cohort self-test: single-mode effect aggregation differs");
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
  const exhaustivePairCount = pairConfigs().length;
  let maxPairs = exhaustivePairCount;
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
  if (maxPairs < exhaustivePairCount) {
    throw new Error(
      `mode cohort requires one exhaustive common pair matrix (${exhaustivePairCount} pairs)`,
    );
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
        "bun", join(ROOT, "tools/lib/mode_sweep.ts"), source,
        "--pairs", "--triples", "--family-factorial", "--jobs", "1", "--top", "1",
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
  const irreducible = irreducibleConfigurationImprovements(reports);
  const multiRegion = multiRegionImprovements(reports);
  const singleEffects = singleModeEffects(reports);
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
    format: 4,
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
    irreducible_configuration_improvements:
      irreducible.filter((row) => row.ids.length > 1),
    multi_region_improvements: multiRegion,
    single_mode_effects: singleEffects,
    common_pair_configurations: exhaustivePairCount,
    auto_promote: false,
  };
  const reportPath = join(output, "report.json");
  writeFileSync(reportPath, canonicalJson(summary) + "\n");
  console.log(
    `cohort=${reports.length} shared_exact=${shared.length} ` +
    `shared_improvements=${improvements.length} multi_region=${multiRegion.length}`,
  );
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
  for (const row of multiRegion) {
    console.log(
      `multi=${row.improved_stems.length} regressed=${row.regressed_stems.length} ` +
      `removed=${row.total_halfwords_removed}hw added=${row.total_halfwords_added}hw  ` +
      `${row.flags.join(" ") || row.ids.join("+")}  ${row.improved_stems.join(",")}`,
    );
  }
  for (const row of singleEffects.filter((effect) => effect.improved_stems.length > 0)) {
    console.log(
      `single=${row.ids.join("+")} improved=${row.improved_stems.length} ` +
      `regressed=${row.regressed_stems.length} removed=${row.total_halfwords_removed}hw ` +
      `added=${row.total_halfwords_added}hw`,
    );
  }
  console.log(`report=${reportPath}`);
}

if (import.meta.main) await main();
