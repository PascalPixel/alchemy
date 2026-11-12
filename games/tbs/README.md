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

1. [The main routine](system.md#the-main-routine). One routine runs the
   game from switch-on to switch-off, one sequence at a time: the field, a
   battle, Lucky Dice or Lucky Wheels.
2. [Waiting and tasks](system.md#waiting-and-tasks). Code that needs time
   simply stops and waits, and the game goes on running inside the wait. If
   you have never met a task system, this is the part that will surprise
   you.

## What runs

- [Field](field.md) is a town, a dungeon or the World Map: who stands
  where, what a talk or a step sets off, and how you leave.
- [Battle](battle.md) is one fight, turn by turn, from its opening to the
  experience and coins.
- [Lucky Dice and Lucky Wheels](src/field/README.md#lucky-dice-and-lucky-wheels)
  are the two games in Tolbi, each a sequence of its own.

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

- [Source](src/README.md) says which folder holds what.
- [Places](https://alchemy.pascalpixel.com/tbs/places) has a page for every
  town and dungeon, with its map and a recording of each event.
- [Unused](https://alchemy.pascalpixel.com/tbs/unused) collects what the
  game keeps but never plays.
- The [glossary](glossary.md) has every word these pages share.
- [Building Alchemy](../../INSTALL.md) gets you a ROM you made yourself.
