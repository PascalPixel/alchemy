# Game rules

![One number, three lists. The Healing Ring is item 265, and that one number finds it in three lists kept in the same order: row 265 of the item table, 800 coins, a ring, uses Cure; entry 265 of the item names, Healing Ring; and entry 265 of the Item screen's lines, Ring: Use to restore 70 HP. The Sleep Ring at 264 and the Unicorn Ring at 266 line up the same way. Only the position ties the lists together, so a row put into one list would shift all the later names.](https://alchemy.pascalpixel.com/illustrations/game/row-id.svg)

Everything the game needs to know about the [party](../../glossary.md#the-party) lives here: who is in it,
each character's level, HP and stats, the items each one carries, the [Djinn](../../glossary.md#djinn)
they hold, and the flags that remember what has happened in the story. So do
the tables that describe every item, ability, Djinni, enemy, [summon](../../glossary.md#summons) and
class. The rest of the game asks this code its questions: a [map](../../glossary.md#map) whether a
chest has been opened, a shop what an item costs, a menu what a character's
Attack would be with another sword, and a battle what a blow does.
[Battles](../battle/README.md) follows an ability from these tables to its
damage.

Two words run through the page. A character's stats, HP
and [PP](../../glossary.md#psynergy-and-pp), items and Djinn make up a unit, a
`struct Unit`; the same struct holds each enemy in a battle. The
number that picks a unit is its unit id: `ISAAC` (0) to `PIERS` (7) for the
eight characters of both games, and from `FIRST_ENEMY_UNIT` (128) for the
current battle's enemies.

The code is its own [bank](../../glossary.md#bank) of the main image, the
`game` bank, so code elsewhere calls it through
[far calls](../../glossary.md#far-call), by names ending in `Far`:
`Unit_GetFar` leads to `Unit_Get`.

## How one number finds a row and its name

The Healing Ring costs 800 coins, heals like Cure when used, and is called
"Healing Ring" on screen. Those facts sit in different places, the price in
the item table and the name in the game's text, and the one thing that joins
them is a number: the ring is item 265, the 265th row of one list and the
265th entry of the others.

In the code, the definition tables have no id column: an entry's id is its
row, the first row after the header being entry 0. Every line of the game's
text is a message with a number, and each table's names are a run of
messages in the same order, starting at a message named for the run:

```
name message = MsgItemName + item id
             = 386 + 265 = 651 for the Healing Ring
```

The same holds for the Item screen's line about an item
(`MsgItemPlainName` + id), an enemy's name (`MsgEnemyName` + id), an
ability's name and description (`MsgAbilityName` and
`MsgAbilityDescription` + id) and a Djinni's name (`MsgDjinnName` + its
row). A Djinni's row also gives its ability, `ABILITY_DJINNI_FIRST` + row.
Code reaches a name by adding (`itemId + MsgItemName` in the shops and
menus).

The tables and the message catalogs (the game's text, one catalog for each
of the six languages) are therefore parallel lists tied only by position. A
row inserted into `items.tsv` would give every later item the name of the
one after it unless all six catalogs gained a message at the same place.
Rows no item uses stay in the table as placeholders, so that later ids
don't move. The Healing Ring is on line 267 of `items.tsv` (line 1 is the
header and line 2 is item 0), so it is item 265; its name is message 651
and its line "Ring: Use to restore 70 HP".

Each table is a text file of tab-separated columns (`.tsv`) that the build
turns into the bytes the game reads. Its header line gives each column a
name and says how much room the column takes, written `name:type`: `u8` is
one byte holding a number from 0 up, `s8` one byte that may be negative,
`u16` and `s16` two bytes, `u32` and `s32` four, and `strN` is text of up to
N bytes. The build's `ags-graphics` packs each row into those bytes and
writes the result as a `.table` beside the `.tsv`; the matching `.s` file
places it with `.incbin` under its symbol, the name code uses for it
(`gItem_DefinitionTable: .incbin "games/tbs/src/game/items.table"`). A
column without a type (`item`, `ability`,
`djinni`, `enemy`) is a label for readers and builds no bytes: the English
name its message gives, `?` or `=` where the message marks the row unused,
or `(no name message)` for a row past the last name. `reserved_` and `pad_`
columns are zero in every row, and `unknown_` and `unk_` columns are fields
whose meaning isn't known. `item_ids.h`, `ability_ids.h` and `djinn_ids.h`
name the ids the code uses.

| Table | One row per | Placed as |
| --- | --- | --- |
| `items.tsv` | item (324) | `gItem_DefinitionTable` (`item_table.s`) |
| `actions.tsv` | ability (519) | `gAbilityDefinitions` (`actions.s`) |
| `djinn.tsv` | Djinni, `DJINN_PER_ELEMENT` (20) to an [element](../../glossary.md#the-four-elements), Venus, Mercury, Mars, Jupiter (80) | `gDjinn_DefinitionTable` (`djinn_table.s`) |
| `enemies.tsv` | enemy (165) | `gEnemy_Definitions` (`enemies.s`) |
| `summons.tsv` | summon (16) | `gSummon_DefinitionTable` (`summons.s`) |
| `classes.tsv` | class (203) | `gClass_DefinitionTable` (`classes.s`) |
| `chars.tsv` | character (8) | `gCharacter_DefinitionTable` (`chars.s`) |
| `elempre.tsv` | enemy element preset (44) | `gEnemy_ElementPresetTable` (`element_table.s`) |
| `artifact_slots.tsv` | 16 item ids, columns `x0` to `xf` (32 rows, 512 ids): a rare item's counter number plus one, or 0 (artifact is the code's word for an item with a counter; [Rare item counters](#rare-item-counters) explains them) | `gItem_ArtifactSlotTable` (`artifact_slots.s`) |
| `level_experience_0.tsv` to `_7.tsv` | level (99), with the experience it needs, one table per character | `gCharacter_LevelExpTable` (`experience_table.s`) |

| Name | Value | Meaning |
| --- | --- | --- |
| `DJINN_PER_ELEMENT` | 20 | Djinn to an element |
| `ABILITY_DJINNI_FIRST` | `0x12c` (300) | the first Djinni's ability; Djinni row n is ability 300 + n |

## An item's row

An item is one row of the item table, and everything the player learns about
it in a shop or on the Item screen comes from that row: its price, who can
equip it, what it adds to Attack or Defense, and what using it does.

![The Long Sword's row of the item table, read column by column. Its price, 200, is what a shop asks in coins. Its type, a weapon, means a character holds one weapon at a time. Its equip mask has one bit for each of the eight characters, Isaac, Garet, Ivan, Mia, Felix, Jenna, Sheba and Piers; the bits for Isaac, Garet and Felix are set, so they can equip it, and for the others the shop says Can't equip. Its first bonus, 14, is added to Attack while it is equipped, before the character's class multiplies it.](https://alchemy.pascalpixel.com/illustrations/game/item-row.svg)

In the game, Bilibin's weapon shop lists the Long Sword at 200 coins. Choosing
who gets it, the hand on Isaac shows his Attack going from 56 with his
Bandit's Sword to 58, and on Ivan the shop says `Can't equip`.

In the code, the row is read as `struct ItemDefinition`
(`games/tbs/include/game/item.h`). The Long Sword's row reads price 200, type
1 (a weapon), flags 0, `equip_mask` `0x13`, icon 1, `primary_bonus` 14, and
element 4, `ELEMENT_NONE`; every other column is 0.

| Column | What it holds |
| --- | --- |
| `price` | the price in coins |
| `type` | `enum ItemType`: `ITEM_TYPE_NONE` for anything that isn't equipment, then weapons, armor, shields, helms, boots, `ITEM_TYPE_PSYNERGY`, rings and shirts; a unit equips one of each type except [Psynergy](../../glossary.md#psynergy-and-pp) items |
| `flags` | `enum ItemFlags`: `ITEM_CURSED`, `ITEM_CANNOT_UNEQUIP`, `ITEM_RARE`, `ITEM_INDISPENSABLE` and `ITEM_STACKABLE` |
| `equip_mask` | one bit per character who can equip it, bit 0 for Isaac: the Long Sword's `0x13` is Isaac, Garet and Felix |
| `icon` | its menu icon (`gItemIcons`) |
| `primary_bonus`, `secondary_bonus` | Attack and Defense added while equipped |
| `unleash_action`, `unleash_chance` | the ability a weapon unleashes, at `unleash_chance` steps of 5 percent (`RollWeaponUnleash`) |
| `use_type` | what using it costs: `ITEM_USE_CONSUMED`, `ITEM_USE_MAY_BREAK` (one time in eight), `ITEM_USE_EMPTIES` (leaves an Empty Bottle), or `ITEM_USE_FREE` |
| `element` | a weapon's attack element, `ELEMENT_NONE` for none |
| `effect0_kind`, `effect0_amount` to `effect3_` | up to four effects while equipped, `enum EquipEffectKind` (`EQUIP_EFFECT_ADD_HP`, `EQUIP_EFFECT_SCALE_ATTACK` and the rest), which `Unit_Calculate` applies |
| `action_id` | the ability the item uses |

`action_id` and `unleash_action` are rows of `actions.tsv`. Used in battle,
the item command builds a turn from its `action_id` (`BattleTurn_Build`,
[battle/battle/plan.c](../battle/battle/plan.c)), and
[Battles](../battle/README.md#how-an-ability-resolves) follows that ability
to its effect. An item whose `use_type` is `ITEM_USE_TEACHES` is never used:
while it is equipped its `action_id` is in the unit's Psynergy list
(`game/levels.c`). These are the eight Psynergy items, from the Orb of Force
on.

The Healing Ring's row shows the pieces together: 800 coins, `type` ring,
`ITEM_RARE`, an `equip_mask` of `0xff` for all eight characters, `use_type`
`ITEM_USE_MAY_BREAK` and `action_id` 90, Cure.

| Name | Value | Meaning |
| --- | --- | --- |
| `ITEM_CURSED` | 1 | equipping it curses the wearer |
| `ITEM_CANNOT_UNEQUIP` | 2 | once equipped it cannot be taken off, or dropped while worn |
| `ITEM_RARE` | 4 | a rare item: the Item screen can call it `A rare item`, a shopkeeper buying it says `What rare stuff!`, and it ranks high among a battle's spoils |
| `ITEM_INDISPENSABLE` | 8 | an important item: the player cannot drop it, no shop buys it, and it ranks first among the items enemies leave |
| `ITEM_STACKABLE` | `0x10` | kept in one inventory entry with a count |
| `ITEM_USE_FREE` | 0 | using it costs nothing, as with equipment |
| `ITEM_USE_CONSUMED` | 1 | one is used up |
| `ITEM_USE_MAY_BREAK` | 2 | it breaks one time in eight |
| `ITEM_USE_TEACHES` | 3 | never used: equipped, it teaches its `action_id` as Psynergy |
| `ITEM_USE_EMPTIES` | 4 | it leaves an Empty Bottle |

## What an inventory slot holds

Each character carries up to 15 things, and the Item screen shows for each
one whether it is equipped, whether it is broken, and how many there are of
a stackable item such as Herbs. All of that fits in one 16-bit number per
slot.

![What an inventory slot holds. Each of a character's slots remembers four things: how many there are, whether the item is broken, whether it is equipped, and which item it is. Three Herbs: three, not broken, not equipped, the Herb. A Long Sword, equipped: one, not broken, equipped, the Long Sword. An empty slot holds nothing at all. A stack grows to 30, and each character has 15 slots.](https://alchemy.pascalpixel.com/illustrations/game/inventory-entry.svg)

In the code, the slot is a halfword, a 16-bit value, split into four parts
from its highest bit down. How many is kept as one less, so that a single
item's count part is 0:

```
bits 15..11   count less one   (INVENTORY_QUANTITY_SHIFT)
bit 10        broken           (INVENTORY_BROKEN)
bit 9         equipped         (INVENTORY_EQUIPPED)
bits 8..0     item id          (INVENTORY_ITEM_ID_MASK)
```

An item, its state and its count move in one load and one store, and an
empty slot is 0, which `Inventory_CompactSlots` relies on to close gaps. A
stack of an `ITEM_STACKABLE` item grows to 30 (`Inventory_AddItem`). Code
that asks which item a slot holds masks it with `INVENTORY_ITEM_ID_MASK` first, since
the same item equipped, broken or stacked is a different halfword. Three
Herbs, item 180, hold 2 in the count part; an equipped Long Sword, item 1,
holds 0 there and has the equipped bit set. The slots
are `Unit.inventory`; `enum InventoryEncoding` in `item.h` names their
parts, and [game/inventory.c](game/inventory.c) reads and writes them.

| Name | Value | Meaning |
| --- | --- | --- |
| `INVENTORY_ITEM_ID_MASK` | `0x1ff` | the item id, 0 to 511 |
| `INVENTORY_EQUIPPED` | `0x200` | the item is equipped |
| `INVENTORY_BROKEN` | `0x400` | the item is broken |
| `INVENTORY_ENTRY_MASK` | `0x7ff` | all but the count: the item with its state |
| `INVENTORY_QUANTITY_SHIFT` | 11 | the first bit of the count less one |
| `INVENTORY_QUANTITY_STEP` | `0x800` | one more in the stack |
| `INVENTORY_QUANTITY_MASK` | `0xf800` | the count less one |
| `INVENTORY_SLOTS` | 15 | the slots a unit has |

## Why stats are always worked out from scratch

When the player points at Remove on Isaac's Machete, the status panel shows
his Attack falling from 24 to 17 before anything is chosen. The game never
adds or subtracts a stat bit by bit: whenever anything that feeds a stat
changes, it works every stat out again from the start, and a menu's preview
is that same calculation, run on Isaac for a moment and then undone.

![How a menu previews a change, pointing at Remove on the Machete. One: copy Isaac aside, every stat and item as he is. Two: take the Machete off the real Isaac, for a moment. Three: work out every stat again. Four: show the new against the old, Attack 24 going to 17. Five: put Isaac back from the copy, so nothing has changed. The preview is exact because it is the same calculation that equipping runs.](https://alchemy.pascalpixel.com/illustrations/game/stats-preview.svg)

![On the Item screen, the hand picks Isaac's Machete and points at Remove, and the status panel shows his Attack falling from 24 to 17](https://alchemy.pascalpixel.com/guides/game/items.gif)

In the code, each character's stats, HP, PP, items and Djinn are kept in a
unit, a `struct Unit`. `Unit_Calculate` ([game/units.c](game/units.c))
works a unit's stats out again from its base stats, equipment, set Djinn, class and battle stat levels,
clamps each and writes them back, keeping current HP and PP at the same
fraction of their maximums. It runs after anything that changes an input:
equipping, setting a Djinni, levelling up, a class change.

A menu that previews a change uses the same path. To show Attack without the
Machete, `StatusPanel_Draw`
([menu/menu/common.c](../menu/menu/common.c))
copies the unit to the [heap](../../glossary.md#heap-and-pools), changes the real unit's equipment, lets
`Unit_Calculate` run, draws the new numbers against the copy and restores the
unit from it. The preview is what equipping would do because it is the same
computation. The Machete adds 6 to Attack, but the panel shows 7 less,
because his class multiplies the sum and the result rounds down. Of the
multipliers in the `atk` column of `classes.tsv`, only 11 tenths (the
Squire's, among others) gives both numbers, from an Attack of 16 before the
class:

```
with the Machete     (16 + 6) * 11 / 10 = 24
without it                 16 * 11 / 10 = 17
```

## The game flags

The game remembers what has happened (a chest opened, a Djinni found, a
scene already played) as yes-or-no flags, 4,096 of them, one bit each, and
eight flags in a row, read together as a byte, also hold small counters.

![A flag is one bit. The 4,096 flags are kept in 512 bytes. Flag 48 is set once Flint, the first Venus Djinni, is found: 48 divided by 8 is byte 6, remainder bit 0. Byte 6 holds flags 48 to 55, Flint, Granite and the Venus Djinn after them. All 80 Djinn have flags 48 to 127, 20 to an element in order, so Fizz, the first Mercury Djinni, is flag 68. Eight flags in a row are one byte, so a byte can hold a number: the Battle Arena's wins live there.](https://alchemy.pascalpixel.com/illustrations/game/game-flags.svg)

In the code, a flag's number says where it is kept:

```
flag number   bits 11..3   its byte of gGameFlagBytes   (GAME_FLAG_BYTE)
              bits 2..0    its bit in that byte
```

`gGameFlagBytes` holds the 4,096 flags in 512 bytes. A flag's low three bits
pick its bit and the next nine its byte; `GAME_FLAG_BYTE` drops the rest, so
a number past 4,095 wraps onto a lower flag instead of reaching other memory.
Eight flags from a multiple of 8 are therefore also one byte, which
`GameFlag_GetByte`, `GameFlag_SetByte` and `GameFlag_IncrementByte` read and
write whole, and four flags one nibble, half a byte (`GameFlag_GetNibble`,
`GameFlag_SetNibble`). Counters live among the flags this way, such as the
Battle Arena's win count (`FLAG_MONSTER_BATTLE_WINS`) and a link battle's
party size (`FLAG_LINK_PARTY_COUNT`), and follow the flags' rules: all are
saved, and one in a scratch range is cleared with it. Two ranges of flags
are scratch space for the map code: a map's own flags, cleared every time
the party enters a map, and a map section's, cleared when it enters another
[map section](../../glossary.md#map-section).
[The front page](../../README.md#game-flags) gives the ranges.

Flags `FLAG_DJINN_FIRST` (48) to 127 are the Djinn's found flags, Djinni n of
element e being flag `FLAG_DJINN_FIRST` + 20 × e + n. The save summary counts
how many are set (`Save_BuildFileSummary`), and the title screen
offers its Battle command only when some save file has found a Djinni
(`gTitleExtraOptionEnabled`).

The flag functions are in [game/flags.c](game/flags.c), `GAME_FLAG_BYTE` in
`game_flags.h` and the named flags in `flags.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `GAME_FLAG_BYTE(flag)` | bits 3 to 11 of `flag` | the byte of `gGameFlagBytes` that holds the flag |
| `FLAG_DJINN_FIRST` | 48 | the first Djinni's found flag; the 80 run to 127 |
| `FLAG_LINK_PARTY_COUNT` | `0x3f0` | a byte: the members the linked opponent sent |
| `FLAG_MONSTER_BATTLE_WINS` | `0x3f8` | a byte: the Battle Arena's win count |

## Rare item counters

A shop never runs out of Herbs, but a rare item such as the Healing Ring can
be sold only as many times as its counter allows, and selling one back to a
shop makes it available again. The rare item counters keep that count.

![Rare item counters. A lookup table gives each item its counter number plus one. The Herb's entry is 0: an ordinary item with no counter, so the shops never run out. The Healing Ring's entry is 96, so its count is counter 95. Buying one takes one off; selling or dropping one puts it back. There are 128 counters, each 0 to 99, one byte each.](https://alchemy.pascalpixel.com/illustrations/game/rare-counters.svg)

`gItemCounters` holds 128 counters, each 0 to 99, for how many of each rare
item the shops may still sell. `gItem_ArtifactSlotTable` gives each item id
its counter number plus one, 0 for an ordinary item. Buying one takes one off
and selling or discarding one puts it back (`Item_AdjustCounter`,
`ItemCounter_Adjust`, in [game/inventory.c](game/inventory.c)).

| Name | Value | Meaning |
| --- | --- | --- |
| `gItemCounters` | 128 bytes | one counter a rare item, 0 to 99 |
| `gItem_ArtifactSlotTable` | 512 entries | an item's counter number plus one, or 0 |

## How it fits with the rest of the game

Code in the other banks calls this one's functions by their far-call names,
and a map's [overlay](../../glossary.md#overlay) through the list of
outside functions it was built with (its [import table](../../glossary.md#entry-table-and-import-table)); some code reads
`gGameState`, the state of the game in progress, directly, as the save
summary does. The battle folder reads `actions.tsv` through
`struct AbilityDefinition` beside its own `actflags.tsv` and `actstat.tsv`, which
choose how an ability is drawn ([Battle](../../battle.md)). The flags and the
units are part of the save file.

## Limits

- Item ids are 9 bits, 0 to 511 (`INVENTORY_ITEM_ID_MASK`); `items.tsv` has 324 rows,
  and rows 269 to 323 have no name message.
- A unit has 15 inventory slots, and a stack holds 30.
- 20 Djinn to an element, 519 abilities, 165 enemies (the last with no name
  message), 203 classes.
- 4,096 [game flags](../../glossary.md#game-flag).
- Levels go up to 99.
- 128 rare item counters, each 0 to 99.

## Files

| File | What it holds |
| --- | --- |
| `game.c` | the object: it includes `game_internal.h`, then the parts in `game/` in ROM order; its opening comment says where each family starts |
| `game_internal.h` | the object's includes and `Inventory_CompactSlots`, which two parts use |
| [`game/units.c`](game/units.c) | the party's units and their stats, a new game, the prologue, HP and PP |
| [`game/inventory.c`](game/inventory.c) | items and inventories: equipping, adding, removing, breaking and counting, and the rare item counters |
| [`game/levels.c`](game/levels.c) | each unit's actions and Psynergy list, the characters, experience and levelling up |
| [`game/flags.c`](game/flags.c) | the game flags, read as bits, bytes and nibbles |
| [`game/party.c`](game/party.c) | enemy units, the active party, coins and counters, and summons |
| [`game/classes.c`](game/classes.c) | element levels and classes |
| [`game/battle_calc.c`](game/battle_calc.c) | battle arithmetic: the attack, Psynergy and healing formulas, the element multiplier, the battle's random numbers, the effect rolls and the weapon unleash |
| [`game/snapshot.c`](game/snapshot.c) | equipping a given item, and the equipment put aside for the Colosso |
| [`game/djinn.c`](game/djinn.c) | the Djinn: their definitions, setting them, standby and recovery, and counting them |
| `starting_equipment.c` | the starting items, class and level of the characters who begin a new game equipped: Isaac, Garet, Ivan, Mia and Jenna |
| `element_curve.s` | `gElement_PowerResistCurve`, the points `Battle_GetElementMultiplier` interpolates between |
| `*.tsv`, each with the `.s` that places it | the definition tables, listed under [their ids](#how-one-number-finds-a-row-and-its-name) |

The prototypes are in `games/tbs/include/unit/game/`, and the bank's
far-call table is `games/tbs/src/system/far_call/GAME.S`.

## Words used here

- **unit id** and **unit**: a unit id is a number, and a unit the
  `struct Unit` `Unit_Get` returns for it, as the opening says. The
  characters' units are kept in `gGameState.units` (`character_ids.h`).
- **party**: the characters travelling together. `FLAG_IN_PARTY(character)`
  says who is in it and `gGameState.party` lists them in order.
- **inventory entry**: one of a unit's `INVENTORY_SLOTS` (15) halfwords,
  holding an item, its state and its count.
- **ability**: a row of `actions.tsv`. Psynergy, a Djinni's and a summon's
  effect, an item's use and an enemy's attack are all abilities.
- **element**: `enum Element`, `ELEMENT_VENUS` to `ELEMENT_JUPITER`, and
  `ELEMENT_NONE`. A character's **element levels** are the Djinn set on it
  plus its own element bonus, an enemy's come from its element preset;
  `Unit_GetElementLevels` returns each level times ten.
- **Djinni** element and index: a Djinni is its element and its number in
  that element (`DJINNI_INDEX`). It is either set on the unit that holds it
  or on standby; the Djinn a summon uses recover for some turns before they
  can be set again.
- **class series**: the classes a character's two strongest elements select;
  `Class_Determine` picks the last in the series whose element levels the
  character reaches.
- **flag**: one of the 4,096 bits of `gGameFlagBytes`, by number; the named
  ones are in `flags.h`.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `Unit_` | one unit: stats, HP and PP, Psynergy, levels, elements |
| `Party_`, `GameState_`, `Game_` | the party and the game state |
| `Item_`, `Inventory_`, `PartyInventory_`, `InventorySnapshot_`, `ItemCounter_`, `Equipment_` | items: one unit's inventory, the party's, the Colosso's snapshot, and the rare item counters |
| `Character_`, `Class_`, `Enemy_`, `Summon_`, `SummonDefinition_`, `BattleAction_` | one entry of a definition table |
| `GameFlag_` | the game flags |
| `Djinn_`, `DjinnRecovery_` | the Djinn, and their recovery lists |
| `Battle_`, `BattleTarget_` | the battle arithmetic the battle folder calls |
| `BattleUnit_` (`BattleUnit_Set`) | filling an enemy unit at the start of a battle |
| `Runtime_` | `Runtime_CopyBytesDirectional` and `Runtime_GetBuildStampTime`: general helpers that sit in this bank but keep the runtime's prefix, as the rest of the engine names them |

## Evidence and open questions

The label columns are the English names from `games/tbs/text/en.po`, by the
message each row's id reaches. The columns are named from what the code reads
in them, and the item effect kinds from what `Unit_Calculate` does with each. Flags
48 to 127 are the Djinn's found flags because `Djinn_GiveToParty`
sets flag `FLAG_DJINN_FIRST` + 20 × element + index when a Djinni joins.

The tables keep no unknown columns. `chars.tsv`'s first twenty words
(`CharacterDefinition.reserved`) and `summons.tsv`'s two bytes after the
ability (`SummonDefinition.reserved`) are 0 in every row and read by no code.
The last byte of `classes.tsv`, `weak_effect3`, is the fourth weak effect,
which `BattleAbility_IsWeakToEffect` stops short of; it too is 0 in every row,
as is the enemies' fourth (`EnemyDefinition.weakEffects`).

Rows 269 to 323 of `items.tsv` are 55 copies of one row: a weapon only Isaac
can equip, priced 100 coins, with Attack 11, no icon and no name message.
Nothing gives one: the shops' stock table (`shop/artifacts.c`) and the
[scripts](../../glossary.md#script) name no item past `ITEM_CLERICS_RING` (268), no enemy in
`enemies.tsv` carries or drops one, and the debug item browser covers 0 to 269
and skips items without an icon. They are filler to 324 rows.


- XXX: what `GameState.unk20b` holds; a new game sets it to 1 and no C reads
  it, so a read in a map overlay or The Lost Age's use of the save's byte
  would settle it.
