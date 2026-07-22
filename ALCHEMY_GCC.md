# alchemy-gcc

`alchemy-gcc/` is the local compiler forge for this reconstruction: releasing
the approved code-generation alchemy into the build while keeping compiler
binaries and third-party environments out of the published source tree.

The ignored directory has one boundary and three roles:

```text
alchemy-gcc/
├── xgcc
├── cc1
├── cpp
├── tradcpp
└── m2c-venv/          optional decompilation helper
```

The four top-level executables are the complete compiler required by the
byte-verified C build. They are native arm64 Mach-O programs and depend only on
macOS `libSystem`; compiler source trees, intermediate objects, nested Git
repositories, duplicate drivers, and target runtime archives do not belong in
the bundle. Native `arm-none-eabi` binutils remain a host dependency supplied
through `PATH`, because they are shared generic assembler/linker tools rather
than part of the approved compiler's code generation.

`bun tools/alchemy_gcc.ts` checks the host architecture, executable set,
and approved SHA-256 digests before compiler use. Every build and matching tool
constructs its compiler invocation through that module; no second compiler
path is permitted.

The directory stays ignored. Public commits contain only this specification,
the validator, reconstructed source, and generic tooling—not compiler binaries
or third-party checkouts.

## Provenance

The bundle's executables are built from
[PascalPixel/alchemy-gcc](https://github.com/PascalPixel/alchemy-gcc), a fork
of [Coaltergeist/camelot-gcc](https://github.com/Coaltergeist/camelot-gcc):
vendored FSF GCC source (the gcc-2.96 development snapshot of 2000-07-31,
ARM/elf) with host-portability patches, all documented in that repository.
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
is known to require pret's `old_agbcc` rather than gcc-2.96. Any adoption of
a second approved compiler for those translation units is a governance
decision recorded here when its evidence lands, following the per-unit
compiler precedent of `pret/pokeruby`.
