#!/usr/bin/env bun
// Build a resumable, expected-value queue for exact-C search. All candidate C,
// compiled artifacts, and diagnostics stay below ignored out/.
import { existsSync, mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { diagnoseCandidate, type CandidateDiagnosis } from "./decomp_diagnose.ts";
import { M2C_PREAMBLE } from "./match_m2c.ts";
import { retainedAssemblyStems } from "./permute_m2c.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const OUT = join(ROOT, "out", "decomp");

interface State {
  best?: { body: string; score: number } | null;
  rounds?: Array<{ before: number; after: number }>;
}

interface QueueItem {
  stem: string;
  candidate: string;
  source_hash: string;
  weighted_score: number;
  expected_value: number;
  plateau: boolean;
  improvement: number;
  diagnosis: CandidateDiagnosis;
}

interface QueueReport {
  format: 1;
  generated_at: string;
  items: QueueItem[];
}

function argumentsOf(argv: string[]): { jobs: number; limit: number; refresh: boolean; targetsOut?: string } {
  let jobs = Math.max(1, Math.min(16, (navigator.hardwareConcurrency || 8) - 2));
  let limit = 200, refresh = false, targetsOut: string | undefined;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--jobs") jobs = Number(argv[++index]);
    else if (argument === "--limit") limit = Number(argv[++index]);
    else if (argument === "--refresh") refresh = true;
    else if (argument === "--targets-out") targetsOut = argv[++index];
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: decomp_queue.ts [--jobs N] [--limit N] [--refresh] [--targets-out FILE]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  if (!Number.isInteger(jobs) || jobs < 1 || !Number.isInteger(limit) || limit < 1) throw new Error("jobs and limit must be positive integers");
  return { jobs, limit, refresh, targetsOut };
}

function expectedValue(diagnosis: CandidateDiagnosis, state: State): { value: number; plateau: boolean; improvement: number } {
  const rounds = state.rounds ?? [];
  const last = rounds.at(-1);
  const improvement = last && Number.isFinite(last.before) ? Math.max(0, last.before - last.after) : 0;
  const plateau = rounds.length >= 2 && rounds.slice(-2).every((round) => round.after >= round.before);
  const closeness = 1000 / (1 + diagnosis.byte_mismatches);
  const tractability = 120 / (1 + diagnosis.actual_size / 48);
  // Semantic mismatches often yield to source-shape constraints; register-only
  // plateaus remain useful, but receive a smaller allocation until a targeted
  // liveness mutation exists for their pattern.
  const diagnosisBonus = diagnosis.semantic_fraction * 70 + diagnosis.register_fraction * 20;
  return { value: closeness + tractability + diagnosisBonus + Math.min(80, improvement * 4) - (plateau ? 35 : 0), plateau, improvement };
}

function candidateFor(stem: string, state: State): string | null {
  const directory = join(OUT, "candidates");
  mkdirSync(directory, { recursive: true });
  const output = join(directory, `${stem}.c`);
  if (state.best?.body) {
    const body = state.best.body.startsWith("typedef") ? state.best.body : M2C_PREAMBLE + state.best.body;
    writeFileSync(output, body);
    return output;
  }
  const brute = join(ROOT, "out", "brute", "best", `${stem}.c`);
  if (existsSync(brute)) { writeFileSync(output, readFileSync(brute)); return output; }
  return null;
}

async function main(): Promise<void> {
  const options = argumentsOf(Bun.argv.slice(2));
  mkdirSync(OUT, { recursive: true });
  const reportPath = join(OUT, "queue.json");
  const previous = !options.refresh && existsSync(reportPath)
    ? JSON.parse(readFileSync(reportPath, "utf8")) as QueueReport : null;
  const cached = new Map((previous?.items ?? []).map((item) => [`${item.stem}:${item.source_hash}`, item]));
  const retained = retainedAssemblyStems();
  const states = join(ROOT, "out", "permute1", "state");
  const pending: Array<{ stem: string; state: State; source: string; hash: string }> = [];
  for (const name of readdirSync(states).filter((entry) => entry.endsWith(".json")).sort()) {
    const stem = basename(name, ".json");
    if (retained.has(stem) || !existsSync(join(ROOT, "asm", `${stem}.s`))) continue;
    const state = JSON.parse(readFileSync(join(states, name), "utf8")) as State;
    const source = candidateFor(stem, state);
    if (source === null) continue;
    const hash = Bun.hash(readFileSync(source)).toString(36);
    pending.push({ stem, state, source, hash });
  }
  pending.sort((left, right) => (left.state.best?.score ?? Infinity) - (right.state.best?.score ?? Infinity));
  const selected = pending.slice(0, options.limit);
  const rom = readFileSync(join(ROOT, "gs1-en.gba"));
  const items = new Array<QueueItem | null>(selected.length).fill(null);
  let cursor = 0;
  async function worker(): Promise<void> {
    while (true) {
      const index = cursor++;
      if (index >= selected.length) return;
      const target = selected[index];
      const hit = cached.get(`${target.stem}:${target.hash}`);
      if (hit) { items[index] = hit; continue; }
      try {
        const diagnosis = await diagnoseCandidate(target.source, rom, join(OUT, "work", target.stem));
        const priority = expectedValue(diagnosis, target.state);
        items[index] = {
          stem: target.stem, candidate: target.source.slice(ROOT.length + 1), source_hash: target.hash,
          weighted_score: target.state.best?.score ?? Number.MAX_SAFE_INTEGER,
          expected_value: priority.value, plateau: priority.plateau, improvement: priority.improvement, diagnosis,
        };
      } catch (error) {
        console.error(`skip ${target.stem}: ${(error as Error).message}`);
      }
    }
  }
  await Promise.all(Array.from({ length: Math.min(options.jobs, selected.length) }, worker));
  const ranked = items.filter((item): item is QueueItem => item !== null)
    .sort((left, right) => right.expected_value - left.expected_value || left.stem.localeCompare(right.stem));
  const report: QueueReport = { format: 1, generated_at: new Date().toISOString(), items: ranked };
  writeFileSync(reportPath, JSON.stringify(report, null, 2) + "\n");
  if (options.targetsOut) writeFileSync(options.targetsOut, ranked.map((item) => item.stem).join("\n") + "\n");
  const plateaus = ranked.filter((item) => item.plateau).length;
  const registerLed = ranked.filter((item) => item.diagnosis.register_fraction >= 0.5).length;
  console.log(`queued=${ranked.length} plateaus=${plateaus} register_led=${registerLed} report=${reportPath}`);
  for (const item of ranked.slice(0, 20)) {
    console.log(`${item.stem}\tvalue=${item.expected_value.toFixed(1)}\tbytes=${item.diagnosis.byte_mismatches}\t${item.diagnosis.dominant}`);
  }
}

if (import.meta.main) await main();
