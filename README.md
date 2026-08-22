<p align="center">
  <img src="games/gs1/assets/readme/djinn_101_idle.gif" width="64" height="64" alt="Venus Djinn">
  <img src="games/gs1/assets/readme/djinn_102_idle.gif" width="64" height="64" alt="Jupiter Djinn">
  <img src="games/gs1/assets/readme/djinn_103_idle.gif" width="64" height="64" alt="Mars Djinn">
  <img src="games/gs1/assets/readme/djinn_104_idle.gif" width="64" height="64" alt="Mercury Djinn">
</p>

<h1 align="center">Alchemy</h1>

<p align="center"><strong>Golden Sun 25th Anniversary Preservation Project</strong></p>

The historical reconstructions are also the preservation base for Alchemy's
eventual third product: one reintegrated game spanning both games' content.
That integration lives separately from the twelve historical targets, so it
cannot blur or weaken their reconstruction evidence.

Everything here is reconstructed from the approved cartridges themselves. No
other Golden Sun project supplies code, names, or game knowledge to this one.

## Repository shape

The layout follows the useful pret convention: one shared source tree per
game, selected per edition at build time, rather than six copied source trees.

## DONE: Currently 40%

DONE measures executable bytes that rebuild identically to the target: exact C
plus code that is permanently assembly. Nothing else counts.

Three figures, because one of them alone would mislead:

|                    |       bytes |                   share |
| ------------------ | ----------: | ----------------------: |
| Exact C            |     282,108 |     20.9% of executable |
| Permanent assembly |     216,278 |     16.0% of executable |
| **DONE**           | **498,386** | **37.0% of executable** |

Permanent assembly is not a shortcut and not a backlog. It is overwhelmingly
linker veneers and alignment padding, which the linker and assembler emit and no
C can produce, plus a small set of hand-written routines using calling
conventions C cannot express. It rebuilds identically, so it is done, and it will
never become C.

That also means 100% is not the target. Measured against only the bytes that
can ever be C, **exact C stands at 24.9%**. An overlay whose every function is
reconstructed still reports short of 100%, because its veneers count against it.

Tracked reconstruction C is reported separately. It preserves recovered
algorithms and failed experiments, but contributes nothing to this table until
the linked bytes for an edition are exact.

The four maps below show the current byte metrics for the English derived
build. Progress is measured by bytes, not function counts. Tracked but
non-exact reconstruction is shown separately and never contributes to DONE.

### Main game

The main game contains 548,364 executable bytes in the purple band.

![Main-image code coverage box tree in the purple band; code categories use the legend below.](games/gs1/assets/readme/gs1-en-core.svg?v=4967d7ec8baf4a71)

### Code overlays

The game loads 96 additional code overlays containing 798,758 executable bytes
in the cyan band.

![Decoded code-overlay coverage box tree in the cyan band; code categories use the legend below.](games/gs1/assets/readme/gs1-en-overlays.svg?v=a86ed4ebf6d1b116)

### Images and data

![Image and data maturity box tree in the magenta band; brightness is maturity.](games/gs1/assets/readme/gs1-en-images.svg?v=792cd3c89cb9f217)

### Music and sound

![Music and sound maturity box tree in the orange band; brightness is maturity.](games/gs1/assets/readme/gs1-en-music.svg?v=b40d538dfe095aa0)
