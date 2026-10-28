# Game rules

Everything the game needs to know about the [party](../../glossary.md#the-party)
lives here: who is in it, each character's level, HP and stats, the items each
one carries, the [Djinn](../../glossary.md#djinn) they hold, and the flags that
remember what has happened in the story. So do the tables that describe every
item, ability, Djinni, enemy, [summon](../../glossary.md#summons) and class.
The rest of the game asks this code its questions: a
[scene](../../glossary.md#scene) whether a chest has been opened, a shop what an
item costs, a menu what a character's Attack would be with another sword, and
a battle what a blow does. [Battles](../battle/README.md) follows an ability
from these tables to its damage.

A character's stats, HP and [PP](../../glossary.md#psynergy-and-pp), items and
Djinn make up a [unit](../../glossary.md#unit), and the same struct holds each
enemy in a battle. The
number that picks a unit is its unit id: `ISAAC` (0) to `PIERS` (7) for the
eight characters of both games, and from `FIRST_ENEMY_UNIT` (128) for the
current battle's enemies. A unit is a `struct Unit`, which `Unit_Get` returns
for a unit id. This code is its own [bank](../../glossary.md#bank), so code
elsewhere calls it through [far calls](../../glossary.md#far-call), by names
ending in `Far`.

## How one number finds a row and its name

The Healing Ring costs 800 coins, heals like Cure when used, and is called
"Healing Ring" on screen. Those facts sit in different places, the price in
the item table and the name in the game's text, and what joins them is a
number: the ring is item 265.

The tables have no column saying which item a row is: an item's number is its
row, counting from 0. Every line of the game's text is a numbered message, and
each table's names are a run of messages in the same order, so the name of
item 265 is the 265th message after the first item name:

```
name message = MsgItemName + item id
             = 386 + 265 = 651 for the Healing Ring
```

Enemies, abilities and Djinn find their names and descriptions the same way.
The tables and the six languages' text are parallel lists tied only by
position: a row inserted into the item table would give every later item the
next one's name.

Each table is a C array of the struct that reads it, with a comment above each
entry giving the English name its message holds: `gItem_DefinitionTable`
(`item_table.c`), `gAbilityDefinitions` (`abilities.c`),
`gDjinn_DefinitionTable` (`djinn_table.c`), `gEnemy_Definitions`
(`enemies.c`), `gSummon_DefinitionTable` (`summons.c`),
`gClass_DefinitionTable` (`classes.c`) and `gCharacter_DefinitionTable`
(`chars.c`). `item_ids.h`, `ability_ids.h` and `djinn_ids.h` name the ids the
code uses.

## An item's row

Everything the player learns about an item in a shop or on the Item screen
comes from one row of the item table: its price, who can equip it, what it
adds to Attack or Defense, and what using it does. The Long Sword's row says
200 coins, a weapon, Attack 14, for Isaac, Garet and Felix, which is why a shop
shows `Can't equip` beside Ivan.

A row also says what the item costs to use: whether it is used up, may break
(one time in eight, after which a smith can mend it,
[Shops](../shop/README.md#repairing)), leaves an Empty Bottle, or costs
nothing. It carries the item's flags too: cursed, cannot be taken off, rare,
important (never dropped and worth nothing to a shop), or kept in a stack. The
eight Psynergy items, from the Orb of Force to the Catch Beads, are never used
up: while one is equipped, the Psynergy it names is on its owner's list
(`ITEM_USE_TEACHES`).

The row is read as `struct ItemDefinition` (`item.h`),
whose fields and flags say what each holds. When an item is used in battle,
the battle builds an action from the ability the row names
([Battles](../battle/README.md#how-an-ability-resolves)).

## What an inventory slot holds

Each character carries up to 15 things, and the Item screen shows for each
whether it is equipped (a small E), whether it is broken (a red cross) and how
many there are of a stackable item such as Herbs. All of that fits in one
16-bit number per slot, so an item, its state and its count move together.

The number has four parts: the item, an equipped mark, a broken mark and the
count. The count is kept as one less, so a single item's count part is 0, and
a stack grows to 30. Because the marks and the count share the number, the
same item equipped, broken or stacked is a different number, and code that
asks which item a slot holds masks the rest away first.

The slots are `Unit.inventory`; `enum InventoryEncoding` in `item.h` gives the
bit layout, and [game/inventory.c](game/inventory.c) reads and writes them.

## Why stats are always worked out from scratch

![On the Item screen, the hand picks Isaac's Machete and points at Remove, and the status panel shows his Attack falling from 24 to 17](https://alchemy.pascalpixel.com/guides/game/items.gif)

When the player points at Remove on Isaac's Machete, the status panel shows
his Attack falling from 24 to 17 before anything is chosen. The game never
adds or subtracts a stat bit by bit: whenever anything that feeds a stat
changes, it works every stat out again from the character's base stats,
equipment, Djinn and class. A menu's preview is that same calculation, run on
the changed unit and then undone from a copy.

The Machete adds 6 to Attack, but the panel shows 7 less, because the class
multiplies the sum and the result rounds down. With an Attack of 16 before the
class and a class that multiplies by 11 tenths (the Squire's, among others):

```
with the Machete     (16 + 6) * 11 / 10 = 24
without it                 16 * 11 / 10 = 17
```

Working stats out from scratch means no change can leave a stat a point off,
and current HP and PP keep the same share of their new maximums.
`Unit_Calculate` ([game/units.c](game/units.c)) is the calculation; it runs
after equipping, setting a Djinni, levelling up or a class change.

## The game flags

![In Lunpa Fortress the Mercury Djinni Tonic, beside Isaac, vanishes, a stream of blue drops falls onto him, and a window says `The Mercury Djinni Tonic joined Isaac.`](https://alchemy.pascalpixel.com/places/lunpa-fortress-upper-floor/events/Event_DjinniJoins.gif)

The game remembers what has happened (a chest opened, a Djinni found, an event
already played) as 4,096 yes-or-no flags, one bit each. When Tonic joins, its
found flag is set: a Djinni found once cannot join again, and each save file's
summary counts the Djinn found. The File Screen offers its Battle command only
once some save file has found a Djinni.

Eight flags in a row, read together as a byte, also hold small counters, such
as the Battle Arena's win count. Two ranges are scratch space for the scene
code: a scene's own flags, cleared each time the party walks into a scene (not
on coming back from a battle), and an [area](../../glossary.md#area)'s,
cleared when it enters another
([Scenes](../scene/README.md#state-that-cleans-itself-up) gives the ranges).

The flags are `gGameFlagBytes`, read and written by the functions in
[game/flags.c](game/flags.c); the named flags are in `flags.h`, where the
Djinn's found flags start at `FLAG_DJINN_FIRST`, 20 to an
[element](../../glossary.md#the-four-elements).

## Rare item counters

A shop never runs out of Herbs, but it can sell a rare item such as the Magic
Rod only while that item's counter is above zero;
[Shops](../shop/README.md#artifacts) shows what the player sees of it. There
are 128 counters, each kept between 0 and 99 (`RareItemStock_Adjust`).

The counters are `gRareItemStock`, and `gItem_ArtifactSlotTable` says which
counter, if any, each item uses; `Item_AdjustCounter` changes an item's
counter.

## How it fits with the rest of the game

Code in the other banks calls this one's functions by their far-call names,
and a scene's [overlay](../../glossary.md#overlay) through the list of
outside functions it was built with (its
[import table](../../glossary.md#entry-table-and-import-table)). The units
and the party live in the
[main routine's work](../../glossary.md#main-routines-work)
(`gMainRoutineWork.units`, `.party`), which other code also reads directly, as
the save file summary does. The battle folder
reads `abilities.c` beside its own tables, which choose how an ability is
drawn ([Battle](../../battle.md)). The flags and the units are part of the
save file.

## Limits

- Item ids are 9 bits, 0 to 511; `item_table.c` has 324 rows, and rows 269 to
  323 have no name.
- A unit has 15 inventory slots, and a stack holds 30.
- 20 Djinn to an element, 519 abilities, 165 enemies (the last with no name),
  203 classes.
- 4,096 [game flags](../../glossary.md#game-flag); a flag number past that
  wraps onto a lower flag.
- Levels go up to 99.
- 128 rare item counters, each 0 to 99.

## Words used here

- **unit id** and **unit**: a unit id is a number, and a unit the
  `struct Unit` `Unit_Get` returns for it. The characters' units are kept in
  `gMainRoutineWork.units`, by the character ids in `character_ids.h`.
- **party**: the characters travelling together. `FLAG_IN_PARTY(character)`
  says who is in it and `gMainRoutineWork.party` lists them in order.
- **inventory slot**: one of a unit's `INVENTORY_SLOTS` (15) halfwords,
  holding an item, its state and its count.
- **ability**: a row of `abilities.c`. Psynergy, what a Djinni or summon does,
  an item's use and an enemy's attack are all abilities.
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

## Evidence

The label columns are the English names from `games/tbs/text/en.po`, by the
message each row's id reaches. The columns are named from what the code reads
in them, and the equip effect kinds (`EQUIP_EFFECT_`) from what
`Unit_Calculate` does with each. The 80 flags from `FLAG_DJINN_FIRST` are the
Djinn's found flags because `Djinn_GiveToParty` sets
flag `FLAG_DJINN_FIRST` + 20 × element + index when a Djinni joins. The
Machete's arithmetic on this page is the one multiplier in `classes.c` that
gives both numbers the recording shows.

`chars.c`'s first twenty words (`CharacterDefinition.reserved`) and
`summons.c`'s two bytes after the ability (`SummonDefinition.reserved`) are 0
in every row and read by no code. The last byte of each `classes.c` row,
`weakEffects[3]`, is the fourth weak effect,
which `BattleAbility_IsWeakToEffect` stops short of; it too is 0 in every row,
as is the enemies' fourth (`EnemyDefinition.weakEffects`).

Rows 269 to 323 of `item_table.c` are 55 copies of one row: a weapon only Isaac
can equip, priced 100 coins, with Attack 11, no icon and no name message.
Nothing gives one: the shops' stock table (`shop/shop/artifacts.c`) and the
[events](../../glossary.md#event) name no item past `ITEM_CLERICS_RING` (268),
no enemy in `enemies.c` carries or drops one, and the debug item browser
covers 0 to 269 and skips items without an icon. They are filler to 324 rows.
