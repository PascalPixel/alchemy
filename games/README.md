# Introduction

Alchemy rebuilds the Game Boy Advance games Golden Sun: The Broken Seal and
Golden Sun: The Lost Age byte for byte from readable C, assembly and editable
pictures, music and text, in all six languages they were released in. These
pages explain how the games work, read from that source: what the player sees,
what the game does to make it happen, and where in the code it does it.

[The Broken Seal](tbs/README.md) is rebuilt completely, and its pages cover
the whole game. Its front page explains how the game is put together, one scene
at a time, and four section pages go deeper:

- [System](tbs/system.md): start-up, sequences, frames and tasks, memory,
  overlays, saving and sound.
- [Battle](tbs/battle.md): how a battle runs, how damage and healing are worked
  out, and how the battle effects are drawn.
- [Field](tbs/field.md): walking around, scenes and their maps, and what
  happens in them.
- [User Interface](tbs/user_interface.md): windows and the messages typed into
  them, the field menu and its screens, the Pause menu, shops and inns.

[Places](tbs/src/field/scenes.s) has a page for every place the party can
walk around in, in story order, with a recording of every event that happens
there. The [glossary](tbs/glossary.md) explains the words the pages share.

The Lost Age is still being rebuilt; its parts that are not yet rewritten in C
are not described here.

To build the games yourself, see [Building Alchemy](../INSTALL.md);
[Matching](tbs/src/MATCHING.md) explains how C is written so that it compiles
to exactly the original bytes.

<img src="https://alchemy.pascalpixel.com/PROGRESS_CHART.png" width="838" alt="How much of each game's code and data is rebuilt, day by day since 16 July 2026" />
