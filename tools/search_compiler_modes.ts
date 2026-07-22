#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md.
// Batch-probe ordinary source-level compiler modes against the ranked C-debt
// queue. Private binaries and diagnostics remain below ignored out/.
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { ROM_BASE, verifyCandidate } from "./match_m2c.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const OUT = join(ROOT, "out/decomp/compiler-modes");

interface Options { limit: number; jobs: number; queue: string; rom: string }
interface QueueItem { stem: string; candidate: string; diagnosis: { expected_size: number } }
interface Mode { name: string; flags: string[] }

const MODES: Mode[] = [
  { name: "o1", flags: ["-O1"] },
  { name: "unschedule-1", flags: ["-O2", "-fno-schedule-insns"] },
  { name: "unschedule-2", flags: ["-O2", "-fno-schedule-insns2"] },
  { name: "unscheduled", flags: ["-O2", "-fno-schedule-insns", "-fno-schedule-insns2"] },
  { name: "no-gcse", flags: ["-O2", "-fno-gcse"] },
  { name: "no-cse-follow", flags: ["-O2", "-fno-cse-follow-jumps"] },
  { name: "no-expensive", flags: ["-O2", "-fno-expensive-optimizations"] },
  { name: "fixed-r3", flags: ["-O2", "-ffixed-r3"] },
  { name: "no-inline", flags: ["-O2", "-fno-inline"] },
  { name: "no-peephole", flags: ["-O2", "-fno-peephole"] },
  { name: "no-strength-reduce", flags: ["-O2", "-fno-strength-reduce"] },
];

function optionsOf(arguments_: string[]): Options {
  const options: Options = {
    limit: 250,
    jobs: Math.min(12, navigator.hardwareConcurrency || 1),
    queue: join(ROOT, "out/decomp/queue.json"),
    rom: join(ROOT, "roms", "gs1-en.gba"),
  };
  for (let index = 0; index < arguments_.length; index++) {
    const argument = arguments_[index];
    if (argument === "--limit") options.limit = Number(arguments_[++index]);
    else if (argument === "--jobs") options.jobs = Number(arguments_[++index]);
    else if (argument === "--queue") options.queue = arguments_[++index];
    else if (argument === "--rom") options.rom = arguments_[++index];
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: search_compiler_modes.ts [--limit N] [--jobs N] [--queue FILE] [--rom FILE]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  if (!Number.isInteger(options.limit) || options.limit < 1 ||
      !Number.isInteger(options.jobs) || options.jobs < 1) throw new Error("limit and jobs must be positive integers");
  return options;
}

async function main(): Promise<void> {
  const options = optionsOf(Bun.argv.slice(2));
  const queue = JSON.parse(readFileSync(options.queue, "utf8")) as { items: QueueItem[] };
  const items = queue.items.filter((item) => existsSync(join(ROOT, item.candidate))).slice(0, options.limit);
  const rom = readFileSync(options.rom);
  mkdirSync(OUT, { recursive: true });
  const tasks = items.flatMap((item) => MODES.map((mode) => ({ item, mode })));
  const matches: Array<{ stem: string; mode: string; flags: string[]; source: string }> = [];
  let cursor = 0;
  let attempted = 0;
  let compiled = 0;
  async function worker(): Promise<void> {
    while (true) {
      const index = cursor++;
      if (index >= tasks.length) return;
      const { item, mode } = tasks[index];
      const source = join(ROOT, item.candidate);
      const scratch = join(OUT, "scratch", item.stem, mode.name);
      mkdirSync(scratch, { recursive: true });
      attempted++;
      try {
        const verification = await verifyCandidate(source, rom, scratch, mode.flags);
        compiled++;
        const expected = rom.subarray(
          Number.parseInt(item.stem, 16) - ROM_BASE,
          Number.parseInt(item.stem, 16) - ROM_BASE + item.diagnosis.expected_size,
        );
        if (verification.size === expected.length && verification.actual.equals(expected)) {
          const match = { stem: item.stem, mode: mode.name, flags: mode.flags, source: item.candidate };
          matches.push(match);
          console.log(`exact ${item.stem} mode=${mode.name}`);
        }
      } catch {}
    }
  }
  await Promise.all(Array.from({ length: Math.min(options.jobs, tasks.length) }, worker));
  const report = { format: 1, attempted, compiled, modes: MODES, matches };
  writeFileSync(join(OUT, "report.json"), JSON.stringify(report, null, 2) + "\n");
  console.log(`attempted=${attempted} compiled=${compiled} matches=${matches.length} report=${join(OUT, "report.json")}`);
}

if (import.meta.main) await main();
