#!/usr/bin/env bun
// Resumable end-to-end exact-C campaign for a high-core-count workstation.
// It refreshes learned dictionaries, diagnoses and constrains ranked candidates,
// probes adjacent modules, and then runs guided annealing.
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const OUT = join(ROOT, "out", "decomp");
const CHECKPOINT = join(OUT, "overnight.json");

interface Options { jobs: number; limit: number; waves: number; steps: number; restarts: number; fresh: boolean; dryRun: boolean }
interface Checkpoint { format: 1; learned: boolean; completed_wave: number; stage: string; updated_at: string }

function optionsOf(argv: string[]): Options {
  const options: Options = {
    jobs: Math.max(2, Math.min(16, (navigator.hardwareConcurrency || 8) - 2)),
    limit: 160, waves: 4, steps: 5000, restarts: 8, fresh: false, dryRun: false,
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--jobs") options.jobs = Number(argv[++index]);
    else if (argument === "--limit") options.limit = Number(argv[++index]);
    else if (argument === "--waves") options.waves = Number(argv[++index]);
    else if (argument === "--steps") options.steps = Number(argv[++index]);
    else if (argument === "--restarts") options.restarts = Number(argv[++index]);
    else if (argument === "--fresh") options.fresh = true;
    else if (argument === "--dry-run") options.dryRun = true;
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: decomp_overnight.ts [--jobs N] [--limit N] [--waves N] [--steps N] [--restarts N] [--fresh] [--dry-run]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  for (const [name, value] of Object.entries(options)) {
    if (typeof value === "number" && (!Number.isInteger(value) || value < 1)) throw new Error(`${name} must be a positive integer`);
  }
  return options;
}

async function run(command: string[], tolerate = false): Promise<{ code: number; output: string }> {
  const child = Bun.spawn(command, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  const [stdout, stderr, code] = await Promise.all([
    new Response(child.stdout).text(), new Response(child.stderr).text(), child.exited,
  ]);
  const output = (stdout + stderr).trim();
  if (code !== 0 && !tolerate) throw new Error(`${command.join(" ")} failed: ${output.slice(-1200)}`);
  return { code, output };
}

function save(checkpoint: Checkpoint, stage: string, wave = checkpoint.completed_wave): void {
  checkpoint.stage = stage; checkpoint.completed_wave = wave; checkpoint.updated_at = new Date().toISOString();
  writeFileSync(CHECKPOINT, JSON.stringify(checkpoint, null, 2) + "\n");
}

function targets(): string[] {
  const path = join(OUT, "targets.txt");
  return existsSync(path) ? readFileSync(path, "utf8").split(/\s+/).filter(Boolean) : [];
}

async function moduleProbes(stems: string[]): Promise<number> {
  const manifest = JSON.parse(readFileSync(join(ROOT, "out/full/asm/manifest.json"), "utf8")) as
    { regions: Array<{ address: number; size: number; source: string; retention: string }> };
  const selected = new Set(stems);
  const ordinary = manifest.regions.filter((region) => region.retention === "c_candidate");
  let installed = 0;
  for (let index = 1; index < ordinary.length; index++) {
    const left = /([0-9a-f]{8})\.s$/.exec(ordinary[index - 1].source)?.[1];
    const right = /([0-9a-f]{8})\.s$/.exec(ordinary[index].source)?.[1];
    const gap = ordinary[index].address - (ordinary[index - 1].address + ordinary[index - 1].size);
    if (!left || !right || !selected.has(left) || !selected.has(right) || gap < 0 || gap > 2) continue;
    const probe = await run(["bun", "tools/decomp_module.ts", left, right], true);
    if (probe.output.includes("exact=true")) {
      const result = await run(["bun", "tools/decomp_module.ts", "--install", left, right], true);
      if (result.code === 0) installed++;
    }
  }
  return installed;
}

async function main(): Promise<void> {
  const options = optionsOf(Bun.argv.slice(2));
  mkdirSync(OUT, { recursive: true });
  if (options.dryRun) {
    console.log(`dry-run jobs=${options.jobs} limit=${options.limit} waves=${options.waves} steps=${options.steps} restarts=${options.restarts}`);
    console.log("stages=learn,queue,constraints,module-probes,guided-permutation,full-build");
    return;
  }
  const checkpoint: Checkpoint = !options.fresh && existsSync(CHECKPOINT)
    ? JSON.parse(readFileSync(CHECKPOINT, "utf8")) : { format: 1, learned: false, completed_wave: 0, stage: "new", updated_at: new Date().toISOString() };
  if (!checkpoint.learned) {
    save(checkpoint, "learn");
    await run(["bun", "tools/mine_idioms.ts"]);
    checkpoint.learned = true; save(checkpoint, "learned");
  }
  for (let wave = checkpoint.completed_wave; wave < options.waves; wave++) {
    save(checkpoint, "queue", wave);
    const queue = await run(["bun", "tools/decomp_queue.ts", "--refresh", "--jobs", String(options.jobs), "--limit", String(options.limit), "--targets-out", join(OUT, "targets.txt")]);
    console.log(`wave=${wave + 1}/${options.waves} ${queue.output.split("\n")[0]}`);
    const stems = targets();
    if (stems.length === 0) break;
    save(checkpoint, "constraints", wave);
    await run(["bun", "tools/decomp_constraints.ts", ...stems]);
    save(checkpoint, "module-probes", wave);
    const modules = await moduleProbes(stems);
    if (modules > 0) console.log(`module_matches=${modules}`);
    save(checkpoint, "guided-permutation", wave);
    const permutation = await run([
      "bun", "tools/permute_v1.ts", "--targets", join(OUT, "targets.txt"), "--steps", String(options.steps),
      "--restarts", String(options.restarts), "--seed", String(wave), "--jobs", String(options.jobs),
    ]);
    console.log(permutation.output.split("\n").filter((line) => /^(matched|floor|unusable|targets=|cache=)/.test(line)).slice(-30).join("\n"));
    save(checkpoint, "full-build", wave);
    const build = await run(["bun", "tools/build_full.ts"]);
    console.log(build.output.split("\n").slice(-8).join("\n"));
    save(checkpoint, "wave-complete", wave + 1);
  }
  save(checkpoint, "complete", options.waves);
  console.log(`overnight campaign complete waves=${options.waves}`);
}

if (import.meta.main) await main();
