# 🌋 Alchemy

Alchemy is a decompilation of the Game Boy Advance games ☀️ **Golden Sun: The Broken Seal** and ⚓️ **Golden Sun: The Lost Age**.

## What that means

Camelot wrote Golden Sun as source code and compiled it into `0`'s and `1`'s on a game cartridge. That source was never released, and Alchemy tries to recover that source code by guessing what it was based on the `0`'s and `1`'s on the game cartridge. 

Like guessing the exact ingredients in a cake by making new cakes and seeing if it tastes the same. It's mad. And it's working.

With the source you can read exactly how Golden Sun works, from the damage formulas to every cutscene, and change it: rebalance it, translate it, add scenes, or build editors and ports on top of it.

## What it isn't

- **Not a remake, a mod or a PC port.** It builds exactly the games Camelot shipped, unchanged, and that is all it will ever be. Mods and ports belong in projects of their own, built from it.
- **Not new content.** Every picture, song and line of dialogue is Camelot's, taken from the cartridge into editable files. No art, music or writing in it is AI-generated.
- **Not a download.** There's no installer, `.exe` or ROM image. The repository holds the source code, music, and images, and the build recreates the original ROMs exactly.

## How it's made

Alchemy is a hobby project I started in July 2026 to enjoy making things again after a burnout. I build it with AI coding assistants: they propose C code, the build compares it with the cartridge, and keep what matches. I direct the work, decide how the code is organized and named, and fix what is broken, or stuck.

## Where it stands

- **The Broken Seal** builds entirely from this repository, in all six languages. No ROM is needed.
- **The Lost Age** is in progress. It builds too, but the parts not yet rewritten still come from your own cartridge dump.

<img src="https://raw.githubusercontent.com/PascalPixel/alchemy/progress/PROGRESS_CHART.png" width="838" alt="How much of each game's code and data is rebuilt, day by day since 16 July 2026" />
<img src="https://raw.githubusercontent.com/PascalPixel/alchemy/progress/PROGRESS.png" width="838" alt="A map of the project's files by size" />

## Try it

[INSTALL.md](INSTALL.md) has the few commands to build it on Linux, macOS or Windows. It builds these ROMs, so anyone with their own cartridge dumps can check the result:

| Language | The Broken Seal                            | The Lost Age                               |
| -------- | ------------------------------------------ | ------------------------------------------ |
| Japanese | `35ef9e4c9f38183ebd6a3e3923a11ce9a4333718` | `4c79e60962299654981628ccd99b122ef27043f7` |
| English  | `5c4695205413df7db52b9a184815a07783999971` | `b500663220cb9bf56b9f8e8c0c544f1d6fa3a824` |
| German   | `c23ab91a39434623cf1a999f387d743ade1a22a6` | `7820b4c7ba9002b0e9be23a6f45a9ab204986438` |
| Spanish  | `de0bdc889594b012cad213ca83707feb81ada28b` | `a2e3e80036195343c2713410ac14f4e329e3f40d` |
| French   | `42f3b262c16cfc5bde1fa2b25016fb74046de1b3` | `f14855c0f8c87a95cc52189e8e3b2bd9df186322` |
| Italian  | `ca45e04b26b2f040ac63e679a459d21869f5f28d` | `57f1d005666b81645e55412658427e27099c5907` |

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were created by Camelot Software Planning and published by Nintendo. Alchemy is an unofficial fan project and is not affiliated with or endorsed by either company.

Thank you to:

- The [r/GoldenSun community](https://www.reddit.com/r/GoldenSun/), for sharing Alchemy, cheering it on, and keeping the love for these games alive.
- Tarpman and Karathan, for identifying the compiler and flags Camelot used, and Coaltergeist, for [camelot-gcc](https://github.com/Coaltergeist/camelot-gcc), the compiler Alchemy first built with, since replaced by agscc.
- [pret](https://github.com/pret), whose decompilations set the standard Alchemy measures itself against, and whose [agbcc](https://github.com/pret/agbcc) Alchemy builds the games’ library code with.
- [Simon Lindholm](https://github.com/simonlindholm/decomp-permuter), for the permuter whose behavior [Psynergy](https://github.com/PascalPixel/psynergy) reimplements in Rust.
