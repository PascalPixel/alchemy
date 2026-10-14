# Instructions

These instructions build **The Broken Seal** ☀️ from source in all six
editions and check each one against the SHA-1 in [`rom.sha1`](rom.sha1). ☀️
needs no ROM: every byte comes from the source in this repository. **The Lost
Age** ⚓️ still copies part of itself from your own cartridge dump; see
[The Lost Age](#the-lost-age-) below.

The build is tested on Apple Silicon macOS and on 64-bit ARM and x86-64
Linux. [agscc's README](agscc/README.md#hosts) lists the other hosts its
compilers were tested on. Other systems are untested.

## macOS

Install the Xcode command line tools (`xcode-select --install`), which bring
clang, `make`, `git`, `curl` and `patch`, then the ARM binutils and `xz` (for
example `brew install arm-none-eabi-binutils xz`) and Rust with Cargo from
[rustup](https://rustup.rs). CI uses Rust 1.98.1.

## Linux

Install a C compiler (clang or GCC), `make`, `git`, `curl`, `patch`, `xz`,
`sha1sum`, the ARM binutils and Rust with Cargo from
[rustup](https://rustup.rs). On Debian/Ubuntu-based distributions:

```sh
sudo apt install build-essential git curl patch xz-utils binutils-arm-none-eabi
```

## Installation

The first setup needs a network connection. It reaches GitHub (the compiler
submodules), crates.io (the locked Rust dependencies) and ftp.gnu.org (GNU
binutils 2.10 and GCC 2.95.2, each checked against a pinned SHA-256 before
use). After setup, building needs no network.

```sh
git clone --recursive https://github.com/PascalPixel/alchemy.git
cd alchemy
make bootstrap
```

In a clone made without `--recursive`, run
`git submodule update --init --recursive` first.

`make bootstrap` builds the project's small tools, then the three compilers
from the `agscc` submodule's source, the assembler their output needs (GNU
binutils 2.10's), and the compilers' runtime library. Everything it makes goes
in `tools/out/`. On an 18-core Apple Silicon Mac it takes about two minutes.

The compilers are one fixed set: GCC 2.96 (the development snapshot of 31
July 2000, which names itself `2.96 20000731 (experimental)`; the FSF never
released a 2.96) for the games' Thumb code, GCC 2.95.2 for their ARM code,
and pret's agbcc for the sound and flash libraries.
[CONTRIBUTING.md](CONTRIBUTING.md#compilers) gives each
one's options and the reasons for them.

## Build The Broken Seal ☀️

```sh
make -j8 compare-tbs
```

This builds all six editions and checks each against `rom.sha1`. Each prints
one line:

```
out/tbs-ja/tbs-ja.gba: OK
out/tbs-en/tbs-en.gba: OK
...
```

The built ROMs are `out/<edition>/<edition>.gba`.

# Building guidance

## Parallel builds

Use `make -jN`, with N about the number of cores.

## Compare ROM to the original

To build one edition, run `make compare-tbs-en` and so on, with `ja`, `en`,
`de`, `es`, `fr` or `it`; `make compare` is the Japanese edition. To check
against your own cartridge, compare your dump's SHA-1 with the line for its
edition in `rom.sha1`.

## The Lost Age ⚓️

Part of ⚓️ is still copied from the original: its scaffold under `recon/tla`
includes bytes from your own dump with `.incbin "baserom.gba"`, as pret's
projects did before they finished. Put your dumps in `roms/`, named
`tla-ja.gba`, `tla-en.gba`, `tla-de.gba`, `tla-es.gba`, `tla-fr.gba` and
`tla-it.gba`. The build checks each against `rom.sha1` before it uses it.
Then:

```sh
make compare-tla-ja    # the Japanese edition
make compare-tla       # all six ⚓️ editions
make compare-editions  # all twelve editions of both games
```

`roms/` and every `.gba` file are ignored by git. Never commit a ROM or any
other copy of the original games.

## A modern compiler

```sh
make modern
```

This compiles the six ☀️ editions' C with a current `arm-none-eabi-gcc`, as
pret's `make modern` does: every file must compile without errors, and the
warnings are for the cleanup to read. Install the compiler from your package
manager (for example Homebrew's `arm-none-eabi-gcc`, or Debian's
`gcc-arm-none-eabi`), or name another one with
`make modern MODERN_CC=/path/to/arm-none-eabi-gcc`.

## See the build

Every rule the ROM is built by is in the [Makefile](Makefile), and make prints
each command as it runs it: each C file's compiler with its full options, each
assembler, each overlay's link and packing, the main link, objcopy and the
header fix-up. Every game Thumb file of an edition compiles with one option
set, for example ☀️'s:

```
tools/out/compilers/xgcc -Btools/out/compilers/ -O2 -mthumb -mthumb-interwork -mcpu=arm7tdmi -nostdinc -fcall-used-r4 -I... -DTBS_EDITION_EN=1 -Iout/tbs-en -S -o out/tbs-en/obj/games/tbs/src/battle/battle.s games/tbs/src/battle/battle.c
```

## Tests

```sh
make test
```

runs the tools' tests. `make land` builds and compares all twelve editions,
runs the tests and counts progress, so it needs the ⚓️ dumps as well.

# Useful additional tools

The `alchemy` tool (`tools/out/cargo-target/release/alchemy`) holds the
matching aids and the progress count; `alchemy --help` lists them:

- `alchemy diff DRAFT.c` compares a C draft with the code it would replace.
- `alchemy permute DRAFT.c` searches rewrites of a draft toward a match.
- `alchemy progress` counts DONE code and data in each edition built
  (`make progress`).
