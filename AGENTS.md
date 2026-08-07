# Working on Alchemy

Alchemy reconstructs *Golden Sun* (GBA) as byte-exact C. A change counts only
when the rebuilt bytes are identical to the released ROM.

There are twelve tools. Each is a folder under `tools/` with an `index.ts` that
takes a subcommand, plus the smaller modules it owns. Shared code lives in
`tools/lib/` and belongs to no tool.

**Every tool must have a section on this page, or it must not exist.**
`tools/check/architecture.ts` enforces that, and `bun run lint` runs it. There is
no second list of tools anywhere: adding one without documenting it fails the
build, which is how the count stays honest. This repository reached 184 tools
because adding one cost nothing and nobody could see the total.

Run any tool with no arguments to list its subcommands.

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

The gate. Rebuilds the ROM, every overlay, and the asset tree from tracked
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
This is the pipeline `verify` drives, exposed separately so you can rebuild one
stage while iterating instead of paying for the full gate each time. Named
`make` because `build`, `rom` and `dist` are all publication-blocked directory
names.

## overlay

Everything about the 96 code overlays, which hold roughly 60% of the executable.
`overlay_disasm` and `overlay_show` read one owner, `overlay_inventory` builds
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
and bulk dumps — see PROVENANCE.md and `check_publication`.

## compiler

The `alchemy-gcc` fork and its routing. `alchemy_gcc` owns the flag tables, the
per-source routing sets, and the approved cc1 digests; `mode_sweep` searches
compiler configurations for one candidate; `mode_cohort` runs a hypothesis
across many owners at once. The routing sets are append-only registries of
independent discoveries — never resolve a merge in them by taking one side.

## search

Finds a source form that compiles byte-exact, holding the compiler fixed.
`shape_sweep` applies transforms seeded from LAWS.md and is bounded and
deterministic, so "it found nothing" is a real result; the annealers
(`permute_v1`, `permute_overlay`, `alchemist`) search far wider but
stochastically, and their measured yield is low, so they are a rescue tool under
the bounded-probe rule in HANDOVER.md. Never promote a near-match.

## rtl

Sees inside the compiler when a residual survives both search axes. A 2-5
halfword difference at the exact size is usually a scheduler tie, which
`mode_sweep` flags as `escalation: compiler-rtl-scheduler-trace`; this tool
parses the fork's own RTL dumps and reproduces its real scheduler tier order
rather than modelling it. Reach for it when you need the reason instead of
another guess.

## decomp

Picks and diagnoses owners. `decomp_queue` ranks candidates by expected value
and is resumable, `decomp_diagnose` explains why a specific candidate misses,
and `match_m2c` and `integrate_matches` handle drafting and installation. Start
here when you do not already know which owner you are working on.

## semantic

Owns the semantic-C tree — readable reconstructions that are not yet byte-exact.
`semantic_queue` lists what is ready to attempt, `semantic_owner_scope` checks an
owner covers exactly its region, and `semantic_superseded` retires sources that
adoption has replaced. Semantic C is the staging ground: it is how an owner gets
understood before it gets matched.

## metrics

Measures and draws progress. `full_c_progress` produces the byte counts every
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
ledger, and `architecture` enforces the shape of `tools/` itself: every tool
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
