# Game rules

This folder holds The Broken Seal's game rules and the tables they read: the
party's units and their stats, a new game and the prologue, items and
inventories, Psynergy and levelling up, the game flags, enemy units, the
party's coins and counters, element levels and classes, battle arithmetic and
the Djinn.

## Files

| File | What it holds |
| --- | --- |
| `game.c` | the guide to the game object: it includes `game_internal.h`, then the parts in `game/` in ROM order |
| `game_internal.h` | the object's includes, and `Inventory_CompactSlots`, which two parts use |
| `starting_equipment.c` | the characters given starting equipment |
| `*.tsv` with the `.s` beside them | the definition tables: actions, items, characters, classes, enemies, summons, Djinn, element presets and levels, experience |

Each `.tsv` is a table with one row per entry and a `name:type` header; the
build writes it to a `.table` beside it, which its `.s` places with its label
(`classes.tsv` becomes `gClass_DefinitionTable`). Edit the `.tsv`; a column
named `unknown_` or `pad_` is a field whose meaning is not known yet.

The guide's comment says where to start, and each part's second line says
what it holds: `head -2 game/*.c` lists them. A part includes the private
header, so it resolves when opened on its own, but it is compiled only through
`game.c`. The ROM shows the game rules as one object; the parts are ours, cut
where one family ends and the next begins. Link order is in
`games/tbs/objects.tsv`, from `game`.

The unit headers are in `games/tbs/include/unit/game/`. Code in other banks
reaches these functions through the far-call table
`games/tbs/src/system/far_call/GAME.S` (the `...Far` names), so a rename
crosses folders.

## Words used here

- **owner** and **unit**: an owner is a number, a unit the `struct
  BattleUnit` record `UnitGet(owner)` returns. 0 to 7 are the playable
  characters (`ISAAC` to `JENNA` and the rest of `character_ids.h`), kept in
  the game state; 0x80 and up are the enemies of the current battle.
- **party** and **active owners**: the characters travelling with the party,
  in order (`gGameState.activeOwners`, `Party_CountActiveOwners`).
- **inventory entry**: a halfword of a unit's 15 inventory slots: the item id
  in its low 9 bits (`ITEM_ID_MASK`), then `INVENTORY_EQUIPPED`,
  `INVENTORY_BROKEN` and the quantity less one from
  `INVENTORY_QUANTITY_SHIFT`.
- **element**: 0 Venus, 1 Mercury, 2 Mars, 3 Jupiter (`ELEMENT_VENUS` and the
  rest). `Unit_GetElementLevels` gives each element level times ten.
- **Djinn** `group` and `index`: a Djinni is its element (0 to 3) and its
  number within it (0 to 19). A unit holds it, and it is set (on the unit) or
  on standby, waiting to be set again.
- **class series**: the classes a character's two strongest elements select;
  the class is the last in the series whose element levels the character
  reaches (`Class_Determine`).
- **flag**: one of 4096 bits in `gGameFlagBytes`, by number; byte and nibble
  flags use the same numbering (`flags.c`). Named flags are in `flags.h`.
- `unk<offset>`: a field whose meaning is not known yet.

## Prefixes

| Prefix | What it acts on |
| --- | --- |
| `Unit_`, `UnitGet`, `UnitCalc`, `UnitRatio`, `Owner_` | one unit: stats, HP and PP, Psynergy, levels, elements |
| `Party_`, `GameState_`, `Game_` | the party and the game state |
| `Item_`, `Inventory_`, `PartyInventory_`, `InventorySnapshot_`, `ItemCounter_`, `Equipment_` | items: one unit's inventory, the party's, and the snapshot the Colosso takes |
| `Character_`, `Class_`, `Enemy_`, `Summon_`, `SummonDefinition_`, `BattleAction_` | one entry of a definition table |
| `GameFlag_` | the game flags |
| `Djinn_`, `DjinnRecovery_` | the Djinn, and their standby lists |
| `Battle_`, `BattleRandom`, `BattleTarget_`, `BattleFx_` | battle arithmetic the battle folder calls |
| `BattleUnitSet` | filling an enemy unit at the start of a battle |

## Spellings

Each site keeps the form it matches with:

- A unit as `UnitGet(owner)`, or as the far-call `UnitGetFar(owner)` from
  other banks.
- An element pair array as `s16 [4][2]` where the matching code indexes it, or
  through `struct BattleElementStats` with `UBFIX`.
