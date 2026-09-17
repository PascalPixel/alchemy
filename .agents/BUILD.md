# Build, verify, commit

## Setup

Install Rust and Ninja. Supply approved ROMs under ignored `roms/`, initialize
the compiler submodules, and run `make bootstrap`:

```sh
git submodule update --init
git config core.hooksPath .hooks
./alchemy --help
./psynergy --help
make compiler-source-check
make bootstrap
```

If the compiler installation is missing, bootstrap builds the pinned `agscc`
and `agbcc` sources and GNU GAS 2.10, from the
[official release](https://ftp.gnu.org/gnu/binutils/binutils-2.10.tar.gz)
checked against a committed SHA-256 before extraction. It also builds GNU
binutils 2.33.1 for `arm-none-eabi` natively from its
[official release](https://ftp.gnu.org/gnu/binutils/binutils-2.33.1.tar.xz),
checked the same way, into ignored `tools/binutils/`; `alchemy` puts those
tools first on `PATH`, so builds need no system binutils or emulated host
toolchain. No third-party source edits or game flag changes are involved.

The installer checks executable hashes and smoke compilations before
installing the six required executables under ignored `tools/compilers/`.
`bootstrap --check` validates without building; `bootstrap --build` rebuilds
dependencies. `make bootstrap BUNDLE=/path/to/approved/bundle` accepts an
existing distribution. Bootstrap never replaces an existing different
installation or admits new executable hashes. `alchemy build compilers` (or
`make compiler-sources`) builds pinned sources without installing or admitting
executables; a locally built compiler is not necessarily byte-identical to an
approved distribution, and admission still requires Pascal's approval and the
existing reproduction evidence. Downloaded source and temporary compilation
work live under `out/compilers/`.

Initialize submodules only in the main checkout. Worktrees may symlink its
`roms/` and whole `tools/compilers/` directories. The launcher builds current
tooling offline.

## Output

`out/` holds only generated objects, Rust binaries, reports, previews,
downloaded assembler source, build caches and work directories. Deleting it is
normal and needs no compiler recovery or approval, and rebuilding it is the
cure for a stale report. `make clean` preserves maintained game inputs, private
`roms/` and installed `tools/compilers/`. Builds, caches and reports belong in
their existing stable target directories under `out/`; reuse them rather than
adding a directory for every export.

## Verify

Use `alchemy score` while iterating, and narrow builds when needed:
`make build-claimed`, `make build-asm`, `make build-assets`,
`make overlay-check`, `make check-owners`. Do not run the full gate after every
source spelling.

Before committing, stage exactly the intended batch by explicit path, then:

```sh
git diff --cached --check
make verify
make progress-subject
```

The index and worktree must agree: `make verify` refuses unstaged tracked
changes and untracked files, so stage first. It checks staged source, the
production TBS EN ROM, overlays, units and their instances, ownership,
siblings, ordinary C, compiler provenance, repository rules, the document
rule, and publication rules for every tracked file. It does not rescore every
draft, run every test or build twelve ROMs. `make verify-clean` deletes
generated output and runs the gate from scratch.

| Changed surface | Additional check |
| --- | --- |
| Ownership, labels or coverage claims | `make coverage` |
| Rust tooling or dashboard | `make test` |
| Shared edition or preprocessor logic | `make targets` (compile-only) |
| Retained-assembly classification | `make classification-check` |
| Candidate-corpus policy | `make candidate-corpus-check` |
| Tools or documents | `make tooling-index-check` |

An unscored complete candidate is not a parked match. Use `make audit` for
exhaustive release or major compiler and ownership checks and `make reports` to
regenerate analysis, not inside every edit loop.

## Commit and push

The subject starts with the exact prefix `make progress-subject` prints, such
as `☀️ 56% – `, and the hook accepts only the verified tree. A commit written by
an agent ends with that agent's `Co-Authored-By` trailer. Commits need not raise
DONE ([RECOVERY](RECOVERY.md)).

Push only when Pascal asks. Passing verification is not authorization to change
origin. When a push is requested, push `main` only after the batch's
`make coverage`, `make verify` and, when tooling changed, `make test` have
passed on the committed tree. The pre-push hook checks every outgoing commit's
publication rules. `main` is the only branch; nothing else is pushed.
