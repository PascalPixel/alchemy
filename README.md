# Golden Sun Decompilation

Alchemy is a decompilation of both Golden Sun games for the Game Boy Advance:
**The Broken Seal** ☀️ and **The Lost Age** ⚓️.

In Golden Sun, lighting the four Elemental Lighthouses releases Alchemy upon
the world. This project hopes to do something similar for fans.

## What is a decompilation?

Camelot wrote Golden Sun in a programming language and turned it into the
machine code on the cartridge. The original source code was never released. A
decompilation works backwards: it rewrites the game as readable source code
that turns back into the exact same game, byte for byte.

## Why?

Once a game exists as readable source, it can be changed and rebuilt like any
other program. That is the path to a **recompilation**: Golden Sun running
natively on modern systems, and a base for everything fans have dreamed of:

- new storylines, quests and Djinn
- quality-of-life fixes
- widescreen and other aspect ratios
- higher frame rates, new translations and more

Alchemy is the foundation those projects can build on.

## Status: ☀️ 57.43% · ⚓️ 2.14%

<img src="PROGRESS.svg" width="830" alt="Alchemy progress for The Broken Seal and The Lost Age, above a map of the repository's files">

The numbers are strict: anything found to match only through a trick is
withdrawn, even when that lowers the percentage.

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were created by
Camelot Software Planning and published by Nintendo. Alchemy is an unofficial
fan project and is not affiliated with or endorsed by either company.

Thank you to:

- The [r/GoldenSun community](https://www.reddit.com/r/GoldenSun/), for sharing
  Alchemy, cheering it on, and keeping the love for these games alive.
- Tarpman and Karathan, for working out which compiler and flags Camelot used.
- Coaltergeist, for [camelot-gcc](https://github.com/Coaltergeist/camelot-gcc),
  the compiler Alchemy first built with.
- [pret](https://github.com/pret), whose decompilations set the standard Alchemy
  measures itself against, and whose [agbcc](https://github.com/pret/agbcc)
  Alchemy builds with.
- Brankale and Pokefan531, for the
  [Handheld Color Space Project](https://github.com/Brankale/Handheld-Color-Space-Project),
  whose screen measurements drive the map viewer's GBA, GBA SP and Game Boy
  Micro colour filters.
