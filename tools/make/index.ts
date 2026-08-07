#!/usr/bin/env bun
// Build the ROM, its overlays, and the asset tree from tracked sources.
//
// One entry point per domain, the way pokeemerald ships 11 binaries built from
// 82 source files. The modules under tools/make/ are implementation; this is
// the command. Every entry point must be documented in AGENTS.md -- enforced by
// tools/check/documented.ts.
//
//   bun tools/build.ts <subcommand> [args...]
//   bun tools/build.ts --list
import { join, dirname } from "node:path";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
export const SUBCOMMANDS = [
  "build_asm",
  "build_assets",
  "build_claimed",
  "build_full",
  "build_rom",
  "build_semantic",
] as const;

async function main(): Promise<void> {
  const [subcommand, ...rest] = Bun.argv.slice(2);
  if (subcommand === undefined || subcommand === "--list" || subcommand === "--help") {
    console.log("usage: build.ts <subcommand> [args...]");
    for (const name of SUBCOMMANDS) console.log(`  ${name}`);
    return;
  }
  if (subcommand === "--self-test") {
    // Widened: SUBCOMMANDS is a const tuple, so its length is a literal type and
    // a direct `=== 0` compares two literals that can never be equal.
    if ((SUBCOMMANDS as readonly string[]).length === 0) throw new Error("build has no subcommands");
    console.log(`build self-test ok (${SUBCOMMANDS.length} subcommands)`);
    return;
  }
  if (!(SUBCOMMANDS as readonly string[]).includes(subcommand)) {
    console.error(`unknown build subcommand: ${subcommand}`);
    console.error(`try one of: ${SUBCOMMANDS.join(", ")}`);
    process.exit(2);
  }
  // Spawn rather than import: each module owns its own argv handling and exit
  // code, and re-implementing that here would be the ceremony this replaces.
  const child = Bun.spawn(["bun", join(ROOT, "tools", "build", `${subcommand}.ts`), ...rest], {
    cwd: ROOT, stdout: "inherit", stderr: "inherit", stdin: "inherit",
  });
  process.exit(await child.exited);
}

if (import.meta.main) await main();
