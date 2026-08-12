# Working on Alchemy

Alchemy reconstructs *Golden Sun* (GBA) as byte-exact C and independently
described assets. A code change counts as Exact C only when the rebuilt bytes
equal the released ROM. `DONE` in the public charts is Exact C plus the small,
audited permanent-assembly category; semantic C is useful work but is not done.

Read [CONTRIBUTING.md](CONTRIBUTING.md) for the human workflow,
[docs/TOOLS.md](docs/TOOLS.md) for commands, [STATUS.md](STATUS.md) for the live
frontier, and [PROVENANCE.md](PROVENANCE.md) before handling evidence or build
artifacts.

## Repository shape

The native Rust crates under `tools/` are one operational tooling layer, not a
hundred independent products. Public commands live in the dispatcher registry;
the root Makefile is the workflow facade and its `dispatch-*` targets invoke
that registry. Support libraries, internal diagnostics, self-test helpers, and
benches have an explicit non-public classification. `architecture` rejects
unregistered, unclassified, unreachable, or stale targets, and `documented`
keeps the public catalog synchronized.

Shared behavior belongs in the owning crate's library module or a consumed
support crate. A throwaway probe belongs in ignored `tools/scratch/`; graduate
it after a second real use or delete it. Never create a parallel helper merely
to avoid understanding the native path.

Preserve unrelated working-tree changes. The compiler fork may also be dirty
with active experiments. Do not run `git submodule` commands inside a worktree:
they can rewrite the shared `core.worktree` configuration and break the main
checkout.

## The contributor loop

An open-ended request such as "continue decompilation" is coordinator work,
not permission for a worker to roam. A worker must have one owner or proved
family, one disjoint write set, one authoritative scoring path, a 30-minute
default deadline, and a terminal report. If any field is absent, stop after
discovery and ask the coordinator for a generated brief; do not repeatedly
inspect reports, invent ad-hoc probes, switch owners, or choose a compiler
theory. For a main-image source, generate the canonical brief with
`decomp_diagnose --agent-brief`. An overlay coordinator must provide the same
fields using the canonical overlay scorer. Production-path scores outrank
internal dump diagnostics.

1. Pick a small owner with `semantic semantic_queue`, or an overlay family with
   `overlay overlay_twins --leads`.
2. Recover the real boundary, entries, types, signedness, aliases, calls, stack
   arguments, control flow, and side effects in `semantic/`.
3. Compile and diagnose functions or residual clusters independently. A wrong
   size or large diffuse residual means the source model is not ready.
4. Try an exact sibling's witnessed shape, then bounded deterministic source
   forms, then compiler modes. Use stochastic permutation only for a localized,
   semantically reviewed residual.
5. Review every automatic candidate for C dependency and behavior preservation.
   A lower score is not proof of valid source.
6. Prove the candidate at its eventual path and use the owner-specific adoption
   gate: `integrate_matches` for main-image drafts and `overlay_adopt` for code
   overlays. Adopt from outside `exact/`; copying a file is not proof.
7. Record reusable evidence, run the focused checks, then run `make verify` once
   before committing with regenerated metrics.

## Productive rounds

The default multi-agent unit is the bounded round defined in
[CONTRIBUTING.md](CONTRIBUTING.md). Assume lanes share the checkout and generated
outputs. Every lane needs an assigned owner or family, a disjoint write set,
explicit search and time limits, and a terminal report. Source lanes may not
edit routing, ledgers, compiler state, adoption outputs, metrics, or git.

The coordinator owns the baseline, assignments, shared outputs, root routing,
adoption, metrics, verification, and commits. At most one designated compiler
lane may have exclusive access to `alchemy-gcc/` and its staged runtime; all
other lanes hold compiler and route fixed. Commands that mutate `out/`,
manifests, inventory, generated assembly, or staged compiler outputs must use a
supported isolated work path or be serialized.

Do not integrate or start another round until every lane and descendant process
is stopped, every terminal report is collected, and the shared diff is audited.
Apply adoptions serially and run one final `make verify`. If no separate
coordinator exists, the sole agent assumes that role and performs the sequence
serially.

## Common entry points

Use the root Makefile for normal work:

```sh
make build-semantic  # compile semantic C while reconstructing
make build-claimed   # link current exact owners
make inventory       # provide the diagnostic overlay inventory when needed
make progress        # calculate live Exact C
make coverage-check  # report stale dashboard artifacts
make test            # policy gates and native self-tests
make lint            # architecture and publication policy
make verify          # authoritative pre-commit gate
```

Run one public command through the dispatcher:

```sh
cargo run --release --manifest-path tools/dispatch/Cargo.toml -- \
  semantic semantic_queue --help
```

Focused commands and their caches are the iteration loop. `make verify` is the
final proof, not a five-minute tax after every edit. Cache identity must include
the source, route, compiler bundle, linker/scorer inputs, and relevant tool
version; never reuse a result whose evidence changed.

## verify

`make verify` rebuilds tracked ROM stages and asset outputs, runs policy checks
and native self-tests, verifies semantic and exact ownership, and checks the
regenerated metric. A failure is evidence to fix or explain, not a green result
with a footnote. Run focused tests first and the complete gate before each
commit that changes deliverable behavior or progress.

## self-test

Each public binary owns a meaningful `--self-test` or an explicit policy saying
why it is not independently exercised. The native runner derives targets from
the canonical binary classification rather than scanning source text. A new
binary must be classified, reachable, documented when public, and covered by
tests before architecture accepts it.

