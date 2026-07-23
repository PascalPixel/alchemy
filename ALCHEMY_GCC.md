# alchemy-gcc

The approved compiler bundle lives in the sibling repository checkout
`../alchemy-gcc/dist/` ([PascalPixel/alchemy-gcc](https://github.com/PascalPixel/alchemy-gcc)):
compiler source, build scripts, and the ignored runtime bundle stay out of
this reconstruction tree entirely.

The bundle directory has one boundary and three approved compiler lanes:

```text
../alchemy-gcc/dist/
├── xgcc, cc1, cpp, tradcpp       # GS1: GCC 2.96 snapshot
├── gs2/
│   └── xgcc, cc1, cpp0, tradcpp0 # GS2: Camelot GCC 3.0 backend
├── agbcc/
│   └── old_agbcc                  # source-scoped stock m4a compiler
└── m2c-venv/          optional decompilation helper
```

The four top-level executables are the GS1 compiler. The four executables under
`gs2/` are the reconstructed GS2 backend. `agbcc/old_agbcc` is a separately
validated compiler used only by the explicit source-address allowlist in
`tools/alchemy_gcc.ts`. Run `./stage.sh gcc296`, `./stage.sh gs2`, and
`./stage.sh agbcc` in the sibling compiler repository after building those
targets; `./stage.sh --check <target>` verifies that an ignored runtime stage
still matches its local build. All nine programs are native arm64 Mach-O
executables and depend only on macOS `libSystem`; compiler source trees,
intermediate objects, nested Git repositories, duplicate drivers, and target
runtime archives do not belong in the bundle. Native `arm-none-eabi` binutils
remain a host dependency supplied through `PATH`, because they are shared
generic assembler/linker tools rather than part of the approved compiler's code
generation.

`bun tools/alchemy_gcc.ts gs1`, `bun tools/alchemy_gcc.ts gs2`, and
`bun tools/alchemy_gcc.ts agbcc` check the host architecture, target-specific
executable set, and approved SHA-256 digests before compiler use. Every build
and matching tool constructs its source-aware compiler invocation through that
module; no unvalidated compiler path is permitted.

The bundle stays outside this repository. Public commits contain only this specification,
the validator, reconstructed source, and generic tooling—not compiler binaries
or third-party checkouts.

## Provenance

The bundle's executables are built from
[PascalPixel/alchemy-gcc](https://github.com/PascalPixel/alchemy-gcc), a fork
of [Coaltergeist/camelot-gcc](https://github.com/Coaltergeist/camelot-gcc):
vendored FSF GCC source (the gcc-2.96 development snapshot of 2000-07-31 for
GS1 and GCC 3.0 with the reconstructed Camelot backend for GS2) with
host-portability patches, all documented in that repository.
One vendored patch affects code generation: `config/arm/elf.h` emits
`.align N, 0` so modern assemblers pad with zeros the way period binutils
did. The compiler contains no game-derived material; it is generic public
toolchain source under the GPL.

The selection of this compiler version originated outside this repository:
the public Golden Sun decompilation community identified the snapshot, and
camelot-gcc packaged and validated it. Within this repository the selection
is independently discharged by the strongest available evidence: hundreds of
byte-identical function matches and a byte-identical full-ROM source build
through this compiler alone. A wrong vintage could not produce either. That
external hint concerns only which public tool to run; every piece of game
knowledge in this tree remains derived from the approved ROM set.

The stock m4a ("Sappy") audio library that period licensees linked verbatim
requires pret's `old_agbcc` rather than gcc-2.96. The source-address allowlist
currently covers `080fa1fc`, `080fa2a0`, `080fa324`, `080fa350`, `080fa39c`,
`080fa3f0`, `080fa424`, `080fa458`, `080fa490`, `080fa514`, `080fa83c`,
`080fa8d4`, `080fa928`, `080fa9a4`, `080fa9e0`, `080fab3c`, `080facf8`,
`080fb2cc`, `080fb334`, `080fb3a8`, `080fb430`, `080fb4a4`, `080fb670`,
and `080fb6a4`; each was independently
assembled and linked to an exact target interval before adoption. No generic
directory or subsystem switch selects this compiler, following the per-unit
compiler precedent of `pret/pokeruby`.

`080fb670` additionally uses old_agbcc's default-off
`-mliteral-before-shift` compatibility mode. It only moves an independent
constant-pool load ahead of an adjacent left shift after proving the two
destinations do not overlap; the other routed units retain stock ordering.

`080fa514` uses `-O1` plus old_agbcc's default-off
`-mcommutative-copy-constant` mode. The backend swaps a destructive
commutative copy/AND pair only when the AND's other register was loaded with a
constant by the immediately preceding instruction and all three registers are
distinct.

`080fb2cc`, `080fb334`, and `080fb3a8` use old_agbcc's default-off
`-mprologue-next-high-reg` mode. After register allocation, it finds the
highest live callee-saved high register and marks its successor live only
during prologue generation. This retains the stock object's otherwise-unused
adjacent high-register save without changing the function body, and the
epilogue sees the same save mask when restoring registers.

The GS1 compiler also exposes a default-off `-mgrouped-dma-store` compatibility
mode for the historical three-word Thumb DMA descriptor sequence. It lowers
three explicit scalar words to `stmia` plus a base restore without inline
assembly or register pins. Only the independently verified `080049e8`,
`08004a28`, `08004a5c`, `080958a8`, and `0809bb34` sources enable it;
ordinary GS1 sources retain the default code generation path. `080049e8`
also uses the existing source-scoped `-O1` route because `-O2` swaps two
independent descriptor loads.
