# Contributing to Alchemy

Alchemy is an unofficial clean-room reconstruction of the Game Boy Advance
releases of _Golden Sun_ and _Golden Sun: The Lost Age_. The Japanese releases
are the source editions; localized releases are measured deltas. English GS1 is
currently the only complete byte-identical build.

The goal is readable, ordinary C that reproduces the shipped bytes. A plausible
draft, a close compiler match, or a useful name is progress, but it is not DONE.
Only byte-exact C and assembly proved permanent count.

## Rules

### Work cleanly

Use only the reference ROMs held locally, independently reconstructed material
in this repository, decoded data, relocations, verified calls, and public
documentation about C, ARM, GCC, and file formats. Compiler source is evidence
about compiler behaviour only.

Do not inspect or copy leaked source, another Golden Sun decompilation, or its
symbols, pseudocode, types, comments, or target-specific conclusions. Candidate
names and neighboring drafts are leads, not evidence.

Never commit or transmit ROMs, ROM fragments, extracted binaries, objects,
ELFs, compiler output, or patches containing reference bytes. Generated work
belongs under ignored `out/` or `scratch/`.

### Keep exactness literal

Adopt an owner only when its linked `differing_halfwords` is zero and the
production build uses the source for its complete audited extent. Similarity,
matching size, and behavioral plausibility do not count.

Production C must compile through the recorded GCC 2.96 route. Do not use inline
assembly, fixed-register variables, empty barriers, copied instructions, forced
scheduling, or compiler patches to disguise an incorrect source model.

Permanent assembly needs evidence that the approved compiler cannot emit the
required instruction shape, or that the bytes are genuinely structural
assembly such as a veneer, alignment, or runtime primitive. Record that reason
in the appropriate main-image or overlay classification.

Never fill an unfinished build from the reference ROM.

## Setup

Install Rust, Ninja, and `arm-none-eabi-binutils`. Put approved ROMs in ignored
`roms/` and the approved compiler bundle in `alchemy-gcc/dist/`.

Configure the repository hooks once, then build the contributor host:

```sh
git config core.hooksPath .hooks
cargo build --offline --release --manifest-path tools/compiler/Cargo.toml
```

Worktrees may symlink `roms/` and `alchemy-gcc/dist/` from the main checkout.
Do not run submodule commands from a worktree.

## Owners and names

`games/<game>/source-paths.json` is the one authored project-wide register
connecting an address-qualified owner to its semantic name and production
source path. Translation-unit manifests may describe compile-local structure,
but they derive owner names from this register. Do not create another address
map in a header, tool, dashboard, or document. Derived coverage, family, twin,
correspondence, and build reports are not name authorities and should never be
edited by hand.

Production paths describe the evidenced job of the code:

```text
battle/effects/random_particle/start_emitter.c
psynergy_menu/draw_list_page.c
party/apply_state_preset.c
```

Do not add address filenames, ordinal placeholders such as
`runtime_owner_119.c`, game codenames, or resource-number folders. Keep the
address in the register and ABI alias; keep overlay membership in the owner key.
If the role is not known, use the narrowest honest classification instead of a
confident guess.

The neutral naming voice is a Japanese commercial C codebase from 2000–2001:

- Use a stable project label such as `Subsystem_VerbObject` for a proved owner
  role; it is a navigation label, not recovered original spelling.
- Prefer short, conventional local names such as `pos`, `cnt`, `tbl`, `buf`,
  and `work` when their meanings are evidenced.
- Use `unk`, `field_<offset>`, or a raw offset when they are not. Reserve `pad`
  for bytes proved to be padding.
- Avoid fan-lore names, modern abstractions such as `EffectManager`, fake
  Japanese, and comments written as if they came from the lost source.
- Keep compiler-shape comments short and beside the constrained expression.
- `Func_<address>` is the ABI compatibility alias, not a second semantic name.

Promote declarations into a header only for a real shared interface, table
family, or stable ABI layout. Owner-specific compiler shaping stays with its
owner.

## Recover and adopt

1. Confirm a complete function owner. Main owners begin in retained assembly;
   overlay owners use their audited overlay extent. Metrics account for bytes
   but do not prove function boundaries.
2. Recover the algorithm, control flow, types, access widths, constants,
   lifetimes, and call relationships in ordinary C89. Do not transcribe assembly
   instruction by instruction.
3. Compile and inspect the linked result:

   ```sh
   out/cargo-target/release/compiler candidate-show \
     games/gs1/recon/en/main/<address>.c --align --first
   ```

4. For a declared shared translation unit, compile it once and score every
   member from the same object:

   ```sh
   out/cargo-target/release/compiler candidate-show \
     --unit <translation-unit-id> --first
   ```

