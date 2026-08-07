# Working on Alchemy

Alchemy reconstructs *Golden Sun* (GBA) as byte-exact C. A change counts only
when the rebuilt bytes are identical to the released ROM.

There are ten tools. Nine are folders under `tools/` with an `index.ts` that
takes a subcommand, plus the smaller modules each owns; `self_test` is a single
file at the top. Shared code lives in
`tools/lib/` and belongs to no tool.

**Every tool must have a section on this page, or it must not exist.**
`tools/check/architecture.ts` enforces that, and `bun run lint` runs it. There is
no second list of tools anywhere: adding one without documenting it fails the
build, which is how the count stays honest. This repository reached 184 tools
because adding one cost nothing and nobody could see the total.

Run any tool with no arguments to list its subcommands.

`tools/scratch/` is exempt from every gate — `documented`, `architecture`,
`typecheck` (tsconfig `exclude`) and `self_test` discovery — and is git-ignored. Throwaway probes
belong there: the best result of the restructure came from a one-off script that
found 31 fork modes missing from the sweep list, and a rule demanding every tool
be documented would have refused it. Graduate what survives; delete the rest.

## The loop

1. **Pick an owner** — `bun tools/decomp/index.ts decomp_queue`, or `overlay twins --leads`.
2. **Write C** into `semantic/<owner>.c` until it compiles and reads correctly.
3. **Close the residual** — the two search axes below.
4. **Adopt it** — `bun tools/overlay/index.ts overlay_adopt`. This is what counts.
5. **Verify, then commit** — `bun run verify` is the only authority.

```bash
bun run verify   # rebuilds the ROM and every overlay (slow, authoritative)
bun run test     # lint + every tool's self-test (~7s)
bun run lint     # the gates alone, including this page's rule
bun run typecheck  # strict tsc, zero errors, gating via lint
```

`tsconfig.json` had been strict since it was written and nothing ever ran it, so
the first run reported 2,200 real errors. 2,114 of those came from
`noUncheckedIndexedAccess` alone, which is off for the whole tree rather than
waived per file: this code indexes ROM byte arrays constantly with bounds proven
by the surrounding loop, so the rule buys thousands of non-null assertions and
buries the few genuine cases. Turning it on is its own wave. The other 86 are
fixed and `typecheck` gates through `lint`.

It paid for itself immediately: it found four scheduler modes tagged
`family: "schedule"` against a union whose member is `"scheduler"`, so the rule
excluding scheduler modes from `old-agbcc` never excluded them and the search
generated configurations that compiler cannot honour. It also found an
`interface extends` a union, which erased a discriminant and silently untyped
every narrowing below it.

## verify

`bun run verify` — the gate. Rebuilds the ROM, every overlay, and the asset tree from tracked
sources and compares the result to the released image byte for byte. Nothing
else in this repository is authoritative: if `verify` is green the work is real,
and if it is red nothing you believe about the tree matters. Run it before every
commit. It is slow because it is complete.

## self_test

Runs every tool's `--self-test`, discovered by walking `tools/` rather than from
a list. The chain it replaced named 107 tools by hand and silently missed 31
that had tests nobody ran; discovery removes the drift, because forgetting to
register a tool is no longer possible. Takes about seven seconds for all of them
in parallel.

## make

Builds the ROM and its parts: `build_rom` for the whole image, and `build_asm`,
`build_assets`, `build_claimed`, `build_full`, `build_semantic` for the stages.
This is the pipeline `tools/lib/verify.ts` drives, exposed separately so you can rebuild one
stage while iterating instead of paying for the full gate each time. Named
`make` because `build`, `rom` and `dist` are all publication-blocked directory
names.

## overlay

Everything about the 96 code overlays, which hold roughly 60% of the executable.
`tools/lib/overlay_disasm.ts` and `overlay_show` read one owner, `overlay_inventory` builds
the discovery queue, `overlay_adopt` installs a byte-exact reconstruction and is
the step that moves the counter, and `overlay_twins --leads` finds families where
reading one owner lets you transpose the rest. Overlays reuse whole routines
across maps, so those families are the cheapest bytes on the board.

## assets

