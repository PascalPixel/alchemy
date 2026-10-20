# Building Alchemy

The Broken Seal builds on Linux, macOS and Windows with nothing but this repository. The Lost Age is optional and also needs your own cartridge dumps.

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

`make bootstrap` builds the compilers once. It downloads their sources and prints plenty of old-compiler warnings, which is normal. `make tbs-en` builds the English Broken Seal as `out/tbs-en/tbs-en.gba`, ready for an emulator. The other languages are `tbs-ja`, `tbs-de`, `tbs-es`, `tbs-fr` and `tbs-it`.

To check a build against the original, run `make compare-tbs-en`. It ends with `OK` when every byte matches.

## 3. Change something

- `games/tbs/text/en.po` holds every line of English dialogue.
- `games/tbs/src` holds the code, with the pictures beside it as PNG files.
- `games/tbs/sound` holds the music and sound effects.

Edit, run `make tbs-en` again, and play your version. A changed game no longer matches the original, so `compare` will report a difference. Please don't share built ROMs, original or modified.

CI checks formatting with clang-format 21, available from [LLVM's packages](https://apt.llvm.org/). To use the same check locally, run `make format-check CLANG_FORMAT=clang-format-21`, or set `CLANG_FORMAT` to the path of your clang-format 21 executable.

## The Lost Age (optional)

Dump your own cartridge and put the file in `roms/` as `tla-en.gba` (or `tla-ja.gba`, `tla-de.gba` and so on), then run `make tla-en`. Don't download ROMs; the build checks each one against `rom.sha1`.

## If something fails

- **`png.h`, `utf8proc.h` or `arm-none-eabi-as` not found:** one of the packages in step 1 is missing.
- **`alchemy-gcc` is empty:** rerun `git submodule update --init --recursive alchemy-gcc`, then `make bootstrap`.
- **Odd path errors:** clone into a folder whose path has no spaces.

`make help` lists every other command.
