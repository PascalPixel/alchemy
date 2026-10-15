# ⛰️ Alchemy: the Golden Sun decompilation

In Golden Sun, lighting the four Elemental Lighthouses releases Alchemy upon
the world. I’m doing the same thing to the games themselves. **The Broken
Seal** ☀️ and **The Lost Age** ⚓️ are being rebuilt from readable source, piece
by piece.

**The Broken Seal ☀️ is 100% rebuilt.** All six languages build byte for byte
from the source in this repository, with no ROM. The Lost Age ⚓️ is next.

## Progress

<img src="https://raw.githubusercontent.com/PascalPixel/alchemy/progress/PROGRESS_CHART.png" width="838" alt="How much of each game's code and data is rebuilt, day by day since 16 July 2026">

<img src="https://raw.githubusercontent.com/PascalPixel/alchemy/progress/PROGRESS.png" width="838" alt="A map of the project's files by size">

Each game has two percentages. Code is how much of the game’s code has been
rewritten so that it rebuilds the original exactly, in all six languages
(Japanese, English, German, Spanish, French and Italian). Data is how much of
everything else on the cartridge (pictures, music, text, tables) is built from
our own source files rather than copied from the original. A piece only counts
once every language that uses it comes out byte for byte identical, and a
game is only 100% done when both numbers are.

The chart shows that climb day by day: code as the bright line and data,
measured since 8 October 2026, as the darker one. Sudden drops are honesty
checks: when we find something that only looked finished, it comes back off
the count until it’s done properly. The map below it shows the whole project by size: everything
in light rose is still waiting to be rewritten.

## What this is

When Camelot made Golden Sun, they wrote it as instructions a person can read
and then turned those into the unreadable code that sits on the cartridge. The
readable version was never released. Alchemy rebuilds it, and every piece is
checked against the original until the two are identical, in all six
languages. The AI does the heavy lifting, and the original game is the judge.
Names follow the Japanese release: Sukureta for Kraden, Gerald for Garet.

Alchemy is not a remake, a mod, an emulator or a download of the games, and it
doesn’t include them. It will open to outside contributions once both games
are complete.

It builds the following ROMs of The Broken Seal ☀️:

- Japanese: `sha1: 35ef9e4c9f38183ebd6a3e3923a11ce9a4333718`
- English: `sha1: 5c4695205413df7db52b9a184815a07783999971`
- German: `sha1: c23ab91a39434623cf1a999f387d743ade1a22a6`
- Spanish: `sha1: de0bdc889594b012cad213ca83707feb81ada28b`
- French: `sha1: 42f3b262c16cfc5bde1fa2b25016fb74046de1b3`
- Italian: `sha1: ca45e04b26b2f040ac63e679a459d21869f5f28d`

To set up the repository and check it against your own cartridge, see
[INSTALL.md](INSTALL.md).

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were created by
Camelot Software Planning and published by Nintendo. Alchemy is an unofficial
fan project and is not affiliated with or endorsed by either company.

Thank you to:

- The [r/GoldenSun community](https://www.reddit.com/r/GoldenSun/), for sharing
  Alchemy, cheering it on, and keeping the love for these games alive.
- Tarpman and Karathan, for identifying the compiler and flags Camelot used.
- Coaltergeist, for [camelot-gcc](https://github.com/Coaltergeist/camelot-gcc),
  the compiler Alchemy first built with.
- [pret](https://github.com/pret), whose decompilations set the standard Alchemy
  measures itself against, and whose [agbcc](https://github.com/pret/agbcc)
  Alchemy builds the games’ library code with.
- [Simon Lindholm](https://github.com/simonlindholm/decomp-permuter), for the
  permuter whose behavior Alchemy reimplements in Rust.