Extracts and rebuilds every ROM asset format: graphics, tilemaps, sprite banks,
maps, text, audio sequences, and the bespoke Camelot containers. This is our
`gbagfx` equivalent, except Golden Sun's formats are per-resource rather than
standard GBA ones, so it is a module per format instead of one converter with
flags. Converted assets (PNG, WAV, JSON descriptions) are tracked and
publishable, as in pret's decomps; what stays out of git is the ROM image itself
and bulk dumps — see PROVENANCE.md and `tools/check/check_publication.ts`.

## compiler

The `alchemy-gcc` fork and its routing. `tools/lib/alchemy_gcc.ts` owns the flag tables, the
per-source routing sets, and the approved cc1 digests; `tools/lib/mode_sweep.ts` searches
compiler configurations for one candidate; `mode_cohort` runs a hypothesis
across many owners at once. The routing sets are append-only registries of
independent discoveries — never resolve a merge in them by taking one side.

When a residual survives both search axes it is usually a scheduler tie, which
`tools/lib/mode_sweep.ts` flags as `escalation: compiler-rtl-scheduler-trace`. The RTL
readers that answer it (`tools/lib/rtl_sexpr.ts`, `tools/lib/rtl_insn.ts`, `tools/lib/rtl_align.ts`, `tools/lib/rtl_schedule.ts`,
`tools/lib/thumb_disasm.ts`, `tools/lib/candidate_explain.ts`) live in `tools/lib/` because several tools
share them; run them directly. They parse the fork's own dumps and reproduce its
real scheduler tier order rather than modelling it.

## search

Finds a source form that compiles byte-exact, holding the compiler fixed.
`shape_sweep` applies transforms seeded from LAWS.md and is bounded and
deterministic, so "it found nothing" is a real result; the annealers
(`permute_v1`, `permute_overlay`, `tools/assets/alchemist.ts`) search far wider but
stochastically, and their measured yield is low, so they are a rescue tool under
the bounded-probe rule in HANDOVER.md. Never promote a near-match.

## decomp

Picks and diagnoses owners. `decomp_queue` ranks candidates by expected value
and is resumable, `decomp_diagnose` explains why a specific candidate misses,
and `tools/lib/match_m2c.ts` and `tools/lib/integrate_matches.ts` handle drafting and installation. Start
here when you do not already know which owner you are working on.

## semantic

Owns the semantic-C tree — readable reconstructions that are not yet byte-exact.
`semantic_queue` lists what is ready to attempt, `semantic_owner_scope` checks an
owner covers exactly its region, and `semantic_superseded` retires sources that
adoption has replaced. Semantic C is the staging ground: it is how an owner gets
understood before it gets matched.

## metrics

Measures and draws progress. `tools/lib/full_c_progress.ts` produces the byte counts every
commit subject must carry, `coverage_map` renders the README's box trees, and
`audit_residuals` accounts for what remains. Progress is byte-exact executable
bytes, never function counts: 69% of our owners are converted but only 23.5% of
the bytes, because the remaining owners are far larger than the ones already
closed.

## check

The gates, run by `bun run lint` and by the git hooks. `no_asm_c` enforces the
hardest rule in the repository (C and headers may never use an assembly escape
hatch, in any spelling), `check_publication` keeps ROM bytes and generated
directories out of anything publishable, `check_commit_progress` enforces the
`[C exact/total bytes]` commit subject, `check_sanctum` gates the sealed-owner
ledger, `source_citations` keeps the provenance comments in reconstructed
sources pointing at tools that exist and proves every retirement row in
PROVENANCE.md actually recovers, and `architecture` enforces the shape of `tools/` itself: every tool
documented on this page, no tool importing another tool, every module reachable,
and every import resolving. Each of those rules was an ad-hoc script first —
a check worth running twice belongs in here, not in a shell one-liner.

## Ledgers

| File | Owns |
|---|---|
| [LAWS.md](LAWS.md) | compiler behaviour we proved, with its evidence |
| [SANCTUM.md](SANCTUM.md) | owners withdrawn from routine attack, both axes spent |
| [PROVENANCE.md](PROVENANCE.md) | the clean-room boundary |
| `metrics/gs1-en-progress.json` | the headline byte counts |

## Hard rules

- **Never `asm(...)`** in C or headers, in any spelling, including fixed-register
  bindings and empty barriers. Byte equality never overrides this.
- **Never copy game-specific code** from another decompilation project. Tooling
  and methodology are reusable; function bodies, names, types and comments are
  not. See [PROVENANCE.md](PROVENANCE.md).
