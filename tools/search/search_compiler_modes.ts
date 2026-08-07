#!/usr/bin/env bun
// Batch front-end for the authoritative compiler configuration explorer.
//
// Candidate compilation, family selection, flag replacement, cache identity,
// compatibility rules, and scoring all live in mode_sweep.ts.  This file only
// selects queue members, invokes that engine, and aggregates exact results.
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
import { canonicalJson } from "../lib/canonical_json.ts";
import { modeSweepOutputDirectory } from "../lib/mode_sweep.ts";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
const OUT = join(ROOT, "out/decomp/compiler-modes");

interface Options {
  limit: number;
  jobs: number;
  queue: string;
  rom: string;
  pairs: boolean;
  triples: boolean;
  maxPairs: number;
  maxTriples: number;
}
interface QueueItem { stem: string; candidate: string }
interface SweepResult {
  config: {
    ids: string[];
    flags: string[];
    remove_flags: string[];
    compiler_family: string;
  };
  compiled: boolean;
  evidence?: { exact: boolean };
}
interface SweepReport {
  format: number;
  stem: string;
  source: string;
  source_sha256: string;
  reference_sha256: string;
  compiler_signature: string;
  planning: { bounded_search_complete: boolean };
  results: SweepResult[];
}

function optionsOf(arguments_: string[]): Options {
  const options: Options = {
    limit: 250,
    jobs: Math.min(8, navigator.hardwareConcurrency || 1),
    queue: join(ROOT, "out/decomp/queue.json"),
    rom: join(ROOT, "roms", "gs1-en.gba"),
    pairs: false,
    triples: false,
    maxPairs: 256,
    maxTriples: 64,
  };
  for (let index = 0; index < arguments_.length; index++) {
    const argument = arguments_[index];
    if (argument === "--limit") options.limit = Number(arguments_[++index]);
    else if (argument === "--jobs") options.jobs = Number(arguments_[++index]);
    else if (argument === "--queue") options.queue = arguments_[++index];
    else if (argument === "--rom") options.rom = arguments_[++index];
    else if (argument === "--pairs") options.pairs = true;
    else if (argument === "--triples") options.triples = options.pairs = true;
    else if (argument === "--max-pairs") options.maxPairs = Number(arguments_[++index]);
    else if (argument === "--max-triples") options.maxTriples = Number(arguments_[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log([
        "usage: search_compiler_modes.ts [options]",
        "  --limit N --jobs N --queue FILE --rom FILE",
        "  --pairs [--max-pairs N]",
        "  --triples [--max-triples N]",
      ].join("\n"));
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  for (const [name, value] of [
    ["limit", options.limit], ["jobs", options.jobs],
    ["max-pairs", options.maxPairs], ["max-triples", options.maxTriples],
  ] as const) {
    if (!Number.isInteger(value) || value < 1) throw new Error(`${name} must be a positive integer`);
  }
  return options;
}

function selfTest(): void {
  const parsed = optionsOf(["--limit", "7", "--pairs", "--max-pairs", "11"]);
  if (parsed.limit !== 7 || !parsed.pairs || parsed.triples || parsed.maxPairs !== 11) {
    throw new Error("compiler mode batch option parsing differs");
  }
  const triple = optionsOf(["--triples"]);
  if (!triple.pairs || !triple.triples) throw new Error("triples must imply pairs");
  console.log("compiler mode batch self-test passed");
}

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const options = optionsOf(Bun.argv.slice(2));
  const queue = JSON.parse(readFileSync(options.queue, "utf8")) as { items: QueueItem[] };
  const items = queue.items
    .map((item) => ({ ...item, source: resolve(ROOT, item.candidate) }))
    .filter((item) => existsSync(item.source))
    .slice(0, options.limit);
  if (items.length === 0) throw new Error("no queued candidates have source files");

  const reports = new Array<SweepReport>(items.length);
  let cursor = 0;
  async function worker(): Promise<void> {
    while (true) {
      const index = cursor++;
      if (index >= items.length) return;
      const item = items[index];
      const command = [
        "bun", join(ROOT, "tools/lib/mode_sweep.ts"), item.source,
        "--rom", options.rom, "--jobs", "1", "--top", "1",
        "--max-pairs", String(options.maxPairs),
        "--max-triples", String(options.maxTriples),
        ...(options.pairs ? ["--pairs"] : []),
        ...(options.triples ? ["--triples"] : []),
      ];
      const child = Bun.spawn(command, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
      const [code, stdout, stderr] = await Promise.all([
        child.exited,
        new Response(child.stdout).text(),
        new Response(child.stderr).text(),
      ]);
      if (code !== 0) throw new Error(`${item.stem}: ${(stderr || stdout).trim().slice(0, 600)}`);
      reports[index] = JSON.parse(
        readFileSync(join(modeSweepOutputDirectory(item.source), "report.json"), "utf8"),
      ) as SweepReport;
    }
  }
  await Promise.all(Array.from({ length: Math.min(options.jobs, items.length) }, worker));

  const matches = reports.flatMap((report) => report.results
    .filter((result) => result.compiled && result.evidence?.exact)
    .map((result) => ({
      stem: report.stem,
      source: report.source,
      config: result.config,
    })));
  const summary = {
    format: 2,
    engine: "tools/lib/mode_sweep.ts",
    search: {
      pairs: options.pairs,
      triples: options.triples,
      max_pairs: options.maxPairs,
      max_triples: options.maxTriples,
    },
    members: reports.map((report) => ({
      stem: report.stem,
      source: report.source,
      source_sha256: report.source_sha256,
      reference_sha256: report.reference_sha256,
      compiler_signature: report.compiler_signature,
      bounded_search_complete: report.planning.bounded_search_complete,
    })),
    matches,
    auto_promote: false,
  };
  mkdirSync(OUT, { recursive: true });
  const reportPath = join(OUT, "report.json");
  writeFileSync(reportPath, canonicalJson(summary) + "\n");
  console.log(`candidates=${reports.length} exact_configurations=${matches.length} report=${reportPath}`);
}

if (import.meta.main) await main();
