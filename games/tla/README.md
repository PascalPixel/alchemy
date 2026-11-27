# The Lost Age

![The title after the prologue: the Golden Sun The Lost Age logo fades in from white over the valley and its lighthouse and is held](https://alchemy.pascalpixel.com/tla/story/title.gif)

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

1. [The main routine](../../docs/tla/system.md#the-main-routine). One function runs the
   game from switch-on to switch-off, one mode at a time: the field, a
   battle, Lucky Dice, Lucky Wheels or Super Lucky Dice.
2. [Waiting and events](../../docs/tla/system.md#waiting-and-events). No loop at the top
   turns once a frame. The frame loop is inside one function, `sySynch()`:
   code that needs time to pass calls it, and it runs that many frames of
   the game and returns. What it runs each frame is a short list of
   functions, the events. This is the part that will surprise you.

## What runs

- [The field engine](../../docs/tla/field.md) runs a town, a dungeon or the
  World Map: who stands where, what a talk or a step sets off, and how you
  leave.
- [The battle engine](../../docs/tla/battle.md) runs one fight, turn by turn,
  from its opening to the experience and coins.
- [Lucky Dice, Lucky Wheels and Super Lucky Dice](src/field/README.md#lucky-dice-lucky-wheels-and-super-lucky-dice)
  are the three games in Contigo, each a mode of its own.

## What they all share

- [The system library](../../docs/tla/system.md) is start-up, the main routine,
  events, overlays and [saving](../../docs/tla/system.md#saving).
- [The menu engine](../../docs/tla/interface.md) is the windows, the menus and
  the shops.
- [Sound](src/sound/README.md) is the music and how it comes back after a
  jingle.
- [The game logic](src/game/README.md) is the units, items, Djinn and classes.
- [Graphics](src/graphics/README.md) is how a picture is stored: formats,
  colours and sizes.
- [The Game Pak](src/system/README.md) is how the cartridge is laid out.
- [Debug scenes](src/debug/README.md) are the test rooms Camelot left
  inside.

## Looking something up

- [How the two games relate](../../docs/tla/sequel.md) reads The Broken Seal in this
  game's code: what it grew from, and what both still carry.
- The [glossary](../../docs/tla/glossary.md) has every word these pages share.
- [Build it yourself](../README.md#build-it-yourself) gets you a ROM you made yourself.
