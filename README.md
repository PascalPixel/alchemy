## ⛰️ Alchemy

# Golden Sun Decompilation

In Golden Sun, lighting the four Elemental Lighthouses releases Alchemy upon the world. This project hopes to do something similar for fans: recovering the source code of both games (The Broken Seal and The Lost Age) using AI, for video game preservation, and opening the door to fan projects built on it.

The two games share much of the same code, so Alchemy recovers them side by side.

## Status: ☀️ 55.96% · ⚓️ 0.28%

![ROM contents](<games/THE BROKEN SEAL/PREVIEW/TBS-EN-ROM.SVG?v=1b69176ae80cc3d6>)

**DONE** measures recovered code: byte-exact C and evidenced permanent
assembly, divided by each game's audited executable bytes. ☀️ is The Broken
Seal; ⚓️ is The Lost Age. Shared code counts in each game only where it has
been verified for that game. Assets appear in the viewer but do not raise DONE.

The Broken Seal's English ROM rebuilds byte for byte. The Lost Age's recovered
functions are checked individually; it does not yet have a complete ROM build.

Alchemy is not a remake, a mod, an emulator or a download of the games, and you
need your own copy of the games to build it. Build instructions are in
[AGENTS.md](AGENTS.md), and what is left to do is in [TODO.md](TODO.md).

## Clean room

Alchemy follows a documented clean-room policy modelled on long-running
decompilations such as pret's. It is written from the games themselves, public
documentation and its own recovery work, and records where each file comes
from. It will open to outside contributions when it reaches 100%.

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
