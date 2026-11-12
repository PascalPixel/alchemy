# The Broken Seal's source

This page is for finding things. Everything is under `games/tbs`: `src/`
is the code, the pictures and the maps, `include/` is the names,
`text/` is every line the game can show, and `sound/` is the music.

## Finding the code for something in the game

Start from the text. Every line the game shows has a name, and the name
leads to the code that shows it.

1. Search [`text/en.po`](../text/en.po) for a few words of the line. Its
   entry opens with its name, such as `MsgValeYouGuysAreSoLuckyGaret`.
2. Search the source for that name. It turns up in the scene that uses it:

   ```c
   { TRIGGER_TALK, ACTOR_GARETS_SISTER, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreSoLuckyGaret) },
   ```

That works for anything with words in it, menus and battle lines included.
For the rest:

| To find | Open |
| --- | --- |
| a town's people, lines and events | its file in `src/scene/`, such as [vale.c](scene/vale.c) |
| the inside of a house | follow the door: the town's exit row names the scene, such as `CottageRoomsC` |
| what an item, ability, enemy, class or Djinni is | its row in a `src/game/*_table.c` |
| what a shop sells | [src/shop/shop/stock.c](shop/shop/stock.c) |
| how an ability looks | its file in `src/battle_effect/battle_effect/`, whose opening comment lists the abilities it draws |
| a picture | a PNG under `src/graphics/` |
| a tune | a MIDI file under [`sound/songs`](../sound/songs) |
| a map | `src/map/map_NNN/map.txt` ([Maps](map/README.md#finding-a-scenes-map) says how to find its number) |
| the name of a flag, sound, scene, item or sprite | a header in `include/`: `flags.h`, `sounds.h`, `scene_ids.h`, `item_ids.h`, `sprite_ids.h` |

To read the game in the order it runs, start at `System_Initialize` in
[system/start_up.c](system/system/start_up.c), which hands over to the main
routine in [system/main_routine.c](system/main_routine.c).

## What each folder holds

A place, and walking around it ([Field](../field.md)):

- `scene/` is each place's people, lines and events
  ([Scenes](scene/README.md)).
- `field/` is the code that runs any place, with Lucky Dice and Lucky
  Wheels ([Field code](field/README.md)).
- `map/` is the ground each place is drawn on ([Maps](map/README.md)).

A fight ([Battle](../battle.md)):

- `battle/` is the fight itself ([Battles](battle/README.md)).
- `battle_effect/` is what each ability looks like
  ([Battle effects](battle_effect/README.md)).
- `game/` is the tables and the rules that read them
  ([Game rules](game/README.md)).

What the player reads ([Interface](../interface.md)):

- `window/` is the boxes and their text ([Windows](window/README.md)).
- `menu/` is the field menu's screens, the title intro and the credits
  ([Menus](menu/README.md)).
- `shop/` is shops, sanctums and inns ([Shops](shop/README.md)).

What everything stands on ([System](../system.md)):

- `system/` is start-up, the main routine, the frame, tasks and saving
  ([System code](system/README.md)).
- `sound/` is the sound code ([Sound](sound/README.md)).
- `graphics/` is every picture ([Graphics](graphics/README.md)).
- `gamepak/` is the cartridge's own parts
  ([The Game Pak](gamepak/README.md)).
- `debug/` is the developers' test scenes
  ([Debug scenes](debug/README.md)).

## How the files are cut

The layout follows the cartridge, which explains a few things that look
odd at first.

### Banks and folders

The always-loaded code lies in fourteen
[banks](../glossary.md#bank), each with a fixed start
([The Game Pak](gamepak/README.md#why-the-main-image-is-cut-into-banks)).
Each large bank is a folder. A small one sits in the folder it belongs
with: the credits and the title intro are banks of their own and live in
`menu/`.

### Objects and parts

Inside a file, functions keep the order they have on the cartridge, so
neighbours in a file are neighbours in the ROM.

Some files would be far too long to read. The battle's runs to about
13,000 lines. Such a file is written as parts, each holding one family of
functions, in a folder of the same name: `battle/battle.c` is a guide to
its parts followed by a list of them, and the code is in
`battle/battle/`. That is why several folders hold a folder of their own
name, and why the guide at the top of the `.c` file is the place to start.

### Overlays

A place's code is an [overlay](../glossary.md#overlay), loaded when the
party arrives ([System](../system.md#overlays)). Each is one `.c` file in
`scene/` or `debug/`, with its entry and import tables beside it as small
assembly files.

### Headers

[`include/`](../include) holds the names the whole game shares. A C
file's own declarations for other files are in its
[module header](../glossary.md#module-header), at the same path under
`include/module` as the file has under `src/`.

### C and assembly

Nearly all of the game is C. The files ending in `.s` are assembly, and
most of them only place data: a picture, a table, a map. A few are
routines written by hand, such as the sound mixer, the interrupt handlers
and the decoders that unpack pictures and text.
