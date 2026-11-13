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

The source gives every number a name, so a table reads like this:

```c
[ITEM_LONG_SWORD] = {
    .price = 200,
    .type = ITEM_TYPE_WEAPON,
    .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
    .icon = 1,
    .primaryBonus = 14,
    .element = ELEMENT_NONE,
},
```

The tables are one file each: `item_table.c`, `ability_table.c`,
`enemy_table.c`, `class_table.c`, `djinn_table.c`, `summon_table.c` and
`character_table.c`. A row's fields are documented where its struct is
declared.

## The unit

![Isaac opens the field menu in Vale and picks Status. Look for his class, his Attack and the Venus Djinn set on him, with each element's level, Power and Resist beside them. His Psynergy list, his equipment and Garet's Status page follow.](https://alchemy.pascalpixel.com/guides/menu/summary-screen.gif)

A [unit](../../glossary.md#unit) is one fighter's whole record: HP, PP,
stats, class, Djinn, Psynergy and items. The eight playable characters each
have one from the first moment of a new game, long before most of them
join. An enemy's is filled in from its row of the enemy table when a
battle starts. Both sides being the same thing is what lets one set of
rules serve the party and the monsters alike.

A unit carries 15 items, and each slot is a single number that packs the
item, whether it is worn, whether it is broken, and how many there are. So
handing an item to someone else moves one number, and its state goes with
it.

## Why stats are always worked out from scratch

A unit keeps each stat twice: the base stat, earned by levelling up, and
the stat on the screen, with equipment, Djinn and class on top.

The game never adjusts the stat on the screen. Whenever anything behind it
changes, it throws the number away and works every stat out again from the
base. That is the one idea to hold on to in this folder. There is no code
anywhere that undoes a bonus, so a bonus can never be left behind or
applied twice, and a new kind of bonus only has to be added in one place.

HP and PP are the exception, since the current amount cannot be worked out
from anything. The unit remembers each as a share of its maximum, and
rebuilds it when the maximum changes.

## Djinn and classes

![Isaac opens the Djinn menu in Vale with Flint and Granite set, and puts Flint on standby. At `Is this OK?` read the preview: his class, his HP and his Attack all fall, and his Psynergy is marked as unchanged.](https://alchemy.pascalpixel.com/guides/menu/djinn-standby.gif)

A Djinni set on a character does two things. It adds a bonus of its own to
the stats, from its row of the Djinn table. And it raises the character's
level in its element.

### Element levels

A character has a level in each of the four elements: one for each Djinni
set in that element, plus a small bonus of its own from the character
table.

The class is chosen from those four levels and nothing else. The two
strongest elements pick a series of classes, and the character gets the
highest class in the series whose requirements it meets. The class then
multiplies the stats and lists the Psynergy the character knows, each with
the level it is learned at. So one Djinni moved can change a character's
class, stats and spells at once, and it all falls out of working everything
out from scratch.

## Levels and experience

Each character has its own list of experience thresholds, and its own
growth curve for each stat. A level up adds a little to each base stat
from the curve, with some luck in the rounding.

A new game is the same mechanism run from nothing: each character is
raised to its starting level one level at a time. So Mia's stats when she
joins were decided at the very start of the game.

## The party and its coins

The party is kept two ways at once: a flag for each character saying
whether it is in, and a list of the members in order, which is the order
the menus and the battle show them in. The coins are one number for
everyone.

## The game flags

![In Kolima Isaac talks to the Venus Djinni Granite beside him. The screen turns green, lights rain onto Isaac, a ring of light closes on him, and the message says Granite has joined him. Then watch the end: the screen fades out, and when Kolima fades in again Isaac stands in the same place with no one beside him.](https://alchemy.pascalpixel.com/guides/game/djinni-flag.gif)

The game remembers the story as 4,096 yes-or-no facts, the
[game flags](../../glossary.md#game-flag). A chest opened, a Djinni found,
a conversation had: each is one flag, with a name in
[flags.h](../../include/flags.h).

A few runs have a use of their own. The first eight say who is in the party. One run is
the 80 Djinn. Two runs of 256 are wiped automatically, one on entering a
scene and one on entering a new area
([Scenes](../scene/README.md#state-that-cleans-itself-up)). Everything
else is free for the story.

Flags and the main routine's work are all a save holds, so anything the
game should remember has to be one or the other.

## Rare item counters

A shop never runs out of Herbs, but a rare item can sell out. Each rare
item has a counter of how many the shops may still sell. Buying one takes
one off and selling one puts it back, which is why an artifact sold in one
town can be bought back in another.

## Battle arithmetic

![Isaac attacks Ivan with the Swift Sword. Watch for the message that the sword lets out a howl: the plain blow is replaced by Sonic Smash, a burst of light and a blue slash.](https://alchemy.pascalpixel.com/guides/battle/unleash.gif)

The sums that decide a fight live here, not in the battle: whether a
weapon unleashes, whether Sleep takes hold, how much a blow hurts. The
battle asks and shows the answer. So does the menu, which is why Cure cast
in Vale heals by the same sum as Cure cast in a fight.

Several answers are rolls of dice. The battle has dice of its own, apart
from the field's, and in a link battle the two consoles pass their state
across each turn so that both screens roll the same.

## Where the code is

- The `*_table.c` files are the tables.
- `game/units.c` works a unit's stats out; `classes.c`, `djinn.c`,
  `levels.c`, `inventory.c`, `party.c` and `flags.c` are what their
  names say.
- `game/battle_arithmetic.c` is the sums.
