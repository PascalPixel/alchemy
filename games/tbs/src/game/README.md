# Game rules

The game keeps its rules in one place: who's in the party, what each of them
carries, how they level up and change class, the Djinn they hold, and the
flags that remember the journey.

Nearly every other part of the game comes here to ask or change something. A
scene asks whether a chest has been opened, a shop what an item costs, a menu
what a character's Attack is with their equipment and Djinn, and a battle how
hard a blow lands. The tables all of this reads, every item, ability, Djinni,
enemy, summon and class in the game, are the `.tsv` files in this folder, so
it's a good place to start when you want to know what something in the game
actually is. [Battles](../battle/README.md) follows an ability from these
tables to its damage, and [Shops](../shop/README.md) sell the items.

## Files

| File | What it holds |
| --- | --- |
| `game.c` | the game object itself: it includes `game_internal.h`, then the parts in `game/` in the order their code has in the ROM, and its opening comment says where each subject starts |
| `game_internal.h` | what the parts share: the object's includes, and `Inventory_CompactSlots`, which two parts use |
| [`game/battle_calc.c`](game/battle_calc.c) | battle arithmetic: the attack, Psynergy and healing formulas, the element multiplier, the battle's random numbers, the effect rolls and the weapon unleash |
| [`game/classes.c`](game/classes.c) | element levels and classes |
| [`game/djinn.c`](game/djinn.c) | the Djinn: their definitions, setting them and putting them on standby, and counting them |
| [`game/flags.c`](game/flags.c) | the game flags, read as bits, bytes and nibbles |
| [`game/inventory.c`](game/inventory.c) | items and inventories: equipping, adding, removing, breaking and counting |
| [`game/levels.c`](game/levels.c) | each unit's actions and Psynergy list, the characters, experience and levelling up |
| [`game/party.c`](game/party.c) | enemy units, the active party, coins and counters, and summons |
| [`game/snapshot.c`](game/snapshot.c) | equipping a given item, and the equipment put aside for the Colosso |
| [`game/units.c`](game/units.c) | the party's units and their stats, a new game, the prologue, HP and PP |
| `starting_equipment.c` | the characters who start a new game with their own items equipped, their class and their starting level: Isaac, Garet, Ivan, Mia and Jenna |
| `*.tsv`, each with the `.s` that places it | the definition tables, below |

| Table | One row per | Placed as | Label column |
| --- | --- | --- | --- |
| `items.tsv` | item, by item id (324) | `gItem_DefinitionTable` (`item_table.s`) | `item` |
| `actions.tsv` | ability, by ability id (519) | `gBattleAction_DefinitionTable` (`actions.s`) | `ability` |
| `djinn.tsv` | Djinni, 20 to an element: Venus, Mercury, Mars, Jupiter (80) | `gDjinn_DefinitionTable` (`djinn_table.s`) | `djinni` |
| `enemies.tsv` | enemy, by enemy id (165) | `gEnemy_Definitions` (`enemies.s`) | `enemy` |
| `summons.tsv` | summon (16) | `gSummon_DefinitionTable` (`summons.s`) | none |
| `classes.tsv` | class (203) | `gClass_DefinitionTable` (`classes.s`) | none |
| `chars.tsv` | character (8) | `gCharacter_DefinitionTable` (`chars.s`) | none |
| `elempre.tsv` | enemy element preset (44) | `gEnemy_ElementPresetTable` (`element_table.s`) | none |
| `artifact_slots.tsv` | 16 item ids (columns `x0` to `xf`), so its 32 rows give one byte to each of the 512 item ids: a rare item's counter number plus one, or 0 for an ordinary item | `gItem_ArtifactSlotTable` (`artifact_slots.s`) | none |
| `level_experience_0.tsv` to `level_experience_7.tsv` | level, with the experience it needs; one table per character (99 levels each) | `gCharacter_LevelExpTable` (`experience_table.s`) | none |

Each `.tsv` is a plain table. Its header line names every column as
`name:type`, and then comes one row per entry with no row numbers, so the
first row after the header is entry 0. The type says how big the field is:
`u8` is one byte, a whole number from 0 to 255, `s8` a signed byte from -128
to 127, `u16` and `s16` two bytes, `u32` and `s32` four, and `strN` text of
up to N bytes. The build turns each `.tsv` into a `.table` beside it, which
its `.s` places under its symbol name, such as `gItem_DefinitionTable`; the
`.tsv` is the source and the `.table` only build output. A column named
without a type, such as `item`, is a label for people, and the build writes
nothing for it. A column named `unknown_` or `unk_` is a field whose meaning
isn't known yet, and `reserved_` and `pad_` name bytes that are zero in every
row.

