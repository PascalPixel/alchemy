# Runtime

When an event pauses for half a second, the code that runs it simply waits,
yet the screen keeps moving: windows still type, the [scene](../../glossary.md#scene)
still scrolls with the camera. Everything in the game waits the same way,
through one function, `WaitFrames`, which runs a short list of jobs, the
[tasks](../../glossary.md#task), once every [frame](../../glossary.md#frame)
while it waits. That function and what it needs around it are the runtime:
start-up, the frame and its hand-over to the screen, the
[heap](../../glossary.md#heap-and-pools), the task runner, the queue of
hardware writes, the buttons and sleep, saving and the link cable. The memory
it works in is laid out in [System](../system/README.md).

## Start-up

![In Vale, A, B, Select and Start are pressed together: the screen goes black at once and the game starts again, Camelot's logo appearing and fading](https://alchemy.pascalpixel.com/guides/runtime/soft-reset.gif)

Pressing A, B, Select and Start together throws the game away and starts it
again from the cartridge's first instruction, exactly as switching on does.
Start-up puts memory in order and starts the services everything relies on,
once, then hands over to the loop of scenes for as long as the console is on.

In order, start-up stops any screen effect left running, clears the fast
memory, [IWRAM](../../glossary.md#iwram-and-ewram), and empties the heap,
copies the routines that must run fastest into IWRAM, loads the windows'
graphics, sets up the code that hands each frame to the screen, arms the
reset buttons, starts the sound and empties the task list. It then waits ten
frames and starts the loop of scenes, which never ends
([Running a scene](../overworld/README.md)). The reset buttons work through an
interrupt: pressing all four marks a reset as wanted, and the next frame
carries it out.

The cartridge header jumps to `System_Entry`, which sets up the stacks and
calls `System_Initialize`; that ends by calling `SequenceManager_RunFar`. The reset is
`Input_HandleKeyInterrupt`.

## The frame loop

The screen is redrawn about sixty times a second, and each frame has two
halves: the game's work, then a wait for the vertical blank (VBlank), the
pause after the last line, in which everything lands on screen at once.
Changing the picture while it is drawn would show half the change, so code
writes new positions, scroll and colours to memory during the frame, and the
VBlank's code copies them to the hardware together.

Each frame `WaitFrames` runs the frame's tasks, gathers every
[sprite](../../glossary.md#sprite-and-oam) the code asked to show into the
list the hardware reads, marks the frame finished and halts the processor
until the blank. In the blank, the VBlank's code first does what has to land
on screen: the sprite list, the scroll of the four
[background layers](../../glossary.md#background-layer) and the queued
register writes. Then it runs the blank's tasks, reads the buttons and mixes
the next stretch of sound.

The screen work in the blank happens only if the frame was marked finished.
So a frame whose work runs late changes nothing on screen: the picture keeps
the previous frame's sprites, scroll and settings together, rather than a
mixture of two frames. Two small delays follow from the order: sprites placed
by a blank's task show a frame later, and buttons read in the blank reach the
game's code at the start of its next frame.

The frame is `WaitFrames` and the blank `System_VBlankHandler`; the sprites
are sorted by depth in `gOamDepthLists` and gathered by `Oam_BuildBuffer`.

### Buttons and sleep

![Isaac stands at the Sanctum's door in Vale while only L and R are held; after three seconds the screen turns white as the console goes to sleep](https://alchemy.pascalpixel.com/guides/runtime/sleep.gif)

Holding L and R alone for three seconds puts the console to sleep, with the
screen off and the battery spared until L and R wake it. The frame loop
counts the frames each button combination has been held, and also repeats a
held button, as when the player holds Down in a menu: the first repeat comes
after about a third of a second, then one every tenth of a second. Where
several directions are held, the repeat keeps only the one pressed last.

The game can also fall asleep by itself after three minutes with no button
pressed, where a screen allows it. Before it sleeps the screen turns white
for a second; after waking it waits a few frames before carrying on. Some
moments forbid sleep altogether. In debug mode, Start and Select held pause
the game, and A, B, Select or the D-pad then step it one frame at a time.

The repeat and sleep are in `WaitFrames` and
`Input_UpdateKeyRepeatAndDirection`; the timings are `SLEEP_COMBO_FRAMES`,
`SLEEP_IDLE_FRAMES` and `INPUT_KEY_REPEAT_FIRST`.

## The stack switch in `WaitFrames`

The stack is the scratch memory every function call takes a little of, and it
is small: 1,792 bytes at the top of IWRAM, which the interrupts share. Code
can ask to wait from deep inside its own calls, when most of the stack is
already in use, and the tasks that run while it waits need stack of their
own. So before running any frames, `WaitFrames` copies the deepest part of
the waiting code's stack aside into ordinary memory and starts afresh from a
fixed line 512 bytes above the stack's floor; when the wait is over it puts
the copy back.

The frame loop, the tasks and the VBlank's work therefore always start with
the same 512 bytes, however deep the waiting code. The price is one rule: a
task must never wait, because a second wait would overwrite the copy the
first one saved. Nothing checks, and no task breaks the rule (see
[Evidence](#evidence)).

The line is `Ram_FrameWaitStackTop`, and the copy goes to `gSavedStack`.

## The heap

The heap lends blocks of memory to the rest of the game: a scene's working
state, a menu's buffers, a battle animation's pictures. It works like a pile:
each block goes on top of the last, and giving one back gives back everything
taken after it. That makes both almost free, at the price of one rule: blocks
go back in the reverse of the order they were taken. The whole heap is
emptied before each scene, so no block outlives the scene that took it.

There are two piles, one in fast IWRAM and one in the larger EWRAM; a request
says which to try first and falls back on the other. Some blocks have to be
found again by code that didn't take them, such as a scene's state, which a
task reads every frame. Those are kept in numbered places, the
[heap slots](../../glossary.md#heap-slot), and asking for a slot that already
holds a block simply returns that block. Blocks are not cleared when lent.

Giving blocks back out of order doesn't fail at once: later blocks stay in
their slots, pointing at memory the next request hands out again. That is why
a battle animation stops the task that copies its pictures before it gives
its blocks back.

A block is taken with `Alloc` or, for a heap slot, `AllocHeapSlot`, and given
back with `Free` or `FreeHeapSlot`; the slots are `gHeapSlots`.

## The task runner

Some work has to happen every frame whatever else is going on: scrolling the
scene, typing the windows' text, copying their changed tiles to the screen in
the blank. Each is a task, added once and run until it is removed or the next
scene starts. A task's priority says which of the two passes runs it, the
frame's or the blank's, and where in that pass. That order matters: a
battle's camera runs after the battle's figures, so that it moves to where
they now are.

A task can be paused and resumed without losing its place in the order. When
a menu opens over a scene, the scene's own tasks pause, except the few a scene
marks to keep running under a menu, as the Battle Arena and Colosso do. The
table holds 20 tasks. The game never adds a task while a pass is running, so
a pass never sees the table reordered under it.

The original game sorts the table with a small mistake: the first two tasks
are never compared, so they can run in the wrong order
([`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix)).

Tasks are added with `Task_Add`, removed with `Task_Remove` and run by
`Task_RunPass`; the passes are `TASK_PRIORITY_MAIN_LOOP` and
`TASK_PRIORITY_VBLANK`.

## The register write queue

The hardware's registers decide how the screen is drawn: which layers show,
how two blend, where a window cuts the picture. Code doesn't write them
directly. It adds its writes to a queue, and the VBlank's code carries them
out in the blank together with the sprites and the scroll, and only for a
frame whose work has finished, so a frame's changes reach the screen together
or not at all. The queue takes 32 writes a frame; a write to a full queue is
lost.

The code that carries the writes out is copied into IWRAM each time and run
there, since the processor reads each of its instructions from IWRAM in one
step and from the cartridge in two.

The queue is `gIoWriteQueue`, emptied by `IoWriteQueue_FlushPending`.

## Saving

![In Vale, Start opens the pause menu; Save your quest is chosen, then the first of three empty files, and the game says "Saving..." and then "Your game was saved."](https://alchemy.pascalpixel.com/guides/runtime/save.gif)

Saving writes the new copy of a save before it lets go of the old one, so
switching off at the wrong moment never loses a save. The cartridge's flash
memory is written a sector at a time; each save goes into an empty sector
numbered one higher than the old copy, and only once it is written is the old
copy marked free. Whenever the power goes, one whole copy remains, and the
game keeps the higher number.

The flash has 16 sectors, and each of the three save files takes two. A new
copy goes into a free sector chosen at random, which spreads the wear over the
chip, and the last free sector is kept back for replacing an existing save,
so replacing one always has room. Every save, load, copy or delete starts by
reading all 16 sectors to find the newest copy of each part.

The sectors are written by `Save_WriteRecord` and read by
`Save_InitializeWork`; the Save command itself is `Save_ToChosenFile`
([Windows](../window/README.md)).

## The link cable

Two consoles joined by a cable trade a small fixed message every frame, and
anything larger, such as a [party](../../glossary.md#the-party) for a link
battle, rides inside it in numbered pieces that are sent again when one goes
missing. Link battles, the Battle Arena and the main menu's transfer use it.

Each frame's message carries 24 bytes each way, and larger data travels 20
bytes a frame. The receiver keeps a piece only when its number is the one it
expects, and otherwise asks for that one again; the sender then goes back and
sends from there. A message that arrives while the hardware reports a failed
transfer is thrown away.

The exchange is `Serial_ExchangePayloads`, and larger data starts with
`Serial_StartSend` and `Serial_StartReceive`.

## Design

The game is written as ordinary functions that run until they are done. A
battle animation, an event or a menu screen is a function with its own loop
that waits when time has to pass and carries on from the same place; nothing
returns to a main loop between frames. Most of the runtime's shape follows:
tasks keep the world moving while such a function waits, the stack switch
keeps the stack bounded however deep it waits, task priorities give tasks a
known order, and heap slots let a task find blocks that other code took.

The second choice is that a frame's changes reach the hardware together: code
writes copies in memory (the sprite lists, the scroll, the write queue) and
the VBlank's code commits them only for a finished frame, so a late frame
shows the previous picture whole rather than a mixture of two.

The ROM shows the runtime as one object, compiled once through `runtime.c`;
the files are our cut of it. The scenes' code reaches many of its functions
through the runtime bank's [far-call](../../glossary.md#far-call) table, so
renaming one touches other folders too.

## How it fits with the rest of the game

Every other folder runs on the runtime: the field, the battles and the menus
let time pass through `WaitFrames`, keep their per-frame work as tasks, take
their memory from the heap and reach the screen through the sprite lists, the
scroll and the write queue. Other banks call the runtime directly, and
overlays through its far-call table
([System](../system/README.md#why-the-program-is-cut-into-banks)).

## Limits

- 62 heap slots; the IWRAM heap ends below the stack, the EWRAM heap at the
  end of EWRAM.
- 20 tasks at a time.
- 32 queued register writes a frame.
- 96 entries in the cache of sprite pictures, sharing 32 KB of video memory,
  at most 8 KB for one entry.
- 256 depth levels for sprites and 32 rotation-scaling matrices a frame; once
  all 32 are taken, a sprite shares whatever the first matrix holds.
- 512 bytes of stack for the frame loop, its tasks and the VBlank work.
- 16 flash sectors for the three save files.

## Words used here

Frame, VBlank, IWRAM, EWRAM, heap slot, task, sprite and OAM are in the
[glossary](../../glossary.md).

- **16.16 fixed point**: a whole number whose upper half is the whole part
  and lower half a fraction, so 65,536 means 1.0. Positions, scales and the
  camera's transform use it.
- **angle**: a whole turn is 65,536 steps, so a quarter turn is 16,384.
- **queued write**: a register write that the VBlank's code carries out once a
  frame has finished.
- **depth slot**: one of 256 lists sprites are put into each frame, walked in
  order to build the hardware's list.
- **cache entry**: one of the 96 entries of the sprite picture cache, each
  owning a run of the 64-byte blocks sprite video memory is split into.
- **transform**: `gTransform`, the rotation and position the battle and field
  cameras work through.
- **record**: one save record in one 4 KB flash sector.
- **payload** and **block**: each frame two linked consoles trade a 24-byte
  payload, and larger data rides on it in 20-byte blocks.

## Evidence

The names come from what the code does: `WaitFrames` from its loop, `Task_`
from the table it keeps, the heap's from its two pointers and its heap slots,
the save code's from the flash records it writes. Constants such as
`TASK_PRIORITY_MAIN_LOOP` are the values the callers pass.

`gSavedStack` holds 3,152 bytes, though a stack below the frame-wait top and
above the heap's end is at most 512. `ewram.s` lays EWRAM out in 4 KB groups
(`gSceneLocalState` opens the second, `gSleepActive` the third, the music state
the fourth), and `gSavedStack` is the last cell of the third, sized to reach
the fourth.

No task waits, and none adds a task. A call graph of the English build's
objects, main image and overlays, from each function the code adds as a task,
each object hook and each `SC_CALL` callback, finds nine tasks with a path of
calls to `WaitFrames`, and none takes it: the scene tasks turn object events
with a wait of 0 (`ObjectMotion_ArmCallback`), the world map's lighthouse
jitter moves the camera with a glide, Lucky Wheels and the link countdown
open instant windows (`WINDOW_FLAG_INSTANT`), and battle playback's sprite
respawn tests `gTasksRunning`. The tenth path is the debug scene selector,
which only the unused `DebugMapSelect_Schedule` would add as a task. The same
graph has no path from any task to `Task_Add`.
