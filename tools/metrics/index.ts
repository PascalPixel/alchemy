#!/usr/bin/env bun
// Progress, coverage maps, history, and residual audits.
//
// One entry point per domain, the way pokeemerald ships 11 binaries built from
// 82 source files. The modules under tools/metrics/ are implementation; this is
// the command. Every entry point must be documented in AGENTS.md -- enforced by
// tools/check/documented.ts.
//
//   bun tools/metrics.ts <subcommand> [args...]
//   bun tools/metrics.ts --list
import { join, dirname } from "node:path";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
export const SUBCOMMANDS = [
  "audit_residuals",
  "compare_roms",
  "coverage_map",
  "dashboard_server",
  "full_c_history",
  "full_c_progress",
] as const;

async function main(): Promise<void> {
  const [subcommand, ...rest] = Bun.argv.slice(2);
  if (subcommand === undefined || subcommand === "--list" || subcommand === "--help") {
    console.log("usage: metrics.ts <subcommand> [args...]");
    for (const name of SUBCOMMANDS) console.log(`  ${name}`);
    return;
  }
  if (subcommand === "--self-test") {
    if (SUBCOMMANDS.length === 0) throw new Error("metrics has no subcommands");
    console.log(`metrics self-test ok (${SUBCOMMANDS.length} subcommands)`);
    return;
  }
  if (!(SUBCOMMANDS as readonly string[]).includes(subcommand)) {
    console.error(`unknown metrics subcommand: ${subcommand}`);
    console.error(`try one of: ${SUBCOMMANDS.join(", ")}`);
    process.exit(2);
  }
  // Spawn rather than import: each module owns its own argv handling and exit
  // code, and re-implementing that here would be the ceremony this replaces.
  const child = Bun.spawn(["bun", join(ROOT, "tools", "metrics", `${subcommand}.ts`), ...rest], {
    cwd: ROOT, stdout: "inherit", stderr: "inherit", stdin: "inherit",
  });
  process.exit(await child.exited);
}

if (import.meta.main) await main();