The ROM shows the game rules as a single object, so that's how they're built:
everything is compiled once, through `game.c`. The split into parts is ours,
cut where one family ends and the next begins. Every part starts with
`/// @file` and, on the line after it, a one-line summary of what it holds, so
the first two lines of the parts read together as a table of contents. Each
part includes the private header too, so it still makes sense when you open
it on its own. The object's place in the link order is in
`games/tbs/objects.tsv`, from the `game` row.

The prototypes are in `games/tbs/include/unit/game/`. Code elsewhere in the
ROM, in the other banks, reaches these functions through the far-call table
`games/tbs/src/system/far_call/GAME.S`, under names ending in `Far`, so a
function here can be known by two names, its own and its far-call one.

## Words used here

- **owner** and **unit**: an owner is a number, and a unit is the record that
  number leads to, the `struct BattleUnit` that `UnitGet(owner)` returns,
  with its stats, HP and PP, inventory and Djinn. Owners 0 to 7 are the eight
  characters, kept in the game state: Isaac (`ISAAC`, 0) to Piers (`PIERS`,
  7) in `games/tbs/include/game/character_ids.h`. Owners 128 to 133 are the enemies of the current battle.
- **party** and **active owners**: the characters travelling together, in
  order (`gGameState.activeOwners`, `Party_CountActiveOwners`). Game flags 0
  to 7 say who's in it.
- **inventory entry**: one of a unit's 15 inventory slots, a halfword (two
  bytes) holding the item, whether it's equipped or broken, and how many
  there are. [An inventory slot is one halfword](#an-inventory-slot-is-one-halfword)
  explains the layout.
- **ability**: a row of `actions.tsv`. Psynergy, a Djinni's and a summon's
  effect, an item's use and an enemy's attack are all abilities.
- **element**: 0 Venus, 1 Mercury, 2 Mars, 3 Jupiter (`ELEMENT_VENUS` and the
  rest), and 4 for none. A character's **element levels** come from the Djinn
  set on them plus their own element bonus, and an enemy's from its element
  preset; `Unit_GetElementLevels` gives each level times ten.
- **Djinni** `group` and `index`: a Djinni is its element (0 to 3), which the
  code calls its group, and its number within it (0 to 19). A unit holds it,
  and it's either set on that unit or on standby, waiting to be set again.
  The Djinn a summon uses recover for some turns before they're set again.
- **class series**: the classes a character's two strongest elements select.
  The class is the last in the series whose element levels the character
  reaches (`Class_Determine`). Isaac and Garet as children, and Jenna, have
  fixed classes (200, 201 and 202).
- **flag**: one of the game's 4,096 yes-or-no facts, a bit in
  `gGameFlagBytes`, found by its number. Byte and nibble flags use the same
  numbering (`flags.c`), and the flags with names are in `flags.h`.
- `unk<offset>`: a field whose meaning isn't known yet.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `Unit_`, `UnitGet`, `UnitCalc`, `UnitRatio`, `Owner_` | one unit: stats, HP and PP, Psynergy, levels, elements |
| `Party_`, `GameState_`, `Game_` | the party and the game state |
| `Item_`, `Inventory_`, `PartyInventory_`, `InventorySnapshot_`, `ItemCounter_`, `Equipment_` | items: one unit's inventory, the party's, the snapshot the Colosso takes, and the rare item counters that limit what the shops sell |
| `Character_`, `Class_`, `Enemy_`, `Summon_`, `SummonDefinition_`, `BattleAction_` | one entry of a definition table |
| `GameFlag_` | the game flags |
| `Djinn_`, `DjinnRecovery_` | the Djinn, and their standby lists |
| `Battle_`, `BattleRandom`, `BattleTarget_`, `BattleFx_` | battle arithmetic the battle folder calls |
| `BattleUnitSet` | filling an enemy unit at the start of a battle |
| `Runtime_` | two engine helpers that live here: a byte copy and the build stamp |

[Matching](../MATCHING.md) explains the odd spellings and the comment markers.

## Why it works this way

### A row's place is its id, and its name is the message at the same place

The definition tables have no id column. An item is the row it sits on, and
the same number is its place among the messages too. Its name is message
`MsgItemName` + id, and the short line the item screen shows about it is
`MsgItemPlainName` + id: the Broad Sword's just says Long Sword, its kind of
weapon, and the Healing Ring's says Ring: Use to restore 70 HP. Code reaches
both by adding the id to the first message's number (`itemId + MsgItemName`
in the shops and the menus). Abilities, enemies and Djinn work the same way,
each with its own first message.

