# Building Alchemy

This page builds **The Broken Seal** ☀️ from source in all six editions and
checks each one against the SHA-1 in [`rom.sha1`](rom.sha1). ☀️ needs no ROM:
every byte comes from the source in this repository. **The Lost Age** ⚓️ still
copies part of itself from your own cartridge dump; see
[The Lost Age](#the-lost-age-) below.

## What you need

- macOS or Linux. The whole build is tested on Apple Silicon macOS and on
  64-bit ARM Linux (Debian 12). [agscc's README](agscc/README.md#hosts) lists
  the other hosts its compilers were tested on. Other systems are untested.
- A C compiler (clang or GCC), `make`, `git`, `curl`, `patch`, `xz`, and
  `sha1sum` or `shasum`.
- Rust with Cargo. CI uses Rust 1.98.1.
- A network connection for the first setup. It reaches GitHub (the compiler
  submodules), crates.io (the locked Rust dependencies) and ftp.gnu.org (GNU
  binutils 2.10 and 2.33.1 and GCC 2.95.2, each checked against a pinned
  SHA-256 before use). After setup, building needs no network.

## Get the source

```sh
git clone --recursive https://github.com/PascalPixel/alchemy.git
cd alchemy
```

In a clone made without `--recursive`, run
`git submodule update --init --recursive` first.

## Build the compilers

```sh
make bootstrap
```

This builds the `alchemy` tool, then the pinned assembler, linker and the
three compilers from the `agscc` submodule's source, then the compilers'
runtime library. Everything it makes goes in `tools/out/`. On an 18-core Apple
Silicon Mac it takes about two minutes.

The compilers are one fixed set: GCC 2.96 of 31 July 2000 for the games'
Thumb code, GCC 2.95.2 for their ARM code, and pret's agbcc for the sound and
flash libraries. [AGENTS.md](AGENTS.md#compilers) gives each one's options and
the reasons for them.

## Build The Broken Seal ☀️

```sh
make compare-tbs
```

This builds all six editions and checks each against `rom.sha1`. Each prints
one line:

```
out/tbs-ja/tbs-ja.gba: OK
out/tbs-en/tbs-en.gba: OK
...
```

The built ROMs are `out/<edition>/<edition>.gba`. To build one edition, run
`make compare-tbs-en` and so on, with `ja`, `en`, `de`, `es`, `fr` or `it`;
`make compare` is the Japanese edition. To check against your own cartridge,
compare your dump's SHA-1 with the line for its edition in `rom.sha1`.

## See the compile

```sh
make V=1 compare-tbs-en
```

prints every command the build runs, one line each, as a shell would read
it: each C file's compiler with its full options, each assembler, the linker,
objcopy and the header fix-up. A step the build cache already holds prints
after `# cached: `, so the list is always the whole build. The same option is
`alchemy build rom --target tbs-en --commands`. Every game Thumb file of an
edition compiles with one option set, for example ☀️'s:

```
tools/out/compilers/xgcc -Btools/out/compilers/ -O2 -mthumb -mthumb-interwork -mcpu=arm7tdmi -nostdinc -fcall-used-r4 -I... -DTBS_EDITION_EN=1 -Iout/tbs-en -S -o out/tbs-en/obj/games/tbs/src/battle/tgtexec.s games/tbs/src/battle/tgtexec.c
```

## The Lost Age ⚓️

Part of ⚓️ is still copied from the original: its scaffold under `recon/tla`
includes bytes from your own dump with `.incbin "baserom.gba"`, as pret's
projects did before they finished. Put your dumps in `roms/`, named
`tla-ja.gba`, `tla-en.gba`, `tla-de.gba`, `tla-es.gba`, `tla-fr.gba` and
`tla-it.gba`. The build checks each file's size and SHA-256 before it uses it.
Then:

```sh
make compare-tla       # the Japanese edition
make compare-tla-en    # one other edition
make compare-editions  # all twelve editions of both games
```

`roms/` and every `.gba` file are ignored by git. Never commit a ROM or any
other copy of the original games.

## A current GCC

```sh
make modern
```

This compiles the six ☀️ editions with a current `arm-none-eabi-gcc` into
`out/modern`, as pret's `make modern` does. The result does not match the
original and is not checked against `rom.sha1`; it must compile without
errors, and its warnings are written to `out/modern/warnings.txt`. Install the
compiler from your package manager (for example Homebrew's
`arm-none-eabi-gcc`, or Debian's `gcc-arm-none-eabi`), or name another one with
`make modern MODERN_CC=/path/to/arm-none-eabi-gcc`.

## Tests

```sh
make test
```

runs the tools' tests. `make verify` runs every check and compares all twelve
editions, so it needs the ⚓️ dumps as well.
