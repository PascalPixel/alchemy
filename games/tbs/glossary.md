# Glossary

Every page links here the first time it uses one of these words. They come in
the order a newcomer meets them: the game's own words, then the project's,
then the words of the code's four divisions, the system, the field, the battle
and the user interface. Each entry ends with where its word comes from: from
Camelot (the game's credits, its text and its debug labels) where Camelot has
a word for the thing, else from Sega, whose programmers Camelot grew out of
(Sega's SGL library and 平山尚's book from Sega's training), else from the
tradition of the time (the game programmers who wrote the Digital Content
Association of Japan's reports, DCAJ), else plain English.

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
ends. The story is the order the [game flags](#game-flag) are set in, so pages
list places in that order; [Places](src/field/scenes.s) lists every scene.

Evidence: Camelot: the area names in the game's text.

### Psynergy and PP

![Isaac casts Growth in battle: light gathers around him, then a spiked vine bursts out of the ground under Ivan](https://alchemy.pascalpixel.com/abilities/growth.gif)

Psynergy is the game's magic, cast in battle (Quake, Cure, Growth) and on the
field (Move, Reveal), where it pushes, grows or uncovers things; it is
uncountable, so a character uses Psynergy, never "a Psynergy". Each use costs
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

Each fighter has a Power and a Resist in each element: Power raises the damage
and healing of what it casts in that element, and Resist lowers the damage it
takes from it ([Battles](src/battle/README.md#the-element-multiplier)).

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
`BUG` and `BUGFIX` markers. Its words are not a source for Alchemy's: the
names of things are Camelot's where Camelot has one.

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

`// BUG:` marks a bug of the original game, kept as it is, with a fix behind
`BUGFIX`; `// UB:` marks undefined behaviour kept in the form that matches,
with the defined form behind `UBFIX`
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

## The system

### System

The code the field, the battles and the user interface are built on: start-up,
the sequence parent, waiting and [tasks](#task), interrupts, the
[heap](#heap-and-pools), [banks](#bank), [overlays](#overlay), saving and the
sound driver. It is one of the code's four divisions, with Field, Battle and
User Interface; [System](system.md) explains it.

Evidence: Camelot: the credits, "Programming", then "System", "Battle",
"Field", "User Interface" and "Support" (`gCredits_Lines`).

### Sequence parent

The loop that runs one [sequence](#sequence) after another for as long as the
console is on, `SequenceParent_Run`. Each pass reads one id from the
[sequence parent's work](#sequence-parents-work) and runs one
thing: a [scene](#scene), any row of the scene table, the Title and the File
Screen included, which the
[scene sequence](#scene-sequence) runs until the party takes an exit; or the
battle, dice or slots sequence. In each transition it tears down what the last
sequence set up (its tasks, its interrupt handlers, the heap) and starts the
next, so going from one scene to the next is a transition just as going into a
battle is ([The front page](README.md#the-sequence-parent)).

Evidence: Sega: a coinage from its lineage. "Sequence" for one flow of the
game is Namco's (黒須一雄; 遠藤雅伸's ゲームシーケンス) and Sega's through
平山尚 (ゲームプログラマになる前に覚えておきたい技術, 2008, from Sega's course
for new programmers), and "parent" only 平山尚's `Sequence::Parent`; no source
names Camelot's routine.

### Sequence

One flow of the game, run from its start to its end: the scene sequence, the
battle sequence, the dice sequence (Lucky Dice) or the slots sequence (Lucky
Wheels), `SEQUENCE_` in code. One runs at a time
([The front page](README.md#the-sequence-parent)).

Evidence: Sega: 平山尚's sequences (2008). Camelot uses プログラム only for the
whole game's code.

### Transition and next sequence

The handover from one sequence to the next. A sequence that ends names the
next sequence in the [sequence parent's work](#sequence-parents-work), and the
sequence parent starts it
([The front page](README.md#how-a-sequence-finds-its-way-back)).

Evidence: Sega: 平山尚's シーケンス遷移 and its 遷移判定 (2008).

### Sequence parent's work

The one struct every sequence shares and that outlives each of them,
`gSequenceParentWork` (`struct SequenceParentWork`, `sequence_parent_work.h`).
It holds the sequence parent's own transition data (the scene and entrance it
runs next, what the last sequence reported in `sequenceResult`, the return
point, the after-battle and defeat destinations and the restart point) and
what every sequence shares: the party, where it stands, the coins, the Djinn,
the play time and the options. It is a fixed struct in EWRAM, never freed and
in no [heap slot](#heap-slot). Save Game writes it to the cartridge with the
game flags ([The front page](README.md#how-a-sequence-finds-its-way-back)).

Evidence: Sega: 平山尚's `Sequence::Parent` keeps what its children share
(2008); work: working memory, as the [work](#work) entry says. The game's
"Save your adventure?" (`MsgSaveAdventureQuestion`) names what the player
saves, not a memory structure.

### Frame

One step of the game: every [task](#task) in the frame pass runs once and one
picture is prepared, then the game halts until the next [VBlank](#vblank)
shows it. Code lets time pass with `wait(n)`, and every wait, walk and
animation is counted in frames
([The front page](README.md#waiting-in-place)).

Evidence: tradition: フレーム for one pass of a game's processing (黒須, 田村祐樹 and
小久保, DCAJ 2008).

### VBlank

One refresh of the screen. The console draws about 59.73 pictures a second,
and after each it pauses in the vertical blank (VBlank), whose interrupt, the
VBlank interrupt, mixes the sound and reads the buttons whatever the game is
doing. The music keeps time in VBlanks ([System](system.md)).

Evidence: tradition: V ブランク and its interrupt (山口, DCAJ 2009), the
hardware's own name.

### Slowdown

A frame that overruns one VBlank and lasts two or more: every wait,
walk and animation slows down together, while the music keeps its tempo
([The front page](README.md#when-a-frame-runs-long)).

Evidence: tradition: 処理落ち (山口, DCAJ 2009).

### Wait

Letting frames pass where the code stands: `wait(n)` returns to the
next line once n frames have run
([System](system.md#waiting-and-tasks)).

Evidence: tradition: 黒須's WAIT and 田村祐樹's `wait()` (DCAJ 2008).

### Task

A short routine run once every [frame](#frame), or once every
[VBlank](#vblank), until it is removed: the windows' typing, the map's
scrolling, the lightning over Vale on the stormy night. The task table,
`gTasks`, holds up to 20, each with its function (`func`) and a priority that
picks its pass, the frame pass (`TASK_PASS_FRAME`, which `wait()` runs) or
the VBlank pass (`TASK_PASS_VBLANK`, which the VBlank interrupt runs), and its
order within it. A task carries no memory of its own: what it works on is a
[work](#work) ([Runtime](src/runtime/README.md#the-task-runner)).

Evidence: tradition: タスク and its 実行関数 (田村祐樹, DCAJ 2008).

### Work

The working memory a piece of code keeps its state in, held in a
[heap slot](#heap-slot) and named `g<Slot>Work`: the field's is `gFieldWork`,
the menus' `gMenuWork`. The sequence parent's is the one kept in a fixed
struct instead, [`gSequenceParentWork`](#sequence-parents-work).

Evidence: tradition, extended: ワーク names working memory in general in the
console's own Work RAM (GBATEK's On-board and On-chip Work RAM), 山口's
ワークエリア for the RAM a program can use (DCAJ 2009) and the システムワーク領域
of BASIC or a monitor program, a system's own work area (DCAJ 2009); 田村祐樹
uses it for one task's memory, its 汎用ワーク (DCAJ 2008). A block kept by a
whole system, as `gFieldWork` is, extends that word; no source uses it so.

### IWRAM and EWRAM

The console's two areas of RAM: IWRAM, 32 KB inside the processor and the
fastest it has, and EWRAM, 256 KB beside it and slower
([Cartridge and memory](src/cartridge/README.md#memory-map)).

Evidence: tradition: the hardware's own names.

### Heap and pools

The heap lends blocks of memory from two pools, one in IWRAM and one in EWRAM;
blocks go back in the reverse of the order they were taken, and the whole heap
is emptied before each sequence ([Runtime](src/runtime/README.md#the-heap)).

Evidence: plain English.

### Heap slot

One of 62 numbered places that each hold a heap block other code has to find
again, such as the running scene's [work](#work) (`gHeapSlots`).

Evidence: plain English; the project's own word.

### Main image

The code the processor runs straight from the cartridge, about 1 MB in
[banks](#bank); the rest runs from RAM, a scene's [overlay](#overlay) in EWRAM
and the fast routines in IWRAM.

Evidence: plain English; the project's own word.

### Bank

One of the fourteen stretches the main image is cut into, each at a fixed
address with room to spare and opening with a [far-call](#far-call) table, so
one bank can grow without moving anything another depends on
([Cartridge and memory](src/cartridge/README.md#why-the-main-image-is-cut-into-banks)).
Every bank is always in place; none is switched in or out.

Evidence: plain English; the project's own word.

### Far call

A call from one bank into another, through an 8-byte jump in the callee's
jump table named after the function with `Far` added: `Battle_RunFar` leads to
`Battle_Run` ([System](system.md#banks-and-far-calls)).

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
([System](system.md#overlays)).

Evidence: tradition: the GNU linker's word.

### Entry table and import table

The two tables an overlay meets the main image through: the entry table opens
it with six veneers the field calls to ask the scene its questions
(`struct SceneEntryPoints`), and the import table ends it with one veneer for each
main-image function it calls
([Cartridge and memory](src/cartridge/README.md#how-an-overlay-reaches-the-rest-of-the-game)).

Evidence: plain English.

### Resource directory

The one table of every packed thing on the cartridge, an overlay, a map or a
picture, found by its row
([Cartridge and memory](src/cartridge/README.md#the-resource-directory-and-table-order)).

Evidence: plain English.

### File and part

A file is one of the three saves on the cartridge. Each is kept as two parts,
and each copy of a part sits in one 4 KB sector of the flash chip
([Runtime](src/runtime/README.md#saving)).

Evidence: Camelot: "Please choose a file." (きろくばしょを えらんでください); "part"
is plain English, and "sector" the flash chip's.

### Background layer

One of the console's four tiled pictures, `BG0` to `BG3`, each scrolling as a
whole; a town is drawn on three of them, the world map on the two the hardware
can scale and rotate ([Field](field.md#map-layers)).

Evidence: tradition: the hardware's BG.

### Sprite and OAM

A sprite is a small picture the hardware draws over or between the background
layers; OAM, the object attribute memory, is the hardware's list of them,
rebuilt every frame ([Runtime](src/runtime/README.md#the-frame-loop)).

Evidence: tradition: スプライト and the hardware's OAM.

### Sprite animation

One of the up to four pictures laid over each other in a sprite, each playing
its own run of pictures (`struct SpriteAnimation`): Isaac's body, his weapon
and his shadow are three sprite animations of one sprite
([Field](src/field/README.md#sprites-and-their-animation)).

Evidence: plain English. No word of the time names one layer of a sprite.

### Picture

One drawing in a sprite animation or a tile animation, shown for a number of
frames before the next ([Field](src/field/README.md#sprites-and-their-animation)).

Evidence: plain English. The time's パターン and コマ each mean other things too.

### Palette number

Which of the sixteen 16-colour palettes a tile or a sprite is drawn in, the
4-bit number the hardware reads beside each tile
([Graphics](src/graphics/README.md#the-palette-fade)).

Evidence: plain English for the hardware's own field.

### Palette fade

The change of every colour on screen, backgrounds and sprites alike, an equal
step each frame towards a target, so that they all arrive together; it brings
scenes in from black and takes them out again
([Graphics](src/graphics/README.md#the-palette-fade)).

Evidence: plain English.

### Line scroll

A table that gives each of the screen's 160 lines its own background scroll,
written into the scroll registers line by line by HBlank DMA; Mind Read's wave,
the Lamakan Desert and the Lemurian Ship's ripple bend a layer with it
(`struct LineScrollWork`, `LineScroll_`) ([Field](field.md#field-psynergy)).

Evidence: Sega: SGL's ラインスクロール.

### Sound

Every sound the game plays, music, a jingle or a sound effect (SE), is asked
for by a sound number: below 80 music, from 80 the jingles, and from 100 the
sound effects ([Sound](src/sound/README.md#what-a-sound-number-means)). The
sound driver plays them, and calls each one's data a song.

Evidence: Camelot: the credits' "Music" and "Sound Effects", and The Lost
Age's sound test's "Music", "Jingle" and "SE".

## The field

### Field

Everywhere the party walks, a town, a dungeon or the World Map, and the code
that runs it ([Field](field.md)).

Evidence: Camelot: the credits' "Field", and せんとう&フィールド
(`MsgUsableAnywhere`).

### Scene

One row of the scene table (`struct Scene` in `gScenes`): a room, a town, a
dungeon floor, the World Map, or a screen such as the Title or the File
Screen, 201 in all, each running an overlay's code and drawn on a map
([The front page](README.md#scenes-and-overlays)).

Evidence: tradition: シーン, a screen of a game; the row is the project's.
Camelot labels each row (TITLE, UTIL, WORLD, `MsgDebugCategoryName`).

### Scene sequence

The sequence that runs one scene, `SceneSequence_Run`: a frame of its tasks,
then the field requests, until the party takes an exit. The next scene is the
sequence parent's to start
([The front page](README.md#the-scene-sequence)).

Evidence: Sega: 平山尚's sequence (2008), named for what it runs.

### Room

A scene inside a building, a house, a shop or an inn.

Evidence: Camelot: the scene labels' ROOM and RM (HAIROOM1, KUP_RM01,
FUCHINRM).

### Area

A group of scenes, 71 in all, sharing their music, their area name and one
range of [scratch flags](#game-flag)
([The front page](README.md#the-scene-table-and-areas)). The game names each
by its area name, the one the debug menu lists and the area name window shows.

Evidence: Camelot: the area names.

### Map

What a scene shows and stands on: three background layers and a grid of cells
under them, 128 by 128; several scenes can share one ([Maps](src/map/README.md)).

Evidence: Camelot: the credits' "Maps".

### Map characters

The field's people and things as drawn, the sprite sheets the actors show
([Field](src/field/README.md#sprites-and-their-animation)).

Evidence: Camelot: the credits' "Map Characters".

### Still picture

The World Map's tilted view drawn once into an ordinary background picture,
so that a menu's or an event's windows can use the layer the tilt was using
(`WorldMap_StartStillPicture`) ([Field](field.md#the-world-map)). The map R
shows of the whole world is the world picture, a screen of its own
([Running a scene](src/overworld/README.md#the-world-picture)).

Evidence: plain English for what the player sees, the view standing still.

### Sheet of rooms

A map that packs [rooms](#room) drawn in one tile style side by side, each its
own scene with a camera box its entrance chooses; its rooms may belong
to different places, so it is named for the style they are drawn in, as
Cottage rooms or Stucco rooms ([Scenes](src/scene/README.md#design)).

Evidence: plain English; the project's own word.

### Cell

One 16 by 16 pixel square of a map: which cell picture is drawn there, the
shape and height of the ground, and its cell behavior
([Maps](src/map/README.md#cells)).

Evidence: plain English.

### Cell picture

The picture of one cell: 2 by 2 of the console's 8-pixel tiles.

Evidence: plain English. No word of the time names the 16-pixel unit, and
Sega's "cell" is the 8-pixel tile.

### Cell behavior

The byte of a cell that says what standing on it, walking into it or searching
it does: a wall, a ledge, a jar. One range of behaviors means whatever the
scene's own trigger table says
([Field](field.md#cells-and-cell-behaviors)).

Evidence: plain English. Camelot's 属性 (attribute) means an element.

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
(`ACTOR_ID_COUNT` of them): 0 to 7 the party, the leader at 0, and from 8
(`ACTOR_FIRST_PLACED`) the actor placements' rows in order; each scene names
its own in an enum.

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

Evidence: tradition: スクリプト for a character's commands (小久保, DCAJ 2008);
"built-in script" is plain English.

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

Evidence: tradition: the Western word (QuakeC, Unreal).

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
scene and entrance
([The front page](README.md#exits-and-entrances)).

Evidence: Camelot: the game's text, 出口.

### Entrance

Where the party arrives in a scene, named for where it comes from
(`VALE_PLAZA_FROM_SANCTUM`): a position, a facing and the camera's limits
([The front page](README.md#exits-and-entrances)).

Evidence: Camelot: the game's text, 入口.

### Field request

What the scene sequence answers once the leader stops walking: one of twelve
`FieldWork` members asks for an exit, an encounter, a talk, an examine, an
item, a field Psynergy, a menu or a raised trigger, and the scene sequence
serves them most urgent first ([Field](field.md#the-scene-sequence-and-its-requests)).

Evidence: plain English; the project's own word.

### Event

The C routine a trigger runs, named for its scene (`Vale_EnterIsaacsHouse`):
a cutscene, or a conversation that does more than show one message. It runs
between `Event_Start` and `Event_End`, one step to a line, each step that
takes time running its frames before the next starts. A trigger row that
only shows a message runs no event ([Scenes](src/scene/README.md#events)).

Evidence: Camelot: the Shining Force III credits' "Events", by two of Golden
Sun's field programmers.

### Event command

A routine an event calls: `Event_` for speech, waits and destinations,
`Actor_` for actors and `FieldCamera_` for the camera. An event calls most of
them through one inline each in `event_inline.h`, beside `event.h`
([Scenes](src/scene/README.md#events)).

Evidence: plain English.

### Destination

The scene and entrance the sequence parent runs next (`gSequenceParentWork.sceneId`
and `entrance`): an exit sets it, and an event can set it itself
(`Event_SetDestination`) or set it and leave at once (`Event_LeaveFor`).
Three others are read only at given moments: after a battle the party goes to
the after-battle destination when it won and one is set, else back to the
return point, the scene it left; after a defeat it goes to the defeat
destination when one is set, else to the restart point, usually the last
Sanctum ([The front page](README.md#exits-and-entrances)).

Evidence: plain English.

### Game flag

One of the 4,096 yes-or-no facts saved with the game, such as a chest being
open; two ranges are scratch space, cleared as the party enters scenes and
areas ([The front page](README.md#game-flags)).

Evidence: Camelot: 田口泰宏, the System programmer, speaks of flags (Shining
Force III, 1998).

### Debug menus

What the released game keeps for its makers and never shows: the debug scene
selector and the test rooms it reaches, and, in debug mode, the Pause menu's
Rough menu, Recovery test and Restart quest
([Debug rooms](src/debug/README.md#how-the-rooms-are-reached)).

Evidence: Camelot: the menus' own labels, "Rough menu"
(`MsgWorkspaceRoughMenu`) and "Recovery test", and the scene labels TEST0 to
TEST7.

## The battle

### Battle

The fight between the party and a formation of enemies, a sequence of its own
([Battle](battle.md)).

Evidence: Camelot: the credits' "Battle".

### Unit

Either side's fighter in a battle (`struct Unit`); a party member is a unit
in the party ([Battle](battle.md#units-and-abilities)).

Evidence: plain English. The game's キャラ means only a party member.

### Formation

One group of enemies a battle is fought against, a row of
`src/battle/formations.c`, 380 in all
([Battle](battle.md#starting-a-battle)).

Evidence: plain English.

### Turns and actions

A turn is every unit in a battle acting once; an action is one unit's go
within it, chosen as a `struct BattleAction` and carried out as a
`struct BattleActionRun` ([Battles](src/battle/README.md#turns)).

Evidence: Camelot: "HP recovery / turn" (かいふくHP/ターン), and 行動 for one
unit's go ("つぎに2かい行動する").

### Staging

How an action is shown, apart from what it does: who steps forward, what is
announced and which battle effect plays (`gAbilityStaging`)
([Battles](src/battle/README.md#staging-and-announcement)).

Evidence: tradition: 演出 (DCAJ 2008).

### Playback queue

The list of what each target's rules left to show, the flinch, the message,
the falling HP, which a task plays once the battle effect says the blow has
landed ([Battles](src/battle/README.md#rules-and-pictures-meet-at-the-playback-queue)).

Evidence: plain English.

### Battle effect

The picture an ability plays over a battle, drawn by code: Quake's shaking
ground, a summon's arrival. It decides when a hit shows, never what it does
([Battle effects](src/battle_effect/README.md)).

Evidence: Camelot: the credits' "Battle Effects", beside "Battle Characters"
and "Battle Backgrounds".

## The user interface

### User Interface

The windows, the messages typed into them, the menus and the counters of the
shops, sanctums and inns ([User Interface](user_interface.md)).

Evidence: Camelot: the credits' "User Interface".

### Window

A bordered box the game opens for text or choices, growing out of its centre
(`struct Window`) ([Windows](src/window/README.md#windows)).

Evidence: Camelot: ウインドウ, as in ウインドウカラー, "Window color" (`MsgWindowColorLabel`).

### Border

The edge drawn around a window (`Window_DrawBorder`)
([User Interface](user_interface.md#windows)).

Evidence: plain English.

### Message

The text the game types into a window a few letters each frame, with the
speaker's voice blip, waiting for a button at each page break. Up to three are
typed at once (`struct Message`, `Message_Type`)
([Windows](src/window/README.md#typing)).

Evidence: Camelot: メッセージスピード (`MsgMessageSpeedLabel`), "Message speed" in the English settings menu.

### The screens

The Title; the File Screen, where a file is chosen, copied or erased; the
Pause menu, which Start opens on the field; the field menu with its
Psynergy, Djinn, Item and Status screens; the settings menu; name entry; and
the world picture ([User Interface](user_interface.md)).

Evidence: Camelot: "To the File Screen", "The Pause menu is not..."
(`MsgFieldPauseUnavailable`), the scene label TITLE; the rest plain English.
