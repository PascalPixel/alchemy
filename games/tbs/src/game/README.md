# Game rules

Everything the game needs to know about the [party](../../glossary.md#the-party)
lives here: who is in it, each character's level, HP and stats, the items each
one carries, the [Djinn](../../glossary.md#djinn) they hold, and the flags that
remember what has happened in the story. So do the tables that describe every
item, ability, Djinni, enemy, [summon](../../glossary.md#summons) and class.
The rest of the game asks this code its questions: a
[map](../../glossary.md#map) whether a chest has been opened, a shop what an
item costs, a menu what a character's Attack would be with another sword, and
a battle what a blow does. [Battles](../battle/README.md) follows an ability
from these tables to its damage.

Two words run through the page. A character's stats, HP and
[PP](../../glossary.md#psynergy-and-pp), items and Djinn make up a unit, a
`struct Unit`; the same struct holds each enemy in a battle. The number that
picks a unit is its unit id: `ISAAC` (0) to `PIERS` (7) for the eight
characters of both games, and from `FIRST_ENEMY_UNIT` (128) for the current
battle's enemies. The code is its own [bank](../../glossary.md#bank), so code
elsewhere calls it through [far calls](../../glossary.md#far-call), by names
ending in `Far`: `Unit_GetFar` leads to `Unit_Get`.

## How one number finds a row and its name

The Healing Ring costs 800 coins, heals like Cure when used, and is called
"Healing Ring" on screen. Those facts sit in different places, the price in
the item table and the name in the game's text, and the one thing that joins
them is a number: the ring is item 265, the 265th row of one list and the
265th entry of the others.

The definition tables have no id column: an entry's id is its row, the first
being entry 0. Every line of the game's text is a numbered message, and each
table's names are a run of messages in the same order, starting at a message
named for the run:

```
name message = MsgItemName + item id
             = 386 + 265 = 651 for the Healing Ring
```

The same holds for an item's line on the Item screen (`MsgItemPlainName`),
an enemy's name (`MsgEnemyName`), an ability's name and description
(`MsgAbilityName`, `MsgAbilityDescription`) and a Djinni's name
(`MsgDjinnName`); a Djinni's row also gives its ability,
`ABILITY_DJINNI_FIRST` + row. So the tables and the six languages' message
catalogs are parallel lists tied only by position: a row inserted into
`gItem_DefinitionTable` would give every later item the next one's name.
Rows no item uses stay as placeholders so that later ids don't move.

Each table is a C array of the struct that reads it, written with designated
initializers: a field left out is 0, and a field with a meaning shows its enum
or flag names (`ITEM_TYPE_WEAPON`, `ELEMENT_VENUS`). The comment above each
entry is the English name its message gives. `item_ids.h`, `ability_ids.h`
and `djinn_ids.h` name the ids the code uses.

| Table | One entry per | File |
| --- | --- | --- |
| `gItem_DefinitionTable` | item (324) | `item_table.c` |
| `gAbilityDefinitions` | ability (519) | `abilities.c` |
| `gDjinn_DefinitionTable` | Djinni, 20 to an [element](../../glossary.md#the-four-elements) (80) | `djinn_table.c` |
| `gEnemy_Definitions` | enemy (165) | `enemies.c` |
| `gSummon_DefinitionTable` | summon (16) | `summons.c` |
| `gClass_DefinitionTable` | class (203) | `classes.c` |
| `gCharacter_DefinitionTable` | character (8) | `chars.c` |
| `gEnemy_ElementPresetTable` | enemy element preset (44) | `element_table.c` |
| `gItem_ArtifactSlotTable` | item id (512): a rare item's counter number plus one, or 0 | `artifact_slots.c` |
| `gCharacter_LevelExpTable` | level (99) of each character, with the experience it needs | `experience_table.c` |

| Name | Value | Meaning |
| --- | --- | --- |
| `DJINN_PER_ELEMENT` | 20 | Djinn to an element |
| `ABILITY_DJINNI_FIRST` | 300 | the first Djinni's ability; Djinni row n is ability 300 + n |

## An item's row

![Isaac buys at Bilibin's weapon shop: the list starts with the Long Sword at 200 coins; asked `Which one of you gets this?`, the hand on Isaac shows his Attack going from 56 with his Bandit's Sword to 58, on Garet from 58 with his Battle Axe down to 48, and on Ivan `Can't equip`.](https://alchemy.pascalpixel.com/guides/game/long-sword.gif)

Everything the player learns about an item in a shop or on the Item screen
comes from one row of the item table: its price, who can equip it, what it
adds to Attack or Defense, and what using it does. The Long Sword's row says
200 coins, a weapon, Attack 14, for Isaac, Garet and Felix, which is why Ivan
gets `Can't equip`.

The row is read as `struct ItemDefinition`
(`games/tbs/include/game/item.h`). The Long Sword's reads price 200, type 1 (a
weapon), `equipMask` `0x13`, icon 1, `primaryBonus` 14 and element
`ELEMENT_NONE`; every other field is 0. `abilityId` and `unleashAbility` are
rows of `abilities.c`: used in battle, the item command builds a turn from
`abilityId` (`BattleTurn_Build`, [battle/battle/turn.c](../battle/battle/turn.c)),
and [Battles](../battle/README.md#how-an-ability-resolves) follows it from
there. An item whose `useType` is `ITEM_USE_TEACHES` is never used: while it
is equipped its `abilityId` is in the unit's Psynergy list (`game/levels.c`).
These are the eight Psynergy items, from the Orb of Force on.

| Field | What it holds |
| --- | --- |
| `price` | the price in coins |
| `type` | `enum ItemType`: `ITEM_TYPE_NONE` for anything that isn't equipment, then weapons, armor, shields, helms, boots, `ITEM_TYPE_PSYNERGY`, rings and shirts; a unit equips one of each type except [Psynergy](../../glossary.md#psynergy-and-pp) items |
| `flags` | `ITEM_CURSED`, `ITEM_CANNOT_UNEQUIP`, `ITEM_RARE`, `ITEM_INDISPENSABLE` and `ITEM_STACKABLE` |
| `equipMask` | one bit per character who can equip it, bit 0 for Isaac |
| `icon` | its menu icon (`gItemIcons`) |
| `primaryBonus`, `secondaryBonus` | Attack and Defense added while equipped |
| `unleashAbility`, `unleashChance` | the ability a weapon unleashes, at `unleashChance` steps of 5 percent (`RollWeaponUnleash`) |
| `useType` | what using it costs: `ITEM_USE_CONSUMED`, `ITEM_USE_MAY_BREAK` (one time in eight), `ITEM_USE_EMPTIES` (leaves an Empty Bottle), or `ITEM_USE_FREE` |
| `element` | a weapon's attack element, `ELEMENT_NONE` for none |
| `effects` | up to four effects while equipped, each a kind (`enum EquipEffectKind`) and an amount, which `Unit_Calculate` applies |
| `abilityId` | the ability the item uses, or teaches as Psynergy |

| Name | Value | Meaning |
| --- | --- | --- |
| `ITEM_CURSED` | 1 | equipping it curses the wearer |
| `ITEM_CANNOT_UNEQUIP` | 2 | once equipped it cannot be taken off, or dropped while worn |
| `ITEM_RARE` | 4 | a rare item: the Item screen can call it `A rare item`, a shopkeeper buying it says `What rare stuff!`, and it ranks high among a battle's spoils |
| `ITEM_INDISPENSABLE` | 8 | an important item: the player cannot drop it, no shop buys it, and it ranks first among the items enemies leave |
| `ITEM_STACKABLE` | `0x10` | kept in one inventory entry with a count |
| `ITEM_USE_MAY_BREAK` | 2 | it breaks one time in eight, and a smith can mend it ([Shops](../shop/README.md#repairing)) |
| `ITEM_USE_TEACHES` | 3 | never used: equipped, it teaches its `abilityId` as Psynergy |

## What an inventory slot holds

![In Xian, Isaac's Item screen list: a Herb with a small 3 beside its icon, a Long Sword drawn as a red cross because it is broken, and his Broad Sword, Adept's Clothes and Bronze Shield each with a small green E because he wears them; the hand moves down the list and the line at the bottom names each one.](https://alchemy.pascalpixel.com/guides/game/inventory.gif)

Each character carries up to 15 things, and the Item screen shows for each
whether it is equipped (a small E), whether it is broken (a red cross) and how
many there are of a stackable item such as Herbs. All of that fits in one
16-bit number per slot, so an item, its state and its count move together in
one load and one store.

The slot is a halfword split into four parts from its highest bit down. The
count is kept as one less, so a single item's count part is 0, and an empty
slot is 0, which `Inventory_CompactSlots` relies on to close gaps. A stack
grows to 30 (`Inventory_AddItem`). Code that asks which item a slot holds
masks it with `INVENTORY_ITEM_ID_MASK` first, since the same item equipped,
broken or stacked is a different halfword: three Herbs, item 180, hold 2 in
the count part. The slots are `Unit.inventory`; `enum InventoryEncoding` in
`item.h` names their parts, and [game/inventory.c](game/inventory.c) reads and
writes them.

```
bits 15..11   count less one   (INVENTORY_QUANTITY_SHIFT)
bit 10        broken           (INVENTORY_BROKEN)
bit 9         equipped         (INVENTORY_EQUIPPED)
bits 8..0     item id          (INVENTORY_ITEM_ID_MASK)
```

| Name | Value | Meaning |
| --- | --- | --- |
| `INVENTORY_ITEM_ID_MASK` | `0x1ff` | the item id, 0 to 511 |
| `INVENTORY_EQUIPPED` | `0x200` | the item is equipped |
| `INVENTORY_BROKEN` | `0x400` | the item is broken |
| `INVENTORY_QUANTITY_SHIFT` | 11 | the first bit of the count less one |
| `INVENTORY_SLOTS` | 15 | the slots a unit has |

## Why stats are always worked out from scratch

![On the Item screen, the hand picks Isaac's Machete and points at Remove, and the status panel shows his Attack falling from 24 to 17](https://alchemy.pascalpixel.com/guides/game/items.gif)

When the player points at Remove on Isaac's Machete, the status panel shows
his Attack falling from 24 to 17 before anything is chosen. The game never
adds or subtracts a stat bit by bit: whenever anything that feeds a stat
changes, it works every stat out again from the start, and a menu's preview
is that same calculation, run on Isaac for a moment and then undone.

`Unit_Calculate` ([game/units.c](game/units.c)) works a unit's stats out from
its base stats, equipment, set Djinn, class and battle stat levels, clamps
each and writes them back, keeping current HP and PP at the same fraction of
their maximums. It runs after equipping, setting a Djinni, levelling up or a
class change. To preview, `StatusPanel_Draw`
([menu/menu/common.c](../menu/menu/common.c)) copies the unit to the
[heap](../../glossary.md#heap-and-pools), changes the real unit's equipment,
lets `Unit_Calculate` run, draws the new numbers and restores the unit from
the copy. The Machete adds 6 to Attack, but the panel shows 7 less, because
his class multiplies the sum and the result rounds down; of the multipliers in
`classes.c`, only 11 tenths (the Squire's, among others) gives both numbers,
from an Attack of 16 before the class:

```
with the Machete     (16 + 6) * 11 / 10 = 24
without it                 16 * 11 / 10 = 17
```

## The game flags

![In Lunpa Fortress the Mercury Djinni Tonic, beside Isaac, vanishes, a stream of blue drops falls onto him, and a window says `The Mercury Djinni Tonic joined Isaac.`](https://alchemy.pascalpixel.com/places/lunpa-fortress-upper-floor/events/Event_DjinniJoins.gif)

The game remembers what has happened (a chest opened, a Djinni found, a scene
already played) as yes-or-no flags, 4,096 of them, one bit each. When Tonic
joins, its found flag is set: a Djinni found once cannot join again
(`Djinn_GiveToParty` refuses it), and the save files count the Djinn found.
Eight flags in a row, read together as a byte, also hold small counters.

A flag's low three bits pick its bit and the next nine its byte of
`gGameFlagBytes` (512 bytes); `GAME_FLAG_BYTE` drops the rest, so a number
past 4,095 wraps onto a lower flag instead of reaching other memory.
`GameFlag_GetByte`, `GameFlag_SetByte` and `GameFlag_IncrementByte` read eight
flags as one byte, as the Battle Arena's win count (`FLAG_MONSTER_BATTLE_WINS`)
is kept, and `GameFlag_GetNibble` and `GameFlag_SetNibble` four. Two ranges are
scratch space for the map code: a map's own flags, cleared every time the
party enters a map, and a map section's, cleared when it enters another
[map section](../../glossary.md#map-section)
([the front page](../../README.md#game-flags) gives the ranges).

Flags `FLAG_DJINN_FIRST` (48) to 127 are the Djinn's found flags, Djinni n of
element e being `FLAG_DJINN_FIRST` + 20 × e + n: Tonic, Mercury's Djinni 5 counting from 0, is
flag 73. The save summary counts them (`Save_BuildFileSummary`), and the title
screen offers its Battle command only when some save file has found a Djinni
(`gTitleBattleOptionEnabled`). The flag functions are in
[game/flags.c](game/flags.c), `GAME_FLAG_BYTE` in `game_flags.h` and the named
flags in `flags.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `GAME_FLAG_BYTE(flag)` | bits 3 to 11 of `flag` | the byte of `gGameFlagBytes` that holds the flag |
| `FLAG_DJINN_FIRST` | 48 | the first Djinni's found flag; the 80 run to 127 |
| `FLAG_LINK_PARTY_COUNT` | `0x3f0` | a byte: the members the linked opponent sent |
| `FLAG_MONSTER_BATTLE_WINS` | `0x3f8` | a byte: the Battle Arena's win count |

## Rare item counters

A shop never runs out of Herbs, but a rare item such as the Healing Ring can
be sold only as many times as its counter allows; [Shops](../shop/README.md#artifacts)
shows what the player sees of it.

`gRareItemStock` holds 128 counters, each 0 to 99, for how many of each rare
item the shops may still sell. `gItem_ArtifactSlotTable` gives each item id
its counter number plus one, 0 for an ordinary item (artifact is the code's
word for an item with a counter). Buying one takes one off and selling or
discarding one puts it back (`Item_AdjustCounter`, `RareItemStock_Adjust`, in
[game/inventory.c](game/inventory.c)).

| Name | Value | Meaning |
| --- | --- | --- |
| `gRareItemStock` | 128 bytes | one counter a rare item, 0 to 99 |
| `gItem_ArtifactSlotTable` | 512 entries | an item's counter number plus one, or 0 |

## How it fits with the rest of the game

Code in the other banks calls this one's functions by their far-call names,
and a map's [overlay](../../glossary.md#overlay) through the list of
outside functions it was built with (its [import table](../../glossary.md#entry-table-and-import-table)); some code reads
`gSaveBlock`, the state of the game in progress, directly, as the save
summary does. The battle folder reads `abilities.c` through
`struct AbilityDefinition` beside its own `gAbilityAnimations` and `gAbilityStaging`, which
choose how an ability is drawn ([Battle](../../battle.md)). The flags and the
units are part of the save file.

## Limits

- Item ids are 9 bits, 0 to 511 (`INVENTORY_ITEM_ID_MASK`); `item_table.c` has 324 rows,
  and rows 269 to 323 have no name message.
- A unit has 15 inventory slots, and a stack holds 30.
- 20 Djinn to an element, 519 abilities, 165 enemies (the last with no name
  message), 203 classes.
- 4,096 [game flags](../../glossary.md#game-flag).
- Levels go up to 99.
- 128 rare item counters, each 0 to 99.

## Words used here

- **unit id** and **unit**: a unit id is a number, and a unit the
  `struct Unit` `Unit_Get` returns for it, as the opening says. The
  characters' units are kept in `gSaveBlock.units` (`character_ids.h`).
- **party**: the characters travelling together. `FLAG_IN_PARTY(character)`
  says who is in it and `gSaveBlock.party` lists them in order.
- **inventory entry**: one of a unit's `INVENTORY_SLOTS` (15) halfwords,
  holding an item, its state and its count.
- **ability**: a row of `abilities.c`. Psynergy, a Djinni's and a summon's
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

## Evidence and open questions

The label columns are the English names from `games/tbs/text/en.po`, by the
message each row's id reaches. The columns are named from what the code reads
in them, and the item effect kinds from what `Unit_Calculate` does with each. Flags
48 to 127 are the Djinn's found flags because `Djinn_GiveToParty`
sets flag `FLAG_DJINN_FIRST` + 20 × element + index when a Djinni joins.

The tables keep no unknown columns. `chars.c`'s first twenty words
(`CharacterDefinition.reserved`) and `summons.c`'s two bytes after the
ability (`SummonDefinition.reserved`) are 0 in every row and read by no code.
The last byte of `classes.c`, `weakEffects[3]`, is the fourth weak effect,
which `BattleAbility_IsWeakToEffect` stops short of; it too is 0 in every row,
as is the enemies' fourth (`EnemyDefinition.weakEffects`).

Rows 269 to 323 of `item_table.c` are 55 copies of one row: a weapon only Isaac
can equip, priced 100 coins, with Attack 11, no icon and no name message.
Nothing gives one: the shops' stock table (`shop/artifacts.c`) and the
[scripts](../../glossary.md#script) name no item past `ITEM_CLERICS_RING` (268), no enemy in
`enemies.c` carries or drops one, and the debug item browser covers 0 to 269
and skips items without an icon. They are filler to 324 rows.

The save block's byte after the Speech option, `unusedAfterSpeech`, is set to 1
by a new game and read by nothing in the game: the option menu keeps only the
window colour, message speed, Speech and auto-sleep, and no map reaches the
save block by offset there. A cleared file sent by cable sends the 4,100
bytes from `gSaveBuffer`, which hold the game flags and the save block, this
byte included, to The Lost Age. The Lost Age does not read it either: its
cable transfer copies the received flags over its own and the received save
block whole into its own, 32 bytes further on, then reads only the window
colour back from the copy. The byte lands on one that The Lost Age's new game
sets to 1 and that none of its code, in the main image or any overlay, loads.
