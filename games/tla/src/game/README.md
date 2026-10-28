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
  [classes.c](../../../tbs/src/game/classes.c)).
- Each file holds a single function or family; some files are still
  named by address (`character/add5c.c`, `character/d00c6604.s`).
