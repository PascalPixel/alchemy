# 🌋 The Alchemy Project

The Alchemy project reconstructs the source code of both Golden Sun games on Game Boy Advance, ☀️ The Broken Seal and ⚓️ The Lost Age, for video game preservation. It is a 100% matching decompilation: readable code, images, music and text that build the same ROMs as the originals, byte for byte.

## Documentation

[![Alchemy's documentation on the interface: Isaac talks to Garet's sister in Vale, beside the guide's sidebar](https://alchemy.pascalpixel.com/docs.png)](https://alchemy.pascalpixel.com)

The documentation is hosted at [alchemy.pascalpixel.com](https://alchemy.pascalpixel.com), where you can search it. It explains how both games work, written from this source, with recordings from the games themselves.

You can also browse it in this repository: start at [`games/README.md`](games/README.md). Each folder has a `README.md` for what’s in it, and the longer pages are in [`docs/`](docs).

## Build

**macOS**, with [Homebrew](https://brew.sh):

```bash
xcode-select --install
brew install arm-none-eabi-binutils libpng
```

**Linux** (Debian or Ubuntu; the same packages under their local names elsewhere):

```bash
sudo apt install build-essential binutils-arm-none-eabi git libpng-dev
```

**Windows:** run `wsl --install` in PowerShell, restart, open **Ubuntu** and follow the Linux step inside it.

Then clone and build.

```bash
git clone --recurse-submodules https://github.com/PascalPixel/alchemy
cd alchemy
make
```

`make` builds all twelve editions and checks each against the original's hash in `rom.sha1`. The first run also builds the old compilers, once, so it takes a while. `make tbs-en` builds one edition: `tbs` or `tla`, then `ja`, `en`, `de`, `es`, `fr` or `it`. `make help` lists the rest.

## Contributing

Issues and pull requests are welcome. [CONTRIBUTING.md](CONTRIBUTING.md) lists what you can help with and how to check that your change still builds every ROM identical to the original.

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were made by Camelot Software Planning and published by Nintendo. Alchemy is an unofficial fan project, and neither company is involved with it or endorses it.

Thank you to:

- [pret](https://github.com/pret), whose decompilations set the standard, and whose [agbcc](https://github.com/pret/agbcc) builds the games’ sound and save code.
- [Coaltergeist](https://github.com/Coaltergeist), [Karathan](https://github.com/SBird1337) and [Tarpman](https://github.com/tarpman), whose earlier public work identified the compiler and flags behind The Broken Seal.
- [simonlindholm](https://github.com/simonlindholm), for [decomp-permuter](https://github.com/simonlindholm/decomp-permuter), the permuter we used to find matches.
- [GBATEK](https://problemkaputt.de/gbatek.htm), nocash’s hardware reference, for the hardware names.
- [SGL](https://www.infochunk.com/saturn/segahtml_en/sgl/sglr/), Sega’s Saturn library, for the fixed-point, angle and matrix names.
- Telago, whose GameFAQs walkthroughs of [The Broken Seal](https://gamefaqs.gamespot.com/gba/468548-golden-sun/faqs/39216) and [The Lost Age](https://gamefaqs.gamespot.com/gba/561356-golden-sun-the-lost-age/faqs/44287) gave many places their names.
- Everyone at [r/GoldenSun](https://www.reddit.com/r/GoldenSun/) who cheered Alchemy on and builds with it.
