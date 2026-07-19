# Alchemy — Golden Sun decompilation

An all-AI, for-fun attempt at decompiling Golden Sun (GBA). The reconstruction
and tooling in this repository are being written collaboratively with AI coding
agents—Anthropic's Claude and OpenAI's Codex—as an experiment in how far they
can take a clean-room decompilation together. It is a hobby/research project,
not a serious or community-driven decomp, and it is nowhere near complete.

```text
Project completion — audit in progress
[????????????????????] score withheld until every quality gate is measurable

Measured milestones
[###################.]  99.57% ROM addresses owned by source
[##################..]  93.70% byte reconstruction
[####################] 100.00% remaining assembly classified
[#...................]   5.68% identified executable work moved to C
[????????????????????] asset semantics audit pending
[????????????????????] repository organization audit pending
```

<sub>“Project completion” means a byte-identical, pret-like source tree with
clean code, coherent source art and audio, and a flat, intentional repository
layout. No overall percentage is published until semantic asset maturity and
repository organization have auditable criteria. Byte reconstruction only
measures deterministic round trips; it does not certify that an atlas, map,
sequence, or archive is already presented in its final human-readable form.</sub>

<p align="center">
  <img src="assets/readme/djinn_101_idle.gif" width="128" height="128" alt="Recovered Djinn battle idle loop from graphics resource 101">
  <img src="assets/readme/djinn_102_idle.gif" width="128" height="128" alt="Recovered Djinn battle idle loop from graphics resource 102">
  <img src="assets/readme/djinn_103_idle.gif" width="128" height="128" alt="Recovered Djinn battle idle loop from graphics resource 103">
  <img src="assets/readme/djinn_104_idle.gif" width="128" height="128" alt="Recovered Djinn battle idle loop from graphics resource 104">
</p>

The name comes from the moment alchemy is released upon Weyard. This project
is similarly trying to release the game from one finished cartridge image into
code, art, maps, and music that people can explore.

It is not a remake, ROM hack, emulator, or game download. The long-term target
is a byte-perfect reconstruction of the English GBA release. The repository is
still in active decompilation and does not yet provide a standalone playable
game.

## Explore what has been recovered

- Browse [character animation art](assets/graphics/chr), including party
  members, NPCs, monsters, and field sprites.
- Explore [map sources](assets/maps) and recovered world objects such as the
  [golden doorway](assets/graphics/resource_152/objects/ougonmon.png),
  [giant tree stump](assets/graphics/resource_158/objects/kirikabu.png), and
  [rock formations](assets/graphics/resource_184/objects/iwa.png).
- See reconstructed [battle-effect graphics](assets/graphics/battle_effect_data)
  and [battle-menu art](assets/data/sentou_menu).
- Browse the [music sequences](assets/audio/sequences),
  [instrument samples](assets/audio/waves), and recovered audio-engine data.
- Explore the [message, font, and staff-roll sources](assets/text).

These are canonical build inputs, not screenshots or disposable extraction
previews: every asset is meant to encode back into its exact place in the game.

## Where the project stands

The ROM layout is 99.57% represented by reconstructed source, but that is not
the same as being 99.57% complete. A large body of byte-verified assembly still
has to become matching C, a smaller set of unowned data regions still needs a
proper source representation, and round-tripping asset containers still need
a semantic quality audit. Exact encoding is necessary, but a mechanically
wrapped or fragmented source image is still provisional work.

See [STATUS.md](STATUS.md) for the live byte counts, what has been recovered,
what remains, and how verification works.

## Building and contributing

You can run every source-only structural check without a ROM:

```sh
bun run test
bun tools/build_full.ts --source-only
```

A complete private verification build requires your own legally obtained
English Golden Sun ROM and the matching compiler described in the technical
documents. ROMs, playable builds, private comparison reports, toolchains, and
generated build products are never tracked here.

Alchemy is guided by Pascal Pixel, is not affiliated with or endorsed by
Nintendo or Camelot, and is non-commercial.

## Guide to every Markdown file

- [README.md](README.md) — the fan-facing project introduction you are reading.
- [STATUS.md](STATUS.md) — current reconstruction totals, recovered systems,
  remaining work, build commands, and the definition of completion.
- [AGENTS.md](AGENTS.md) — evidence, clean-room, verification, and contribution
  rules followed by coding agents.
- [ALCHEMY_GCC.md](ALCHEMY_GCC.md) — the private historical compiler bundle and
  native Apple Silicon build environment.
- [ASSEMBLY.md](ASSEMBLY.md) — which assembly remains, why it remains, and which
  regions are still C-decompilation work.
- [ASSETS.md](ASSETS.md) — how graphics, maps, text, audio, archives, and other
  game data become canonical source assets.
- [NAMING.md](NAMING.md) — the period-authentic Japanese naming and comment
  conventions used by the reconstruction.
- [PUBLICATION.md](PUBLICATION.md) — the source-only publication boundary and
  the material that must never enter the public repository.
- [assets/README.md](assets/README.md) — the asset tree, manifests, encoders, and
  exact round-trip requirements.
- [assets/audio/README.md](assets/audio/README.md) — the reconstructed Golden Sun
  sound engine, sequences, instruments, and wave data.