So the tables and the message catalogs are parallel lists tied only by
position. A row inserted in the middle of `items.tsv` would move every later
item one place along, and each would show the name of the item that used to
follow it, unless the catalogs of all six languages gained a message at the
same place. That's also why rows no item uses keep their places, with `?` or
`=` as their names.

The label columns are the English edition's names from
`games/tbs/text/en.po`, by the message each row's id reaches. An item's name
is message 386 + id (`MsgItemName`), an enemy's 655 + id (`MsgEnemyName`),
an ability's 819 + id (`MsgAbilityName`), and a Djinni's 1119 + its row
(`MsgDjinnName`), which is also its ability, 300 + row. A row's id is its
line number less two, since the header is line 1 and entry 0 is line 2:
`Healing Ring` is on line 267 of `items.tsv`, so it's item 265 and its name
is message 651. In the same way `Quake` is ability 3 in `actions.tsv`,
`Flint` row 0 of `djinn.tsv` and `Vermin` enemy 1 in `enemies.tsv`. A row
whose message is `?` or `=` is unused, and the labels keep those marks; a row
past the last name message says `(no name message)`. `item_ids.h`,
`ability_ids.h` and `djinn_ids.h` name the ids the code uses.

### Flags are bits that can also be read as bytes

The game remembers 4,096 yes-or-no facts about the journey: whether a chest
has been opened, a boss beaten, a Djinni found. Each one is a flag, and each
flag is a bit, a single on-or-off. Eight bits make a byte, so all the flags
together fill 512 bytes, the array `gGameFlagBytes`. Flags 48 to 127, for
example, are the 80 Djinn's found flags: Djinni n of element e is flag
48 + 20 × e + n (`FLAG_DJINNI_0`). A chest has one of its own, such as
`FLAG_ALTIN_PEAK_LUCKY_MEDAL`, and so does a boss, such as
`FLAG_TRET_DEFEATED`.

A flag's number says where it lives. Its low three bits, a number from 0 to
7, pick the bit within a byte, and the next nine bits, 0 to 511, pick the
byte (`game/flags.c`). Anything above that is dropped (`GAME_FLAG_BYTE`), so
a number past 4,095 wraps round onto a flag below it instead of reaching
other memory. That layout means eight flags in a row, starting at a multiple
of 8, are also one byte, and the byte calls (`GameFlag_GetByte`,
`GameFlag_SetByte`, `GameFlag_IncrementByte`) read and write it whole; the
nibble calls (`GameFlag_GetNibble`, `GameFlag_SetNibble`) do the same with
four flags. So a small counter can live among the flags, which is how the
Colosso scenes, the world map and the Battle Arena's win counts keep theirs,
and it follows the flags' rules. One kept in a scene's own range
(`FLAG_SCENE_FIRST` to `FLAG_SCENE_LAST`) is cleared with the rest on every
ordinary entrance to the scene (see [Scenes](../scene/README.md)), and all of
them are saved with the game.

The save file's summary counts how many of flags 48 to 127 are set
(`SaveState_BuildSummaryHeader`), which is how many Djinn the file has found.
The title menu offers Battle only when some file has found one
(`gTitleExtraOptionEnabled`), and the save menu's `SAVE_MENU_EXTRA` mode lists
only those files (`window/battle_command/save.c`).

### An inventory slot is one halfword

Each of a unit's 15 inventory slots is a halfword, two bytes or 16 bits,
packed like this:

```
bits 15 to 11   how many there are, less one
bit 10          broken
bit 9           equipped
bits 8 to 0     the item id
```

The low nine bits hold the item's number, 0 to 511 (`ITEM_ID_MASK`).
Bit 9 says it's equipped (`INVENTORY_EQUIPPED`), bit 10 that it's broken
(`INVENTORY_BROKEN`), and the five bits from
bit 11 up hold how many there are, less one (`INVENTORY_QUANTITY_SHIFT`); a
stack of a stackable item grows to 30. So an item, its state and its stack
move together in one load and one store, and a slot of 0 is empty, which
`Inventory_CompactSlots` relies on when it closes the gaps. Code that asks
which item a slot holds masks it with `ITEM_ID_MASK` first, since the same
item equipped, broken or stacked is a different halfword.

