# Engine

Everything the player sees moves to one beat: the screen is redrawn about
sixty times a second, and the game prepares one picture for each. The engine
is the code that keeps that beat and that the field, the battles and the menus
are built on. It starts the console, lets code wait without the screen
freezing, splits the program into [banks](glossary.md#bank), swaps each
[scene](glossary.md#scene)'s code in and out of memory and plays the music.

The console draws each picture one line at a time, 160 lines from top to
bottom, then pauses for about as long as 68 more lines, the vertical blank
(VBlank). Changing the picture while it is being drawn would show half the
change, so the game does its work during the [frame](glossary.md#frame) and
hands the finished picture over in the blank.

## Start-up

![The console switched on: Camelot's logo, then the Golden Sun logo](https://alchemy.pascalpixel.com/guides/engine/boot.gif)

Between switching on and the title, the game puts memory in order and starts
the services everything else relies on: the code that hands each frame to the
screen, the sound and the list of [tasks](glossary.md#task). It copies the
routines that must run fastest into the console's fast memory, waits ten
frames, and starts the loop of scenes on the [front page](README.md#the-sequence-manager),
which runs for as long as the console is on. A soft reset starts the game
again from its very first instruction, so start-up runs again in full.

The cartridge's first instruction leads to `System_Entry`, which calls
`System_Initialize`. The logos are drawn by
[the title sequence](src/graphics/README.md#the-title-sequence), and
[Runtime](src/runtime/README.md#start-up) goes through start-up step by step.

## Waiting and tasks

![Isaac talks to the strongman by Vale's river: a window opens and types out "I don't blame Felix", its red arrow bobbing while it waits; A brings a second page, and the window closes](https://alchemy.pascalpixel.com/guides/engine/waiting-talk.gif)

While a line of dialogue waits for A, the code that showed it simply stops
and waits, yet the arrow keeps bobbing and the river keeps rippling. The
game has no main loop that calls each part once a frame. A talk, an event,
a menu screen or a battle animation is an ordinary piece of code that runs
from start to finish, and when time has to pass it asks to wait a number of
frames. What must keep moving meanwhile is a task, a short job run once every
frame whatever the waiting code is doing. In the recording the window's
typing and its arrow are one task and the river's moving water another.

Each task has a priority, which says whether it runs during the frame or in
the blank, and in what order. Order matters: a battle's camera runs after the
battle's figures have moved, so that it follows where they are rather than
where they were. A wait can be asked for from deep inside other code, so the
engine sets the waiting code's memory aside while the frames pass; the tasks
always start with the same room to work in. The one rule that follows is that
a task must never wait itself, and none does.

Waiting is `WaitFrames(n)`; a task is added with `Task_Add` and stopped with
`Task_Remove`. [Runtime](src/runtime/README.md#the-frame-loop) has the
frame's order, [the stack switch](src/runtime/README.md#the-stack-switch-in-waitframes)
and [the task runner](src/runtime/README.md#the-task-runner).

## Banks and far calls

The [main image](glossary.md#main-image), the code that runs straight from
the cartridge, is laid out in fourteen banks, each starting at a fixed place
with room to spare after it. Each bank opens with a short list of entrances,
one for each of its functions that other banks call, and a call from another
bank goes through the entrance rather than straight to the function. So one
bank's code can grow or shrink without moving anything another bank relies
on. Distance isn't the reason: a call can reach every bank from every other.
The runtime bank is the exception: every bank calls its functions directly,
and its entrances serve only the scenes' code.

An entrance has the function's name with `Far` added: the field plays the
chest's fanfare with `Sound_PlaySongFar`, which leads to `Sound_PlaySong`.
[System](src/system/README.md#why-the-program-is-cut-into-banks) says more.

## Overlays

Each scene's code is an [overlay](glossary.md#overlay), unpacked into one 32 KB
area of memory when the party arrives, over the scene before it. A town's
people and events matter only while the party is there, and all the scenes'
code would not fit in memory at once. The 201 scenes share 96 overlays, since
the rooms of one dungeon often share their code: all fourteen of Crossbone
Isle's scenes run one. Only one is loaded at a time, so overlays never call
each other, and code several scenes need is copied into each.

An overlay meets the rest of the game at two tables. At its start, the
[entry table](glossary.md#entry-table-and-import-table) holds the six
questions the field asks a scene (set yourself up, where can the party arrive,
where do the exits lead, who stands here, what happens here, which areas lead
out). At its end, the import table holds one entrance for each function of
the main image the scene calls, so the scene need not know where those functions
are, only the entrances, which stay put.

The entry table is `struct SceneEntryPoints`, and `Overlay_Load` unpacks an overlay.
[System](src/system/README.md#how-an-overlay-reaches-the-rest-of-the-game)
describes both tables.

## Songs

Every sound, music or effect, is asked for with one call and a number, and the
number's range says what kind of sound it is: below 80 music, from 80 to 99 a
fanfare, such as the short tune when a chest is opened, which silences the
music until it ends, and from 100 up the sound effects. The sound driver has
eight players, and each kind of sound has its own, so a sound effect never
stops the music. Asking for the music already playing does nothing, so a scene
can name its music on every arrival without restarting it. A few numbers are
orders rather than sounds, such as "fade the music out". The driver is
Nintendo's MusicPlayer2000, which [pret](glossary.md#pret)'s decompilations
call m4a.

A sound is played with `Sound_PlaySong`; the ranges are
`SONG_FANFARE_FIRST` and `SONG_SE_FIRST`. [Sound](src/sound/README.md#what-a-song-number-means)
covers the players, [the fanfare's return](src/sound/README.md#the-fanfares-return-to-the-music)
and [the driver's ident](src/sound/README.md#the-drivers-ident).

## Debug rooms

![With debug mode switched on, Start pressed with B held in Vale opens a one-line window naming a scene; Left steps the number down past the World Map and the title screen and round to the menu test room, and An exits there, and the test room's people stand on a black floor](https://alchemy.pascalpixel.com/guides/engine/debug-select.gif)

The cartridge keeps three test rooms no player can walk into: one that hands
out [Djinn](glossary.md#djinn), one for raising levels and browsing items,
and one of shop and message tests. Only the debug scene selector reaches them,
and only in debug mode, which the released game never turns on. The selector
exits to any scene by number; the field opens it when Start is pressed with B
held. Each room is an ordinary scene overlay, so the rooms are the smallest
complete examples of a scene.
[Debug rooms](src/debug/README.md#how-the-rooms-are-reached) goes through
them.

## Limits

- The frame's work, its tasks and the VBlank work share 512 bytes of stack,
  and a task must not wait.
- 20 tasks at a time.
- A scene's code has the 32 KB of the overlay area, and a bank the space up to
  the next bank's fixed start.
- Eight music players; song numbers run from 0 to 4,095.
