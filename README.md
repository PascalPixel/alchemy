<p align="center">
  <img src="games/gs1/assets/readme/djinn_101_idle.gif" width="64" height="64" alt="Venus Djinn">
  <img src="games/gs1/assets/readme/djinn_102_idle.gif" width="64" height="64" alt="Jupiter Djinn">
  <img src="games/gs1/assets/readme/djinn_103_idle.gif" width="64" height="64" alt="Mars Djinn">
  <img src="games/gs1/assets/readme/djinn_104_idle.gif" width="64" height="64" alt="Mercury Djinn">
</p>

<h1 align="center">Alchemy</h1>

<p align="center"><strong>Golden Sun 25th Anniversary Preservation Project</strong></p>

Alchemy is an unofficial clean-room reconstruction of _Golden Sun_ and
_Golden Sun: The Lost Age_ for the Game Boy Advance. It preserves all six
Japanese and European-language editions of each game from the original
cartridges, with Japanese as the canonical source base and the English
_Golden Sun_ release as the first complete derived build target.

## Status: 50.10% DONE

![Code](games/gs1/assets/readme/gs1-en-code.svg?v=6bd51b45f65797ae)

![Data](games/gs1/assets/readme/gs1-en-data.svg?v=22cc8a0789412d12)

## What this is

Alchemy rebuilds both games from source code the way a studio would have kept
it on disk: readable C, a little hand-written assembly where the original
programmers clearly wrote it by hand, and named assets. Every build is checked
byte for byte against the original cartridge, so a rebuilt edition is either
identical to the real game or it is not counted.

**DONE** is the share of the game's program that has been recovered as source
and proven identical. The picture above shows it: solid blocks are recovered
and proven, lighter blocks are drafts that do not yet match exactly, and the
rest is still unknown. The Data picture shows the same for graphics, sound and
text.

All twelve editions (six of each game) share one source tree per game rather
than a copy per language. English _Golden Sun_ is the first edition rebuilt in
full; the others are checked against it.

This is a preservation and decompilation project — not a remake, ROM hack,
emulator or game download. No game data is included, and nothing here comes
from Camelot's original source. To build it yourself or help with the
reconstruction, see [CONTRIBUTING.md](CONTRIBUTING.md).

## Acknowledgements

_Golden Sun_, its characters, music, art, and original code were created by
Camelot Software Planning and published by Nintendo. Alchemy is an independent
fan preservation project and is not affiliated with or endorsed by either
company.

Thank you to everyone documenting the Game Boy Advance, improving preservation
tooling, testing reconstructed editions, and contributing careful clean-room
work to Alchemy.
