#!/usr/bin/env bun
// The alchemy-gcc fork: routing, flag sweeps, and the compiler-side corpus regression.
//
// One entry point per tool, the way pokeemerald ships 11 binaries built from 82
// source files. The modules beside this file are its implementation. Every tool
// must be documented in AGENTS.md -- enforced by tools/check/architecture.ts.
//
// Subcommands are READ FROM DISK, never listed here. A hand-written tuple was
// generated once and then went stale the moment modules moved: 92 of 130
// declared subcommands pointed at files that no longer existed, and every gate
// stayed green because nothing checked the list against the folder.
//
//   bun tools/compiler/index.ts <subcommand> [args...]
//   bun tools/compiler/index.ts --list
import { readdirSync } from "node:fs";
import { dirname, join } from "node:path";

const HERE = dirname(Bun.fileURLToPath(import.meta.url));
const ROOT = dirname(dirname(HERE));

// Tools migrated to Rust: the .ts file is gone, so it can no longer be found
// by scanning HERE. This map is the one unavoidable exception to "never
// hand-list subcommands" -- it points a still-valid subcommand name at the
// native binary that replaced its .ts file, not at a stale path.
const RUST_SUBCOMMANDS: Record<string, string> = {
  compiler_corpus_regression: "tools-rs/compiler-corpus-regression/target/release/compiler-corpus-regression",
  mode_cohort: "tools-rs/mode-cohort/target/release/mode-cohort",
  statement_order_sweep_main: "tools-rs/statement-order-sweep-main/target/release/statement-order-sweep-main",
};

export function subcommands(): string[] {
  return [
    ...readdirSync(HERE)
      .filter((name) => name.endsWith(".ts") && name !== "index.ts")
      .map((name) => name.slice(0, -3)),
    ...Object.keys(RUST_SUBCOMMANDS),
  ].sort();
}

async function main(): Promise<void> {
  const [subcommand, ...rest] = Bun.argv.slice(2);
  const available = subcommands();
  if (subcommand === undefined || subcommand === "--list" || subcommand === "--help") {
    console.log("usage: compiler <subcommand> [args...]");
    for (const name of available) console.log(`  ${name}`);
    return;
  }
  if (subcommand === "--self-test") {
    if (available.length === 0) throw new Error("compiler has no subcommands");
    console.log(`compiler self-test ok (${available.length} subcommands)`);
    return;
  }
  if (!available.includes(subcommand)) {
    console.error(`unknown compiler subcommand: ${subcommand}`);
    console.error(`try one of: ${available.join(", ")}`);
    process.exit(2);
  }
  // Spawn rather than import: each module owns its own argv handling and exit
  // code, and re-implementing that here would be the ceremony this replaces.
  const rustBinary = RUST_SUBCOMMANDS[subcommand];
  const child = rustBinary
    ? Bun.spawn([join(ROOT, rustBinary), ...rest], { cwd: ROOT, stdout: "inherit", stderr: "inherit", stdin: "inherit" })
    : Bun.spawn(["bun", join(HERE, `${subcommand}.ts`), ...rest], { cwd: ROOT, stdout: "inherit", stderr: "inherit", stdin: "inherit" });
  process.exit(await child.exited);
}

if (import.meta.main) await main();
