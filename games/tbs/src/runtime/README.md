# Runtime

When an event pauses for half a second, the code that runs it simply waits,
yet the screen keeps moving: windows still type, the [map](../../glossary.md#map)
still scrolls with the camera. Everything in the game lets time pass the same
way, through one function, [`wait()`](../../glossary.md#wait), which runs a
short list of routines, the [tasks](../../glossary.md#task), once every
[frame](../../glossary.md#frame) while it waits. That function and what it
needs around it are the runtime's core: start-up, the frame and its handover
at VBlank, the buttons and sleep, the stack switch, the
[heap](../../glossary.md#heap-and-pools) and the task runner. Beside them sit
the services every part of the game shares: the register write queue, saving,
the link cable, the [palette fades](../../glossary.md#palette-fade), the
sprite picture cache, fixed-point maths and the 3D transform, decompression
and debug text. The memory it works in is laid out in
[Cartridge and memory](../cartridge/README.md).

## Start-up

Pressing A, B, Select and Start together restarts the game from the
cartridge's first instruction, as switching on does once the
console's own logo has gone. Start-up puts memory in order and starts the
services everything relies on, once, then hands over to the
[main routine](../../glossary.md#main-routine) for as long as the console is
on.

In order, start-up stops any screen effect left running, clears the fast
memory, [IWRAM](../../glossary.md#iwram-and-ewram), below the stacks, and
empties the heap, installs the interrupt handlers and the routines that live
in IWRAM, loads the windows' graphics, sets up the code that hands each frame
to the screen, arms the reset buttons, starts the sound and empties the task
table. It then waits ten frames and starts the
[main routine](../../system.md#the-main-routine), which never returns
(`MainRoutine_Run`). The reset buttons
work through an interrupt: pressing all four marks a reset as wanted, and the
next frame carries it out.

The cartridge header jumps to `System_Entry`, which sets up the stacks and
calls `System_Initialize`; that ends with `MainRoutine_RunFar`. The reset is
`Input_HandleKeyInterrupt`.

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
Camelot's debug mode (`gDebugMode`), Start and Select held pause the game,
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
taken after it. That makes taking and giving back a block almost free, at the price of one rule: blocks
go back in the reverse of the order they were taken. The main routine empties
the whole heap before each sequence, so no block outlives the sequence that
took it.

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

## Saving

Saving writes the new copy of a save before it lets go of the old one, so
switching off at the wrong moment never loses a save. The cartridge's flash
memory is written a sector at a time; each save goes into an empty sector,
its copy stamped with a count one higher than the old copy's, and only once
it is written is the old copy marked free. Whenever the power goes, one whole
copy remains, and the game keeps the one with the higher count.

The flash has 16 sectors, and each of the three save files is two parts, each
in a sector of its own. A new copy of a part goes into a free sector chosen at
random, which spreads the wear over the chip, and the last free sector is kept
back for replacing an existing save, so replacing one always has room. Every
save, load, copy or delete starts by reading all 16 sectors to find the newest
copy of each part.

The sectors are written by `Save_WritePart` and read by
`Save_InitializeWork`; the Save command itself is `Save_ToChosenFile`
([Windows](../window/README.md)).

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
([System](../../system.md#design) covers the rest): code writes copies in memory (the sprite lists, the scroll, the
write queue) and VBlank's code commits them only for a finished frame.

The ROM shows the runtime as one object, compiled once through `runtime.c`;
the files are our cut of it.

## How it fits with the rest of the game

Every other folder runs on the runtime: the field, the battles and the menus
let time pass through `wait()`, keep their per-frame work as tasks, take
their memory from the heap and reach the screen through the sprite lists, the
scroll and the write queue. Other banks call the runtime directly, and
overlays through its [far-call](../../glossary.md#far-call) table
([Cartridge and memory](../cartridge/README.md#why-the-main-image-is-cut-into-banks)).

## Limits

- 62 heap slots; the IWRAM heap ends below the stack, the EWRAM heap at the
  end of EWRAM.
- 20 tasks at a time.
- 32 queued register writes.
- 256 depth levels for sprites and 32 rotation-scaling matrices a frame; once
  all 32 are taken, a sprite is drawn with whatever the first matrix holds.
- At least 512 bytes of stack for the frame loop and its tasks.
- 16 flash sectors for the three save files.

## Words used here

Frame, VBlank, IWRAM, EWRAM, heap slot, task, sprite and OAM, file and part
are in the [glossary](../../glossary.md).

## Evidence

The names come from what the code does; none is recovered from Camelot.

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
