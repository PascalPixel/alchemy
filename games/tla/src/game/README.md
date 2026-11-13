# The Lost Age: game

The Lost Age's game rules and their tables: [units](../../../tbs/glossary.md#unit) and characters,
items and inventories, the [Djinn](../../../tbs/glossary.md#djinn), [the party](../../../tbs/glossary.md#the-party) and
the [game flags](../../../tbs/glossary.md#game-flag). [Game rules](../../../tbs/src/game/README.md)
explains them for The Broken Seal.

## What differs from The Broken Seal

- The tables are C arrays under The Broken Seal's struct names, in the order
  the ROM keeps them, at the end of `game.c`, the bank's one object, after its
  code: the experience table, the artifact slots with the item, ability and
  enemy tables (parts under `artifact_slots/`), and the character, summon,
  class and element tables. Their globals keep this game's names for now
  (`gCharacter_DefinitionTable` for The Broken Seal's
  `gCharacter_DefinitionTable`), and a few readers still view a row through
  a struct of their own (`EnemyRecord`, `OwnerGrowthRecord`).
- An enemy's definition lacks the ten bytes The Broken Seal's keeps before
  its items and ends in two bytes of padding, and a class's abilities are
  held in a halfword, since this game has more than 255.
- `game.c` is The Broken Seal's game object, written as parts under `game/`
  cut where The Broken Seal's are, each function in the part holding its
  twin, and one of them, `flags.c`, missing: the game flags are read through
  `GameFlag_` functions that this game links in its first bank, so they are
  in [The Lost Age: system](../system/README.md) (`system/flags.c`).
- `classes.c` ends with `Unit_RefreshPsynergy`, which here only calls
  `Unit_UpdateClass`; The Broken Seal's function of that name rebuilds the
  Psynergy list, as `Owner_UpdateClassActions` in `levels.c` does here.
