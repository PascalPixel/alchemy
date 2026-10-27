# The Broken Seal's source

[The Broken Seal](../glossary.md#the-two-games) is the first Golden Sun game, and this folder is its
source: C and assembly that compile back into the cartridge's exact bytes
(a [matching decompilation](../glossary.md#matching-decompilation)). A Game Boy Advance cartridge holds the
program as one long run of bytes, and Golden Sun's program is laid out in
parts: [banks](../glossary.md#bank) of code the CPU runs straight from the cartridge, and the
code of each [scene](../glossary.md#scene), which is copied into memory only while the [party](../glossary.md#the-party) is
there. The source follows that layout, so a folder tells you where its code
lies on the cartridge. The cartridge keeps no file or folder names, so every
name here is ours, chosen for what the folder holds.

## What each folder holds

The folders divide the game by what it does, in the order the cartridge lays
their banks out.

The field is split four ways. [Field](field/README.md) is the engine under
every town, dungeon and the world map: the [actors](../glossary.md#actor), movement, and
loading, scrolling and drawing the map grid. [Running a scene](overworld/README.md),
the `overworld` folder, runs
one scene on that engine, from its set-up to the [exit](../glossary.md#exit) the party leaves
by, with random encounters, field [Psynergy](../glossary.md#psynergy-and-pp) and the commands events
use; `Monitor_Run`, the loop that runs one scene after another, is there too.
[Scenes](scene/README.md) holds the scenes, one [overlay](../glossary.md#overlay) each: [actor](../glossary.md#actor)
templates, [triggers](../glossary.md#trigger) and [events](../glossary.md#event). [Maps](map/README.md)
holds the [maps](../glossary.md#map) as data.

[Battles](battle/README.md) runs a fight: turn order, the enemies' choices,
what each ability does to its targets and the spoils.
[Battle animations](battle_anim/README.md) draws what each ability shows on screen,
one function for each animation.

[Windows](window/README.md) draws the framed boxes text appears in, decodes
the compressed messages and types them out, and holds the screens built from
windows, among them the battle menus, the file list and the name entry screen.
[Menus](menu/README.md) holds the field menu's Psynergy, [Djinn](../glossary.md#djinn), Item and
Status screens, and three overlays: the Title, the File Screen and the
Battle Arena. [Shops](shop/README.md) holds every counter that buys,
sells, repairs or heals.

[Game rules](game/README.md) holds the party's state and the rules other
code asks about: units and their stats, items and inventories, levels,
classes, Djinn and the [game flags](../glossary.md#game-flag), with the `.tsv` tables that define every
item, ability, Djinni, enemy, [summon](../glossary.md#summons) and class.

[Runtime](runtime/README.md) starts the console and runs every [frame](../glossary.md#frame)
through `WaitTicks`, with the keypad, the [heap](../glossary.md#heap-and-pools), screen fades, [sprites](../glossary.md#sprite-and-oam), saving and
the link cable. [System](system/README.md) holds the memory map, the
[far-call](../glossary.md#far-call) tables, the overlay stubs, the flash save library and the resource
directory, the numbered list through which every picture, table and overlay
is found. [Graphics](graphics/README.md) holds most of the pictures, the
title intro, the credits and the handwritten decoders. [Sound](sound/README.md)
holds the song layer and Nintendo's MusicPlayer2000 driver; the songs and
samples are in [`games/tbs/sound`](../sound). [Debug rooms](debug/README.md)
holds the three test scenes the ROM keeps.

## Banks and folders

Each large bank of the [main image](../glossary.md#main-image) is one folder, and each small bank's code
sits in the folder it belongs with.

`main.ld` lays the main image out in fourteen banks, each starting at an
address fixed per [edition](../glossary.md#editions) and opening with a far-call table; the
glossary's [bank](../glossary.md#bank) entry says why. Code in another bank calls a function
through its table entry, by its name with `Far` added
(`Battle_RunFar`); inside a bank, functions call each other
directly. Every bank calls the runtime bank's functions directly instead,
and its table serves only the overlays
([Engine](../engine.md#banks-and-far-calls)).

Which folder each bank's code comes from is the object list,
[`objects.tsv`](../objects.tsv), whose `place` column names the bank
(`rom_main_00`, numbered from 0 in cartridge order) as
[`main.ld`](../main.ld) does; the bank names are the far-call tables'
(`system/far_call/CREDITS.S` and the rest).

`graphics`, `map`, `sound` and `system` don't follow a bank: each gathers
one kind of thing (pictures, maps, sound, the memory map) from wherever
the linker places it. `map` and `debug` hold overlays, and `menu` holds
three overlays beside its bank.

## Objects and parts

Every file the build compiles or assembles is one object, and an object too
long to read as one file is written as parts that only its `.c` compiles.
Object here means the linker's object file; it has nothing to do with the
actors in a scene.

[The original](../glossary.md#the-original) program was built from pieces, objects, laid one after another in
a fixed order, and within an object the code lies in the order it was
written. Some objects are huge: the battle code runs to some 13,000 lines. Rather
than one enormous file, such an object is written as parts, each holding one
family of functions, which one short `.c` file stitches back together in
cartridge order before compiling.

[`objects.tsv`](../objects.tsv) lists the objects in link order, with the image
each belongs to (`main`, or an overlay's resource number, its row in the
resource directory), the place in that image (`rom_main_07` is the battle
bank, as the table above lists) and its sections, and
[`main.ld`](../main.ld) gives each bank its start in each edition. Within
one object, code and data follow definition order, so a function's place in
its file is its place in the ROM.

`battle/battle.c` includes its private header, `battle_internal.h`, and then
the files under `battle/battle/` in ROM order; only `battle.c` is compiled.
The guide comment at the top of such a `.c` says where each family starts,
and each part's first line says what it holds, so the tops of a folder's
files read as its table of contents. The parts are ours, not [Camelot](../glossary.md#camelot)'s
files.

| File | What it says |
| --- | --- |
| `games/tbs/objects.tsv` | every object in link order, its image, its place and its sections |
| `games/tbs/main.ld` | each bank's start in each edition |
| `<object>.c` | the guide comment: where each family of its parts starts |
| `<object>_internal.h` | what the parts share; places no bytes |
| `<object>/*.c` | the parts, each opening with what it holds |

## Overlays

The 201 scenes of the scene table share 96 overlays, unpacked one at a time
into the overlay area when a scene loads.

Each scene's people and events are code that is needed only while
the party is there, so it is kept apart from the main image as an overlay,
packed on the cartridge and unpacked into one 32 KB area of EWRAM when the
party arrives ([Engine](../engine.md#overlays)). The scene table,
`gScenes` in [field/scenes.s](field/scenes.s), has 201 scenes, and the
rooms of one dungeon often share one overlay. `Scene_LoadOverlay`
([overworld/overworld/monitor.c](overworld/overworld/monitor.c)) unpacks the overlay a scene's row
names before `FieldLoop_Run` runs it; a battle or a minigame is main-image
code and loads none.

In the source an overlay is three files in its folder:

- `entry/<scene>.s`, the [entry table](../glossary.md#entry-table-and-import-table): six [veneers](../glossary.md#veneer) the field code
  calls to start the scene and to ask it its questions, such as where its
  [entrances](../glossary.md#entrance) are ([System](system/README.md#how-an-overlay-reaches-the-rest-of-the-game)
  lists them).
- `<scene>.c`, the scene's code and tables.
- `import/<scene>.s`, the import table: the veneers the overlay calls the main
  image through, chosen from the list in `system/overlay.inc`.

Only one overlay is loaded at a time, so code several scenes use is in
headers under `scene/shared/`, and each scene that includes one compiles its
own copy. [System](system/README.md#how-an-overlay-reaches-the-rest-of-the-game)
describes the tables and how an overlay is packed.

| Folder | Overlays |
| --- | --- |
| `map/` | 90 |
| `menu/` | 3: the Title, the File Screen and the Battle Arena |
| `debug/` | 3: the test rooms |

## Headers

The shared headers hold what the whole game uses, and `include/unit` mirrors
this folder with one header for each compiled C file.

[`games/tbs/include`](../include) holds what the whole game shares: types and
structures, the flags in `flags.h`, the songs and sound effects in
`songs.h`, and the calls events are written in, in `event_layer.h`. The
`Makefile` puts `include` and its folders `battle`, `field`, `game`,
`graphics`, `lib`, `menu`, `sound` and `system` on the include path, so code
includes those headers by name alone.

[`include/unit`](../include/unit) mirrors this folder: each compiled C
file has a header at the same path under `unit` that declares what it
defines (`unit/battle/battle.h` for `battle/battle.c`; `unit/scene/` has one
for each scene). [`include/farcall`](../include/farcall) has a header for
each bank with its `Far` names. An object written as parts keeps its private
header beside it.

| Header | What it holds |
| --- | --- |
| `include/flags.h` | the [game flags](../glossary.md#game-flag) |
| `include/songs.h` | the songs and sound effects |
| `include/event_layer.h` | the calls events are written in |
| `include/unit/<path>.h` | what the C file at `<path>.c` defines |
| `include/farcall/` | each bank's `Far` names |

## C and assembly

Nearly all the code is C from one of three [compilers](../glossary.md#the-compilers); assembly stays only
where its instructions prove no compiler wrote it, and for the stubs.

In tbs-en, 95% of the bytes of code come from C. The processor has two
instruction sets: Thumb, compact 16-bit instructions, in which most of the
game is written, and ARM, its full 32-bit instructions, used where speed
matters. alchemy-gcc, a GCC of July 2000, compiles the game's Thumb C. Three
files written for ARM
(`window/glyph_arm.c`, `field/update.c` and `field/vertices.c`) are compiled
by GCC 2.95.2, and the sound driver's C and the flash save library by [pret](../glossary.md#pret)'s
`old_agbcc`. `make modern` also builds the C with a current GCC.
[Matching](MATCHING.md) explains how a build is judged against the
cartridge and why some of the C is written as it is.

Code stays assembly only where its instructions show that none of those
compilers wrote it. Each such file states that proof in comment lines at
its top that begin `@ Why`; assembly counts as source only with a proof
([Matching](MATCHING.md#the-other-markers)). The proofs are instructions
the compilers never emit: a
branch on a shift's carry (`runtime/trig.s`,
`system/resource/decode_byte_lz.s`), a load into `pc` from a table the code
rewrites (`graphics/tile/decode.s`), a switch from Thumb to ARM inside a
routine (`sound/mixer.s`), a write to the processor's mode
(`system/entry.s`). This handwritten code is about 16 KB, mostly decoders,
the sound mixing and the routines copied into [IWRAM](../glossary.md#iwram-and-ewram). The few handwritten
helpers inside the C are each marked `CAMELOT_ASM` with their proof.

The rest of the assembly, about 50 KB, is stubs: the 8-byte entries of the
far-call tables in `system/far_call/` (`.S` files, which the C preprocessor
reads first) and of every overlay's entry and import tables. The compiler's
library routines come from libgcc, built from alchemy-gcc's sources; two
overlays use its double-precision arithmetic.

Most `.s` files place data, not code: a picture built from the PNG beside it,
a table from its `.tsv`, or a map, under the name the code uses.

| Code | Compiler | Where |
| --- | --- | --- |
| the game's Thumb C | alchemy-gcc, a GCC of July 2000 | everywhere else |
| ARM C | GCC 2.95.2 | `window/glyph_arm.c`, `field/update.c`, `field/vertices.c` |
| the sound driver's C, the flash library | pret's `old_agbcc` | `sound/`, `system/save/` |
| handwritten assembly, about 16 KB | | decoders, sound mixing, IWRAM routines |
| stubs, about 50 KB | | `system/far_call/`, each overlay's entry and import tables |

## Where to start reading

`System_Initialize`, in [runtime/runtime/system.c](runtime/runtime/system.c),
is the first C to run; it ends by calling `Monitor_Run`, which is short enough
to read whole. [Runtime](runtime/README.md) follows the same path.

To find the code behind something in the game, search for its English name.
Each animation's comment names the abilities that play it, so searching
`battle_anim/` for Quake finds [battle_anim/parts/quake.c](battle_anim/parts/quake.c). Each
place's code is the scene named for it. The game's text is in
[`games/tbs/text`](../text), a PO catalog for each language, where each message
carries its linker name: "(No saved data)" is `MsgSaveNoData`, which leads to
the save file list in
[window/screens/save.c](window/screens/save.c).
