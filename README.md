# 🌋 Alchemy

This is a decompilation of Golden Sun: The Broken Seal and Golden Sun: The Lost Age.

The Broken Seal builds entirely from source. The Lost Age is still in progress and requires your own cartridge dumps.

It builds the following ROMs:

| Language | The Broken Seal | The Lost Age |
| --- | --- | --- |
| Japanese | `35ef9e4c9f38183ebd6a3e3923a11ce9a4333718` | `4c79e60962299654981628ccd99b122ef27043f7` |
| English | `5c4695205413df7db52b9a184815a07783999971` | `b500663220cb9bf56b9f8e8c0c544f1d6fa3a824` |
| German | `c23ab91a39434623cf1a999f387d743ade1a22a6` | `7820b4c7ba9002b0e9be23a6f45a9ab204986438` |
| Spanish | `de0bdc889594b012cad213ca83707feb81ada28b` | `a2e3e80036195343c2713410ac14f4e329e3f40d` |
| French | `42f3b262c16cfc5bde1fa2b25016fb74046de1b3` | `f14855c0f8c87a95cc52189e8e3b2bd9df186322` |
| Italian | `ca45e04b26b2f040ac63e679a459d21869f5f28d` | `57f1d005666b81645e55412658427e27099c5907` |

To set up the repository, see [INSTALL.md](INSTALL.md).

## Progress

<img src="https://raw.githubusercontent.com/PascalPixel/alchemy/progress/PROGRESS_CHART.png" width="838" alt="How much of each game's code and data is rebuilt, day by day since 16 July 2026" />
<img src="https://raw.githubusercontent.com/PascalPixel/alchemy/progress/PROGRESS.png" width="838" alt="A map of the project's files by size" />

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were created by Camelot Software Planning and published by Nintendo. Alchemy is an unofficial fan project and is not affiliated with or endorsed by either company.

Thank you to:

- Tarpman and Karathan, for identifying the compiler and flags Camelot used, and Coaltergeist, for [camelot-gcc](https://github.com/Coaltergeist/camelot-gcc), the compiler Alchemy first built with.
- [pret](https://github.com/pret), whose decompilations set the standard Alchemy measures itself against, and whose [agbcc](https://github.com/pret/agbcc) Alchemy builds the games’ library code with.
- [Simon Lindholm](https://github.com/simonlindholm/decomp-permuter), for the permuter whose behavior [Psynergy](https://github.com/PascalPixel/psynergy) reimplements in Rust.
- The [r/GoldenSun community](https://www.reddit.com/r/GoldenSun/), for sharing Alchemy, cheering it on, and keeping the love for these games alive.
