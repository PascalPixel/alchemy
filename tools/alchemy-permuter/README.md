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
plus `report.json` under ignored `out/alchemy-permuter/`. `--resume` journals
successful measurements incrementally. Its cache identity includes the target
and compiler inputs, so changed compiler evidence cannot reuse a stale score.

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
