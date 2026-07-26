#!/usr/bin/env bun
// Sweep one candidate across the fork's evidenced -m modes and the stock pass
// switches, singly and optionally in pairs, scoring each against the whole
// assembly region.
//
// Why this exists: tools/search_compiler_modes.ts probes stock GCC flags in
// bulk across the ranked queue, but the alchemy-gcc fork carries eleven -m
// compatibility modes and nothing enumerated them for a single target. Deciding
// whether a residual is source-shaped or compiler-shaped meant hand-running the
// driver once per flag, which is slow enough that agents skipped it and called
// plateaus early. One sweep answers the question the routing table asks: is
// there an evidenced mode that closes this region, and if not, does any mode
// move it at all?
//
// Scores span every compiled Func_ symbol, matching the gate in
// tools/integrate_matches.ts, so a region holding a trailing leaf is measured
// whole. Candidate binaries stay under ignored out/.
import { mkdirSync, readFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { ROM_BASE, verifyCandidate } from "./match_m2c.ts";
import { linkedFunctionExtent } from "./integrate_matches.ts";
import { regionSize } from "./candidate_show.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

// The fork's compatibility modes, as declared in gcc-2.96/gcc/config/arm/arm.h.
export const FORK_MODES = [
  "-mgrouped-dma-store", "-mpreserve-single-bit-test", "-mentry-low-register-order",
  "-mthumb-and-sets-cc", "-mcall-arg0-move-first", "-mearly-frame-allocation",
  "-mhigh-register-move-first", "-mthumb-entry-literal-first",
  "-mthumb-early-literal-pool", "-mthumb-immediate-latency", "-mthumb-load-latency-one",
] as const;

// -fno-schedule-insns is deliberately absent. The pre-reload scheduler does
// nothing in this fork: compiling 40 converted sources, including the largest,
// with -fschedule-insns and with -fno-schedule-insns gives byte-identical
// assembly every time (work/sched_probe.ts). Sweeping it wastes a compile per
// target and, worse, puts a line in the results that reads like evidence when a
// residual happens to be unchanged by it. Only -fno-schedule-insns2 can reorder
// anything, which is why every routed member of UNSCHEDULED_SOURCES is really
// carried by that flag alone.
export const STOCK_SWITCHES = [
  "-fno-schedule-insns2", "-fno-gcse", "-fno-cse-follow-jumps",
  "-fno-expensive-optimizations", "-fno-peephole", "-fno-strength-reduce", "-fno-regmove",
] as const;

interface Options { source: string; rom: string; pairs: boolean; jobs: number; top: number }

export function combinations(pairs: boolean): string[][] {
  const singles: string[][] = [[], ...FORK_MODES.map((flag) => [flag]), ...STOCK_SWITCHES.map((flag) => [flag])];
  if (!pairs) return singles;
  const both: string[][] = [];
  for (const left of FORK_MODES) {
    for (const right of [...FORK_MODES, ...STOCK_SWITCHES]) {
      if (left < right) both.push([left, right]);
    }
  }
  return [...singles, ...both];
}

function optionsOf(argv: string[]): Options {
  const options: Options = {
    source: "",
    rom: join(ROOT, "roms/gs1-en.gba"),
    pairs: false,
    jobs: Math.max(1, Math.min(10, (navigator.hardwareConcurrency || 8) - 2)),
    top: 16,
  };
  const rest: string[] = [];
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--rom") options.rom = argv[++index];
    else if (argument === "--pairs") options.pairs = true;
    else if (argument === "--jobs") options.jobs = Number(argv[++index]);
    else if (argument === "--top") options.top = Number(argv[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: mode_sweep.ts <candidate.c> [--pairs] [--jobs N] [--top N] [--rom FILE]");
      process.exit(0);
    } else rest.push(argument);
  }
  if (rest.length !== 1) throw new Error("usage: mode_sweep.ts <candidate.c> [--pairs]");
  if (!Number.isInteger(options.jobs) || options.jobs < 1) throw new Error("jobs must be a positive integer");
  options.source = rest[0];
  return options;
}

function selfTest(): void {
  const singles = combinations(false);
  if (singles[0].length !== 0) throw new Error("mode sweep self-test: the routed default must be scored first");
  if (singles.length !== 1 + FORK_MODES.length + STOCK_SWITCHES.length) {
    throw new Error("mode sweep self-test: single-flag combination count differs");
  }
  const paired = combinations(true);
  if (paired.length <= singles.length) throw new Error("mode sweep self-test: --pairs must add combinations");
  if (paired.some((flags) => flags.length === 2 && flags[0] >= flags[1])) {
    throw new Error("mode sweep self-test: pairs must be ordered and distinct");
  }
  console.log("mode sweep self-test passed");
}

async function main(): Promise<void> {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) {
    selfTest();
    return;
  }
  const options = optionsOf(argv);
  const stem = basename(options.source, ".c");
  const address = Number.parseInt(stem, 16);
  const wanted = regionSize(stem);
  if (wanted === null) throw new Error(`no assembly region is recorded for ${stem}`);
  const rom = readFileSync(options.rom);
  const reference = rom.subarray(address - ROM_BASE, address - ROM_BASE + wanted);

  async function score(flags: string[]): Promise<{ bytes: number; size: number } | null> {
    const work = join(ROOT, "out", "modesweep", stem, flags.join("_").replaceAll("-", "") || "routed");
    mkdirSync(work, { recursive: true });
    try {
      await verifyCandidate(options.source, rom, work, flags, ROM_BASE);
      const linked = readFileSync(join(work, `${stem}.bin`));
      const symbols = Bun.spawnSync(["arm-none-eabi-nm", "-S", "--defined-only", join(work, `${stem}.elf`)], { stdout: "pipe" });
      const extent = linkedFunctionExtent(symbols.stdout.toString(), `Func_${stem}`, address, linked.length);
      const actual = linked.subarray(0, extent);
      let bytes = Math.abs(actual.length - reference.length);
      for (let index = 0; index < Math.min(actual.length, reference.length); index++) {
        if (actual[index] !== reference[index]) bytes++;
      }
      return { bytes, size: actual.length };
    } catch {
      return null;
    }
  }

  const combos = combinations(options.pairs);
  const results: Array<{ flags: string[]; bytes: number; size: number }> = [];
  let cursor = 0;
  async function worker(): Promise<void> {
    while (cursor < combos.length) {
      const flags = combos[cursor++];
      const result = await score(flags);
      if (result !== null) results.push({ flags, ...result });
    }
  }
  await Promise.all(Array.from({ length: Math.min(options.jobs, combos.length) }, worker));
  results.sort((left, right) => left.bytes - right.bytes || left.flags.length - right.flags.length);
  console.log(`region=${wanted}B combos=${combos.length} scored=${results.length}`);
  for (const row of results.slice(0, options.top)) {
    console.log(`${String(row.bytes).padStart(4)}  size=${String(row.size).padStart(4)}  ${row.flags.join(" ") || "(routed default)"}`);
  }
}

if (import.meta.main) await main();
