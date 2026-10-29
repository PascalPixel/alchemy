# Introduction

Alchemy rebuilds the Game Boy Advance games Golden Sun: The Broken Seal and
Golden Sun: The Lost Age byte for byte from readable C, assembly and editable
pictures, music and text, in all six languages they were released in:
Japanese, English, German, Spanish, French and Italian. Every build is
checked against the original cartridges' hashes, bugs included, so the source
is the game as it shipped. The Broken Seal is complete, and these pages are
written from its source; The Lost Age is still being rebuilt.

The pages explain how The Broken Seal works, from the moment the console is
switched on to a battle's spoils, with each idea explained before anything
leans on it and everything the player can see shown as a recording of the
game itself. Whoever wants the code reads the code; the pages say how its
parts fit together, a level above it.

The chapters run in this order:

1. [The Broken Seal](tbs/README.md), the first chapter, explains the four
   mechanisms the whole game runs on, each inside the one before, and
   follows one moment of the game through all four.
2. The four divisions of the code have a page each, in the same shape:
   [System](tbs/system.md), the base everything else runs on;
   [Battle](tbs/battle.md), a fight from its opening to its spoils;
   [Field](tbs/field.md), everything outside a battle; and
   [Interface](tbs/interface.md), the windows, menus and shops.
3. Each folder of the source has a guide of its own, one level closer to the
   code, which the division pages link to section by section;
   [Source](tbs/src/README.md) says what each folder holds and where its code
   lies on the cartridge.

Beside the chapters, [Places](https://alchemy.pascalpixel.com/tbs/places) has
a page for every scene in story order, with its map and a recording of each
of its events; [Unused](https://alchemy.pascalpixel.com/tbs/unused) collects
what the game keeps but never plays; and the [glossary](tbs/glossary.md)
defines the words every page shares, in the order a newcomer meets them, with
the source of each word.

To build the games yourself, see [Building Alchemy](../INSTALL.md);
[Matching](tbs/src/MATCHING.md) explains how the C is written so that it
compiles to exactly the original bytes.

<img src="https://alchemy.pascalpixel.com/PROGRESS_CHART.png" width="838" alt="How much of each game's code and data is rebuilt, day by day since 16 July 2026" />
