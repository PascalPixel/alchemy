# 🌋 The Alchemy Project

The Alchemy project reconstructs the source code of both Golden Sun games on Game Boy Advance, ☀️ [The Broken Seal](https://alchemy.pascalpixel.com/tbs) and ⚓️ [The Lost Age](https://alchemy.pascalpixel.com/tla), for video game preservation. It is a 100% [matching decompilation](https://alchemy.pascalpixel.com/tbs/glossary#matching-decompilation): readable code, images, music and text that build the same ROMs as the originals, byte for byte.

<a href="https://alchemy.pascalpixel.com"><img src="https://alchemy.pascalpixel.com/docs.png" align="right" width="400" alt="Alchemy's documentation on the interface: Isaac talks to Garet's sister in Vale, beside the guide's sidebar" /></a>

[alchemy.pascalpixel.com](https://alchemy.pascalpixel.com) explains how both games work, written from this source, with recordings from the games themselves.

<br clear="right" />

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

Issues and pull requests are welcome. If something is wrong, badly named or hard to find, [open an issue](https://github.com/PascalPixel/alchemy/issues); if you know the fix, send it.

A pull request keeps every ROM identical to the original: `make` prints OK for all twelve, and `make modern` and `make test` pass. Work from your own ROMs, this repository and public documentation, and from nothing else: no other project’s Golden Sun work and no leaked code. Please don’t attach ROMs, original or modified.

Mods and new features live in your own repository, built on Alchemy. This one stays the original games, so that everything made from it starts from the same known place.

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were made by Camelot Software Planning and published by Nintendo. Alchemy is an unofficial fan project, and neither company is involved with it or endorses it.

Thank you to:

- [pret](https://github.com/pret), whose decompilations set the standard, and whose [agbcc](https://github.com/pret/agbcc) builds the games’ sound and save code.
- [GBATEK](https://problemkaputt.de/gbatek.htm), nocash’s hardware reference, for the hardware names.
- [Coaltergeist](https://github.com/Coaltergeist), [Karathan](https://github.com/SBird1337) and [Tarpman](https://github.com/tarpman), whose earlier public work identified the compiler and flags behind The Broken Seal.
- [SGL](https://www.infochunk.com/saturn/segahtml_en/sgl/sglr/), Sega’s Saturn library, for the fixed-point, angle and matrix names.
- Telago, whose GameFAQs walkthroughs of [The Broken Seal](https://gamefaqs.gamespot.com/gba/468548-golden-sun/faqs/39216) and [The Lost Age](https://gamefaqs.gamespot.com/gba/561356-golden-sun-the-lost-age/faqs/44287) gave many places their names.
- [simonlindholm](https://github.com/simonlindholm), for [decomp-permuter](https://github.com/simonlindholm/decomp-permuter), which [Alchemy Devtools](https://github.com/PascalPixel/alchemy-devtools) reimplements in Rust.
- Everyone at [r/GoldenSun](https://www.reddit.com/r/GoldenSun/) who cheered Alchemy on and builds with it.
