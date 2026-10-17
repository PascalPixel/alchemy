# Instructions

These instructions explain how to set up the tools required to build **Alchemy**, which assembles the source files into ROMs.

**The Broken Seal** (`tbs`) builds entirely from this repository; no ROM is needed. **The Lost Age** (`tla`) is still in progress and needs your own cartridge dumps (see [Build The Lost Age](#build-the-lost-age)).

These instructions come with notes which can be expanded by clicking the "<i>Note...</i>" text. In general, you should not need to open these unless you get an error or need additional clarification.

## macOS
1. If the Xcode Command Line Tools are not installed, open the Terminal and run:

    ```bash
    xcode-select --install
    ```

2. If [Homebrew](https://brew.sh) is not installed, install it. Then install the remaining packages:

    ```bash
    brew install arm-none-eabi-binutils xz pkg-config libpng utf8proc
    ```

Then proceed to [Installation](#installation).

<details>
    <summary><i>Note for Apple Silicon...</i></summary>

>   Everything builds natively; Rosetta is not needed.
</details>

## Linux
Open a terminal and enter the following commands, depending on which distro you're using.

### Debian/Ubuntu-based distributions
Run the following command to install the necessary packages:

```bash
sudo apt install build-essential git curl patch xz-utils binutils-arm-none-eabi pkg-config libpng-dev libutf8proc-dev
```

Then proceed to [Installation](#installation).

### Other distributions
Install the equivalents of: a C compiler, make, git, curl, patch, xz, sha1sum, the ARM binutils (`arm-none-eabi-as`, `ld`, `ar`, `objcopy`), pkg-config, libpng and utf8proc, with their development headers. Then proceed to [Installation](#installation).

## Windows
Windows is not currently tested. WSL with Ubuntu may work by following the [Linux](#debianubuntu-based-distributions) instructions inside WSL, but it is not validated.

## Installation

1. If Alchemy is not already downloaded, run:

    ```bash
    git clone https://github.com/PascalPixel/alchemy
    cd alchemy
    ```

    <details>
        <summary><i>Note...</i></summary>

    >   Use a folder path without spaces.
    </details>

2. Fetch the compilers' source and build the tools:

    ```bash
    git submodule update --init --recursive agscc
    make bootstrap
    ```

    <details>
        <summary><i>Note...</i></summary>

    >   `make bootstrap` builds the C tools in `tools/`, the pinned compilers from the `agscc` submodule, their assembler and their runtime library, all under `tools/out/`; nothing is installed system-wide.
    >   The first run downloads GNU binutils 2.10 and GCC 2.95.2 from ftp.gnu.org and checks them against pinned SHA-256 hashes. After that, building needs no network connection.
    >   The old compiler sources print many warnings on current systems; only the final `make` error matters when something fails.
    </details>

Now you're ready to [build **The Broken Seal**](#build-the-broken-seal).

## Build The Broken Seal
To build **tbs-en.gba** (Note: to speed up builds, see [Parallel builds](#parallel-builds)):

```bash
make tbs-en
```

If it has built successfully you will have the output file **out/tbs-en/tbs-en.gba**. The other editions are `tbs-ja`, `tbs-de`, `tbs-es`, `tbs-fr` and `tbs-it`.

## Build The Lost Age
Put your own dumps in `roms/`, named `tla-ja.gba`, `tla-en.gba`, `tla-de.gba`, `tla-es.gba`, `tla-fr.gba` and `tla-it.gba`. You only need the dump of the edition you build. Then, for example:

```bash
make tla-en
```

The parts of The Lost Age not yet rebuilt are copied from your dump. Each dump is checked against `rom.sha1` before use. `roms/` and built ROMs are ignored by Git and must never be committed.

# Building guidance

## Parallel builds

To speed up building, first get the number of processor cores:

```bash
nproc
```

`nproc` is not available on macOS. The alternative is `sysctl -n hw.ncpu`. Builds can then be sped up by running:

```bash
make -j<number of cores> tbs-en
```

## Compare ROM to the original

For contributing, or if you'd simply like to verify that your ROM is identical to the original game, run:

```bash
make compare-tbs-en
```

If it matches, you will see the following at the end of the output:

```bash
out/tbs-en/tbs-en.gba: OK
```

To compare every edition of a game, or of both games, run `make compare-tbs`, `make compare-tla` or `make compare-editions`.

## Modern compiler

The source also compiles with a current ARM GCC. Install `arm-none-eabi-gcc` (Homebrew) or `gcc-arm-none-eabi` (Debian/Ubuntu), then run:

```bash
make modern
```

To choose a compiler explicitly:

```bash
make modern MODERN_CC=/path/to/arm-none-eabi-gcc
```

This checks that The Broken Seal's C compiles without errors; it does not link a ROM. GCC 16.2.0 is tested.

## Editors

[clangd](https://clangd.llvm.org/installation.html) reads the checked-in `.clangd` and `compile_flags.txt` for completion and diagnostics; build an edition first so its generated headers exist. Format C with `clang-format`, which reads `.clang-format`.

## Troubleshooting

| Symptom | Check |
| --- | --- |
| `png.h`, `utf8proc.h` or a pkg-config package is missing | Install libpng, utf8proc and pkg-config, including development packages on Linux. |
| `arm-none-eabi-as`, `ld` or `ar` is missing | Install the ARM binutils and make sure they are on `PATH`. |
| `agscc` is empty | Run `git submodule update --init --recursive agscc`, then `make bootstrap`. |
| A download fails during bootstrap | Check access to GitHub and ftp.gnu.org, then rerun `make bootstrap`. |
| A Lost Age dump is missing or its hash fails | Check its edition, filename and SHA-1 against `rom.sha1`. |
| `compare` fails after you changed something | Expected: a changed game has a different hash. Build with `make tbs-en` instead. |

`make clean` removes `out/` and generated assets, keeping the bootstrapped tools.

# Useful additional tools

* [Psynergy](https://github.com/PascalPixel/psynergy), in the optional `psynergy` submodule, for matching functions and measuring progress (needs Rust; see [AGENTS.md](AGENTS.md#commands))
* [mGBA](https://mgba.io) for playing and debugging the built ROMs
