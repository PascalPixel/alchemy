# 🌋 Alchemy

Alchemy is a reconstruction of the source for the Game Boy Advance games ☀️ **[Golden Sun: The Broken Seal](https://alchemy.pascalpixel.com/tbs)** and ⚓️ **Golden Sun: The Lost Age**. It is a [matching decompilation](games/tbs/glossary.md#matching-decompilation): readable code, images, music and text that build the same ROMs as the originals, byte for byte.

## Documentation

<a href="https://alchemy.pascalpixel.com"><img src="https://alchemy.pascalpixel.com/guides/docs/battle-effects.png" width="838" alt="Alchemy's documentation on battle effects: Isaac summons Judgment above a diagram of the three ways a battle plays an effect, beside the guide's sidebar" /></a>

[alchemy.pascalpixel.com](https://alchemy.pascalpixel.com) explains how The Broken Seal works, written from its rebuilt source, with recordings from the game itself.

## Source

Each game has its own folder of code, pictures, music and text: [games/tbs](games/tbs) for The Broken Seal and [games/tla](games/tla) for The Lost Age. The Broken Seal is fully rebuilt from this repository, in Japanese, English, German, French, Spanish, and Italian. The Lost Age is in progress, and the parts not yet rewritten still come from your own cartridge dump.

<img src="https://alchemy.pascalpixel.com/PROGRESS_CHART.png" width="838" alt="How much of each game's code is rebuilt, day by day since 16 July 2026" />

## Build

[INSTALL.md](INSTALL.md) has the commands for Linux, macOS and Windows. Every build is checked against the original cartridges’ hashes in [rom.sha1](rom.sha1).

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were made by Camelot Software Planning and published by Nintendo. Alchemy is an unofficial fan project, and neither company is involved with it or endorses it.

Thank you to:

- Everyone on [r/GoldenSun](https://www.reddit.com/r/GoldenSun/), for cheering Alchemy on and keeping the love for these games alive.
- Tarpman and Karathan, who worked out the compiler and flags Camelot used.
- Coaltergeist, for [camelot-gcc](https://github.com/Coaltergeist/camelot-gcc), the compiler Alchemy first built with.
- [pret](https://github.com/pret), whose decompilations set the standard Alchemy measures itself against, and whose [agbcc](https://github.com/pret/agbcc) builds the games’ sound and save code.
- [Simon Lindholm](https://github.com/simonlindholm/decomp-permuter), for decomp-permuter, which Alchemy Devtools reimplements in Rust.
- [mGBA](https://mgba.io), whose libmgba plays the builds for every picture and recording in the documentation.
