# System code

When an event pauses for half a second, the code that runs it simply waits,
yet the screen keeps moving: windows still type, the [map](../../glossary.md#map)
still scrolls with the camera. Everything in the game lets time pass the same
way, through one function, [`wait()`](../../glossary.md#wait), which runs a
short list of routines, the [tasks](../../glossary.md#task), once every
[frame](../../glossary.md#frame) while it waits. That function and what it
needs around it are the core of the [System](../../glossary.md#system), the
base every other division stands on: start-up, the
[main routine](../../glossary.md#main-routine), the frame and its handover at
VBlank, the buttons and sleep, the stack switch, the
[heap](../../glossary.md#heap-and-pools) and the task runner. Beside them sit
the services every part of the game shares: the register write queue, saving,
the link cable, the screen fades, the sprite picture cache, fixed-point maths
and the 3D transform, decompression and debug text, and the map of the memory
all of it works in, with the [overlays](../../glossary.md#overlay) each scene's
code is unpacked from.

`system.c` is one object written as parts under `system/`, in cartridge order
([Source](../README.md#objects-and-parts) says how parts are compiled); the
main routine is an object of its own, `main_routine.c`, linked first in bank
04 before the field sequence. The other files are the memory's layout
(`ewram.s`, `iwram.s`, `iwram_high.s`), the interrupt code and the routines
copied into [IWRAM](../../glossary.md#iwram-and-ewram) (`entry.s`, `irq.s`,
`irq_table.s`, `iwram_routines.s`, `iwram_entry.s`), the BIOS calls
(`bios.s`), the decompressors, the maths tables, the save format
(`savedata.c`), the sprite bank's one routine (`sprite_metadata.c`) and the
overlays' streams and link script (`overlay/`). The cartridge itself, its
header, the far-call tables of the banks, the resource directory and the flash
chip, is [The Game Pak](../gamepak/README.md).

## Start-up

Pressing A, B, Select and Start together restarts the game from the
cartridge's first instruction, as switching on does once the
console's own logo has gone. Start-up puts memory in order and starts the
services everything relies on, once, then hands over to the
main routine for as long as the console is on.

In order, start-up stops any screen effect left running, clears the fast
memory, IWRAM, below the stacks, and
empties the heap, installs the interrupt handlers and the routines that live
in IWRAM, loads the windows' graphics, sets up the code that hands each frame
to the screen, arms the reset buttons, starts the sound and empties the task
table. It then waits ten frames and starts the main routine, which never
returns (`MainRoutine_Run`). The reset buttons work through an interrupt:
pressing all four marks a reset as wanted, and the next frame carries it out.

The cartridge header jumps to `System_Entry`, which sets up the stacks and
calls `System_Initialize`; that ends with `MainRoutine_RunFar`. The reset is
`Input_HandleKeyInterrupt`.

## The main routine

Every scene, battle and minigame runs under `MainRoutine_Run`, which
`System_Initialize` starts at power-on through `MainRoutine_RunFar` and which
never returns; [System](../../system.md#the-main-routine) gives its place in
the game. Its argument, an `enum GameStart`, chooses the first scene: the
Title, or in debug mode Vale or the File Screen. Each pass of its loop does the
same work around one [sequence](../../glossary.md#sequence).

A pass opens with `SE_STOP_ACTION_SE`, a sound with no tracks that stops any
loop the last scene left playing on sound player 3, such as a rolling log.
`FLAG_SKIP_SCENE_CHANGE_SOUND`, which the World Map's lighthouse transition
and two Venus Lighthouse scenes set, skips it once and clears itself. The pass
stops DMA 0, which a scene may have left repeating for an HBlank effect, then
tears down what the last sequence left: `Task_ResetAll` empties the task
table, `Irq_SetHandler` drops the HBlank and VCount handlers,
`Heap_Initialize` empties the heap, and `DebugText_Clear` and
`VramCache_Reset` clear the debug text and the sprite VRAM cache. What lives
outside the heap survives: the
[main routine's work](../../glossary.md#main-routines-work),
`gMainRoutineWork`, the game flags, and fixed buffers such as
`gActorSnapshot`.

It then reads `gMainRoutineWork.next`. An id from `SEQUENCE_NONE` up runs
another sequence ([System](../../system.md#the-main-routine) lists them).
`MainRoutine_RunDice` and `MainRoutine_RunSlots` copy BG palettes 14 and 15
(15 holds the window colours) into a 64-byte heap block before the minigame
and back after it. Afterwards `Scene_ReturnFromSequence` chooses the scene
that comes back. After a defeat in the prologue (`FLAG_CHILDHOOD`), Isaac and
Garet are healed in full as well as the leader revived. Events set the defeat
destination with `Event_SetDefeatDestination` and the after-battle destination
with `Event_SetAfterBattleDestination`; `FLAG_KEEP_PARTY_POSITION` is set only
when no after-battle destination was given and the party goes back to the
return point. A destination may name only its scene or only its entrance; the
return point fills in the other.

Any other id is a [scene](../../glossary.md#scene), and its entry is either an
ordinary entry, one without `FLAG_KEEP_PARTY_POSITION`, or a return, with it.
Unless `FLAG_KEEP_PARTY_POSITION` is set, `Scene_Arrive` clears the flags that
last for one scene and, on entering a new [area](../../glossary.md#area), those
that last for one area
([State that cleans itself up](../scene/README.md#state-that-cleans-itself-up)
gives their ranges); a new area also shows its name, resets the counters of
random battles and hazardous ground and makes this entrance the place Retreat
leads back to. Every ordinary entry also clears the Djinn's hiding flags,
`FLAG_DJINN_HIDING_FIRST` and the 79 after it, one for each Djinni id, which
keep a Djinni that got away out of its room until the room is entered again,
and the 16 flags after them, which nothing else uses.

`Scene_ChooseMusic` takes the first rule of `gScene_MusicRules` that matches
the scene or its area, the entrance and a flag, or `SOUND_KEEP_BGM` when none
does. On an ordinary entry that music plays unless `FLAG_KEEP_MUSIC_ONCE`
(cleared as it is used) or `FLAG_KEEP_MUSIC_HELD` (left set) keeps the music
playing. On a return, `gMainRoutineWork.returnMusic` plays instead if it holds
any music: it is saved whenever the Pause menu opens, and a random battle and
every ordinary entry set it to -1, so after a random battle the scene's own
music plays.

Last, `Scene_LoadOverlay` unpacks the scene's overlay into `gOverlayArea`,
`FieldSequence_Run` runs the scene
([Field code](../field/README.md#the-field-sequence-serves-requests)), and
`Scene_TakeExit` turns the kind of exit it ended on into the next
[destination](../../glossary.md#destination): the first exit in the
overlay's exit table that is of that kind, or marked for any kind, and whose
flag, if it has one, is clear; with none, the scene of the scene's first exit,
at entrance 0. `EXIT_KEEP_DESTINATION` means the destination is already
written: by an event, Retreat, a random battle or a party that fell.

## The frame loop

The screen is redrawn about 60 times a second, and after each picture the
console pauses in the vertical blank, [VBlank](../../glossary.md#vblank),
before it starts the next. Changing the picture while it is drawn would show
half the change, so code writes new positions, scroll and colours to memory
during the frame, and the VBlank interrupt's code copies them to the hardware
together.

Each frame `wait()` runs the frame pass's tasks, gathers every
[sprite](../../glossary.md#sprite-and-oam) the code asked to show into the
list the hardware reads, marks the frame finished and halts the processor
until VBlank. At VBlank the interrupt's code, after the link cable's exchange
and the music and screen fades, does what has to land on screen: the sprite
list, the scroll of the four
[background layers](../../glossary.md#background-layer) and the queued
register writes. Then it runs the VBlank pass's tasks, reads the buttons and
mixes the next stretch of sound.

The screen work at VBlank happens only if the frame was marked finished, so a
frame whose work runs past VBlank changes nothing on screen until the next
one: the picture keeps the previous frame's sprites, scroll and settings
together, rather than a mixture of two frames, and the frame lasts two
VBlanks or more, which is [slowdown](../../glossary.md#slowdown). The
music is mixed at every VBlank whatever the frame is doing, so it keeps its
tempo. Two small delays follow from the order: sprites placed by a VBlank task
show a frame later, and buttons read at VBlank reach the game's code at the
start of its next frame.

`wait()` runs the frame and `System_VBlankHandler` VBlank's work; sprites wait
in depth order in `gOamDepthLists` until `Oam_BuildBuffer` gathers them.

### Buttons and sleep

![Isaac stands at the Sanctum's door in Vale while only L and R are held; after three seconds the screen turns white as the console goes to sleep](https://alchemy.pascalpixel.com/guides/runtime/sleep.gif)

Holding L and R alone for three seconds puts the console to sleep, with the
screen off and the battery spared until L and R wake it. The frame loop
counts the frames L and R have been held, and also repeats a held button, as
when the player holds Down in a menu: the first repeat comes after about a
third of a second, then one every tenth of a second. Where several directions
are held, the repeat keeps only the one pressed last.

The game can also fall asleep by itself after three minutes with no button
pressed, where a screen allows it. Either way, the screen turns white for a
second before the console sleeps, and after waking the game waits a few
VBlanks before carrying on. Some moments forbid sleep altogether. In
debug mode (`gDebugMode`), Start and Select held pause the game,
and A, B, Select or the D-pad then step it one frame at a time.

`wait()` counts the hold and the idle frames against `SLEEP_COMBO_FRAMES` and
`SLEEP_IDLE_FRAMES`; `Input_UpdateKeyRepeatAndDirection` repeats keys after
`INPUT_KEY_REPEAT_FIRST` frames, then every `INPUT_KEY_REPEAT_NEXT`.

## The stack switch in `wait()`

The stack is the scratch memory every function call takes a little of, and it
is small: 1,792 bytes at the top of IWRAM, which the interrupts share. Code
can ask to wait from deep inside its own calls, when most of the stack is
already in use, and the tasks that run while it waits need stack of their
own. So before running any frames, when the waiting code's stack has grown
below a fixed line 512 bytes above the stack's floor,
`Ram_FrameWaitStackTop`, `wait()` copies the part below the line aside into
ordinary memory, `gSavedStack`, and starts afresh from the line; when the
wait is over it puts the copy back. A caller whose stack is still above the
line keeps it.

The frame loop and the tasks therefore have at least 512 bytes, however deep
the waiting code, and so does VBlank's work when the VBlank arrives during the
halt. The price is one rule: a task
must never wait, because a second wait would overwrite the copy the first one
saved. Nothing checks, and no task breaks the rule (see
[Evidence](#evidence)).

## The heap

The heap lends blocks of memory to the rest of the game: a scene's working
state, a menu's buffers, a battle effect's pictures. It works like a pile:
each block goes on top of the last, and giving one back gives back everything
taken after it. That makes taking and giving back a block almost free, at the
price of one rule: blocks go back in the reverse of the order they were taken.
The main routine empties the whole heap before each sequence, so no block
outlives the sequence that took it.

There are two piles, one in fast IWRAM and one in the larger EWRAM; a request
tries IWRAM first and falls back on EWRAM, or the other way round for
`Heap_AllocEwram` and `HeapSlot_AllocEwram`. Some blocks have to be found
again by code that didn't take them, such as a scene's
[work](../../glossary.md#work), which a task reads every frame. Those are kept
in numbered places, the [heap slots](../../glossary.md#heap-slot), and asking
for a slot that already holds a block simply returns that block. Blocks are
not cleared when lent.

Giving blocks back out of order doesn't fail at once: later blocks stay in
their slots, pointing at memory the next request hands out again. For the
same reason, code must stop using a block before giving it back, which is why
a battle effect stops the task copying its pictures first.

Code takes a block with `Heap_Alloc` or, for a heap slot, `HeapSlot_Alloc`,
and gives it back with `Heap_Release` or `HeapSlot_Release`; the slots live in
`gHeapSlots`, which `Heap_Initialize` empties.

## The task runner

Some work has to happen every frame whatever else is going on: scrolling the
map, typing the windows' text, copying the windows' tilemap to the screen at
VBlank. Each is a task, added once and run until it is removed or the main
routine empties the table at the next transition. A task's priority says
which of the two passes runs it, the frame's or VBlank's, and where in that
pass, highest first. The battle camera, `BattleCamera_Update`, ranks one below
the battle's playback, so in every frame it runs after it.

A task can be paused and resumed without losing its place in the order. When
a menu opens over a scene, the tasks whose code is in the scene's overlay
pause, except the few a scene marks to keep running under a menu, as the
Battle Arena and Colosso do. The table holds 20 tasks. The game never adds a
task while a pass is running, so a pass never sees the table reordered under
it.

The original game's sort stops one pass early, so the first two tasks in the
table can be left in the wrong order
([`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix)).

`Task_Add` adds a task and `Task_Remove` removes it; `Task_RunPass` runs
`TASK_PASS_FRAME` or `TASK_PASS_VBLANK`, and `Task_ResetAll` clears the
table.

## The register write queue

The hardware's registers decide how the screen is drawn: which layers show,
how two blend, where a window cuts the picture. Code that changes them while
a frame is being prepared doesn't write them directly: it adds its writes to
a queue, and VBlank's code carries them out
together with the sprites and the scroll, and only for a frame whose work has
finished, so a frame's changes reach the screen together or not at all. A
frame that is not finished keeps its writes for the next. The queue takes 32
writes; a write to a full queue is lost.

`IoWriteQueue_FlushPending` empties the queue, `gIoWriteQueue`, by copying
the code that carries the writes out onto the stack, in IWRAM, each time and
running it there.

## Memory map

The console has two kinds of working memory, IWRAM and EWRAM. EWRAM, 256 KB,
is large but slower and holds most of the game's data: its variables, the
current scene's code, the map's grid and pictures being unpacked, and the
larger heap. IWRAM, 32 KB, is fast and holds what must run or be read
quickly: routines copied from the cartridge at power-on, the busiest
variables, a second heap and the stacks. Video memory holds the tiles and tile
maps the screen is drawn from, and the processor runs the
[main image](../../glossary.md#main-image) straight from the cartridge.

Each area has a fixed place, the same in every
[edition](../../glossary.md#editions), except that German reserves 16 bytes
before the IWRAM variables, so all of them sit 16 bytes higher (three of them
are fixed addresses, below):

| EWRAM, from | Size | What |
| --- | --- | --- |
| 0 KB | 32 KB | the game's variables: the main routine's work, the [game flags](../../glossary.md#game-flag), the sound driver's work |
| 32 KB | 32 KB | the current scene's code, `gOverlayArea` |
| 64 KB | 112 KB | the map grid's [cells](../../glossary.md#cell), the battle figures' pictures, unpacking space, the map's [cell pictures](../../glossary.md#cell-picture) and its animated tiles |
| 176 KB | 16 KB | the shapes of the ground |
| 192 KB | 32 KB | the EWRAM heap |
| 224 KB | 32 KB | background tiles being prepared, used only by the world map |

| IWRAM, from | What |
| --- | --- |
| 0 KB | 5 KB of routines copied from the cartridge: the interrupt code, the sound mixing and small helpers the rest of the game calls |
| 5 KB | the busiest variables, among them the heap slots |
| 8 KB | the IWRAM heap |
| 28 KB | the sound driver's own mixer, copied in when sound starts |
| 30 KB | the build stamp and the start of the stacks |
| the last 256 bytes | the stack tops and the address the console jumps to on an interrupt |

The allocators test only against the end of EWRAM and against the IWRAM
heap's end at 30 KB, so each heap can run into the buffer above it, and in
play both do: the EWRAM heap reaches into the world map's tile buffer in
battle, and in battle the IWRAM heap runs over the sound mixer's copy at
28 KB, which never runs during a battle (the measurements are in the comments
on `gEwramHeap` and `gIwramHeap`, `include/system/heap_release.h`).
`main.ld` declares the areas, `ewram.s`, `iwram.s` and `iwram_high.s` lay out
their variables, and the English link map, `out/tbs-en/tbs-en.map`, has every
address.

### Video memory

The screen is built from 8 by 8 pixel tiles. A background layer is a sheet of
tiles plus a screen map saying which tile goes where; a sprite draws its own
tiles wherever it is put. On a scene, video memory holds the windows' tiles
first, then the windows' screen map and the three map layers' screen maps,
then the map's three sheets of tiles, and in its last 32 KB the sprites'
tiles, which a cache hands out in 512 blocks of 64 bytes. The credits screen
divides the background memory into two halves instead
([Menus](../menu/README.md#two-battle-backgrounds-in-one-background-memory)).

The window tiles are placed by `Window_LoadGraphics`, the map's by
`MapGrid_LoadMap` ([Maps](../map/README.md)), and the sprite tiles by the
cache's `VramCache_Load` and `VramCache_AllocateBlocks`.

## How an overlay reaches the rest of the game

Since all the scenes' code together would not fit in memory, each scene's code
is packed on the cartridge as an overlay and unpacked, when the party arrives,
into the 32 KB overlay area, over the scene before. Every overlay is built to
run at that one address, so only one is ever loaded and an overlay never calls
another; code several scenes need is copied into each. There are 96 overlays:
several scenes can share one, and the Title and a few screens that are mostly
menus are overlays too.

An overlay meets the rest of the game at two tables:

- At its start, the
  [entry table](../../glossary.md#entry-table-and-import-table): the functions
  the main image calls to ask the scene about itself
  ([Scenes](../scene/README.md#the-entry-table) explains each).
- At its end, the import table: one entry for each function of the main image
  the overlay calls. The overlay area is too far from the cartridge for a
  direct call, so each call goes through a [veneer](../../glossary.md#veneer)
  holding the address of the function's veneer in its bank's
  [far-call](../../glossary.md#far-call) table
  ([The Game Pak](../gamepak/README.md#why-the-main-image-is-cut-into-banks)),
  which stays put while the banks' code moves.

Every overlay keeps the same list of possible imports in the same order, and
takes from it only what it uses. As the game unpacks an overlay it also fixes
up the overlay's calls: on the cartridge each call is stored relative to the
start of the overlay, and the processor needs it relative to the call itself.

The entry table is `struct SceneEntryPoints`; the list of imports is
`overlay/imports.inc`, which every overlay's entry and import tables include.
Before each scene the main routine calls `Scene_LoadOverlay`, whose
`Overlay_Load` unpacks the overlay the scene table names. `overlay/overlay.ld`
links each overlay at `gOverlayArea`, `overlay/overlays.s` places the 96
packed streams, and the packer is `tools/ags-overlays`.

## The addresses that never move

Some code says "the sound mixer is at this address" rather than "wherever the
linker put the sound mixer". [The original](../../glossary.md#the-original)
holds 24 such fixed addresses, so the matching build compiles the same
numbers, and the memory map cannot move what they point at: the cartridge's
first instruction, eleven routines copied into IWRAM, the map and decode
buffers that follow the overlay area, the sound mixer, the end of the IWRAM
heap, the stack `wait()` runs its frames on, and three IWRAM variables whose
German address differs. Each is defined once, with why it cannot move, and
the check build with a current GCC (`make modern`) reaches each by its name
instead.

They are the `FIXED_ADDRESS` lines of `include/system/ram_buffer.h`,
`include/system/iwram_call.h` and `include/gamepak/rom_header.h`.

## Saving

Saving writes the new copy of a save before it lets go of the old one, so
switching off at the wrong moment never loses a save. The cartridge's
[flash chip](../gamepak/README.md#the-flash-chip) is written a sector at a
time; each save goes into an empty sector, its copy stamped with a count one
higher than the old copy's, and only once it is written is the old copy
marked free. Whenever the power goes, one whole copy remains, and the game
keeps the one with the higher count.

The flash has 16 sectors, and each of the three save
[files](../../glossary.md#file-and-part) is two parts, each in a sector of its
own. A new copy of a part goes into a free sector chosen at random, which
spreads the wear over the chip, and the last free sector is kept back for
replacing an existing save, so replacing one always has room. Every save,
load, copy or delete starts by reading all 16 sectors to find the newest copy
of each part.

The sectors are written by `Save_WritePart` and read by
`Save_InitializeWork`; the Save command itself is `Save_ToChosenFile`
([Windows](../window/README.md)), and `savedata.c` holds the tables that lay a
part out.

## The link cable

Two consoles joined by a cable trade a small fixed payload every VBlank, and
anything larger, such as a [party](../../glossary.md#the-party) for a link
battle, rides inside it in numbered blocks that are sent again when one goes
missing. Link battles, the Battle Arena and the File Screen's transfer use it.

Each VBlank's payload carries 24 bytes each way, and larger data travels 20
bytes a VBlank. The receiver keeps a block only when its number is the one it
expects, and otherwise asks for that one again; the sender then goes back and
sends from there. A payload that arrives while the hardware reports a failed
transfer is thrown away.

VBlank's code trades the payloads through `Serial_ExchangePayloads`; larger
data starts with `Serial_StartSend` and `Serial_StartReceive`.

## Design

A frame's changes reach the hardware together
([System](../../system.md#design) covers the rest): code writes copies in
memory (the sprite lists, the scroll, the write queue) and VBlank's code
commits them only for a finished frame.

The ROM shows the System's bank 00 code as one object, compiled once through
`system.c`; the parts are our cut of it. The main routine is the first code of
bank 04, and nothing it defines is read by the field sequence's object after
it, so it is compiled as an object of its own in this folder, with the System
it belongs to.

## How it fits with the rest of the game

Every other folder runs on the System: the field, the battles and the menus
let time pass through `wait()`, keep their per-frame work as tasks, take
their memory from the heap and reach the screen through the sprite lists, the
scroll and the write queue. Other banks call the System's bank 00 directly,
and overlays through its far-call table
([The Game Pak](../gamepak/README.md#why-the-main-image-is-cut-into-banks)).
The main routine loads each scene's overlay by the number the scene table
gives, and every overlay in [Scenes](../scene/README.md) and
[Debug scenes](../debug/README.md) is built from the entry and import tables
described here.

## Limits

- 62 heap slots; the IWRAM heap ends below the stack, the EWRAM heap at the
  end of EWRAM, and nothing keeps either out of the buffers above it.
- 20 tasks at a time.
- 32 queued register writes.
- 256 depth levels for sprites and 32 rotation-scaling matrices a frame; once
  all 32 are taken, a sprite is drawn with whatever the first matrix holds.
- At least 512 bytes of stack for the frame loop and its tasks.
- An overlay has the 32 KB of the overlay area.
- 16 flash sectors for the three save files.

## Words used here

- **resident code**: code copied into IWRAM at power-on or when sound starts.

Frame, VBlank, IWRAM, EWRAM, heap slot, task, sprite and OAM, file and part
are in the [glossary](../../glossary.md).

## Evidence

The folder is named for the credits' Programming "System" (`gCredits_Lines`).
The names inside it come from what the code does; none is recovered from
Camelot. `IwramIrqMain` and the other resident routines are named from their
code, and the fixed addresses come from the literals in the ROM's code. No
build step compares a `FIXED_ADDRESS` literal with its symbol's linked
address; a one-off comparison of every one of them with the six Broken Seal
linker maps found every literal equal to its symbol in each edition its `#if`
selects.

The heaps' high points (how far each heap reached) were measured in the
English edition in libmgba, with the heap's next address read at every frame
and every release: a Goma Cave battle, the field menu, the Item screen, field
[Psynergy](../../glossary.md#psynergy-and-pp), a shop, an inn, a talk, the
world map and its still picture, the credits and sleep mode.

`gSavedStack` holds 3,152 bytes, though a stack below the frame-wait top and
above the heap's end is at most 512. `ewram.s` lays EWRAM out in 4 KB groups
(`gSceneLocalState` opens the second, `gSleepActive` the third, the music state
the fourth), and `gSavedStack` is the last cell of the third, sized to reach
the fourth.

No task waits, and none adds a task. A call graph of the English build's
objects, main image and overlays, from each function the code adds as a task,
each actor update routine and each `SC_CALL` routine, finds ten paths to
`wait()`: nine from tasks, none of which takes it, and one from the debug
scene selector, which only the unused `DebugSceneSelect_Schedule` would add
as a task. Of the nine, the scene tasks face actors in a direction with a
wait of 0 (`Actor_FaceDirection`), the world map's lighthouse jitter moves
the camera with a glide, Lucky Wheels and the link countdown open instant
windows (`WINDOW_FLAG_INSTANT`), and battle playback's sprite respawn tests
`gTasksRunning`. The same graph has no path from any task to `Task_Add`.
