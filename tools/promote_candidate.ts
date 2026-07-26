#!/usr/bin/env bun
// Promote a hand-written candidate into out/permute1/state/<stem>.json.
//
// tools/decomp_queue.ts regenerates out/decomp/candidates/<stem>.c from that
// state file on every run, so a candidate improved by hand and only copied into
// the candidates directory is silently clobbered by the next queue refresh. The
// search state is the durable store; this tool is the way to write to it.
//
// All candidate C, compiled artifacts and scoring scratch stay below ignored
// out/. Nothing here reads or emits ROM bytes outside the local build.
import { existsSync, mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { Scorer } from "./permute_v1.ts";
import { M2C_PREAMBLE } from "./match_m2c.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const STATE_DIR = join(ROOT, "out", "permute1", "state");

interface TargetState {
  best: { body: string; score: number } | null;
  population: Array<{ body: string; score: number }>;
  operators: Record<string, { tried: number; accepted: number }>;
  rounds: Array<{ seed: number; before: number; after: number }>;
}

function regionSize(stem: string): number | null {
  const manifest = [join(ROOT, "out/full/asm/manifest.json"), join(ROOT, "out/asm/manifest.json")].find(existsSync);
  if (!manifest) return null;
  const document = JSON.parse(readFileSync(manifest, "utf8")) as { regions?: Array<{ source: string; size: number }> };
  for (const region of document.regions ?? []) {
    if (basename(region.source, ".s") === stem) return region.size;
  }
  return null;
}

async function promote(path: string, force: boolean, override?: string): Promise<boolean> {
  const stem = override ?? basename(path, ".c");
  if (!/^08[0-9a-f]{6}$/.test(stem)) {
    throw new Error(`not a region stem: ${basename(path)} (pass --stem=08xxxxxx for a differently named file)`);
  }
  const size = regionSize(stem);
  if (size === null) throw new Error(`${stem}: no asm manifest entry; run tools/build_asm.ts first`);

  const rom = readFileSync(join(ROOT, "roms", "gs1-en.gba"));
  const address = Number.parseInt(stem, 16) - 0x08000000;
  const expected = Buffer.from(rom.subarray(address, address + size));

  const scratch = join(ROOT, "out", "decomp", "promote", stem);
  mkdirSync(scratch, { recursive: true });
  const source = readFileSync(path, "utf8");
  const probe = source.startsWith("typedef") ? source : M2C_PREAMBLE + source;
  const score = await new Scorer(stem, expected, scratch).score(probe);
  if (score === Number.MAX_SAFE_INTEGER) throw new Error(`${stem}: candidate does not compile and link`);

  const statePath = join(STATE_DIR, `${stem}.json`);
  const state: TargetState = existsSync(statePath)
    ? JSON.parse(readFileSync(statePath, "utf8")) as TargetState
    : { best: null, population: [], operators: {}, rounds: [] };
  const previous = state.best?.score ?? Number.MAX_SAFE_INTEGER;
  if (score >= previous && !force) {
    console.log(`skip ${stem} score=${score} not better than stored ${previous} (use --force to override)`);
    return false;
  }

  const body = probe.replace(M2C_PREAMBLE, "");
  state.best = { body, score };
  state.population = [{ body, score }, ...state.population.filter((entry) => entry.body !== body)].slice(0, 16);
  mkdirSync(STATE_DIR, { recursive: true });
  writeFileSync(statePath, JSON.stringify(state));
  writeFileSync(join(ROOT, "out", "decomp", "candidates", `${stem}.c`), probe);
  console.log(`promote ${stem} score=${score} was=${previous === Number.MAX_SAFE_INTEGER ? "none" : previous}`);
  return true;
}

async function main(): Promise<void> {
  const argv = Bun.argv.slice(2);
  const force = argv.includes("--force");
  const override = argv.find((argument) => argument.startsWith("--stem="))?.slice(7);
  const paths = argv.filter((argument) => !argument.startsWith("--"));
  if (paths.length === 0) {
    console.log("usage: promote_candidate.ts [--force] [--stem=08xxxxxx] <file.c|directory> ...");
    process.exitCode = 2;
    return;
  }
  if (override !== undefined && paths.length !== 1) throw new Error("--stem= applies to a single file");
  const files: string[] = [];
  for (const path of paths) {
    if (!existsSync(path)) throw new Error(`no such path: ${path}`);
    if (readdirSync(dirname(path)).length >= 0 && path.endsWith(".c")) { files.push(path); continue; }
    for (const name of readdirSync(path).filter((entry) => entry.endsWith(".c")).sort()) files.push(join(path, name));
  }
  let promoted = 0;
  for (const file of files) if (await promote(file, force, override)) promoted++;
  console.log(`promoted=${promoted} examined=${files.length}`);
}

if (import.meta.main) await main();
