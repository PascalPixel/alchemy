# Native command guide

Alchemy's Rust crates form one tooling layer. The root Makefile is the workflow
facade: its `dispatch-<group>` targets invoke the nine dispatcher groups, while
its hyphenated build and check targets invoke native implementations directly.
Contributors normally use that facade rather than treating every Cargo package
as a separate product:

```sh
cargo run --release --manifest-path tools/dispatch/Cargo.toml -- \
  <group> <command> [arguments]
```

Use `<group> --list` to list commands and `<group> <command> --help` for the
command contract. The dispatcher registry is the public command inventory;
`architecture` compares it with Cargo metadata, and the explicit non-public
policy accounts for diagnostics, support binaries, and benches. For example,
`make dispatch-semantic ARGS='semantic_queue --help'` invokes a registered
command; `make build-semantic` invokes the repository's semantic build stage.

## Recommended path

```text
read the target assembly and write the fact sheet down
        -> semantic_queue / overlay_twins
        -> reconstruct and review semantic C (compiler input, not output)
        -> decomp_diagnose / overlay_show
        -> shape_sweep
        -> mode_sweep or mode_cohort, only with ledger evidence
        -> alchemy_permuter, last mile only, behind the CONTRIBUTING.md gate
        -> integrate_matches (main image) or overlay_adopt (code overlay)
        -> make verify
```

The list below is a map, not an instruction to run every search. Start with the
smallest command that can answer the current question, and escalate only with a
new structural fact in hand: two stalled searches on one axis end that axis.

## assets

| Command | Purpose |
|---|---|
| `bl_site_symbols` | Recover and audit branch-and-link symbol evidence; scan mode reports every failure and omission. |
| `extract_resource` | Decode one resource by ID or address and optionally verify the round trip. |
| `export_asset` | Render supported binary graphics, palette, byte, and RGBA forms into source assets. |
| `import_asset` | Convert supported PNG, indexed-image, and MIDI sources into build inputs. |
| `tilemap` | Round-trip the textual tilemap source format. |

Low-level container codecs used only by `build_assets` remain internal so their
subprocess protocol is not mistaken for the contributor interface.

## check

| Command | Purpose |
|---|---|
| `architecture` | Require every crate, binary, dispatch target, path, and classification to be valid and reachable. |
| `cache_key_lint` | Ensure expensive cached results include all inputs that can change their meaning. |
| `check_commit_progress` | Compare the staged progress report with the required Exact-C percentage subject prefix. |
| `check_publication` | Reject ROMs, build products, opaque dumps, credentials, and other non-publishable content. |
| `check_sanctum` | Validate sealed owners or print the one-axis source-shape queue. |
| `core_retained_audit` | Verify that retained main-image assembly has explicit, complete justification. |
| `documented` | Keep public command groups and the command catalog synchronized with the registry. |
| `no_asm_c` | Reject inline assembly, fixed-register bindings, and assembly barriers in C and headers. |
| `source_citations` | Check live tool citations and recoverable retired-tool provenance. |

Checks fail when they scan nothing. A zero exit status must mean that the
intended corpus was actually inspected.

## compiler

| Command | Purpose |
|---|---|
| `compiler_corpus_regression` | Recompile routed `exact/*.c` owners and require zero byte regressions. |
| `mode_cohort` | Test one compiler hypothesis across a bounded owner cohort. |
| `mode_sweep` | Search the approved compiler/flag matrix for one fixed source candidate. |
| `statement_order_sweep_main` | Test bounded statement-order forms for a main-image candidate. |

Compiler searches are evidence tools, not source generators. Pin the source,
route, bundle digest, and candidate span before interpreting a result.

## decomp

| Command | Purpose |
|---|---|
| `discover` | Produce the function/instruction/call discovery report from a local ROM. |
| `decomp_diagnose` | Compile and explain a numeric main-image candidate's localized residual; `--agent-brief` emits the bounded worker contract and canonical score. |
| `integrate_matches` | Gate main-image `src_<address>.c` drafts and install byte-identical matches into `exact/` only with explicit apply mode. |
| `remaining_survey` | Survey remaining executable regions from any current working directory. |

Discovery reports can be large. Write them under ignored `out/` or an operating
system temporary directory; never publish ROM-derived reports.

## make

The `make` rows below are the dispatcher group's registered native operations;
they are distinct from the root `Makefile` command. Prefer the root
`Makefile`'s `make <stage>` targets for workflows unless developing a command
itself.

| Commands | Purpose |
|---|---|
| `build_asm`, `build_assets`, `build_claimed`, `build_full`, `build_rom`, `build_semantic` | Build the smallest authoritative stage needed for the current check. |
| `archive_asset`, `byte_henkan`, `byte_value_regions`, `f0_archive`, `gba_header`, `indexed_still`, `message_archive`, `pairtable`, `resource_directory`, `wordstream` | Generic ROM-layout, stream, archive, image, and table builders used by tracked plans. |
| `audio_engine_data`, `audio_wave`, `music`, `music_residuals` | Rebuild audio engines, samples, sequences, and residual audio ownership. |
| `battle_effect_data`, `encounter_data`, `sentou_gamen_data`, `sentou_hyouji`, `sentou_kouka_runtime`, `sentou_menu_data`, `sentou_resources` | Rebuild battle and encounter data families. |
| `character_catalog`, `localization_font`, `localization_tables`, `namae_nyuuryoku`, `staff_roll` | Rebuild character, language, name-entry, font, and credits data. |
| `chiiki_map_resources`, `kind1_map_grid`, `map_container_components`, `tokushu_map_resources` | Rebuild map grids, regional maps, special maps, and container components. |
| `early_runtime_data`, `late_runtime_residual`, `runtime_support_data`, `executable_gap_sources` | Rebuild or classify runtime-support and executable-gap inputs. |
| `resource_01c`, `resource_3ce`, `resource_5`, `resource_byte_canvases`, `resource_d1_d3` | Rebuild resource-specific formats whose contracts are not safely generic. |
| `simple_resources`, `skip_sprite_archive`, `static_sprite_series`, `title_resources` | Rebuild simple plans, sprite families, skipped archives, and title resources. |

