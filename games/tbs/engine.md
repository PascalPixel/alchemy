# Engine

![The console redraws its screen about sixty times a second, each picture a frame. It draws a picture line by line, from the top of the screen to the bottom, 160 lines. After the last line comes a pause about as long as 68 more lines, the vertical blank, when nothing is drawn. Then the next picture starts at the top. Changing the picture while it is being drawn would show half the change, so the game does its work during the frame and hands the finished picture to the hardware in the blank. On a timeline, each frame is a long stretch of drawing followed by a short blank, over and over.](https://alchemy.pascalpixel.com/illustrations/engine/frame-rate.svg)

Everything the player sees moves to one beat: the screen is redrawn about
sixty times a second, and the game prepares one picture for each. The engine
is the code that keeps that beat and that the field, the battles and the
menus are built on. It starts the console, runs every [frame](glossary.md#frame) and hands the
finished picture over in the screen's blank moment, lets code wait without
the screen freezing, splits the program into [banks](glossary.md#bank) and swaps each [map](glossary.md#map)'s
code in and out of memory, and plays the music and sounds.

## Start-up

Start-up takes the console from the moment it is switched on to the title
screen, and never comes back.

![Start-up, in order. The cartridge's first instruction jumps to System_Entry, which sets up the stacks. System_Initialize then prepares memory: it clears IWRAM, empties the heap and copies the fast routines into IWRAM. It starts the services: the VBlank handler, the sound driver and an empty task table. It waits ten frames, then Game_Run runs one map after another, starting with the title. Before each map it drops the last map's tasks and empties the heap. None of these steps returns.](https://alchemy.pascalpixel.com/illustrations/runtime/start-up.svg)

Memory is put in order and the services everything relies on are started
once; after that one loop runs map after map for as long as the console
is on.

The console has two kinds of work memory: [IWRAM](glossary.md#iwram-and-ewram), 32 KB inside the processor
chip and the fastest memory it has, and EWRAM, 256 KB beside it and slower.
The [heap](glossary.md#heap-and-pools) is the part of that memory from which code borrows blocks for its
state and gives them back. A stack is the memory a function's local values
live in while it runs, and an interrupt is the hardware stopping the
program to run a handler, as it does at the end of every frame.

The cartridge header jumps to `System_Entry` (`src/system/entry.s`), which
sets up the interrupt and system stacks at the top of IWRAM and calls
`System_Initialize`. That function clears IWRAM below the stacks, empties the
heap, copies the resident runtime (the routines that must run fast) into
IWRAM and points the interrupt vector at its dispatcher, installs `System_VBlankHandler`, starts the sound driver,
resets the [task](glossary.md#task) table, waits ten frames and calls `Game_RunFar`, the jump in
the object bank's [far-call](glossary.md#far-call) table that leads to `Game_Run`
([Banks and far calls](#banks-and-far-calls) explains them). Neither
function returns. `Game_Run` loads one map after another for as long as the
game runs, starting with the title. A soft reset (A, B, Select and Start held
together) sets `gResetRequested`, and the frame loop restarts the game from
the cartridge's entry point. The title's intro, with the Nintendo and
[Camelot](glossary.md#camelot) logos before it, is code in
[Graphics](src/graphics/README.md#the-title-sequence).

[Runtime](src/runtime/README.md#start-up) goes through start-up step by step.

## Waiting and tasks

Code lets time pass by calling `WaitFrames`, and work that must go on while it
waits runs as a task the scheduler calls every frame.

![Five frames of a field map. A script's code does some work in the first frame, then calls WaitFrames to wait three frames, and carries on in the fifth. Meanwhile two tasks run once in every one of the five frames, waiting or not: the windows' task opens, closes and types the text windows, and the map's task scrolls the map's layers with the camera. WaitFrames is what runs them.](https://alchemy.pascalpixel.com/illustrations/runtime/waiting.svg)

When a cutscene pauses, the code that runs it simply waits; the screen keeps
moving because a short list of jobs, the tasks, runs once every frame
whatever that code is doing. There is no main loop that calls each part of
the game once a frame. A menu screen, a cutscene or a battle effect is an
ordinary function with a loop of its own, and when time has to pass it calls
`WaitFrames(n)` and continues after `n` frames.

Each frame, `WaitFrames` runs the main-loop tasks
(`Scheduler_RunPass(SCHEDULER_KEY_MAIN_LOOP)`), builds the [sprite](glossary.md#sprite-and-oam) list
(`Render_BuildOamList`: the position, size and picture of every sprite to
draw, which the hardware keeps in its OAM, the object attribute memory) and
halts the CPU until VBlank, the pause after the last line of the picture.
The VBlank handler then copies the sprite list and the layers' scroll
positions to the hardware, carries out
the queued register writes and runs the VBlank tasks. A task is a routine
added with `Scheduler_AddOrUpdateCallback`; window animation, battle event
playback (`BattleEvent_Playback`) and a map's callbacks are tasks.

This design puts a bound on the stack. The system stack is 1,792 bytes of
IWRAM, and the interrupt handlers and the sound mixer run on it too. When the
stack pointer is below `Ram_FrameWaitStackTop`, 512 bytes above the heap's
end, `WaitFrames` copies everything from the stack pointer up to that top into
`gSavedStack` in EWRAM, runs its frames from the top and copies the stack back
on return, so tasks always start with the same 512 bytes. It also means a task
must not call `WaitFrames`: the nested call would overwrite the outer caller's
saved stack. Nothing guards against it, and no task does.

| Name | Value | Meaning |
| --- | --- | --- |
| `SCHEDULER_KEY_MAIN_LOOP` | 3200 (`0xc80`) | the pass `WaitFrames` runs each frame |
| `SCHEDULER_KEY_VBLANK` | 1152 (`0x480`) | the pass the VBlank handler runs |
| `Ram_IwramHeapEnd` | `0x03007800` | the end of the IWRAM heap, as an address in IWRAM |
| `Ram_FrameWaitStackTop` | `0x03007a00` | the stack top `WaitFrames` runs its frames from, 512 bytes above the heap's end |

[Runtime](src/runtime/README.md#the-frame-loop) has the frame's order,
[the stack switch](src/runtime/README.md#the-stack-switch-in-waitframes) and
[the task scheduler](src/runtime/README.md#the-task-scheduler).

## Banks and far calls

The [main image](glossary.md#main-image)'s code is laid out in banks at fixed addresses, and a call
from one bank to another goes through an 8-byte jump in the callee's far-call
table.

![Two banks of the program as they lie in the cartridge, each starting at a fixed address. The sound bank starts with its far-call table, one short entry for each of its functions that other banks call, followed by its code and then empty room up to the next fixed start. A function in the field bank that plays a sound calls the table's entry, AudioCommand_PlayFar, which jumps on to AudioCommand_Play wherever it lies in the sound bank. If the sound bank's code changes size, AudioCommand_Play moves, but the table entry stays where it is, so nothing in the field bank changes.](https://alchemy.pascalpixel.com/illustrations/system/far-calls.svg)

The program is cut into banks so that one bank's code can change size without
moving anything another bank depends on. Each bank starts at a fixed address
with room to spare, and opens with a table of short jumps, one for each
function other banks call. A call from another bank goes to the jump, and the
jump goes on to the function wherever it is.

`main.ld` places the main image's code in banks (`runtime`, `field`,
`window`, `game` and the rest), each at an address fixed per [edition](glossary.md#editions), with
unused space after it. A caller in another bank calls the table entry
(`AudioCommand_PlayFar` for `AudioCommand_Play`) rather than the function's
own address. The runtime bank is the exception: every bank calls its
functions directly, and its table (`WaitFramesFar` for `WaitFrames`) serves
the [overlays](glossary.md#overlay).

Distance isn't the reason. The processor's call instruction (a Thumb `bl`)
reaches 4 MB either way, and the farthest call in the program, from the
first bank to the resource bank's table after the sound code and the music,
spans a little over 1.5 MB; the fourteen banks themselves take about 1 MB.
How Camelot built the banks is an open question (below).
[System](src/system/README.md#why-the-program-is-cut-into-banks) has the
memory map and the per-edition bank addresses.

## Overlays

Each map's code is an overlay, unpacked into one fixed area of EWRAM when
the map loads, and it meets the main image only through two tables.

![The cartridge holds each map's code packed, as one of 96 overlays: Vale, Vault, Bilibin, Bilibin's houses, Kolima, Imil and the rest. In RAM there is room for one, the 32 KB overlay area. While the party is in Bilibin, Bilibin's overlay is unpacked there. When the party walks into one of Bilibin's houses, the new map's overlay, the houses', is unpacked into the same area over Bilibin's, which is gone until the party walks back out.](https://alchemy.pascalpixel.com/illustrations/system/overlays.svg)

A town's people, [scripts](glossary.md#script) and cutscenes are code that is needed only while the
[party](glossary.md#the-party) is there, and all the maps' code together would not fit in RAM. So
each map's code is packed on the cartridge and unpacked into the same
32 KB area when the party arrives, over the map before it.

An overlay is a compressed resource unpacked into `gOverlayArea`, 32 KB of
EWRAM. There are 96 of them, listed in the map table (`src/field/maps.s`).
Only one is loaded at a time, so overlays never call each other, and code
several maps need is copied into each of them (`src/map/common/`).

An overlay and the main image meet at two tables, each in a fixed order:

- The [entry table](glossary.md#entry-table-and-import-table) (`map/entry/<map>.s`) opens the overlay with six stubs
  the field code calls through `struct MapEntries`: the map's start-up
  and the functions that return its [warp destinations](glossary.md#warp-destination), [warps](glossary.md#warp), [object event](glossary.md#object-event)
  templates, [map events](glossary.md#map-event) and warp regions.
- The import table (`map/import/<map>.s`) holds the main-image functions
  the overlay calls. EWRAM is out of `bl` range of the ROM, so each call goes
  through a stub holding the address of the function's far-call entry, never
  the function itself; an overlay therefore depends only on positions in the
  far-call tables.

[System](src/system/README.md#how-an-overlay-reaches-the-rest-of-the-game)
describes both tables and how `Resource_LoadCode` restores the overlay's
branches as it unpacks it.

## Sound cues

Every sound is played by `AudioCommand_Play` with a cue number, whose range
decides whether it is music, a fanfare or a sound from the song table.

![Every sound starts as one call with a cue number, and the number's range decides what it is. Cues below 80 are music, played on the music player; asking for the music already chosen does nothing. Cues from 80 to 99 are fanfares, played on the fanfare player while the music is silenced. Cues from 100 up are rows of the song table, mostly sound effects, each played on the player its row names. Three cues are controls instead of sounds: cue 17 fades the music out, cue 18 keeps the music that is playing, and cue 289 fades out player 3.](https://alchemy.pascalpixel.com/illustrations/sound/cue-ranges.svg)

The game asks for every sound the same way, with one number, and the number
says what kind of sound it is. The sound driver has eight players, and each
kind of sound has players of its own, so a sound effect never stops the
music.

Below 80 a cue is music; from 80 to 99 (`AUDIO_JINGLE_FIRST`) a fanfare;
from 100 (`AUDIO_SONG_FIRST`) an entry of the song table, played on the
player its row names, which is how sound effects play. Cues 17, 18 and 289
are controls rather than sounds.

A fanfare doesn't stop the music. The music's volume drops to 0 while it keeps
playing, the fanfare plays on player 0, and once that player stops the music
fades back in over 64 frames, so the code that asked for the fanfare doesn't
have to track it. The driver is Nintendo's MusicPlayer2000.

| Name | Value | Meaning |
| --- | --- | --- |
| `AUDIO_CUE_FADE_BGM` | 17 | fade the music out over 112 frames |
| `AUDIO_CUE_KEEP_BGM` | 18 | keep the music that is playing |
| `AUDIO_JINGLE_FIRST` | 80 | first fanfare cue; below it, music |
| `AUDIO_SONG_FIRST` | 100 | first cue that is a song table row |
| `AUDIO_CUE_FADE_SLOT3` | `0x121` (289) | fade player 3 out over 48 frames |

[Sound](src/sound/README.md#what-a-cue-number-means) covers the players,
[the fanfare's return](src/sound/README.md#the-fanfares-return-to-the-music)
and [the driver's lock](src/sound/README.md#the-drivers-lock).

## Debug rooms

Three test maps kept in the ROM are reached only from the debug map
selector.

![No door, table or script of the game leads to the three test rooms. The only way in is the debug map selector, which warps to any map by number, and the field opens it only when the Start menu is opened with B held while debug mode is on. Debug mode is switched off at power-on, and nothing the player can reach switches it on, so in the released game the selector, and the rooms behind it, are never reached. From the selector come the Djinn room, which eight maps share, the level room and the menu test room.](https://alchemy.pascalpixel.com/illustrations/debug/reached.svg)

The ROM keeps three test maps: a room that hands out [Djinn](glossary.md#djinn), a room for
levels and for browsing items, icons and letters, and a room of shop and
message tests. Only the debug map selector reaches them, and only while
`gDebugMode` is set, which the released game never does. Each is an ordinary
map overlay, so they're the smallest complete examples of how a map is
built. [Debug rooms](src/debug/README.md#how-the-rooms-are-reached) goes
through them.

## Limits

- The frame loop, its tasks and the VBlank work share 512 bytes of stack, and
  a task must not call `WaitFrames`.
- 20 scheduled tasks at a time.
- An overlay has the 32 KB of the overlay area, and a bank the space up to
  the next bank's fixed start.
- Eight sound players; cues are 12 bits, 0 to 4,095.

## Open questions

- XXX: likely Camelot linked each bank on its own against the others'
  tables, because fixed starts and tables are what that would need; the ROM
  shows only the result.
