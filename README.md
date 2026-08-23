<p align="center">
  <img src="games/gs1/assets/readme/djinn_101_idle.gif" width="64" height="64" alt="Venus Djinn">
  <img src="games/gs1/assets/readme/djinn_102_idle.gif" width="64" height="64" alt="Jupiter Djinn">
  <img src="games/gs1/assets/readme/djinn_103_idle.gif" width="64" height="64" alt="Mars Djinn">
  <img src="games/gs1/assets/readme/djinn_104_idle.gif" width="64" height="64" alt="Mercury Djinn">
</p>

<h1 align="center">Alchemy</h1>

<p align="center"><strong>Golden Sun 25th Anniversary Preservation Project</strong></p>

Alchemy is an unofficial clean-room reconstruction of _Golden Sun_ and
_Golden Sun: The Lost Age_ for the Game Boy Advance: two games, each with
Japanese, English, German, Spanish, French, and Italian editions. The Japanese
edition of each game is its canonical source base. The English _Golden Sun_
ROM is currently the first complete derived build target.

The historical reconstructions are also the preservation base for Alchemy's
eventual third product: one reintegrated game spanning both games' content.
That integration lives separately from the twelve historical targets, so it
cannot blur or weaken their reconstruction evidence.

Everything here is reconstructed from the approved cartridges themselves. No
other Golden Sun project supplies code, names, or game knowledge to this one.

The project is a decompilation and preservation effort, not a remake, ROM hack,
emulator, or game distribution. No ROM is distributed by this repository.
_Golden Sun_ and its original material are
copyright Nintendo and Camelot Software Planning. Alchemy is not affiliated
with or endorsed by either company.

## Repository shape

The layout follows the useful pret convention: one shared source tree per
game, selected per edition at build time, rather than six copied source trees.

- `games/gs1/` contains the shared GS1 source, retained assembly, source
  assets, headers, reconstruction corpus, semantic records, and metrics.
- `games/gs2/` is the equivalent independent product tree for GS2.
- `games/alchemy/` contains only the future reintegration. It has no reference
  ROM and never counts as a historical byte-exact target.

Each historical target has its own edition define and isolated output under
`out/<game>-<edition>/`. `make targets` proves that all twelve source routes
compile. That is deliberately weaker than reproduction: only `gs1-en` has its
complete link, ownership, asset, and byte-identical verification surfaces
installed today. `make verify` remains that authoritative full-ROM gate.

## DONE: Currently 45%

DONE measures executable bytes that rebuild identically to the target: exact C
plus code that is permanently assembly. Nothing else counts.

Three figures, because one of them alone would mislead:

|                    |       bytes |                   share |
| ------------------ | ----------: | ----------------------: |
| Exact C            |     299,774 |     22.2% of executable |
| Permanent assembly |     310,566 |     23.0% of executable |
| **DONE**           | **610,340** | **45.3% of executable** |

Permanent assembly is not a shortcut and not a backlog. It includes linker
veneers and alignment padding, plus audited routines whose instruction shape or
calling convention the approved C compiler cannot emit. Every retained region
records that evidence and rebuilds identically, so it is done and will never
become C.

That also means 100% is not the target. Measured against only the bytes that
can ever be C, **exact C stands at 28.9%**. An overlay whose every function is
reconstructed still reports short of 100%, because its veneers count against it.

Tracked reconstruction C is reported separately. It preserves recovered
algorithms and failed experiments, but contributes nothing to this table until
the linked bytes for an edition are exact.

The four maps below show the current byte metrics for the English derived
build. Progress is measured by bytes, not function counts. Tracked but
non-exact reconstruction is shown separately and never contributes to DONE.

In the two code charts, the bright colour is exact C, the pale colour is
permanent assembly, and the near-white ground is what is still to reconstruct.
Each footer reports that band on its own, so it differs from the whole-ROM
figure above.

### Main game

The main game contains 548,364 executable bytes in the purple band.

![Main-image code coverage box tree in the purple band; code categories use the legend below.](games/gs1/assets/readme/gs1-en-core.svg?v=99b5c0653c65f7a9)

### Code overlays

The game loads 96 additional code overlays containing 798,758 executable bytes
in the cyan band.

![Decoded code-overlay coverage box tree in the cyan band; code categories use the legend below.](games/gs1/assets/readme/gs1-en-overlays.svg?v=5854531410f83bd3)

### Images and data

This map covers 6,735,557 bytes of graphics, maps, text, and other data.
**Named** is the finished form: a standalone file whose name says what it
is, like `vale_night.png`, `rock_front.png`, `isaac_running_south_west.png`
or `djinn_venus.gif`. **Extracted** is the same picture sitting under the
ROM's own numbering — `resource_226_grid_value_high.png` — which is not the
same as knowing what it is. Almost everything is still Extracted.

![Image and data maturity box tree in the magenta band; brightness is maturity.](games/gs1/assets/readme/gs1-en-images.svg?v=dac5fe2ce044fef6)

### Music and sound

This map covers 563,198 bytes of music, samples, sequences, and soundfonts.
The finished form is `growl.wav`, `good_morning.mid` and `alchemy.sf2`.
Every file now carries a name instead of a ROM index, but read the tier as
**provisional**: the names are manufactured from what each sound measurably is —
`music_walking_passage_quintet_full`, `effect_sample_large_oneshot` — and none of
them claims a title or a scene. Nothing was taken from outside the ROM.

![Music and sound maturity box tree in the orange band; brightness is maturity.](games/gs1/assets/readme/gs1-en-music.svg?v=0fe5700a51fd7ccf)

## Contributing and project status

[CONTRIBUTING.md](CONTRIBUTING.md) is the only other page in this repository.
It carries the workflow, the tools, and the rules the project holds itself to.
Current work and known blockers live in the commit history, attached to the
change that established them.
