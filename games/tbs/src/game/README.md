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
| `game/battle_calc.c` | damage and success rates: the attack, Psynergy and healing formulas, the element multiplier, the effect rolls and the weapon unleash |
| `starting_equipment.c` | the characters given starting equipment |
| `*.tsv` with the `.s` beside them | the definition tables (below) |

| Table | One row per | Placed as | Label column |
| --- | --- | --- | --- |
| `items.tsv` | item, by item id (324) | `gItem_DefinitionTable` (`item_table.s`) | `item` |
| `actions.tsv` | ability, by ability id (519) | `gBattleAction_DefinitionTable` (`actions.s`) | `ability` |
| `djinn.tsv` | Djinni, 20 to an element: Venus, Mercury, Mars, Jupiter (80) | `gDjinn_DefinitionTable` (`djinn_table.s`) | `djinni` |
| `enemies.tsv` | enemy, by enemy id (165) | `gEnemy_Definitions` (`enemies.s`) | `enemy` |
| `summons.tsv` | summon (16) | `summons.s` | none |
| `classes.tsv` | class (203) | `classes.s` | none |
| `chars.tsv`, `elempre.tsv`, `artifact_slots.tsv`, `level_experience_*.tsv` | character, element preset, artifact slot, level | their `.s` | none |

Each `.tsv` has a `name:type` header, one row per entry, and no row numbers:
the first row after the header is entry 0. The build writes it to a
`.table` beside it, which its `.s` places with its label. Edit the `.tsv`.
A column named without a type, such as `item`, is a label for people: the
build skips it and writes nothing for it. A column named `unknown_` or
`unk_` is a field whose meaning is not known yet; `reserved_` and `pad_`
name bytes that are zero in every row.

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

## Words

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
- **ability**: a row of `actions.tsv`; Psynergy, a Djinni's and a summon's
  effect, an item's use and an enemy's attack are all abilities.
- **element**: 0 Venus, 1 Mercury, 2 Mars, 3 Jupiter (`ELEMENT_VENUS` and the
  rest), 4 none. `Unit_GetElementLevels` gives each element level times ten.
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

## How to find a row

Search the table for the English name in its label column: `Healing Ring`
in `items.tsv`, `Quake` in `actions.tsv`, `Flint` in `djinn.tsv`, `Vermin` in
`enemies.tsv`. The row's id is its line number less two (the header is line
1, entry 0 is line 2). The labels are the English edition's names from
`games/tbs/text/en.po`: an item's name is message `MsgItemName` + id (386 +
id), an enemy's `MsgEnemyName` + id (655 + id), an ability's
`MsgAbilityName` + id (819 + id), and a Djinni's `MsgDjinnName` + its row
(1119 + row), which is also its ability, 300 + row. A row whose message is
`?` or `=` is unused, and the labels keep those marks; a row past the last
name message says `(no name message)`. `item_ids.h`, `ability_ids.h` and
`djinn_ids.h` name the ids the code uses.

## How to change an item

An item is its row of `items.tsv` (`struct ItemDefinition` in
`games/tbs/include/game/item.h`):

- **Price**: `price`, in coins.
- **Who can equip it**: `equip_mask`, one bit per character, bit 0 for
  Isaac (`character_ids.h`).
- **What it adds to Attack and Defense while equipped**: `primary_bonus`
  and `secondary_bonus` (`UnitCalc`, `game/units.c`).
- **What it does while equipped**: up to four effects, `effect0_kind` and
  `effect0_amount` to `effect3_`; the kinds are `enum ItemEffectKind` in
  `item.h` (`ITEM_EFFECT_ADD_HP` is 1, `ITEM_EFFECT_SCALE_ATTACK` 11).
- **What a weapon unleashes**: `unleash_action`, an ability id, at
  `unleash_chance` times 5 percent.
- **What using it does**: `action_id`, an ability id; see below.
- **Its name**: message `MsgItemName` + id in the PO catalogs
  (`games/tbs/text`); the line the item screen shows about it is
  `MsgItemPlainName` + id.

## How an item reaches an ability

An item names an ability by its row in `actions.tsv`, in two columns:

- `action_id` is what the item does when used. In battle
  (`BattleCommand_BuildPlan`, `../battle/battle/plan.c`) the item command
  plans that ability; `use_type` says what the use costs: 1 one is used up,
  2 it may break (one time in eight), 4 it leaves an Empty Bottle. With
  `use_type` 3 the ability is instead taught while the item is equipped, as
  Psynergy (`game/levels.c`; the Orb of Force and the rest, `type` 6).
- `unleash_action` is what a weapon unleashes in place of an attack
  (`RollWeaponUnleash`, `game/battle_calc.c`).

The ability's row of `actions.tsv` then gives its element, power, range and
kind; `../battle/README.md` follows it to the damage.

## Limits

- Item ids are 9 bits (`ITEM_ID_MASK`); `items.tsv` has 324 rows, of which
  rows 269 to 323 have no name message of their own.
- A unit carries 15 inventory slots (`INVENTORY_SLOTS`).
- 20 Djinn to an element; 519 abilities; 165 enemies, of which the last has
  no name message.
- 4096 game flags.
