# Glossary

Every page links here the first time it uses one of these words. They come
in the order a newcomer meets them: the game's own words, then the
project's, the engine's and the maps'.

## Golden Sun

### The two games

Golden Sun is a role-playing game for the Game Boy Advance in two parts:
The Broken Seal (2001), which these pages explain and whose builds are
named `tbs`, and The Lost Age (2002), its sequel, named `tla`. The second
continues the story with a second party on the same engine; each game has its own copy of the source under `games/`.

### Camelot

Camelot Software Planning made both games and Nintendo published them.
Their source was never released.

### The party

The party is the characters travelling together, led by Isaac: in The
Broken Seal, Isaac, Garet, Ivan and Mia. The code numbers all eight
characters of both games, `ISAAC` (0) to `PIERS` (7) in `character_ids.h`,
The Lost Age's Felix, Jenna, Sheba and Piers among them. A character's or an
enemy's record is a unit (`struct Unit`), its number the unit id, and a unit
in the party a member. [Game rules](src/game/README.md#words-used-here) lists
the words.

### The places, in story order

The story starts in Vale, Isaac's village under Mt. Aleph, when the
Elemental Stars are stolen from Sol Sanctum, the shrine inside the
mountain. The party follows the thieves through Vault, Bilibin, Kolima and
Imil to Mercury Lighthouse, then through Xian, Altin, Kalay and Tolbi, and
south across Gondowan to Lalivero and Venus Lighthouse, where the game
ends. The story is the order the [game flags](#game-flag) are set in, so
pages list places in that order; [Places](src/field/map_headers.s) lists every map.

### Psynergy and PP

Psynergy is the game's magic, in battle (Quake, Cure) and on the field
(Move, Growth, Reveal), where it pushes, grows or uncovers things in a map.
It is uncountable: a character uses Psynergy, never "a Psynergy". Each use
costs PP (Psynergy Points), which come back as the party walks outside
towns and at inns. [World](world.md#field-psynergy) explains field
Psynergy.

### The four elements

Venus (earth), Mercury (water), Mars (fire) and Jupiter (wind), in that
order in every table the code keeps per element (`enum Element`). Every
character, Djinni, summon and most Psynergy belongs to one; Isaac is
Venus, Garet Mars, Ivan Jupiter and Mia Mercury.

### Power and Resist

Each fighter has a Power and a Resist in each element, capitalized as the
status screen writes them. Power raises the damage and healing of what it
casts in that element, and Resist lowers the damage it takes from it.
[Battles](src/battle/README.md#the-element-multiplier) gives the formula.

### Djinn

Djinn are elemental creatures the party finds and collects; one is a
Djinni, several are Djinn. A Djinni is set on a character, where it
raises the character's stats and, with the others set there, decides the
character's class. Unleashed in battle, it does its own move and goes on
standby; a summon spends Djinn on standby, which are then recovering for a
few turns before they are set again. [Game rules](src/game/README.md) keeps
their tables.

### Summons

A summon calls up an elemental being (Venus, Ramses, Judgment and the
others) by spending one to four Djinn on standby. Afterwards those Djinn
recover, and the summoner gains Power in the element it spent most Djinn
of. [Battles](src/battle/README.md#commands-and-ability-ids) shows how a
summon is chosen.

### Rounds and turns

A round is every unit in a battle acting once; a turn is one unit's action
within it, built as a `struct BattleTurn` while it resolves. What pret
calls a battle turn is this game's round. [Battles](src/battle/README.md#rounds)
explains both.

### Battle animation

The picture an ability plays over a battle, drawn by code: Quake's shaking
ground, a summon's arrival. It decides when a hit shows, never what it
does; pret uses the same word (`battle_anim.c`), and this game's are
`BattleAnim_` functions. [Battle animations](src/battle_anim/README.md) explains
them.

## The project

### Matching decompilation

Alchemy is C and assembly written by hand that compiles back into exactly
the bytes on the cartridges. A build passes only when each edition's
SHA-1 hash equals the one recorded for the original in `rom.sha1`, so the
code has to produce the same instructions, not just the same behaviour.
That is why the source sometimes keeps an unusual form, or reads one
buffer through several structs, where plainer C would do the same thing.
[README](../../README.md) introduces the project and
[Matching](src/MATCHING.md) explains the work.

### The original

The game as [Camelot](#camelot) shipped it: each edition's cartridge
image, which the build must reproduce byte for byte. A bug "in the
original" is kept in the code, because fixing it would change the bytes.

### Editions

Each game was released in six languages, and each is an edition with a
build of its own: `tbs-ja`, `tbs-en`, `tbs-de`, `tbs-es`, `tbs-fr` and
`tbs-it` for Japanese, English, German, Spanish, French and Italian, and
`tla-ja` to `tla-it` for The Lost Age, twelve in all. The source is
written from the Japanese edition. The other five, English, German,
Spanish, French and Italian, are the international editions, written as
their measured differences from it, chosen with `LANGUAGE` from
`edition.h`. Code that changed between The
Broken Seal's editions is grouped in four patch levels, `TBS_PATCH_1` (every
international edition) to `TBS_PATCH_4` (Spanish and Italian), which are the
project's groups, not Camelot's version numbers.

### pret

pret is the group behind the Pokémon decompilations. Alchemy follows their
conventions where they settle a question: naming, the `BUG` and `BUGFIX`
markers, and the words of the map layer, which come from pokeemerald. The
games' sound and save code is built with pret's `old_agbcc` compiler.

### The compilers

The game's Thumb C builds with alchemy-gcc, a GCC of July 2000 in the
pinned `alchemy-gcc` submodule, its few ARM files with GCC 2.95.2, and its
sound and save code with `old_agbcc`; GNU's assembler 2.10 assembles their
output. `make modern` also compiles the C with a current GCC, only to check
it. [Matching](src/MATCHING.md) explains how they are matched.

### FAKEMATCH

A tag on code kept in an unusual form only because the plain C compiles to
different instructions from the original's. It names the plain form and
how many instructions it gets wrong. [Matching](src/MATCHING.md#fakematch)
lists the kinds.

### BUG, BUGFIX and UBFIX

`// BUG:` marks a bug of the original game, kept as it is; a fix, where
there is one, sits behind `BUGFIX`, which is off. `// UB:` marks undefined
behaviour kept in the form that matches, with the defined form behind
`UBFIX`. [Matching](src/MATCHING.md#the-other-markers) has the rest of the
markers.

### Unused

`UNUSED` marks a function nothing calls, kept because its bytes are on the
cartridge. A comment that opens with `Unused:` marks something the game
can never reach, and says why; a thing the player would have seen also
carries `Unused` in its name (`EventScript_UnusedGaretJoinsIsaac`). What only
the debug menus reach is not unused: its comment says so instead.
[Unused](https://alchemy.pascalpixel.com/tbs/unused) collects them.

### XXX

`XXX:` marks what isn't known yet, with the evidence that would settle it,
and `XXX: likely` a reasoned guess. Each folder guide gathers its open
questions in its last section, Evidence and open questions.

## The engine

### Frame

One picture of the screen. The console draws about 59.73 a second, line by
line, then pauses in the vertical blank (VBlank), when the game hands over
the next picture. Durations in the code are counted in frames, and code
lets time pass with `WaitFrames(n)`. [Engine](engine.md) opens with it.

### IWRAM and EWRAM

The console's two kinds of work memory: IWRAM, 32 KB inside the processor
chip and the fastest it has, and EWRAM, 256 KB beside it and slower. The
program, pictures and music stay on the cartridge, which can only be read.
[System](src/system/README.md#memory-map) shows what lives where.

### Background layer

One of the console's four tiled pictures, `BG0` to `BG3`, that scroll as a
whole. A town or dungeon is drawn on three of them, and the world map on
the two the hardware can scale and rotate.
[World](world.md#map-layers) explains the layers.

### Sprite and OAM

A sprite is a small picture the hardware draws over or between the
background layers; OAM, the object attribute memory, is the hardware's list
of them, rebuilt every frame. [Runtime](src/runtime/README.md#the-frame-loop)
builds it.

### Main image

The program the processor runs straight from the cartridge, about 1 MB of
code in the English edition, laid out in [banks](#bank). The rest of the
code runs from RAM: a map's [overlay](#overlay) in EWRAM, and the routines
copied into IWRAM because they must run fast.

### Bank

One of the fourteen stretches the main image is cut into, each starting at
an address fixed per edition with room to spare after it, and opening with
a [far-call](#far-call) table. Because other banks call only that table,
one bank's code can grow or shrink without moving anything another bank
depends on. Distance is not the reason: the processor's call instruction
reaches 4 MB and the program's farthest call about 1.5 MB. How Camelot
built the banks is an open question. Each of the nine large banks has
a folder of the source. [System](src/system/README.md#why-the-program-is-cut-into-banks)
explains them.

### Far call

A call from one bank into another. It goes to an 8-byte jump in the
callee's far-call table, which leads on to the function, and the jump is
named after the function with `Far` added: `Battle_RunFar` leads
to `Battle_Run`. Every bank calls the runtime bank directly; its
table serves the overlays. [Engine](engine.md#banks-and-far-calls) explains
them.

### Veneer

A tiny piece of code that only jumps on: 8 bytes holding an instruction
that loads an address, one that jumps to it, and the address. Every
far-call table entry and every stub of an overlay's entry and import tables
is one (the `overlay_veneer` macro).

### Overlay

A packed piece of code and data that is unpacked into one 32 KB area of
EWRAM, `gOverlayArea`, when it is needed, over the one before. Each map's
code is an overlay, 96 in all, the title screen and the debug rooms among
them. Only one is loaded at a time, so overlays never call each other.
[Engine](engine.md#overlays) explains them.

### Entry table and import table

The two tables an overlay meets the main image through. The entry table
opens it: six veneers the field engine calls to ask the map its questions
(`struct MapEntries`). The import table ends it: one veneer for each
main-image function the overlay calls, holding the address of that
function's far-call entry (or, for four routines in IWRAM, of the routine
itself), since EWRAM is out of reach of a direct call.
[System](src/system/README.md#how-an-overlay-reaches-the-rest-of-the-game)
describes both.

### Heap and pools

The heap lends blocks of memory to the rest of the game from two pools,
one in IWRAM and one in EWRAM. Each hands out memory from the bottom up,
so blocks go back in the reverse of the order they were taken, and the
whole heap is emptied before each map. `Alloc` and `Free` lend and return
blocks the way pret's `malloc.h` does, and `AllocEwram` takes a short-lived
block with no heap slot from the EWRAM pool first.
[Runtime](src/runtime/README.md#the-heap) explains it.

### Heap slot

One of the 62 named words of `gHeapSlots` (`HEAP_SLOT_*`), each holding a
heap block that has to be found again: the running map's state is
`HEAP_SLOT_FIELD` (`gFieldWork`), an open menu's `HEAP_SLOT_MENU`; each slot's
global is named after the slot.

### Task

A routine run once every frame, or once every VBlank, until it is removed:
the windows' typing, the map's scrolling, a map's own callbacks. As in
pret, tasks sit in one table, `gTasks`, which holds 20, and run in the order
of their priority (`TASK_PRIORITY_*`). [Runtime](src/runtime/README.md#the-task-runner)
explains the task runner.

### Song

Every sound the game plays, music or effect, is asked for by a song
number, a row of the song table, as in pret's `songs.h`: below
`SONG_FANFARE_FIRST` (80) music, then the fanfares, and from
`SONG_SE_FIRST` (100) mostly sound effects. "BGM" names what the music
player is playing now, "music" what a map chooses.
[Sound](src/sound/README.md#what-a-song-number-means) explains the ranges.

## Maps

Most of these words are pret's, from pokeemerald, so a reader of that
project finds them in the same sense.

### Map

One row of the map table: a room, a town, a dungeon floor or the world
map, 201 in all. Each runs the code of an overlay and shows a layout, and
the field engine runs one at a time. [The front page](README.md#the-main-loop)
explains the loop of maps.

### Map section

A group of maps, 71 in all, that share their music, their area name and
one range of [scratch flags](#game-flag). [The front
page](README.md#the-map-table-and-map-sections) explains them.

### Layout

What a town or dungeon map shows and stands on: three background layers
and one grid of cells under them, 128 by 128. Several maps can share a
layout. [Layouts](src/layout/README.md) explains them.

### Cell

One 16 by 16 pixel square of a layout, a 32-bit word once loaded: which
metatile is drawn there, the shape and height of the ground, and its
metatile behavior. [Layouts](src/layout/README.md#cells) gives its parts.

### Metatile

The picture of one cell: 2 by 2 of the console's 8-pixel tiles.

### Metatile behavior

The byte of a cell that says what standing on it, walking into it or
searching it does: a wall, a ledge, a jar, the desert sand that hurts the
party. Behaviors from `MB_EVENT_FIRST` to `MB_EVENT_LAST` mean whatever the
map's own event table says; the rest mean the same on every map. [World](world.md#cells-and-metatile-behaviors)
explains them.

### Object

One of the 64 slots of `gObjects` (`struct Object`): anything in a map that
acts on its own, from the party and the townspeople to the
camera and the footprints in snow. Each runs an object script.
[World](world.md#objects-and-their-scripts) explains them.

### Object event

A person or thing a map knows by its local id: the party, and everyone and
everything its object event templates place, each a row of the map's table
that puts one on the map. Each object event is an [object](#object), but the
camera and the footprints are objects and not object events.
[Maps](src/map/README.md#who-stands-where) explains them.

### Local id

An object event's number in its map: 0 to 7 the party, with the leader the
player moves at 0, and from 8 (`LOCALID_FIRST_PLACED`) the object event
templates' rows in order, row n always at 8 plus n (a row that shows a
party member takes that member's id instead). Each map names its own in an
enum.

### Map event

A row of a map's event table (`struct MapEvent`): a kind, a trigger, a
condition and what to run. When the player talks, searches, steps on a
marked cell or uses Psynergy, the first row whose kind, trigger and
condition fit runs. [Maps](src/map/README.md#what-a-map-event-row-says)
lists the kinds.

### Trigger

What a map event row answers: an object event's local id for talking, a
metatile behavior for a cell, or a value the map's code raises.

### Coord event

A map event that runs when the leader steps onto, or walks into, a cell
with a given metatile behavior: a doorstep, a ladder, the edge of a path.

### Bg event

A map event that runs when the player presses A facing a cell: a jar, a
chest, a bookcase. "Bg" is pret's word for these and has nothing to do
with the background layers `BG0` to `BG3`.

### Warp

A way out of a map. Taking one ends the map: the warp names where it
leads (`VALE_PLAZA_TO_WORLD_MAP` in `map_warps.h`), and the first row for it
in the map's warp table whose condition holds gives the next map and the
warp destination there. A warp is taken from a cell, from a warp region
(a box on the map) or by a script. [The front
page](README.md#warps-and-warp-destinations) explains them.

### Warp destination

Where the party arrives in a map: a position, a facing and the camera's
limits, named by where the party comes from (`VALE_PLAZA_FROM_SANCTUM`). A
map's initialization often tests it, to play the part of a sequence the
last map sent the party to.

### Script

A function a map event names (`EventScript_EnterIsaacsHouse`), written as
one step to a line in the commands of `script.h`. It runs from top to
bottom while the walk waits, and a step that takes time, a wait, a line of
dialogue or a walk, runs those frames itself before the next line starts.
An object script is something else: the list of `SC_` commands an
[object](#object) runs each frame. "Scene" means only a cutscene.
[Maps](src/map/README.md#scripts) explains scripts.

### Game flag

One of the 4,096 yes-or-no facts saved with the game, such as a chest
being open or a Djinni found. Two ranges are scratch space, cleared as
the party enters maps and map sections. [The front
page](README.md#game-flags) explains them.
