# Building Alchemy

Want to build Golden Sun yourself? Three steps, and you have a ROM you made from source.

Alchemy is a [matching decompilation](games/tbs/glossary.md#matching-decompilation) of Golden Sun: The Broken Seal (`tbs`) and its sequel, Golden Sun: The Lost Age (`tla`): it rebuilds both, byte for byte, from the source in this repository. [README.md](README.md) says more about the project.

Every byte of The Broken Seal comes from source, its pictures, music and text included. So it builds on Linux, macOS and Windows with nothing but this repository. The Lost Age builds the same way, from this repository alone, with no cartridge dump.

## 1. Install the tools

**Linux** (Debian or Ubuntu):

```bash
sudo apt install build-essential binutils-arm-none-eabi git libpng-dev
```

On other distributions, install the same packages under their local names.

**macOS** (with [Homebrew](https://brew.sh)):

```bash
xcode-select --install
brew install arm-none-eabi-binutils libpng
```

**Windows:** open PowerShell, run `wsl --install`, restart, then open **Ubuntu** from the Start menu and follow the Linux steps inside it.

## 2. Build

```bash
git clone --recurse-submodules https://github.com/PascalPixel/alchemy
cd alchemy
make
```

The first `make` takes a while, because it starts by building the old compilers the games were matched with: alchemy-gcc, pret's old_agbcc, GCC 2.95.2 and GNU's assembler 2.10. They all come from source in the `alchemy-gcc` submodule, with no download, and they are built only once. A current compiler would produce different instructions, so the system's `arm-none-eabi-gcc` can't stand in for them.

Expect plenty of old-compiler warnings, which is normal, and about 500 MB for the compilers and build tools.

Then it builds the English Broken Seal as `out/tbs-en/tbs-en.gba`, ready for an emulator. Other languages build with `make tbs-ja`, `tbs-de`, `tbs-es`, `tbs-fr` or `tbs-it`.

To check your build against the original, run `make compare`. It needs no cartridge: it compares the SHA-1 hash of the built file with the original's, recorded in `rom.sha1`, and ends with `OK` when they are equal.

Every edition has its own `compare-` target, such as `compare-tbs-ja`, and `compare-tbs` checks all six of The Broken Seal. `make -j8` builds in parallel.

## 3. Change something

- `games/tbs/text/en.po` holds every line of English dialogue.
- `games/tbs/src` holds the code, with the pictures beside it as PNG files.
- `games/tbs/sound` holds the music and sound effects.

Edit, run `make` again, and play your version. A changed game no longer matches the original, so `compare` will report a difference, which is exactly what it should do. Please don't share built ROMs, original or modified.

CI checks formatting with clang-format 21, available from [LLVM's packages](https://apt.llvm.org/). To use the same check locally, run `make format-check CLANG_FORMAT=clang-format-21`, or set `CLANG_FORMAT` to the path of your clang-format 21 executable.

## Reconstruction tools (optional)

The tools for matching, finding C that compiles to a function's original instructions, and for measuring how much is rebuilt are [Alchemy Devtools](https://github.com/PascalPixel/alchemy-devtools). They need Rust, and the first `make devtools` clones them into `alchemy-devtools/`:

```sh
make devtools
alchemy-devtools/target/release/alchemy-devtools --help
```

The documentation site, [alchemy.pascalpixel.com](https://alchemy.pascalpixel.com/), is built separately in [alchemy-docs](https://github.com/PascalPixel/alchemy-docs); building the games needs neither Doxygen nor Node.

## The Lost Age

Run `make tla-en` (or `tla-ja`, `tla-de`, `tla-es`, `tla-fr` or `tla-it`), and `make compare-tla-en` to check it against `rom.sha1`; `compare-tla` checks all six. Like The Broken Seal, it needs no cartridge dump.

## If something fails

- **`png.h`, `libpng-config` or `arm-none-eabi-as` not found:** one of the packages in step 1 is missing.
- **`alchemy-gcc` is empty:** the clone was made without `--recurse-submodules`; run `git submodule update --init --recursive`, then `make`.
- **Odd path errors:** clone into a folder whose path has no spaces.

`make help` lists every other command. Once the build works, [the documentation](https://alchemy.pascalpixel.com/) explains how the game works, and [the source's guide](games/tbs/src/README.md) what lives in which folder.

## Upgrading an old checkout

Alchemy Devtools used to be a submodule. In an older checkout, `git pull` leaves `alchemy-devtools/` where it is, and `make devtools` keeps using it; then run `git submodule update --init --recursive` and `make`.
