# The Lost Age: game

The Lost Age's game rules and their tables: [units](../../../tbs/glossary.md#unit) and characters,
items and inventories, the [Djinn](../../../tbs/glossary.md#djinn), [the party](../../../tbs/glossary.md#the-party) and
the [game flags](../../../tbs/glossary.md#game-flag). [Game rules](../../../tbs/src/game/README.md)
explains them for The Broken Seal.

## What differs from The Broken Seal

- The tables sit with the family that reads them: `item/items.tsv`,
  `inventory/actions.tsv`, `djinn/djinn.tsv`, and the characters, classes,
  enemies, summons and experience tables under `character/`. Each is still
  TSV, packed by the build into a `.table` file that a `.s` beside it
  includes, where The Broken Seal's are C arrays
  ([item_table.c](../../../tbs/src/game/item_table.c),
  [class_table.c](../../../tbs/src/game/class_table.c)).
- `game.c` is The Broken Seal's game object, written as parts under `game/`
  cut where The Broken Seal's are, each function in the part holding its
  twin, and one of them, `flags.c`, missing: the game flags are read through
  `GameFlag_` functions that this game links in its first bank, so they are
  in [The Lost Age: system](../system/README.md) (`flags/`).
- `classes.c` ends with `Unit_RefreshPsynergy`, which here only calls
  `Unit_UpdateClass`; The Broken Seal's function of that name rebuilds the
  Psynergy list, as `Owner_UpdateClassActions` in `levels.c` does here.
- Some data files are still named by address (`character/d00c6604.s`).
