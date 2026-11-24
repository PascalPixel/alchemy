# Glossary

These are the words the pages about The Broken Seal share. Each entry says
what the word means and links to the page that explains it.

A word is the one a modern engineer who knows the Game Boy Advance, or a
player who knows the game in English, follows without a gloss. An entry's
Sources line says what the game itself, Sega's SGL manual and GBATEK each
call the thing, or none where one has no word for it.

## Golden Sun

### The two games

Golden Sun is a role-playing game for the Game Boy Advance, told in two
parts. The first is The Broken Seal (2001), which these pages are about and
whose builds are named `tbs`. Its sequel is The Lost Age (2002), named `tla`.

Sources: the game: the credits' "GOLDEN SUN STAFF CREDITS", in the Japanese
edition "OUGON NO TAIYO STAFF". SGL: none. GBATEK: none.

### Camelot

Camelot Software Planning made both games, and Nintendo published them. The
game opens on Nintendo's logo and then Camelot's. Their source was never
released.

Sources: the game: the credits' "CAMELOT Co.,Ltd.". SGL: none. GBATEK: none.

### The party

The party is Isaac and whoever is travelling with him. In The Broken Seal
that is Isaac, Garet, Ivan and Mia, and they share one purse of coins. In a
battle each of them is a [unit](#unit)
([Game rules](src/game/README.md#the-party-and-its-coins)).

Sources: the game: "joined your party" (なかま, and once パーティ). SGL: none.
GBATEK: none.

### The places, in story order

The story starts in Vale, under Mt. Aleph, and follows the thieves of the
Elemental Stars through Vault, Bilibin, Kolima, Imil, Mercury Lighthouse,
Xian, Altin, Kalay and Tolbi to Lalivero and Venus Lighthouse, where the game
ends. Pages list places in that order, and
[Places](https://alchemy.pascalpixel.com/tbs/places) shows every one.

Sources: the game: the area names, such as "Mercury Lighthouse". SGL: none.
GBATEK: none.

### Prologue

The prologue is the stormy night three years before the journey, played
before the story proper. Isaac and Garet are children. Huge rocks fall
across Vale's paths in the storm, Garet joins Isaac, and the elders hold
back the Boulder on Mt. Aleph with their Psynergy. When the night ends the game says `Three
years later...` and the journey begins. The prologue is a scene of its
own, Vale on that night (`SceneId_ValePrologue`), which stands in for Vale
while it lasts ([Scenes](src/scene/README.md)).

Sources: the game: the debug scene label PLOGV00. SGL: none. GBATEK: none.

### Psynergy and PP

![Isaac casts Growth in battle: light gathers around him, then a spiked vine bursts out of the ground under Ivan](https://alchemy.pascalpixel.com/abilities/growth.gif)

Psynergy is the game's magic. Some is cast in battle, such as Quake, Cure
and Growth, and some on the field, such as Move and Reveal
([Field code](src/field/README.md#how-field-psynergy-reaches-its-target)).

Each use costs PP, Psynergy Points: Quake costs 4, Cure 3 and Move 2. PP
comes back as the party walks anywhere but in a town, and a night at an inn
restores it.

Sources: the game: "Psynergy", "Psynergy Points" (エナジー, and EP for PP). SGL:
none. GBATEK: none.

### The four elements

The four elements are Venus (earth), Mercury (water), Mars (fire) and Jupiter
(wind), and the game always lists them in that order. Every character,
Djinni and summon belongs to one. Isaac is Venus, Garet Mars, Ivan Jupiter
and Mia Mercury ([Game rules](src/game/README.md#element-levels)).

Sources: the game: "Venus", "Mercury", "Mars" and "Jupiter" (ぞくせい for an
element). SGL: none. GBATEK: none.

### Power and Resist

For each of the [four elements](#the-four-elements), every fighter has a
Power and a Resist, which the Status screen shows. Power raises the damage
and healing of what the fighter casts in that element. Resist lowers the
damage it takes from it. A damage line that ends in three exclamation marks
has struck the element its target resists worst
([Battles](src/battle/README.md#the-element-multiplier)).

Sources: the game: the Status screen's "Power" and "Resist" (パワー, たいせい). SGL:
none. GBATEK: none.

### Djinn

![Ivan unleashes the Jupiter Djinni Gust in battle: pink wind streams across the field, then a blue blast strikes its target](https://alchemy.pascalpixel.com/abilities/gust.gif)

Djinn are elemental creatures the party collects. One is a Djinni, several
are Djinn. Set on a character, a Djinni raises that character's stats and
helps decide its class: Flint, a Venus Djinni, gives 8 HP, 4 PP and 3
Attack. Unleashed in battle, it does its own move and goes on standby, ready
for a [summon](#summons) ([Game rules](src/game/README.md#djinn-and-classes)).

Sources: the game: "Djinn", "Djinni" (ジン). SGL: none. GBATEK: none.

### Summons

![Isaac summons Ramses: the screen wipes to a desert where a giant stone figure appears and pounds its targets with its fist](https://alchemy.pascalpixel.com/abilities/ramses.gif)

A summon spends [Djinn](#djinn) on standby to call up an elemental being.
The summon named Venus takes one Venus Djinni, Ramses two, Cybele three and
Judgment four. The Djinn it spent then need a few
[turns](#turns-and-actions) to recover
([Battles](src/battle/README.md#commands-and-ability-ids)).

Sources: the game: "Summon" (しょうかん). SGL: none. GBATEK: none.

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

Sources: the game: none. SGL: none. GBATEK: none.

### The original

The original is the game as [Camelot](#camelot) shipped it: the cartridge of
each [edition](#editions). The source reproduces each one byte for byte,
bugs included.

Sources: the game: none. SGL: none. GBATEK: none.

### Editions

Each game was released in Japanese, English, German, Spanish, French and
Italian, and each release is an edition (`tbs-ja` to `tbs-it`). The five
other than the Japanese are the international editions. They differ in more
than their text: the Japanese edition can have twelve
[windows](#window) open at once, the others eight.

Sources: the game: none. SGL: none. GBATEK: none.

### pret

For readers of the code: pret is the group of fans behind the
decompilations of other Game Boy Advance games. This source borrows its
habits so that anyone who has read one of pret's games can read this one. A
named piece of code is spelled like `Battle_Run`, a value the whole game
shares starts with `g`, like `gTasks`, and bugs are marked `BUG` and
`BUGFIX`.

Sources: the game: none. SGL: none. GBATEK: none.

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

Sources: the game: none. SGL: none. GBATEK: none.

### Unused

Some of what is on the cartridge never happens in the game. A comment
opening with `Unused:` marks such a thing and gives the reason. One is a
second version of Garet joining Isaac on the stormy night: every path to
the spot that would start it crosses the spot that starts the version the
player sees. `UNUSED` in the code marks the plainest case, a named piece of
code that nothing else in the game ever asks for.

What only the [debug menus](#debug-menus) reach is not unused.
[Unused](https://alchemy.pascalpixel.com/tbs/unused) collects them.

Sources: the game: none. SGL: none. GBATEK: none.

### Module header

For readers of the code: the source is many files, and a file can only use
what another file has announced. A module header is the short file where
one file of code makes its announcements. The main routine's header is
where the rest of the game learns that `MainRoutine_Run` exists. A header
sits in `include/` under its code's folder, named for its code:
`src/system/main_routine.c` has `include/system/main_routine.h`
([Source](src/README.md#headers)).

Sources: the game: none. SGL: ヘッダファイル, its `sgl.h`. GBATEK: none.

### Object

For readers of the code: the game is not built in one go. Each file of the
source is turned into bytes separately, and the result is called an object.
The objects are then laid end to end to make the cartridge. The word has
nothing to do with the things on the field that the game's text calls
objects, which are [actors](#actor), nor with GBATEK's OBJ, which is a
[sprite](#sprite-and-oam).

A very long file is written as several smaller parts beside it, each named
for it, and still makes one object. The shops are one: `src/shop/shop.c`
gathers the inn, buying and selling from `shop_inn.c`, `shop_buy.c` and
`shop_sell.c`
([Source](src/README.md#objects-and-parts)).

Sources: the game: none. SGL: オブジェクト (`OBJECT`), a placed model. GBATEK:
"OBJ", a sprite.

## The system

### System

The System is the code everything else stands on: starting up, deciding what
runs next, keeping time, handing out memory, saving and sound. The field,
the battles and the menus all use it and none of them could run without it.
It is one of the code's four divisions, with Field, Battle and Interface
([System](system.md)).

Sources: the game: the credits' "Programming", then "System". SGL: none.
GBATEK: none.

### Main routine

The main routine, `MainRoutine_Run`, starts when the console is switched on
and never finishes. All it does is call one [sequence](#sequence) as a
function, clear away what that one set up when it returns, and call the
next. Walk into a battle, win it and walk on, and the main routine has run
three: the cave, the battle, the cave again. It is a loop, but one trip
round it is a whole sequence: the loop that goes round once a
[frame](#frame) is inside [`wait()`](#wait)
([System](system.md#the-main-routine)).

Sources: the game: none. SGL: メインループ, the loop run once a frame. GBATEK: none.

### Sequence

A sequence is one whole kind of play, run from its start to its end. There
are four: the field sequence (walking around a [scene](#scene)), the battle
sequence (one fight), the dice sequence (Lucky Dice) and the slots sequence
(Lucky Wheels). The code names the last three `SEQUENCE_BATTLE`,
`SEQUENCE_DICE` and `SEQUENCE_SLOTS`. The field sequence has no such name:
it is asked for by naming the scene to walk around. The Title and the File
Screen are scenes, so the field sequence runs them too, and a menu or a
shop is a function called from inside a sequence. Only one sequence runs
at a time
([System](system.md#the-main-routine)). The sound driver's sequence is
another thing, a piece of music.

Sources: the game: none. SGL: シーケンス, a piece of music. GBATEK: none.

### Field sequence

The field sequence runs one [scene](#scene), from the party's arrival to the
[exit](#exit) it leaves by. In Vale it runs from the moment Vale fades in
until Isaac steps through a door. On the way it answers whatever the leader
runs into, the [field requests](#field-request)
([Field code](src/field/README.md#the-field-sequence-serves-requests)).

Sources: the game: the credits' "Field". SGL: none. GBATEK: none.

### Transition and next sequence

A transition is the handover from one [sequence](#sequence) to the next.
Stepping through a door is one, and so is going into a battle. The sequence
that is ending writes down what should run next, a scene or another
sequence, in the [main routine's state](#main-routines-state), and the
[main routine](#main-routine) starts it. The word is kept for this
handover. The wipe or fade the player sees while it happens is a screen
transition, `ScreenTransition_` in the code, which is only the drawing
([System code](src/system/README.md#the-main-routine)).

Sources: the game: none. SGL: none. GBATEK: none.

### Main routine's state

`gMainRoutineState` is the block of memory that outlives every
[sequence](#sequence): one C struct in a global. What a battle or a scene
set up for itself is cleared away when it ends, but this block is left
alone, so the next sequence can read it. It holds what runs next, who is in
the party, their Djinn and their coins, which is why a battle's winnings
are still there back on the field. It is emptied when the console is
switched on and when a new game begins. Two other things are left alone
with it, the [game flags](#game-flag) and the snapshot of where a scene's
[actors](#actor) stand, and a save file keeps all three
([System](system.md#saving)).

Sources: the game: none. SGL: none. GBATEK: none.

### Frame

A frame is one image on the screen and the game's work to prepare it,
about a sixtieth of a second. Everything that takes time is counted in
frames. When Dora, at her door in Vale, says `Isaac... That's a lie!`, the
event at her door then calls [`wait()`](#wait) for 60 frames before going
on: one second, when nothing slows the game down
([System code](src/system/README.md#the-frame-loop)).

Sources: the game: none. SGL: フレーム, one picture shown. GBATEK: "Frame 0-1", a
bitmap buffer.

### Wait

To wait is to let [frames](#frame) go by in the middle of a piece of code.
`wait(10)` runs ten frames of the game, there inside the call, and then
carries on with the next line: the game's frame loop is this function, and
there is no other. One piece of code waits at a time. An
[actor script](#actor-script)'s `SC_WAIT` is another thing, a count kept in
the actor. When a woman in Vale feels a tremor, her code starts the screen
shaking, waits ten frames, and only then goes on to her line. So a scene can
be written top to bottom, in the order things happen
([System](system.md#waiting-and-tasks)).

Sources: the game: none. SGL: `slSynch`, which waits for the next picture
(次のビデオ表示を待つ). GBATEK: none.

### Task

A task is a routine on a list that [`wait()`](#wait) calls once in every
[frame](#frame), until it is taken off the list. Tasks run only while some
code is waiting, and a task never waits itself. A task is a whole piece
of work, such as moving every [actor](#actor) a step, not one thing in the
world. The task that types a villager's line into its window letter by
letter is one. Each task has a rank, which the code calls its `priority`,
and a higher rank runs earlier in the frame. A few tasks are called by the
[VBlank](#vblank) handler instead, once a VBlank. The list, `gTasks`, has
room for 20
([System code](src/system/README.md#the-task-runner)).

Sources: the game: none. SGL: イベント (`EVENT`) with its 実行関数 is this thing; its
タスク is the second processor's. GBATEK: none.

### VBlank

VBlank is the short gap after the console has drawn the screen and before
it starts drawing it again, about 60 times a second. At each one the console
breaks off whatever the game is doing and runs one small piece of its code.
That piece puts the finished [frame](#frame) on screen, prepares the sound
and reads the buttons ([System code](src/system/README.md#what-vblank-does)).

Sources: the game: none. SGL: ブランキング. GBATEK: "V-Blank", "VBlank".

### Slowdown

The game has about a sixtieth of a second to get each [frame](#frame)
ready, before the console starts drawing the screen again. Slowdown is what
happens when a frame's work is not done in time: the frame stays on screen
while the console draws the screen twice or more. Everything counted in
frames, every walk, pause and animation, drags together. The music does
not, because the sound is prepared at every [VBlank](#vblank) whether the
frame is ready or not. So the tune keeps its speed while the image on
screen falls behind it ([System code](src/system/README.md#what-vblank-does)).

Sources: the game: none. SGL: 処理落ち. GBATEK: none.

### Fixed-point number

The console has no decimals, so a position, a speed or a scale is a whole
number counted in 65,536ths: its upper 16 bits are the integer part and its
lower 16 bits the decimal part. `toFIXED(0.8)` writes one, `FIXED_ONE` is
1.0, and `FIXEDtoINT()` gives back the integer part, the pixel a position is
on. In the code its type is `FIXED`. A routine that takes such numbers
where its plain form takes whole pixels ends in `FX`
(`Actor_SetPositionFX`), and angles are counted the same way, 65,536 to the
turn, written `DEGtoANG(90)`. `FIXED`, `toFIXED`, `DEGtoANG` and the `FX`
suffix are the words of the reference for SGL, Sega’s library for the Saturn,
whose fixed-point, angle and matrix design this code matches; the current
matrix's `Matrix_Push`, `Matrix_RotateX` and `Matrix_Translate` are modelled
on its `slPushMatrix`, `slRotX` and `slTranslate`.

Sources: the game: none. SGL: 固定小数点 (`FIXED`), with `toFIXED`, `DEGtoANG` and
the FX suffix. GBATEK: "Integer portion" and "Fractional portion".

### State

A system's state is the block of memory it keeps its running values in. It
is set up when the system starts and kept for as long as the system runs:
one C struct, `struct <Owner>State`, in a global named `g<Owner>State`. The
field's is `gFieldState`, and among what it holds are who the people in the
scene are and what the leader has just run into. The menus' is
`gMenuState`. Each is found again through a [heap slot](#heap-slot). The
[main routine's](#main-routines-state) is the one that is kept apart and
outlives the rest
([System](system.md#saving)).

Sources: the game: none. SGL: ワーク (`WORK`), a pooled 64-byte block attached to
an event. GBATEK: "Work RAM".

### Heap and pools

The heap is where code gets memory for a while. Code that needs room for
something, such as an image being unpacked, asks the heap and is handed a
block. The blocks are cut from two pools, which are simply two stretches of
free memory, one in each of the console's
[two areas](#iwram-and-ewram).

In each pool a new block always goes on top, and blocks come off from the
top: handing one back also hands back everything that was put on after it.
That is simple and quick. The whole heap is emptied before each
[sequence](#sequence), so a battle starts with nothing left over from the
cave it began in
([System code](src/system/README.md#the-heap)).

Sources: the game: none. SGL: none. GBATEK: none.

### Heap slot

A heap slot is one of 62 numbered places, each remembering where one block
from the [heap](#heap-and-pools) is, so that other code can find the block
again. The field's [state](#state) is found through one, and the menus'
state through another ([System code](src/system/README.md#the-heap)).

Sources: the game: none. SGL: none. GBATEK: none.

### IWRAM and EWRAM

IWRAM and EWRAM are the console's two areas of work RAM, RAM being memory,
as Game Boy Advance programmers name them; GBATEK calls them the on-chip
and the on-board work RAM. IWRAM is 32 KB inside
the processor, the chip that follows the game's instructions, and it is the
fastest memory there is. EWRAM is 256 KB beside the processor, and slower.
So the code that must be quickest is copied into IWRAM, and the big things
go in EWRAM: the map the party is walking on takes 64 KB of it
([System code](src/system/README.md#memory-map)).

Sources: the game: none. SGL: none. GBATEK: "On-chip Work RAM" and "On-board
Work RAM".

### Main image

The main image is the code the processor runs straight from the cartridge,
without copying it into memory first. It is about 1 MB, in fourteen
[banks](#bank), and holds what is needed all the time: walking, battles,
windows, menus, shops, sound. What only one scene needs is kept apart, in
[overlays](#overlay) ([System](system.md#overlays)).

Sources: the game: none. SGL: none. GBATEK: none.

### Game Pak

The Game Pak is the cartridge as a piece of hardware. It holds the game
itself, which cannot be changed: the code in its [banks](#bank), and every
image, map and tune, listed in the
[resource directory](#resource-directory). It also holds the
[flash chip](#flash-chip) the saves are written to
([The Game Pak](src/gamepak/README.md)).

Sources: the game: none. SGL: none. GBATEK: "Game Pak", "GamePak ROM".

### Flash chip

The flash chip is the small chip on the cartridge the three save
[files](#file-and-part) are written to. It keeps what is written with the
power off. It is 64 KB, divided into 16 sectors of 4 KB; an Atmel chip's
own sectors are 128 bytes, and the game erases thirty-two of them as one
sector of 4 KB. A sector is the smallest piece the game saves at a time
([The Game Pak](src/gamepak/README.md#the-flash-chip)).

Sources: the game: none. SGL: none. GBATEK: "GBA Cart Backup Flash ROM".

### Bank

A bank is one of the fourteen stretches the [main image](#main-image) is cut
into, roughly one for each part: one for the field, one for the windows, one
for the battles, one for sound. Each starts at a fixed place with room to
spare after its code, so one bank can grow without moving anything another
depends on
([The Game Pak](src/gamepak/README.md#why-the-main-image-is-cut-into-banks)).
For readers who know older consoles: every bank is always in place, and none
is switched in or out.

Sources: the game: none. SGL: カラーバンク, a bank of colours. GBATEK: "Wave RAM
Bank", a bank of the wave memory.

### Bank folder

A bank folder is a folder of `src/` that holds one or more
[banks](#bank)' code. The folders are grouped by the four divisions. The
System has `system/`, `gamepak/` and `sound/`. The Field has `field/`. The
Battle has `battle/`, `game/` and `battle_effect/`. The Interface has
`window/`, `menu/` and `shop/` ([Source](src/README.md#banks-and-folders)).

Sources: the game: the credits' "System", "Battle", "Field", "User Interface"
and "Support". SGL: none. GBATEK: none.

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

Sources: the game: none. SGL: none. GBATEK: none.

### Veneer

A veneer is a tiny piece of code, 8 bytes long, that does one thing: it
jumps on to a piece of code somewhere else. It stands at a place that never
changes and points at something that may move, so everyone who needs that
thing can go to the veneer and be sent the right way. `Battle_RunFar` is
one, and all it does is jump to `Battle_Run`. The lists a [bank](#bank)
and an [overlay](#overlay) open with are made of them
([The Game Pak](src/gamepak/README.md#a-veneer)).

Sources: the game: none. SGL: none. GBATEK: none.

### Overlay

An overlay is the code of one scene, or of a few that belong together,
stored packed small on the cartridge and unpacked into memory only when the
party arrives there. Vale's villagers and what happens in Vale are in
Vale's overlay, and are of no use in Vault. There are 96 overlays for the
201 [scenes](#scene), because neighbours share: Crossbone Isle and the
floors of its cave have one between them, and its code looks at which of
those scenes the party is in. They all unpack into the same 32 KB of
memory, so only one is loaded at a time ([System](system.md#overlays)).

Sources: the game: none. SGL: none. GBATEK: none.

### Entry table and import table

An [overlay](#overlay) is a scene's own code, loaded only while the party
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

Sources: the game: none. SGL: none. GBATEK: none.

### Resource directory

The resource directory is the cartridge's one list of every packed thing on
it: each overlay, map and image is a numbered row. The game finds a thing
by its row, so Vale's [overlay](#overlay) is row 883
([The Game Pak](src/gamepak/README.md#the-resource-directory-and-table-order)).

Sources: the game: none. SGL: none. GBATEK: none.

### File and part

A file is one of the three saves the File Screen offers. Each is kept on
the [flash chip](#flash-chip) as two parts, and each part fills one of the
chip's 4 KB sectors. A new copy of a part is written to a free sector
first, and only then is the old copy given up, so there is no moment at which
neither copy is on the chip ([System code](src/system/README.md#saving)).

Sources: the game: "Please choose a file." (きろくばしょ, a place of record). SGL:
none. GBATEK: none.

### Background layer

A background layer is one of the console's four large layers of drawing,
stacked one behind another, each sliding as a whole. A layer is made of
tiles, small square drawings laid side by side like a mosaic. A town is
drawn on three of them: the ground, the walls, and the roofs and treetops.
The World Map uses the two the console can scale and rotate
([Field code](src/field/README.md#towns-and-dungeons-on-screen)).

Sources: the game: none. SGL: スクロール画面, NBG0 to NBG3 and RBG0. GBATEK: "BG",
"BG0-BG3".

### Sprite and OAM

A sprite is a small figure the console can place anywhere on the screen,
over or between the [background layers](#background-layer). It shows one
[picture](#picture) of its [sprite sheet](#sprite-sheet) at a time. The
people are sprites, and so are the pointing hand in a menu and the letters
of a spoken line. GBATEK calls a sprite an OBJ, and the code's names for
the console's sprite fields start `OBJ_`. OAM, the object attribute memory,
is the console's list of where each one sits, and the game writes it afresh
every [frame](#frame)
([System code](src/system/README.md#the-frames-sprites)).

Sources: the game: none. SGL: スプライト, the small figure moved freely over the
scrolls, which `SPRITE` records as a command. GBATEK: "OBJ", "OAM - Object
Attribute Memory".

### Sprite sheet

A sprite sheet is the file of a [sprite](#sprite-and-oam)'s drawings: every
pose Isaac is ever shown in, or every stage of a tornado, one
[picture](#picture) after another. A sprite comes from its sprite sheet. In
the code's names Sheet is short for it (`RESOURCE_TORNADO_SHEET`). Tiles and
letters are kept the same way, on a tile sheet and a font sheet
([Graphics](src/graphics/README.md#sprites-and-their-animations)).

Sources: the game: none. SGL: none. GBATEK: none.

### Sprite animation

A figure on screen is often not one drawing but several laid over one
another, up to four. Each of those layers is a sprite animation: it has its
own run of [pictures](#picture) and steps through them on its own.

In a battle Isaac is three layers: his body, his weapon and his shadow. Body
and weapon change pose together, each from its own run of pictures, while
the shadow stays flat on the ground. On the
field a figure's shadow is not a layer but a fixed part of the figure
([Field code](src/field/README.md#sprites-and-their-animation)).

Sources: the game: none. SGL: none. GBATEK: none.

### Picture

A picture is one drawing on a [sprite sheet](#sprite-sheet), such as one
pose of Isaac's walk. It is the stored drawing; the
[sprite](#sprite-and-oam) is what shows it on screen, or a
[battle effect](#battle-effect)'s blitter copies it onto the canvas the
effect draws on. A [sprite animation](#sprite-animation) shows a picture for
a number of [frames](#frame) before the next one takes its place, and the
animation of a map's tiles steps through pictures the same way
([Field code](src/field/README.md#sprites-and-their-animation)).

Sources: the game: none. SGL: ピクチャー (`PICTURE`), the record of a sprite's
picture data. GBATEK: none.

### Texture

A texture is a drawing that a surface is painted with. As the surface is
drawn, the texture is mapped across it, stretched and turned with it. The
World Map's ground is one: the land is laid on a plane that tilts away from
the camera, and each row of the screen reads the texture at its own scale
([Field code](src/field/README.md#the-world-map-in-perspective)). A
[picture](#picture) is shown by a [sprite](#sprite-and-oam) as it was
drawn; a texture is painted onto the surface it covers and takes its shape.

Sources: the game: none. SGL: テクスチャ (`TEXTURE`), a two-dimensional picture
pasted onto a polygon. GBATEK: none.

### Palette number

A drawing on the cartridge does not hold colours. It holds numbers, and a
palette is the list that says which colour each number stands for. The
console keeps sixteen palettes of 16 colours for the
[background layers](#background-layer) and another sixteen for the
[sprites](#sprite-and-oam). A palette number says which of its set's
sixteen a sprite, or a tile, one of the small squares a background layer is
made of, is painted in.
So one drawing can appear in several colourings: three kinds of enemy share
the Skeleton's drawings, each with a palette of its own
([Graphics](src/graphics/README.md#how-the-console-draws-an-image)).

Sources: the game: none. SGL: パレットナンバー. GBATEK: "Palette Number".

### Palette fade

A palette fade moves every colour on screen a little closer to a target
colour each [frame](#frame), in equal steps, so that they all arrive
together. Vale fading in as the party arrives is one, and so is the tint
that comes over the screen while field Psynergy is cast
([Field code](src/field/README.md#the-palette-fade)).

Sources: the game: none. SGL: none. GBATEK: none.

### Sound

Every sound the game plays is asked for by a number, and the number's range
says what kind it is. Below 80 it is music: Vale's tune is 0. From 80 it is
a jingle, starting with the one for a solved puzzle. From 100 it is a sound
effect, an SE, starting with Vault's bell
([Sound](src/sound/README.md#what-a-sound-number-means)).

Sources: the game: the credits' "Music" and "Sound Effects". SGL: ＢＧＭ and 効果音.
GBATEK: "Sound 1-4" and "DMA Sound A", the console's channels.

## The field

### Field

The field is everywhere the party walks: a town, a dungeon or the World Map.
The word also names the code that runs it, which is everything outside a
battle ([Field](field.md)).

Sources: the game: the credits' "Field"; せんとう&フィールド, in English
"Towns/Dungeons/Battle" (`MsgUsableAnywhere`). SGL: none. GBATEK: none.

### Scene

A scene is one place the game loads as a whole: a town's streets, a floor
of a dungeon, a set of rooms, or the World Map. A few are screens instead,
such as the Title and the File Screen. There are 201, each a row of the
list `gScenes`: Vale is one, and the Sanctum's hall another, which is why
the screen fades between them. Each has a short label Camelot gave it
(`TITLE`, `WORLD`, `PLOGV00` for Vale on the stormy night of the [prologue](#prologue))
([Field](field.md#scenes-and-areas)).

Sources: the game: the debug scene selector's labels, one a scene (TITLE,
PLOGV00). SGL: シーン, a scene file of a modelling program. GBATEK: none.

### Room

A room is the inside of a building: a house, a shop, an inn. Most rooms are
not [scenes](#scene) of their own. Isaac's house, Kraden's study and Vale's
inn are rooms of one scene, on its [rooms map](#rooms-map).

Sources: the game: "What is this room?" (へや); the debug labels HAIROOM1,
KUP_RM01 and FUCHINRM. SGL: none. GBATEK: none.

### Area

An area is a group of [scenes](#scene) that belong together, 71 in all.
Vale's streets, its plaza, its houses and the Sanctum are eight scenes of
one area. An area's scenes share their music and their name, which comes up
on screen only when the party arrives from another area. They also share
one set of short-lived [game flags](#game-flag)
([Field](field.md#scenes-and-areas)).

Sources: the game: the area names; "This area is off-limits". SGL: none.
GBATEK: none.

### Map

A map is the ground a [scene](#scene) is drawn on: a grid of
[cells](#cell), 128 on a side, shown as three
[background layers](#background-layer). Two scenes can share one: Vault
before Kalay and Vault after are two scenes on the same map
([Maps](src/map/README.md)). The World Map is a scene of its own, and the
chart R opens on it is the [map screen](#map-screen). GBATEK's BG Map is
another thing, the grid of tile numbers one background layer shows, its
tilemap.

Sources: the game: the credits' "Maps"; "World Map" (ワールドマップ). SGL: マップ, the
largest unit of a scroll. GBATEK: "BG Map", a layer's tilemap.

### Still image

A still image is a whole screen drawn once and then shown standing still.
The game shows one on the World Map whenever a menu or a conversation opens
there. The land's tilt is drawn in a way that leaves no
[background layer](#background-layer) for [windows](#window) to appear on,
so the game stops, draws the tilted view once onto an ordinary layer, and
shows that still image behind the windows until they close
([Field code](src/field/README.md#the-world-map-in-perspective)).

It is not the [map screen](#map-screen), the map of the whole world that R
opens.

Sources: the game: none. SGL: none. GBATEK: none.

### Map screen

The map screen is the map of the whole world that R opens on the World Map.
It is drawn flat, with a dot on each area the party has visited, and a
cursor that names the area nearest it; A or B closes it. The World Map is a
[scene](#scene), the land the party walks on, drawn tilted away from it.
The map screen is no scene: it is a chart of that land, shown without
leaving the World Map, and nobody walks on it
([Field code](src/field/README.md#the-map-screen)).

Sources: the game: "This place doesn't seem to be on the map..." (ちず). SGL:
none. GBATEK: none.

### Rooms map

A rooms map is the [map](#map) of a [scene](#scene) that holds several
[rooms](#room) side by side, all drawn in one style. A room comes from its
rooms map. The door the party comes in by picks the room. The rooms need not
be in one town: Vale's inn and the room of Babi's scholars in Tolbi are on
the same rooms map, because both are drawn as cottages. So a rooms map is
named for its style, as Cottage rooms or Stucco rooms
([Scenes](src/scene/README.md#design)).

Sources: the game: the credits' "Maps"; the debug labels' ROOM. SGL: none.
GBATEK: none.

### Cell

A cell is one 16-pixel square of a [map](#map). It says which [cell tiles](#cell-tiles) draw it, the
shape and height of the ground, and its [cell behavior](#cell-behavior)
([Maps](src/map/README.md#cells)).

Sources: the game: none. SGL: セル (`CELL`), the 8-pixel tile. GBATEK: none.

### Cell tiles

A cell's tiles are the four tiles of the [tileset](#tileset) that draw one
[cell](#cell) of a map, two by two. A tile is the console's own building
block, a square 8 pixels on a side ([Maps](src/map/README.md#cells)).

Sources: the game: none. SGL: キャラクタパターン, a square of one cell or two by two.
GBATEK: none.

### Cell behavior

A cell behavior is what a [cell](#cell) does when the leader stands on it,
walks into it or searches it. It is what makes one cell a wall, another a
ledge to jump down, and another a jar worth pressing A at
([Maps](src/map/README.md#cells)).

Sources: the game: none. SGL: none. GBATEK: none.

### Actor

An actor is anything on the field that moves or is drawn on its own. The
party and the townspeople are actors, and so are the footprints left in
snow and the point the camera follows. A scene has room for 64, the places
of the list `gActors`
([Field code](src/field/README.md#everything-that-moves-runs-an-actor-script)).

Sources: the game: none. SGL: none. GBATEK: none.

### Actor id

An actor id is an [actor](#actor)'s number in its scene. Numbers 0 to 7 are
kept for the eight characters the code knows (`ACTOR_GARET` is 1), so Jenna
has a number, 5, although she is not one of the four in the party. From 8
on come the people and things the scene places, in the order of its list of
them ([Scenes](src/scene/README.md#who-stands-where)).

Sources: the game: none. SGL: none. GBATEK: none.

### Actor placement

An actor placement is one row of a scene's list of who stands where. It
gives one person or thing its looks, the spot it stands on, the way it
faces, how it moves and the condition for it being there at all. One row of
Vale's puts an old woman at her spot, facing north and standing still
([Scenes](src/scene/README.md#who-stands-where)).

Sources: the game: none. SGL: none. GBATEK: none.

### Actor script

An actor script is the list of small orders an [actor](#actor) follows, a
little each [frame](#frame): move here, wait, jump, go back to the start.
These pages call its items orders, and keep "command" for an
[event command](#event-command). The orders' names start `SC_`, for script
command, the code's own word. A script does not run by itself: one
[task](#task) steps every actor's script each frame, and a script's wait,
`SC_WAIT`, is a count in the actor, not [`wait()`](#wait). A villager who
wanders near home is running one. An order can also hand the actor to a
piece of the field's own code that steers it each frame, and the player's
control of the leader is one of those, `SC_PLAYER`
([Field code](src/field/README.md#how-an-actor-script-runs)).

Sources: the game: none. SGL: none. GBATEK: none.

### Watched actor

The watched actor is the one another [actor](#actor) keeps its eye on. An
[actor script](#actor-script)'s orders to face, follow or walk up to
someone do not say whom. They mean the watched actor, so one script serves
whoever is watching whomever. In the Elemental Star Room of Sol Sanctum,
Jenna, Kraden, Saturos, Menardi and Alex are each given the leader to watch
and then all run the same script
([Field code](src/field/README.md#who-runs-which-script)).

Sources: the game: none. SGL: none. GBATEK: none.

### Field effect

A field effect is a small [actor](#actor) that is only decoration, such as
the sparks of a field Psynergy. It moves, grows and spins by an
[actor script](#actor-script) of its own
([Field code](src/field/README.md#one-actor-for-everything)).

Sources: the game: none. SGL: none. GBATEK: none.

### Trigger

A trigger is one row of a scene's list of things that can happen. The row
says what starts it (talking, searching, stepping somewhere, using
Psynergy), who or where it is for, the condition under which it still
counts, and what it does. When the player does one of those things, the
first row that fits runs. In Vale, one row says that walking north into the
door of Isaac's house runs the [event](#event) at that door
([Scenes](src/scene/README.md#what-a-trigger-row-says)).

Sources: the game: none. SGL: トリガー, a shoulder button. GBATEK: none.

### Step trigger

A step trigger is a [trigger](#trigger) that runs when the leader steps
onto, or walks into, a [cell](#cell) with a given
[cell behavior](#cell-behavior). Walking north into the ladder in Vale
starts the climb onto the roof
([Scenes](src/scene/README.md#which-triggers-a-cell-starts)).

Sources: the game: none. SGL: none. GBATEK: none.

### Examine trigger

An examine trigger is a [trigger](#trigger) that runs when the player
presses A facing a [cell](#cell). Searching a locked door in Vale is one.
Jars, barrels, boxes, graves, books and wells have lines of their own,
the same in every scene
([Scenes](src/scene/README.md#which-triggers-a-cell-starts)).

Sources: the game: "searched the" (しらべる), the game's verb. SGL: none. GBATEK:
none.

### Exit

An exit is a way out of a [scene](#scene): a doorway, a path, the edge of
the map. It is named for where it leads, so the way from Vale's plaza out to
the World Map is `VALE_PLAZA_TO_WORLD_MAP`. A scene's list of exits says
which scene and [entrance](#entrance) each one leads to, and one exit can
have several rows, so that the same path leads somewhere new once the story
has moved on ([Scenes](src/scene/README.md#exits)).

Sources: the game: "the exit" (出口). SGL: none. GBATEK: none.

### Entrance

An entrance is where the party appears in a [scene](#scene), named for
where it comes from: `VALE_PLAZA_FROM_SANCTUM` is the spot in Vale's plaza
for a party arriving from the Sanctum. It gives a spot to stand on, a way to
face and how far the camera may travel
([Scenes](src/scene/README.md#entrances)).

Sources: the game: "Return to the dungeon's entrance." (入口). SGL: none.
GBATEK: none.

### Field request

A field request is a note in the field's [state](#state) that something
needs an answer. There are twelve kinds: an exit taken, a battle due, a
person to talk to, a cell to search, a menu to open, and others. Three
kinds of code set one. The leader's [actor script](#actor-script) sets the
ones for buttons: press Start and its order `SC_PLAYER` sets the request
for the Pause menu. The field sequence's own routine sets the ones for
where the leader has stepped. And an [event](#event) or a menu sets one for
something the field must do after it has returned. The
[field sequence](#field-sequence) reads the requests each time its
`wait(1)` returns, never in the middle of a frame, and answers the most
urgent first ([Field code](src/field/README.md#the-twelve-requests)).

Sources: the game: none. SGL: none. GBATEK: none.

### Event

An event is the piece of code a [trigger](#trigger) runs when something
happens in a scene: a cutscene, or a conversation that does more than show
one line. It is named for its scene. `Vale_EnterIsaacsHouse` is what happens
at the door of Isaac's house, where Dora will not open to a party that has
left Vale and not yet reached Kalay. In the code it sits between
`Event_Start` and `Event_End`, one [event command](#event-command) to a
line ([Scenes](src/scene/README.md#events)).

Sources: the game: none. SGL: イベント (`EVENT`), which names our task. GBATEK:
none.

### Event command

An event command is one step of an [event](#event). The ones named `Event_`
are for speech, pauses and where the party goes next, the ones named
`Actor_` move the people, and the ones named `FieldCamera_` move the camera.
At Dora's door, `Event_ShowMessage(ACTOR_DORA, 0)` shows the event's next
line as Dora's. The 0 asks for nothing special. A command is a C call. It
is not an order of an [actor script](#actor-script), although both can be
about an actor: `Actor_WalkTo` is a command an event calls, and `SC_MOVE`
is an order in a script's table. A command that takes time, such as
showing a line or one whose name ends `AndWait`, calls [`wait()`](#wait)
until what it started is over. One that only starts something, such as
`Actor_WalkTo`, returns at once
([Scenes](src/scene/README.md#events)).

Sources: the game: none. SGL: none. GBATEK: none.

### Destination

The destination is the [scene](#scene) and [entrance](#entrance) the game
goes to next. An [exit](#exit) sets it, and an [event](#event) can set it
itself.

After a battle the party goes back where it was fighting, unless an event
chose somewhere else. After a defeat it goes to the restart point, usually
the last Sanctum it visited, again unless an event chose somewhere else
([System code](src/system/README.md#running-another-sequence)).

Sources: the game: none. SGL: none. GBATEK: none.

### Game flag

A game flag is one of the 4,096 yes-or-no facts the game remembers and
saves, such as a chest being open or Mt. Aleph having erupted. Two ranges of
256 are short-lived instead. One is wiped each time the party enters a
scene, which is why a log rolled into the river in Kolima Forest is back on
the bank when the party walks in again. The other is wiped on entering a
new [area](#area), which is why Vale's pillar stays pushed while the party
goes in and out of Vale's houses. A battle wipes neither
([Game rules](src/game/README.md#the-game-flags)).

Sources: the game: none. SGL: フラグ, any yes-or-no value. GBATEK: none.

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

Sources: the game: "Rough menu" (がさつメニュー), "Recovery test", "Restart quest",
"Menu test" and パラメータセット; the debug labels TEST0 to TEST7. SGL: none. GBATEK:
none.

## The battle

### Battle

A battle is one fight between the party and a
[formation](#formation) of enemies, from its opening to the experience and
coins at the end. It is a [sequence](#sequence) of its own, so the scene the
party was walking in is cleared away while it runs and set up again after
([Battle](battle.md)).

Sources: the game: the credits' "Battle"; "Battle" (バトル, せんとう). SGL: none.
GBATEK: none.

### Unit

A unit is one fighter in a battle, on either side: Isaac is a unit, and so
is each enemy. The game keeps the same set of facts for every one of
them, its stats, Psynergy, items, Djinn and ailments, which the code calls
`struct Unit`. A unit in the party is a party member
([Game rules](src/game/README.md#the-unit)).

Sources: the game: none. SGL: none. GBATEK: none.

### Formation

A formation is one group of enemies a battle is fought against. The game
has 380 of them in a list. One is a lone Vermin, and another is a Vermin
with a Bat
([Battles](src/battle/README.md#the-enemies-of-a-formation)).

Sources: the game: none. SGL: none. GBATEK: none.

### Turns and actions

A turn is one pass through a battle's fighters, in which every
[unit](#unit) acts once. An
action is one unit's go within it: Isaac's attack is one action, and the
Vermin's answer is another ([Battles](src/battle/README.md#turns)).

Sources: the game: "HP recovery / turn" (かいふくHP/ターン); 行動 for one unit's go.
SGL: none. GBATEK: none.

### Staging

Staging is how an [action](#turns-and-actions) is shown, as apart from what
it does: who steps forward, what line announces it and which
[battle effect](#battle-effect) plays
([Battles](src/battle/README.md#staging-and-announcement)).

Sources: the game: none. SGL: none. GBATEK: none.

### Playback queue

The game works out what an [action](#turns-and-actions) does before it
shows any of it. The playback queue is the list of what is left to show for
each target: the flinch, the line such as `Isaac takes 27 damage!`, the HP
going down. A [task](#task) plays it, a little each frame, once the
[battle effect](#battle-effect) has called `BattlePlayback_Start` to say
the blow has landed
([Battles](src/battle/README.md#how-outcomes-reach-the-screen)).

Sources: the game: none. SGL: none. GBATEK: none.

### Battle effect

A battle effect is the show an ability puts on over a battle, such as
Quake's shaking ground or a summon's arrival. Each is a piece of code that
moves and draws its parts [frame](#frame) by frame: a loop that draws one
frame and calls [`wait(1)`](#wait). It decides when a hit is shown, never
what the hit does
([Battle effects](src/battle_effect/README.md)).

Sources: the game: the credits' "Battle Effects". SGL: none. GBATEK: none.

### Mote

A mote is one of the small specks an effect scatters by the dozen: the
specks of light that tumble in on each target of Wish, or the ring of them
the summon Thor draws. Each has its own place, speed and life, and the
effect moves every one of them each [frame](#frame). A
[battle effect](#battle-effect)'s mote is not a [sprite](#sprite-and-oam):
the effect's blitter copies its [picture](#picture) onto the canvas the
effect draws on, most often a round glowing dot at one of ten sizes, often
a smaller one the farther the mote is
([Battle effects](src/battle_effect/README.md#pictures-and-particles)). On
the field a mote is a sprite, such as the motes that circle down onto the
leader as a Venus Djinni joins.

Sources: the game: none. SGL: none. GBATEK: none.

### Battle background

A battle background is the image behind a battle. It is chosen by where
the fight happens, such as the grassland, Goma Cave or a ship's deck
([Graphics](src/graphics/README.md#battle-backgrounds)).

Sources: the game: the credits' "Battle Backgrounds". SGL: none. GBATEK: none.

### Battle Arena

The Battle Arena is the hall where two players fight each other's parties,
their consoles joined by a link cable, or fight the monsters. It is reached
from the File Screen by choosing Battle with a save that has a Djinni, and
the game greets the party with `Welcome to Battle Arena,`. It is a
[scene](#scene) with an [overlay](#overlay) of its own, and most of its
code is the link: the two consoles trade their parties before the battle
([Scenes](src/scene/README.md#the-title-the-file-screen-and-the-battle-arena)).

Sources: the game: "Welcome to Battle Arena" (バトルステージ), "Linked Battles and
Monster Battles"; the debug label ROUNGE. SGL: none. GBATEK: none.

## The interface

### Interface

The interface is everything the player reads and chooses from: the
[windows](#window), the [messages](#message) typed into them, the menus, and
the counters of the shops, sanctums and inns ([Interface](interface.md)).

Sources: the game: the credits' "User Interface". SGL: none. GBATEK: none.

### Window

A window is a bordered box the game opens for text or choices. Talk to a
villager and one grows out of its own centre, then shrinks back into it when
the line is done ([Windows](src/window/README.md#windows)). The console's
own display windows, which show or hide layers over parts of the screen, are
another thing, which the code names `WIN0` and `WININ`.

Sources: the game: "Window color" (ウインドウカラー). SGL: ウインドウ, a display window.
GBATEK: "Window 0", "OBJ Window", the display windows.

### Border

The border is the edge drawn around a [window](#window). It is drawn in the
window colour the player chose in the settings, as the window's filling is
([Windows](src/window/README.md#windows)).

Sources: the game: none. SGL: none. GBATEK: none.

### Message

A message is a piece of text the game types into a [window](#window) a few
letters at a time, stopping for a button where a page ends. A villager's
line is one, and so is a battle's `Isaac takes 27 damage!`. Up to three can
be typing at once ([Windows](src/window/README.md#typing)).

Sources: the game: "Message Speed" (メッセージスピード). SGL: none. GBATEK: none.

### The screens

The screens are the Title, the File Screen, the Pause menu, the field menu
with its Psynergy, Djinn, Item and Status screens, the settings menu, name
entry and the [map screen](#map-screen). The File Screen is where a
[file](#file-and-part) is chosen, copied or erased, and the Pause menu is
what Start opens on the field ([Interface](interface.md)).

Sources: the game: "Title screen" (タイトルがめん), "To the File Screen" (データロードがめん,
the data load screen), "The Pause menu is not available now." (ポーズメニュー); the
debug label TITLE. SGL: none. GBATEK: "Screen Base Block", a layer's map.

### Password

A password carries a cleared game's party over to The Lost Age, for a
player without a second Game Boy Advance and a link cable. The File Screen
offers three sizes, Bronze, Silver and Gold, and the more it carries the
longer it is: Gold adds the whole inventory and the coins. The player
copies it down by hand. A Japanese password is hiragana; the others leave
out the characters that look alike, I, l and 1, and O, o and 0
([Scenes](src/scene/README.md#passwords)).

Sources: the game: "Password" (パスワード), "Password Selection". SGL: none.
GBATEK: none.

### Menu

The menu is the field menu, the row of icons that A opens on the field, and
its Psynergy, Djinn, Item and Status screens
([Menus](src/menu/README.md#the-screens)). The folder `menu/` also holds the
staff credits and the title intro.

Sources: the game: "Rough menu" (がさつメニュー), "Menu test"; the debug label MENU.
SGL: none. GBATEK: none.

### Shop

A shop is any counter the party does business at: a shop's, a sanctum's or
an inn's. Bilibin's innkeeper asking 7 coins a guest is the shop code at
work ([Shops](src/shop/README.md)).

Sources: the game: "Thank you for shopping", "weapon shop" (武器屋, やど屋: a shop
by its trade). SGL: none. GBATEK: none.

## The folders

The folders of `src/` that are not named for a bank of code.

### Game rules

The game rules are what a [unit](#unit), an item or a Djinni is and does:
how much PP Cure costs, what Flint adds to a character. They are kept in
one place because the field, the battles and the menus all need the same
answers ([Game rules](src/game/README.md)).

Sources: the game: the credits' "Game Design" and "Game Planning". SGL: none.
GBATEK: none.

### Graphics

Graphics is the game's art as files anyone can open and edit. Each
folder is named for what it holds or the place it belongs to
([Graphics](src/graphics/README.md)).

Sources: the game: the credits' "Graphic Arts". SGL: none. GBATEK: none.

### Graphics folders

For readers of the files: the folders of images under `src/graphics/`,
and what is in each.

- `battle_backgrounds`: the [battle backgrounds](#battle-background).
- `battle_effects`: the [battle effects](#battle-effect)' drawings and
  colours.
- `field_effects`: the sparks field Psynergy draws, its
  [field effects](#field-effect).
- `font`: the letters, how wide each one is, and the tables that match
  text to letters.
- `monster`: the enemies' battle sprites.
- `palettes`: the windows' colours and the colours most sprites share.
- `palette_editor`: the one image of a colour editor, a tool the
  [debug menus](#debug-menus) reach.
- `sprite`: the drawings of every person and thing that moves, and their
  animations, as [sprites](#sprite-and-oam).
- `tileset`: the tiles and colours a map's
  [cell tiles](#cell-tiles) come from.
- `ui`: every [Interface](#interface) image, such as the menus' icons.

Sources: the game: the credits' "Battle Backgrounds", "Battle Effects",
"Monster Design", "Map Characters", "Battle Characters", "Maps" and "User
Interface"; "Monsters won't attack". SGL: none. GBATEK: none.

### Places and minigames

The graphics folders named for a place or a thing in the game:

- `colosso`, the Colosso's lettering, counters and gauge.
- `lamakan_desert`, the desert's heat gauge.
- `sol_sanctum_statue`, the images for Sol Sanctum's statue room.
- `lucky_dice` and `lucky_wheels`, the two minigames' images.
- `world_map`, the World Map's own images.
- `title`, the Title's images and its prompt.
- `credits`, the staff roll's font.

Sources: the game: "Colosso", "Lucky Dice" (ギャンブルダイス), "Lucky Wheels"
(ふくびきスロット), "World Map", the area names Lamakan Desert and Sol Sanctum; the
debug label TITLE; the credits' "GOLDEN SUN STAFF CREDITS". SGL: none. GBATEK:
none.

### Tileset

A tileset is the set of tiles, with their colours, that a map's
[cell tiles](#cell-tiles) come from. A tile is a square 8
pixels on a side, the console's own building block
([Graphics](src/graphics/README.md#tilesets)).

Sources: the game: none. SGL: none. GBATEK: none.

### Monster

A monster is an enemy as the player sees it, its sprite in a battle
([Graphics](src/graphics/README.md#monsters)). In the code an enemy is a
[unit](#unit) like any other.

Sources: the game: the credits' "Monster Design"; "Monsters won't attack so
often now." (モンスター). SGL: none. GBATEK: none.
