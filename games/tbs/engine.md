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

The cartridge header jumps to `System_Entry` (`src/system/entry.s`), which
sets up the interrupt and system stacks at the top of [IWRAM](glossary.md#iwram-and-ewram) and calls
`System_Initialize`. That function, in order:

1. clears IWRAM below the stacks and empties the [heap](glossary.md#heap-and-pools);
2. copies the resident runtime, the routines that must run fast, into IWRAM
   and points the interrupt vector at its dispatcher;
3. installs `System_VBlankHandler`, starts the sound driver and empties the
   [task](glossary.md#task) table;
4. waits ten frames and calls `Game_Run`, the loop of maps, which never
   returns.

`Game_Run` lives in another [bank](glossary.md#bank), so start-up calls it through its
[far-call](glossary.md#far-call) entry, `Game_RunFar` ([Banks and far calls](#banks-and-far-calls)).
It starts with the title. Before each map it drops the last map's tasks,
empties the heap, unpacks the map's [overlay](glossary.md#overlay) and runs the map until the
[party](glossary.md#the-party) takes a [warp](glossary.md#warp), whose row names the next map; a battle or a
minigame runs in its place without an overlay.

A soft reset (A, B, Select and Start held together) sets `gResetRequested`;
`WaitFrames` sees it after its next frame and jumps to `Cart_Restart`, the
cartridge's first instruction, so start-up runs again from the top. The
title's intro, with the Nintendo and [Camelot](glossary.md#camelot) logos before it, is code in
[Graphics](src/graphics/README.md#the-title-sequence).

[Runtime](src/runtime/README.md#start-up) goes through start-up step by step.

## Waiting and tasks

Code lets time pass by calling `WaitFrames`, and work that must go on while it
waits runs as a task the scheduler calls every frame.

![Five frames of a field map. A script's code does some work in the first frame, then calls WaitFrames to wait three frames, and carries on in the fifth. Meanwhile two tasks run once in every one of the five frames, waiting or not: the windows' task opens, closes and types the text windows, and the map's task scrolls the map's layers with the camera. WaitFrames is what runs them.](https://alchemy.pascalpixel.com/illustrations/runtime/waiting.svg)

When a cutscene pauses, the code that runs it simply waits; the screen keeps
moving because a short list of jobs, the tasks, runs once every frame
whatever that code is doing. There is no main loop that calls each part of
the game once a frame. A menu screen, a cutscene or a battle animation is an
ordinary function with a loop of its own, and when time has to pass it calls
`WaitFrames(n)`, which runs `n` frames itself and then returns. The Camelot
logo at power-on is such a loop (`Title_ShowCamelotLogo`, simplified):

```c
for (i = 0; i < 120; i++) {
  // show the logo's next animation frame
  Dma_Set(((gFrameCount >> 3) & 3) * 0x400 + frames,
          BG_CHAR_BLOCK(1) + 0x100, DMA_COPY_WORDS(0x340 / 4), REG_DMA3);
  if (gKeysPressed & (KEY_A | KEY_START))
    break;
  WaitFrames(1);
}
```

A task is a `void (*)(void)` routine, added once with an order and run every
frame until it is removed (`Task_Remove`) or the next map
empties the table. A battle adds its camera this way (`Battle_Run`):

```c
Task_Add(BattleCamera_Update, 0xc7f);
```

The order's high byte picks the pass that runs the task, `0x0c` for the
frame's work (`TASK_PRIORITY_MAIN_LOOP`) and `0x04` for the vertical blank
(`TASK_PRIORITY_VBLANK`), and its low byte ranks it within the pass, higher
first: at `0x7f` the camera moves only after the battle's playback
and sprites, added at `0x80`, have run. Window animation, battle playback
(`BattlePlayback_Run`) and a map's callbacks are tasks too.

Each frame, `WaitFrames`:

1. runs the main-loop tasks;
2. builds the frame's [sprite](glossary.md#sprite-and-oam) list (`Oam_BuildBuffer`);
3. halts the CPU until VBlank.

The VBlank handler then copies the sprite list, the layers' scroll positions
and the register writes the frame's code queued to the hardware, and runs
the VBlank tasks.

The stack has a bound. Code may call `WaitFrames` from deep inside its own
calls, so `WaitFrames` first copies the part of the caller's stack below
`Ram_FrameWaitStackTop` aside into EWRAM, and runs its frames from that line.
The frame loop, its tasks and the VBlank work therefore always have the same
512 bytes, from the line down to the heap's end, however deep the caller. A
task must not call `WaitFrames`: the nested call would overwrite the outer
caller's saved stack. Nothing guards against it, and no task does.

| Name | Value | Meaning |
| --- | --- | --- |
| `TASK_PRIORITY_MAIN_LOOP` | `0xc80` | order of a task in the frame's work, ranked in the middle |
| `TASK_PRIORITY_VBLANK` | `0x480` | order of a task in the vertical blank, ranked in the middle |
| `Ram_FrameWaitStackTop` | `0x03007a00` | the stack line `WaitFrames` runs its frames from, 512 bytes above the heap's end |

[Runtime](src/runtime/README.md#the-frame-loop) has the frame's order,
[the stack switch](src/runtime/README.md#the-stack-switch-in-waitframes) and
[the task runner](src/runtime/README.md#the-task-runner).

## Banks and far calls

The [main image](glossary.md#main-image)'s code is laid out in banks at fixed addresses, and a call
from one bank to another goes through an 8-byte jump in the callee's far-call
table.

![Two banks of the program as they lie in the cartridge, each starting at a fixed address. The sound bank starts with its far-call table, one short entry for each of its functions that other banks call, followed by its code and then empty room up to the next fixed start. A function in the field bank that plays a sound calls the table's entry, Sound_PlaySongFar, which jumps on to Sound_PlaySong wherever it lies in the sound bank. If the sound bank's code changes size, Sound_PlaySong moves, but the table entry stays where it is, so nothing in the field bank changes.](https://alchemy.pascalpixel.com/illustrations/system/far-calls.svg)

The program is cut into banks so that one bank's code can change size without
moving anything another bank depends on. Each bank starts at a fixed address
with room to spare, and opens with a table of short jumps, one for each
function other banks call. A call from another bank goes to the jump, and the
jump goes on to the function wherever it is.

`main.ld` places the main image's code in fourteen banks (`runtime`,
`field`, `window`, `game` and the rest), each at an address fixed per
[edition](glossary.md#editions), with unused space after it. A caller in another bank calls
the table entry, named after the function with `Far` added, rather than the
function's own address:

```c
Sound_PlaySongFar(MUS_TREASURE_FOUND); // the field opens a chest
```

goes to the sound bank's table, whose entry jumps on to `Sound_PlaySong`.
The runtime bank is the exception: every bank calls its functions directly,
and its table (`WaitFramesFar` for `WaitFrames`) serves only the overlays.
Why the runtime alone may be called directly is an open question (below).

The reason for banks isn't distance: a call instruction reaches every bank
from every other, as the glossary's [bank](glossary.md#bank) entry measures.
[System](src/system/README.md#why-the-program-is-cut-into-banks) has the
memory map and the per-edition bank addresses.

## Overlays

Each map's code is an overlay, unpacked into one fixed area of EWRAM when
the map loads, and it meets the main image only through two tables.

![The cartridge holds each map's code packed, as one of 96 overlays: Vale, Vault, Bilibin, Bilibin's houses, Kolima, Imil and the rest. In RAM there is room for one, the 32 KB overlay area. While the party is in Bilibin, Bilibin's overlay is unpacked there. When the party walks into one of Bilibin's houses, the new map's overlay, the houses', is unpacked into the same area over Bilibin's, which is gone until the party walks back out.](https://alchemy.pascalpixel.com/illustrations/system/overlays.svg)

A town's people, [scripts](glossary.md#script) and cutscenes are code that is needed only while the
party is there, and all the maps' code together would not fit in RAM. So
each map's code is packed on the cartridge and unpacked into the same
32 KB area, `gOverlayArea` in EWRAM, when the party arrives, over the map
before it. The 201 maps share 96 overlays, since the rooms of one dungeon
often share its code: all fourteen of Crossbone Isle's maps run one overlay
(the map table, `src/field/maps.s`, names each map's). Only one is loaded at a time, so overlays never call each other,
and code several maps need is copied into each of them (`src/map/common/`).

An overlay and the main image meet at two tables, each in a fixed order:

- The [entry table](glossary.md#entry-table-and-import-table) (`map/entry/<map>.s`) opens the overlay with six stubs
  the field code calls through `struct MapEntries`: the map's start-up
  and the functions that return its [warp destinations](glossary.md#warp-destination), warps, [object event](glossary.md#object-event)
  templates, [map events](glossary.md#map-event) and warp regions.
- The import table (`map/import/<map>.s`) holds the main-image functions
  the overlay calls. The overlay runs from EWRAM, at addresses from
  `0x02000000`, and the cartridge starts at `0x08000000`, beyond the 4 MB a
  `bl` reaches, so each call goes through a stub holding a full address.
  That address is the function's far-call entry, never the function itself,
  so an overlay depends only on positions in the far-call tables, which stay
  put while a bank's code moves.

[System](src/system/README.md#how-an-overlay-reaches-the-rest-of-the-game)
describes both tables, and how `Overlay_Load` rewrites the overlay's
calls as it unpacks it.

## Songs

Every sound is played by `Sound_PlaySong` with a song number, whose range
decides whether it is music, a fanfare or a sound from the song table.

![Every sound starts as one call with a song number, and the number's range decides what it is. Songs below 80 are music, played on the music player; asking for the music already chosen does nothing. Songs from 80 to 99 are fanfares, played on the fanfare player while the music is silenced. Songs from 100 up are rows of the song table, mostly sound effects, each played on the player its row names. Three song numbers are controls instead of sounds: song 17 fades the music out, song 18 keeps the music that is playing, and song 289 fades out player 3.](https://alchemy.pascalpixel.com/illustrations/sound/cue-ranges.svg)

The game asks for every sound the same way, with one number, and the number
says what kind of sound it is. The sound driver has eight players, and each
kind of sound has players of its own, so a sound effect never stops the
music.

Below 80 a song is music; from 80 to 99 (`SONG_FANFARE_FIRST`) a fanfare;
from 100 (`SONG_SE_FIRST`) an entry of the song table, played on the
player its row names, which is how sound effects play. Songs 17, 18 and 289
are controls rather than sounds.

A fanfare doesn't stop the music. The music's volume drops to 0 while it keeps
playing, the fanfare plays on player 0, and once that player stops the music
fades back in over 64 frames, so the code that asked for the fanfare doesn't
have to track it: opening a chest is the one call above. The driver
underneath is Nintendo's MusicPlayer2000, the sound library many Game Boy
Advance games ship ([pret](glossary.md#pret)'s decompilations call it m4a).

| Name | Value | Meaning |
| --- | --- | --- |
| `SONG_FADE_OUT_BGM` | 17 | fade the music out over 112 frames |
| `SONG_KEEP_BGM` | 18 | keep the music that is playing |
| `SONG_FANFARE_FIRST` | 80 | first fanfare; below it, music |
| `SONG_SE_FIRST` | 100 | first song that is a song table row |
| `SONG_FADE_OUT_ACTION_SE` | `0x121` (289) | fade player 3 out over 48 frames |

[Sound](src/sound/README.md#what-a-song-number-means) covers the players,
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

- The frame loop, its tasks and the VBlank work share the 512 bytes of stack
  between `Ram_FrameWaitStackTop` and the heap's end, and a task must not call
  `WaitFrames`.
- 20 tasks at a time.
- An overlay has the 32 KB of the overlay area, and a bank the space up to
  the next bank's fixed start.
- Eight sound players; song numbers are 12 bits, 0 to 4,095.

## Open questions

- XXX: likely Camelot linked each bank on its own against the others'
  tables, because fixed starts and tables are what that would need; the ROM
  shows only the result.
- XXX (no evidence): why every bank calls the runtime bank's functions
  directly, when it reaches every other bank through its table; the ROM shows
  only the direct calls, and only Camelot's build could say how the runtime
  was linked.