## What an item's row holds

An item is its row of `items.tsv` (`struct ItemDefinition` in
`games/tbs/include/game/item.h`). The Healing Ring, row 265, makes a good
example to read along with: it costs 800 coins, anyone can wear it, it's a
rare ring, and used in battle it casts Cure, though it may break.

| Column | What it holds |
| --- | --- |
| `price` | the price, in coins |
| `type` | what kind of item it is (`enum ItemType`): 0 for anything that isn't equipment, then 1 weapons, 2 armor and clothing, 3 shields, gloves and armlets, 4 headgear, 5 boots, 6 the items that teach Psynergy, 8 rings and 9 shirts; a unit equips one of each type except the Psynergy items |
| `flags` | `enum ItemFlags`: cursed (1), can't be taken off (2), rare (4), never dropped (8) and stackable (0x10, 16) |
| `equip_mask` | who can equip it, one bit per character with bit 0 for Isaac (`character_ids.h`): the Long Sword's 0x13 is bits 0, 1 and 4, so Isaac, Garet and Felix, and the Healing Ring's 0xff is all eight |
| `icon` | its icon in the menus (`gUiIcon_ItemIconPointers`) |
| `primary_bonus`, `secondary_bonus` | what it adds to Attack and Defense while equipped (`UnitCalc`, `game/units.c`) |
| `unleash_action`, `unleash_chance` | what a weapon unleashes, an ability id, at `unleash_chance` times 5 percent |
| `use_type` | what using it costs (below) |
| `element` | a weapon's attack element, 4 for none |
| `effect0_kind` and `effect0_amount` to `effect3_` | up to four effects while it's equipped; the kinds are `enum ItemEffectKind` in `item.h` (`ITEM_EFFECT_ADD_HP` is 1, `ITEM_EFFECT_SCALE_ATTACK` 11) |
| `action_id` | what using it does, an ability id (below) |

Its name is message `MsgItemName` + id in the PO catalogs
(`games/tbs/text`), and the line the item screen shows about it is
`MsgItemPlainName` + id.

## How an item reaches an ability

An item names an ability by its row in `actions.tsv`, in two columns.

`action_id` is what the item does when it's used. In battle
(`BattleCommand_BuildPlan`, `../battle/battle/plan.c`) the item command plans
that ability, and `use_type` says what the use costs: 1 one is used up, 2 it
may break (one time in eight), 4 it leaves an Empty Bottle. With `use_type` 3
the ability is instead taught while the item is equipped, as Psynergy
(`game/levels.c`): those are the Orb of Force and the rest of `type` 6.

`unleash_action` is what a weapon unleashes in place of an attack
(`RollWeaponUnleash`, `game/battle_calc.c`).

The ability's row of `actions.tsv` then gives its element, power, range and
kind, and [Battles](../battle/README.md#how-an-ability-resolves) follows it
to the damage.

## Limits

- Item ids are 9 bits, 0 to 511 (`ITEM_ID_MASK`). `items.tsv` has 324 rows,
  and rows 269 to 323 have no name message of their own.
- A unit carries 15 inventory slots (`INVENTORY_SLOTS`), and a stack holds 30
  at most.
- 20 Djinn to an element; 519 abilities; 165 enemies, the last of them with
  no name message.
- 4,096 game flags.
- Levels go up to 99.
- 128 rare item counters, each 0 to 99 (`gItemCounters`, `ItemCounter_Adjust`).
  A counter is how many of its rare item the shops may still sell: buying one
  takes one off, and selling or discarding one puts it back
  (`Item_AdjustCounter`).

## Evidence and open questions

The tables' label columns are the English edition's names from
`games/tbs/text/en.po`, by the message each row's id reaches. The columns are
named from what the code reads in them, and the item effect kinds from what
`UnitCalc` does with each. `item_ids.h`, `ability_ids.h` and `djinn_ids.h`
name only the ids the code uses. Flags 48 to 127 are the Djinn's found flags
because `Djinn_AddToLeastLoadedOwner` and the field's Djinni effect set flag
48 + 20 × element + index when a Djinni joins.

Still open:

- XXX: the `unknown_` and `unk_` columns of the tables, and what rows 269 to
  323 of `items.tsv` were for, as they have no name message.
