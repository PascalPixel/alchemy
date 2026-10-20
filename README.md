# 🌋 Alchemy

Alchemy is a decompilation of the Game Boy Advance games ☀️ **Golden Sun: The Broken Seal** and ⚓️ **Golden Sun: The Lost Age**.

## What that means

Camelot wrote Golden Sun as source code and compiled it into the `0`s and `1`s on a game cartridge. That source was never released, so Alchemy writes it again: C code that compiles back into exactly the same `0`s and `1`s.

It’s like working out a cake’s exact recipe by baking new cakes until one tastes identical. It’s mad, but it works!

With the source you can read exactly how Golden Sun works, from the damage formulas to every cutscene, and change it: rebalance it, translate it, add scenes, or build editors and ports on top of it.

## What you get

Alchemy builds the games Camelot shipped, byte for byte, in all six languages. Every picture, song and line of dialogue is Camelot’s own, taken from the cartridge into files you can open and edit. There’s nothing to download and play: the repository holds the source, and the build turns it back into the original games. Remakes, mods and ports can grow from it in projects of their own.

## How it’s made

Alchemy is a hobby project I started in July 2026 to enjoy making things again after a burnout. I build it with AI coding assistants. They propose C code, the build compares it with the cartridge, and only what matches stays. I direct the work: what to tackle next, how the code is organized and named, and what counts as finished. When something is broken or stuck, that’s where I dig in.

## Where it stands

- **The Broken Seal** builds entirely from this repository, in all six languages. No ROM needed.
- **The Lost Age** builds too and is well on its way. The parts not yet rewritten still come from your own cartridge dump.

<img src="https://pascalpixel.github.io/alchemy-docs/PROGRESS_CHART.png" width="838" alt="How much of each game's code and data is rebuilt, day by day since 16 July 2026" />
<img src="https://pascalpixel.github.io/alchemy-docs/PROGRESS.png" width="838" alt="A map of the project's files by size" />

## Try it

[Read the documentation](https://pascalpixel.github.io/alchemy-docs/) to browse the games' code and folder guides.

[INSTALL.md](INSTALL.md) has the few commands to build it on Linux, macOS or Windows. Anyone with their own cartridges can check the result against these hashes:

| Language | The Broken Seal                            | The Lost Age                               |
| -------- | ------------------------------------------ | ------------------------------------------ |
| Japanese | `35ef9e4c9f38183ebd6a3e3923a11ce9a4333718` | `4c79e60962299654981628ccd99b122ef27043f7` |
| English  | `5c4695205413df7db52b9a184815a07783999971` | `b500663220cb9bf56b9f8e8c0c544f1d6fa3a824` |
| German   | `c23ab91a39434623cf1a999f387d743ade1a22a6` | `7820b4c7ba9002b0e9be23a6f45a9ab204986438` |
| Spanish  | `de0bdc889594b012cad213ca83707feb81ada28b` | `a2e3e80036195343c2713410ac14f4e329e3f40d` |
| French   | `42f3b262c16cfc5bde1fa2b25016fb74046de1b3` | `f14855c0f8c87a95cc52189e8e3b2bd9df186322` |
| Italian  | `ca45e04b26b2f040ac63e679a459d21869f5f28d` | `57f1d005666b81645e55412658427e27099c5907` |

To find your way around the code, start with the battle effects: [their guide](games/tbs/src/effect/README.md) shows how to find the code behind any move and change it.

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were created by Camelot Software Planning and published by Nintendo. Alchemy is an unofficial fan project and is not affiliated with or endorsed by either company.

Thank you to:

- The [r/GoldenSun community](https://www.reddit.com/r/GoldenSun/), for sharing Alchemy, cheering it on, and keeping the love for these games alive.
- Tarpman and Karathan, for identifying the compiler and flags Camelot used, and Coaltergeist, for [camelot-gcc](https://github.com/Coaltergeist/camelot-gcc), the compiler Alchemy first built with, since replaced by alchemy-gcc.
- [pret](https://github.com/pret), whose decompilations set the standard Alchemy measures itself against, and whose [agbcc](https://github.com/pret/agbcc) Alchemy builds the games’ library code with.
- [Simon Lindholm](https://github.com/simonlindholm/decomp-permuter), for the permuter whose behavior [Alchemy Devtools](https://github.com/PascalPixel/alchemy-devtools) reimplements in Rust.
