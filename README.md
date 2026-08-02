<p align="center">
  <img src="assets/readme/djinn_101_idle.gif" width="128" height="128" alt="Venus Djinn">
  <img src="assets/readme/djinn_102_idle.gif" width="128" height="128" alt="Jupiter Djinn">
  <img src="assets/readme/djinn_103_idle.gif" width="128" height="128" alt="Mars Djinn">
  <img src="assets/readme/djinn_104_idle.gif" width="128" height="128" alt="Mercury Djinn">
</p>

<h1 align="center">Alchemy</h1>

<p align="center"><strong>Golden Sun 25th Anniversary Preservation Project</strong></p>

Twenty-five years after *Golden Sun* was first released, Alchemy is an
automated, clean-room attempt to reconstruct the Game Boy Advance classic as
readable C code, data, artwork, maps, and music.

Decompilation works backwards from a finished game to human-readable source
code that can be understood, preserved, and built again. A faithful source tree
can make native ports possible, keep a game working on modern hardware, add
widescreen and high-frame-rate support, improve controls and accessibility, fix
long-standing bugs, and provide a foundation for mods and entirely new
adventures.

Alchemy is not a remake, ROM hack, emulator, or game download. Its immediate
goal is a byte-perfect reconstruction of the English release of *Golden Sun*,
followed by *The Lost Age* and the games' other languages.

The name comes from the moment alchemy is released upon Weyard. In much the same
spirit, this project is releasing *Golden Sun* from its finished ROM image into
code and assets that people can study and learn from. We hope that, in doing so,
Alchemy inspires new developers to create wonderful RPGs of their own.

## Progress: Currently 20% of the way done

20% of the game's audited executable bytes now build from exact, byte-matching
C. These three maps show where the code and assets of the English release stand
today.

### Main game

The main game is the 548,364 bytes of code that remain available while *Golden
Sun* is running. Bright purple is exact, byte-matching C; the softer purple is
readable C that still needs to be matched. Orange marks the small pieces of
low-level runtime structure intentionally retained as assembly.

![Main-image code coverage box tree in purple with retained assembly in orange.](assets/readme/gs1-en-core.svg?v=900bd060)

### Code overlays

*Golden Sun* also loads 96 additional code modules into memory when it needs
them. This panel tracks all 795,090 executable bytes across those modules.
Bright cyan is exact C, the softer cyan is readable C still being matched, gray
is code still to reconstruct, and orange is retained low-level assembly.

![Decoded code-overlay coverage box tree in the cyan band; brightness is completion.](assets/readme/gs1-en-overlays.svg?v=601c7eb5)

### Data and assets

The remaining 7,300,700 bytes contain the game's graphics, maps, text, music,
and other data. This panel follows 2,431 verified objects as they move from
encoded ROM data to individually understood and reusable assets. Brighter
magenta means a more complete reconstruction.

![Asset maturity box tree in the magenta band; brightness is maturity.](assets/readme/gs1-en-assets.svg?v=930f8eb5)

Exact-C progress is measured by executable bytes rather than function counts,
so every claimed byte must compile to the original game before it contributes
to the headline.

## Preservation and copyright

Alchemy is an unofficial, noncommercial decompilation and preservation project.
*Golden Sun* and all associated names, characters, graphics, music, text, and
other original material are copyright Nintendo and Camelot Software Planning.
Alchemy is not affiliated with or endorsed by either company, and no ownership
of *Golden Sun* is claimed.

This repository does not distribute a game or ROM. Its reconstructed code is
written from scratch from the released game. No original source code, leaked
material, or other proprietary development material is used. The goal of this
zero-provenance, clean-room approach is to keep Alchemy safe to study, share,
and preserve online for many years to come.
