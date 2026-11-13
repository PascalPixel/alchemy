# Welcome

Welcome, fellow Adept!

Alchemy is a reconstruction of the source code for the Game Boy Advance games
Golden Sun: The Broken Seal and Golden Sun: The Lost Age.

It is a "[matching decompilation](tbs/glossary.md#matching-decompilation)":
readable code, viewable images, music and every line of dialogue, which build
the same ROMs as the originals, byte for byte.

This lets you explore how the games are made, and helps preserve them for
posterity.

These pages are the documentation that goes with it. They explain how
both games work, from the moment the console is switched on, through their
battles, and even the Lucky Wheels. 🎰

Enjoy! They are fascinating games with unique engineering, and a great read.

## Where to start

Start with [The Broken Seal](tbs/README.md). It says which two ideas to
read first and maps everything else.

After that, go wherever you are curious:

- [System](tbs/system.md) is the base everything else runs on.
- [Battle](tbs/battle.md) is a fight, from its opening to its spoils.
- [Field](tbs/field.md) is everything outside a battle.
- [Interface](tbs/interface.md) is the windows, the menus and the shops.

Each of the four links on to guides one step closer to the code, and
[Source](tbs/src/README.md) says what each folder of it holds.

[The Lost Age](tla/README.md) has a guide of its own, in exactly the same
shape. Most of its code works as The Broken Seal's does, so each of its pages
says plainly what the sequel adds: a party of eight, the ship, the forge, and a
third game of chance.

There is more to wander through in The Broken Seal.
[Places](https://alchemy.pascalpixel.com/tbs/places) has a page for every
town and dungeon in story order, with its map and a recording of each of its
events. [Unused](https://alchemy.pascalpixel.com/tbs/unused) collects what
the game keeps but never plays. And the [glossary](tbs/glossary.md) is there
whenever a word is new.

## How far along it is

Both games are fully rebuilt in all six languages they were released in:
Japanese, English, German, Spanish, French and Italian.

## Build it yourself

Three commands, and you have a ROM you made from source.

```bash
git clone --recurse-submodules https://github.com/PascalPixel/alchemy
cd alchemy
make
```

The [README](https://github.com/PascalPixel/alchemy#build) lists what to
install first on macOS, Linux and Windows.
