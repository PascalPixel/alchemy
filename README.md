<p align="center">
  <img src="assets/readme/djinn_101_idle.gif" width="64" height="64" alt="Venus Djinn">
  <img src="assets/readme/djinn_102_idle.gif" width="64" height="64" alt="Jupiter Djinn">
  <img src="assets/readme/djinn_103_idle.gif" width="64" height="64" alt="Mars Djinn">
  <img src="assets/readme/djinn_104_idle.gif" width="64" height="64" alt="Mercury Djinn">
</p>

<h1 align="center">Alchemy</h1>

<p align="center"><strong>Golden Sun 25th Anniversary Preservation Project</strong></p>

Alchemy is an unofficial clean-room reconstruction of the English release of
*Golden Sun* for the Game Boy Advance. It documents and rebuilds the program,
assets, and data needed to study the original game and its construction.

## DONE: Currently 26%

DONE measures executable bytes that rebuild identically to the target: exact C
plus the small amount of code intentionally retained as permanent assembly.
Semantic C is tracked separately as readable reconstruction work; it does not
count toward DONE until its rebuilt bytes are exact.

The four maps below show the current byte metrics for the English release.
Progress is measured by bytes, not function counts.

For the two code charts, the bright band color marks Exact C, the pale band
color marks permanent assembly, and the near-white ground covers Semantic C and
ordinary assembly still to reconstruct. The footer reports separate byte
shares for Semantic, Permanent ASM, and Exact C; ordinary assembly is not a
separate footer item.

### Main game

The main game contains 548,364 executable bytes in the purple band.

![Main-image code coverage box tree in the purple band; code categories use the legend below.](assets/readme/gs1-en-core.svg?v=a6e07af8)

### Code overlays

The game loads 96 additional code overlays containing 798,890 executable bytes
in the cyan band.

![Decoded code-overlay coverage box tree in the cyan band; code categories use the legend below.](assets/readme/gs1-en-overlays.svg?v=576a475f)

### Images and data

This map covers 6,735,557 bytes of graphics, maps, text, and other data as they
are converted into understood and reusable assets.

![Image and data maturity box tree in the magenta band; brightness is maturity.](assets/readme/gs1-en-images.svg?v=01eb15ee)

### Music and sound

This map covers 563,198 bytes of music, samples, sequences, and soundfonts.

![Music and sound maturity box tree in the orange band; brightness is maturity.](assets/readme/gs1-en-music.svg?v=0de9466b)
