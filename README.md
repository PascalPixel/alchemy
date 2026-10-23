# 🌋 Alchemy

Alchemy is a decompilation of the Game Boy Advance games ☀️ **[Golden Sun: The Broken Seal](games/tbs/README.md)** and ⚓️ **Golden Sun: The Lost Age**.

## Decompilation

[Camelot](games/tbs/glossary.md#camelot) wrote Golden Sun as source code and compiled it into the `0`s and `1`s on the cartridge. That source was never released, so Alchemy writes it again: C code that compiles back into exactly the same `0`s and `1`s.

## The source

With the source, anyone can read how Golden Sun works and change it: rebalance it, translate it, or build editors and ports on top of it. Every picture, song and line of dialogue is a file you can open and edit, in all six languages Camelot released: PNG, MIDI, WAV and translation catalogues.

## Progress

The Broken Seal is fully rebuilt from this repository, in all six languages, and I’m now making its code easy to read. The Lost Age builds too, but the parts not yet rewritten still come from your own cartridge dump.

<img src="https://alchemy.pascalpixel.com/PROGRESS_CHART.png" width="838" alt="How much of each game's code and data is rebuilt, day by day since 16 July 2026" />

## Building

[INSTALL.md](INSTALL.md) has the commands for Linux, macOS and Windows. Every build is checked against the original cartridges’ hashes in [rom.sha1](rom.sha1).

## How it’s made

I’m Pascal Pixel, and I started Alchemy in July 2026 as a hobby, to enjoy making things again after a burnout. AI coding agents write the code, and the build keeps only what matches the cartridge. I oversee them: I decide what to work on, how the code is organized and named, and what counts as finished, and I get them unstuck. It’s like managing a team of engineers, just without the meetings 😝 Their rules are in [AGENTS.md](AGENTS.md).

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were made by Camelot Software Planning and published by Nintendo. Alchemy is an unofficial fan project, and neither company is involved with it or endorses it.

Thank you to:

- Everyone on [r/GoldenSun](https://www.reddit.com/r/GoldenSun/), for cheering Alchemy on and keeping the love for these games alive.
- Tarpman and Karathan, who worked out the compiler and flags Camelot used.
- Coaltergeist, for [camelot-gcc](https://github.com/Coaltergeist/camelot-gcc), the compiler Alchemy first built with.
- [pret](https://github.com/pret), whose decompilations set the standard Alchemy measures itself against, and whose [agbcc](https://github.com/pret/agbcc) builds the games’ sound and save code.
- [Simon Lindholm](https://github.com/simonlindholm/decomp-permuter), for decomp-permuter, which Alchemy Devtools reimplements in Rust.
- [mGBA](https://mgba.io), whose libmgba plays the builds for every picture and recording in the documentation.
