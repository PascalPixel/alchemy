<p align="center">
  <img src="assets/readme/djinn_101_idle.gif" width="64" height="64" alt="Venus Djinn">
  <img src="assets/readme/djinn_102_idle.gif" width="64" height="64" alt="Jupiter Djinn">
  <img src="assets/readme/djinn_103_idle.gif" width="64" height="64" alt="Mars Djinn">
  <img src="assets/readme/djinn_104_idle.gif" width="64" height="64" alt="Mercury Djinn">
</p>

<h1 align="center">Alchemy</h1>

<p align="center"><strong>Golden Sun 25th Anniversary Preservation Project</strong></p>

Twenty-five years after *Golden Sun* was first released, Alchemy is an
automated, clean-room attempt to reconstruct the Game Boy Advance classic as
readable C code, data, artwork, maps, and music.

## DONE: Currently 27%

26% of the game's audited executable bytes now build from exact, byte-matching
C, or belong to the small pieces of low-level code intentionally kept as
permanent assembly. These four maps show where the code and assets of the
English release stand today.

### Main game

The main game is the 548,364 bytes of code that remain available while *Golden
Sun* is running. Bright purple is exact, byte-matching C; the softer purple is
readable C that still needs to be matched. Dark gray marks the small pieces of
low-level runtime structure kept as permanent assembly.

![Main-image code coverage box tree in purple with permanent assembly in dark gray.](assets/readme/gs1-en-core.svg?v=b0df3801)

### Code overlays

*Golden Sun* also loads 96 additional code modules into memory when it needs
them. This panel tracks all 795,370 executable bytes across those modules.
Bright cyan is exact C, the softer cyan is readable C still being matched, gray
is code still to reconstruct, and dark gray is permanent low-level assembly.
Eleven of those modules are already completely exact, byte-matching C.

![Decoded code-overlay coverage box tree in the cyan band; brightness is completion.](assets/readme/gs1-en-overlays.svg?v=8d7da471)

### Images and data

![Image and data maturity box tree in the magenta band; brightness is maturity.](assets/readme/gs1-en-images.svg?v=01eb15ee)

### Music and sound

The remaining 563,198 bytes of music, samples, sequences, and soundfonts have
their own maturity panel so audio progress remains legible instead of being
compressed into the image chart. Brighter orange means a more complete
reconstruction.

![Music and sound maturity box tree in the orange band; brightness is maturity.](assets/readme/gs1-en-music.svg?v=0de9466b)

Progress is measured by executable bytes rather than function counts, so every
claimed byte must compile to the original game before it contributes to the
headline.

## Preservation and copyright

Alchemy is an unofficial, noncommercial decompilation and preservation project.
*Golden Sun* and all associated names, characters, graphics, music, text, and
other original material are copyright Nintendo and Camelot Software Planning.
Alchemy is not affiliated with or endorsed by either company, and no ownership
of *Golden Sun* is claimed.
