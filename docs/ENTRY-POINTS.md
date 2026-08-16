# Entry points: the consolidation, and how to finish it

**Do not re-derive any of this. Execute it.**

This file exists because one agent has worked this repository across many
sessions for a month, and every session independently concluded that merging
tooling was too risky and deferred it. Deferring is why `tools/` sat at 121
executables. There is no better-placed later reader. There is only the same work,
later, with more of it.

## The problem, stated once

132 crates shipped 121 separate executables. Nothing could enumerate them, so
agents repeatedly rebuilt work that already existed:

- `reverse-gcc296` reimplemented three `alchemy-permuter` passes, `shape-sweep`'s
  `InlineSingleUseTemp`, and all of `statement-order-sweep-main`
- a duplicate of `tools/architecture`'s registration check, written *while
  auditing for duplicates*

Names were blamed. Names are a symptom. **The disease is entry-point count.**

## The principle

Only 32 crates are invoked by Makefile targets. The other ~100 are one-shot
extractors whose output is already committed: 11,636 files under `assets/`.

They are not "used", but they are not deletable either. In a clean-room
decompilation the extractor is the **provenance record**: the executable proof
that an asset was derived from the ROM by a documented transform. Delete it and
the output survives while the evidence does not.

> **Executables are what you RUN. Everything else is a documented derivation, and
> derivations are libraries.**

That is why the target is ~9 and not 121, and it is why nothing gets deleted.

## Naming: settled, do not reopen

`data battle-display` is discoverable. `sentou-hyouji` is the honest record of the
ROM's own subsystem name. **Rename the surface, keep the source.** No mass rename
is needed and none should be attempted; it is weeks of churn for nothing that
`--list` and `architecture --search` do not already solve.

## Done so far (121 -> 99)

| entry point | absorbed | note |
|---|---|---|
| `overlay` | 12 crates | template for linking crates that already export `run` |
| `resource` | 5 crates | |
| `data` | 7 crates | 4 linked + 3 extracted |
| `rtl` | 2 crates | both extracted |
| `dev` | 3 crates | `permute`, `residuals`, `bl-symbols` |
| `shape-sweep` | `statement-order-sweep-main` deleted, `reverse-gcc296` folded in | |

Plumbing that had to change, and is now in place:

- `dispatch::Target::Sub(path, subcommand)` so several commands share a binary
- `architecture` identity is `(crate, binary, subcommand)`; "one binary per
  command" was correct at 121 and is exactly what consolidation must break

## The two mechanical recipes

### A. Crate already exports `pub fn run`

Copy `tools/overlay/src/main.rs`. Add path deps, write one match arm per command,
**lift each adapter from that crate's own `main.rs`** -- signatures are never
uniform (seven distinct shapes across the twelve overlay crates). Then strip
`[[bin]]`, repoint the `dispatch` entries to `Target::Sub`, delete stale
`NonPublicTarget` rows.

### B. Logic lives inside `main.rs` (~75 crates)

Scripted, with auto-revert. The script is NOT checked in: `lang-ban` correctly
bans non-Rust files under `tools/`, and tooling there must be a workspace crate.
The seven steps below are the whole algorithm, so rewrite it in the scratchpad:

1. move `src/main.rs` to `src/cli.rs`
2. `fn main` becomes `pub fn entry(arguments: &[String]) -> ExitCode`
   **not `run`** -- many of these files already define a private `fn run`, and the
   collision failed all five of the first batch
3. replace the `env::args().skip(1).collect()` binding with `arguments.to_vec()`
4. rewrite `mycrate::` to `crate::`
5. insert `pub mod cli;` **after** the crate's `//!` doc comment, or rustc rejects
   it as a misplaced inner doc comment
6. delete `[[bin]]`
7. **build; on any failure restore the crate from a backup and move on**

Success rate on the one real batch: 6 of 9, with 3 restored cleanly.

## Traps, all of which cost real time

- **Cargo auto-discovers `src/main.rs` as a binary** even with no `[[bin]]`
  section. Removing the section is not enough; the file must go, or set
  `autobins = false`.
- **Some crates `include_str!("main.rs")`** to self-test their own usage text. That
  file is compile-time DATA. Rename it `legacy_entry.rs.txt` and fix the macro.
- **Stale artifacts produce false conclusions.** Three times in one session: a
  parity diff that "failed" against an unrebuilt binary, `cargo test` not
  rebuilding the binary under test, and a lint reading a deleted crate's leftover
  executable. Always rebuild the comparison side; `rm` the old binary after
  stripping `[[bin]]`.
- **`make build-full` already fails on HEAD** (`assembly overlaps another source
  at 0x0800a97c`, the resource_3ce/resource_372 blocker). It cannot be used as a
  smoke test. Verified by stashing and reproducing on a clean tree.
- **`pub fn run` is NOT a reliable signal that a crate exposes a CLI.**
  `build_rom::run(root, command)` is a *subprocess spawner* that executes
  `command[0]`. Linking it as a command adapter made `make build-rom` panic
  instead of failing cleanly. Read what the function DOES, never just its name.
  This is the same class of error as assuming uniform signatures.
- **`for c in $VAR` DOES NOT WORD-SPLIT IN ZSH.** Four consecutive batches
  reported `extracted=0` and were read four times as "these crates resist
  extraction". Each had iterated ONCE over the entire string. Zero attempted, not
  zero succeeded. Use an array: `B=(a b c); for c in $B`. This is the third
  distinct zsh word-splitting failure recorded on this project.
- **Do not run diagnostics that `rm -rf` a crate directory.** One session deleted
  three crates this way and recovered only because everything was committed.
  Diagnose on a copy under the scratchpad.

## Order of work remaining (~90 binaries)

1. Remaining crates exporting `run`: `build-claimed`, `compare-roms`,
   `map-resources`. Recipe A, but VERIFY each `run` is really a CLI entry first
   (see the `build_rom` trap). `build-rom` is excluded: its `run` is a subprocess
   spawner and its real entry is `parse_args` + `main_pipeline`.
2. The archival extractors, recipe B in batches with auto-revert, folded behind a
   single `assets` entry point. This is the bulk: roughly seventy crates behind
   one binary, not seven binaries of ten.
3. `check`, `semantic`, `candidate` groups.
4. `build-assets` (5,291 lines in `main.rs`) LAST and ALONE. It is on the ROM
   build path.

Target end state: `dispatch`, `overlay`, `resource`, `data`, `rtl`, `assets`,
`build`, `check`, plus `alchemy-lints` and the self-tests which must run when
dispatch is broken.
