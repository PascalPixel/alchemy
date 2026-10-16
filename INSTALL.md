# Build Alchemy

These instructions build **The Broken Seal** (`tbs`) in all six editions, then check the results against the published hashes. It needs no input ROM: its code and data are rebuilt from the source in this repository.

**The Lost Age** (`tla`) is still incomplete and needs your own cartridge dumps. Its additional setup is [below](#the-lost-age).

The full build is tested on Apple Silicon macOS and 64-bit ARM and x86-64 Linux. [agscc's host notes](agscc/README.md#hosts) describe the compiler tests on other hosts. Native Windows and WSL are not currently validated by this project.

## 1. Install prerequisites

### Linux

Install a C compiler, Make, Git, curl, patch, xz, sha1sum, ARM binutils, pkg-config, libpng and utf8proc. On Debian or Ubuntu:

```sh
sudo apt install build-essential git curl patch xz-utils binutils-arm-none-eabi pkg-config libpng-dev libutf8proc-dev
```

### macOS

With [Homebrew](https://brew.sh) installed:

```sh
brew install arm-none-eabi-binutils xz pkg-config libpng utf8proc
```

The build uses the Clang, Make and Git supplied by Apple's command-line tools. No Rust or Rosetta is needed.

## 2. Get the source and build the tools

```sh
git clone https://github.com/PascalPixel/alchemy.git
cd alchemy
git submodule update --init --recursive agscc
make bootstrap
```

Use a checkout path without spaces. The `agscc` submodule supplies the matching compilers. Building and modding need only the C tools in `tools/`; Rust and the optional `psynergy` submodule are for reconstruction work ([AGENTS.md](AGENTS.md#commands)).

The first setup downloads the compiler sources from GitHub, and GNU binutils 2.10 and GCC 2.95.2 from ftp.gnu.org. The GNU archives are checked against pinned SHA-256 hashes. Once setup is complete, building needs no network connection.

`make bootstrap` builds the tools, the pinned compilers, their assembler and runtime library. Generated tools go under `tools/out/`; no compiler is installed system-wide. The historical compiler sources emit many warnings on current hosts; look for the command's exit status and a final Make error when diagnosing a failure.

The matching compiler choices and their fixed options are documented in [AGENTS.md](AGENTS.md#compilers); their source is in the `agscc` submodule.

## 3. Build and verify The Broken Seal

For the English edition:

```sh
make compare-tbs-en
```

Success ends with:

```text
out/tbs-en/tbs-en.gba: OK
```

To build and verify all six editions:

```sh
make -j8 compare-tbs
```

Each edition prints its own `OK`. The output files are `out/<edition>/<edition>.gba`. Language suffixes are `ja`, `en`, `de`, `es`, `fr` and `it`. Choose a parallel job count near your available CPU cores.

`compare` checks against [rom.sha1](rom.sha1), so it is for the unchanged game. After deliberately changing the game, use `make tbs-en` to build without that check.

## The Lost Age

Put your own dumps in `roms/`, named `tla-ja.gba`, `tla-en.gba`, `tla-de.gba`, `tla-es.gba`, `tla-fr.gba` and `tla-it.gba`. Each edition's build checks its input against `rom.sha1` before using it. You need only the dump for the edition you choose to build:

```sh
make compare-tla-en    # English only
make -j8 compare-tla   # all six TLA editions
make -j8 compare-editions  # both games, all twelve editions
```

The unfinished source under `recon/tla` copies the parts still being reconstructed from those dumps. `roms/` and generated `.gba` files are ignored by Git and must never be committed.

## Editors and a current compiler

[clangd](https://clangd.llvm.org/installation.html) uses the checked-in `.clangd` and `compile_flags.txt` for completion and diagnostics. Build the edition first so its generated message headers exist. The editor configuration uses the English modern-build view; it does not check every edition or the matching compiler's behavior. The C build tools get their own host compiler flags from `make tools`, including the installed library headers. Use `clang-format` with `.clang-format` for formatting, and reload the language server after changing its configuration.

The optional `make modern` check compiles the six first-game editions' C with a current ARM GCC. Install `arm-none-eabi-gcc` on Homebrew or `gcc-arm-none-eabi` on Debian/Ubuntu, then run:

```sh
make modern
```

Or select a compiler explicitly:

```sh
make modern MODERN_CC=/path/to/arm-none-eabi-gcc
```

Check `arm-none-eabi-gcc --version` if the command rejects a compiler option. GCC 16.2.0 is tested on macOS. The old Arm 2019 toolchain's GCC 9.2.1 does not accept the current diagnostic flags. This optional compiler is separate from the historical matching compilers built by `make bootstrap`.

This produces objects under `out/<edition>/modern/`. It checks that the C compiles; it does not link a ROM, create a PC executable or compare hashes.

## Troubleshooting

| Symptom | Check |
| --- | --- |
| `png.h`, `utf8proc.h` or a pkg-config package is missing | Install libpng, utf8proc and pkg-config, including development packages on Linux, using the commands above. |
| `arm-none-eabi-as`, `ld` or `ar` is missing | Install ARM binutils and ensure its executables are on `PATH`. |
| `agscc` is empty or compiler sources are missing | Run `git submodule update --init --recursive agscc`, then `make bootstrap`. |
| A download fails during bootstrap | Check access to GitHub and ftp.gnu.org, then rerun `make bootstrap`. |
| `make modern` rejects `-fdiagnostics-plain-output` | Select a current ARM GCC with `MODERN_CC`; check which installation your shell finds. |
| An ARM disassembly test differs only in instruction spelling | Check `command -v arm-none-eabi-objdump` and `arm-none-eabi-objdump --version`. On Apple Silicon, Homebrew normally uses `/opt/homebrew/bin`; put it before older ARM tool installations on `PATH`. |
| A TLA dump is missing or its hash fails | Check its edition, filename and SHA-1 against `rom.sha1`. |
| Hash comparison fails after an intentional edit | Build with `make tbs-en`; a modified game is expected to have a different hash. |
| An unchanged checkout fails | Save the first error and the command, revision, OS and tool versions when reporting it. |

Make prints the build commands as they run. To remove generated game files and rebuild, `make clean` removes `out/` and generated assets, while retaining the source and bootstrapped tools. Save any generated output you want to keep first.
