# The Lost Age

Welcome to The Lost Age!

This is the second Golden Sun, from Venus Lighthouse, where The Broken Seal
ends, to the beacon on top of Mars Lighthouse, and all of it is
here to read: 325 scenes, every battle, and every line anyone says.

It is the same wonderful program as [The Broken Seal](../tbs/README.md),
grown bigger: a party of eight, a ship that sails and then flies, summons
that take Djinn of several elements, a forge, and a third game of chance.
Camelot built it in a way you rarely see any more, and these pages show
you how to think about each part of it before you open the code.

## Start here

Two of Camelot's ideas are worth meeting first. Once they click, the rest
of the code opens up.

1. [The main routine](system.md#the-main-routine). One routine runs the
   game from switch-on to switch-off, one sequence at a time: the field, a
   battle, Lucky Dice, Lucky Wheels or Super Lucky Dice.
2. [Waiting and tasks](system.md#waiting-and-tasks). Code that needs time
   simply stops and waits, and the game goes on running inside the wait. If
   you have never met a task system, this is the part that will surprise
   you.

## What runs

- [Field](field.md) is a town, a dungeon or the World Map: who stands
  where, what a talk or a step sets off, and how you leave.
- [Battle](battle.md) is one fight, turn by turn, from its opening to the
  experience and coins.
- [Lucky Dice, Lucky Wheels and Super Lucky Dice](src/field/README.md#lucky-dice-lucky-wheels-and-super-lucky-dice)
  are the three games in Contigo, each a sequence of its own.

## What they all share

- [System](system.md) is start-up, the main routine, tasks, overlays and
  [saving](system.md#saving).
- [Interface](interface.md) is the windows, the menus and the shops.
- [Sound](src/sound/README.md) is the music and how it comes back after a
  jingle.
- [Game rules](src/game/README.md) are the units, items, Djinn and classes.
- [Graphics](src/graphics/README.md) is how a picture is stored: formats,
  colours and sizes.
- [The Game Pak](src/gamepak/README.md) is how the cartridge is laid out.
- [Debug scenes](src/debug/README.md) are the test rooms Camelot left
  inside.

## Looking something up

- The [glossary](glossary.md) has every word these pages share.
- [Build it yourself](../README.md#build-it-yourself) gets you a ROM you made yourself.