- **Commit subjects end with `[C exact/total bytes]`** matching the regenerated
  metrics. A deliberate regression needs `metrics: revert reduces Full-C`; a
  changed denominator needs `metrics: correct executable denominator`.
- **Never resolve a merge in the compiler routing tables or the fork's `arm.c`
  by taking one side.** They are append-only registries of independent
  discoveries. Three-way merge against the merge-base, then resolve per entry —
  a deletion carries intent too.
- **Adopt from outside `exact/`.** `overlay_adopt` assembles the overlay as a
  pre-flight check, so a source already sitting in `exact/` fails on its own
  baseline. Always rebuild the inventory before regenerating progress.

## The compiler fork

`alchemy-gcc/` is ours to edit, not a third-party dependency. `dist/` holds the
built toolchain and every worktree symlinks to one canonical copy, so all lanes
compile with the same binary — three different `cc1` builds across three
worktrees is how "witnessed byte-exact" quietly stops meaning the same thing.
Rebuilding means re-pinning the cc1 digest from a green verify.

Do not run `git submodule` commands from inside a worktree: it rewrites the
shared `core.worktree` and breaks git in the main checkout.

## Where the tools went

Paths moved in the 2026-08-07 restructure. If a helper of yours hard-codes an
old path, this is the mapping; `bun run lint` catches stale paths in documents
and in source provenance comments, but not in your own scripts.

