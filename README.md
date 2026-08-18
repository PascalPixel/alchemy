<p align="center">
  <img src="assets/readme/djinn_101_idle.gif" width="64" height="64" alt="Venus Djinn">
  <img src="assets/readme/djinn_102_idle.gif" width="64" height="64" alt="Jupiter Djinn">
  <img src="assets/readme/djinn_103_idle.gif" width="64" height="64" alt="Mars Djinn">
  <img src="assets/readme/djinn_104_idle.gif" width="64" height="64" alt="Mercury Djinn">
</p>

<h1 align="center">Alchemy</h1>

<p align="center"><strong>Golden Sun 25th Anniversary Preservation Project</strong></p>

Alchemy is an unofficial clean-room reconstruction of the English release of
_Golden Sun_ for the Game Boy Advance. It documents and rebuilds the program,
assets, and data needed to study the original game and its construction.

Everything here is reconstructed from the original cartridge itself. No other
Golden Sun project supplies code, names, or game knowledge to this one.

## DONE: Currently 37%

DONE measures executable bytes that rebuild identically to the target: exact C
plus code that is permanently assembly. Nothing else counts.

Three figures, because one of them alone would mislead:

|                    |       bytes |                   share |
| ------------------ | ----------: | ----------------------: |
| Exact C            |     275,672 |     20.5% of executable |
| Permanent assembly |     216,278 |     16.0% of executable |
| **DONE**           | **491,950** | **36.5% of executable** |

Permanent assembly is not a shortcut and not a backlog. It is overwhelmingly
linker veneers and alignment padding, which the linker and assembler emit and no
C can produce, plus a small set of hand-written routines using calling
conventions C cannot express. It rebuilds identically, so it is done, and it will
never become C.

That also means 100% is not the target. Measured against only the bytes that
can ever be C, **exact C stands at 24.4%**. An overlay whose every function is
reconstructed still reports short of 100%, because its veneers count against it.

A large step in this figure during August 2026 came from correctly classifying
code that can never be C, not from new reconstruction. Exact C did not move.

The four maps below show the current byte metrics for the English release.
Progress is measured by bytes, not function counts.

### Main game

The main game contains 548,364 executable bytes in the purple band.

![Main-image code coverage box tree in the purple band; code categories use the legend below.](assets/readme/gs1-en-core.svg?v=b899b032)

### Code overlays

The game loads 96 additional code overlays containing 798,758 executable bytes
in the cyan band.

![Decoded code-overlay coverage box tree in the cyan band; code categories use the legend below.](assets/readme/gs1-en-overlays.svg?v=5ec4d5d4)

### Images and data

![Image and data maturity box tree in the magenta band; brightness is maturity.](assets/readme/gs1-en-images.svg?v=e865af5f)

### Music and sound

![Music and sound maturity box tree in the orange band; brightness is maturity.](assets/readme/gs1-en-music.svg?v=d4f23519)