## metrics

| Command | Purpose |
|---|---|
| `audit_residuals` | Account for every remaining executable byte and its current ownership class. |
| `compare_roms` | Compare approved local ROM editions without publishing byte diffs. |
| `coverage_map` | Check or regenerate the dashboard, exhaustive core-target index, `TARGETS.md`, SVGs, and README versions. |
| `dashboard_server` | Serve one live worktree dashboard on localhost and an optional LAN address. |
| `full_c_history` | Write the first-parent Exact-C ledger; `--all` exports an independently measured reachable-DAG audit. |
| `full_c_progress` | Calculate, check, or write the audited Exact-C report and denominator. |

`coverage_map --write`, `full_c_history --write`, and the `full_c_progress`
write modes change tracked files. Run their check/read modes first when merely
inspecting status.

## overlay

| Command | Purpose |
|---|---|
| `overlay_disasm`, `overlay_show`, `overlay_entry` | Inspect decoded owner bytes, entries, calls, and linked layout. |
| `exact_reading_list`, `overlay_twins` | Find exact relatives and reusable owner families. |
| `overlay_candidate_rank`, `overlay_call_order_check` | Rank candidate residuals and compare resolved call order, including human-readable aliases, suffixed ABI spellings, and site-relative overlay relocation symbols. |
| `overlay_gaps`, `overlay_unindexed`, `overlay_inventory` | Find unowned, unindexed, or newly discoverable executable spans; `overlay_inventory` produces the diagnostic inventory file used by semantic compilation and overlay adoption. |
| `overlay_mode_cohort` | Test a routed compiler-mode hypothesis across overlay owners. |
| `overlay_certify` | Report owner-certification findings; `--check` turns findings into a failing gate. |
| `overlay_showcase` | Render a representative exact overlay for regression and demonstration. |
| `overlay_driver` | Execute the low-level overlay reconstruction driver. |
| `overlay_adopt` | Preflight and install an exact code-overlay owner; the inventory file must exist, but an explicit `--span` need not be listed; mutation requires explicit apply intent. |

`overlay_inventory` has a material cold scan but a fast content-addressed reuse.
Run the root `make inventory` target when `out/decomp/overlays.json` is absent or
needs refresh. Do not delete its cache just to make a routine run look fresh;
invalidate it only when an identity input is missing or wrong. The inventory is
discovery input, not the overlay adoption safety proof: boundary, alias, and
whole-overlay byte comparisons remain the acceptance checks.

## search

| Command | Purpose |
|---|---|
| `decomp_constraints` | Derive bounded structural constraints for a candidate. |
| `shape_sweep` | Search deterministic, behavior-preserving source forms with the compiler fixed. |
| `search_compiler_modes` | Search approved compiler modes through the candidate-compiler interface. |
| `alchemy_permuter` | Run a larger bounded source permutation job with linked-byte scoring. |

The permuter has a 100,000-iteration hard ceiling, dependency-aware reordering,
and cleanup limited to repository `out/` or OS-temporary paths. A winning source
still requires human review and exact-path adoption.

## semantic

| Command | Purpose |
|---|---|
| `semantic_queue` | Rank current semantic candidates while ignoring resolved blockers. |
| `semantic_owner_scope` | Verify that a flat semantic source covers its complete audited owner. |
| `semantic_superseded` | Find semantic sources made obsolete by exact adoption. |

Semantic tools operate on readable ownership, not byte equality. Passing them
does not turn a source into an exact claim.

## Internal binaries

These binaries were exercised during the 2026-08-09 audit and remain outside
the dispatcher deliberately. Their classification is enforced in
`tools/dispatch`; adding another unregistered binary fails architecture.

| Class | Binaries | Why non-public |
|---|---|---|
| compiler/RTL diagnostics | `candidate-explain`, `candidate-show`, `rtl-align`, `rtl-insn`, `rtl-schedule`, `rtl-sexpr`, `thumb-disasm` | Read compiler dumps or expose narrow implementation details after a public compiler command has isolated a theory. |
| build internals | `kind2-resources`, `late-runtime-data`, `overlay-published`, `alchemy-zlib` | Subprocess or format protocols owned by a public build command. |
| repository diagnostics | `alchemy-lints`, `jobs`, `lang-ban`, `parity_dump` | Gate internals, worker plumbing, language policy, or a migration-parity auxiliary. |
| self-test/support | `dispatch`, `alchemy-selftest`, `cache-entry`, `decomp-targets`, `self-test` | Hosts or fixtures used to enforce the public layer rather than separate workflows. |
| benches | `compiler-corpus-regression-bench`, `integrate-matches-bench` | Measured development auxiliaries, never repository operations. |

## Cost and cache expectations

Most focused reports complete in well under a second once release binaries and
content caches exist. Whole-overlay discovery, full routed corpora, asset scans,
and authoritative verification necessarily touch much larger evidence sets.
Use a focused command while iterating, retain valid ignored caches, and run the
complete `make verify` once before committing. A cache is valid only when its
key covers every source, compiler, route, linker/scorer, and tool input that can
change the result.
