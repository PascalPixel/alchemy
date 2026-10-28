# Glossary

Every page links here the first time it uses one of these words, which come in
the order a newcomer meets them: the game's own words, then the project's,
then those of the code's four divisions, the system, the field, the battle and
the user interface, and last the names of the source's folders. Each entry ends with where its word comes from: Camelot
where Camelot has a word for the thing, else the tradition of the time, else
plain English.

## Golden Sun

### The two games

Golden Sun is a role-playing game for the Game Boy Advance in two parts: The
Broken Seal (2001), whose builds are named `tbs`, and its sequel The Lost Age
(2002), named `tla`, each with its own copy of the source under `games/`.

Evidence: Camelot: the games' titles.

### Camelot

Camelot Software Planning made both games, and Nintendo published them; their
source was never released.

Evidence: Camelot: the credits.

### The party

The characters travelling together, led by Isaac: in The Broken Seal, Isaac,
Garet, Ivan and Mia. In a battle each of them is a [unit](#unit), and a unit in
the party is a party member
([Game rules](src/game/README.md#words-used-here)).

Evidence: Camelot: the game's text says party throughout.

### The places, in story order

The story starts in Vale, under Mt. Aleph, and follows the thieves of the
Elemental Stars through Vault, Bilibin, Kolima, Imil, Mercury Lighthouse,
Xian, Altin, Kalay and Tolbi to Lalivero and Venus Lighthouse, where the game
ends. The [game flags](#game-flag) are set in story order, so pages list
places in that order; [Places](https://alchemy.pascalpixel.com/tbs/places)
shows every scene.

Evidence: Camelot: the area names in the game's text.

### Psynergy and PP

![Isaac casts Growth in battle: light gathers around him, then a spiked vine bursts out of the ground under Ivan](https://alchemy.pascalpixel.com/abilities/growth.gif)

Psynergy is the game's magic, cast in battle (Quake, Cure, Growth) and on the
field (Move, Reveal), where it pushes, grows or uncovers things. Each use costs
PP (Psynergy Points), which come back as the party walks outside towns and at
inns ([Field](field.md#field-psynergy)).

Evidence: Camelot: the game's text ("Your Psynergy Points.").

### The four elements

Venus (earth), Mercury (water), Mars (fire) and Jupiter (wind), in that order
in every table the code keeps per element (`enum Element`). Every character,
Djinni and summon belongs to one: Isaac is Venus, Garet Mars, Ivan Jupiter and
Mia Mercury.

Evidence: Camelot: the game's text.

### Power and Resist

For each element, every fighter has a Power and a Resist: Power raises the
damage and healing of what it casts in that element, and Resist lowers the
damage it takes from it ([Battles](src/battle/README.md#the-element-multiplier)).

Evidence: Camelot: the Status screen's labels.

### Djinn

![Ivan unleashes the Jupiter Djinni Gust in battle: pink wind streams across the field, then a blue blast strikes its target](https://alchemy.pascalpixel.com/abilities/gust.gif)

Elemental creatures the party collects; one is a Djinni, several are Djinn.
Set on a character, a Djinni raises its stats and helps decide its class;
unleashed in battle, it does its own move and goes on standby, ready for a
summon ([Game rules](src/game/README.md)).

Evidence: Camelot: the game's text.

### Summons

![Isaac summons Ramses: the screen wipes to a desert where a giant stone figure appears and pounds its targets with its fist](https://alchemy.pascalpixel.com/abilities/ramses.gif)

A summon spends one to four Djinn on standby to call up an elemental being
(Venus, Ramses, Judgment and the others); the Djinn then recover for a few
[turns](#turns-and-actions)
([Battles](src/battle/README.md#commands-and-ability-ids)).

Evidence: Camelot: the game's text.

## The project

### Matching decompilation

Alchemy is C and assembly written by hand that compiles back into exactly the
bytes on the cartridges, checked against `rom.sha1`, so the source sometimes
keeps an unusual form where plainer C would behave the same
([Introduction](../README.md), [Matching](src/MATCHING.md)).

Evidence: plain English; the project's own words.

### The original

The game as [Camelot](#camelot) shipped it: each edition's cartridge image,
which the build reproduces byte for byte, bugs included.

Evidence: plain English; the project's own word.

### Editions

Each game was released in Japanese, English, German, Spanish, French and
Italian, each an edition with its own build (`tbs-ja` to `tbs-it`, `tla-ja` to
`tla-it`). The source is written from the Japanese edition; the other five,
the international editions, are its measured differences, chosen with
`LANGUAGE` from `edition.h` and grouped in patch levels `TBS_PATCH_1` (all
five) to `TBS_PATCH_4` (Spanish and Italian), which are the project's groups,
not Camelot's version numbers.

Evidence: plain English; the project's own word.

### pret

The group behind other decompilations of Game Boy Advance games, whose
standard Alchemy measures itself by: what counts as matched, how the build is
laid out, the shape of names (`PascalCase` functions, `gName` globals) and the
`BUG` and `BUGFIX` markers. The names of things here are Camelot's wherever
Camelot has one.

Evidence: the group's own name.

### The compilers

The game's Thumb C builds with alchemy-gcc, a GCC of July 2000 in the
`alchemy-gcc` submodule, its few ARM files with GCC 2.95.2, and its sound and
save code with pret's `old_agbcc`; `make modern` also compiles the C with a
current GCC, only to check it ([Matching](src/MATCHING.md)).

Evidence: the compilers' own names.

### FAKEMATCH

A tag on code kept in an unusual form only because the plain C compiles to
different instructions; it names the plain form and how many instructions it
gets wrong ([Matching](src/MATCHING.md#fakematch)).

Evidence: plain English; the project's own word.

### BUG, BUGFIX and UBFIX

`// BUG:` keeps one of the original game's bugs, with a fix behind `BUGFIX`;
`// UB:` keeps undefined behaviour as it matches, with the defined form behind
`UBFIX`
([Matching](src/MATCHING.md#the-other-markers)).

Evidence: plain English, in pret's markers.

### Unused

`UNUSED` marks a function nothing calls, and a comment opening with `Unused:`
something the game can never reach, with the reason; what only the debug menus
reach is not unused. [Unused](https://alchemy.pascalpixel.com/tbs/unused)
collects them.

Evidence: plain English.

### Module header

The header under `include/module/` that declares one module's functions, at
the same path as its C file under `src/`.

Evidence: plain English; the project's own word.

### Object

One compiled unit of the build, a row of `objects.tsv`, in the linker's sense
of an object file, unrelated to a scene's actors. A large
one is written as parts in a folder of its own name beside it
(`system/system/` for `system.c`), which only it includes
([Source](src/README.md#objects-and-parts)). A part of an object is unrelated
to a part of a save file ([File and part](#file-and-part)).

Evidence: plain English; the GNU tools' "object file".

### Shared

Used by several scenes or tools, as the headers in `src/scene/shared/` are,
each scene that includes one compiling its own copy; never a synonym for
common ([Scenes](src/scene/README.md#code-shared-between-overlays)).

Evidence: plain English.

## The system

### System

The code the field, the battles and the user interface are built on: start-up,
the main routine, waiting and [tasks](#task), interrupts, the
[heap](#heap-and-pools), the [overlays](#overlay), saving and the sprite
bank's metadata reader. It is one of the code's four divisions, with Field,
Battle and User Interface; [System](system.md) explains it. Its code is in
`src/system/` ([System code](src/system/README.md)); the
[Game Pak](#game-pak)'s is beside it in `src/gamepak/`.

Evidence: Camelot: the credits, "Programming", then "System", "Battle",
"Field", "User Interface" and "Support" (`gCredits_Lines`).

### Main routine

The routine that runs everything outside the interrupts, `MainRoutine_Run`:
called at power-on by `System_Initialize`, and again by `Game_RestartQuest`,
it never returns. Each pass of its loop tears down what the last
[sequence](#sequence) set up (its tasks, its HBlank and VCount handlers, the
heap),
reads one id from the [main routine's work](#main-routines-work) and runs one
sequence: the [field sequence](#field-sequence) for a [scene](#scene), any row
of the scene table, the Title and the File Screen included; or the battle,
dice or slots sequence. A change of scene is a transition too, as going into a
battle is ([System](system.md#the-main-routine)). Its object is
`src/system/main_routine.c`, the first code of bank 04.

Evidence: tradition: メインルーチン, the period's word for the code that runs
outside the interrupts.

### Sequence

One flow of the game, run from its start to its end: the field sequence, the
battle sequence, the dice sequence (Lucky Dice) or the slots sequence (Lucky
Wheels), `SEQUENCE_` in code. One runs at a time
([System](system.md#the-main-routine)).

Evidence: tradition: ゲームシーケンス.

### Field sequence

The sequence that runs one [scene](#scene), `FieldSequence_Run`: it allocates
`gFieldWork`, loads the map, starts the actors and windows and calls the
overlay's initialize entry, then waits a frame at a time, answering the
[field requests](#field-request), until one ends the scene. The next scene is
the main routine's to start
([Field](field.md#the-field-sequence-and-its-requests)). Its object is
`src/field/field_sequence.c`, `FieldSequence_Run` its part `run.c`
([Field code](src/field/README.md#the-field-sequence-serves-requests)).

Evidence: Camelot: the credits' "Field" (`gCredits_Lines`); tradition: sequence.

### Transition and next sequence

The handover from one sequence to the next. The field sequence names the next
one, a scene or a battle, in the [main routine's work](#main-routines-work); a
battle, dice or slots sequence returns its result, and the main routine picks
the scene to go back to (`Scene_ReturnFromSequence`)
([System](system.md#the-main-routine)).

Evidence: tradition: シーケンス遷移.

### Main routine's work

The main routine's [work](#work), the one struct every sequence shares and
that outlives each of them, `gMainRoutineWork` (`struct MainRoutineWork`,
`main_routine_work.h`): a fixed struct in EWRAM, never freed and in no
[heap slot](#heap-slot); Save Game writes it with the game flags
([System](system.md#the-main-routine)).

Evidence: tradition: work, named for its owner, the main routine.

### Frame

One pass of the game's work: every [task](#task) in the frame pass runs once
and the screen is prepared, then the game halts until the next
[VBlank](#vblank) shows it. Waits, walking and animation are all counted in
frames ([System](system.md#waiting-and-tasks)).

Evidence: tradition: フレーム, one pass of a game's processing.

### Wait

Letting frames pass in the middle of a routine: `wait(n)` runs n frames and
then returns to the next line, so a sequence or an event is written as
straight-line code ([System](system.md#waiting-and-tasks)).

Evidence: tradition: WAIT, the period's word for it.

### Task

A short routine run once every [frame](#frame), or once every
[VBlank](#vblank), until it is removed: the windows' typing, the map's
scrolling, the lightning over Vale on the stormy night. The task table,
`gTasks`, holds up to 20, each with its function (`func`) and a priority that
picks its pass, the frame pass (`TASK_PASS_FRAME`, which `wait()` runs) or
the VBlank pass (`TASK_PASS_VBLANK`, which the VBlank interrupt runs), and its
order within it. A task has no state of its own: it keeps what it works on in
a [work](#work), a block in a heap slot
([System code](src/system/README.md#the-task-runner)).

Evidence: tradition: タスク and its 実行関数.

### VBlank

One refresh of the screen. The console refreshes the screen about 59.73 times
a second, and after each it pauses in the vertical blank (VBlank), whose
interrupt, the
VBlank interrupt, mixes the sound and reads the buttons whatever the game is
doing. The music keeps time in VBlanks
([System](system.md#waiting-and-tasks)).

Evidence: tradition: V ブランク, the hardware's own name.

### Slowdown

A frame that overruns one VBlank and lasts two or more: every wait,
walk and animation slows down together, while the music keeps its tempo
([System](system.md#slowdown)).

Evidence: tradition: 処理落ち.

### Work

The working memory a piece of code keeps its state in, held in a
[heap slot](#heap-slot) and named `g<Slot>Work`: the field's is `gFieldWork`,
the menus' `gMenuWork`. The main routine's is the one kept in a fixed
struct instead, [`gMainRoutineWork`](#main-routines-work).

Evidence: tradition: ワーク, working memory, as in the hardware's Work RAM,
used here for a system's block.

### Heap and pools

The heap lends blocks of memory from two pools, one in IWRAM and one in EWRAM;
the last block taken is the first given back, and the whole heap is emptied
before each sequence ([System code](src/system/README.md#the-heap)).

Evidence: plain English.

### Heap slot

One of 62 numbered places that each hold a heap block other code has to find
again, such as the running scene's [work](#work) (`gHeapSlots`).

Evidence: plain English; the project's own word.

### IWRAM and EWRAM

The console's two areas of RAM: IWRAM, 32 KB inside the processor and the
fastest it has, and EWRAM, 256 KB beside it and slower
([System code](src/system/README.md#memory-map)).

Evidence: tradition: the hardware's own names.

### Main image

The code the processor runs straight from the cartridge, about 1 MB in
[banks](#bank); the rest runs from RAM, a scene's [overlay](#overlay) in EWRAM
and the fast routines in IWRAM.

Evidence: plain English; the project's own word.

### Game Pak

The cartridge as hardware: its ROM, cut into [banks](#bank) with their
far-call tables, the [resource directory](#resource-directory), and the
[flash chip](#flash-chip) that holds the saves
([The Game Pak](src/gamepak/README.md)). Folder, file and symbol names say
Game Pak (`src/gamepak/`); prose may say cartridge for the physical thing.

Evidence: Nintendo: "Game Pak", the instruction booklets' word for the
cartridge; tradition: GBATEK's "GamePak ROM", "GamePak SRAM" and "GamePak
Waitstates".

### Flash chip

The 64 KB chip on the cartridge the three save [files](#file-and-part) are
written to, 16 sectors of 4 KB, driven by the library in
`src/gamepak/flash/` for whichever maker's chip the cartridge holds
([The Game Pak](src/gamepak/README.md#the-flash-chip)).

Evidence: the hardware's word, GBATEK's "GamePak Flash ROM"; the library's
file names are the project's, from the chip makers' names as GBATEK lists them
(Atmel, SST, Macronix).

### Bank

One of the fourteen stretches the main image is cut into, each starting at a
fixed address, with space after its code, and opening with a
[far-call](#far-call) table, so
one bank can grow without moving anything another depends on
([The Game Pak](src/gamepak/README.md#why-the-main-image-is-cut-into-banks)).
Every bank is always in place; none is switched in or out.

Evidence: plain English; the project's own word.

### Bank folder

A folder of `src/` that holds one or more banks' code, grouped by the
credits' four programming divisions: the System's `system/` (bank 00, the
RAM side and the main routine) beside `gamepak/` (the Game Pak, not a
division); the Field's `field/` (banks 01, 04, 11 and 12); the Battle's
`battle/` (bank 07) and `battle_effect/` (bank 08); and the User
Interface's `window/` (bank 02), `menu/` (bank 05, with the credits, bank
09, and the Title's intro, bank 10) and `shop/` (bank 06). `game/` (bank 03,
[Game rules](#game-rules)) and `sound/` (bank 13, [Sound](#sound)) belong to
no programming division. Within a bank folder, an object written as parts
keeps them in a folder of its own name ([Object](#object)), and a header
for one bank folder sits in `include/<folder>/`
([Source](src/README.md#banks-and-folders)).

Evidence: Camelot: the credits' four Programming divisions, "System",
"Battle", "Field" and "User Interface" (`gCredits_Lines`); `window`,
`menu` and `shop` from their own entries; plain English for the grouping.

### Far call

A call from one bank into another through the callee bank's far-call table,
an 8-byte jump named after the function with `Far` added (`Battle_RunFar`
leads to `Battle_Run`); only the System's bank 00 is called directly
([System](system.md#banks-and-far-calls)). The tables are in
`src/gamepak/far_call/`, one per bank, and their C names in
`include/gamepak/far_call/`.

Evidence: tradition: ジャンプテーブル for the table; "far call" is plain English.

### Veneer

A tiny piece of code that only jumps on: 8 bytes holding a load of an address,
a jump to it, and the address (the `overlay_veneer` macro). Every far-call
table entry and every stub of an overlay's tables is one.

Evidence: plain English; the project's own word.

### Overlay

A packed piece of code unpacked into one 32 KB area of EWRAM, `gOverlayArea`,
when it is needed: each holds the code of one scene or of a few neighbouring
scenes, 96 in all for 201 scenes, and only one is loaded at a time
([System](system.md#overlays)). Its packed streams and link script are in
`src/system/overlay/`, its code in `src/scene/` or `src/debug/`, and an
overlay's globals still written in assembly in `src/scene/bss/`.

Evidence: tradition: the GNU linker's word; the linker's `.bss` for the
globals' folder.

### Entry table and import table

The two tables an overlay meets the main image through: the entry table opens
it with six veneers the field calls to ask the scene its questions
(`struct SceneEntryPoints`), and the import table ends it with one veneer for
each main-image function it calls
([System code](src/system/README.md#how-an-overlay-reaches-the-rest-of-the-game)).
The scenes' tables are in `src/scene/entry/` and `src/scene/import/`, and the
debug menus' in `src/debug/entry/` and `src/debug/import/`.

Evidence: plain English.

### Resource directory

The one table of every packed thing on the cartridge, an overlay, a map or an
image, found by its row
([The Game Pak](src/gamepak/README.md#the-resource-directory-and-table-order)).

Evidence: plain English.

### File and part

A file is one of the three saves on the cartridge. Each is kept as two parts,
and each copy of a part sits in one 4 KB sector of the flash chip
([System code](src/system/README.md#saving)).

Evidence: Camelot: "Please choose a file." (きろくばしょを えらんでください).

### Background layer

One of the console's four tiled layers, `BG0` to `BG3`, each scrolling as a
whole; a town is drawn on three of them, the World Map on the two the hardware
can scale and rotate ([Field](field.md#map-layers)).

Evidence: tradition: the hardware's BG.

### Sprite and OAM

A sprite is a small image the hardware draws over or between the background
layers; OAM, the object attribute memory, is the hardware's list of them,
rebuilt every frame ([System code](src/system/README.md#the-frame-loop)).

Evidence: tradition: スプライト and the hardware's OAM.

### Sprite animation

One of the up to four layers of a sprite, each playing its own run of
[pictures](#picture) (`struct SpriteAnimation`): Isaac's body, his weapon
and his shadow are three sprite animations of one sprite
([Field](src/field/README.md#sprites-and-their-animation)).

Evidence: plain English; no word of the time names one layer of a sprite.

### Picture

One drawing in a sprite animation or a tile animation, shown for a number of
frames before the next ([Field](src/field/README.md#sprites-and-their-animation)).

Evidence: plain English; the time's パターン and コマ each mean other things too.

### Palette number

Which of the sixteen 16-colour palettes a tile or a sprite is drawn in, the
4-bit number the hardware reads beside each tile
([Graphics](src/graphics/README.md#how-an-image-is-built-and-placed)).

Evidence: plain English for the hardware's own field.

### Palette fade

The change of the backgrounds' and sprites' colours, by an equal amount each
frame, towards a target, so that they all arrive together; the field fades
each scene in from its backdrop colour with it, and the weather and Mind Read
tint the screen with it (`PaletteFade_Update`, in
[palette_fade.c](src/field/field_sequence/palette_fade.c)).

Evidence: plain English.

### Line scroll

A table that gives each of the screen's 160 lines its own background scroll,
written into the scroll registers line by line by HBlank DMA; Mind Read's wave,
the Lamakan Desert and the Lemurian Ship's ripple bend a layer with it
(`struct LineScrollWork`, `LineScroll_`, in
[line_scroll.c](src/field/event_commands/line_scroll.c)).

Evidence: tradition: ラインスクロール.

### Sound

Every sound the game plays, music, a jingle or a sound effect (SE), is asked
for by a sound number: below 80 music, from 80 the jingles, and from 100 the
sound effects ([Sound](src/sound/README.md#what-a-sound-number-means)). The
sound driver plays them; its code, with the mixer copied into IWRAM, is in
`src/sound/`, and the music and samples in `games/tbs/sound/`.

Evidence: Camelot: the credits' "Music" and "Sound Effects", and The Lost
Age's sound test's "Music", "Jingle" and "SE".

## The field

### Field

Everywhere the party walks, a town, a dungeon or the World Map, and the code
that runs it ([Field](field.md)). Its code is in `src/field/`
([Field code](src/field/README.md)).

Evidence: Camelot: the credits' "Field", and せんとう&フィールド
(`MsgUsableAnywhere`).

### Scene

One row of the scene table (`struct Scene` in `gScenes`): a room, a town, a
dungeon floor, the World Map, or a screen (the Title, the File Screen), 201
in all, each running an overlay's code and drawn on a map. Each
row carries Camelot's scene code (PLOGV00 for Vale on the stormy night of the
prologue, TITLE, WORLD; `MsgDebugSceneCode`) and its area name (`MsgDebugAreaName`), which the debug scene selector shows
([Field](field.md#scenes-and-areas)). Each scene's overlay is in `src/scene/`,
with its entry and import tables in `scene/entry/` and `scene/import/`.

Evidence: Camelot: The Lost Age's developer test message 05746 heads a location name シーン.

### Room

A scene inside a building, a house, a shop or an inn.

Evidence: Camelot: the scene labels' ROOM and RM (HAIROOM1, KUP_RM01,
FUCHINRM).

### Area

A group of scenes, 71 in all, sharing their music, their area name and one
range of [scratch flags](#game-flag)
([Field](field.md#scenes-and-areas)). The game names each by its area name,
the one the debug menu lists and the area name window shows.

Evidence: Camelot: the area names.

### Map

What a scene shows and stands on: three background layers and a grid of cells
under them, 128 by 128; several scenes can share one ([Maps](src/map/README.md)).
Each map is a folder of `src/map/` named by its row of `gMaps`
(`map_151/`), or for its place where one is known (`colosso/`).

Evidence: Camelot: the credits' "Maps".

### Map characters

The field's people and things as drawn, the sprite sheets the actors show
([Field](src/field/README.md#sprites-and-their-animation)).

Evidence: Camelot: the credits' "Map Characters".

### Still picture

The World Map's tilted view drawn once into an ordinary background layer,
freeing the tilt's layer for a menu's or an event's windows
(`WorldMap_StartStillPicture`) ([Field](field.md#the-world-map)). The map R
shows of the whole world is the world picture, a screen of its own
([Field code](src/field/README.md#the-world-picture)).

Evidence: plain English for what the player sees, the view standing still.

### Sheet of rooms

A map that packs [rooms](#room) drawn in one tile style side by side, each its
own scene with a camera box its entrance chooses; its rooms may belong
to different places, so it is named for the style they are drawn in, as
Cottage rooms or Stucco rooms ([Scenes](src/scene/README.md#design)).

Evidence: plain English; the project's own word.

### Cell

A 16-pixel square of a map: which cell picture is drawn there, the
shape and height of the ground, and its cell behavior
([Maps](src/map/README.md#cells)).

Evidence: plain English.

### Cell picture

The picture of one cell: four of the console's 8-pixel tiles, two by two.

Evidence: plain English.

### Cell behavior

The byte of a cell that says what standing on it, walking into it or searching
it does: a wall, a ledge, a jar. One range of behaviors means whatever the
scene's own trigger table says
([Field](field.md#cells-and-cell-behaviors)).

Evidence: plain English; Camelot's 属性 (attribute) means an element.

### Terrain

The heights of the ground within a cell, worked out from its shape by the
`Terrain_` family's curves (`field/terrain_tables.c`; The Lost Age's
`field/terrain/`, which still says `Curve_`)
([Field code](src/field/README.md#how-high-a-step-can-go)).

Evidence: plain English.

### Actor

One of the 64 slots of `gActors` (`struct Actor`): anything on the field that
moves or is drawn on its own, from the party and the townspeople to the
camera and the footprints in snow ([Field](field.md#actors-and-their-actor-scripts)).
A scene knows the party and the actors its actor placements put there by
their actor ids; the camera and the footprints have none
([Scenes](src/scene/README.md#who-stands-where)).

Evidence: tradition: the Western word for a thing on a game's stage.

### Actor id

An actor's number in its scene, the index of `FieldWork.actors`
(`ACTOR_ID_COUNT` of them): 0 to 7 the characters, each by its character
number (`ACTOR_GARET` is 1), and from 8 (`ACTOR_FIRST_PLACED`) the actor
placements' rows in order; each scene names its own in an enum.

Evidence: plain English.

### Actor placement

A row of a scene's actor table (`struct ActorPlacement`): the sprite, place,
facing, actor script and condition of one person or thing the scene puts on
the field when it loads ([Scenes](src/scene/README.md#who-stands-where)).

Evidence: plain English for what a row does, place an actor.

### Actor script

The list of `SC_` commands an actor runs each frame: waits, moves, jumps,
loops and tests of game flags. One command can hand the actor to a
built-in script, a routine built into the field that steers it every frame,
such as player control (`SC_PLAYER`) or the camera's
([Field](field.md#actors-and-their-actor-scripts)).

Evidence: tradition: スクリプト for a character's commands.

### Watched actor

The actor another actor's actor script faces, follows or approaches
(`Actor.watchedActor`, `SC_FACE_WATCHED`, `SC_APPROACH_WATCHED`)
([Field](field.md#actors-and-their-actor-scripts)).

Evidence: plain English for what the commands do.

### Field effect

A small decorative actor on the field, such as a ledge jump's dust puffs, the
World Map's footprints or an emote over someone's head, that moves at its
velocity and grows and spins at its rates, run by its own script
(`struct FieldEffect`)
([Field](field.md#actors-and-their-actor-scripts)).

Evidence: plain English.

### Trigger

A row of a scene's trigger table (`struct Trigger`): a kind, the subject it
answers, a condition and the event it runs. The subject is an actor id for
talking, a cell behavior for a cell, a treasure spot, or a value the
scene's code raises. When the player talks, searches, steps on a marked cell or
uses Psynergy, the first row that fits runs
([Scenes](src/scene/README.md#what-a-trigger-row-says)).

Evidence: tradition: the Western word for a spot on a map that starts something.

### Step trigger

A trigger that runs when the leader steps onto, or walks into, a cell with a
given cell behavior: a doorstep, a ladder, the edge of a path
([Scenes](src/scene/README.md#which-triggers-a-cell-starts)).

Evidence: plain English.

### Examine trigger

A trigger that runs when the player presses A facing a cell: a jar, a chest,
a bookcase ([Scenes](src/scene/README.md#which-triggers-a-cell-starts)).

Evidence: plain English.

### Exit

A way out of a scene, named for where it leads (`VALE_PLAZA_TO_WORLD_MAP`); the
first row for it in the scene's exit table whose condition holds gives the next
scene and entrance ([Field](field.md#exits-and-entrances)).

Evidence: Camelot: the game's text, 出口.

### Entrance

Where the party arrives in a scene, named for where it comes from
(`VALE_PLAZA_FROM_SANCTUM`): a position, a facing and the camera's limits
([Field](field.md#exits-and-entrances)).

Evidence: Camelot: the game's text, 入口.

### Field request

What the field sequence answers: as soon as one of twelve `FieldWork` members
is set (an exit, an encounter, a talk, an examine, a step trigger, a push, the
A button, an item, a field Psynergy, a menu or a raised trigger), it stops the
leader and serves them most urgent first ([Field](field.md#the-field-sequence-and-its-requests)).

Evidence: plain English; the project's own word.

### Event

The C routine a trigger runs, named for its scene (`Vale_EnterIsaacsHouse`):
a cutscene, or a conversation that does more than show one message. It runs
between `Event_Start` and `Event_End`, one event command to a line, each
command that takes time running its frames before the next starts. A trigger row that
only shows a message runs no event ([Scenes](src/scene/README.md#events)).

Evidence: Camelot: the Shining Force III credits' "Events", by two of Golden
Sun's field programmers.

### Event command

A routine an event calls: `Event_` for speech, waits and destinations,
`Actor_` for actors and `FieldCamera_` for the camera. An event calls most of
them through one inline each in `event_inline.h`, beside `event.h`
([Scenes](src/scene/README.md#events)). They are the parts of
`src/field/event_commands.c`, under `field/event_commands/`.

Evidence: plain English.

### Destination

The scene and entrance the main routine runs next (`gMainRoutineWork.sceneId`
and `entrance`): an exit sets it, and an event can set it itself
(`Event_SetDestination`) or set it and leave at once (`Event_LeaveFor`).
Four others are read only at given moments: after a battle or minigame the
after-battle destination when one is set, else the return point, the scene
the party left; after a defeat the defeat destination when one is set, else
the restart point, usually the last Sanctum ([Field](field.md#exits-and-entrances)).

Evidence: plain English.

### Game flag

One of the 4,096 yes-or-no facts saved with the game, such as a chest being
open; two ranges are scratch space, cleared as the party enters scenes and
areas ([Game rules](src/game/README.md#the-game-flags)).

Evidence: Camelot: 田口泰宏, the System programmer, speaks of flags (Shining
Force III, 1998).

### Debug menus

What the released game keeps for its makers and never shows: the debug scene
selector (`DebugSceneSelect_Run`), which names each scene by its scene code
and area name, and the test scenes it reaches (`SceneId_Test0` to
`SceneId_Test7`); in debug mode, the Pause menu's Rough menu, Recovery test
and Restart quest; the parameter sets the Battle Arena's debug helper loads
(`gDebug_ParameterSets`); and the sound test (`SoundTest_Run`)
([Debug scenes](src/debug/README.md#how-the-debug-scenes-are-reached)). The
debug menus' three overlays are in `src/debug/`.

Evidence: Camelot: the menus' labels ("Rough menu", "Recovery test",
パラメータセット), the scene codes TEST0 to TEST7 and The Lost Age's サウンドテスト.

## The battle

### Battle

The fight between the party and a formation of enemies, a sequence of its own
([Battle](battle.md)). Its code is in `src/battle/`, and the curtain pictures
of its opening transition in `battle/transition/`. That transition is how a
battle opens, with the curtain or the Venus Lighthouse storm
(`BattleTransition_Run`), not the handover between sequences
([Transition and next sequence](#transition-and-next-sequence)).

Evidence: Camelot: the credits' "Battle"; `battle/transition/` takes the name of
the code that draws it.

### Unit

Either side's fighter in a battle (`struct Unit`); a party member is a unit
in the party ([Battle](battle.md#units-and-abilities)).

Evidence: plain English; the game's キャラ means only a party member.

### Formation

One group of enemies a battle is fought against, a row of
`src/battle/formations.c`, 380 in all
([Battle](battle.md#starting-a-battle)).

Evidence: plain English.

### Turns and actions

A turn is every unit in a battle acting once; an action is one unit's go
within it, chosen as a `struct BattleAction` and carried out as a
`struct BattleActionRun` ([Battles](src/battle/README.md#turns)).

Evidence: Camelot: "HP recovery / turn" (かいふくHP/ターン) and 行動 for one
unit's go.

### Staging

How an action is shown, apart from what it does: who comes forward, what is
announced and which battle effect plays (`gAbilityStaging`)
([Battles](src/battle/README.md#staging-and-announcement)).

Evidence: tradition: 演出.

### Playback queue

The list of what each target's rules left to show, the flinch, the message,
the falling HP, which a task plays once the battle effect says the blow has
landed ([Battles](src/battle/README.md#how-outcomes-reach-the-screen)).

Evidence: plain English.

### Battle effect

What an ability plays over a battle, drawn by code: Quake's shaking
ground, a summon's arrival. It decides when a hit shows, never what it does
([Battle effects](src/battle_effect/README.md)). Its code is in
`src/battle_effect/`, and its sheets in `src/graphics/battle_effects/`.

Evidence: Camelot: the credits' "Battle Effects", beside "Battle Characters"
and "Battle Backgrounds", a Graphic Arts heading; the code is the Battle
division's.

### Battle background

The picture behind a battle, chosen by the place it is fought in: the
grassland, Goma Cave, a ship's deck (`src/graphics/battle_backgrounds/`).

Evidence: Camelot: the credits' "Battle Backgrounds".

## The user interface

### User Interface

The windows, the messages typed into them, the menus and the counters of the
shops, sanctums and inns ([User Interface](user_interface.md)). Its code is in
`src/window/`, `src/menu/` and `src/shop/`, and its pictures in
`src/graphics/ui/`.

Evidence: Camelot: the credits' "User Interface".

### Window

A bordered box the game opens for text or choices, growing out of its centre
(`struct Window`) ([Windows](src/window/README.md#windows)). The window bank's
code is in `src/window/`.

Evidence: Camelot: ウインドウ, as in "Window color" (`MsgWindowColorLabel`).

### Border

The edge drawn around a window (`Window_DrawBorder`)
([User Interface](user_interface.md#windows)).

Evidence: plain English.

### Message

The text the game types into a window a few letters at a time, to the
speaker's voice blip, waiting for a button at each page break. Up to three are
typed at once (`struct Message`, `Message_Type`)
([Windows](src/window/README.md#typing)).

Evidence: Camelot: the settings label メッセージスピード ("Message" in English,
`MsgMessageSpeedLabel`).

### The screens

The Title; the File Screen, where a file is chosen, copied or erased; the
Pause menu, which Start opens on the field; the field menu with its
Psynergy, Djinn, Item and Status screens; the settings menu; name entry; and
the world picture ([User Interface](user_interface.md)). The Pause menu, the
file list saving and the File Screen share, name entry, the command icon menus
and the battle's command menus are in `src/window/screens/`; the field menu's
screens are in `src/menu/`.

Evidence: Camelot: "To the File Screen", "The Pause menu is not..."
(`MsgFieldPauseUnavailable`), the scene label TITLE; the rest plain English.

### Menu

The field menu and its Psynergy, Djinn, Item and Status screens, kept in
their own bank (`src/menu/`), with the staff credits and the Title's intro,
which the Title calls ([Menus](src/menu/README.md)).

Evidence: Camelot: "Rough menu" (`MsgWorkspaceRoughMenu`), "Menu test" and
the debug row MENU.

### Shop

The counters of the shops, sanctums and inns, one bank (`src/shop/`)
([Shops](src/shop/README.md)).

Evidence: the game: "weapon shop", "Thank you for shopping".

## The folders

Each folder of `src/` is a [bank folder](#bank-folder), a place where one
kind of thing gathers, or one of the folders below.

### Game rules

The game bank's code and tables: what a unit, an item or a Djinni is and does,
which the field, the battles and the menus all ask (`src/game/`,
[Game rules](src/game/README.md)); no programming division is credited with
it.

Evidence: Camelot: the credits' "Game Design" and "Game Planning"; plain
English "game rules", said so.

### Graphics

The pictures in editable form, each folder named for the credits' heading, the
game's word or the place it belongs to, with the `.s` files that only place
them; a picture only a mixed data file reads sits beside that file
(`src/graphics/`, [Graphics](src/graphics/README.md)). The display headers
several divisions read are in `include/graphics/`.

Evidence: Camelot: the credits' "Graphic Arts"; plain English.

### Graphics folders

- `battle_backgrounds`: the [battle backgrounds](#battle-background).
  Evidence: Camelot: the credits' "Battle Backgrounds".
- `battle_effects`: the [battle effects](#battle-effect)' sheets and
  palettes. Evidence: Camelot: the credits' "Battle Effects".
- `field_effects`: the sparks field Psynergy draws. Evidence: the
  [field effect](#field-effect)'s word.
- `font`: the text font, its widths and character maps. Evidence: plain
  English.
- `monster`: the enemies' battle pictures. Evidence: Camelot: the credits'
  "Monster Design"; the game: "Monsters won't attack".
- `palettes`: the window palette and the basic sprite palettes. Evidence:
  plain English.
- `palette_editor`: the debug palette editor's swatch, which the debug menus
  reach. Evidence: plain English.
- `sprite`: every sprite sheet and its animations. Evidence: the hardware's
  word ([Sprite and OAM](#sprite-and-oam)); Camelot: the credits' "Map
  Characters" and "Battle Characters" for the work.
- `tileset`, with `set_NN/` and `palettes/`: the tiles and palettes a
  map's [cell pictures](#cell-picture) are drawn from. Evidence: tradition:
  the tile set; Camelot: the credits' "Maps" for the work.
- `ui`, with `icons/`, `marks/`, `menu_icons/`, `menu_titles/`,
  `menu_cursor/`, `menu_sprites/` and `sprite_cursors/`: every
  [User Interface](#user-interface) picture. Evidence: Camelot: the credits'
  "User Interface", whose banks place them; plain English for the subfolders.
- The Lost Age's `ending/`, `lettering/`, `summon/`, `field/` and
  `ui/headings/`. Evidence: plain English for what they hold, kept until
  The Lost Age's own pass reads them.

### Places and minigames

The folders named for a place or game thing, each with its evidence in H3's
form:

- `colosso`, the Colosso's lettering, counters and gauge: ★ Named from the
  game: "Colosso".
- `lamakan_desert`, the desert's heat gauge: ★ Named from its area name:
  Lamakan Desert.
- `sol_sanctum_statue`, the statue room's scrolling rows: ★ Named from its
  area name: Sol Sanctum.
- `lucky_dice` and `lucky_wheels`, the two minigames' pictures and tables:
  ★ Named from the game: "Lucky Dice", "Lucky Wheels".
- `world_map`, the World Map's tiles and the lighthouse transition: ★ Named
  from the game: "World Map".
- `title`, the Title's pictures and its prompt: Camelot: the scene label
  TITLE.
- `credits`, the staff roll's font: Camelot: the credits themselves ("GOLDEN
  SUN STAFF CREDITS").

### Tileset

The tiles and palette a map's cell pictures are drawn from, one sheet per
layer (`src/graphics/tileset/`).

Evidence: tradition: the tile set; plain English.

### Monster

An enemy as drawn, its battle picture (`src/graphics/monster/`); in the code
an enemy is a [unit](#unit).

Evidence: Camelot: the credits' "Monster Design"; the game: "Monsters won't
attack".

### The Lost Age's folders

The Lost Age takes the same top folders, and inside them keeps family folders
of its own until its own pass:

- `field/camera`, `event`, `map`, `script` and `field_effect`: the same as
  the [Field](#field)'s camera, [events](#event), [maps](#map),
  [actor scripts](#actor-script) and [field effects](#field-effect).
- `field/object`: The Lost Age's word for an [actor](#actor) until its pass.
- `field/field_sequence`, `event_commands`, `lucky_dice`, `lucky_wheels`
  and `terrain`: the objects of the banks that in The Broken Seal are
  `field_sequence.c`, `event_commands.c`, `lucky_dice.c`,
  `lucky_wheels.c` and the [terrain](#terrain) tables.
- `battle/motion`, `party` and `summon`: plain English for the battle's
  motion, its party and the [summons](#summons).
- `menu/ability`, `item`, `credits` and `title_intro`: the field menu's
  screens, and the banks of the staff credits and the Title's intro.
- `game/character`, `djinn`, `inventory`, `item` and `party`: the
  same as the [Game rules](#game-rules)' characters, [Djinn](#djinn),
  inventory, items and [party](#the-party).
- `system/flags`, `save`, `serial`, `task`, `resource` and
  `overlay`: the [game flags](#game-flag), saving, the link cable, the
  [tasks](#task), the [resource directory](#resource-directory)'s loader and
  the [overlays](#overlay).
- `sound/driver`: the m4a driver's assembly.
- `scene/clear`, the scene after the Title with its password grid, and
  `scene/lobby`, where a link battle is arranged: named from
  `gScene_AreaNameRules` at that pass. `debug/level`, `psynergy_test`,
  `psynergy_test_2`, `rooms` and `shop_test`: the
  [debug menus](#debug-menus)' overlays.

Evidence: the same as the entries they share, or plain English, said so.

### Placeholders

`field/work9c/` and `field/worka0/` are named for heap slot offsets `0x9c`
and `0xa0` (their headers: "Heap-slot 0x9c workspace: eight slots and 128
nodes"), and `battle/presentation/` for a guess. Each is a placeholder until
The Lost Age's own pass, which names it from its slot's readers
(`HEAP_SLOT_` and its family) and, for `presentation/`, from its 47 files'
families.

Evidence: placeholder: no evidence yet, said so.
