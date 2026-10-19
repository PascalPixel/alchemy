# The Lost Age: game

This folder holds The Lost Age's game rules and their tables: units and
characters, items and inventories, the Djinn, the party and the game flags.
☀️'s [Game rules](../../../tbs/src/game/README.md) explains the tables, the
flags and the inventory slots; this page says only what differs here.

## What differs from ☀️

- **Layout.** The tables sit with the family that reads them, not in one
  folder: `item/items.tsv`, `inventory/actions.tsv`, `djinn/djinn.tsv`, and
  the characters, classes, enemies, summons and level tables under
  `character/`. Each is still a `.tsv` with its `.s` beside it, built the same
  way. The code is one function or small family to a file.
- **Contents.** The tables are ⚓️'s own: its items, abilities, Djinn,
  enemies and classes, with ids and message numbers of its own, so a ☀️ id
  is not a ⚓️ id.
- **Compiler.** K2's options on top of ☀️'s (`CFLAGS_tla` in the
  `Makefile`): `-mthumb-split-constants`, `-mthumb-call-via-lr` and
  `-mtune=arm9tdmi`, without `-mthumb-interwork`.
- **Names.** A function that does what its ☀️ twin does takes its name
  (`Inventory_Discard`), so ☀️'s guide applies to it as written.

## Status

Functions not yet matched are drafts under `recon/tla/en/main`, by address.
Some files are still named by address (`character/add5c.c`,
`character/d00c6604.s`). `make progress` gives the counts.

## Evidence and open questions

- XXX: what `item/encode_banked_id.c` encodes, and whether ⚓️'s item ids are
  wider than ☀️'s 9 bits.
