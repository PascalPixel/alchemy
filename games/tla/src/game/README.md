# Game rules

This folder is the game's numbers: who has what stats, what an item does,
which class a character is, what the game remembers. Nearly all of it is
tables, written as C you can read, and a small amount of code that works
things out from them.

If you want to know what an item, an enemy or a class is made of, the
answer is a row in one of these files.

## How one number finds a row and its name

Everything in the game is known by a number, and the number is nothing but
a row's position. The Healing Ring is item 265: row 265 of the item table
holds its price and what it does, and its name is the message at 265 past
the first item name. Nothing in a row says which item it is.

The Lost Age's tables keep each row at its place, with its name from the
English release in a comment above it, so a table reads like this:

```c
    // Long Sword
    {
        .price = 200,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = 0x93,
        .icon = 1,
        .primaryBonus = 14,
        .element = ELEMENT_NONE,
    },
```

The mask has a bit for each character who can wield it: Isaac, Garet,
Felix and Piers.

The tables are `gItem_DefinitionTable` (516 items),
`gAbilityDefinitions` (734 abilities), `gEnemy_DefinitionTable` (380
enemies), `gClass_DefinitionTable` (244 classes), `gDjinn_DefinitionTable`,
`gSummon_DefinitionTable` and `gCharacter_DefinitionTable`. A row's fields
are documented where its struct is declared. With more than 255 abilities,
a class lists the ones it teaches in halfwords.

## The unit

A [unit](../../glossary.md#unit) is one fighter's whole record: HP, PP,
stats, class, Djinn, Psynergy and items, 332 bytes of it. The eight
playable characters each have one from the first moment of a new game,
long before most of them join. An enemy's is filled in from its row of the
enemy table when a battle starts, six at most. Both sides being the same
thing is what lets one set of rules serve the party and the monsters
alike.

A unit carries 15 items, and each slot is a single number that packs the
item, whether it is worn, whether it is broken, and how many there are. So
handing an item to someone else moves one number, and its state goes with
it.

## Why stats are always worked out from scratch

A unit keeps each stat twice: the base stat, earned by levelling up, and
the stat on the screen, with equipment, Djinn and class on top.

The game never adjusts the stat on the screen. Whenever anything behind it
changes, it throws the number away and works every stat out again from the
base (`Unit_Calculate`). That is the one idea to hold on to in this folder.
There is no code anywhere that undoes a bonus, so a bonus can never be left
behind or applied twice, and a new kind of bonus only has to be added in
one place.

HP and PP are the exception, since the current amount cannot be worked out
from anything. The unit remembers each as a share of its maximum, and
rebuilds it when the maximum changes.

## Djinn and classes

A Djinni set on a character does two things. It adds a bonus of its own to
the stats, from its row of the Djinn table. And it raises the character's
level in its element.

The Djinn table has 80 rows, twenty to an element, and 72 of them are
Djinn with names: every Djinni of both games, 18 to an element, since a
save from The Broken Seal can bring its Djinn across. Piers brings Spring
and Shade when he joins, and Isaac, Garet, Ivan and Mia bring theirs at
Contigo.

### Element levels

A character has a level in each of the four elements: one for each Djinni
set in that element, on top of a start of its own from the character
table, five in the character's own element and none in the others.

The class is chosen from those four levels. The two strongest elements
pick a series of classes, and the character gets the highest class in the
series whose requirements it meets (`Class_Determine`). Jenna and Piers
have a series of their own where the others share one, and three items
pick a series when held: the Mysterious Card, the Trainer's Whip and the
Tomegathericon. The class then multiplies the stats and lists the Psynergy
the character knows, each with the level it is learned at. So one Djinni
moved can change a character's class, stats and spells at once, and it all
falls out of working everything out from scratch.

## Levels and experience

Each character has its own list of experience thresholds, 99 levels long,
and its own growth curve for each stat, a point every 20 levels. A level
up adds a little to each base stat from the curve, with some luck in the
rounding.

A new game is the same mechanism run from nothing: each character is
raised to its starting level one level at a time. Felix, Jenna and Sheba
start at level 5, Piers at 18, and Isaac's party at 28. So Piers's stats
when he joins were decided at the very start of the game, unless a save
from The Broken Seal brings Isaac's party across with its own.

## The party and its coins

The party is kept two ways at once: a flag for each character saying
whether it is in, and a list of the members in order, which is the order
the menus and the battle show them in. In a battle the first four in that
list fight and the rest wait behind
([Battles](../battle/README.md#the-partys-commands)). The coins are one
number for everyone, 200 at a new game.

## The game flags

The game remembers the story as 4,096 yes-or-no facts, the
[game flags](../../glossary.md#game-flag). A chest opened, a Djinni found,
a conversation had: each is one flag, with a name in
[flags.h](../../include/flags.h) once its meaning is known. The Lost Age
reads them through `GameFlag_Get` and its neighbours, which it links in its
first bank, so they are in the [system](../system/README.md) and not here.
They can also read eight flags as a byte or four as a number, for the
counts the story keeps.

A few runs have a use of their own. The first eight say who is in the party. One run is
the 80 Djinn. Two runs of 256 are wiped automatically, one on entering a
scene and one on entering a new area
([Scenes](../scene/README.md#state-that-cleans-itself-up)). Everything
else is free for the story.

## Rare item counters

A shop never runs out of Herbs, but a rare item can sell out. Each rare
item has a counter of how many the shops may still sell. Buying one takes
one off and selling one puts it back, which is why an artifact sold in one
town can be bought back in another.

## Battle arithmetic

The sums that decide a fight live here, not in the battle: whether a
weapon unleashes, whether Sleep takes hold, how much a blow hurts. The
battle asks and shows the answer. So does the menu, which is why Cure cast
in Daila heals by the same sum as Cure cast in a fight.

Several answers are rolls of dice. The battle has dice of its own, apart
from the field's (`Battle_Random`). In a link battle both consoles start
those dice from the same seed and shake hands after every action, so that
both screens roll the same.

## Where the code is

- The tables are C arrays at their places in the ROM; the item, ability
  and enemy tables are parts of one object with each item's rare item
  counter.
- The game object is written as parts cut where The Broken Seal's are:
  `Unit_Calculate` works a unit's stats out, `Class_Determine` its class,
  and the rest go by subject, Djinn, levels, items and the party.
- `Battle_GetElementMultiplier` and its neighbours are the sums.
