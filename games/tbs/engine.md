# Engine

Everything the player sees moves to one beat: the screen is redrawn about
sixty times a second, and the game prepares one picture for each. The engine
is the code that keeps that beat and that the field, the battles and the menus
are built on. It starts the console, lets code wait without the screen
freezing, splits the program into [banks](glossary.md#bank), swaps each
[map](glossary.md#map)'s code in and out of memory and plays the music.

The console draws each picture one line at a time, 160 lines from top to
bottom, then pauses for about as long as 68 more lines, the vertical blank
(VBlank). Changing the picture while it is being drawn would show half the
change, so the game does its work during the [frame](glossary.md#frame) and
hands the finished picture over in the blank.

## Start-up

![The console switched on: Camelot's logo, then the Golden Sun logo](https://alchemy.pascalpixel.com/guides/engine/boot.gif)

Between switching on and the title, the game puts memory in order and starts
the services everything else relies on: the code that hands each frame to the
screen, the sound and the list of tasks. That happens once; after it, one loop
runs map after map for as long as the console is on.

The cartridge header jumps to `System_Entry` (`src/system/entry.s`), which
sets up the stacks at the top of [IWRAM](glossary.md#iwram-and-ewram) and
calls `System_Initialize`. That clears IWRAM and empties the
[heap](glossary.md#heap-and-pools), copies the routines that must run fast
into IWRAM, installs `System_VBlankHandler`, starts the sound driver, empties
the [task](glossary.md#task) table, waits ten frames and calls `Game_Run`
through its [far-call](glossary.md#far-call) entry, `Game_RunFar`. `Game_Run`
is the loop of maps on the [front page](README.md#the-main-loop) and never
returns. A soft reset jumps to `Cart_Restart`, the cartridge's first
instruction, and start-up runs again. The logos are drawn by
[the title sequence](src/graphics/README.md#the-title-sequence);
[Runtime](src/runtime/README.md#start-up) goes through start-up step by step.

## Waiting and tasks

![Isaac talks to the strongman by Vale's river: a window opens and types out "I don't blame Felix", its red arrow bobbing while it waits; A brings a second page, and the window closes](https://alchemy.pascalpixel.com/guides/engine/waiting-talk.gif)

While a line of dialogue waits for A, the code that showed it simply stops
and waits, yet the arrow keeps bobbing and the river keeps rippling. The
game has no main loop that calls each part once a frame. A talk, a cutscene,
a menu screen or a battle animation is an ordinary function with a loop of its
own, and when time has to pass it calls `WaitFrames(n)`, which runs `n`
frames and then returns. What must keep moving meanwhile is a task, a short
job run once every frame whatever the waiting code is doing.

`WaitFrames` runs the frame-work tasks, builds the frame's
[sprite](glossary.md#sprite-and-oam) list (`Oam_BuildBuffer`) and halts until
VBlank; the VBlank handler then copies the sprites, the scroll and the queued
register writes to the hardware and runs the VBlank tasks. In the recording
the window's typing and arrow are `WindowSystem_Update` and the river's
animated tiles `TilesetAnim_Update`, both tasks. The Camelot logo at power-on
is such a waiting loop (`Title_ShowCamelotLogo`, simplified):

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

A task is a `void (*)(void)` routine added once with a priority and run every
frame until `Task_Remove` or the next map empties the table. The priority's
high byte picks the pass (`TASK_PRIORITY_MAIN_LOOP` for the frame's work,
`TASK_PRIORITY_VBLANK` for the blank) and its low byte the order within it,
higher first. A battle adds its camera at `0x7f`, below the playback and
sprites at `0x80`, so the camera moves after them (`Battle_Run`):

```c
Task_Add(BattleCamera_Update, 0xc7f);
```

Code can call `WaitFrames` from deep inside its own calls, so `WaitFrames`
first moves the part of the stack below `Ram_FrameWaitStackTop` aside: the
frame's work always has the same 512 bytes of stack however deep the caller.
A task must not call `WaitFrames`, since the nested call would overwrite the
saved stack; nothing guards against it, and no task does.

| Name | Value | Meaning |
| --- | --- | --- |
| `TASK_PRIORITY_MAIN_LOOP` | `0xc80` | a task in the frame's work, ranked in the middle |
| `TASK_PRIORITY_VBLANK` | `0x480` | a task in the vertical blank, ranked in the middle |
| `Ram_FrameWaitStackTop` | `0x03007a00` | the stack line `WaitFrames` runs its frames from |

[Runtime](src/runtime/README.md#the-frame-loop) has the frame's order,
[the stack switch](src/runtime/README.md#the-stack-switch-in-waitframes) and
[the task runner](src/runtime/README.md#the-task-runner).

## Banks and far calls

The [main image](glossary.md#main-image)'s code is laid out in fourteen
banks, each at an address fixed per [edition](glossary.md#editions) with room
to spare after it. Each bank opens with a table of short jumps, one for each
function other banks call, and a call from another bank goes to the jump,
named after the function with `Far` added, rather than to the function:

```c
Sound_PlaySongFar(MUS_TREASURE_FOUND); // the field opens a chest
```

goes to the sound bank's table, whose entry jumps on to `Sound_PlaySong`. So
one bank's code can grow or shrink without moving anything another bank
holds. Distance isn't the reason: a call instruction reaches every bank from
every other. The runtime bank is the exception: every bank calls its
functions directly, and its table (`WaitFramesFar` for `WaitFrames`) serves
only the overlays. [System](src/system/README.md#why-the-program-is-cut-into-banks)
has the per-edition addresses.

## Overlays

Each map's code is an overlay, unpacked into one 32 KB area of EWRAM,
`gOverlayArea`, when the party arrives, over the map before it. A town's
people and cutscenes matter only while the party is there, and all the maps'
code would not fit in RAM. The 201 maps share 96 overlays, since the rooms of
one dungeon often share its code; all fourteen of Crossbone Isle's maps run
one. Only one is loaded at a time, so overlays never call each other, and code
several maps need is copied into each (`src/map/common/`).

An overlay meets the main image at two tables in a fixed order:

- The [entry table](glossary.md#entry-table-and-import-table)
  (`map/entry/<map>.s`) opens it: six stubs the field calls through
  `struct MapEntries`, for the map's start-up and the functions that return
  its [warp destinations](glossary.md#warp-destination), warps,
  [object event](glossary.md#object-event) templates,
  [map events](glossary.md#map-event) and warp regions.
- The import table (`map/import/<map>.s`) holds the main-image functions it
  calls. EWRAM is beyond a `bl`'s 4 MB reach of the cartridge, so each call
  goes through a stub holding a full address: the function's far-call entry,
  which stays put while a bank's code moves.

[System](src/system/README.md#how-an-overlay-reaches-the-rest-of-the-game)
describes both tables and how `Overlay_Load` fixes the overlay's calls as it
unpacks it.

## Songs

Every sound, music or effect, is asked for with one call and a number,
`Sound_PlaySong(id)`, and the number's range says what kind of sound it is.
The sound driver has eight music players, and each kind of sound has its own,
so a sound effect never stops the music. Below 80 a song is music; from 80 to
99 (`SONG_FANFARE_FIRST`) a fanfare, which silences the music until it ends,
as when a chest is opened; from 100 (`SONG_SE_FIRST`) a row of the song table,
which is how sound effects play. Asking for the music already chosen does
nothing, so a map can name its music on every arrival. The driver is
Nintendo's MusicPlayer2000, which [pret](glossary.md#pret)'s decompilations
call m4a.

| Name | Value | Meaning |
| --- | --- | --- |
| `SONG_FADE_OUT_BGM` | 17 | fade the music out over 112 frames |
| `SONG_KEEP_BGM` | 18 | keep the music that is playing |
| `SONG_FANFARE_FIRST` | 80 | first fanfare; below it, music |
| `SONG_SE_FIRST` | 100 | first song that is a song table row |
| `SONG_FADE_OUT_ACTION_SE` | 289 | fade music player 3 out over 48 frames |

[Sound](src/sound/README.md#what-a-song-number-means) covers the music
players, [the fanfare's return](src/sound/README.md#the-fanfares-return-to-the-music)
and [the driver's ident](src/sound/README.md#the-drivers-ident).

## Debug rooms

![With debug mode switched on, Start pressed with B held in Vale opens a one-line window naming a map; Left steps the number down past the World Map and the title screen and round to the menu test room, and A warps there, and the test room's people stand on a black floor](https://alchemy.pascalpixel.com/guides/engine/debug-select.gif)

The cartridge keeps three test rooms no player can walk into: one that hands
out [Djinn](glossary.md#djinn), one for raising levels and browsing items,
and one of shop and message tests. Only the debug map selector reaches them,
and only in debug mode, which the released game never turns on. The selector
warps to any map by number; the field opens it when Start is pressed with B
held, while `gDebugMode` is set. Each room is an ordinary map overlay, so the
rooms are the smallest complete examples of a map.
[Debug rooms](src/debug/README.md#how-the-rooms-are-reached) goes through
them.

## Limits

- The frame's work, its tasks and the VBlank work share 512 bytes of stack,
  and a task must not call `WaitFrames`.
- 20 tasks at a time.
- An overlay has the 32 KB of the overlay area, and a bank the space up to the
  next bank's fixed start.
- Eight music players; song numbers are 12 bits, 0 to 4,095.
