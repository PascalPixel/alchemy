#!/usr/bin/env bun
// Extract and rebuild every ROM asset format: graphics, maps, audio, text, tables.
//
// One entry point per domain, the way pokeemerald ships 11 binaries built from
// 82 source files. The modules under tools/assets/ are implementation; this is
// the command. Every entry point must be documented in AGENTS.md -- enforced by
// tools/check/documented.ts.
//
//   bun tools/assets.ts <subcommand> [args...]
//   bun tools/assets.ts --list
import { join, dirname } from "node:path";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
export const SUBCOMMANDS = [
  "alignment_tail",
  "archive_asset",
  "asset_paths",
  "audio_engine_data",
  "audio_wave",
  "battle_effect_data",
  "bl_site_symbols",
  "byte_henkan",
  "byte_value_regions",
  "canonical_json",
  "character_catalog",
  "chiiki_map_resources",
  "compose_scene",
  "early_runtime_data",
  "encounter_data",
  "executable_gap_sources",
  "export_asset",
  "extract_resource",
  "f0_archive",
  "gba_header",
  "generated_files",
  "import_asset",
  "indexed_still",
  "kind1_map_grid",
  "kind2_resource",
  "kind2_resource_series",
  "late_runtime_data",
  "late_runtime_residual",
  "localization_font",
  "localization_tables",
  "map_container_components",
  "map_load_table",
  "message_archive",
  "midi_roundtrip",
  "midi_sequence",
  "music",
  "music_residuals",
  "music_sequence",
  "namae_nyuuryoku",
  "pairtable",
  "resource_01c",
  "resource_3ce",
  "resource_5",
  "resource_byte_canvases",
  "resource_d1_d3",
  "resource_directory",
  "runtime_support_data",
  "sentou_gamen_data",
  "sentou_hyouji",
  "sentou_kouka_runtime",
  "sentou_menu_data",
  "sentou_resources",
  "simple_resources",
  "skip_sprite_archive",
  "staff_roll",
  "static_sprite_bank",
  "static_sprite_series",
  "text_bg",
  "tile_objects",
  "tilemap",
  "title_resources",
  "tokushu_map_resources",
  "wordstream",
  "zlib",
] as const;

async function main(): Promise<void> {
  const [subcommand, ...rest] = Bun.argv.slice(2);
  if (subcommand === undefined || subcommand === "--list" || subcommand === "--help") {
    console.log("usage: assets.ts <subcommand> [args...]");
    for (const name of SUBCOMMANDS) console.log(`  ${name}`);
    return;
  }
  if (subcommand === "--self-test") {
    if (SUBCOMMANDS.length === 0) throw new Error("assets has no subcommands");
    console.log(`assets self-test ok (${SUBCOMMANDS.length} subcommands)`);
    return;
  }
  if (!(SUBCOMMANDS as readonly string[]).includes(subcommand)) {
    console.error(`unknown assets subcommand: ${subcommand}`);
    console.error(`try one of: ${SUBCOMMANDS.join(", ")}`);
    process.exit(2);
  }
  // Spawn rather than import: each module owns its own argv handling and exit
  // code, and re-implementing that here would be the ceremony this replaces.
  const child = Bun.spawn(["bun", join(ROOT, "tools", "assets", `${subcommand}.ts`), ...rest], {
    cwd: ROOT, stdout: "inherit", stderr: "inherit", stdin: "inherit",
  });
  process.exit(await child.exited);
}

if (import.meta.main) await main();
