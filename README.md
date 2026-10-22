# 🌋 Alchemy

Alchemy is a decompilation of the Game Boy Advance games ☀️ **[Golden Sun: The Broken Seal](games/tbs/README.md)** and ⚓️ **Golden Sun: The Lost Age**.

<img src="https://alchemy.pascalpixel.com/guides/field/layers-3d.gif" width="520" alt="Vale pulled apart into its layers: the ground at the back, then the middle layer, the actors and the overhead layer in front" />

## What is a decompilation?

Camelot wrote Golden Sun as source code, then compiled it into the `0`s and `1`s on the cartridge. They never released that source, so Alchemy writes it again: C code that compiles back into exactly the same `0`s and `1`s.

It’s like working out a cake’s exact recipe by baking new cakes until one tastes identical. It’s mad, but it works!

With the source, anyone can read how Golden Sun really works, from the damage formulas to every cutscene, and change it: rebalance it, translate it, add scenes, or build editors and ports on top of it.

## What’s in it?

The source, not a game to download. Building it gives you back the games exactly as Camelot shipped them, in Japanese, English, German, Spanish, French and Italian. Every picture, song and line of dialogue is Camelot’s own, taken from the cartridge into files you can open and edit: PNGs, MIDI, WAV, and the text as translation catalogues. Remakes, mods and ports can grow from it in projects of their own.

## How far along is it?

The Broken Seal is fully rebuilt: every byte of all six editions comes from this repository, with no cartridge needed to build it. Now I’m making that code a pleasure to read, one place and one system at a time: real names from the game’s own text, comments that say why, and a guide for every folder. The Lost Age builds too and is well on its way, but the parts that aren’t rewritten yet still come from your own cartridge dump.

<img src="https://alchemy.pascalpixel.com/PROGRESS_CHART.png" width="838" alt="How much of each game's code and data is rebuilt, day by day since 16 July 2026" />
<img src="https://alchemy.pascalpixel.com/PROGRESS.png" width="838" alt="A map of the project's files by size" />

## Where do I start reading?

[The documentation](https://alchemy.pascalpixel.com/) explains how The Broken Seal works underneath, and it takes the game apart to show you. [Its front page](games/tbs/README.md) covers the big picture: the loop that runs one scene after another, the places and their flags. Four pages go a layer deeper: [the world](games/tbs/world.md) you walk around, [battles](games/tbs/battle.md), [menus, windows and shops](games/tbs/menus.md) and [the engine](games/tbs/engine.md) underneath them all. The places have pages of their own too, with maps of their rooms and recordings of their events.

If you’d rather start in the code, [the source’s own guide](games/tbs/src/README.md) says what lives in which folder, and every folder has a guide of its own. [MATCHING.md](games/tbs/src/MATCHING.md) collects the plain C spellings that compile to the game’s exact instructions, for anyone who wants to help with The Lost Age.

## Can I build it myself?

Yes! [INSTALL.md](INSTALL.md) has the few commands it takes on Linux, macOS or Windows. Every build is checked against the hashes of the original cartridges, so anyone with their own copy can see it’s the same game:

| Language | The Broken Seal                            | The Lost Age                               |
| -------- | ------------------------------------------ | ------------------------------------------ |
| Japanese | `35ef9e4c9f38183ebd6a3e3923a11ce9a4333718` | `4c79e60962299654981628ccd99b122ef27043f7` |
| English  | `5c4695205413df7db52b9a184815a07783999971` | `b500663220cb9bf56b9f8e8c0c544f1d6fa3a824` |
| German   | `c23ab91a39434623cf1a999f387d743ade1a22a6` | `7820b4c7ba9002b0e9be23a6f45a9ab204986438` |
| Spanish  | `de0bdc889594b012cad213ca83707feb81ada28b` | `a2e3e80036195343c2713410ac14f4e329e3f40d` |
| French   | `42f3b262c16cfc5bde1fa2b25016fb74046de1b3` | `f14855c0f8c87a95cc52189e8e3b2bd9df186322` |
| Italian  | `ca45e04b26b2f040ac63e679a459d21869f5f28d` | `57f1d005666b81645e55412658427e27099c5907` |

## Who’s making it?

Me! I’m Pascal Pixel, and I started Alchemy in July 2026 as a hobby, to enjoy making things again after a burnout. I build it with AI coding assistants. They propose C code, the build compares it with the cartridge, and only what matches stays. I decide what to tackle next, how the code is organized and named, and what counts as finished, and when something’s broken or stuck, that’s where I dig in. The rules they work by are all in [AGENTS.md](AGENTS.md), from what may count as matched to how a room gets its name, so you can see exactly what “done” means here.

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were made by Camelot Software Planning and published by Nintendo. Alchemy is an unofficial fan project, and neither company is involved with it or endorses it.

Thank you to:

- Everyone on [r/GoldenSun](https://www.reddit.com/r/GoldenSun/), for cheering Alchemy on and keeping the love for these games alive.
- Tarpman and Karathan, who worked out the compiler and flags Camelot used.
- Coaltergeist, for [camelot-gcc](https://github.com/Coaltergeist/camelot-gcc), the compiler Alchemy first built with before alchemy-gcc replaced it.
- [pret](https://github.com/pret), whose decompilations set the standard Alchemy measures itself against, and whose [agbcc](https://github.com/pret/agbcc) builds the games’ sound and save code.
- [Simon Lindholm](https://github.com/simonlindholm/decomp-permuter), for the permuter that [Alchemy Devtools](https://github.com/PascalPixel/alchemy-devtools) reimplements in Rust.
- [mGBA](https://mgba.io), whose libmgba plays the builds for every picture and recording in the documentation.
