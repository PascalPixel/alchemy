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

## DONE: Currently 37%

DONE measures executable bytes that rebuild identically to the target: exact C
plus code that is permanently assembly. Nothing else counts. There is no tier
for C that reads well but does not reproduce: a byte is exact C, or it is
assembly still to reconstruct.

Three figures, because one of them alone would mislead:

| | bytes | share |
|---|---:|---:|
| Exact C | 274,858 | 20.40% of executable |
| Permanent assembly | 216,278 | 16.05% of executable |
| **DONE** | **491,136** | **36.46% of executable** |


Permanent assembly is not a shortcut and not a backlog. It is overwhelmingly
linker veneers and alignment padding, which the linker and assembler emit and no
C can produce, plus a small set of hand-written routines using calling
conventions C cannot express. It rebuilds identically, so it is done, and it will
never become C.

That also means 100% of the ROM is not the target. Of the 1,347,122 executable
bytes, 216,278 can never be C, so the reconstruction's own ceiling is 1,130,844
and **exact C stands at 24.31% of the bytes that can be C**. An overlay whose
every function is reconstructed still reports short of 100% because its veneers
count against it.

DONE rose from 26% to 36% in August 2026 without a line of C being written, and
that is worth stating plainly rather than presenting as progress. 132,364 bytes
of hand-written assembly and library code had been sitting in the unfinished
column: `arm.md` gates both store-multiple peepholes on `TARGET_ARM`, so stock
gcc 2.96 emits no Thumb `stmia`/`ldmia` from any source, and 281 owners have one
inside their own span. They rebuild identically and will never be C, which is
what DONE measures. Exact C did not move.

The four maps below show the current byte metrics for the English release.
Progress is measured by bytes, not function counts.

For the two code charts, the bright band color marks Exact C, the pale band
color marks permanent assembly, and the near-white ground is assembly still to
reconstruct. The footer reports Exact C, Permanent ASM and their sum as DONE
for that band alone, so it differs from the whole-ROM figure above; assembly
still to reconstruct is the remainder and is not a separate footer item.

### Main game

The main game contains 548,364 executable bytes in the purple band.

![Main-image code coverage box tree in the purple band; code categories use the legend below.](assets/readme/gs1-en-core.svg?v=b899b032)

### Code overlays

The game loads 96 additional code overlays containing 798,758 executable bytes
in the cyan band.

![Decoded code-overlay coverage box tree in the cyan band; code categories use the legend below.](assets/readme/gs1-en-overlays.svg?v=5ec4d5d4)

### Images and data

This map covers 6,735,557 bytes of graphics, maps, text, and other data.
**Named** is the finished form: a standalone file whose name says what it
is, like `vale_night.png`, `rock_front.png`, `isaac_running_south_west.png`
or `djinn_venus.gif`. **Extracted** is the same picture sitting under the
ROM's own numbering — `resource_226_grid_value_high.png` — which is not the
same as knowing what it is. Almost everything is still Extracted.

![Image and data maturity box tree in the magenta band; brightness is maturity.](assets/readme/gs1-en-images.svg?v=e865af5f)

### Music and sound

This map covers 563,198 bytes of music, samples, sequences, and soundfonts.
The finished form is `growl.wav`, `good_morning.mid` and `alchemy.sf2`.
Every file now carries a name instead of a ROM index, but read the tier as
**provisional**: the names are manufactured from what each sound measurably is —
`music_walking_passage_quintet_full`, `effect_sample_large_oneshot` — and none of
them claims a title or a scene. Nothing was taken from outside the ROM. Replacing
one is a single edit to `assets/audio/sequences/index.json`, which is exactly what
they are there for.

![Music and sound maturity box tree in the orange band; brightness is maturity.](assets/readme/gs1-en-music.svg?v=d4f23519)

## Contributing and project status

[CONTRIBUTING.md](CONTRIBUTING.md) is the only other page in this repository.
It carries the reconstruction and matching workflow, the tool catalog, the
clean-room boundary, the source provenance and the publication rules. Current
work and known blockers live in the commit history, attached to the change that
established them.