| Before | Now |
|---|---|
| `tools/alchemist.ts` | `tools/assets/alchemist.ts` |
| `tools/alchemy_gcc.ts` | `tools/lib/alchemy_gcc.ts` |
| `tools/alignment_tail.ts` | `tools/lib/alignment_tail.ts` |
| `tools/archive_asset.ts` | `tools/make/archive_asset.ts` |
| `tools/asset_paths.ts` | `tools/lib/asset_paths.ts` |
| `tools/audio_engine_data.ts` | `tools/make/audio_engine_data.ts` |
| `tools/audio_wave.ts` | `tools/make/audio_wave.ts` |
| `tools/audit_residuals.ts` | `tools/metrics/audit_residuals.ts` |
| `tools/battle_effect_data.ts` | `tools/make/battle_effect_data.ts` |
| `tools/bl_site_symbols.ts` | `tools/assets/bl_site_symbols.ts` |
| `tools/build_asm.ts` | `tools/make/build_asm.ts` |
| `tools/build_assets.ts` | `tools/make/build_assets.ts` |
| `tools/build_claimed.ts` | `tools/make/build_claimed.ts` |
| `tools/build_full.ts` | `tools/make/build_full.ts` |
| `tools/build_rom.ts` | `tools/make/build_rom.ts` |
| `tools/build_semantic.ts` | `tools/make/build_semantic.ts` |
| `tools/byte_henkan.ts` | `tools/make/byte_henkan.ts` |
| `tools/byte_value_regions.ts` | `tools/make/byte_value_regions.ts` |
| `tools/cache_entry.ts` | `tools/lib/cache_entry.ts` |
| `tools/cache_key_lint.ts` | `tools/check/cache_key_lint.ts` |
| `tools/candidate_explain.ts` | `tools/lib/candidate_explain.ts` |
| `tools/candidate_show.ts` | `tools/lib/candidate_show.ts` |
| `tools/canonical_json.ts` | `tools/lib/canonical_json.ts` |
| `tools/character_catalog.ts` | `tools/make/character_catalog.ts` |
| `tools/check_commit_progress.ts` | `tools/check/check_commit_progress.ts` |
| `tools/check_publication.ts` | `tools/check/check_publication.ts` |
| `tools/chiiki_map_resources.ts` | `tools/make/chiiki_map_resources.ts` |
| `tools/compare_roms.ts` | `tools/metrics/compare_roms.ts` |
| `tools/compiler_corpus_regression.ts` | `tools/compiler/compiler_corpus_regression.ts` |
| `tools/compose_scene.ts` | `tools/lib/compose_scene.ts` |
| `tools/core_retained_audit.ts` | `tools/check/core_retained_audit.ts` |
| `tools/coverage_map.ts` | `tools/metrics/coverage_map.ts` |
| `tools/dashboard/client.ts` | `tools/metrics/dashboard/client.ts` |
| `tools/dashboard_server.ts` | `tools/metrics/dashboard_server.ts` |
| `tools/decomp_constraints.ts` | `tools/search/decomp_constraints.ts` |
| `tools/decomp_diagnose.ts` | `tools/decomp/decomp_diagnose.ts` |
| `tools/decomp_queue.ts` | `tools/decomp/decomp_queue.ts` |
| `tools/decomp_targets.ts` | `tools/lib/decomp_targets.ts` |
| `tools/discover.ts` | `tools/lib/discover.ts` |
| `tools/early_runtime_data.ts` | `tools/make/early_runtime_data.ts` |
| `tools/encounter_data.ts` | `tools/make/encounter_data.ts` |
| `tools/exact_reading_list.ts` | `tools/overlay/exact_reading_list.ts` |
| `tools/executable_gap_sources.ts` | `tools/make/executable_gap_sources.ts` |
| `tools/export_asset.ts` | `tools/lib/export_asset.ts` |
| `tools/extract_resource.ts` | `tools/lib/extract_resource.ts` |
| `tools/f0_archive.ts` | `tools/make/f0_archive.ts` |
| `tools/full_c_history.ts` | `tools/metrics/full_c_history.ts` |
| `tools/full_c_progress.ts` | `tools/lib/full_c_progress.ts` |
| `tools/gba_header.ts` | `tools/make/gba_header.ts` |
| `tools/generated_files.ts` | `tools/lib/generated_files.ts` |
| `tools/import_asset.ts` | `tools/lib/import_asset.ts` |
| `tools/indexed_still.ts` | `tools/make/indexed_still.ts` |
| `tools/integrate_matches.ts` | `tools/lib/integrate_matches.ts` |
| `tools/kind1_map_grid.ts` | `tools/make/kind1_map_grid.ts` |
| `tools/kind2_resource.ts` | `tools/make/kind2_resource.ts` |
| `tools/kind2_resource_series.ts` | `tools/make/kind2_resource_series.ts` |
| `tools/late_runtime_data.ts` | `tools/lib/late_runtime_data.ts` |
| `tools/late_runtime_residual.ts` | `tools/make/late_runtime_residual.ts` |
| `tools/localization_font.ts` | `tools/make/localization_font.ts` |
| `tools/localization_tables.ts` | `tools/make/localization_tables.ts` |
| `tools/map_container_components.ts` | `tools/make/map_container_components.ts` |
| `tools/map_load_table.ts` | `tools/lib/map_load_table.ts` |
| `tools/match_m2c.ts` | `tools/lib/match_m2c.ts` |
| `tools/message_archive.ts` | `tools/make/message_archive.ts` |
| `tools/midi_roundtrip.ts` | `tools/lib/midi_roundtrip.ts` |
| `tools/midi_sequence.ts` | `tools/lib/midi_sequence.ts` |
| `tools/mode_cohort.ts` | `tools/compiler/mode_cohort.ts` |
| `tools/mode_sweep.ts` | `tools/lib/mode_sweep.ts` |
| `tools/music.ts` | `tools/make/music.ts` |
| `tools/music_residuals.ts` | `tools/make/music_residuals.ts` |
| `tools/music_sequence.ts` | `tools/lib/music_sequence.ts` |
| `tools/namae_nyuuryoku.ts` | `tools/make/namae_nyuuryoku.ts` |
| `tools/no_asm_c.ts` | `tools/check/no_asm_c.ts` |
| `tools/overlay_adopt.ts` | `tools/overlay/overlay_adopt.ts` |
| `tools/overlay_call_order_check.ts` | `tools/overlay/overlay_call_order_check.ts` |
| `tools/overlay_call_targets.ts` | `tools/lib/overlay_call_targets.ts` |
| `tools/overlay_candidate_rank.ts` | `tools/overlay/overlay_candidate_rank.ts` |
| `tools/overlay_certify.ts` | `tools/overlay/overlay_certify.ts` |
| `tools/overlay_disasm.ts` | `tools/lib/overlay_disasm.ts` |
| `tools/overlay_driver.ts` | `tools/overlay/overlay_driver.ts` |
| `tools/overlay_entry.ts` | `tools/overlay/overlay_entry.ts` |
| `tools/overlay_gaps.ts` | `tools/overlay/overlay_gaps.ts` |
| `tools/overlay_inventory.ts` | `tools/overlay/overlay_inventory.ts` |
| `tools/overlay_mode_cohort.ts` | `tools/overlay/overlay_mode_cohort.ts` |
| `tools/overlay_published.ts` | `tools/lib/overlay_published.ts` |
| `tools/overlay_show.ts` | `tools/overlay/overlay_show.ts` |
| `tools/overlay_showcase.ts` | `tools/overlay/overlay_showcase.ts` |
| `tools/overlay_twins.ts` | `tools/overlay/overlay_twins.ts` |
| `tools/overlay_unindexed.ts` | `tools/overlay/overlay_unindexed.ts` |
| `tools/pairtable.ts` | `tools/make/pairtable.ts` |
| `tools/permute_m2c.ts` | `tools/lib/permute_m2c.ts` |
| `tools/permute_overlay.ts` | `tools/search/permute_overlay.ts` |
| `tools/permute_v1.ts` | `tools/search/permute_v1.ts` |
| `tools/remaining_survey.ts` | `tools/decomp/remaining_survey.ts` |
| `tools/resource_01c.ts` | `tools/make/resource_01c.ts` |
| `tools/resource_3ce.ts` | `tools/make/resource_3ce.ts` |
| `tools/resource_5.ts` | `tools/make/resource_5.ts` |
| `tools/resource_byte_canvases.ts` | `tools/make/resource_byte_canvases.ts` |
| `tools/resource_d1_d3.ts` | `tools/make/resource_d1_d3.ts` |
| `tools/resource_directory.ts` | `tools/make/resource_directory.ts` |
| `tools/rtl_align.ts` | `tools/lib/rtl_align.ts` |
| `tools/rtl_insn.ts` | `tools/lib/rtl_insn.ts` |
| `tools/rtl_schedule.ts` | `tools/lib/rtl_schedule.ts` |
| `tools/rtl_sexpr.ts` | `tools/lib/rtl_sexpr.ts` |
| `tools/runtime_support_data.ts` | `tools/make/runtime_support_data.ts` |
| `tools/search_compiler_modes.ts` | `tools/search/search_compiler_modes.ts` |
| `tools/semantic_owner_scope.ts` | `tools/semantic/semantic_owner_scope.ts` |
| `tools/semantic_queue.ts` | `tools/semantic/semantic_queue.ts` |
| `tools/semantic_regions_sync.ts` | `tools/lib/semantic_regions_sync.ts` |
| `tools/semantic_superseded.ts` | `tools/semantic/semantic_superseded.ts` |
| `tools/sentou_gamen_data.ts` | `tools/make/sentou_gamen_data.ts` |
| `tools/sentou_hyouji.ts` | `tools/make/sentou_hyouji.ts` |
| `tools/sentou_kouka_runtime.ts` | `tools/make/sentou_kouka_runtime.ts` |
| `tools/sentou_menu_data.ts` | `tools/make/sentou_menu_data.ts` |
| `tools/sentou_resources.ts` | `tools/make/sentou_resources.ts` |
| `tools/simple_resources.ts` | `tools/make/simple_resources.ts` |
| `tools/skip_sprite_archive.ts` | `tools/make/skip_sprite_archive.ts` |
| `tools/staff_roll.ts` | `tools/make/staff_roll.ts` |
| `tools/statement_order_sweep_main.ts` | `tools/compiler/statement_order_sweep_main.ts` |
| `tools/static_sprite_bank.ts` | `tools/lib/static_sprite_bank.ts` |
| `tools/static_sprite_series.ts` | `tools/make/static_sprite_series.ts` |
| `tools/text_bg.ts` | `tools/lib/text_bg.ts` |
| `tools/thumb_disasm.ts` | `tools/lib/thumb_disasm.ts` |
| `tools/tile_objects.ts` | `tools/lib/tile_objects.ts` |
| `tools/tilemap.ts` | `tools/lib/tilemap.ts` |
| `tools/title_resources.ts` | `tools/make/title_resources.ts` |
| `tools/tokushu_map_resources.ts` | `tools/make/tokushu_map_resources.ts` |
| `tools/verify.ts` | `tools/lib/verify.ts` |
| `tools/wordstream.ts` | `tools/make/wordstream.ts` |
| `tools/zlib.ts` | `tools/lib/zlib.ts` |

53 tools were deleted rather than moved; PROVENANCE.md lists the six that
source comments still cite, and `git show 1e01df6aa:<path>` recovers any of them.
