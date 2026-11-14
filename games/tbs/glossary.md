# Glossary

These are the words the pages about The Broken Seal share. Each entry says
what the word means and links to the page that explains it.

An entry's `Evidence:` line says where its word comes from. That is Camelot's
own word where Camelot has one for the thing, else the word programmers of
the time used, else plain English.

## Golden Sun

### The two games

Golden Sun is a role-playing game for the Game Boy Advance, told in two
parts. The first is The Broken Seal (2001), which these pages are about and
whose builds are named `tbs`. Its sequel is The Lost Age (2002), named `tla`.

Evidence: Camelot: the games' titles.

### Camelot

Camelot Software Planning made both games, and Nintendo published them. The
game opens on Nintendo's logo and then Camelot's. Their source was never
released.

Evidence: Camelot: the credits.

### The party

The party is Isaac and whoever is travelling with him. In The Broken Seal
that is Isaac, Garet, Ivan and Mia, and they share one purse of coins. In a
battle each of them is a [unit](#unit)
([Game rules](src/game/README.md#the-party-and-its-coins)).

Evidence: Camelot: the game's text says party throughout.

### The places, in story order

The story starts in Vale, under Mt. Aleph, and follows the thieves of the
Elemental Stars through Vault, Bilibin, Kolima, Imil, Mercury Lighthouse,
Xian, Altin, Kalay and Tolbi to Lalivero and Venus Lighthouse, where the game
ends. Pages list places in that order, and
[Places](https://alchemy.pascalpixel.com/tbs/places) shows every one.

Evidence: Camelot: the area names in the game's text.

### Psynergy and PP

![Isaac casts Growth in battle: light gathers around him, then a spiked vine bursts out of the ground under Ivan](https://alchemy.pascalpixel.com/abilities/growth.gif)

Psynergy is the game's magic. Some is cast in battle, such as Quake, Cure
and Growth, and some on the field, such as Move and Reveal
([Field code](src/field/README.md#how-field-psynergy-reaches-its-target)).

Each use costs PP, Psynergy Points: Quake costs 4, Cure 3 and Move 2. PP
comes back as the party walks anywhere but in a town, and a night at an inn
restores it.

Evidence: Camelot: the game's text ("Your Psynergy Points.").

### The four elements

The four elements are Venus (earth), Mercury (water), Mars (fire) and Jupiter
(wind), and the game always lists them in that order. Every character,
Djinni and summon belongs to one. Isaac is Venus, Garet Mars, Ivan Jupiter
and Mia Mercury ([Game rules](src/game/README.md#element-levels)).

Evidence: Camelot: the game's text.

### Power and Resist

For each of the [four elements](#the-four-elements), every fighter has a
Power and a Resist, which the Status screen shows. Power raises the damage
and healing of what the fighter casts in that element. Resist lowers the
damage it takes from it. A damage line that ends in three exclamation marks
has struck the element its target resists worst
([Battles](src/battle/README.md#the-element-multiplier)).

Evidence: Camelot: the Status screen's labels.

### Djinn

![Ivan unleashes the Jupiter Djinni Gust in battle: pink wind streams across the field, then a blue blast strikes its target](https://alchemy.pascalpixel.com/abilities/gust.gif)

Djinn are elemental creatures the party collects. One is a Djinni, several
are Djinn. Set on a character, a Djinni raises that character's stats and
helps decide its class: Flint, a Venus Djinni, gives 8 HP, 4 PP and 3
Attack. Unleashed in battle, it does its own move and goes on standby, ready
for a [summon](#summons) ([Game rules](src/game/README.md#djinn-and-classes)).

Evidence: Camelot: the game's text.

### Summons

![Isaac summons Ramses: the screen wipes to a desert where a giant stone figure appears and pounds its targets with its fist](https://alchemy.pascalpixel.com/abilities/ramses.gif)

A summon spends [Djinn](#djinn) on standby to call up an elemental being.
The summon named Venus takes one Venus Djinni, Ramses two, Cybele three and
Judgment four. The Djinn it spent then need a few
[turns](#turns-and-actions) to recover
([Battles](src/battle/README.md#commands-and-ability-ids)).

Evidence: Camelot: the game's text.

## The project

### Matching decompilation

A matching decompilation is a game's code written out again so that people
can read it, and written so exactly that it turns back into the cartridge's
contents, byte for byte. A program called a compiler does the turning, and
that step is called building. Nothing is tidied on the way, so the game's
bugs are all still there ([BUG, BUGFIX and UBFIX](#bug-bugfix-and-ubfix)
shows one). Sometimes a line is written in an unusual way where a plainer
one would do the same, because only that way builds the same bytes
([Welcome](../README.md)).

Evidence: plain English; the project's own words.

### The original

The original is the game as [Camelot](#camelot) shipped it: the cartridge of
each [edition](#editions). The source reproduces each one byte for byte,
bugs included.

Evidence: plain English; the project's own word.

### Editions

Each game was released in Japanese, English, German, Spanish, French and
Italian, and each release is an edition (`tbs-ja` to `tbs-it`). The five
other than the Japanese are the international editions. They differ in more
than their text: the Japanese edition can have twelve
[windows](#window) open at once, the others eight.

Evidence: plain English; the project's own word.

### pret

For readers of the code: pret is the group of fans behind the
decompilations of other Game Boy Advance games. This source borrows its
habits so that anyone who has read one of pret's games can read this one. A
named piece of code is spelled like `Battle_Run`, a value the whole game
shares starts with `g`, like `gTasks`, and bugs are marked `BUG` and
`BUGFIX`.

Evidence: the group's own name.

### BUG, BUGFIX and UBFIX

`// BUG:` marks one of the original game's bugs, kept as it is. Cast Sleep
and twelve sheep drop onto the targets. They were meant to bounce off the
ground, but a slip in the check means they almost always fall on through.
The source keeps the slip, with the working check beside it behind `BUGFIX`.

`// UB:` is for readers of the code. The game is written in a language
called C, and C leaves a few things open. In several battle effects one
line both changes a number and reads it, and C does not say which happens
first. On the cartridge it came out one particular way, and the source
keeps that. The comment marks the spot, and a version that leaves nothing
open sits beside it behind `UBFIX`.

Evidence: plain English, in pret's markers.

### Unused

Some of what is on the cartridge never happens in the game. A comment
opening with `Unused:` marks such a thing and gives the reason. One is a
second version of Garet joining Isaac on the stormy night: every path to
the spot that would start it crosses the spot that starts the version the
player sees. `UNUSED` in the code marks the plainest case, a named piece of
code that nothing else in the game ever asks for.

What only the [debug menus](#debug-menus) reach is not unused.
[Unused](https://alchemy.pascalpixel.com/tbs/unused) collects them.

Evidence: plain English.

### Module header

For readers of the code: the source is many files, and a file can only use
what another file has announced. A module header is the short file where
one file of code makes its announcements. The main routine's header is
where the rest of the game learns that `MainRoutine_Run` exists. A header
sits in `include/` under its code's folder, named for its code:
`src/system/main_routine.c` has `include/system/main_routine.h`
([Source](src/README.md#headers)).

Evidence: plain English; the project's own word.

### Object

For readers of the code: the game is not built in one go. Each file of the
source is turned into bytes separately, and the result is called an object.
The objects are then laid end to end to make the cartridge. The word has
nothing to do with the people and things in a scene, which are
[actors](#actor).

A very long file is written as several smaller parts beside it, each named
for it, and still makes one object. The shops are one: `src/shop/shop.c`
gathers the inn, buying and selling from `shop_inn.c`, `shop_buy.c` and
`shop_sell.c`
([Source](src/README.md#objects-and-parts)).

Evidence: plain English; the GNU tools' "object file".

## The system

### System

The System is the code everything else stands on: starting up, deciding what
runs next, keeping time, handing out memory, saving and sound. The field,
the battles and the menus all use it and none of them could run without it.
It is one of the code's four divisions, with Field, Battle and Interface
([System](system.md)).

Evidence: Camelot: the credits, "Programming", then "System", "Battle",
"Field", "User Interface" and "Support" (`gCredits_Lines`).

### Main routine

The main routine, `MainRoutine_Run`, starts when the console is switched on
and never finishes. All it does is run one [sequence](#sequence), clear away
what that one set up, and start the next. Walk into a battle, win it and
walk on, and the main routine has run three: the place, the battle, the
place again ([System](system.md#the-main-routine)).

Evidence: tradition: メインルーチン, the period's word for the code that runs
outside the interrupts.

### Sequence

A sequence is one whole kind of play, run from its start to its end. There
are four: the field sequence (walking around a place), the battle sequence
(one fight), the dice sequence (Lucky Dice) and the slots sequence (Lucky
Wheels). The code names the last three `SEQUENCE_BATTLE`, `SEQUENCE_DICE`
and `SEQUENCE_SLOTS`. The field sequence has no such name: it is asked for
by naming the place to walk around. Only one sequence runs at a time
([System](system.md#the-main-routine)).

Evidence: tradition: ゲームシーケンス.

### Field sequence

The field sequence runs one [scene](#scene), from the party's arrival to the
[exit](#exit) it leaves by. In Vale it runs from the moment Vale fades in
until Isaac steps through a door. On the way it answers whatever the leader
runs into, the [field requests](#field-request)
([Field code](src/field/README.md#the-field-sequence-serves-requests)).

Evidence: Camelot: the credits' "Field" (`gCredits_Lines`); tradition: sequence.

### Transition and next sequence

A transition is the handover from one [sequence](#sequence) to the next.
Stepping through a door is one, and so is going into a battle. The sequence
that is ending writes down what should run next, a scene or another
sequence, in the [main routine's work](#main-routines-work), and the
[main routine](#main-routine) starts it
([System code](src/system/README.md#the-main-routine)).

Evidence: tradition: シーケンス遷移.

### Main routine's work

`gMainRoutineWork` is the one block of memory that outlives every
[sequence](#sequence). Everything else a battle or a place set up is cleared
away when it ends, but this block is left alone, so the next sequence can
read it. It holds what runs next, who is in the party, their Djinn and
their coins, which is why a battle's winnings are still there back on the
field. It is emptied when the console is switched on and when a new game
begins. A save file keeps it, with the [game flags](#game-flag)
([System](system.md#saving)).

Evidence: tradition: work, named for its owner, the main routine.

### Frame

A frame is one picture on the screen and the game's work to prepare it,
about a sixtieth of a second. Everything that takes time is counted in
frames. When Dora, at her door in Vale, says `Isaac... That's a lie!`, she
pauses 60 frames before going on: one second
([System code](src/system/README.md#the-frame-loop)).

Evidence: tradition: フレーム, one pass of a game's processing.

### Wait

To wait is to let [frames](#frame) go by in the middle of a piece of code.
`wait(10)` lets ten frames of the game run and then carries on with the next
line. When a woman in Vale feels a tremor, her code starts the screen
shaking, waits ten frames, and only then goes on to her line. So a scene can
be written top to bottom, in the order things happen
([System](system.md#waiting-and-tasks)).

Evidence: tradition: WAIT, the period's word for it.

### Task

A task is a routine the game runs once in every [frame](#frame), even
while other code is [waiting](#wait), until it is taken off the list. The
task that types a villager's line into its window letter by letter is
one. The list, `gTasks`, has room for 20
([System code](src/system/README.md#the-task-runner)).

Evidence: tradition: タスク and its 実行関数.

### VBlank

VBlank is the short gap after the console has drawn one picture and before
it starts the next, about 60 times a second. At each one the console breaks
off whatever the game is doing and runs one small piece of its code. That
piece puts the finished [frame](#frame) on screen, prepares the sound and
reads the buttons ([System code](src/system/README.md#what-vblank-does)).

Evidence: tradition: V ブランク, the hardware's own name.

### Slowdown

The game has about a sixtieth of a second to get each [frame](#frame)
ready, before the console starts drawing the next picture. Slowdown is what
happens when a frame's work is not done in time: the frame stays on screen
for two pictures or more. Everything counted in frames, every walk, pause
and animation, drags together. The music does not, because the sound is
prepared at every [VBlank](#vblank) whether the frame is ready or not. So
the tune keeps its speed while the picture falls behind it
([System code](src/system/README.md#what-vblank-does)).

Evidence: tradition: 処理落ち.

### Work

A work is the block of memory where one part of the game keeps what it is
working on while it runs. It is named `g<Owner>Work`. The field's is
`gFieldWork`, and among what it holds are who the people in the scene are
and what the leader has just run into. The menus' is `gMenuWork`. Each is
found again through a [heap slot](#heap-slot). The
[main routine's](#main-routines-work) is the one that is kept apart and
outlives the rest
([System](system.md#saving)).

Evidence: tradition: ワーク, working memory, as in the hardware's Work RAM,
used here for a system's block.

### Heap and pools

The heap is where code gets memory for a while. Code that needs room for
something, such as a picture being unpacked, asks the heap and is handed a
block. The blocks are cut from two pools, which are simply two stretches of
free memory, one in each of the console's
[two areas](#iwram-and-ewram).

In each pool a new block always goes on top, and blocks come off from the
top: handing one back also hands back everything that was put on after it.
That is simple and quick. The whole heap is emptied before each
[sequence](#sequence), so a battle starts with nothing left over from the
cave it began in
([System code](src/system/README.md#the-heap)).

Evidence: plain English.

### Heap slot

A heap slot is one of 62 numbered places, each remembering where one block
from the [heap](#heap-and-pools) is, so that other code can find the block
again. The field's [work](#work) is found through one, and the menus' work
through another ([System code](src/system/README.md#the-heap)).

Evidence: plain English; the project's own word.

### IWRAM and EWRAM

IWRAM and EWRAM are the console's two areas of memory. The letters stand
for internal and external work RAM, RAM being memory. IWRAM is 32 KB inside
the processor, the chip that follows the game's instructions, and it is the
fastest memory there is. EWRAM is 256 KB beside the processor, and slower.
So the code that must be quickest is copied into IWRAM, and the big things
go in EWRAM: the map the party is walking on takes 64 KB of it
([System code](src/system/README.md#memory-map)).

Evidence: tradition: the hardware's own names.

### Main image

The main image is the code the processor runs straight from the cartridge,
without copying it into memory first. It is about 1 MB, in fourteen
[banks](#bank), and holds what is needed all the time: walking, battles,
windows, menus, shops, sound. What only one place needs is kept apart, in
[overlays](#overlay) ([System](system.md#overlays)).

Evidence: plain English; the project's own word.

### Game Pak

The Game Pak is the cartridge as a piece of hardware. It holds the game
itself, which cannot be changed: the code in its [banks](#bank), and every
picture, map and tune, listed in the
[resource directory](#resource-directory). It also holds the
[flash chip](#flash-chip) the saves are written to
([The Game Pak](src/gamepak/README.md)).

Evidence: Nintendo: "Game Pak", the instruction booklets' word for the
cartridge; tradition: GBATEK's "GamePak ROM", "GamePak SRAM" and "GamePak
Waitstates".

### Flash chip

The flash chip is the small chip on the cartridge the three save
[files](#file-and-part) are written to. It keeps what is written with the
power off. It is 64 KB, divided into 16 sectors of 4 KB. A sector is the
smallest piece the game saves at a time
([The Game Pak](src/gamepak/README.md#the-flash-chip)).

Evidence: the hardware's word, GBATEK's "GamePak Flash ROM"; the library's
file names are the project's, from the chip makers' names as GBATEK lists them
(Atmel, SST, Macronix).

### Bank

A bank is one of the fourteen stretches the [main image](#main-image) is cut
into, roughly one for each job: one for the field, one for the windows, one
for the battles, one for sound. Each starts at a fixed place with room to
spare after its code, so one bank can grow without moving anything another
depends on
([The Game Pak](src/gamepak/README.md#why-the-main-image-is-cut-into-banks)).
For readers who know older consoles: every bank is always in place, and none
is switched in or out.

Evidence: plain English; the project's own word.

### Bank folder

A bank folder is a folder of `src/` that holds one or more
[banks](#bank)' code. The folders are grouped by the four divisions. The
System has `system/`, `gamepak/` and `sound/`. The Field has `field/`. The
Battle has `battle/`, `game/` and `battle_effect/`. The Interface has
`window/`, `menu/` and `shop/` ([Source](src/README.md#banks-and-folders)).

Evidence: Camelot: the credits' four Programming divisions, "System",
"Battle", "Field" and "User Interface" (`gCredits_Lines`); `window`,
`menu` and `shop` from their own entries; plain English for the grouping.

### Far call

The code is cut into [banks](#bank), and the code in one often needs
something in another. A far call is how it asks. The trouble is that things
move about inside a bank whenever its code changes, so the asker cannot
know exactly where to go.

So each bank opens with a list, at a place that never moves, with one entry
for each thing other banks may ask of it. The asker goes to the entry, and
the entry, a [veneer](#veneer), sends it on. An entry is named after what it
leads to with `Far` added: the main routine starts a battle through
`Battle_RunFar`, which leads to `Battle_Run`
([The Game Pak](src/gamepak/README.md#why-the-main-image-is-cut-into-banks)).

Evidence: tradition: ジャンプテーブル for the table; "far call" is plain English.

### Veneer

A veneer is a tiny piece of code, 8 bytes long, that does one thing: it
jumps on to a piece of code somewhere else. It stands at a place that never
changes and points at something that may move, so everyone who needs that
thing can go to the veneer and be sent the right way. `Battle_RunFar` is
one, and all it does is jump to `Battle_Run`. The lists a [bank](#bank)
and an [overlay](#overlay) open with are made of them
([The Game Pak](src/gamepak/README.md#a-veneer)).

Evidence: plain English; the project's own word.

### Overlay

An overlay is the code of one place, stored packed small on the cartridge
and unpacked into memory only when the party arrives there. Vale's
villagers and what happens in Vale are in Vale's overlay, and are of no use
in Vault. There are 96 overlays for the 201 [scenes](#scene), because
neighbours share: Crossbone Isle and the floors of its cave have one between
them. They all unpack into the same 32 KB of memory, so only one is loaded
at a time ([System](system.md#overlays)).

Evidence: tradition: the GNU linker's word; the linker's `.bss` for the
globals' folder.

### Entry table and import table

An [overlay](#overlay) is a place's own code, loaded only while the party
is there. The rest of the game, the [main image](#main-image), cannot know
where things are inside it, so the two reach each other through two lists
of [veneers](#veneer).

The entry table is the first thing in the overlay. It has six veneers, one
for each thing the field asks of any scene, such as who stands where and
which exits it has. The import table is the last thing in it, and works the
other way: one veneer for each piece of the main image the overlay uses.
When Isaac goes in at the Sanctum's door in Vale, Vale's overlay asks the
field for that exit through its import table
([System code](src/system/README.md#how-an-overlay-reaches-the-rest-of-the-game)).

Evidence: plain English.

### Resource directory

The resource directory is the cartridge's one list of every packed thing on
it: each overlay, map and picture is a numbered row. The game finds a thing
by its row, so Vale's [overlay](#overlay) is row 883
([The Game Pak](src/gamepak/README.md#the-resource-directory-and-table-order)).

Evidence: plain English.

### File and part

A file is one of the three saves the File Screen offers. Each is kept on
the [flash chip](#flash-chip) as two parts, and each part fills one of the
chip's 4 KB sectors. A newly saved part is written to a free sector first,
and only then is the old copy given up, so there is no moment at which
neither copy is on the chip ([System code](src/system/README.md#saving)).

Evidence: Camelot: "Please choose a file." (きろくばしょを えらんでください).

### Background layer

A background layer is one of the console's four large sheets of picture,
stacked one behind another, each sliding as a whole. A sheet is made of
tiles, small square pictures laid side by side like a mosaic. A town is
drawn on three of them: the ground, the walls, and the roofs and treetops.
The World Map uses the two the console can scale and rotate
([Field code](src/field/README.md#towns-and-dungeons-on-screen)).

Evidence: tradition: the hardware's BG.

### Sprite and OAM

A sprite is a small picture the console can place anywhere on the screen,
over or between the [background layers](#background-layer). The people are
sprites, and so are the pointing hand in a menu and the letters of a spoken
line. OAM, the object attribute memory, is the console's list of where each
one sits, and the game writes it afresh every [frame](#frame)
([System code](src/system/README.md#the-frames-sprites)).

Evidence: tradition: スプライト and the hardware's OAM.

### Sprite animation

A figure on screen is often not one drawing but several laid over one
another, up to four. Each of those layers is a sprite animation: it has its
own run of [pictures](#picture) and steps through them on its own.

In a battle Isaac is three layers: his body, his weapon and his shadow. Body
and weapon change pose together, each from its own run of pictures, while
the shadow stays flat on the ground. On the
field a figure's shadow is not a layer but a fixed part of the figure
([Field code](src/field/README.md#sprites-and-their-animation)).

Evidence: plain English; no word of the time names one layer of a sprite.

### Picture

A picture is one drawing in a [sprite animation](#sprite-animation) or in
the animation of a map's tiles, such as one pose of Isaac's walk. It is
shown for a number of [frames](#frame) before the next one takes its place
([Field code](src/field/README.md#sprites-and-their-animation)).
Picture is the one word for a single drawing on a sheet of drawings.

Evidence: plain English; the time's パターン and コマ each mean other things too.

### Palette number

A drawing on the cartridge does not hold colours. It holds numbers, and a
palette is the list that says which colour each number stands for. The
console keeps sixteen palettes of 16 colours each. A palette number says
which of them a [sprite](#sprite-and-oam) or a tile, one of the small
squares a [background layer](#background-layer) is made of, is painted in.
So one drawing can appear in several colourings: three kinds of enemy share
the Skeleton's drawings, each with a palette of its own
([Graphics](src/graphics/README.md#how-the-console-draws-a-picture)).

Evidence: plain English for the hardware's own field.

### Palette fade

A palette fade moves every colour on screen a little closer to a target
colour each [frame](#frame), in equal steps, so that they all arrive
together. Vale fading in as the party arrives is one, and so is the tint
that comes over the screen while field Psynergy is cast
([Field code](src/field/README.md#the-palette-fade)).

Evidence: plain English.

### Sound

Every sound the game plays is asked for by a number, and the number's range
says what kind it is. Below 80 it is music: Vale's tune is 0. From 80 it is
a jingle, starting with the one for a solved puzzle. From 100 it is a sound
effect, an SE, starting with Vault's bell
([Sound](src/sound/README.md#what-a-sound-number-means)).

Evidence: Camelot: the credits' "Music" and "Sound Effects", and The Lost
Age's sound test's "Music", "Jingle" and "SE".

## The field

### Field

The field is everywhere the party walks: a town, a dungeon or the World Map.
The word also names the code that runs it, which is everything outside a
battle ([Field](field.md)).

Evidence: Camelot: the credits' "Field", and せんとう&フィールド
(`MsgUsableAnywhere`).

### Scene

A scene is one place the game loads as a whole: a town's streets, a floor
of a dungeon, a set of rooms, or the World Map. A few are screens instead,
such as the Title and the File Screen. There are 201, each a row of the
list `gScenes`: Vale is one, and the Sanctum's hall another, which is why
the screen fades between them. Each has a short label Camelot gave it
(`TITLE`, `WORLD`, `PLOGV00` for Vale on the stormy night of the prologue)
([Field](field.md#scenes-and-areas)).

Evidence: Camelot: The Lost Age's developer test message 05746 heads a location name シーン.

### Room

A room is the inside of a building: a house, a shop, an inn. Most rooms are
not [scenes](#scene) of their own. Isaac's house, Kraden's study and Vale's
inn are rooms of one scene, a [sheet of rooms](#sheet-of-rooms).

Evidence: Camelot: the scene labels' ROOM and RM (HAIROOM1, KUP_RM01,
FUCHINRM).

### Area

An area is a group of [scenes](#scene) that belong together, 71 in all.
Vale's streets, its plaza, its houses and the Sanctum are eight scenes of
one area. An area's scenes share their music and their name, which comes up
on screen only when the party arrives from another area. They also share
one set of short-lived [game flags](#game-flag)
([Field](field.md#scenes-and-areas)).

Evidence: Camelot: the area names.

### Map

A map is the ground a [scene](#scene) is drawn on: a grid of
[cells](#cell), 128 on a side, shown as three
[background layers](#background-layer). Two scenes can share one: Vault
before Kalay and Vault after are two scenes on the same map
([Maps](src/map/README.md)).

Evidence: Camelot: the credits' "Maps".

### Still picture

On the World Map the land tilts away from the party. The tilt is drawn in a
special way that leaves no [background layer](#background-layer) for
[windows](#window) to appear on.

So when a menu or a conversation opens there, the game stops. It draws the
tilted view once, as a picture that stands still, on an ordinary layer.
That frees a layer for the windows, and the still picture is what the
player sees behind them
([Field code](src/field/README.md#the-world-map-in-perspective)).

It is not the map of the whole world that R shows. That is the world
picture ([Field code](src/field/README.md#the-world-picture)).

Evidence: plain English for what the player sees, the view standing still.

### Sheet of rooms

A sheet of rooms is one [scene](#scene) whose map holds several
[rooms](#room) side by side, all drawn in one style. The door the party
comes in by picks the room. The rooms need not be in one town: Vale's inn
and the room of Babi's scholars in Tolbi are on the same sheet, because both
are drawn as cottages. So a sheet is named for its style, as Cottage rooms
or Stucco rooms ([Scenes](src/scene/README.md#design)).

Evidence: plain English; the project's own word.

### Cell

A cell is one 16-pixel square of a [map](#map). It says which [cell picture](#cell-picture) is drawn there, the
shape and height of the ground, and its [cell behavior](#cell-behavior)
([Maps](src/map/README.md#cells)).

Evidence: plain English.

### Cell picture

A cell picture is what is drawn on one [cell](#cell). It is four tiles, two
by two. A tile is the console's own building block, a square 8 pixels on a
side ([Maps](src/map/README.md#cells)).

Evidence: plain English.

### Cell behavior

A cell behavior is what a [cell](#cell) does when the leader stands on it,
walks into it or searches it. It is what makes one cell a wall, another a
ledge to jump down, and another a jar worth pressing A at
([Maps](src/map/README.md#cells)).

Evidence: plain English; Camelot's 属性 (attribute) means an element.

### Actor

An actor is anything on the field that moves or is drawn on its own. The
party and the townspeople are actors, and so are the footprints left in
snow and the point the camera follows. A scene has room for 64, the places
of the list `gActors`
([Field code](src/field/README.md#everything-that-moves-runs-an-actor-script)).

Evidence: tradition: the Western word for a thing on a game's stage.

### Actor id

An actor id is an [actor](#actor)'s number in its scene. Numbers 0 to 7 are
kept for the eight characters the code knows (`ACTOR_GARET` is 1), so Jenna
has a number, 5, although she is not one of the four in the party. From 8
on come the people and things the scene places, in the order of its list of
them ([Scenes](src/scene/README.md#who-stands-where)).

Evidence: plain English.

### Actor placement

An actor placement is one row of a scene's list of who stands where. It
gives one person or thing its looks, the spot it stands on, the way it
faces, how it moves and the condition for it being there at all. One row of
Vale's puts an old woman at her spot, facing north and standing still
([Scenes](src/scene/README.md#who-stands-where)).

Evidence: plain English for what a row does, place an actor.

### Actor script

An actor script is the list of small commands an [actor](#actor) follows, a
little each [frame](#frame): move here, wait, jump, go back to the start.
The commands' names start `SC_`. A villager who wanders near home is running
one. A command can also hand the actor to a built-in script, a piece of the
field's own code that steers it, and the player's control of the leader is
one of those
([Field code](src/field/README.md#how-an-actor-script-runs)).

Evidence: tradition: スクリプト for a character's commands.

### Watched actor

The watched actor is the one another [actor](#actor) keeps its eye on. An
[actor script](#actor-script)'s commands to face, follow or walk up to
someone do not say whom. They mean the watched actor, so one script serves
whoever is watching whomever. In the Elemental Star Room of Sol Sanctum,
Jenna, Kraden, Saturos, Menardi and Alex are each given the leader to watch
and then all run the same script
([Field code](src/field/README.md#who-runs-which-script)).

Evidence: plain English for what the commands do.

### Field effect

A field effect is a small [actor](#actor) that is only decoration, such as
the sparks of a field Psynergy. It moves, grows and spins by a script of its
own ([Field code](src/field/README.md#one-actor-for-everything)).

Evidence: plain English.

### Trigger

A trigger is one row of a scene's list of things that can happen. The row
says what starts it (talking, searching, stepping somewhere, using
Psynergy), who or where it is for, the condition under which it still
counts, and what it does. When the player does one of those things, the
first row that fits runs. In Vale, one row says that walking north into the
door of Isaac's house runs the [event](#event) at that door
([Scenes](src/scene/README.md#what-a-trigger-row-says)).

Evidence: tradition: the Western word for a spot on a map that starts something.

### Step trigger

A step trigger is a [trigger](#trigger) that runs when the leader steps
onto, or walks into, a [cell](#cell) with a given
[cell behavior](#cell-behavior). Walking north into the ladder in Vale
starts the climb onto the roof
([Scenes](src/scene/README.md#which-triggers-a-cell-starts)).

Evidence: plain English.

### Examine trigger

An examine trigger is a [trigger](#trigger) that runs when the player
presses A facing a [cell](#cell). Searching a locked door in Vale is one.
Jars, barrels, boxes, graves, books and wells have lines of their own,
the same in every place
([Scenes](src/scene/README.md#which-triggers-a-cell-starts)).

Evidence: plain English.

### Exit

An exit is a way out of a [scene](#scene): a doorway, a path, the edge of
the map. It is named for where it leads, so the way from Vale's plaza out to
the World Map is `VALE_PLAZA_TO_WORLD_MAP`. A scene's list of exits says
which scene and [entrance](#entrance) each one leads to, and one exit can
have several rows, so that the same path leads somewhere new once the story
has moved on ([Scenes](src/scene/README.md#exits)).

Evidence: Camelot: the game's text, 出口.

### Entrance

An entrance is where the party appears in a [scene](#scene), named for
where it comes from: `VALE_PLAZA_FROM_SANCTUM` is the spot in Vale's plaza
for a party arriving from the Sanctum. It gives a spot to stand on, a way to
face and how far the camera may travel
([Scenes](src/scene/README.md#entrances)).

Evidence: Camelot: the game's text, 入口.

### Field request

A field request is the field asking itself to deal with something the
walking leader has run into. There are twelve kinds: an exit taken, a
battle due, a person to talk to, a cell to search, a menu to open, and
others. Press Start and the request for the Pause menu is set. The
[field sequence](#field-sequence) reads the requests and answers the most
urgent first ([Field code](src/field/README.md#the-twelve-requests)).

Evidence: plain English; the project's own word.

### Event

An event is the piece of code a [trigger](#trigger) runs when something
happens in a scene: a cutscene, or a conversation that does more than show
one line. It is named for its scene. `Vale_EnterIsaacsHouse` is what happens
at the door of Isaac's house, where Dora will not open to a party that has
left Vale and not yet reached Kalay. In the code it sits between
`Event_Start` and `Event_End`, one [event command](#event-command) to a
line ([Scenes](src/scene/README.md#events)).

Evidence: Camelot: the Shining Force III credits' "Events", by two of Golden
Sun's field programmers.

### Event command

An event command is one step of an [event](#event). The ones named `Event_`
are for speech, pauses and where the party goes next, the ones named
`Actor_` move the people, and the ones named `FieldCamera_` move the camera.
At Dora's door, `Event_ShowMessage(ACTOR_DORA, 0)` shows the event's next
line as Dora's. The 0 asks for nothing special
([Scenes](src/scene/README.md#events)).

Evidence: plain English.

### Destination

The destination is the [scene](#scene) and [entrance](#entrance) the game
goes to next. An [exit](#exit) sets it, and an [event](#event) can set it
itself.

After a battle the party goes back where it was fighting, unless an event
chose somewhere else. After a defeat it goes to the restart point, usually
the last Sanctum it visited, again unless an event chose somewhere else
([System code](src/system/README.md#running-another-sequence)).

Evidence: plain English.

### Game flag

A game flag is one of the 4,096 yes-or-no facts the game remembers and
saves, such as a chest being open or Mt. Aleph having erupted. Two ranges of
256 are short-lived instead. One is wiped each time the party enters a
scene, which is why a puzzle is set up again when the party walks back in.
The other is wiped on entering a new [area](#area)
([Game rules](src/game/README.md#the-game-flags)).

Evidence: Camelot: 田口泰宏, the System programmer, speaks of flags (Shining
Force III, 1998).

### Debug menus

The debug menus are tools Camelot left in the released game for its own use,
which the player is never shown. The main one is the debug scene selector.
It lists every [scene](#scene) by its label and area name, and sends the
party straight to the one picked.

Among the scenes it reaches are eight test scenes, all the same dark room
where four Djinn stand waiting to be collected. The Pause menu gains three
lines, `Rough menu`, `Recovery test` and `Restart quest`, but they are only
labels now, and picking one just closes the menu. There is also a sound
test ([Debug scenes](src/debug/README.md#how-the-debug-scenes-are-reached)).

Evidence: Camelot: the menus' labels ("Rough menu", "Recovery test",
パラメータセット), the scene codes TEST0 to TEST7 and The Lost Age's サウンドテスト.

## The battle

### Battle

A battle is one fight between the party and a
[formation](#formation) of enemies, from its opening to the experience and
coins at the end. It is a [sequence](#sequence) of its own, so the place the
party was walking in is cleared away while it runs and loaded again after
([Battle](battle.md)).

Evidence: Camelot: the credits' "Battle"; `battle/transition/` takes the name of
the code that draws it.

### Unit

A unit is one fighter in a battle, on either side: Isaac is a unit, and so
is each enemy. The game keeps the same sheet of facts for every one of
them, its stats, Psynergy, items, Djinn and ailments, which the code calls
`struct Unit`. A unit in the party is a party member
([Game rules](src/game/README.md#the-unit)).

Evidence: plain English; the game's キャラ means only a party member.

### Formation

A formation is one group of enemies a battle is fought against. The game
has 380 of them in a list. One is a lone Vermin, and another is a Vermin
with a Bat
([Battles](src/battle/README.md#the-enemies-of-a-formation)).

Evidence: plain English.

### Turns and actions

A turn is one round of a battle, in which every [unit](#unit) acts once. An
action is one unit's go within it: Isaac's attack is one action, and the
Vermin's answer is another ([Battles](src/battle/README.md#turns)).

Evidence: Camelot: "HP recovery / turn" (かいふくHP/ターン) and 行動 for one
unit's go.

### Staging

Staging is how an [action](#turns-and-actions) is shown, as apart from what
it does: who steps forward, what line announces it and which
[battle effect](#battle-effect) plays
([Battles](src/battle/README.md#staging-and-announcement)).

Evidence: tradition: 演出.

### Playback queue

The game works out what an [action](#turns-and-actions) does before it
shows any of it. The playback queue is the list of what is left to show for
each target: the flinch, the line such as `Isaac takes 27 damage!`, the HP
going down. It is played once the [battle effect](#battle-effect) says the
blow has landed
([Battles](src/battle/README.md#how-outcomes-reach-the-screen)).

Evidence: plain English.

### Battle effect

A battle effect is the show an ability puts on over a battle, such as
Quake's shaking ground or a summon's arrival. Each is a piece of code that
moves and draws its parts [frame](#frame) by frame. It decides when a hit
is shown, never what the hit does
([Battle effects](src/battle_effect/README.md)).

Evidence: Camelot: the credits' "Battle Effects", beside "Battle Characters"
and "Battle Backgrounds", a Graphic Arts heading; the code is the Battle
division's.

### Battle background

A battle background is the picture behind a battle. It is chosen by where
the fight happens, such as the grassland, Goma Cave or a ship's deck
([Graphics](src/graphics/README.md#battle-backgrounds)).

Evidence: Camelot: the credits' "Battle Backgrounds".

## The interface

### Interface

The interface is everything the player reads and chooses from: the
[windows](#window), the [messages](#message) typed into them, the menus, and
the counters of the shops, sanctums and inns ([Interface](interface.md)).

Evidence: Camelot: the credits' "User Interface".

### Window

A window is a bordered box the game opens for text or choices. Talk to a
villager and one grows out of its own centre, then shrinks back into it when
the line is done ([Windows](src/window/README.md#windows)).

Evidence: Camelot: ウインドウ, as in "Window color" (`MsgWindowColorLabel`).

### Border

The border is the edge drawn around a [window](#window). It is drawn in the
window colour the player chose in the settings, as the window's filling is
([Windows](src/window/README.md#windows)).

Evidence: plain English.

### Message

A message is a piece of text the game types into a [window](#window) a few
letters at a time, stopping for a button where a page ends. A villager's
line is one, and so is a battle's `Isaac takes 27 damage!`. Up to three can
be typing at once ([Windows](src/window/README.md#typing)).

Evidence: Camelot: the settings label メッセージスピード ("Message" in English,
`MsgMessageSpeedLabel`).

### The screens

The screens are the Title, the File Screen, the Pause menu, the field menu
with its Psynergy, Djinn, Item and Status screens, the settings menu, name
entry and the world picture. The File Screen is where a
[file](#file-and-part) is chosen, copied or erased, and the Pause menu is
what Start opens on the field ([Interface](interface.md)).

Evidence: Camelot: "To the File Screen", "The Pause menu is not..."
(`MsgFieldPauseUnavailable`), the scene label TITLE; the rest plain English.

### Menu

The menu is the field menu, the row of icons that A opens on the field, and
its Psynergy, Djinn, Item and Status screens
([Menus](src/menu/README.md#the-screens)). The folder `menu/` also holds the
staff credits and the title intro.

Evidence: Camelot: "Rough menu" (`MsgWorkspaceRoughMenu`), "Menu test" and
the debug row MENU.

### Shop

A shop is any counter the party does business at: a shop's, a sanctum's or
an inn's. Bilibin's innkeeper asking 7 coins a guest is the shop code at
work ([Shops](src/shop/README.md)).

Evidence: the game: "weapon shop", "Thank you for shopping".

## The folders

The folders of `src/` that are not named for a bank of code.

### Game rules

The game rules are what a [unit](#unit), an item or a Djinni is and does:
how much PP Cure costs, what Flint adds to a character. They are kept in
one place because the field, the battles and the menus all need the same
answers ([Game rules](src/game/README.md)).

Evidence: Camelot: the credits' "Game Design" and "Game Planning"; plain
English "game rules", said so.

### Graphics

Graphics is the game's pictures as files anyone can open and edit. Each
folder is named for what it holds or the place it belongs to
([Graphics](src/graphics/README.md)).

Evidence: Camelot: the credits' "Graphic Arts"; plain English.

### Graphics folders

For readers of the files: the folders of pictures under `src/graphics/`,
and what is in each.

- `battle_backgrounds`: the [battle backgrounds](#battle-background).
  Evidence: Camelot: the credits' "Battle Backgrounds".
- `battle_effects`: the [battle effects](#battle-effect)' drawings and
  colours. Evidence: Camelot: the credits' "Battle Effects".
- `field_effects`: the sparks field Psynergy draws. Evidence: the
  [field effect](#field-effect)'s word.
- `font`: the letters, how wide each one is, and the tables that match
  text to letters. Evidence: plain
  English.
- `monster`: the enemies' battle pictures. Evidence: Camelot: the credits'
  "Monster Design"; the game: "Monsters won't attack".
- `palettes`: the windows' colours and the colours most sprites share. Evidence:
  plain English.
- `palette_editor`: the one picture of a colour editor, a tool the
  [debug menus](#debug-menus) reach. Evidence: plain English.
- `sprite`: the drawings of every person and thing that moves, and their
  animations. Evidence: the hardware's
  word ([Sprite and OAM](#sprite-and-oam)); Camelot: the credits' "Map
  Characters" and "Battle Characters" for the work.
- `tileset`: the small squares and colours a map's
  [cell pictures](#cell-picture) are drawn from. Evidence: tradition: the
  tile set; Camelot: the credits' "Maps" for the work.
- `ui`: every [Interface](#interface) picture, such as the menus' icons. Evidence: Camelot: the
  credits' "User Interface", whose banks place them; plain English for the
  subfolders.

### Places and minigames

The graphics folders named for a place or a thing in the game:

- `colosso`, the Colosso's lettering, counters and gauge. Evidence: the
  game: "Colosso".
- `lamakan_desert`, the desert's heat gauge. Evidence: its area name,
  Lamakan Desert.
- `sol_sanctum_statue`, the pictures for Sol Sanctum's statue room.
  Evidence: its area name, Sol Sanctum.
- `lucky_dice` and `lucky_wheels`, the two minigames' pictures.
  Evidence: the game: "Lucky Dice", "Lucky Wheels".
- `world_map`, the World Map's own pictures.
  Evidence: the game: "World Map".
- `title`, the Title's pictures and its prompt. Evidence: Camelot: the scene
  label TITLE.
- `credits`, the staff roll's font. Evidence: Camelot: the credits
  themselves ("GOLDEN SUN STAFF CREDITS").

### Tileset

A tileset is the set of tiles, with their colours, that a map's
[cell pictures](#cell-picture) are put together from. A tile is a square 8
pixels on a side, the console's own building block
([Graphics](src/graphics/README.md#tilesets)).

Evidence: tradition: the tile set; plain English.

### Monster

A monster is an enemy as the player sees it, its picture in a battle
([Graphics](src/graphics/README.md#monsters)). In the code an enemy is a
[unit](#unit) like any other.

Evidence: Camelot: the credits' "Monster Design"; the game: "Monsters won't
attack".
