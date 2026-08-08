#!/usr/bin/env bun
// Build the ROM, its overlays, and the asset tree from tracked sources.
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
//   bun tools/make/index.ts <subcommand> [args...]
//   bun tools/make/index.ts --list
import { readdirSync } from "node:fs";
import { dirname, join } from "node:path";

const HERE = dirname(Bun.fileURLToPath(import.meta.url));
const ROOT = dirname(dirname(HERE));

const RUST_SUBCOMMANDS: Record<string, string> = {
  audio_engine_data: "tools-rs/audio-engine-data/target/release/audio-engine-data",
  audio_wave: "tools-rs/audio-wave/target/release/audio-wave",
  archive_asset: "tools-rs/archive-asset/target/release/archive-asset",
  battle_effect_data: "tools-rs/battle-effect-data/target/release/battle-effect-data",
  build_asm: "tools-rs/build-asm/target/release/build-asm",
  build_assets: "tools-rs/build-assets/target/release/build-assets",
  build_claimed: "tools-rs/build-claimed/target/release/build-claimed",
  build_full: "tools-rs/build-full/target/release/build-full",
  build_rom: "tools-rs/build-rom/target/release/build-rom",
  build_semantic: "tools-rs/build-semantic/target/release/build-semantic",
  byte_value_regions: "tools-rs/target/release/byte-value-regions",
  character_catalog: "tools-rs/character-catalog/target/release/character-catalog",
  encounter_data: "tools-rs/encounter-data/target/release/encounter-data",
  early_runtime_data: "tools-rs/early-runtime-data/target/release/early-runtime-data",
  gba_header: "tools-rs/gba-header/target/release/gba-header",
  byte_henkan: "tools-rs/byte-henkan/target/release/byte-henkan",
  localization_tables: "tools-rs/localization-tables/target/release/localization-tables",
  late_runtime_residual: "tools-rs/late-runtime-residual/target/release/late-runtime-residual",
  localization_font: "tools-rs/localization-font/target/release/localization-font",
  kind1_map_grid: "tools-rs/kind1-map-grid/target/release/kind1-map-grid",
  map_container_components: "tools-rs/map-container-components/target/release/map-container-components",
  message_archive: "tools-rs/message-archive/target/release/message_archive",
  music: "tools-rs/music/target/release/music",
  pairtable: "tools-rs/target/release/pairtable",
  resource_directory: "tools-rs/resource-directory/target/release/resource-directory",
  resource_01c: "tools-rs/resource-01c/target/release/resource-01c",
  resource_3ce: "tools-rs/resource-3ce/target/release/resource-3ce",
  resource_d1_d3: "tools-rs/resource-d1-d3/target/release/resource-d1-d3",
  resource_byte_canvases: "tools-rs/resource-byte-canvases/target/release/resource-byte-canvases",
  resource_5: "tools-rs/resource-5/target/release/resource-5",
  runtime_support_data: "tools-rs/runtime-support-data/target/release/runtime-support-data",
  simple_resources: "tools-rs/simple-resources/target/release/simple-resources",
  wordstream: "tools-rs/target/release/wordstream",
};

export function subcommands(): string[] {
  return [
      ...readdirSync(HERE)
      .filter((name) => name.endsWith(".ts") && name !== "index.ts" && name !== "character_catalog.ts" && name !== "simple_resources.ts" && name !== "localization_font.ts" && name !== "resource_3ce.ts" && name !== "resource_d1_d3.ts" && name !== "resource_byte_canvases.ts" && name !== "resource_5.ts" && name !== "kind1_map_grid.ts" && name !== "map_container_components.ts" && name !== "runtime_support_data.ts")
      .map((name) => name.slice(0, -3)),
    ...Object.keys(RUST_SUBCOMMANDS),
  ].sort();
}

async function main(): Promise<void> {
  const [subcommand, ...rest] = Bun.argv.slice(2);
  const available = subcommands();
  if (subcommand === undefined || subcommand === "--list" || subcommand === "--help") {
    console.log("usage: make <subcommand> [args...]");
    for (const name of available) console.log(`  ${name}`);
    return;
  }
  if (subcommand === "--self-test") {
    if (available.length === 0) throw new Error("make has no subcommands");
    console.log(`make self-test ok (${available.length} subcommands)`);
    return;
  }
  if (!available.includes(subcommand)) {
    console.error(`unknown make subcommand: ${subcommand}`);
    console.error(`try one of: ${available.join(", ")}`);
    process.exit(2);
  }
  // Spawn rather than import: each module owns its own argv handling and exit
  // code, and re-implementing that here would be the ceremony this replaces.
  const rustBinary = RUST_SUBCOMMANDS[subcommand];
  const child = rustBinary
    ? Bun.spawn([join(ROOT, rustBinary), ...rest], {
        cwd: ROOT, stdout: "inherit", stderr: "inherit", stdin: "inherit",
      })
    : Bun.spawn(["bun", join(HERE, `${subcommand}.ts`), ...rest], {
    cwd: ROOT, stdout: "inherit", stderr: "inherit", stdin: "inherit",
  });
  process.exit(await child.exited);
}

if (import.meta.main) await main();
