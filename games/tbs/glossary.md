# Glossary

Every page links here the first time it uses one of these words. They come in
the order a newcomer meets them: the game's own words, then the project's,
the system's and the scenes'.

## Golden Sun

### The two games

Golden Sun is a role-playing game for the Game Boy Advance in two parts: The
Broken Seal (2001), whose builds are named `tbs`, and its sequel The Lost Age
(2002), named `tla`, each with its own copy of the source under `games/`.

### Camelot

Camelot Software Planning made both games, and Nintendo published them; their
source was never released.

### The party

The characters travelling together, led by Isaac: in The Broken Seal, Isaac,
Garet, Ivan and Mia. A character's or an enemy's record is a unit
(`struct Unit`), and a unit in the party a party member
([Game rules](src/game/README.md#words-used-here)).

### The places, in story order

The story starts in Vale, under Mt. Aleph, and follows the thieves of the
Elemental Stars through Vault, Bilibin, Kolima, Imil, Mercury Lighthouse,
Xian, Altin, Kalay and Tolbi to Lalivero and Venus Lighthouse, where the game
ends. The story is the order the [game flags](#game-flag) are set in, so pages
list places in that order; [Places](src/field/scenes.s) lists every scene.

### Psynergy and PP

![Isaac casts Growth in battle: light gathers around him, then a spiked vine bursts out of the ground under Ivan](https://alchemy.pascalpixel.com/abilities/growth.gif)

Psynergy is the game's magic, cast in battle (Quake, Cure, Growth) and on the
field (Move, Reveal), where it pushes, grows or uncovers things; it is
uncountable, so a character uses Psynergy, never "a Psynergy". Each use costs
PP (Psynergy Points), which come back as the party walks outside towns and at
inns ([Field](field.md#field-psynergy)).

### The four elements

Venus (earth), Mercury (water), Mars (fire) and Jupiter (wind), in that order
in every table the code keeps per element (`enum Element`). Every character,
Djinni and summon belongs to one: Isaac is Venus, Garet Mars, Ivan Jupiter and
Mia Mercury.

### Power and Resist

Each fighter has a Power and a Resist in each element: Power raises the damage
and healing of what it casts in that element, and Resist lowers the damage it
takes from it ([Battles](src/battle/README.md#the-element-multiplier)).

### Djinn

![Ivan unleashes the Jupiter Djinni Gust in battle: pink wind streams across the field, then a blue blast strikes its target](https://alchemy.pascalpixel.com/abilities/gust.gif)

Elemental creatures the party collects; one is a Djinni, several are Djinn.
Set on a character, a Djinni raises its stats and helps decide its class;
unleashed in battle, it does its own move and goes on standby, ready for a
summon ([Game rules](src/game/README.md)).

### Summons

![Isaac summons Ramses: the screen wipes to a desert where a giant stone figure appears and pounds its targets with its fist](https://alchemy.pascalpixel.com/abilities/ramses.gif)

A summon spends one to four Djinn on standby to call up an elemental being
(Venus, Ramses, Judgment and the others); the Djinn then recover for a few
turns ([Battles](src/battle/README.md#commands-and-ability-ids)).

### Rounds and turns

A round is every unit in a battle acting once; a turn is one unit's action
within it (`struct BattleTurn`). What pret calls a battle turn is this game's
round ([Battles](src/battle/README.md#rounds)).

### Battle animation

The picture an ability plays over a battle, drawn by code: Quake's shaking
ground, a summon's arrival. It decides when a hit shows, never what it does
([Battle animations](src/battle_anim/README.md)).

## The project

### Matching decompilation

Alchemy is C and assembly written by hand that compiles back into exactly the
bytes on the cartridges, checked against `rom.sha1`, so the source sometimes
keeps an unusual form where plainer C would behave the same
([Introduction](../README.md), [Matching](src/MATCHING.md)).

### The original

The game as [Camelot](#camelot) shipped it: each edition's cartridge image,
which the build reproduces byte for byte, bugs included.

### Editions

Each game was released in Japanese, English, German, Spanish, French and
Italian, each an edition with its own build (`tbs-ja` to `tbs-it`, `tla-ja` to
`tla-it`). The source is written from the Japanese edition; the other five,
the international editions, are its measured differences, chosen with
`LANGUAGE` from `edition.h` and grouped in patch levels `TBS_PATCH_1` (all
five) to `TBS_PATCH_4` (Spanish and Italian), which are the project's groups,
not Camelot's version numbers.

### pret

The group behind the Pokémon decompilations, whose conventions Alchemy follows
where they settle a question: the shape of names, the `BUG` and `BUGFIX`
markers and the sound driver's routines. Alchemy's words for things are the
game's own, not pret's; the entries below say where pret uses another word.

### The compilers

The game's Thumb C builds with alchemy-gcc, a GCC of July 2000 in the
`alchemy-gcc` submodule, its few ARM files with GCC 2.95.2, and its sound and
save code with pret's `old_agbcc`; `make modern` also compiles the C with a
current GCC, only to check it ([Matching](src/MATCHING.md)).

### FAKEMATCH

A tag on code kept in an unusual form only because the plain C compiles to
different instructions; it names the plain form and how many instructions it
gets wrong ([Matching](src/MATCHING.md#fakematch)).

### BUG, BUGFIX and UBFIX

`// BUG:` marks a bug of the original game, kept as it is, with a fix behind
`BUGFIX`; `// UB:` marks undefined behaviour kept in the form that matches,
with the defined form behind `UBFIX`
([Matching](src/MATCHING.md#the-other-markers)).

### Unused

`UNUSED` marks a function nothing calls, and a comment opening with `Unused:`
something the game can never reach, with the reason; what only the debug menus
reach is not unused. [Unused](https://alchemy.pascalpixel.com/tbs/unused)
collects them.

## The system

### System

The code the field, the battles and the user interface are built on: start-up,
waiting and [tasks](#task), interrupts, the [heap](#heap-and-pools),
[banks](#bank), [overlays](#overlay), saving and the sound driver.
It is one of the code's divisions, with Battle, Field and User Interface;
[System](system.md) explains it.

Evidence: Camelot's credits, "Programming", then "System", "Battle", "Field",
"User Interface" and "Support" (`gCredits_Lines`).

### Frame

One step of the game: every queued [task](#task) runs once and one picture is
prepared, then the game halts until the next [VBlank](#vblank) shows it. Code
lets time pass with `WaitFrames(n)`, and every wait, walk and animation is
counted in frames. A frame normally lasts one VBlank; one whose work overruns
lasts two or more, which is the game's slowdown
([The front page](README.md#when-a-frame-runs-long)).

### VBlank

One refresh of the screen. The console draws about 59.73 pictures a second,
and after each it pauses in the vertical blank (VBlank), whose interrupt mixes
the sound and reads the buttons whatever the game is doing. The music keeps
time in VBlanks
([System](system.md)).

### IWRAM and EWRAM

The console's two work memories: IWRAM, 32 KB inside the processor and the
fastest it has, and EWRAM, 256 KB beside it and slower
([Cartridge and memory](src/cartridge/README.md#memory-map)).

### Background layer

One of the console's four tiled pictures, `BG0` to `BG3`, each scrolling as a
whole; a town is drawn on three of them, the world map on the two the hardware
can scale and rotate ([Field](field.md#map-layers)).

### Sprite and OAM

A sprite is a small picture the hardware draws over or between the background
layers; OAM, the object attribute memory, is the hardware's list of them,
rebuilt every frame ([Runtime](src/runtime/README.md#the-frame-loop)).

### Main image

The code the processor runs straight from the cartridge, about 1 MB in
[banks](#bank); the rest runs from RAM, a scene's [overlay](#overlay) in EWRAM
and the fast routines in IWRAM.

### Bank

One of the fourteen stretches the main image is cut into, each at a fixed
address with room to spare and opening with a [far-call](#far-call) table, so
one bank can grow without moving anything another depends on
([Cartridge and memory](src/cartridge/README.md#why-the-main-image-is-cut-into-banks)).

### Far call

A call from one bank into another, through an 8-byte jump in the callee's
table named after the function with `Far` added: `Battle_RunFar` leads to
`Battle_Run` ([System](system.md#banks-and-far-calls)).

### Veneer

A tiny piece of code that only jumps on: 8 bytes holding a load of an address,
a jump to it, and the address (the `overlay_veneer` macro). Every far-call
table entry and every stub of an overlay's tables is one.

### Overlay

A packed piece of code unpacked into one 32 KB area of EWRAM, `gOverlayArea`,
when it is needed: each holds the code of one scene or of a few neighbouring
scenes, 96 in all for 201 scenes, and only one is loaded at a time ([System](system.md#overlays)).

### Entry table and import table

The two tables an overlay meets the main image through: the entry table opens
it with six veneers the field calls to ask the scene its questions
(`struct SceneEntryPoints`), and the import table ends it with one veneer for each
main-image function it calls
([Cartridge and memory](src/cartridge/README.md#how-an-overlay-reaches-the-rest-of-the-game)).

### Heap and pools

The heap lends blocks of memory from two pools, one in IWRAM and one in EWRAM;
blocks go back in the reverse of the order they were taken, and the whole heap
is emptied before each scene ([Runtime](src/runtime/README.md#the-heap)).

### Heap slot

One of 62 numbered places that each hold a heap block other code has to find
again, such as the running scene's state (`gHeapSlots`).

### Task

A short job run once every [frame](#frame), or once every [VBlank](#vblank),
until it is removed: the windows' typing, the map's scrolling, the lightning
over Vale on the stormy night. Each runs in one of two passes,
`TASK_PASS_FRAME`, which `WaitFrames` runs once a frame, or `TASK_PASS_VBLANK`,
which the VBlank handler runs once a VBlank; up to 20 at a time run, in the
order of their priority
([Runtime](src/runtime/README.md#the-task-runner)).

### Song

Every sound the game plays is asked for by a song number: below 80 music,
from 80 the fanfares, and from 100 the sound effects
([Sound](src/sound/README.md#what-a-song-number-means)).

## Scenes and maps

The words for places are the game's own where it has one (its area names, its
menus, its debug names), else a word of the game's time, else a plain
description of the thing. A reader coming from pret's decompilations, such as
pokeemerald, finds each entry saying what pret calls the nearest thing.

### Sequence

One flow of the game, run from its start to its end: the scene sequence, the
battle sequence, the dice sequence (Lucky Dice) or the slots sequence (Lucky
Wheels), `SEQUENCE_` in code. One runs at a time, until it ends and names the
next sequence in the [adventure](#adventure); that handover is the transition.
pret's DS decompilations call the nearest thing an application
([The front page](README.md#the-parent-sequence)).

Evidence: Sega's word for one flow of a game, and the handover's (遷移); Camelot
uses プログラム only for the whole game's code.

### Parent sequence

The loop that runs one [sequence](#sequence) after another for as long as the
console is on, `ParentSequence_Run`: in each transition it tears down what the
last sequence set up (its tasks, its interrupt handlers, its heap blocks) and
starts the next ([The front page](README.md#the-parent-sequence)).

Evidence: Sega's word for what runs its child sequences in turn.

### Adventure

The one struct every sequence shares and that outlives each of them,
`gAdventure` (`struct Adventure`, `adventure.h`): the party, where it stands,
the coins and play time, the options, the next sequence and what the last one
reported. Save Game writes it to the cartridge with the game flags
([The front page](README.md#how-a-sequence-finds-its-way-back)).

Evidence: the game's own word for what saving keeps, "Save your adventure?"
(`MsgSaveAdventureQuestion`; ぼうけんを きろくしますか? in Japanese).

### Scene

One row of the scene table (`struct Scene` in `gScenes`): a room, a town, a
dungeon floor, the World Map, or a screen such as the Title or the File
Screen, 201 in all, each running an overlay's code and drawn on a map
([The front page](README.md#scenes-and-overlays)).

### Area

A group of scenes, 71 in all, sharing their music, their area name and one
range of [scratch flags](#game-flag)
([The front page](README.md#the-scene-table-and-areas)). The game names each
by its area name, the one the debug menu lists and the area name window shows;
pret calls the nearest thing a map section.

### Map

What a scene shows and stands on: three background layers and a grid of cells
under them, 128 by 128; several scenes can share one ([Maps](src/map/README.md)).

### Sheet of rooms

A map that packs indoor rooms drawn in one tile style side by side, each its
own scene with a camera box its entrance chooses; its rooms may belong
to different places, so it is named for the style they are drawn in, as
Cottage rooms or Stucco rooms ([Scenes](src/scene/README.md#design)).

### Cell

One 16 by 16 pixel square of a map: which metatile is drawn there, the
shape and height of the ground, and its cell behavior
([Maps](src/map/README.md#cells)).

### Metatile

The picture of one cell: 2 by 2 of the console's 8-pixel tiles.

### Cell behavior

The byte of a cell that says what standing on it, walking into it or searching
it does: a wall, a ledge, a jar. One range of behaviors means whatever the
scene's own trigger table says
([Field](field.md#cells-and-cell-behaviors)).

### Actor

One of the 64 slots of `gActors` (`struct Actor`): anything on the field that
moves or is drawn on its own, from the party and the townspeople to the
camera and the footprints in snow ([Field](field.md#actors-and-their-actor-scripts)).
A scene knows the party and the actors its actor templates place by their
actor ids; the camera and the footprints have none
([Scenes](src/scene/README.md#who-stands-where)). pret calls the nearest
thing an object event, and a template an object event template.

### Actor id

An actor's number in its scene, the index of `FieldWork.actors`
(`ACTOR_ID_COUNT` of them): 0 to 7 the party, the leader at 0, and from 8
(`ACTOR_FIRST_PLACED`) the actor templates' rows in order; each scene names
its own in an enum. pret calls it a local id.

### Actor script

The list of `SC_` commands an actor runs each frame: waits, moves, jumps,
loops and tests of game flags. One command can hand the actor to a
controller, a routine built into the field that steers it every frame, such
as player control (`SC_PLAYER`) or the camera's
([Field](field.md#actors-and-their-actor-scripts)).

### Trigger

A row of a scene's trigger table (`struct Trigger`): a kind, the subject it
answers, a condition and the event it runs. The subject is an actor id for
talking, a cell behavior for a cell, a treasure spot, or a value the
scene's code raises. When the player talks, searches, steps on a marked cell or
uses Psynergy, the first row that fits runs
([Scenes](src/scene/README.md#what-a-trigger-row-says)). pret calls a row a
map event.

### Step trigger

A trigger that runs when the leader steps onto, or walks into, a cell with a
given cell behavior: a doorstep, a ladder, the edge of a path. pret calls
it a coord event.

### Examine trigger

A trigger that runs when the player presses A facing a cell: a jar, a chest,
a bookcase. pret calls it a bg event.

### Exit

A way out of a scene, named for where it leads (`VALE_PLAZA_TO_WORLD_MAP`); the
first row for it in the scene's exit table whose condition holds gives the next
scene and entrance
([The front page](README.md#exits-and-entrances)). pret calls it a warp.

### Entrance

Where the party arrives in a scene, named for where it comes from
(`VALE_PLAZA_FROM_SANCTUM`): a position, a facing and the camera's limits.
pret calls it a warp destination.

### Field request

What the scene sequence answers once the leader stops walking: one of twelve
`FieldWork` members asks for an exit, an encounter, a talk, an examine, an
item, a field Psynergy, a menu or a raised trigger, and the scene sequence
serves them most urgent first ([Field](field.md#the-scene-sequence-and-its-requests)).

### Event

The C routine a trigger runs, named for its scene (`Vale_EnterIsaacsHouse`):
a cutscene, or a conversation that does more than show one message. It runs
between `Event_Start` and `Event_End`, one step to a line, each step that
takes time running its frames before the next starts. A trigger row that
only shows a message runs no event ([Scenes](src/scene/README.md#events)).

### Event command

A routine an event calls: `Event_` for speech, waits and destinations,
`Actor_` for actors and `FieldCamera_` for the camera. An event calls most of
them through one inline each in `event_inline.h`
([Scenes](src/scene/README.md#events)).

### Destination

The scene and entrance the parent sequence runs next (`gAdventure.sceneId`
and `entrance`): an exit sets it, and an event can set it itself
(`Event_SetDestination`) or set it and leave at once (`Event_LeaveFor`).
Three others are read only at given moments: after a battle the party goes to
the after-battle destination when it won and one is set, else back to the
return point, the scene it left; after a defeat it goes to the defeat
destination when one is set, else to the restart point, usually the last
Sanctum ([The front page](README.md#exits-and-entrances)).

### Game flag

One of the 4,096 yes-or-no facts saved with the game, such as a chest being
open; two ranges are scratch space, cleared as the party enters scenes and
areas ([The front page](README.md#game-flags)).
