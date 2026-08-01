<p align="center">
  <img src="assets/readme/djinn_101_idle.gif" width="128" height="128" alt="Recovered Djinn battle idle loop from graphics resource 101">
  <img src="assets/readme/djinn_102_idle.gif" width="128" height="128" alt="Recovered Djinn battle idle loop from graphics resource 102">
  <img src="assets/readme/djinn_103_idle.gif" width="128" height="128" alt="Recovered Djinn battle idle loop from graphics resource 103">
  <img src="assets/readme/djinn_104_idle.gif" width="128" height="128" alt="Recovered Djinn battle idle loop from graphics resource 104">
</p>

# Alchemy — Golden Sun decompilation

An all-AI, for-fun attempt at decompiling Golden Sun (GBA). The reconstruction
and tooling in this repository are being written collaboratively with AI coding
agents—Anthropic's Claude and OpenAI's Codex—as an experiment in how far they
can take a clean-room decompilation together. It is a hobby/research project,
not a serious or community-driven decomp, and it is nowhere near complete.

The name comes from the moment alchemy is released upon Weyard. This project
is similarly trying to release the games from their finished cartridge images
into code, art, maps, and music that people can explore.

It is not a remake, ROM hack, emulator, or game download. The long-term target
is one byte-perfect source tree for Golden Sun and Golden Sun: The Lost Age in
English, Japanese, German, Spanish, French, and Italian. The immediate target
is 100% byte closure for the English Golden Sun release. The repository is
still in active decompilation and does not yet provide a standalone playable
game.

## Coverage map

**Core** — main-image code, 548 KB of the executable universe. Purple shows
semantic and exact C; orange shows the reviewed pools, alignment, veneers, and
runtime structure intentionally retained as assembly. There is no ordinary
gray semantic debt left in the core census:

![Main-image code coverage box tree in purple with retained assembly in orange.](assets/readme/gs1-en-core.svg?v=aa982ad0)

**Overlays** — decoded overlay code, 791 KB:

![Decoded overlay code coverage box tree in the cyan band; brightness is completion.](assets/readme/gs1-en-overlays.svg?v=5eba47e2)

**Assets** — the 7.3 MB of the cartridge that is data, not code. Same ladder in
magenta: faint is byte-represented, a third is b&w sheets, two thirds is
coloured sheets, full blaze is assets cut into individual objects:

![Asset maturity box tree in the magenta band; brightness is maturity.](assets/readme/gs1-en-assets.svg?v=7b01f5d0)

Every byte of the English cartridge. The left card is the ROM as it ships; the
right card is the audited executable denominator behind Full-C Byte Share.
Bright color is byte-exact C, the middle tone is reviewed semantic C, gray is
ordinary assembly debt, orange is reviewed permanent assembly, and pink is
non-code data reproduced from tracked asset sources.

The picture is derived from tracked evidence alone—no ROM, no toolchain, no
build output—so it redraws in about a second:

```sh
bun run coverage
```

`main` is the authoritative integrated tree. The map records the exact and
semantic source refs used for a draw; the normal `main` close-out uses its own
worktree for both. Regenerate it after a verified integration so the README and
the local dashboard display the same evidence.

The measured totals live in
[`metrics/gs1-en-coverage-map.json`](metrics/gs1-en-coverage-map.json). The
exact-C numbers behind the picture are reconciled against the
`metrics/gs1-en-progress.json` of the same tree the lane was drawn from, and a
disagreement is an error rather than a redrawn picture. The semantic lane is
drawn beside the headline metric rather than folded into it.

## Clean-room boundary

The evidence and publication rules are part of the repository in
[PROVENANCE.md](PROVENANCE.md). A fresh clone should activate the tracked
publication hooks once with:

```sh
git config core.hooksPath .hooks
```

Current measured decompilation status and the remaining-work breakdown are in
[docs/PATH-TO-COMPLETION.md](docs/PATH-TO-COMPLETION.md).

## Full-C Byte Share

Alchemy has one headline progress metric: exact executable bytes generated
from byte-matching, canonical C divided by all audited executable bytes in the
main image and decoded overlays.

```sh
bun run progress
```

The exact fraction is stored in
[`metrics/gs1-en-progress.json`](metrics/gs1-en-progress.json). Function counts,
source ownership, asset round trips, and assembly-region counts are diagnostics
rather than overall decompilation progress. Historical first-parent measurements
are published non-destructively in
[`docs/full-c-history.json`](docs/full-c-history.json); existing commit IDs have
not been rewritten.

## Verification

With the approved local toolchains and target inputs in place, the canonical
verification entry point is:

```sh
bun run verify
```

It runs the complete self-test suite, compiles every exact-C claim, proves that
the source-only image owns all ROM bytes, and verifies that the normal full
build is byte-identical with zero ROM fallback. Only after those checks pass
does it independently regenerate and validate Full-C Byte Share.
