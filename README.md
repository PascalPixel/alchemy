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

The project is a decompilation and preservation effort, not a remake, ROM hack,
emulator, or game distribution. The target is the released English ROM; no ROM
is distributed by this repository. _Golden Sun_ and its original material are
copyright Nintendo and Camelot Software Planning. Alchemy is not affiliated
with or endorsed by either company.

## DONE: Currently 36%

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

In the two code charts, the bright colour is exact C, the pale colour is
permanent assembly, and the near-white ground is what is still to reconstruct.
Each footer reports that band on its own, so it differs from the whole-ROM
figure above.

### Main game

The main game contains 548,364 executable bytes in the purple band.

![Main-image code coverage box tree in the purple band; code categories use the legend below.](assets/readme/gs1-en-core.svg?v=4b08c9b3cac16631)

### Code overlays

The game loads 96 additional code overlays containing 798,758 executable bytes
in the cyan band.

![Decoded code-overlay coverage box tree in the cyan band; code categories use the legend below.](assets/readme/gs1-en-overlays.svg?v=251a2692f384ae2e)

### Images and data

This map covers 6,735,557 bytes of graphics, maps, text, and other data.
**Named** is the finished form: a standalone file whose name says what it
is, like `vale_night.png`, `rock_front.png`, `isaac_running_south_west.png`
or `djinn_venus.gif`. **Extracted** is the same picture sitting under the
ROM's own numbering — `resource_226_grid_value_high.png` — which is not the
same as knowing what it is. Almost everything is still Extracted.

![Image and data maturity box tree in the magenta band; brightness is maturity.](assets/readme/gs1-en-images.svg?v=792cd3c89cb9f217)

### Music and sound

This map covers 563,198 bytes of music, samples, sequences, and soundfonts.
The finished form is `growl.wav`, `good_morning.mid` and `alchemy.sf2`.
Every file now carries a name instead of a ROM index, but read the tier as
**provisional**: the names are manufactured from what each sound measurably is —
`music_walking_passage_quintet_full`, `effect_sample_large_oneshot` — and none of
them claims a title or a scene. Nothing was taken from outside the ROM.

![Music and sound maturity box tree in the orange band; brightness is maturity.](assets/readme/gs1-en-music.svg?v=b40d538dfe095aa0)

## Contributing and project status

[CONTRIBUTING.md](CONTRIBUTING.md) is the only other page in this repository.
It carries the workflow, the tools, and the rules the project holds itself to.
Current work and known blockers live in the commit history, attached to the
change that established them.
