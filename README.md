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
[`metrics/gs1-en-progress.json`](metrics/gs1-en-progress.json). Function
counts, source ownership, asset round trips, and assembly-region counts are
diagnostics rather than overall decompilation progress. Historical first-parent
measurements are published non-destructively in
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
