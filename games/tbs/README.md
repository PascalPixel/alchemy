# The Broken Seal

![The Title: clouds and Mt. Aleph scroll past the sun and its lens flares, and the Golden Sun title card settles over them](https://alchemy.pascalpixel.com/story/title.gif)

Welcome to The Broken Seal!

This is the first Golden Sun, from the stormy night in Vale to the storm on
top of Venus Lighthouse, and all of it is here to read: 201 scenes, every
battle, and every line anyone says.

It is a wonderful program to wander around in. Camelot built it in a way
you rarely see any more, and these pages show you how to think about each
part of it before you open the code.

## Start here

Two of Camelot's ideas are worth meeting first. Once they click, the rest
of the code opens up.

1. [The main routine](../../docs/tbs/system.md#the-main-routine). One
   function runs the game from switch-on to switch-off, one mode at a
   time: the field, a battle, Lucky Dice or Lucky Wheels.
2. [Waiting and events](../../docs/tbs/system.md#waiting-and-events). No loop at the top
   turns once a tick. The tick loop is inside one function, `sySynch()`:
   code that needs time to pass calls it, and it runs that many ticks of
   the game and returns. What it runs each tick is a short list of
   functions, the events. This is the part that will surprise you.

## What runs

- [The field engine](../../docs/tbs/field.md) runs a town, a dungeon or the
  World Map: who stands where, what a talk or a step sets off, and how you
  leave.
- [The battle engine](../../docs/tbs/battle.md) runs one fight, turn by
  turn, from its opening to the experience and coins.
- [Lucky Dice and Lucky Wheels](src/field/README.md#lucky-dice-and-lucky-wheels)
  are the two games in Tolbi, each a mode of its own.

## What they all share

- [The system library](../../docs/tbs/system.md) is start-up, the main
  routine, events, overlays and [saving](../../docs/tbs/system.md#saving).
- [The menu engine](../../docs/tbs/interface.md) is the windows, the menus
  and the shops.
- [Sound](src/sound/README.md) is the music and how it comes back after a
  jingle.
- [The game logic](src/game/README.md) is the units, items, Djinn and
  classes.
- [Graphics](src/graphics/README.md) is how a picture is stored: formats,
  colours and sizes.
- [The Game Pak](src/system/README.md) is how the cartridge is laid out.
- [Debug scenes](src/debug/README.md) are the test rooms Camelot left
  inside.

## Looking something up

- [Source](src/README.md) says which folder holds what.
- [Places](https://alchemy.pascalpixel.com/tbs/places) has a page for every
  town and dungeon, with its map and a recording of each script.
- [Unused](https://alchemy.pascalpixel.com/tbs/unused) collects what the
  game keeps but never plays.
- The [glossary](../../docs/tbs/glossary.md) has every word these pages share.
- [Build it yourself](../README.md#build-it-yourself) gets you a ROM you made yourself.
