# Building Alchemy

## Dependencies

**macOS**, with [Homebrew](https://brew.sh):

```bash
xcode-select --install
brew install arm-none-eabi-binutils libpng
```

**Linux** (Debian or Ubuntu; the same packages under their local names elsewhere):

```bash
sudo apt install build-essential binutils-arm-none-eabi git libpng-dev
```

**Windows:** run `wsl --install` in PowerShell, restart, open **Ubuntu** from the Start menu and follow the Linux steps inside it.

## Build

```bash
git clone --recurse-submodules https://github.com/PascalPixel/alchemy
cd alchemy
make
```

The first `make` builds the old compilers from the `alchemy-gcc` submodule, once, then the English Broken Seal as `out/tbs-en/tbs-en.gba`. No ROM is needed.

| Command | Builds |
|---|---|
| `make tbs-en` | one edition: `tbs` or `tla`, then `ja`, `en`, `de`, `es`, `fr` or `it` |
| `make compare-tbs-en` | that edition, checked against the original's hash in `rom.sha1` |
| `make -j8 compare-tbs` | all six editions of The Broken Seal (`compare-tla` for The Lost Age) |
| `make -j8 compare-editions` | all twelve |

`make help` lists the rest.
