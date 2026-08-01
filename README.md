> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

<p align="center">
  <img src="assets/readme/djinn_101_idle.gif" width="128" height="128" alt="Recovered Djinn battle idle loop from graphics resource 101">
  <img src="assets/readme/djinn_102_idle.gif" width="128" height="128" alt="Recovered Djinn battle idle loop from graphics resource 102">
  <img src="assets/readme/djinn_103_idle.gif" width="128" height="128" alt="Recovered Djinn battle idle loop from graphics resource 103">
  <img src="assets/readme/djinn_104_idle.gif" width="128" height="128" alt="Recovered Djinn battle idle loop from graphics resource 104">
</p>

# Alchemy — Golden Sun decompilation

A for-fun, clean-room attempt at decompiling Golden Sun (GBA). It is a
hobby/research project, not a serious or community-driven decomp, and it is
nowhere near complete.

The name comes from the moment alchemy is released upon Weyard. This project
is similarly trying to release the games from their finished ROM images
into code, art, maps, and music that people can explore.

It is not a remake, ROM hack, emulator, or game download. The long-term target
is one byte-perfect source tree for Golden Sun and Golden Sun: The Lost Age in
English, Japanese, German, Spanish, French, and Italian. The immediate target
is 100% byte closure for the English Golden Sun release. The repository is
still in active decompilation and does not yet provide a standalone playable
game.

## Coverage map

**Main image** — 548,364 bytes of resident code. Thirteen real 64 KiB address
banks wrap the audited source-owner leaves; the final bank contains only the
executable bytes actually present in that window. Purple shows
semantic and exact C; orange shows the reviewed pools, alignment, veneers, and
runtime structure intentionally retained as assembly. There is no ordinary
gray semantic debt left in the main-image census. Each tile follows one audited
source/owner boundary and keeps that owner's natural byte size:

![Main-image code coverage box tree in purple with retained assembly in orange.](assets/readme/gs1-en-core.svg?v=9e6e9812)

**Code overlays** — 795,046 executable bytes across 96 decoded RAM-loaded
modules. Each module is a real parent containing its exact-C owners,
source-backed semantic owners, and contiguous unowned assembly runs:

![Decoded code-overlay coverage box tree in the cyan band; brightness is completion.](assets/readme/gs1-en-overlays.svg?v=83611569)

**Data / assets** — 7,298,755 bytes of the ROM image that are data, not code.
Fifty-two real manifest families wrap their built leaf kinds and 2,335 disjoint,
byte-verified ROM objects. The magenta ladder shows representation maturity:
faint is encoded bytes, a third is b&w sheets, two thirds is coloured sheets,
and full blaze is assets cut into individual objects:

![Asset maturity box tree in the magenta band; brightness is maturity.](assets/readme/gs1-en-assets.svg?v=9fbd282c)

These are the same complete SVG panels used by the live dashboard: title,
hierarchical byte tree, and left-aligned legend are all inside each artifact.
There is no separate HTML-authored graph chrome to drift from the README.
Bright color is byte-exact C, the middle tone is reviewed semantic C, gray is
ordinary assembly debt, and orange is reviewed permanent assembly. Leaf gutters
are zero; only real parent folders have outlines.

Exact and semantic ownership come directly from tracked evidence. Orange uses
the latest verified full-build assembly manifest, so regenerate the publication
snapshot after `bun run verify`:

```sh
bun run coverage
```

For a hands-off working view, run:

```sh
bun tools/dashboard_server.ts
```

The local dashboard scans the actual worktree, rebuilds its three graphs in
memory, and pushes changes to the browser as source, evidence, or the verified
assembly or asset manifest changes. It does not need `bun run coverage`; that command
updates the checked-in publication snapshot. `main` remains the authoritative
integrated tree.

The measured totals live in
[`metrics/gs1-en-coverage-map.json`](metrics/gs1-en-coverage-map.json). The
exact-C numbers behind the picture are reconciled against
`metrics/gs1-en-progress.json` from the same source revision, and a disagreement
is an error rather than a redrawn picture. Semantic coverage is drawn beside
the headline metric rather than folded into it.

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
main image and decoded code overlays.

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
