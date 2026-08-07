#!/usr/bin/env bun
// Everything about the 96 code overlays: disassemble, inventory, adopt, compare, certify.
//
// One entry point per domain, the way pokeemerald ships 11 binaries built from
// 82 source files. The modules under tools/overlay/ are implementation; this is
// the command. Every entry point must be documented in AGENTS.md -- enforced by
// tools/check/documented.ts.
//
//   bun tools/overlay.ts <subcommand> [args...]
//   bun tools/overlay.ts --list
import { join, dirname } from "node:path";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
export const SUBCOMMANDS = [
  "overlay_adopt",
  "overlay_call_order_check",
  "overlay_call_targets",
  "overlay_candidate_rank",
  "overlay_certify",
  "overlay_disasm",
  "overlay_driver",
  "overlay_entry",
  "overlay_gaps",
  "overlay_inventory",
  "overlay_mode_cohort",
  "overlay_published",
  "overlay_show",
  "overlay_showcase",
  "overlay_twins",
  "overlay_unindexed",
] as const;

async function main(): Promise<void> {
  const [subcommand, ...rest] = Bun.argv.slice(2);
  if (subcommand === undefined || subcommand === "--list" || subcommand === "--help") {
    console.log("usage: overlay.ts <subcommand> [args...]");
    for (const name of SUBCOMMANDS) console.log(`  ${name}`);
    return;
  }
  if (subcommand === "--self-test") {
    // Widened: SUBCOMMANDS is a const tuple, so its length is a literal type and
    // a direct `=== 0` compares two literals that can never be equal.
    if ((SUBCOMMANDS as readonly string[]).length === 0) throw new Error("overlay has no subcommands");
    console.log(`overlay self-test ok (${SUBCOMMANDS.length} subcommands)`);
    return;
  }
  if (!(SUBCOMMANDS as readonly string[]).includes(subcommand)) {
    console.error(`unknown overlay subcommand: ${subcommand}`);
    console.error(`try one of: ${SUBCOMMANDS.join(", ")}`);
    process.exit(2);
  }
  // Spawn rather than import: each module owns its own argv handling and exit
  // code, and re-implementing that here would be the ceremony this replaces.
  const child = Bun.spawn(["bun", join(ROOT, "tools", "overlay", `${subcommand}.ts`), ...rest], {
    cwd: ROOT, stdout: "inherit", stderr: "inherit", stdin: "inherit",
  });
  process.exit(await child.exited);
}

if (import.meta.main) await main();
