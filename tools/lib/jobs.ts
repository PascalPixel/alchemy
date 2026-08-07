#!/usr/bin/env bun
// How many things this repository may run at once.
//
// 80% of the cores, never all of them. Every parallel runner here spawns a full
// Bun process that imports a large part of the tool tree, so "cores - 2" is not
// a 2-core reserve, it is 16 concurrent runtimes. Doing that to probe 95
// subcommands drove an 18-core, 128 GB machine into swap and a watchdog panic.
// The headroom is not for the scheduler, it is for the rest of the machine.
//
//   bun tools/lib/jobs.ts             # print the cap
//   bun tools/lib/jobs.ts --self-test
import { availableParallelism } from "node:os";

export const SHARE = 0.8;

export function jobsFor(cores: number, share = SHARE): number {
  return Math.max(1, Math.floor(cores * share));
}

// Callers accept --jobs; this clamps whatever they were given. An explicit
// number above the cap is still a mistake, just a deliberate-looking one.
export function resolveJobs(requested?: number, cores = availableParallelism()): number {
  const cap = jobsFor(cores);
  if (requested === undefined || !Number.isInteger(requested) || requested < 1) return cap;
  return Math.min(requested, cap);
}

function selfTest(): void {
  if (jobsFor(18) !== 14) throw new Error(`18 cores must give 14, got ${jobsFor(18)}`);
  if (jobsFor(1) !== 1) throw new Error("a single core must still run one job");
  if (jobsFor(2) !== 1) throw new Error("two cores must give one job");
  if (resolveJobs(4, 18) !== 4) throw new Error("a request under the cap must be honoured");
  if (resolveJobs(64, 18) !== 14) throw new Error("a request over the cap must be clamped");
  if (resolveJobs(0, 18) !== 14) throw new Error("a nonsense request must fall back to the cap");
  if (resolveJobs(undefined, 18) !== 14) throw new Error("no request must give the cap");
  console.log("jobs self-test ok");
}

if (import.meta.main) {
  if (Bun.argv.includes("--self-test")) selfTest();
  else console.log(`${resolveJobs()} of ${availableParallelism()} cores (${SHARE * 100}%)`);
}