## make

The Makefile is the stable façade for builds, assets, metrics, lint, and
verification. Build stages are `build-claimed`, `build-asm`, `build-assets`,
`build-semantic`, `build-full`, and `build-rom`. Use the smallest stage that
answers the current question; avoid rebuilding the whole repository merely to
score one source candidate.

## overlay

The overlay commands inspect, rank, compare, certify, and adopt the 96 loaded
code modules. Use `overlay_disasm` and `overlay_show` for evidence,
`exact_reading_list` and `overlay_twins` for witnessed relatives, and
`overlay_adopt` only after exact-path preflight. Overlay commands and
`build-semantic` require the diagnostic inventory file
`out/decomp/overlays.json`; create it with `make inventory` when needed. The
inventory is discovery input, not the byte-exact acceptance gate: an explicit
`--span` may select an entry absent from its rows. Invalid owner IDs must fail
clearly; advisory reports expose a separate `--check` mode when findings should
gate automation.

## assets

Asset commands extract, import, export, rebuild, and verify graphics, tilemaps,
maps, text, audio, and Camelot containers. Source descriptions needed for a
reproducible build may be tracked. ROMs, raw extraction dumps, built images,
object files, and caches may not. A scanner must report failed or unclassified
owners rather than silently dropping them.

## compiler

Compiler commands search fixed flag sets, compare cohorts, and verify routed
owners. Routing tables and approved digest lists are append-only evidence:
resolve merges per entry, never by taking one side or deleting an inconvenient
route. Internal RTL readers diagnose the fork's real dumps; they do not justify
a compiler change without narrow positive and negative regressions plus a
zero-regression exact corpus.

## search

Search holds one axis fixed. `shape_sweep` applies bounded deterministic source
transforms; compiler search holds source fixed; `alchemy_permuter` is a bounded
rescue search with dependency-safe reordering, a hard iteration ceiling, and
protected output cleanup. Never promote a near-match or use search to conceal
wrong boundaries, missing side effects, guessed types, or diffuse residuals.

## decomp

Decomp commands discover owners, survey remaining executable regions, diagnose
candidates, and integrate reviewed matches. `integrate_matches` is the
main-image draft gate; `overlay_adopt` owns code-overlay adoption. Surveys must
resolve paths from the repository root rather than the caller's current
directory, and resolved blocker records must not remain in the active queue.
Diagnose one independently provable function or cluster at a time even when
adoption is owner-atomic.

## semantic

`semantic/` contains readable, behaviorally reviewed C that is not yet
byte-exact. Its flat filename conventions cover both main-image and overlay
owners. Scope checks must account for complete noncontiguous ranges and shared
entries. Semantic admission is not matching readiness, and matching readiness
is not exact ownership; keep all three boundaries explicit.

## metrics

Metrics measure executable bytes, derive history, and render coverage. Exact C
and DONE have distinct numerators documented in
[docs/FULL-C-BYTE-SHARE.md](docs/FULL-C-BYTE-SHARE.md). The dashboard watches
the worktree and serves localhost by default; an explicit LAN bind must retain
localhost access and share one state/watcher rather than duplicating work.

## check

Check commands enforce architecture, publication boundaries, source citations,
no-inline-assembly policy, exact progress, retained assembly, documentation,
and the exhausted-search ledger. A check that scans nothing must fail. A check
worth running twice belongs in these gates with a self-test, not in a private
script or a warning remembered by one contributor.

## Evidence ledgers

| File | Purpose |
|---|---|
| [LAWS.md](LAWS.md) | reproduced compiler/source behavior and negative stop rules |
| [SANCTUM.md](SANCTUM.md) | owners with both bounded search axes exhausted, plus the live one-axis queue |
| [PROVENANCE.md](PROVENANCE.md) | clean-room evidence, publication boundary, and retired-tool recovery |
| [STATUS.md](STATUS.md) | current measured frontier and next decisions |

Keep dated experiments in the history archive rather than in current guidance.
Do not erase negative evidence simply to make documentation shorter.

## Hard rules

- Never use `asm(...)`, `__asm(...)`, fixed-register bindings, empty assembly
  barriers, copied ROM bytes, or equivalent escape hatches in C or headers.
- Never import game-specific source, names, types, comments, pseudocode, or
  generated implementation from another decompilation. Game-specific facts
  come from the approved ROM set and local reconstruction evidence; generic
  methodology is reusable, but game knowledge is not.
- Adopt from outside `exact/` through the owner-specific gate, ensure the
  diagnostic inventory file exists for overlay tools, and regenerate progress.
- Commit subjects end with `[ ☀️ exact / total ]` matching staged metrics. A
  proved denominator correction and a deliberate regression use their explicit
  repository subject forms.
- Never resolve compiler routing tables or the fork's GS1 source file
  [`alchemy-gcc/gcc-2.96/gcc/config/arm/arm.c`](alchemy-gcc/gcc-2.96/gcc/config/arm/arm.c)
  by taking one side wholesale. Three-way merge and review every independent
  entry.

## The compiler fork

`alchemy-gcc/` is an editable compiler repository with its own source,
licensing, provenance, and build instructions. The main repository pins and
verifies approved compiler bundles; it does not absorb distributed compiler
binaries or licences. Worktrees use the canonical staged compiler so an exact
witness is not ambiguous. Rebuild and append an approved digest only after the
focused regressions, full routed corpus, `build-claimed`, and authoritative
verification all pass.
