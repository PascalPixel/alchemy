# alchemy-permuter

`alchemy-permuter` is Alchemy's native, bounded C permutation search. It keeps
the useful local workflow of
[decomp-permuter](https://github.com/simonlindholm/decomp-permuter) while
sharing Alchemy's compiler routing, overlay linker, and exact-byte authority.
The implementation was written natively against decomp-permuter commit
`2795247304ec4798459b9bc865314e64e5182bf9`; no Python or third-party source is
vendored.

Two backends use the same permutation engine:

- A C path inside Alchemy compiles with that eventual path's routed compiler
  family and flags, links against its real core or overlay address, and scores
  the produced bytes directly.
- A legacy directory containing `base.c`, `target.o`, `compile.sh`, and
  `settings.toml` invokes the supplied compiler and scores normalized objdump
  instructions. This preserves portable ARM32, MIPS, and PowerPC use rather
  than baking Golden Sun paths into the engine.

The manual language supports `PERM_GENERAL`, `PERM_INT`, `PERM_LINESWAP`,
`PERM_LINESWAP_TEXT`, `PERM_ONCE`, `PERM_VAR`, `PERM_IGNORE`, `PERM_PRETEND`,
`PERM_RANDOMIZE`, and `PERM_FORCE_SAMELINE`, including nesting and protected
randomization regions. The 34 established randomization pass families retain
their names and compiler-profile weights; a legacy `[weight_overrides]` table
is honored. Run `--help=randomization-passes` to list them.

Searches are deterministic for a seed, deduplicate generated sources, compile
with bounded parallelism, stop on exact output by default, and write candidates
plus `report.json` under an ignored, dedicated run directory. The default path
includes a full input identity and seed, so same-named files do not collide.
`--output DIR` must be a new child directory under the repository `out/` tree
or the operating system's temporary directory; with multiple inputs it is a
parent and each input receives its own identity-named child. An existing run
directory is accepted only with `--resume` and its marker must match the input,
compiler, and seed. An active claim prevents concurrent runs; a stale claim is
reported for manual inspection rather than guessed away.

Each run records the exact output files it owns. On Unix, the run is opened and
claimed through a directory descriptor; owned writes and cleanup use no-follow,
descriptor-relative operations, so a rename or symlink swap at the visible path
cannot redirect them. Resume cleanup removes only recorded candidate/report
files, never every `candidate-*.c` in a shared directory, and preserves
unrecognized files rather than taking ownership of them.

The v3 journal binds every row to the complete input/compiler/seed run identity,
candidate source fingerprint, and measurement with SHA-256. A changed or
unauthenticated row is rejected; an authenticated exact row is still recompiled
before it can terminate a resumed search. Generated mutation streams are
bounded while each pass is consumed, and source/plan limits are checked before
workers start. Candidate/report and journal budgets are conservatively
preflighted before backend compilation, with runtime checks retained as a final
I/O guard. Generated sources are capped at 8 MiB each, the retained plan at 128
MiB, and journal/results at 32 MiB. Every local run also has a hard ceiling of
100,000 iterations.

Launch the permuter only behind the last-mile gate in
[`../../CONTRIBUTING.md`](../../CONTRIBUTING.md): reference size reached, the
residual localized and reviewed against the matching-readiness checklist in
[`../../semantic/README.md`](../../semantic/README.md), a dominant
register-noise score, and a written hypothesis with a stated budget. It is a
residual search, not a substitute for recovering boundaries, types, aliases,
calls, or side effects; a large diffuse score means the input source is not
ready. Size the round to the source before launching: iterations times source
bytes times three must fit the 128 MiB plan budget, so a 60 kB source affords
roughly 700 iterations per round and a larger request errors before starting.
Tie-accepting walks measurably outperform strict better-only climbs. Two
stalled rounds with no structural insight in the winners end the axis, and a
null run is cost evidence, not exhaustion proof.

Automatic statement reordering must preserve C dependencies: write/read,
read/write, write/write, and ordering across calls whose effects are not proved
independent. A candidate that compiles or scores better is still rejected if it
reads a value before initialization or changes reviewed behavior. Inspect every
winning diff before adoption and reproduce an exact result through the owner's
real routed build.

The local tool does not implement the permuter@home network protocol. Remote
execution brings a separate authenticated service, container, and untrusted
code-execution boundary; adding that transport should be an explicit project
rather than an implicit side effect of the local search port. The compiler and
scorer interfaces are independent of the worker executor so such a transport
does not require another permutation-engine rewrite.

Typical use:

```text
cargo run --release --manifest-path tools/alchemy-permuter/Cargo.toml -- semantic/resource_373_c_02005b48.c --jobs 8
cargo run --release --manifest-path tools/alchemy-permuter/Cargo.toml -- path/to/legacy-directory --jobs 8 --resume
```
