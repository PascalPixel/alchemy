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

> ### Project completion
>
> 🟧 **Not honestly scoreable yet.**
>
> 🟩 Source ownership — 100% of ROM addresses have a tracked producer<br>
> 🟨 Code decompilation — about 6.1% of identified executable work is exact C<br>
> 🟧 Asset quality and repository organization — semantic audit in progress
>
> 🟩 done · 🟨 active · 🟧 unsure/auditing · 🟥 blocked · ⬜️ undone
>
> The often-quoted 94.08% only measures byte reconstruction. It does not measure
> pret-level completion or time remaining. See [the technical status](STATUS.md)
> for the full breakdown.

## Explore what has been recovered

- Browse [character animation art](assets/graphics/characters/chr), including party
  members, NPCs, monsters, and field sprites.
- Explore [map sources](assets/maps) and recovered world objects such as the
  [golden doorway](assets/graphics/map/resource_152/objects/ougonmon.png),
  [giant tree stump](assets/graphics/map/resource_158/objects/kirikabu.png), and
  [rock formations](assets/graphics/map/resource_184/objects/iwa.png).
- See reconstructed [battle-effect graphics](assets/graphics/battle/effects)
  and [battle-menu art](assets/data/sentou_menu).
- Browse the [music sequences](assets/audio/midi),
  [instrument samples](assets/audio/waves), and recovered audio-engine data.
- Explore the [message, font, and staff-roll sources](assets/text).

These are canonical build inputs, not screenshots or disposable extraction
previews: every asset is meant to encode back into its exact place in the game.

## Where the project stands

The canonical English Golden Sun ROM layout is 100% represented by
reconstructed source and builds byte-identically without a ROM fallback. That
does not make the whole project 100% complete. A large body of
byte-verified assembly will still have to become matching C, the other eleven
approved editions remain future build targets, and round-tripping asset
containers still need a semantic quality audit. Exact encoding is necessary,
but a mechanically wrapped or fragmented source image is still provisional
work.

See [STATUS.md](STATUS.md) for the live byte counts, what has been recovered,
what remains, and how verification works.

## Building and contributing

You can run every source-only structural check without a ROM:

```sh
bun run test
bun tools/build_full.ts --source-only
```

The current private verification build requires your own legally obtained
English Golden Sun ROM and the matching compiler described in the technical
documents. Later targets will require the corresponding legally obtained ROM
only for local verification. ROMs, playable builds, private comparison reports,
toolchains, and generated build products are never tracked here.

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