5. Repair structural mismatches before experimenting with declarations,
   temporary lifetimes, expression order, or loop spelling. Search the C; never
   force the allocator or scheduler. The `--allocator-order` decoder names
   both layers: register-role repairs when the instruction streams align, and
   reachability-filtered branch evidence — guard counts, loop shapes,
   mirrored or inverted guards — when they do not. Strong structural findings
   take precedence over an allocator proposal, and `compiler permute`
   searches only the catalogued repairs the decoder names.
6. Register the semantic destination and adopt only at zero linked differences.
   The `integrate` command is for standalone owners. Shared-unit owners are
   installed and retire their assembly together only after every affected owner
   is exact from the shared object.

JA correspondence must be established before EN work is treated as shared game
source. A normalized matching core does not by itself prove identical constants,
bindings, boundaries, or ownership in another edition.

Overlay identity is resource-qualified because load addresses repeat. Exact
main-image C does not prove an overlay. Overlay scoring and audit are available;
do not retire overlay assembly through an adoption route whose decoded inventory
cannot be reproduced from a clean checkout.

## Route residual work

The triage router runs as part of `compiler candidate-show` and assigns each
scored candidate a mechanical residual class and named playbook. Route an owner
only from that result: allocation, scheduling, structure, type/width, extent,
layout, and compiler-classification work stay distinct, while `unclassified`
and uncovered allocation results go to the smart queue rather than a raw-diff
guess.

The executable repair catalog is
`games/gs1/recon/compiler-repair-patterns.json`. Add a repair only with a named
decoder signal, guarded finite operation, recorded verdict, and regression
fixture; never encode fixed registers or instruction scheduling. Run
`compiler permute --acceptance-test` (included by `make test`) to prove the
catalog still names and reverses the controlled perturbations before using a
catalog operation in a wave.

The residual dispatcher is `compiler waves`. Use `inventory` and `bucket` to
classify the existing candidate corpus. For no-candidate drafting, run `draft
prepare` once, run one or more read-only `draft score --shard I/N` workers, then
run `draft collect`. Preparation binds the evidence-mined aggregate context and
ranked exact-family templates into immutable packs; scoring tries the bound m2c
seed followed by those prepared templates in recorded order. Complete-owner,
continuation, and split-region records are parked under their audited route
rather than misreported as standalone functions. The collector rejects stale,
duplicate, incomplete, or unclassified results and publishes the prediction
comparison and class scoreboard. All receipts, packs, shard verdicts, and reports
belong beneath ignored `out/gs1-en/waves/`, never `/tmp`. Cheap agents execute
only prepared shards and named playbooks; exact results still pass through the
explicit integrator before repository source changes.

`make coverage` also refreshes the main-ROM residual class scoreboard. It
re-triages ordinary tracked candidate source with the current router, keeps
absent candidates explicit, and records deterministic source, reference, and
scoring-environment receipts. Aggregate-aware generated drafts remain wave
output and do not affect this tracked-source scoreboard.

## Validate and commit

Portable tooling is capped at 40,000 Rust, TypeScript, JavaScript, and CSS
source lines.
This is a maintainer-owned ceiling: consolidate or remove machinery when the
gate fails; do not raise the limit to accommodate a change.

Use narrow commands while iterating:

```sh
make build-claimed       # adopted main-image C
make build-asm           # retained main-image assembly
make build-assets        # source assets
make overlay-check       # exact overlay owners
make check-owners        # owner paths, names, and aliases
```

Before a normal commit, stage the intended files and run the fast production
gate:

```sh
git diff --cached --check
make verify
make progress-subject
```

`make verify` rebuilds the current production target, proves the complete ROM,
overlay, translation-unit, owner-register, source-tracking, compiler-route, and
ordinary-C contracts. It deliberately does not rescore every draft, rebuild all
twelve editions, regenerate reports, or run every tool test. The commit hook
also rejects a staged tree that differs from the one most recently verified.

Run the extra gate only when the change owns it:

| Change | Additional gate |
| --- | --- |
| Rust tooling | `make test` |
| Shared edition/preprocessor logic | `make targets` |
| Retained-assembly classification | `make classification-check` |
| Candidate corpus policy | `make candidate-corpus-check` |
| Exact ownership, owner labels, or README/coverage figures | `make coverage` |

`make audit` is the explicit exhaustive audit for a release, merge, or large
compiler/ownership batch. It covers every edition, retained candidate,
cross-edition correspondence, family, and progress contract. Use
`make reports` when the ignored analysis reports and coverage figures need to
be refreshed. Neither belongs in the ordinary commit loop.

The tooling ceiling (`TOOLING_LINE_LIMIT`) is Pascal's number, and raising it
is never part of a task. New machinery fits by paring old machinery; a diff
that touches the ceiling without his recorded decision is invalid regardless
of how good the new tools are. Headroom is granted after a tool proves
conversion, not before.

Commit subjects begin with the value printed by `make progress-subject`, for
example `☀️ 52% – `. Attribute work to its actual author.

## Find work

Run `make dashboard` for the live coverage view and unresolved owners. The
dashboard derives its view from the current tree; Alchemy does not maintain a
second generated target list in this guide.
