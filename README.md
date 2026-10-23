# 🌋 Alchemy

Alchemy is a decompilation of the Game Boy Advance games ☀️ **[Golden Sun: The Broken Seal](games/tbs/README.md)** and ⚓️ **Golden Sun: The Lost Age**.

## What is a decompilation?

[Camelot](games/tbs/glossary.md#camelot) wrote Golden Sun as source code, then compiled it into the `0`s and `1`s on the cartridge. They never released that source, so Alchemy writes it again: C code that compiles back into exactly the same `0`s and `1`s.

With the source, anyone can read how Golden Sun works, from the damage formulas to every cutscene, and change it: rebalance it, translate it, add [maps](games/tbs/glossary.md#map), or build editors and ports on top of it.

## What’s in it?

Building it gives you back the games exactly as Camelot shipped them, in Japanese, English, German, Spanish, French and Italian. Every picture, song and line of dialogue is in a file you can open and edit: PNGs, MIDI, WAV, and the text as translation catalogues.

## How far along is it?

The Broken Seal is fully rebuilt: all six language [editions](games/tbs/glossary.md#editions) build from this repository alone, with no cartridge needed. Now I’m making that code readable, one place and one system at a time: names from the game’s own text, comments that say why, and a guide for every folder. The Lost Age builds too, but the parts not yet rewritten still come from your own cartridge dump.

<img src="https://alchemy.pascalpixel.com/PROGRESS_CHART.png" width="838" alt="How much of each game's code and data is rebuilt, day by day since 16 July 2026" />

## Where do I start reading?

[The documentation](https://alchemy.pascalpixel.com/) explains how The Broken Seal works. [Its front page](games/tbs/README.md) covers the big picture, and four pages go a layer deeper: [the world](games/tbs/world.md), [battles](games/tbs/battle.md), [menus, windows and shops](games/tbs/menus.md) and [the engine](games/tbs/engine.md). Every place has a page with maps of its rooms and recordings of its events, and [the glossary](games/tbs/glossary.md) explains the words the pages share.

To start in the code, [the source’s own guide](games/tbs/src/README.md) says what lives in which folder.

## Can I build it myself?

[INSTALL.md](INSTALL.md) has the commands for Linux, macOS and Windows. Every build is checked against the original cartridges’ hashes in [rom.sha1](rom.sha1).

## Who’s making it?

I’m Pascal Pixel. I started Alchemy in July 2026 as a hobby, to enjoy making things again after a burnout. I build it with AI coding assistants: they propose C code, the build compares it with the cartridge, and only what matches stays. I decide what to tackle next, how the code is organized and named, and what counts as finished, and when something’s broken or stuck, that’s where I dig in. The rules they work by are in [AGENTS.md](AGENTS.md).

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were made by Camelot Software Planning and published by Nintendo. Alchemy is an unofficial fan project, and neither company is involved with it or endorses it.

Thank you to:

- Everyone on [r/GoldenSun](https://www.reddit.com/r/GoldenSun/), for cheering Alchemy on and keeping the love for these games alive.
- Tarpman and Karathan, who worked out the compiler and flags Camelot used.
- Coaltergeist, for [camelot-gcc](https://github.com/Coaltergeist/camelot-gcc), the compiler Alchemy first built with before alchemy-gcc replaced it.
- [pret](https://github.com/pret), whose decompilations set the standard Alchemy measures itself against, and whose [agbcc](https://github.com/pret/agbcc) builds the games’ sound and save code.
- [Simon Lindholm](https://github.com/simonlindholm/decomp-permuter), for decomp-permuter, which Alchemy Devtools reimplements in Rust.
- [mGBA](https://mgba.io), whose libmgba plays the builds for every picture and recording in the documentation.
