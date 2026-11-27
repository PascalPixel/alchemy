# Glossary

These are the words the pages about The Lost Age share. Each entry says
what the word means and links to the page that explains it.

A word is the one a modern engineer who knows the Game Boy Advance, or a
player who knows the game in English, follows without a gloss. An entry's
Sources line says what the game itself, Sega's SGL manual and GBATEK each
call the thing, or none where one has no word for it.

## Golden Sun

### The two games

Golden Sun is a role-playing game for the Game Boy Advance, told in two
parts. The first is The Broken Seal (2001), whose builds are named `tbs`.
Its sequel is The Lost Age (2002), which these pages are about and whose
builds are named `tla`.

Sources: the game: "Transfer data from Golden Sun?"; the Japanese text's
黄金の太陽~開かれし封印~ and 失われし時代; the credits' "GOLDEN SUN: THE LOST AGE". SGL: none.
GBATEK: none.

### Camelot

Camelot Software Planning made both games. Each opens on its publisher's
logo and then Camelot's. Their source was never
released.

Sources: the game: the credits' "CAMELOT". SGL: none. GBATEK: none.

### The party

The party is Felix and whoever is travelling with him. A new game starts
with Jenna and Kraden at Venus Lighthouse, and Felix and Sheba join them
on Idejima. Piers joins in Kibombo, and later Isaac, Garet, Ivan and Mia, so
the party grows to eight, who share one purse of coins.
In a battle the first four fight and the rest wait in reserve, ready to
step in when the front line falls. Each of them is a [unit](#unit)
([Game rules](../../games/tla/src/game/README.md#the-party-and-its-coins)).

Sources: the game: "joined your party" (なかま, and once パーティ). SGL: none.
GBATEK: none.

### The places, in story order

The story starts at Venus Lighthouse and on Idejima, and follows Felix
through Daila, the Kandorean Temple, the Dehkan Plateau, Madra, Alhafra,
Garoh and Kibombo, out to sea and on to Lemuria, then to Jupiter
Lighthouse, Magma Rock and Prox, and up Mars Lighthouse, where the game
ends. Pages list places in that order.

Sources: the game: the area names, such as "Jupiter Lighthouse". SGL: none.
GBATEK: none.

### Prologue

The prologue is the part of a new game played before the title comes up
and the game with Felix begins. After the story crawl, headed `Prologue
from Book One`, has told what happened in The Broken Seal, Jenna and
Kraden come out of Venus Lighthouse, Alex sends them west, and they cross
the Suhalla Gate on their way to Idejima. The title then plays its intro once more
(`TITLE_FROM_IDEJIMA`) ([Scenes](../../games/tla/src/scene/README.md)).

Sources: the game: "Prologue from Book One" (ぜんさくのあらすじ, the previous game's
synopsis). SGL: none. GBATEK: none.

### Psynergy and PP

![Jenna casts Fire at Felix, Piers and Sheba: fireballs rise, hang and stream into the three](https://alchemy.pascalpixel.com/tla/abilities/fire.gif)

Psynergy is the game's magic. Some is cast in battle, such as Quake, Cure
and Growth, and some on the field, such as Move, Lash and Reveal
([Field code](../../games/tla/src/field/README.md#how-field-psynergy-reaches-its-target)).

Each use costs PP, Psynergy Points: Quake costs 4, Cure 3 and Move 2. PP
comes back as the party walks, and a night at an inn restores it.

Sources: the game: "Psynergy", "Psynergy Points" (エナジー, and EP for PP). SGL:
none. GBATEK: none.

### The four elements

The four elements are Venus (earth), Mercury (water), Mars (fire) and Jupiter
(wind), and the game always lists them in that order. Every character,
Djinni and summon belongs to one. Felix is Venus, Jenna Mars, Sheba Jupiter
and Piers Mercury ([Game rules](../../games/tla/src/game/README.md#element-levels)).

Sources: the game: "Venus", "Mercury", "Mars" and "Jupiter" (ぞくせい for an
element). SGL: none. GBATEK: none.

### Power and Resist

For each of the [four elements](#the-four-elements), every fighter has a
Power and a Resist, which the Status screen shows. Power raises the damage
and healing of what the fighter casts in that element. Resist lowers the
damage it takes from it
([Battles](../../games/tla/src/battle/README.md#the-element-multiplier)).

Sources: the game: the Status screen's "Power" and "Resist" (パワー, たいせい). SGL:
none. GBATEK: none.

### Djinn

![Felix unleashes the Venus Djinni Echo in battle: he leaps at Sheba, his sword strikes in bursts of light, and she takes the damage](https://alchemy.pascalpixel.com/tla/abilities/echo.gif)

Djinn are elemental creatures the party collects. One is a Djinni, several
are Djinn. Set on a character, a Djinni raises that character's stats and
helps decide its class: Echo, a Venus Djinni, gives 9 HP, 4 PP and 3
Attack. Unleashed in battle, it does its own move and goes on standby, ready
for a [summon](#summons) ([Game rules](../../games/tla/src/game/README.md#djinn-and-classes)).

Sources: the game: "Djinn", "Djinni" (ジン). SGL: none. GBATEK: none.

### Summons

![Felix summons Zagan: the screen flashes white, a beast wielding an axe charges across in streaks of fire and strikes Jenna, Sheba and Piers, lowering their Defense](https://alchemy.pascalpixel.com/tla/abilities/zagan.gif)

A summon spends [Djinn](#djinn) on standby to call up an elemental being.
The summon named Venus takes one Venus Djinni, Ramses two, Cybele three and
Judgment four. The Lost Age adds summons that take Djinn of more than one
element: Zagan takes one Venus and one Mars. The Djinn a summon spent then
need a few [turns](#turns-and-actions) to recover
([Battles](../../games/tla/src/battle/README.md#commands-and-ability-ids)).

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
([Welcome](../../games/README.md)).

Sources: the game: none. SGL: none. GBATEK: none.

### The original

The original is the game as [Camelot](#camelot) shipped it: the cartridge of
each [edition](#editions). The source reproduces each one byte for byte,
bugs included.

Sources: the game: none. SGL: none. GBATEK: none.

### Editions

Each game was released in Japanese, English, German, Spanish, French and
Italian, and each release is an edition (`tla-ja` to `tla-it`). The five
other than the Japanese are the international editions. They differ in more
than their text: when the cursor on the [map screen](#map-screen) reaches a
new marker, the Japanese edition draws its name at once, and the others six
[ticks](#tick) later.

Sources: the game: none. SGL: none. GBATEK: none.

### The style and where it comes from

For readers of the code: the code follows the naming and layout of SGL,
Sega's library for the Saturn, which the [system library](#system-library)
is modelled on. A function's name is two small letters for the part that
owns it and then its words, as `sySynch` or `fdActorGetById`. A type is in
capitals, as `ACTOR`, and a global variable is its name alone, as
`HeapSlots`. Each level is one tab, and comments are written with `//`
([Contributing](../../CONTRIBUTING.md#names)).

This is the usual manner of Japanese studio code around 2000. Names are
short English words, and the common ones are cut down: tbl for a table, wk
for work, chr for a character, pat for a pattern, anm for an animation, flg
for flags. Comments were written in Japanese after the code, in block
comments.

No original name of any function in these games is known. The names here
are ours, written in that manner. Only the game's own words are confirmed
against the game: the Japanese text's enajii (エナジー, Psynergy), jin (ジン,
Djinn), setto (セット, Set), sutanbai (スタンバイ, Standby) and rikabarii
(リカバリー, Recovery). These words of the period help in reading such code:

| Word | What it means |
| --- | --- |
| mode | one kind of play the main routine runs ([mode](#mode)) |
| sequence | a piece of music the sound driver plays |
| event | a function run once a tick from a list ([event](#event)) |
| trigger | a button of the pad, which is why a scene's signal is a [cue](#cue) |
| work | a function's memory |
| transfer | copying to video memory |
| dummy | a placeholder |
| pattern | one drawing of a sprite |
| character | an 8 by 8 tile |
| cell | an 8 by 8 square, our tile, which is why a map's square is a [metatile](#metatile) |
| backup | the save memory ([flash chip](#flash-chip)) |
| encount | an encounter |
| wait | ticks to hold ([wait](#wait)) |

Sources: the game: エナジー, ジン, セット, スタンバイ and リカバリー. SGL: the
names and layout of its reference and its sample code. GBATEK: none.

### BUG, BUGFIX and UBFIX

A `/* BUG: */` comment marks one of the original game's bugs, kept as it
is. In Shaman Village, the code that loads a sprite sheet of lettering asks
for a block of memory with room for seven of its eight pictures, so
unpacking it writes 1,024 bytes past the end of the block. The source keeps
the short block, with one of the right size beside it behind `BUGFIX`.

A `/* UB: */` comment is for readers of the code. The game is written in a
language called C, and C leaves a few things open. In the code that draws the battle
backgrounds, one number is read before anything has given it a value, and C
does not say what it holds then. On the cartridge it came out one
particular way, and the source keeps that. The comment marks the spot, and
a version that leaves nothing open sits beside it behind `UBFIX`.

Sources: the game: none. SGL: none. GBATEK: none.

### Unused

Some of what is on the cartridge never happens in the game. A comment
opening with `Unused:` marks such a thing and gives the reason. One is a
little function in the battle code that runs random battles one after
another, forever: nothing in the game refers to it. `UNUSED` in the code
marks that plainest case, a named piece of code that nothing else in the
game ever asks for.

What only the [debug menus](#debug-menus) reach is not unused.

Sources: the game: none. SGL: none. GBATEK: none.

### Module header

For readers of the code: the source is many files, and a file can only use
what another file has announced. A module header is the short file where one
file of code makes its announcements. The system library's header is where
the rest of the game learns the numbers of the [modes](#mode), such
as `MODE_BATTLE`. A header sits at the same path under `include/` as its
code has under `src/`: `src/battle/battle.c` has `include/battle/battle.h`.
The system library is announced in one header for all of it,
`include/system.h`.

Sources: the game: none. SGL: ヘッダファイル, its `sgl.h`. GBATEK: none.

### Object

For readers of the code: the game is not built in one go. Each file of the
source is turned into bytes separately, and the result is called an object.
The objects are then laid end to end to make the cartridge. The word has
nothing to do with the things on the field that the game's text calls
objects, which are [actors](#actor), nor with GBATEK's OBJ, which is a
[sprite](#sprite-and-oam).

A very long file is written as several smaller parts beside it, each named
for the file and the part, and still makes one object. The shops are one:
`src/shop/shop.c` gathers the inn, buying, selling and the forge from
`src/shop/shop_*.c`.

Sources: the game: none. SGL: オブジェクト (`OBJECT`), a placed model. GBATEK:
"OBJ", a sprite.

## The system library

### System library

The system library is the code everything else stands on: starting up,
deciding what runs next, keeping time, handing out memory, saving and
sound. The field engine, the battle engine and the menu engine all use it,
and none of them could run without it. It is one of the code's four parts,
each the work of its own team
([The system library](system.md)).

Sources: the game: the credits' "Programming", then "System". SGL: none.
GBATEK: none.

### Main routine

The main routine, `gmMainRoutineRun`, starts when the console is switched on
and never finishes. All it does is call one [mode](#mode) as a
function, clear away what that one set up when it returns, and call the
next. Walk into a battle, win it and walk on, and the main routine has run
three: the cave, the battle, the cave again. It is a loop, but one trip
round it is a whole mode: the loop that goes round once a
[tick](#tick) is inside [`sySynch()`](#wait)
([The system library](system.md#the-main-routine)).

Sources: the game: none. SGL: メインループ, the loop run once a tick. GBATEK: none.

### Mode

A mode is one whole kind of play, run from its start to its end. There
are five: the field mode (walking around a [scene](#scene)), the battle
mode (one fight), the dice mode (Lucky Dice), the slots mode
(Lucky Wheels) and a fifth that runs [Super Lucky Dice](#super-lucky-dice).
The code names four of them `MODE_BATTLE`, `MODE_DICE`,
`MODE_SLOTS` and `MODE_SUPER_DICE`. The field mode has no such
name: it is asked for by naming the scene to walk around. The Title and the
File Screen are scenes, so the field mode runs them too, and a menu or
a shop is a function called from inside a mode. Only one mode runs
at a time
([The system library](system.md#the-main-routine)). A sequence is another
thing, a piece of music the sound driver plays, as in SGL.

Sources: the game: none. SGL: `MainMode`, the main sequence mode
(メインシーケンスモード), and シーケンス, a piece of music. GBATEK: none.

### Super Lucky Dice

Super Lucky Dice is the third game of chance, beside Lucky Dice and Lucky
Wheels, and Contigo has all three. It is a [mode](#mode) of its own,
run by `fdSuperLuckyDiceRun`, with a [module](#module) of its own that The Broken
Seal does not have. Like the other two, it keeps the last two background
palettes safe while it runs and puts them back after
([Field code](../../games/tla/src/field/README.md#lucky-dice-lucky-wheels-and-super-lucky-dice)).

Sources: the game: "Super Lucky Dice" (マッチングダイス). SGL: none. GBATEK: none.

### Field mode

The field mode runs one [scene](#scene), from the party's arrival to the
[exit](#exit) it leaves by. In Daila it runs from the moment Daila fades in
until Felix steps through a door. On the way it answers whatever the leader
runs into, the [field requests](#field-request)
([Field code](../../games/tla/src/field/README.md#the-field-mode-serves-requests)).

Sources: the game: the credits' "Field". SGL: none. GBATEK: none.

### Transition and next mode

A transition is the handover from one [mode](#mode) to the next.
Stepping through a door is one, and so is going into a battle. The mode
that is ending writes down what should run next, a scene or another
mode, in the [main routine's state](#main-routines-state), and the
[main routine](#main-routine) starts it. The word is kept for this
handover. The wipe or fade the player sees while it happens is a screen
transition, `fdScreenTransition` in the code, which is only the drawing
([System library code](../../games/tla/src/system/README.md#the-main-routine)).

Sources: the game: none. SGL: none. GBATEK: none.

### Main routine's state

`MainRoutineStateWork` is the block of memory that outlives every
[mode](#mode): one C struct in a global. What a battle or a scene
set up for itself is cleared away when it ends, but this block is left
alone, so the next mode can read it. It holds what runs next, who is in
the party and their coins, which is why a battle's winnings are still there
back on the field. It is set to its defaults when the console is switched
on, and a new game fills it in. Three other things are left alone with it,
the [game flags](#game-flag), the characters' [units](#unit) and the
snapshot of where a scene's [actors](#actor) stand, and a save file keeps
all four
([The system library](system.md#saving)).

Sources: the game: none. SGL: none. GBATEK: none.

### Tick

A tick is one step of the game's own work: one pass of its loop, in which
every [event](#event) runs once and the code that waits goes on by one.
Everything that takes time is counted in ticks. When Jenna wakes on Idejima
and says `What...`, her script then calls [`sySynch()`](#wait) for 40 ticks
before she goes on: two thirds of a second, when nothing slows the game down
and each tick takes one [frame](#frame)
([System library code](../../games/tla/src/system/README.md#the-tick-loop)).

Sources: the game: none. SGL: none. GBATEK: none.

### Frame

A frame is one drawing of the screen by the console, about a sixtieth of a
second, and it is the unit the code counts in when it counts the console's
time rather than the game's: one [VBlank](#vblank) ends each frame.
`FrameCount` counts every frame there has been, a screen fade lasts a number
of frames, and the sound keeps its time in frames. A [tick](#tick) usually
takes one frame, and two or more in [slowdown](#slowdown). One drawing of an
animation is not a frame but a [picture](#picture)
([System library code](../../games/tla/src/system/README.md#what-vblank-does)).

Sources: the game: none. SGL: フレーム, one picture shown. GBATEK: "Frame 0-1", a
bitmap buffer.

### Wait

To wait is to let [ticks](#tick) go by in the middle of a piece of code.
`sySynch(10)` runs ten ticks of the game, there inside the call, and then
carries on with the next line: the game's tick loop is this function, and
there is no other. One piece of code waits at a time. An
[actor script](#actor-script)'s `SC_WAIT` is another thing, a count kept in
the actor. When the waves rise in the Sea of Time, its code moves the
colours one shade toward violet, waits ten ticks, and goes round again
until none has changed. So a scene can be written top to bottom, in the
order things happen ([The system library](system.md#waiting-and-events)).

Sources: the game: none. SGL: `wait`, which waits for the next picture
(次のビデオ表示を待つ). GBATEK: none.

### Event

An event is a function on a list that [`sySynch()`](#wait) calls once in every
[tick](#tick), until it is taken off the list. Events run only while some
code is waiting, and an event never waits itself. An event is a whole piece
of work, such as moving every [actor](#actor) a step, not one thing in the
world. The event that types a villager's line into its window letter by
letter is one. Each event has a rank, its `priority`,
and a higher rank runs earlier in the tick. A few events are called by the
[VBlank](#vblank) handler instead, once a VBlank. The list, `EventBuf`, has
room for 24. The record is `EVENT`, and the calls take SGL's names, the verb
first: `sySetEvent`, `syCloseEvent`, `sySuspendEvent`, `syResumeEvent`,
`syFindEvent`
([System library code](../../games/tla/src/system/README.md#the-event-runner)).
What happens at a door or in a cutscene is a [script](#script), not an event.

Sources: the game: none. SGL: イベント (`EVENT`) with its 実行関数 is this thing,
and `EventBuf` is its list; its タスク is the second processor's. GBATEK: none.

### VBlank

VBlank is the short gap after the console has drawn the screen and before it
starts drawing it again, about 60 times a second. At each one the console
breaks off whatever the game is doing and runs one small piece of its code.
That piece puts what the last [tick](#tick) prepared on screen, prepares the
sound and reads the buttons
([System library code](../../games/tla/src/system/README.md#what-vblank-does)).

Sources: the game: none. SGL: ブランキング. GBATEK: "V-Blank", "VBlank".

### Slowdown

The game has one [frame](#frame), about a sixtieth of a second, to do each
[tick](#tick)'s work before the console starts drawing the screen again.
Slowdown is what happens when a tick's work is not done in time: the last
tick's image stays on screen while the console draws it twice or more, and
the tick takes two frames or more. Everything counted in
ticks, every walk, pause and animation, drags together. The music does
not, because the sound is prepared at every [VBlank](#vblank) whether the
tick is done or not. So the tune keeps its speed while the image on
screen falls behind it
([System library code](../../games/tla/src/system/README.md#what-vblank-does)).

Sources: the game: none. SGL: 処理落ち. GBATEK: none.

### Fixed-point number

The console has no decimals, so a position, a speed or a scale is a whole
number counted in 65,536ths: its upper 16 bits are the integer part and its
lower 16 bits the decimal part. `toFIXED(0.8)` writes one, `toFIXED(1)` is
1.0, and `FIXEDtoINT()` gives back the integer part, the pixel a position is
on. In the code its type is `FIXED`. A function that takes such numbers
where its plain form takes whole pixels ends in `FX`
(`fdActorSetPosFX`), and angles are counted the same way, 65,536 to the
turn, written `DEGtoANG(90)`. `FIXED`, `toFIXED`, `DEGtoANG` and the `FX`
suffix are the words of the reference for SGL, Sega’s engine for the Saturn,
whose fixed-point, angle and matrix design this code matches; the current
matrix's `syPushMatrix`, `syRotX` and `syTranslate` take the names of its
`slPushMatrix`, `slRotX` and `slTranslate`.

Sources: the game: none. SGL: 固定小数点 (`FIXED`), with `toFIXED`, `DEGtoANG` and
the FX suffix. GBATEK: "Integer portion" and "Fractional portion".

### State

A system's state is the block of memory it keeps its running values in. It
is set up when the system starts and kept for as long as the system runs:
one C struct, `struct <Owner>State`, in a global named `g<Owner>State`
where the code has named it. The menus' holds their windows, which question
they are at and where the hand last was for each party member, so that
going back to Jenna's items lands where the player left them. The battle
has one too, and each is found again through a [heap slot](#heap-slot).
The [main routine's](#main-routines-state) is the one that is kept apart
and outlives the rest
([The system library](system.md#saving)).

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
[mode](#mode), so a battle starts with nothing left over from the
cave it began in
([System library code](../../games/tla/src/system/README.md#the-heap)).

Sources: the game: none. SGL: none. GBATEK: none.

### Heap slot

A heap slot is one of 62 numbered places, each remembering where one block
from the [heap](#heap-and-pools) is, so that other code can find the block
again. The battle's [state](#state) is found through one, and the menus'
state through another
([System library code](../../games/tla/src/system/README.md#the-heap)).

Sources: the game: none. SGL: none. GBATEK: none.

### IWRAM and EWRAM

IWRAM and EWRAM are the console's two areas of work RAM, RAM being memory,
as Game Boy Advance programmers name them; GBATEK calls them the on-chip
and the on-board work RAM. IWRAM is 32 KB inside
the processor, the chip that follows the game's instructions, and it is the
fastest memory there is. EWRAM is 256 KB beside the processor, and slower.
So the code that must be quickest is copied into IWRAM, and the big things
go in EWRAM: the scene the party is in unpacks its [overlay](#overlay) into
32 KB of it
([System library code](../../games/tla/src/system/README.md#memory-map)).

Sources: the game: none. SGL: none. GBATEK: "On-chip Work RAM" and "On-board
Work RAM".

### Main image

The main image is the code the processor runs straight from the cartridge,
without copying it into memory first. It is cut into fifteen
[modules](#module), and holds what is needed all the time: walking, battles,
windows, menus, shops, sound. What only one scene needs is kept apart, in
[overlays](#overlay) ([The system library](system.md#overlays)).

Sources: the game: none. SGL: none. GBATEK: none.

### Game Pak

The Game Pak is the cartridge as a piece of hardware. It holds the game
itself, which cannot be changed: the code in its [modules](#module), and every
image, map and tune, listed in the
[resource directory](#resource-directory). It also holds the
[flash chip](#flash-chip) the saves are written to
([The Game Pak](../../games/tla/src/system/README.md)).

Sources: the game: none. SGL: none. GBATEK: "Game Pak", "GamePak ROM".

### Flash chip

The flash chip is the small chip on the cartridge the three save
[files](#file-and-copy) are written to. It keeps what is written with the
power off. It is 64 KB, divided into 16 sectors of 4 KB; an Atmel chip's
own sectors are 128 bytes, and the game erases thirty-two of them as one
sector of 4 KB. A sector is the smallest piece the game saves at a time
([The Game Pak](../../games/tla/src/system/README.md#the-flash-chip)).
The system library's layer over the chip is named as SGL's backup library:
its calls are `syBupReadPart`, `syBupWritePart` and the others, its layout
`BUP_SECTOR_COUNT` and the rest. Save stays the player's word, for the
File Screen and the save menu.

Sources: the game: none. SGL: バックアップライブラリ, the backup library, `BUP_`.
GBATEK: "GBA Cart Backup Flash ROM".

### Module

A module is one of the fifteen stretches the [main image](#main-image) is
cut into, roughly one for each part: one for the field, one for the windows,
one for the battles, one for sound. The Lost Age has one more than The
Broken Seal, for [Super Lucky Dice](#super-lucky-dice). Each starts at a
fixed place with room to spare after its code, so one module can grow
without moving anything another depends on
([The Game Pak](../../games/tla/src/system/README.md#why-the-main-image-is-cut-into-modules)).
For readers who know older consoles: every module is always in place, and
none is switched in or out as a bank would be.

Sources: the game: none. SGL: none. GBATEK: none.

### Engine folder

An engine folder is a folder of `src/` that holds one or more
[modules](#module)' code. The folders are grouped by the four teams. The
system library has `system/` and `sound/`. The field engine has `field/`.
The battle engine has `battle/` and `battle_effect/`, and the game logic in
`game/` is grouped with them. The menu engine has `window/`, `menu/` and
`shop/`.

Sources: the game: the credits' "System", "Battle", "Field" and "User
Interface". SGL: none. GBATEK: none.

### Stub call

The code is cut into [modules](#module), and the code in one often needs
something in another. A stub call is how it asks. The trouble is that things
move about inside a module whenever its code changes, so the asker cannot
know exactly where to go.

So each module opens with a list, at a place that never moves, with one entry
for each thing other modules may ask of it. The asker goes to the entry, and
the entry, a [stub](#stub), sends it on. An entry is named after what it
leads to with `Far` added: the main routine starts a battle through
`btRunStub`, which leads to `btRun`
([The Game Pak](../../games/tla/src/system/README.md#why-the-main-image-is-cut-into-modules)).

Sources: the game: none. SGL: none. GBATEK: none.

### Stub

A stub is a tiny piece of code, 8 bytes long, that does one thing: it
jumps on to a piece of code somewhere else. It stands at a place that never
changes and points at something that may move, so everyone who needs that
thing can go to the stub and be sent the right way.
`btRunStub` is one, and all it does is jump to
`btRun`. The lists a [module](#module) and an
[overlay](#overlay) open with are made of them
([The Game Pak](../../games/tla/src/system/README.md#a-stub)).

Sources: the game: none. SGL: none. GBATEK: none.

### Overlay

An overlay is the code of one scene, or of a few that belong together,
stored packed small on the cartridge and unpacked into memory only when the
party arrives there. Daila's townspeople and what happens in Daila are in
Daila's overlay, and are of no use in Madra. There are 114 overlays for the
325 [scenes](#scene), because neighbours share: the eleven floors of the
Shrine of the Sea God have one between them. Such an overlay's code looks
at which of its scenes the party is in, as the Suhalla Gate's does for its
three. They all unpack into the same 32 KB of memory, so only one is loaded
at a time ([The system library](system.md#overlays)).

Sources: the game: none. SGL: none. GBATEK: none.

### Entry table and import table

An [overlay](#overlay) is a scene's own code, loaded only while the party
is there. The rest of the game, the [main image](#main-image), cannot know
where things are inside it, so the two reach each other through two lists
of [stubs](#stub).

The entry table is the first thing in the overlay. It has seven stubs, one
for each thing the field asks of any scene, such as who stands where and
which exits it has. The import table is the last thing in it, and works the
other way: one stub for each piece of the main image the overlay uses.
When Felix walks into a door in Alhafra, Alhafra's overlay asks the field
for that exit through its import table
([System library code](../../games/tla/src/system/README.md#how-an-overlay-reaches-the-rest-of-the-game)).

Sources: the game: none. SGL: none. GBATEK: none.

### Resource directory

The resource directory is the cartridge's one list of every packed thing on
it: each overlay, map and image is a numbered row. The game finds a thing
by its row, so Daila's [overlay](#overlay) is row 1617
([The Game Pak](../../games/tla/src/system/README.md#the-resource-directory-and-table-order)).

Sources: the game: none. SGL: none. GBATEK: none.

### File and copy

A file is one of the three saves the File Screen offers. Each is kept on
the [flash chip](#flash-chip) as one copy, and a copy fills three of the
chip's 4 KB sectors. A newly saved copy is written to free sectors first,
and only then is the old copy given up, so there is no moment at which
neither copy is on the chip
([System library code](../../games/tla/src/system/README.md#saving)).

Sources: the game: "Please choose a file." (きろくばしょ, a place of record). SGL:
none. GBATEK: none.

### Background layer

A background layer is one of the console's four large layers of drawing,
stacked one behind another, each sliding as a whole. A layer is made of
tiles, small square drawings laid side by side like a mosaic. A town is
drawn on three of them: the ground, the walls, and the roofs and treetops.
The World Map uses the two the console can scale and rotate
([Field code](../../games/tla/src/field/README.md#towns-and-dungeons-on-screen)).
In names, the whole display is `Screen` (`syScreenFadeToBlack`,
`SCREEN_POINT`), and a background layer's tilemap is its screen
(`mnBgScreenFillRect`).

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
every [tick](#tick)
([System library code](../../games/tla/src/system/README.md#the-ticks-sprites)).

Sources: the game: none. SGL: スプライト, the small figure moved freely over the
scrolls, which `SPRITE` records as a command. GBATEK: "OBJ", "OAM - Object
Attribute Memory".

### Sprite sheet

A sprite sheet is the file of a [sprite](#sprite-and-oam)'s drawings: every
pose Felix is ever shown in, or every stage of a tornado, one
[picture](#picture) after another. A sprite comes from its sprite sheet. In
the code's names Sheet is short for it (`RESOURCE_TORNADO_SHEET`). Tiles and
letters are kept the same way, on a tile sheet and a font sheet
([Graphics](../../games/tla/src/graphics/README.md#sprites-and-their-animations)).

Sources: the game: none. SGL: none. GBATEK: none.

### Sprite animation

A figure on screen is often not one drawing but several laid over one
another, up to four. Each of those layers is a sprite animation: it has its
own run of [pictures](#picture) and steps through them on its own.

In a battle Felix's body and his weapon change pose together, each from its
own run of pictures, while his shadow stays flat on the ground
([Field code](../../games/tla/src/field/README.md#sprites-and-their-animation)).

Sources: the game: none. SGL: none. GBATEK: none.

### Picture

A picture is one drawing on a [sprite sheet](#sprite-sheet), such as one
pose of Felix's walk. It is the stored drawing; the
[sprite](#sprite-and-oam) is what shows it on screen, or a
[battle effect](#battle-effect)'s blitter copies it onto the canvas the
effect draws on. A [sprite animation](#sprite-animation) shows a picture for
a number of [ticks](#tick) before the next one takes its place, and the
animation of a map's tiles steps through pictures the same way
([Field code](../../games/tla/src/field/README.md#sprites-and-their-animation)).

Sources: the game: none. SGL: ピクチャー (`PICTURE`), the record of a sprite's
picture data. GBATEK: none.

### Texture

A texture is the drawing a model's faces, or rows of the
[battle effects](#battle-effect)' canvas, are painted with. A model is a
shape of flat faces a battle effect turns in three dimensions, such as the
rings, columns and whirlpools of the summons. As the model is drawn, the
texture is mapped across each face, stretched and turned with it, and it
can scroll across it, so a ring seems to spin
([Battle effects](../../games/tla/src/battle_effect/README.md#models)). A
[picture](#picture) is shown by a [sprite](#sprite-and-oam) as it was
drawn; a texture is painted onto a model and takes the shape of its faces.

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
So one drawing can appear in several colourings
([Graphics](../../games/tla/src/graphics/README.md#how-the-console-draws-an-image)).

Sources: the game: none. SGL: パレットナンバー. GBATEK: "Palette Number".

### Palette fade

A palette fade moves every colour on screen a little closer to a target
colour each [tick](#tick), in equal steps, so that they all arrive
together. A scene fading in as the party arrives is one, and so is the
change of light over 60 ticks before the Gabomba Statue speaks
([Field code](../../games/tla/src/field/README.md#the-palette-fade)).

Sources: the game: none. SGL: none. GBATEK: none.

### Sound

Every sound the game plays is asked for by a number, and the number's range
says what kind it is. Below 80 it is music, from 80 a jingle, and from 100 a
sound effect, an SE
([Sound](../../games/tla/src/sound/README.md#what-a-sound-number-means)).

Sources: the game: the credits' "Music" and "Sound Effects"; the sound test's
"Music", "Jingle" and "SE". SGL: ＢＧＭ and 効果音. GBATEK: "Sound 1-4" and "DMA
Sound A", the console's channels.

## The field

### Field

The field is everywhere the party walks: a town, a dungeon or the World Map.
The word also names the code that runs it, which is everything outside a
battle ([Field](field.md)).

Sources: the game: the credits' "Field"; せんとう&フィールド (`MsgUsableAnywhere`).
SGL: none. GBATEK: none.

### The ship

The ship is the Lemurian Ship, Piers's ship, which carries the party over
the sea on the World Map. At Atteka Inlet, Hamma gives it wings, a
[game flag](#game-flag) is set, and from then on the ship flies
([Scenes](../../games/tla/src/scene/README.md)).

Sources: the game: "Lemurian Ship" (ふね); the debug labels REM_SHIP01 and
REM_FUNE01. SGL: none. GBATEK: none.

### Scene

A scene is what the game loads as a whole: a town's streets, a floor
of a dungeon, a set of rooms, or the World Map. A few are screens instead,
such as the Title. There are 325, each a row of the list `ScenesWork`: Daila is
one, and its sanctum another, which is why the screen fades between them.
Each has a short label Camelot gave it (`TITLE`, `WORLD`, `DELY00` for
Daila) ([Field](field.md#scenes-and-areas)).

Sources: the game: a developer test message's シーン; the debug scene selector's
labels, one a scene (TITLE, DELY00). SGL: シーン, a scene file of a modelling
program. GBATEK: none.

### Room

A room is the inside of a building: a house, a shop, an inn. Most rooms are
not [scenes](#scene) of their own. Daila's inn and its shops are rooms of
one scene, on its [rooms map](#rooms-map).

Sources: the game: へや; the debug labels DLY_ROOM, MD_ROOM01 and KB_ROOM. SGL:
none. GBATEK: none.

### Area

An area is a group of [scenes](#scene) that belong together, 97 in all.
Daila's two town scenes, its rooms and its sanctum are four scenes of one
area. An area's scenes share their name and, unless a scene has music of
its own, their music. They also share one set of short-lived
[game flags](#game-flag)
([Field](field.md#scenes-and-areas)).

Sources: the game: the area names. SGL: none. GBATEK: none.

### Map

A map is the ground a [scene](#scene) is drawn on: a grid of
[metatiles](#metatile), 128 on a side, shown as three
[background layers](#background-layer). Two scenes can share one
([Maps](../../games/tla/src/map/README.md)). The World Map is a scene of its own, and the
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
([Field code](../../games/tla/src/field/README.md#the-world-map-in-perspective)).

It is not the [map screen](#map-screen), the map of the whole world that R
opens.

Sources: the game: none. SGL: none. GBATEK: none.

### Map screen

The map screen is the map of the whole world that R opens on the World Map.
It is drawn flat, with a marker on each town or dungeon the party has
visited and a cursor that L and R step from one marker to the next, naming
the place it rests on. Select shows the whole world at once, and A or B
closes it. The World Map is a [scene](#scene), the land the party walks and
sails on, drawn tilted away from it. The map screen is no scene: it is a
chart of that land, shown without leaving the World Map, and nobody walks on
it ([Field code](../../games/tla/src/field/README.md#the-map-screen)).

Sources: the game: "This place doesn't seem to appear on the map..." (ちず).
SGL: none. GBATEK: none.

### Rooms map

A rooms map is the [map](#map) of a [scene](#scene) that holds several
[rooms](#room) side by side, all drawn in one style. A room comes from its
rooms map. The door the party comes in by picks the room
([Scenes](../../games/tla/src/scene/README.md#design)).

Sources: the game: the credits' "Maps"; the debug labels' ROOM. SGL: none.
GBATEK: none.

### Metatile

A metatile is one 16-pixel square of a [map](#map). It says which [metatile tiles](#metatile-tiles) draw it, the
shape and height of the ground, and its [metatile behavior](#metatile-behavior)
([Maps](../../games/tla/src/map/README.md#metatiles)).

Sources: the game: none. SGL: none; its セル (`CELL`) is the 8-pixel tile, so
our square takes the word of other Game Boy Advance work, metatile. GBATEK:
none.

### Metatile tiles

A metatile's tiles are the four tiles of the [tileset](#tileset) that draw one
[metatile](#metatile) of a map, two by two. A tile is the console's own building
block, a square 8 pixels on a side ([Maps](../../games/tla/src/map/README.md#metatiles)).

Sources: the game: none. SGL: キャラクタパターン, a square of one cell or two by two.
GBATEK: none.

### Metatile behavior

A metatile behavior is what a [metatile](#metatile) does when the leader stands on it,
walks into it or searches it. It is what makes one metatile a wall, another a
ledge to jump down, and another a jar worth pressing A at
([Maps](../../games/tla/src/map/README.md#metatiles)).

Sources: the game: none. SGL: none. GBATEK: none.

### Actor

An actor is anything on the field that moves or is drawn on its own. The
party and the townspeople are actors, and so are the sparks of a Psynergy
and the point the camera follows. A scene has room for 64
([Field code](../../games/tla/src/field/README.md#everything-that-moves-runs-an-actor-script)).

Sources: the game: none. SGL: none. GBATEK: none.

### Actor id

An actor id is an [actor](#actor)'s number in its scene. Numbers 0 to 7 are
kept for the eight characters (`ACTOR_FELIX` is 4), so Isaac has a number,
0, long before he joins the party. From 8 on come the people and things the
scene places, in the order of its list of them
([Scenes](../../games/tla/src/scene/README.md#who-stands-where)).

Sources: the game: none. SGL: none. GBATEK: none.

### Actor placement

An actor placement is one row of a scene's list of who stands where. It
gives one person or thing its looks, the spot it stands on, the way it
faces, how it moves and the condition for it being there at all. Alhafra
keeps four such lists and picks one by how far the story has gone: before
Briggs is caught, after, once the mast is cleared and once the ship has
gone ([Scenes](../../games/tla/src/scene/README.md#who-stands-where)).

Sources: the game: none. SGL: none. GBATEK: none.

### Actor script

An actor script is the list of small orders an [actor](#actor) follows, a
little each [tick](#tick): move here, wait, jump, go back to the start.
These pages call its items orders, and keep "command" for an
[script command](#script-command). The orders' names start `SC_`, for script
command, the code's own word. A script does not run by itself: one
[script](#script) steps every actor's script each tick, and a script's wait,
`SC_WAIT`, is a count in the actor, not [`sySynch()`](#wait). A villager who
wanders near home is running one. An order can also hand the actor to a
piece of the field's own code that steers it each tick, and the player's
control of the leader on foot is one of those, `SC_PLAYER`
([Field code](../../games/tla/src/field/README.md#how-an-actor-script-runs)).

Sources: the game: none. SGL: none. GBATEK: none.

### Watched actor

The watched actor is the one another [actor](#actor) keeps its eye on. An
[actor script](#actor-script)'s orders to face, follow or walk up to
someone do not say whom. They mean the watched actor, so one script serves
whoever is watching whomever. In Lemuria, two people walking their rounds
are each given the other to watch
([Field code](../../games/tla/src/field/README.md#who-runs-which-script)).

Sources: the game: none. SGL: none. GBATEK: none.

### Field effect

A field effect is a small [actor](#actor) that is only decoration, such as
the sparks of a field Psynergy. It moves, grows and spins by an
[actor script](#actor-script) of its own
([Field code](../../games/tla/src/field/README.md#one-actor-for-everything)).

Sources: the game: none. SGL: none. GBATEK: none.

### Cue

A cue is one row of a scene's list of things that can happen. The row
says what starts it (talking, searching, stepping somewhere, using
Psynergy), who or where it is for, the condition under which it still
counts, and what it does. When the player does one of those things, the
first row that fits runs. In Alhafra, one row says that walking into a door
runs the [script](#script) that opens it and walks the leader through
([Scenes](../../games/tla/src/scene/README.md#what-a-cue-row-says)).

Sources: the game: none. SGL: none; its トリガー is a button of the pad, which is
why ours is a cue. GBATEK: none.

### Step cue

A step cue is a [cue](#cue) that runs when the leader steps
onto, or walks into, a [metatile](#metatile) with a given
[metatile behavior](#metatile-behavior). In Alhafra, stepping onto one spot starts
the arrival of Madra's mayor and his party, who have come hunting Briggs
([Scenes](../../games/tla/src/scene/README.md#which-cues-a-metatile-starts)).

Sources: the game: none. SGL: none. GBATEK: none.

### Examine cue

An examine cue is a [cue](#cue) that runs when the player
presses A facing a [metatile](#metatile). On Idejima, searching the spot where
Jenna lies is one: she wakes and joins the party. Chests, jars, barrels,
rocks, graves and trees have lines of their own, the same in every scene
([Scenes](../../games/tla/src/scene/README.md#which-cues-a-metatile-starts)).

Sources: the game: "searched the" (しらべる), the game's verb. SGL: none. GBATEK:
none.

### Exit

An exit is a way out of a [scene](#scene): a doorway, a path, the edge of
the map. A scene's list of exits says which scene and
[entrance](#entrance) each one leads to, and one exit can have several
rows, so that the same path leads somewhere new once the story has moved on
([Scenes](../../games/tla/src/scene/README.md#exits)).

Sources: the game: 出口. SGL: none. GBATEK: none.

### Entrance

An entrance is where the party appears in a [scene](#scene). It gives a
spot to stand on, a way to face and how far the camera may travel
([Scenes](../../games/tla/src/scene/README.md#entrances)).

Sources: the game: "Return to the dungeon's entrance." (入口). SGL: none.
GBATEK: none.

### Field request

A field request is a note in the field's [state](#state) that something
needs an answer. There are twelve kinds: an exit taken, a battle due, a
person to talk to, a metatile to search, a menu to open, and others. Three
kinds of code set one. The leader's [actor script](#actor-script) sets the
ones for buttons: press Start and its order `SC_PLAYER` sets the request
for the Pause menu. The field mode's own function sets the ones for
where the leader has stepped. And a [script](#script) or a menu sets one for
something the field must do after it has returned. The
[field mode](#field-mode) reads the requests each time its
`sySynch(1)` returns, never in the middle of a tick, and answers the most
urgent first ([Field code](../../games/tla/src/field/README.md#the-twelve-requests)).

Sources: the game: none. SGL: none. GBATEK: none.

### Script

A script is the piece of code a [cue](#cue) runs when something
happens in a scene: a cutscene, or a conversation that does more than show
one line. It is named for its scene. `scIdejimaWakeJenna` is what happens
when Felix finds Jenna lying on Idejima's shore: she wakes, asks whether he
has checked on the others, and joins the party. In the code it sits
between `fdScriptStart` and `fdScriptEnd`, one
[script command](#script-command) to a line, and a cue names it as
`SCRIPT_REF(scIdejimaWakeJenna)`
([Scenes](../../games/tla/src/scene/README.md#scripts)). A script is not an
[actor script](#actor-script), which is a table of orders.

Sources: the game: none. SGL: none; its イベント (`EVENT`) is our [event](#event).
GBATEK: none.

### Script command

A script command is one step of a [script](#script). The ones whose
names start `fdScript` are for speech, pauses and where the party goes next,
the ones that start `fdActor` move the people, and the ones that start
`fdCamera` move the camera.
On Idejima, `fdScriptShowMessage(ACTOR_JENNA, 0)` shows the script's next line
as Jenna's. The 0 asks for nothing special. A command is a C call. It is
not an order of an [actor script](#actor-script), although both can be
about an actor: `fdActorWalkTo` is a command a script calls, and `SC_MOVE`
is an order in a script's table. A command that takes time, such as
showing a line or one whose name ends `AndWait`, calls [`sySynch()`](#wait)
until what it started is over. One that only starts something, such as
`fdActorWalkTo`, returns at once
([Scenes](../../games/tla/src/scene/README.md#scripts)).

Sources: the game: none. SGL: none. GBATEK: none.

### Destination

The destination is the [scene](#scene) and [entrance](#entrance) the game
goes to next. An [exit](#exit) sets it, and a [script](#script) can set it
itself.

After a battle the party goes back where it was fighting, unless a script
chose somewhere else. After a defeat it goes to the restart point, again
unless a script chose somewhere else
([System library code](../../games/tla/src/system/README.md#running-another-mode)).

Sources: the game: none. SGL: none. GBATEK: none.

### Game flag

A game flag is one of the 4,096 yes-or-no facts the game remembers and
saves, such as Jenna having joined the party or Briggs having been caught.
Two ranges of 256 are short-lived instead. One is wiped each time the party
enters a scene, which is why a puddle frozen into a pillar of ice in Daila
is a puddle again when the party walks back in. The other is wiped on
entering a new [area](#area), which is why the spot where the Psy Crystal
was dug up stays dug while the party goes in and out of Daila's houses. A
battle wipes neither
([Game rules](../../games/tla/src/game/README.md#the-game-flags)).

Sources: the game: none. SGL: フラグ, any yes-or-no value. GBATEK: none.

### Debug menus

The debug menus are tools Camelot left in the released game for its own use,
which the player is never shown. The main one is the debug scene selector,
which Start opens on the field with B held. It lists every
[scene](#scene) by its number, label and area name, and sends the party
straight to the one picked. With L held, Start opens a palette editor
instead.

Among the scenes it reaches are eight test scenes, which share one overlay
whose scripts hand the party the Djinn Flint, Fizz, Forge and Gust. The
Pause menu gains three lines, `Rough menu`, `Debug menu` and
`Restart quest`. There is also a sound test
([Debug scenes](../../games/tla/src/debug/README.md#how-the-debug-scenes-are-reached)).

Sources: the game: "Rough menu" (がさつメニュー), "Debug menu", "Restart quest",
"Sound test" and "Shop Test"; the debug labels TEST0 to TEST7. SGL: none.
GBATEK: none.

## The battle

### Battle

A battle is one fight between the party and a
[formation](#formation) of enemies, from its opening to the experience and
coins at the end. It is a [mode](#mode) of its own, so the scene the
party was walking in is cleared away while it runs and set up again after
([Battle](battle.md)).

Sources: the game: the credits' "Battle"; "Battle" (バトル, せんとう). SGL: none.
GBATEK: none.

### Unit

A unit is one fighter in a battle, on either side: Felix is a unit, and so
is each enemy. The game keeps the same set of facts for every one of
them, its stats, Psynergy, items, Djinn and ailments, which the code reads
with `syUnitGet`. A unit in the party is a party member
([Game rules](../../games/tla/src/game/README.md#the-unit)).

Sources: the game: none. SGL: none. GBATEK: none.

### Formation

A formation is one group of enemies a battle is fought against. The game
has 660 of them in a list. One is Briggs with a Sea Fighter, and another
is a Wild Wolf with a Punch Ant
([Battles](../../games/tla/src/battle/README.md#the-enemies-of-a-formation)).

Sources: the game: none. SGL: none. GBATEK: none.

### Turns and actions

A turn is one pass through a battle's fighters, in which every
[unit](#unit) has its go. An
action is one unit's go within it: Felix's attack is one action, and the
Wild Wolf's answer is another ([Battles](../../games/tla/src/battle/README.md#turns)).

Sources: the game: "HP recovery / turn" (かいふくHP/ターン); 行動 for one unit's go.
SGL: none. GBATEK: none.

### Staging

Staging is how an [action](#turns-and-actions) is shown, as apart from what
it does: who steps forward, what line announces it and which
[battle effect](#battle-effect) plays
([Battles](../../games/tla/src/battle/README.md#staging-and-announcement)).

Sources: the game: none. SGL: none. GBATEK: none.

### Playback queue

The game works out what an [action](#turns-and-actions) does before it
shows any of it. The playback queue is the list of what is left to show for
each target: the flinch, the line such as `Felix takes 27 damage!`, the HP
going down. An [event](#event) plays it, a little each tick, once the
[battle effect](#battle-effect) has called `btPlaybackStart` to say the
blow has landed
([Battles](../../games/tla/src/battle/README.md#how-outcomes-reach-the-screen)).

Sources: the game: none. SGL: none. GBATEK: none.

### Battle effect

A battle effect is the show an ability puts on over a battle, such as
Quake's shaking ground or a summon's arrival. Each is a piece of code that
moves and draws its parts [tick](#tick) by tick: a loop that draws one
tick and calls [`sySynch(1)`](#wait). It decides when a hit is shown, never
what the hit does
([Battle effects](../../games/tla/src/battle_effect/README.md)).

Sources: the game: the credits' "Battle Effects". SGL: none. GBATEK: none.

### Mote

A mote is one of the small specks an effect scatters by the dozen: the
specks of light that tumble in on each target of Wish, or the ring of them
the summon Thor draws. Each has its own place, speed and life, and the
effect moves every one of them each [tick](#tick). A
[battle effect](#battle-effect)'s mote is not a [sprite](#sprite-and-oam):
the effect's blitter copies its [picture](#picture) onto the canvas the
effect draws on, most often a round glowing dot at one of ten sizes, often
a smaller one the farther the mote is
([Battle effects](../../games/tla/src/battle_effect/README.md#pictures-and-particles)). On
the field a mote is a sprite, such as the motes that circle down onto the
leader as a Venus Djinni joins.

Sources: the game: none. SGL: none. GBATEK: none.

### Battle background

A battle background is the image behind a battle. It is chosen by where
the fight happens, such as the grassland, the Suhalla Desert or a ship's
deck ([Graphics](../../games/tla/src/graphics/README.md#battle-backgrounds)).

Sources: the game: the credits' "Battle Backgrounds". SGL: none. GBATEK: none.

### Battle Arena

The Battle Arena is where two players fight each other's parties, their
consoles joined by a link cable, or fight the monsters. The game greets
them with `Welcome to Battle Arena,`. Its code is the lobby's
[overlay](#overlay), The Broken Seal's arena reworked
([Scenes](../../games/tla/src/scene/README.md#the-title-the-file-screen-and-the-battle-arena)).

Sources: the game: "Welcome to Battle Arena" (バトルステージ), "Linked Battles and
Monster Battles"; the debug label ROUNGE. SGL: none. GBATEK: none.

## The interface

### Interface

The interface is everything the player reads and chooses from: the
[windows](#window), the [messages](#message) typed into them, the menus, and
the counters of the shops, sanctums, inns and the forge
([The menu engine](interface.md)).

Sources: the game: the credits' "User Interface". SGL: none. GBATEK: none.

### Window

A window is a bordered box the game opens for text or choices. Talk to a
villager and one grows out of its own centre, then shrinks back into it when
the line is done ([Windows](../../games/tla/src/window/README.md#windows)). The console's
own display windows, which show or hide layers over parts of the screen, are
another thing, which the code names `WIN0` and `WININ`.

Sources: the game: "Window color" (ウインドウカラー). SGL: ウインドウ, a display window.
GBATEK: "Window 0", "OBJ Window", the display windows.

### Border

The border is the edge drawn around a [window](#window). It is drawn in the
window colour the player chose in the settings, as the window's filling is
([Windows](../../games/tla/src/window/README.md#windows)).

Sources: the game: none. SGL: none. GBATEK: none.

### Message

A message is a piece of text the game types into a [window](#window) a few
letters at a time, stopping for a button where a page ends. A villager's
line is one, and so is a battle's `Felix takes 27 damage!`. Up to three can
be typing at once ([Windows](../../games/tla/src/window/README.md#typing)).

Sources: the game: "Message Speed" (メッセージスピード). SGL: none. GBATEK: none.

### The screens

The screens are the Title, the File Screen, the Pause menu, the field menu
with its Psynergy, Djinn, Item and Status screens, the settings menu, name
entry and the [map screen](#map-screen). The File Screen is where a
[file](#file-and-copy) is chosen, copied or erased, and the Pause menu is
what Start opens on the field ([The menu engine](interface.md)).

Sources: the game: "Title screen" (タイトルがめん), "Please choose a file.", "The
Pause Menu is not available now." (ポーズメニュー); the debug label TITLE. SGL: none.
GBATEK: "Screen Base Block", a layer's map.

### Password

A password carries a party over from The Broken Seal. The player types it
into a grid of letters, and the game reads the party's units and some of
its flags from it; the same can come over a link cable instead. A Japanese
password is kana. The others use capitals, digits, small letters and
symbols, leaving out the letters that look alike: I and O, l and o. Both
ways in are in the scene after the Title
([Scenes](../../games/tla/src/scene/README.md#the-title-the-file-screen-and-the-battle-arena)).

Sources: the game: "Password" (パスワード), "Invalid password.". SGL: none. GBATEK:
none.

### Menu

The menu is the field menu, the row of icons opened on the field, and its
Psynergy, Djinn, Item and Status screens
([Menus](../../games/tla/src/menu/README.md#the-screens)). The folder `menu/` also holds the
staff credits and the title intro.

Sources: the game: "Rough menu" (がさつメニュー), "Debug menu". SGL: none. GBATEK:
none.

### Shop

A shop is any counter the party does business at: a shop's, a sanctum's or
an inn's, or the [forge](#the-forge). Alhafra's innkeeper asking 7 coins a
guest is the shop code at work ([Shops](../../games/tla/src/shop/README.md)).

Sources: the game: "Weapons Shop", "Feel free to keep shopping!" (武器屋, やど屋: a
shop by its trade). SGL: none. GBATEK: none.

### The forge

The forge is Sunshine's smithy in Yallam. The party leaves a material
there, such as a Tear Stone, and comes back for what was made of it. What
comes out is rolled from the material's own list: a Tear Stone becomes a
Cloud Wand, a Clear Bracelet, a Pure Circlet or a Spirit Ring. Its code is
in the shop module, and the material left there is kept in the
[main routine's state](#main-routines-state), so a save remembers it
([Shops](../../games/tla/src/shop/README.md)).

Sources: the game: "Sunshine's smithy" (かじ屋), "(forgeable)" (きたえなおせる). SGL:
none. GBATEK: none.

## The folders

The folders of `src/` that are not named for a module of code.

### Game rules

The game rules are what a [unit](#unit), an item or a Djinni is and does:
how much PP Cure costs, what Echo adds to a character. The game logic,
the code under `game/`, keeps them in one place because the field engine,
the battle engine and the menu engine all need the same answers
([Game rules](../../games/tla/src/game/README.md)).

Sources: the game: the credits' "Game Design" and "Game Planning". SGL: none.
GBATEK: none.

### Graphics

Graphics is the game's art as files anyone can open and edit. Each
folder is named for what it holds or the place it belongs to
([Graphics](../../games/tla/src/graphics/README.md)).

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
- `palette_editor`: the swatch of the palette editor, a tool the
  [debug menus](#debug-menus) reach.
- `sprite`: the drawings of every person and thing that moves, and their
  animations, as [sprites](#sprite-and-oam).
- `tileset`: the tiles and colours a map's
  [metatile tiles](#metatile-tiles) come from.
- `ui`: every [interface](#interface) image, such as the menus' icons.

Sources: the game: the credits' "Battle Backgrounds", "Battle Effects",
"Monster Design", "Map Characters", "Battle Characters", "Maps" and "User
Interface"; "Monsters won't attack". SGL: none. GBATEK: none.

### Places and minigames

The graphics folders named for a place or a thing in the game:

- `aqua_rock` and `aqua_rock_inside`, Aqua Rock's tiles, outside and in.
- `lemuria_palace`, the tiles of Lemuria's palace.
- `trial_road`, Trial Road's image.
- `lucky_dice` and `lucky_wheels`, the two minigames' images.
- `world_map`, the World Map's own images.
- `title`, the Title's images and its intro.
- `credits`, the staff roll's font.
- `ending`, the images of the game's ending.

Sources: the game: "Lucky Dice" (ギャンブルダイス), "Lucky Wheels" (ふくびきスロット), "World
Map", the area names Aqua Rock, Lemuria and Trial Road; the debug label TITLE.
SGL: none. GBATEK: none.

### Tileset

A tileset is the set of tiles, with their colours, that a map's
[metatile tiles](#metatile-tiles) come from. A tile is a square 8
pixels on a side, the console's own building block
([Graphics](../../games/tla/src/graphics/README.md#tilesets)).

Sources: the game: none. SGL: none. GBATEK: none.

### Monster

A monster is an enemy as the player sees it, its sprite in a battle
([Graphics](../../games/tla/src/graphics/README.md#monsters)). In the code an enemy is a
[unit](#unit) like any other.

Sources: the game: the credits' "Monster Design"; "Monsters won't attack so
often now." (モンスター). SGL: none. GBATEK: none.
