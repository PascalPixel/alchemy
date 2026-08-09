# Contributing to Alchemy

Alchemy is a clean-room reconstruction of *Golden Sun* for GBA. A change is
complete only when the rebuilt bytes match the released ROM. Read
[`PROVENANCE.md`](PROVENANCE.md) before contributing: tooling and methods may
be shared, but game-specific facts must be reconstructed from the approved ROM set
and local project evidence. External projects are not source, naming, or
game-knowledge evidence.

## The working loop

Work on one owner at a time.

1. Pick an owner with the dispatcher commands `semantic_queue` or
   `overlay_twins --leads`.
2. Reconstruct credible C in `semantic/<owner>.c`. Establish the boundaries,
   entry points, calls, arguments, types, signedness, aliasing, control flow,
   and side effects from local evidence.
3. Check semantic readiness. The source should have the right size class and a
   residual that can be explained in small clusters. A large, diffuse
   residual usually means the source model is wrong; return to reconstruction.
4. Close a small residual with the escalation below.
5. Verify using the owner's real routed path, then use its adoption gate:
   `integrate_matches` for a main-image draft or `overlay_adopt` for a code
   overlay. Adoption, not a good score, moves the exact-byte counter.
6. Record reusable evidence in `LAWS.md`, and record an exhausted deterministic
   axis in `SANCTUM.md` only when it is genuinely exhausted.
7. Run `make verify` once after the work is complete and before committing.

During iteration, invoke a public command through a `dispatch-*` Makefile
wrapper or use a focused native binary. The root Makefile is the workflow
facade; the dispatcher registry supplies the group and command names:

```sh
make dispatch-semantic ARGS='semantic_queue --help'
make dispatch-decomp ARGS='decomp_diagnose semantic/resource_373_c_0200034c.c'
make dispatch-overlay ARGS='overlay_adopt resource_373:034c --source semantic/resource_373_c_0200034c.c --span BYTES --where'
```

Replace the uppercase placeholders with the owner and span under review.
Direct invocation is useful while developing a command:

```sh
cargo run --release --manifest-path tools/semantic-queue/Cargo.toml -- --help
cargo run --release --manifest-path tools/decomp-diagnose/Cargo.toml -- semantic/resource_373_c_0200034c.c
cargo run --release --manifest-path tools/overlay-adopt/Cargo.toml -- \
  resource_373:034c --source semantic/resource_373_c_0200034c.c --span BYTES --where
```

For a main-image exact candidate, `integrate_matches` accepts a directory
containing a `src_<main-address>.c` draft and, with `--apply`, installs
`exact/<main-address>.c`. For a code-overlay candidate, `overlay_adopt` installs
`exact/resource_<overlay>_c_<overlay-address>.c` after its rehearsal passes.

Use the repository's `Makefile` for shared workflows:

```sh
make test          # policy gates and native self-tests
make lint          # architecture and publication/provenance gates
make progress      # current byte-exact counts
make coverage      # refresh dashboard maps when metrics changed
make verify        # complete pre-commit authority
```

## Escalation order

Use the smallest source change that explains the residual, and keep the
compiler route fixed while changing C.

1. Diagnose the candidate with `decomp-diagnose` and inspect local disassembly,
   call sites, and residual clusters.
2. Compare a known exact sibling with `overlay-twins`; transpose a proven
   machine-producing shape before inventing a new one.
3. Try small, deterministic changes to types, expressions, lifetimes, and
   statement shape with `shape-sweep`.
4. Inspect compiler output and use `mode-sweep` or `mode-cohort` only when the
   source evidence points to a compiler choice.
5. Use `alchemy-permuter` only for a semantically ready candidate with a small,
   localized residual. Keep the search bounded and inspect every winning diff.
6. Change the compiler only with narrow positive and negative regression tests
   and reproducible evidence across the affected owners.

Permutation is a search aid, not a reconstruction engine. It must preserve
write/read, read/write, write/write, initialization, and call-order
dependencies. A candidate that compiles or scores better is still invalid if
it changes reviewed behavior. Never promote a near-match.

## Caching and proof

Native tools cache expensive derived work under ignored `out/` paths. A cache
hit is an optimization, not evidence: cache identities include the relevant
source, compiler, route, and input data, and a changed input must produce a new
identity. If results seem stale, use the tool's report/check mode or remove
only the affected ignored cache after confirming its path; do not treat an old
score as a new build.

Matching under `semantic/` is not final proof. Compiler flags are routed by
repository-relative source path. Main-image drafts are checked by
`integrate_matches`, which routes a `src_<main-address>.c` candidate as its
eventual `exact/<main-address>.c` path. Overlay drafts are rehearsed by
`overlay_adopt ... --where`, which uses the eventual
`exact/resource_<overlay>_c_<overlay-address>.c` path. In both cases the
adoption pre-flight must assemble the owner and compare its bytes; only then
should the source move into `exact/`.

The diagnostic overlay inventory is a required input file for `overlay_adopt`
and `make build-semantic`. When `out/decomp/overlays.json` is absent or needs
refreshing, run `make inventory` first. It supplies discovery rows, not
byte-exact acceptance proof: `overlay_adopt --span` may select a span that the
inventory does not list, while its assembly boundary and whole-overlay
comparison remain the safety checks.

The `resource_373` work made this concrete: a readable rewrite of a proven
sibling regressed badly because it changed alias information, while a literal
address-adjusted transcription of the proven C shape matched. Preserve the
machine-producing structure first; humanize it only after each change remains
exact.

## Safety and publication

- Never use `asm(...)`, fixed-register bindings, empty assembly barriers, or
  another assembly escape hatch in C or headers.
- Do not copy game-specific code from another decompilation. All game-specific
  facts must come from the approved ROM set and local reconstruction evidence;
  record tool provenance where it matters, and keep retirements recoverable.
- Keep ROM images, executable dumps, build outputs, and caches out of the
  publishable repository. Publish source-format assets only when their
  provenance and reproduction role are documented.
- Do not resolve compiler routing tables or
  [`alchemy-gcc/gcc-2.96/gcc/config/arm/arm.c`](alchemy-gcc/gcc-2.96/gcc/config/arm/arm.c)
  by taking one side of a merge. Use the merge base and preserve the intent of
  each entry.
- Preserve existing user work in a dirty checkout. Inspect the working tree
  before editing and keep unrelated changes out of the commit.

## Committing

Regenerate metrics after executable-source changes. The commit subject must
end with the generated progress form:

```text
[ ☀️ exact / total ]
```

Use `make verify` before committing; a green focused test is not a substitute
for the complete gate. A deliberate regression or denominator correction must
say so in the subject as required by the repository checks.
