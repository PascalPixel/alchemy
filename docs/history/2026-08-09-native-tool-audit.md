# Native tool audit — 2026-08-09

This dated record explains the repository-wide tool trial that produced the
current command architecture and documentation. Counts and timings are evidence
from this pass, not permanent live metrics; `architecture`, `self-test`, and
`docs/TOOLS.md` are the current authorities.

## Scope

Before cleanup, the main repository contained 129 Rust packages and 122 binary
targets under `tools/`. The pass exercised:

- all 91 commands then present in the nine dispatcher groups;
- all 31 binaries then outside the dispatcher;
- every library-only support crate through its consumers and focused tests;
- the separate `alchemy-gcc` Rust driver, GCC 2.96 regressions, staging check,
  and tracked Git hooks; and
- representative real worktree, compiler-dump, overlay, asset, and ROM-local
  inputs in addition to runtime self-tests.

“Exercised” means the safest meaningful combination of help/parser checks,
self-test, focused fixtures, and a real read-only or temporary-output run. It
does not mean launching every combinatorial search to exhaustion. Temporary
outputs were confined to `/private/tmp` or ignored repository output paths.

The resulting inventory is 128 packages and 121 binaries: 99 public commands
in nine dispatcher groups and 22 explicitly non-public binaries. The native
self-test runner currently discovers 112 meaningful self-tests. These counts
are checked from Cargo metadata and the dispatcher registry rather than copied
into a second hand-maintained inventory.

## Representative real trials

- `discover` read the local ROM and reported 2,477 functions, 215,055
  instructions, and 15,255 calls.
- Resource 0x19 was extracted by ID and address, exported, imported, and
  tilemap-round-tripped byte-for-byte. PNG tile import recovered the original
  1,024-byte tile input.
- The kind-2 codec encoded 16,384 decoded bytes to 336 bytes and verified the
  real resource at 0x084baab0; it remains an internal build protocol.
- Every overlay command was tried against real owners. The resource 373 audit
  exposed invalid-input swallowing and advisory exit-status ambiguity.
- `candidate-explain` generated a real scheduler dump for resource 373; the RTL
  and Thumb readers consumed it successfully. They remain internal diagnostics
  because their useful entry point is a public compiler investigation.
- The fixed compiler-corpus selector compiled a routed owner from `exact/` and
  reported `available=1 selected=1 exact=1 regressions=0`.
- The dashboard self-test and live service derived Exact C directly from the
  worktree and were checked through localhost and the configured LAN address.

## What changed because of the trial

- Eight genuinely contributor-facing workflows were promoted into the public
  dispatcher: resource extract/export/import/tilemap, discovery, match
  integration, Exact-C progress, and overlay disassembly.
- Every other binary gained an explicit enforced class: internal diagnostic,
  self-test/support, or bench/auxiliary. Adding an accidental binary now fails
  architecture instead of silently increasing the tool count.
- Self-test discovery now uses Cargo metadata and that policy rather than
  searching Rust source text for a magic string.
- Public command documentation is checked command-by-command against
  `docs/TOOLS.md`, not merely by the existence of nine broad headings.
- Public help and unknown-option handling were standardized across the
  commands that previously rejected `--help`, ignored it, or started expensive
  default work.
- Missing runtime self-tests were added for build, metric, check, showcase, and
  stream commands.
- `text-bg` was removed after proving it had no consumers and its tile-entry,
  flip, tilemap, and localization responsibilities were already owned by active
  asset crates.
- Ignored cleanup removed two nested scratch build caches and the unreferenced
  `toolchain/alchemy-gcc` bundle duplicate. The live compiler path is the
  separately maintained `alchemy-gcc/dist`; valid build, inventory, and
  permutation caches were retained so ordinary verification stays fast.
- The resource 373 scratch investigation was reduced from roughly 1.22 GiB to
  16 MiB by removing reproducible compiler intermediates, linked images, and
  duplicate generated reports while retaining its source, target bytes, and
  compact evidence. In total this reclaimed about 1.20 GiB without throwing
  away the useful investigation.
- Two abandoned compiler worktrees were audited and removed. Their distinct
  experiments remain recoverable as named Git stashes, and the small ignored
  assembly/scheduler evidence set was copied to a private temporary archive.

## Defects found and closed

- compiler corpus selection searched obsolete `src/*.c` paths instead of live
  `exact/*.c` owners;
- remaining-region surveys depended on the caller's current directory;
- semantic scope expected an obsolete nested layout and queues retained
  resolved blockers;
- symbol and overlay scans silently ignored invalid spans, failures, or owners,
  and one scanner hid results behind an undocumented cutoff;
- overlay certification had no failing check mode;
- dashboard LAN binding displaced localhost and duplicated the risk of state
  divergence;
- the permuter could accept unsafe dependency reorderings, had no absolute
  iteration ceiling, and could clean an insufficiently constrained output
  path;
- compiler bundle staging and consumption had no shared lock, so an admitted
  process could observe a mixed or post-validation compiler tuple;
- compiler cache identity did not record executable mode or symlink type, and
  a warmed corpus cache could be returned before current compiler admission;
- consumer-side environment overrides could split the compiler path used for
  staging from the canonical bundle used for routing;
- the documented gate covered groups but not individual commands;
- several tools had real warning, help, and self-test gaps; and
- two source-citation tests encoded the obsolete TypeScript tree rather than
  the current Rust corpus.

## Kept deliberately

The high package count is mostly library ownership, not 128 public programs.
All eleven library-only crates had real reverse dependencies and passing tests.
The compiler/RTL readers, low-level kind-2 builder, cache helper, overlay
publication reader, lints, worker helper, and benches each had a demonstrated
consumer or diagnostic role, so deleting them would either duplicate logic or
remove evidence needed after a public command isolates a problem.

## Cost and caching observations

Focused native operations are generally sub-second once release artifacts
exist. Cold whole-overlay inventory took more than 30 seconds and generated a
large report; the valid cached run took about 1.3 seconds. A compiler-mode
fixture took about 8.2 seconds cold and 0.34 seconds warm. Full routed corpora,
asset scans, ROM builds, and `make verify` remain intentionally broader.

The resulting policy is to iterate with a focused command and a complete cache
identity, retain valid ignored caches, and run the authoritative gate once
before committing. Removing every cache or invoking the full gate after every
source edit is neither stronger evidence nor a sensible contributor loop.

## Boundaries

The trial preserved the pre-existing resource 373 and compiler work. It did not
publish ROM-derived reports, run network searches, alter compiler licensing, or
claim that a self-test replaces byte-exact verification. The historical Python
and TypeScript implementations were not restored; provenance citations to
retired tools remain recoverable through `PROVENANCE.md`.
