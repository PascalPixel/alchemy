## ⛰️ Alchemy

# Golden Sun Decompilation

In Golden Sun, lighting the four Elemental Lighthouses releases Alchemy upon
the world. This project hopes to do something similar for the games
themselves: to turn **The Broken Seal** ☀️ and **The Lost Age** ⚓️ back into
source code that anyone can read, change and build.

## Status: ☀️ 62.61% · ⚓️ 2.14%

<img src="PROGRESS.svg" width="830" alt="Alchemy progress for The Broken Seal and The Lost Age, above a map of the repository's files">

The gold bars show how much of each game has been brought back so far. Both
games already rebuild from this repository, exactly as they shipped, down to
the last byte. The percentage is the part of that rebuild that now comes from
readable source rather than raw machine code. The Broken Seal is well past
halfway. The Lost Age has only just begun; much of its code closely resembles
the first game's, which gives it a head start.

## What this is

Camelot wrote Golden Sun in the C programming language and turned it into the
machine code on the cartridge. That original source code was never released.
A decompilation works backwards: piece by piece, it rewrites the game as
readable code that turns back into the very same cartridge. Every function
has to match the original exactly before it counts, so the result is not a
guess at how the game works but the game itself, in a form people can read.

We try to make that code look the way Camelot's own files might have looked
in 2001, from the folder names down to the names of scenes and characters,
which follow the Japanese originals: Sukureta for Kraden, Gerald for Garet.

Alchemy is not a remake, a mod, an emulator or a download. It contains no
ROMs, and you need your own copy of each game to build it.

## Why

Once a game exists as readable source, it can be changed and rebuilt like any
other program. That opens the way to a recompilation: Golden Sun running
natively on modern systems, and a foundation for everything fans have dreamed
of, from new storylines, quests and Djinn to quality-of-life fixes, widescreen,
higher frame rates and new translations.

## How it is made

Alchemy follows the example of pret, whose Pokémon decompilations set the
standard for this kind of work. It is written only from the games themselves,
public documentation and its own research, never from leaked code. Where a
function matches only through an unusual construct, it is marked as such and
cleaned up later, just as pret does. The project will open to outside
contributions once it reaches 100%. Build instructions and the working rules
are in [AGENTS.md](AGENTS.md).

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
