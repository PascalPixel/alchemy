# The Broken Seal's source

This folder is the source of [The Broken Seal](../glossary.md#the-two-games):
C, assembly and editable images, sounds and text that build the cartridge's
exact bytes ([matching decompilation](../glossary.md#matching-decompilation)).
The game's code lies on the cartridge in two kinds of place: the
[banks](../glossary.md#bank) of the [main image](../glossary.md#main-image),
which the processor runs straight from the cartridge, and the
[overlays](../glossary.md#overlay), each holding the code of one or more
[scenes](../glossary.md#scene) and copied into memory only while one of them
runs. Most folders are one bank or one kind of overlay, so a folder mostly
tells you where its code lies. The cartridge keeps no file or folder names, so
every name here is ours.

## What each folder holds

The folders are grouped by the four divisions the game's credits give its
programming, as [the front page](../README.md#the-four-divisions) describes
them.

[System](../system.md), the base code every other part runs on:

- [Runtime](runtime/README.md) starts the console and lets time pass through
  [`wait()`](../glossary.md#wait), one [frame](../glossary.md#frame) at a time,
  with the [tasks](../glossary.md#task), the
  [heap](../glossary.md#heap-and-pools), the buttons, saving and the link
  cable.
- [Cartridge and memory](cartridge/README.md) holds the memory map, the
  [far-call](../glossary.md#far-call) tables, the overlay stubs, the flash
  save library and the [resource directory](../glossary.md#resource-directory),
  the numbered list through which every image, table and overlay is found.
- [Graphics](graphics/README.md) holds most of the images, the title intro,
  the credits and the handwritten decoders.
- [Sound](sound/README.md) holds the sound layer and Nintendo's
  MusicPlayer2000 driver; the music and samples themselves are in
  [`games/tbs/sound`](../sound).

[Field](../field.md), everything outside a battle:

- [Running a scene](overworld/README.md), `field_sequence.c` and
  `event_commands.c` in `field`, runs one scene from its set-up to the
  [exit](../glossary.md#exit) the party leaves by, with random encounters,
  field [Psynergy](../glossary.md#psynergy-and-pp) and the event commands.
  `MainRoutine_Run`, [the main routine](../glossary.md#main-routine), which
  runs one [sequence](../glossary.md#sequence) after another, is in the same
  bank, in [main_routine.c](system/main_routine.c).
- [The field bank](field/README.md) is the code under every town, dungeon and
  the world map: the [actors](../glossary.md#actor), walking, and loading,
  scrolling and drawing the map.
- [Scenes](scene/README.md) holds the scenes' overlays: who stands where, the
  [triggers](../glossary.md#trigger) and the [events](../glossary.md#event).
- [Maps](map/README.md) holds the [maps](../glossary.md#map) as data.
- [Debug scenes](debug/README.md) holds the ten debug scenes the cartridge
  keeps.

[Battle](../battle.md), a fight and the rules it asks about:

- [Battles](battle/README.md) runs a fight: the order of actions, the enemies'
  choices, what each ability does to its targets and the spoils.
- [Battle effects](battle_effect/README.md) draws what each ability shows on
  screen, one function for each battle effect.
- [Game rules](game/README.md) holds the party's state and the rules other
  code asks about: [units](../glossary.md#unit) and their stats, items,
  levels, classes, [Djinn](../glossary.md#djinn) and the
  [game flags](../glossary.md#game-flag), with the C tables that define every
  item, ability, Djinni, enemy, [summon](../glossary.md#summons) and class.

[User Interface](../user_interface.md), the windows, menus and shops:

- [Windows](window/README.md) draws the boxes text appears in, decodes the
  stored messages and types them out, and holds the screens built from
  windows, among them the battle menus, the file list and name entry.
- [Menus](menu/README.md) holds the field menu's Psynergy, Djinn, Item and
  Status screens, and three overlays: the Title, the File Screen and the
  Battle Arena.
- [Shops](shop/README.md) holds every counter that buys, sells, repairs or
  heals, and the inn.

## Banks and folders

[`main.ld`](../main.ld) lays the main image out in fourteen banks, each after
the first starting at an address fixed per
[edition](../glossary.md#editions); other banks call a bank's functions
through its far-call table, except the runtime bank's, which they call
directly; its table, at the start of the cartridge, serves only the scenes
([System](../system.md#banks-and-far-calls)). Each large bank is one folder,
and each small bank's code sits in the folder it belongs with. The object
list, [`objects.tsv`](../objects.tsv), names each object's bank in its
`place` column, as `main.ld` does, and the banks take their names from their
far-call tables.

| Place | Far-call table | Folder |
| --- | --- | --- |
| `rom_main_00` | `SYSTEM.S` | `runtime`, with the decoders and the flash library from `cartridge` |
| `rom_main_01` | `FIELD.S` | `field` |
| `rom_main_02` | `WINDOW.S` | `window` |
| `rom_main_03` | `GAME.S` | `game` |
| `rom_main_04` | `FIELD_SEQUENCE.S` | `field` (`field_sequence.c`, `event_commands.c`) |
| `rom_main_05` | `MENU.S` | `menu` |
| `rom_main_06` | `SHOP.S` | `shop` |
| `rom_main_07` | `BATTLE.S` | `battle` |
| `rom_main_08` | `BATTLE_EFFECT.S` | `battle_effect` |
| `rom_main_09` | `CREDITS.S` | `graphics/credits` |
| `rom_main_10` | `TITLE.S` | `graphics/title` |
| `rom_main_11` | `LUCKY_DICE.S` | `field/lucky_dice.c` |
| `rom_main_12` | `LUCKY_WHEELS.S` | `field/lucky_wheels.c` |
| `rom_main_13` | `SOUND.S` | `sound` |

After the banks come the sound driver with the music and samples, then the
data the resource directory finds, with `SPRITE.S`, a fifteenth far-call
table whose one entry is the resource metadata lookup. `graphics`, `map`, `sound` and `cartridge`
also gather one kind of thing (images, maps, sound, the memory map) from
wherever the linker places it.

## Objects and parts

Every file the build compiles or assembles is one object. Object here means
the linker's object file; it has nothing to do with the actors in a scene.
[`objects.tsv`](../objects.tsv) lists the objects in link order, with the
image each belongs to (`main`, or an overlay's number in the resource
directory), its place in that image and its sections. Within one object,
code and data follow the order they are defined in, so a function's place in
its file is its place on the cartridge.

Some objects are too long to read as one file: the battle object runs to
some 13,000 lines. Such an object is written as parts, each holding one
family of functions. `battle/battle.c` includes its private header,
`battle_internal.h`, and then the files under `battle/battle/`; only
`battle.c` is compiled. The `.c` opens with a guide to its parts and then
includes them in cartridge order, and each part opens with what it holds, so
the tops of a folder's files read as its table of contents. The parts are
ours, not [Camelot](../glossary.md#camelot)'s files.

| File | What it says |
| --- | --- |
| `<object>.c` | the guide comment, then its parts in cartridge order |
| `<object>_internal.h` | what the parts share; places no bytes |
| `<object>/*.c` | the parts, each opening with what it holds |

## Overlays

In the source an overlay ([System](../system.md#overlays)) is three files in
its folder:

- `entry/<scene>.s`, the
  [entry table](../glossary.md#entry-table-and-import-table): the scene's
  set-up and the functions that hand the field sequence its entrances, exits,
  actor placements and triggers ([Scenes](scene/README.md#the-entry-table)).
- `<scene>.c`, the scene's code and tables.
- `import/<scene>.s`, the import table: the [veneers](../glossary.md#veneer)
  the overlay calls the main image through, chosen from the list in
  `system/overlay/imports.inc`.

Code several scenes use is in headers under `scene/shared/`, and each scene
that includes one compiles its own copy.
[Cartridge and memory](cartridge/README.md#how-an-overlay-reaches-the-rest-of-the-game)
describes how an overlay is packed and loaded.

| Folder | Overlays |
| --- | --- |
| `scene/` | 90 |
| `menu/` | 3: the Title, the File Screen and the Battle Arena |
| `debug/` | 3: the test scenes, Increase item and level, and Menu test |

## Headers

[`games/tbs/include`](../include) holds what the whole game shares: types,
structures, flags, sounds and the event calls. The `Makefile` puts `include`
and its folders `battle`, `field`, `game`, `graphics`, `lib`, `menu`, `sound`
and `system` on the include path, so code includes those headers by name
alone. Each compiled C file, a module, has a
[module header](../glossary.md#module-header) at the same path under
[`include/module`](../include/module) (`module/battle/battle.h` for
`battle/battle.c`), and an object written as parts keeps its private header
beside it.

| Header | What it holds |
| --- | --- |
| `include/flags.h` | the [game flags](../glossary.md#game-flag) |
| `include/sounds.h` | the music, jingles and sound effects |
| `include/event.h` | the calls events are written in |
| `include/module/<path>.h` | what the C file at `<path>.c` defines |
| `include/farcall/` | each far-call table's `Far` names |

## C and assembly

About 95% of the bytes of code across the six editions come from C, with the
far-call and overlay stubs counted apart; the three
[compilers](../glossary.md#the-compilers) and their files are below. The three
ARM files are copied into RAM before they run. [Matching](MATCHING.md)
explains how a build is judged against the cartridge and why some of the C is
written as it is.

| Code | Compiler | Where |
| --- | --- | --- |
| ARM C | GCC 2.95.2 | `window/glyph_arm.c`, `field/actor_update_arm.c`, `field/world_map_lines.c` |
| the sound driver's C, the flash library | `old_agbcc` | `sound/`, `gamepak/flash/` |
| the game's Thumb C | alchemy-gcc, a GCC of July 2000 | every other C file, and libgcc |

Code stays assembly only where its instructions show that none of those
compilers wrote it. Each such file states that proof in comment lines at its
top that begin `@ Why` ([Matching](MATCHING.md#the-other-markers)). The
proofs are instructions the compilers never emit: a branch on a shift's carry
(`system/trig.s`, `system/decompress_byte_lz.s`), a load into `pc`
from a table the code rewrites (`graphics/tile/decode.s`), a switch from
Thumb to ARM inside a routine (`sound/mixer.s`), a write to the processor's
mode (`system/entry.s`). Most of this handwritten code is decoders, the
sound mixing and the routines copied into
[IWRAM](../glossary.md#iwram-and-ewram). The few handwritten helpers inside
the C are each marked `CAMELOT_ASM` with their proof.

The rest of the assembly, about 50 KB in each edition, is stubs: the 8-byte
entries of the far-call tables in `gamepak/far_call/` (`.S` files, which the
C preprocessor reads first) and of every overlay's entry and import tables.
The compiler's library routines come from libgcc, built from alchemy-gcc's
sources; two overlays use its double-precision arithmetic. Most `.s` files
place data, not code: an image built from the PNG beside it, a table from its
`.tsv`, or a map, under the name the code uses.

## Where to start reading

`System_Initialize`, in [system/system/start_up.c](system/system/start_up.c),
is the first C to run; it ends by calling `MainRoutine_Run` through its
far-call stub. The main routine, in
[system/main_routine.c](system/main_routine.c), is
short enough to read whole. [Runtime](runtime/README.md#start-up) follows
power-on step by step.

To find the code behind something in the game, search for its English name.
Each battle effect's comment names the abilities that play it, so searching
`battle_effect/parts/` for Quake leads to
[quake.c](battle_effect/parts/quake.c). Each place's code is the overlay in
`scene/` named for it, such as [scene/bilibin.c](scene/bilibin.c). The game's
text is in [`games/tbs/text`](../text), a PO catalog for each language, where
each message carries its linker name: "(No saved data)" is `MsgSaveNoData`,
which leads to the save file list in
[window/screens/save.c](window/screens/save.c).
