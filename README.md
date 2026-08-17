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

Game-specific facts are reconstructed from the approved local ROM evidence.
External projects may inform generic tooling or repository methods only; they
are not source, naming, or game-knowledge evidence.

The project is a decompilation and preservation effort, not a remake, ROM hack,
emulator, or game distribution. The target is the released English ROM; no ROM
is distributed by this repository. *Golden Sun* and its original material are
copyright Nintendo and Camelot Software Planning. Alchemy is not affiliated
with or endorsed by either company.

## DONE: Currently 26%

DONE measures executable bytes that rebuild identically to the target: exact C
plus code that is permanently assembly. Semantic C is tracked separately as
readable reconstruction work; it does not count toward DONE until its rebuilt
bytes are exact.

Three figures, because one of them alone would mislead:

| | bytes | share |
|---|---:|---:|
| Exact C | 274,372 | 20.37% of executable |
| Permanent assembly | 78,932 | 5.86% of executable |
| **DONE** | **353,304** | **26.23% of executable** |

Permanent assembly is not a shortcut and not a backlog. It is overwhelmingly
linker veneers and alignment padding, which the linker and assembler emit and no
C can produce, plus a small set of hand-written routines using calling
conventions C cannot express. It rebuilds identically, so it is done, and it will
never become C.

That also means 100% is not the target. Of the 1,347,122 executable bytes,
78,932 can never be C, so the reconstruction's own ceiling is 1,268,190 and
**exact C stands at 21.63% of the bytes that can be C**. An overlay whose every
function is reconstructed still reports short of 100% because its veneers count
against it.

The four maps below show the current byte metrics for the English release.
Progress is measured by bytes, not function counts.

For the two code charts, the bright band color marks Exact C, the pale band
color marks permanent assembly, and the near-white ground covers Semantic C and
ordinary assembly still to reconstruct. The footer reports separate byte
shares for Semantic, Permanent ASM, and Exact C; ordinary assembly is not a
separate footer item.

### Main game

The main game contains 548,364 executable bytes in the purple band.

![Main-image code coverage box tree in the purple band; code categories use the legend below.](assets/readme/gs1-en-core.svg?v=c298eedc)

### Code overlays

The game loads 96 additional code overlays containing 798,758 executable bytes
in the cyan band.

![Decoded code-overlay coverage box tree in the cyan band; code categories use the legend below.](assets/readme/gs1-en-overlays.svg?v=e01ab921)

### Images and data

This map covers 6,735,557 bytes of graphics, maps, text, and other data as they
are converted into understood and reusable assets.

![Image and data maturity box tree in the magenta band; brightness is maturity.](assets/readme/gs1-en-images.svg?v=01eb15ee)

### Music and sound

This map covers 563,198 bytes of music, samples, sequences, and soundfonts.

![Music and sound maturity box tree in the orange band; brightness is maturity.](assets/readme/gs1-en-music.svg?v=0de9466b)

## Contributing and project status

[CONTRIBUTING.md](CONTRIBUTING.md) is the only other page in this repository.
It carries the reconstruction and matching workflow, the tool catalog, the
clean-room boundary, the source provenance and the publication rules. Current
work and known blockers live in the commit history, attached to the change that
established them.
