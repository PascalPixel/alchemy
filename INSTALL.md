# Building Alchemy

Alchemy is a [matching decompilation](games/tbs/glossary.md#matching-decompilation) of the two Game Boy Advance Golden Sun games: it rebuilds Golden Sun: The Broken Seal (`tbs`) and its sequel, Golden Sun: The Lost Age (`tla`), byte for byte, from the source in this repository. [README.md](README.md) says more about the project.

Every byte of The Broken Seal now comes from source, its pictures, music and text included, so it builds on Linux, macOS and Windows with nothing but this repository. The Lost Age is optional: its code that isn't rewritten yet is still read from your own cartridge dump.

## 1. Install the tools

**Linux** (Debian or Ubuntu):

```bash
sudo apt install build-essential git curl patch xz-utils binutils-arm-none-eabi pkg-config libpng-dev libutf8proc-dev
```

On other distributions, install the same packages under their local names.

**macOS** (with [Homebrew](https://brew.sh)):

```bash
xcode-select --install
brew install arm-none-eabi-binutils xz pkg-config libpng utf8proc
```

**Windows:** open PowerShell, run `wsl --install`, restart, then open **Ubuntu** from the Start menu and follow the Linux steps inside it.

## 2. Build

```bash
git clone https://github.com/PascalPixel/alchemy
cd alchemy
git submodule update --init --recursive alchemy-gcc
make bootstrap
make tbs-en
```

`make bootstrap` builds, once, the [old compilers](games/tbs/glossary.md#the-compilers) the games were matched with: alchemy-gcc and pret's old_agbcc from the submodule, and GCC 2.95.2 and GNU's assembler 2.10, whose sources it downloads from ftp.gnu.org. A current compiler would produce different instructions, so the system's `arm-none-eabi-gcc` can't stand in for them. It prints plenty of old-compiler warnings, which is normal, and the compilers and build tools take about 500 MB. `make tbs-en` builds the English Broken Seal as `out/tbs-en/tbs-en.gba`, ready for an emulator. The other languages are `tbs-ja`, `tbs-de`, `tbs-es`, `tbs-fr` and `tbs-it`.

To check a build against the original, run `make compare-tbs-en`. It needs no cartridge: it compares the SHA-1 hash of the built file with the original's, recorded in `rom.sha1`, and ends with `OK` when they are equal. Every edition has its `compare-` target, `compare-tbs` checks all six of The Broken Seal, and `make -j8` builds in parallel.

## 3. Change something

- `games/tbs/text/en.po` holds every line of English dialogue.
- `games/tbs/src` holds the code, with the pictures beside it as PNG files.
- `games/tbs/sound` holds the music and sound effects.

Edit, run `make tbs-en` again, and play your version. A changed game no longer matches the original, so `compare` will report a difference. Please don't share built ROMs, original or modified.

CI checks formatting with clang-format 21, available from [LLVM's packages](https://apt.llvm.org/). To use the same check locally, run `make format-check CLANG_FORMAT=clang-format-21`, or set `CLANG_FORMAT` to the path of your clang-format 21 executable.

## Reconstruction tools (optional)

The pinned `alchemy-gcc` submodule supplies the compilers. The tools for matching, finding C that compiles to a function's original instructions, and for measuring how much is rebuilt live in the optional `alchemy-devtools` submodule and require Rust:

```sh
git submodule update --init alchemy-devtools
make devtools
alchemy-devtools/target/release/alchemy-devtools --help
```

The documentation site, [alchemy.pascalpixel.com](https://alchemy.pascalpixel.com/), is built separately in [alchemy-docs](https://github.com/PascalPixel/alchemy-docs); building the games needs neither Doxygen nor Node.

## The Lost Age (optional)

Dump your own cartridge and put the file in `roms/` as `tla-en.gba` (or `tla-ja.gba`, `tla-de.gba` and so on), then run `make tla-en`, and `make compare-tla-en` to check it. The build copies the parts not yet rewritten as source from the dump, so it needs the dump of the same edition. Don't download ROMs; the build checks each one against `rom.sha1`.

## If something fails

- **`png.h`, `utf8proc.h` or `arm-none-eabi-as` not found:** one of the packages in step 1 is missing.
- **`alchemy-gcc` is empty:** rerun `git submodule update --init --recursive alchemy-gcc`, then `make bootstrap`.
- **Odd path errors:** clone into a folder whose path has no spaces.

`make help` lists every other command. Once the build works, [the documentation](https://alchemy.pascalpixel.com/) explains how the game works, and [the source's guide](games/tbs/src/README.md) what lives in which folder.

## Upgrading an old checkout

The tools' submodules were renamed. In a checkout from before the rename, run `git submodule sync --recursive` and initialize the submodules as above. The old command `psynergy` is now `alchemy-devtools`, and `make psynergy` is now `make devtools`.
