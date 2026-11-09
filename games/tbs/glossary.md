# Glossary

The words the pages about The Broken Seal share, each with what it means and
a link to where it is explained. An entry's `Evidence:` line says where its
word comes from: Camelot's own word where Camelot has one for the thing, else
the word programmers of the time used, else plain English.

## Golden Sun

### The two games

Golden Sun is a role-playing game for the Game Boy Advance in two parts: The
Broken Seal (2001), whose builds are named `tbs`, and its sequel The Lost Age
(2002), named `tla`.

Evidence: Camelot: the games' titles.

### Camelot

Camelot Software Planning made both games, and Nintendo published them; their
source was never released.

Evidence: Camelot: the credits.

### The party

The characters travelling together, led by Isaac: in The Broken Seal, Isaac,
Garet, Ivan and Mia. In a battle each of them is a [unit](#unit), and a unit
in the party is a party member
([Game rules](src/game/README.md#the-party-and-its-coins)).

Evidence: Camelot: the game's text says party throughout.

### The places, in story order

The story starts in Vale, under Mt. Aleph, and follows the thieves of the
Elemental Stars through Vault, Bilibin, Kolima, Imil, Mercury Lighthouse,
Xian, Altin, Kalay and Tolbi to Lalivero and Venus Lighthouse, where the game
ends. Pages list places in that order;
[Places](https://alchemy.pascalpixel.com/tbs/places) shows every scene.

Evidence: Camelot: the area names in the game's text.

### Psynergy and PP

![Isaac casts Growth in battle: light gathers around him, then a spiked vine bursts out of the ground under Ivan](https://alchemy.pascalpixel.com/abilities/growth.gif)

Psynergy is the game's magic, cast in battle (Quake, Cure, Growth) and on the
field (Move, Reveal), where it pushes, grows or uncovers things
([Field code](src/field/README.md#how-field-psynergy-reaches-its-target)).
Each use costs PP, Psynergy Points, which come back as the party walks
outside towns and at inns.

Evidence: Camelot: the game's text ("Your Psynergy Points.").

### The four elements

Venus (earth), Mercury (water), Mars (fire) and Jupiter (wind), always in
that order. Every character, Djinni and summon belongs to one: Isaac is
Venus, Garet Mars, Ivan Jupiter and Mia Mercury
([Game rules](src/game/README.md#element-levels)).

Evidence: Camelot: the game's text.

### Power and Resist

For each element, every fighter has a Power and a Resist: Power raises the
damage and healing of what it casts in that element, and Resist lowers the
damage it takes from it
([Battles](src/battle/README.md#the-element-multiplier)).

Evidence: Camelot: the Status screen's labels.

### Djinn

![Ivan unleashes the Jupiter Djinni Gust in battle: pink wind streams across the field, then a blue blast strikes its target](https://alchemy.pascalpixel.com/abilities/gust.gif)

Elemental creatures the party collects; one is a Djinni, several are Djinn.
Set on a character, a Djinni raises its stats and helps decide its class;
unleashed in battle, it does its own move and goes on standby, ready for a
summon ([Game rules](src/game/README.md#djinn-and-classes)).

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

Source code written so that it compiles back into exactly the bytes on the
cartridge. The source therefore keeps the game's bugs, and sometimes an
unusual form where plainer C would behave the same
([Introduction](../README.md)).

Evidence: plain English; the project's own words.

### The original

The game as [Camelot](#camelot) shipped it: each edition's cartridge, which
the source reproduces byte for byte, bugs included.

Evidence: plain English; the project's own word.

### Editions

Each game was released in Japanese, English, German, Spanish, French and
Italian, and each release is an edition (`tbs-ja` to `tbs-it`). The five
other than the Japanese are the international editions.

Evidence: plain English; the project's own word.

### pret

The group behind the decompilations of other Game Boy Advance games, whose
conventions this source follows: the shape of names (`PascalCase` functions,
`gName` globals) and the `BUG` and `BUGFIX` markers.

Evidence: the group's own name.

### BUG, BUGFIX and UBFIX

`// BUG:` marks one of the original game's bugs, kept as it is, with a fix
behind `BUGFIX`. `// UB:` marks code whose result the C language leaves
undefined, with a defined form behind `UBFIX`.

Evidence: plain English, in pret's markers.

### Unused

`UNUSED` marks a function nothing calls, and a comment opening with `Unused:`
something the game can never reach, with the reason. What only the
[debug menus](#debug-menus) reach is not unused.
[Unused](https://alchemy.pascalpixel.com/tbs/unused) collects them.

Evidence: plain English.

### Module header

The header that declares what one C file defines, at the same path under
`include/module/` as the file has under `src/`
([Source](src/README.md#headers)).

Evidence: plain English; the project's own word.

### Object

One compiled file of the source, an object file in the linker's sense; the
word has nothing to do with a scene's [actors](#actor). A long one is
written as parts, in a folder of its own name
([Source](src/README.md#objects-and-parts)).

Evidence: plain English; the GNU tools' "object file".

## The system

### System

The code the field, the battles and the interface are built on: start-up,
the main routine, waiting and [tasks](#task), the [heap](#heap-and-pools),
the [overlays](#overlay), saving and sound. It is one of the code's four
divisions, with Field, Battle and Interface ([System](system.md)).

Evidence: Camelot: the credits, "Programming", then "System", "Battle",
"Field", "User Interface" and "Support" (`gCredits_Lines`).

### Main routine

The routine that runs for as long as the console is on, `MainRoutine_Run`:
it runs one [sequence](#sequence), clears away what it set up, and starts
the next
([The Broken Seal](README.md#the-main-routine-and-its-sequences)).

Evidence: tradition: メインルーチン, the period's word for the code that runs
outside the interrupts.

### Sequence

One whole kind of play, run from its start to its end: the field sequence,
the battle sequence, the dice sequence (Lucky Dice) or the slots sequence
(Lucky Wheels), `SEQUENCE_` in code. One runs at a time
([The Broken Seal](README.md#the-main-routine-and-its-sequences)).

Evidence: tradition: ゲームシーケンス.

### Field sequence

The sequence that runs one [scene](#scene), from the party's arrival to the
[exit](#exit) it leaves by, answering the [field requests](#field-request)
on the way
([Field code](src/field/README.md#the-field-sequence-serves-requests)).

Evidence: Camelot: the credits' "Field" (`gCredits_Lines`); tradition: sequence.

### Transition and next sequence

The handover from one sequence to the next: the ending sequence leaves what
runs next in the [main routine's work](#main-routines-work), a scene or
another sequence, and the main routine starts it. A change of scene is a
transition, as going into a battle is
([System code](src/system/README.md#the-main-routine)).

Evidence: tradition: シーケンス遷移.

### Main routine's work

The one piece of memory every sequence shares and that outlives each of
them, `gMainRoutineWork`: what runs next, the party, its coins and Djinn.
A save file keeps it with the game flags
([The Broken Seal](README.md#what-the-game-remembers)).

Evidence: tradition: work, named for its owner, the main routine.

### Frame

One picture on the screen and the game's work for it, about a sixtieth of a
second. Waits, walking and animation are all counted in frames
([The Broken Seal](README.md#the-frame)).

Evidence: tradition: フレーム, one pass of a game's processing.

### Wait

Letting frames pass in the middle of a routine: `wait(n)` runs n frames of
the game and then returns to the next line, so a sequence or an event is
written as straight-line code
([The Broken Seal](README.md#code-that-waits)).

Evidence: tradition: WAIT, the period's word for it.

### Task

A function the game calls in every [frame](#frame) while other code waits,
until it is removed: the windows' typing, the map's scrolling, the actors'
movement. The task table, `gTasks`, holds up to 20
([The Broken Seal](README.md#tasks)).

Evidence: tradition: タスク and its 実行関数.

### VBlank

The pause after the console has drawn one image and before it starts the
next, about 60 times a second. An interrupt at each VBlank puts the finished
frame on screen, mixes the sound and reads the buttons, whatever the game is
doing ([System code](src/system/README.md#what-vblank-does)).

Evidence: tradition: V ブランク, the hardware's own name.

### Slowdown

A frame that takes longer than one VBlank: every wait, walk and animation
slows down together, while the music keeps its tempo
([System](system.md#slowdown)).

Evidence: tradition: 処理落ち.

### Work

The block of memory a piece of code keeps its state in, held in a
[heap slot](#heap-slot) and named `g<Slot>Work`: the field's is `gFieldWork`,
the menus' `gMenuWork`. The [main routine's](#main-routines-work) is the one
kept outside the heap
([The Broken Seal](README.md#what-the-game-remembers)).

Evidence: tradition: ワーク, working memory, as in the hardware's Work RAM,
used here for a system's block.

### Heap and pools

The heap lends blocks of memory from two pools, one in IWRAM and one in
EWRAM; the last block taken is the first given back, and the whole heap is
emptied before each sequence ([System code](src/system/README.md#the-heap)).

Evidence: plain English.

### Heap slot

One of 62 numbered places that each hold a heap block other code has to find
again, such as the running scene's [work](#work)
([System code](src/system/README.md#the-heap)).

Evidence: plain English; the project's own word.

### IWRAM and EWRAM

The console's two areas of RAM: IWRAM, 32 KB inside the processor and the
fastest it has, and EWRAM, 256 KB beside it and slower
([System code](src/system/README.md#memory-map)).

Evidence: tradition: the hardware's own names.

### Main image

The code the processor runs straight from the cartridge, about 1 MB in
fourteen [banks](#bank); the rest runs from RAM, a scene's
[overlay](#overlay) in EWRAM and the fastest routines in IWRAM
([The Broken Seal](README.md#the-console)).

Evidence: plain English; the project's own word.

### Game Pak

The cartridge as hardware: its ROM, cut into [banks](#bank), the
[resource directory](#resource-directory), and the
[flash chip](#flash-chip) that holds the saves
([The Game Pak](src/gamepak/README.md)).

Evidence: Nintendo: "Game Pak", the instruction booklets' word for the
cartridge; tradition: GBATEK's "GamePak ROM", "GamePak SRAM" and "GamePak
Waitstates".

### Flash chip

The 64 KB chip on the cartridge the three save [files](#file-and-part) are
written to, in 16 sectors of 4 KB; it keeps what is written with the power
off ([The Game Pak](src/gamepak/README.md#the-flash-chip)).

Evidence: the hardware's word, GBATEK's "GamePak Flash ROM"; the library's
file names are the project's, from the chip makers' names as GBATEK lists them
(Atmel, SST, Macronix).

### Bank

One of the fourteen stretches the main image is cut into, each starting at a
fixed address with room to spare after its code, so one bank can grow
without moving anything another depends on
([The Game Pak](src/gamepak/README.md#why-the-main-image-is-cut-into-banks)).
Every bank is always in place; none is switched in or out.

Evidence: plain English; the project's own word.

### Bank folder

A folder of `src/` that holds one or more banks' code. The folders are
grouped by the four divisions: the System's `system/`, `gamepak/` and
`sound/`; the Field's `field/`; the Battle's `battle/`, `game/` and
`battle_effect/`; and the Interface's `window/`, `menu/` and `shop/`
([Source](src/README.md#banks-and-folders)).

Evidence: Camelot: the credits' four Programming divisions, "System",
"Battle", "Field" and "User Interface" (`gCredits_Lines`); `window`,
`menu` and `shop` from their own entries; plain English for the grouping.

### Far call

A call from one bank into another through the table of
[veneers](#veneer) the called bank opens with. The veneer is named after its
function with `Far` added: `Battle_RunFar` leads to `Battle_Run`
([The Game Pak](src/gamepak/README.md#why-the-main-image-is-cut-into-banks)).

Evidence: tradition: ジャンプテーブル for the table; "far call" is plain English.

### Veneer

A tiny piece of code that only jumps on to a function somewhere else, 8
bytes long. Every entry of a bank's far-call table and of an overlay's two
tables is one ([The Game Pak](src/gamepak/README.md#a-veneer)).

Evidence: plain English; the project's own word.

### Overlay

A packed piece of code unpacked into one 32 KB area of EWRAM when it is
needed: each holds the code of one scene or of a few neighbouring scenes, 96
in all for 201 scenes, and only one is loaded at a time
([The Broken Seal](README.md#overlays)).

Evidence: tradition: the GNU linker's word; the linker's `.bss` for the
globals' folder.

### Entry table and import table

The two tables an overlay meets the main image through: the entry table
opens it with one veneer for each question the field asks a scene, and the
import table ends it with one veneer for each main-image function it calls
([System code](src/system/README.md#how-an-overlay-reaches-the-rest-of-the-game)).

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

One of the console's four layers of tiles, each scrolling as a whole; a town
is drawn on three of them, the World Map on the two the console can scale
and rotate
([Field code](src/field/README.md#towns-and-dungeons-on-screen)).

Evidence: tradition: the hardware's BG.

### Sprite and OAM

A sprite is a small image the console draws over or between the background
layers; OAM, the object attribute memory, is the console's list of them,
rebuilt every frame ([System code](src/system/README.md#the-frames-sprites)).

Evidence: tradition: スプライト and the hardware's OAM.

### Sprite animation

One of the up to four layers of a sprite, each playing its own run of
[pictures](#picture): Isaac's body, his weapon and his shadow are three
sprite animations of one sprite
([Field code](src/field/README.md#sprites-and-their-animation)).

Evidence: plain English; no word of the time names one layer of a sprite.

### Picture

One drawing in a sprite animation or a tile animation, shown for a number of
frames before the next
([Field code](src/field/README.md#sprites-and-their-animation)).

Evidence: plain English; the time's パターン and コマ each mean other things too.

### Palette number

Which of the sixteen 16-colour palettes a tile or a sprite is drawn in
([Graphics](src/graphics/README.md#how-the-console-draws-a-picture)).

Evidence: plain English for the hardware's own field.

### Palette fade

The change of every colour on screen towards a target, by an equal share
each frame, so that they all arrive together; the field fades each scene in
with it, and field Psynergy tints the screen with it
([Field code](src/field/README.md#the-palette-fade)).

Evidence: plain English.

### Sound

Every sound the game plays is asked for by a sound number, and the number's
range says what kind it is: below 80 music, from 80 a jingle, and from 100 a
sound effect, an SE
([Sound](src/sound/README.md#what-a-sound-number-means)).

Evidence: Camelot: the credits' "Music" and "Sound Effects", and The Lost
Age's sound test's "Music", "Jingle" and "SE".

## The field

### Field

Everywhere the party walks, a town, a dungeon or the World Map, and the code
that runs it ([Field](field.md)).

Evidence: Camelot: the credits' "Field", and せんとう&フィールド
(`MsgUsableAnywhere`).

### Scene

One row of the scene table, `gScenes`: a town, a dungeon floor, a few rooms,
the World Map, or a screen such as the Title or the File Screen, 201 in all.
Each has Camelot's scene code (`PLOGV00` for Vale on the stormy night of the
prologue, `TITLE`, `WORLD`) and an area name
([The Broken Seal](README.md#scenes)).

Evidence: Camelot: The Lost Age's developer test message 05746 heads a location name シーン.

### Room

A scene inside a building, a house, a shop or an inn.

Evidence: Camelot: the scene labels' ROOM and RM (HAIROOM1, KUP_RM01,
FUCHINRM).

### Area

A group of scenes, 71 in all, sharing their music, their area name, the one
the game shows when the party arrives, and one range of scratch
[game flags](#game-flag) ([Field](field.md#scenes-and-areas)).

Evidence: Camelot: the area names.

### Map

What a scene is drawn on: a grid of [cells](#cell), 128 on a side, shown as
three background layers; several scenes can share one
([Maps](src/map/README.md)).

Evidence: Camelot: the credits' "Maps".

### Still picture

The World Map's tilted view drawn once into an ordinary background layer
while a menu or an event is open, which frees a layer for the windows
([Field code](src/field/README.md#the-world-map-in-perspective)). The picture
of the whole world that R shows is the world picture, a screen of its own
([Field code](src/field/README.md#the-world-picture)).

Evidence: plain English for what the player sees, the view standing still.

### Sheet of rooms

A map that holds several [rooms](#room) drawn in one tile style side by
side, each its own scene. Its rooms may belong to different places, so it is
named for the style, as Cottage rooms or Stucco rooms
([Scenes](src/scene/README.md#design)).

Evidence: plain English; the project's own word.

### Cell

A 16-pixel square of a map: which cell picture is drawn there, the shape and
height of the ground, and its cell behavior
([Maps](src/map/README.md#cells)).

Evidence: plain English.

### Cell picture

The picture of one cell: four of the console's 8-pixel tiles, two by two
([Maps](src/map/README.md#cells)).

Evidence: plain English.

### Cell behavior

What standing on a cell, walking into it or searching it does: a wall, a
ledge, a jar ([Maps](src/map/README.md#cells)).

Evidence: plain English; Camelot's 属性 (attribute) means an element.

### Actor

Anything on the field that moves or is drawn on its own, from the party and
the townspeople to the camera and the footprints in snow. There are 64, the
slots of `gActors`
([Field code](src/field/README.md#everything-that-moves-runs-an-actor-script)).

Evidence: tradition: the Western word for a thing on a game's stage.

### Actor id

An actor's number in its scene: 0 to 7 are the characters (`ACTOR_GARET` is
1), and from 8 on come the scene's placed actors in the order of its actor
table ([Scenes](src/scene/README.md#who-stands-where)).

Evidence: plain English.

### Actor placement

A row of a scene's actor table: the sprite, place, facing, actor script and
condition of one person or thing the scene puts on the field when it loads
([Scenes](src/scene/README.md#who-stands-where)).

Evidence: plain English for what a row does, place an actor.

### Actor script

The list of `SC_` commands an actor runs, a little each frame: waits, moves,
jumps and loops. One command can hand the actor to a built-in script, a
routine inside the field that steers it, such as the player's control of the
leader ([Field code](src/field/README.md#how-an-actor-script-runs)).

Evidence: tradition: スクリプト for a character's commands.

### Watched actor

The actor another actor's script faces, follows or walks toward
([Field code](src/field/README.md#who-runs-which-script)).

Evidence: plain English for what the commands do.

### Field effect

A small decorative actor on the field, such as the sparks of a field
Psynergy, that moves, grows and spins by its own script
([Field code](src/field/README.md#one-actor-for-everything)).

Evidence: plain English.

### Trigger

A row of a scene's trigger table: what starts it, who or where it is for,
the condition under which it still applies, and what it does. When the
player talks, searches, steps on a marked cell or uses Psynergy, the first
row that fits runs
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

A way out of a scene, named for where it leads (`VALE_PLAZA_TO_WORLD_MAP`);
the first row for it in the scene's exit table whose condition holds gives
the next scene and entrance ([Scenes](src/scene/README.md#exits)).

Evidence: Camelot: the game's text, 出口.

### Entrance

Where the party arrives in a scene, named for where it comes from
(`VALE_PLAZA_FROM_SANCTUM`): a position, a facing and the camera's limits
([Scenes](src/scene/README.md#entrances)).

Evidence: Camelot: the game's text, 入口.

### Field request

One of twelve notes in the field's work, each a thing the walking leader has
run into: an exit, a battle, a person to talk to, a cell to search, a menu.
The field sequence answers them most urgent first
([Field code](src/field/README.md#the-twelve-requests)).

Evidence: plain English; the project's own word.

### Event

The C routine a trigger runs, named for its scene (`Vale_EnterIsaacsHouse`):
a cutscene, or a conversation that does more than show one message. It runs
between `Event_Start` and `Event_End`, one event command to a line
([Scenes](src/scene/README.md#events)).

Evidence: Camelot: the Shining Force III credits' "Events", by two of Golden
Sun's field programmers.

### Event command

A routine an event calls: `Event_` for speech, waits and destinations,
`Actor_` for actors and `FieldCamera_` for the camera
([Scenes](src/scene/README.md#events)).

Evidence: plain English.

### Destination

The scene and entrance the main routine runs next: an exit sets it, and an
event can set it itself. After a battle the party goes to the after-battle
destination when an event set one, else to the return point, the scene it
left; after a defeat, to the defeat destination when one is set, else to the
restart point, usually the last Sanctum
([System code](src/system/README.md#running-another-sequence)).

Evidence: plain English.

### Game flag

One of the 4,096 yes-or-no facts saved with the game, such as a chest being
open; two ranges are scratch space, cleared as the party enters scenes and
areas ([Game rules](src/game/README.md#the-game-flags)).

Evidence: Camelot: 田口泰宏, the System programmer, speaks of flags (Shining
Force III, 1998).

### Debug menus

What the released game keeps for its makers and never shows the player: the
debug scene selector, which names every scene by its scene code and area
name and takes the party there, the test scenes it reaches, the Pause menu's
Rough menu, Recovery test and Restart quest, and the sound test
([Debug scenes](src/debug/README.md#how-the-debug-scenes-are-reached)).

Evidence: Camelot: the menus' labels ("Rough menu", "Recovery test",
パラメータセット), the scene codes TEST0 to TEST7 and The Lost Age's サウンドテスト.

## The battle

### Battle

The fight between the party and a formation of enemies, a sequence of its
own ([Battle](battle.md)).

Evidence: Camelot: the credits' "Battle"; `battle/transition/` takes the name of
the code that draws it.

### Unit

Either side's fighter in a battle (`struct Unit`); a party member is a unit
in the party ([Game rules](src/game/README.md#the-unit)).

Evidence: plain English; the game's キャラ means only a party member.

### Formation

One group of enemies a battle is fought against, 380 in all
([Battles](src/battle/README.md#the-enemies-of-a-formation)).

Evidence: plain English.

### Turns and actions

A turn is every unit in a battle acting once; an action is one unit's go
within it ([Battles](src/battle/README.md#turns)).

Evidence: Camelot: "HP recovery / turn" (かいふくHP/ターン) and 行動 for one
unit's go.

### Staging

How an action is shown, apart from what it does: who comes forward, what is
announced and which battle effect plays
([Battles](src/battle/README.md#staging-and-announcement)).

Evidence: tradition: 演出.

### Playback queue

The list of what an action's rules left to show for each target, the flinch,
the message, the falling HP, played once the battle effect says the blow has
landed ([Battles](src/battle/README.md#how-outcomes-reach-the-screen)).

Evidence: plain English.

### Battle effect

What an ability plays over a battle, drawn by code: Quake's shaking ground,
a summon's arrival. It decides when a hit shows, never what it does
([Battle effects](src/battle_effect/README.md)).

Evidence: Camelot: the credits' "Battle Effects", beside "Battle Characters"
and "Battle Backgrounds", a Graphic Arts heading; the code is the Battle
division's.

### Battle background

The picture behind a battle, chosen by the place it is fought in: the
grassland, Goma Cave, a ship's deck
([Graphics](src/graphics/README.md#battle-backgrounds)).

Evidence: Camelot: the credits' "Battle Backgrounds".

## The interface

### Interface

The windows, the messages typed into them, the menus and the counters of the
shops, sanctums and inns ([Interface](interface.md)).

Evidence: Camelot: the credits' "User Interface".

### Window

A bordered box the game opens for text or choices, growing out of its centre
([Windows](src/window/README.md#windows)).

Evidence: Camelot: ウインドウ, as in "Window color" (`MsgWindowColorLabel`).

### Border

The edge drawn around a window ([Windows](src/window/README.md#windows)).

Evidence: plain English.

### Message

The text the game types into a window a few letters at a time, waiting for a
button at each page break; up to three are typed at once
([Windows](src/window/README.md#typing)).

Evidence: Camelot: the settings label メッセージスピード ("Message" in English,
`MsgMessageSpeedLabel`).

### The screens

The Title; the File Screen, where a file is chosen, copied or erased; the
Pause menu, which Start opens on the field; the field menu with its
Psynergy, Djinn, Item and Status screens; the settings menu; name entry; and
the world picture ([Interface](interface.md)).

Evidence: Camelot: "To the File Screen", "The Pause menu is not..."
(`MsgFieldPauseUnavailable`), the scene label TITLE; the rest plain English.

### Menu

The field menu and its Psynergy, Djinn, Item and Status screens
([Menus](src/menu/README.md#the-screens)); the folder `menu/` also holds the
staff credits and the title intro.

Evidence: Camelot: "Rough menu" (`MsgWorkspaceRoughMenu`), "Menu test" and
the debug row MENU.

### Shop

The counters of the shops, sanctums and inns ([Shops](src/shop/README.md)).

Evidence: the game: "weapon shop", "Thank you for shopping".

## The folders

The folders of `src/` that are not named for a bank of code.

### Game rules

What a unit, an item or a Djinni is and does, which the field, the battles
and the menus all ask ([Game rules](src/game/README.md)).

Evidence: Camelot: the credits' "Game Design" and "Game Planning"; plain
English "game rules", said so.

### Graphics

The game's pictures in editable form, each folder named for what it holds or
the place it belongs to ([Graphics](src/graphics/README.md)).

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
- `tileset`: the tiles and palettes a map's
  [cell pictures](#cell-picture) are drawn from. Evidence: tradition: the
  tile set; Camelot: the credits' "Maps" for the work.
- `ui`: every [Interface](#interface) picture. Evidence: Camelot: the
  credits' "User Interface", whose banks place them; plain English for the
  subfolders.

### Places and minigames

The graphics folders named for a place or a thing in the game:

- `colosso`, the Colosso's lettering, counters and gauge. Evidence: the
  game: "Colosso".
- `lamakan_desert`, the desert's heat gauge. Evidence: its area name,
  Lamakan Desert.
- `sol_sanctum_statue`, the statue room's scrolling rows. Evidence: its area
  name, Sol Sanctum.
- `lucky_dice` and `lucky_wheels`, the two minigames' pictures and tables.
  Evidence: the game: "Lucky Dice", "Lucky Wheels".
- `world_map`, the World Map's tiles and the lighthouse transition.
  Evidence: the game: "World Map".
- `title`, the Title's pictures and its prompt. Evidence: Camelot: the scene
  label TITLE.
- `credits`, the staff roll's font. Evidence: Camelot: the credits
  themselves ("GOLDEN SUN STAFF CREDITS").

### Tileset

The tiles and palette a map's cell pictures are drawn from
([Graphics](src/graphics/README.md#tilesets)).

Evidence: tradition: the tile set; plain English.

### Monster

An enemy as drawn, its battle picture
([Graphics](src/graphics/README.md#monsters)); in the code an enemy is a
[unit](#unit).

Evidence: Camelot: the credits' "Monster Design"; the game: "Monsters won't
attack".
