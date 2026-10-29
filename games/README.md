# Introduction

Alchemy rebuilds the Game Boy Advance games Golden Sun: The Broken Seal and
Golden Sun: The Lost Age byte for byte from readable C, assembly and editable
pictures, music and text, in all six languages they were released in. These
pages use that source to explain how The Broken Seal works.

Start at [The Broken Seal](tbs/README.md), which explains how the game is put
together and leads to four pages, one for each division of its code:
[System](tbs/system.md), [Battle](tbs/battle.md), [Field](tbs/field.md) and
[User Interface](tbs/user_interface.md).
[Places](https://alchemy.pascalpixel.com/tbs/places) has every scene, with its
maps and its events, and the [glossary](tbs/glossary.md) explains the words
the pages share.

The Lost Age is still being rebuilt and is not described here.

To build the games yourself, see [Building Alchemy](../INSTALL.md);
[Matching](tbs/src/MATCHING.md) explains how C is written so that it compiles
to exactly the original bytes.

<img src="https://alchemy.pascalpixel.com/PROGRESS_CHART.png" width="838" alt="How much of each game's code and data is rebuilt, day by day since 16 July 2026" />
