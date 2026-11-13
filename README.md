# 🌋 Alchemy

Alchemy is a reconstruction of the source for the Game Boy Advance games ☀️ **[Golden Sun: The Broken Seal](https://alchemy.pascalpixel.com/tbs)** and ⚓️ **Golden Sun: The Lost Age**. It is a [matching decompilation](games/tbs/glossary.md#matching-decompilation): readable code, images, music and text that build the same ROMs as the originals, byte for byte.

## Docs

<a href="https://alchemy.pascalpixel.com"><img src="https://alchemy.pascalpixel.com/guides/docs/interface.png" width="838" alt="Alchemy's documentation on the interface: Isaac talks to Garet's sister in Vale above a diagram of what each button opens, beside the guide's sidebar" /></a>

[alchemy.pascalpixel.com](https://alchemy.pascalpixel.com) explains how The Broken Seal works, written from its rebuilt source, with recordings from the game itself. To find the code behind something you saw in the game, start at [the source's own page](games/tbs/src/README.md).

## Build

Install the dependencies.

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

Then clone and build.

```bash
git clone --recurse-submodules https://github.com/PascalPixel/alchemy
cd alchemy
make
```

The first `make` builds the old compilers from the `alchemy-gcc` submodule, once, then the English Broken Seal as `out/tbs-en/tbs-en.gba`.

| Command | Builds |
|---|---|
| `make tbs-en` | one edition: `tbs` or `tla`, then `ja`, `en`, `de`, `es`, `fr` or `it` |
| `make compare-tbs-en` | that edition, checked against the original's hash in `rom.sha1` |
| `make -j8 compare-tbs` | all six editions of The Broken Seal (`compare-tla` for The Lost Age) |
| `make -j8 compare-editions` | all twelve |

`make help` lists the rest.

It builds the following ROMs:

| Language | The Broken Seal                            | The Lost Age                               |
| -------- | ------------------------------------------ | ------------------------------------------ |
| Japanese | `35ef9e4c9f38183ebd6a3e3923a11ce9a4333718` | `4c79e60962299654981628ccd99b122ef27043f7` |
| English  | `5c4695205413df7db52b9a184815a07783999971` | `b500663220cb9bf56b9f8e8c0c544f1d6fa3a824` |
| German   | `c23ab91a39434623cf1a999f387d743ade1a22a6` | `7820b4c7ba9002b0e9be23a6f45a9ab204986438` |
| Spanish  | `de0bdc889594b012cad213ca83707feb81ada28b` | `a2e3e80036195343c2713410ac14f4e329e3f40d` |
| French   | `42f3b262c16cfc5bde1fa2b25016fb74046de1b3` | `f14855c0f8c87a95cc52189e8e3b2bd9df186322` |
| Italian  | `ca45e04b26b2f040ac63e679a459d21869f5f28d` | `57f1d005666b81645e55412658427e27099c5907` |

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were made by Camelot Software Planning and published by Nintendo. Alchemy is an unofficial fan project, and neither company is involved with it or endorses it.

Thank you to:

- [pret](https://github.com/pret), whose decompilations set the standard Alchemy measures itself against, and whose [agbcc](https://github.com/pret/agbcc) builds the games’ sound and save code.
- [GBATEK](https://problemkaputt.de/gbatek.htm), nocash’s hardware reference, from which The Lost Age’s `-mtune=gba` tune and Alchemy’s hardware names come.
- [Coaltergeist](https://github.com/Coaltergeist), [Karathan](https://github.com/SBird1337) and [Tarpman](https://github.com/tarpman), whose earlier public work identified the compiler and flags behind The Broken Seal.
- Telago, whose GameFAQs walkthroughs of [The Broken Seal](https://gamefaqs.gamespot.com/gba/468548-golden-sun/faqs/39216) and [The Lost Age](https://gamefaqs.gamespot.com/gba/561356-golden-sun-the-lost-age/faqs/44287) say what the player sees in every place, which is where many of Alchemy’s names come from.
- [simonlindholm](https://github.com/simonlindholm), for [decomp-permuter](https://github.com/simonlindholm/decomp-permuter), which Alchemy Devtools reimplements in Rust.
- Everyone at [r/GoldenSun](https://www.reddit.com/r/GoldenSun/) who cheered Alchemy on and builds with it.