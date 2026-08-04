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

## DONE: Currently 26%

**DONE is Exact C + Permanent ASM**: the share of the game's audited executable
bytes that either build from exact, byte-matching C or belong to low-level
assembly intentionally kept in its permanent form. These maps show where the
code and assets of the English release stand today.

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

`resource_384`, `resource_37c`, `resource_37d`, `resource_37e`, `resource_388`,
`resource_38c`, `resource_3ac`, `resource_390`, `resource_3a9`,
`resource_3cc`, and `resource_397` are the first eleven pret-like showcase
overlays: all 130 genuine functions are ordinary byte-exact C, with only
export/import veneers, literal data, and alignment retained in assembly.
`bun run verify:showcase` rebuilds every showcase's decoded image and runs the
complete overlay-closure audit.

![Decoded code-overlay coverage box tree in the cyan band; brightness is completion.](assets/readme/gs1-en-overlays.svg?v=8ee3d45a)

### Images and data

![Image and data maturity box tree in the magenta band; brightness is maturity.](assets/readme/gs1-en-images.svg?v=af050cbb)

### Music and sound

The remaining 563,198 bytes of music, samples, sequences, and soundfonts have
their own maturity panel so audio progress remains legible instead of being
compressed into the image chart. Brighter orange means a more complete
reconstruction.

![Music and sound maturity box tree in the orange band; brightness is maturity.](assets/readme/gs1-en-music.svg?v=d62fa185)

DONE is measured by executable bytes rather than function counts. Exact C must
compile to the original game byte for byte; Permanent ASM must have an explicit
source-ownership contract before either contributes to the headline.

## Preservation and copyright

Alchemy is an unofficial, noncommercial decompilation and preservation project.
*Golden Sun* and all associated names, characters, graphics, music, text, and
other original material are copyright Nintendo and Camelot Software Planning.
Alchemy is not affiliated with or endorsed by either company, and no ownership
of *Golden Sun* is claimed.
