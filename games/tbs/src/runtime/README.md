# Runtime

When a cutscene pauses for half a second, the code that runs it simply stops
and waits, yet the screen keeps moving: windows still type their text, the [map](../../glossary.md#map)
still scrolls with the camera. Everything in the game waits the same way,
through one function, `WaitFrames`. While it waits, `WaitFrames` runs a short
list of jobs, called [tasks](../../glossary.md#task), once every [frame](../../glossary.md#frame). That function, and what
it needs around it, is the runtime: the layer every other part of the game
runs on.

The runtime starts the console, runs every frame and hands the finished
picture to the screen at the right moment. Around that it keeps the [heap](../../glossary.md#heap-and-pools),
which lends memory to the rest of the game, the task runner, the queue of
hardware writes that wait for the screen's blank moment, the buttons, sleep
mode, screen fades, the frame's [sprite](../../glossary.md#sprite-and-oam) list, the 3D transform, saving and the
link cable. The memory map it works in, and the fast code it copies into
[IWRAM](../../glossary.md#iwram-and-ewram), are in [System](../system/README.md).

## Start-up

Start-up takes the console from the moment it is switched on to the title
screen, and never comes back.

![The console switched on: the Nintendo logo, then Camelot's, before the title screen](https://alchemy.pascalpixel.com/guides/engine/boot.gif)

Before the game can show anything, memory has to be put in order and the
services everything else relies on have to be running: the code that puts
each frame on screen, the sound, the list of tasks. Start-up does that once,
then hands over to the loop that runs one map after another for as long as
the console is on.

In the code, a few hardware words recur. The console's memories sit at
fixed addresses: EWRAM from `0x02000000`, IWRAM from `0x03000000`, VRAM, the
video memory, from `0x06000000`, and the cartridge from `0x08000000`
([System](../system/README.md#memory-map) lays them out). An interrupt (IRQ)
is the hardware stopping the program to run a handler, as it does at each
VBlank, the pause after the last line of the picture. An HBlank DMA is a copy
the console makes by itself in the pause after each line of the picture.

The cartridge header jumps to `System_Entry`
(`games/tbs/src/system/entry.s`), which sets the IRQ and system stacks at the
top of IWRAM and calls `System_Initialize` (`runtime/system.c`). That
function, in order:

1. stops any HBlank DMA left running and sets the cartridge wait states (how
   long the processor waits for each read from the cartridge);
2. clears IWRAM up to `Ram_IwramHeapEnd`, leaving the stacks above it alone,
   and empties the heap (`InitHeap`);
3. copies the resident runtime into IWRAM and points the interrupt vector at
   its dispatcher (`Runtime_InstallIwramAndIrqs`);
4. copies the build stamp and loads the window graphics;
5. installs `System_VBlankHandler`, arms the soft-reset keys, starts the
   sound driver, and empties the sprite VRAM cache and the task table;
6. waits ten frames and calls `Game_Run`.

The German, French, Spanish and Italian [editions](../../glossary.md#editions) (`TBS_PATCH_2`) also
install `Iwram_Halt` for the cartridge-removed interrupt.

`Game_Run` (`games/tbs/src/object/object/map.c`) is in the object [bank](../../glossary.md#bank), so
start-up reaches it through its [far-call](../../glossary.md#far-call) entry, `Game_RunFar`. It never
returns: it runs one map after another, starting at the title, and before
each one it resets the task table, removes the HBlank and VCount handlers,
empties the heap and resets the VRAM cache.

## The frame loop

The screen is redrawn about sixty times a second, each picture a frame, and
the game prepares one picture per frame.

The console draws the screen line by line, top to bottom, and after the last
line comes a short pause before the next picture starts: the vertical blank,
or VBlank. Changing what is on screen while a picture is being drawn would
show half of the change, so the game does its work during the frame, keeping
the new positions, scroll and colours in RAM, and hands them to the hardware
all at once in the blank. One frame therefore has two halves: the work, then
a wait for the blank in which everything lands.

`WaitFrames(n)` (`runtime/frame.c`) runs `n` frames and returns. If the
caller's stack has grown below `Ram_FrameWaitStackTop`, the stack is first
moved ([below](#the-stack-switch-in-waitframes)). During a frame, code links
each sprite it wants shown into one of 256 depth slots (`gOamDepthLists`),
lists by depth that decide which sprite is drawn in front. Each frame then:

1. runs the main-loop tasks (`Task_RunPass(TASK_PRIORITY_MAIN_LOOP)`);
2. allocates the [heap slot](../../glossary.md#heap-slot) `HEAP_SLOT_FRAME_SCRATCH` (1 KB), builds the OAM
   list in it by walking the depth slots in order (`Oam_BuildBuffer`) and
   sets `gFrameRenderPending`, which marks the frame finished;
3. measures CPU load for the debug display when it is on, and counts the
   frames toward sleep;
4. halts the CPU until the VBlank handler has run (`swi 2`, the BIOS's Halt);
5. releases the scratch block, resets the depth slots, counts the frame and
   updates key repeat (`Input_UpdateKeyRepeatAndDirection`);
6. sleeps, or restarts the game, if either was requested.

VBlank interrupts the halt. `System_VBlankHandler`, reached through the IWRAM
dispatcher and `gIrqHandlers`, then:

1. stops any HBlank DMA, trades the link payloads and steps the music's fades
   and the screen fade;
2. commits the frame, only if `gFrameRenderPending` is set: it copies the
   frame's OAM list (when `gOamCopyEnabled`), the scroll of the four
   [background layers](../../glossary.md#background-layer) from `gBgScroll`, and the queued register writes to the
   hardware, and clears the flag;
3. runs and clears the one-shot `gVBlankCallback`, and runs the VBlank tasks
   (`TASK_PRIORITY_VBLANK`);
4. reads the keypad, counts the frame, sets `gVBlankDone` and mixes the next
   sound buffer.

A frame whose work is still running when VBlank comes has not yet set
`gFrameRenderPending`, so only step 2 is skipped: the VBlank tasks, the keys
and the sound go on every VBlank.

The order has consequences for the code that runs in a frame:

- Tasks never write sprite or scroll registers directly. They write RAM, and
  VBlank commits a whole frame at once. A frame whose work does not finish
  before VBlank (a lag frame) is not committed, so the screen keeps the
  previous frame's sprites, scroll and register writes together
  ([the register write queue](#the-register-write-queue) shows why).
- Sprites must be placed by main-loop tasks, or by the waiting code before it
  calls `WaitFrames`; the OAM list is built right after the main pass, so a
  VBlank task's sprites show a frame later.
- The keys are read in VBlank and turned into repeats after the halt, so code
  sees them at the start of its next frame.

Why the VBlank handler keeps this order is an open question (at the end of
this page).

In a frame of Vale, each person is one 32 by 32 pixel entry in the list, with a
16 by 8 pixel entry for the shadow under their feet; where entries overlap,
their order in the list decides which is drawn in front.

| Name | Value | Meaning |
| --- | --- | --- |
| `TASK_PRIORITY_MAIN_LOOP` | `0xc80` | the pass `WaitFrames` runs each frame |
| `TASK_PRIORITY_VBLANK` | `0x480` | the pass the VBlank handler runs |
| `HEAP_SLOT_FRAME_SCRATCH` | 52 | the heap slot the frame's OAM list is built in |
| `RENDER_DEPTH_SLOT_COUNT` | 256 | depth slots a sprite is linked into |
| `OBJ_AFFINE_MATRIX_COUNT` | 32 | the OAM's rotation-scaling matrices a frame |
| `CPU_LOAD_PEAK_FRAMES` | 30 | frames the debug display holds a CPU load peak |

### Input, soft reset and sleep

The buttons, the soft reset and sleep mode are all looked after by the frame
loop between frames.

A held button repeats, as it does when the player holds Down in a menu: first
after `INPUT_KEY_REPEAT_FIRST` frames and then every `INPUT_KEY_REPEAT_NEXT`.
`gKeysRepeatDirected` is the repeat with at most one D-pad direction: of
several held directions it keeps the one added last, and with all four it
keeps none.

A, B, Select and Start pressed together restart the game. They raise the
keypad interrupt; `Input_HandleKeyInterrupt` sets `gResetRequested`, and the
frame loop jumps to `Cart_Restart`, the cartridge entry, so start-up runs
again from the top. In debug mode
(`gDebugMode`), holding Start and Select pauses the loop; A, B, Select or the
D-pad then advance one frame and Start resumes.

Holding only L and R for `SLEEP_COMBO_FRAMES`, or touching no button for
`SLEEP_IDLE_FRAMES` while `gIdleSleepEnabled` is set, puts the console to
sleep. The loop turns the screen white for `SLEEP_WHITE_FRAMES`, stops the
CPU (`swi 3`, the BIOS's Stop) with the keypad set to wake on L and R, then
restores the display and waits `SLEEP_WAKE_FRAMES`. Nothing sleeps while
`gSleepDisabled` is set.

| Name | Value | Meaning |
| --- | --- | --- |
| `INPUT_KEY_REPEAT_FIRST` | 19 | frames a key is held before its first repeat |
| `INPUT_KEY_REPEAT_NEXT` | 6 | frames between later repeats |
| `SLEEP_COMBO_FRAMES` | `3 * 60` | three seconds holding only L and R |
| `SLEEP_IDLE_FRAMES` | `3 * 60 * 60` | three minutes with no key held |
| `SLEEP_WHITE_FRAMES` | 60 | the white screen shown before the Stop |
| `SLEEP_WAKE_FRAMES` | 10 | frames waited after waking, screen restored |

## The stack switch in `WaitFrames`

`WaitFrames` moves a deep caller's stack out of the way, so the frame loop,
its tasks and the VBlank work always start with the same room.

Every function call leaves a little on the stack, a scratch area that grows
down from the top of IWRAM towards the heap and shrinks again as calls
return. Code can call `WaitFrames` from deep inside its own calls, and the
tasks it runs need stack of their own. If they simply carried on below the
waiting code, a deep enough caller would leave them too little room before
the stack ran into the heap. So `WaitFrames` sets the deepest part aside.

The system stack runs from the top of IWRAM down to the end of the IWRAM heap,
1,792 bytes. Interrupts use it too: the dispatcher, `IwramIrqMain`
(`system/runtime.s`), switches to system mode before calling a handler, so the
VBlank handler, the VBlank tasks and the sound mixer run below whatever they
interrupted.

`WaitFrames` keeps the frame loop's room in four steps:

1. If the stack pointer is below `Ram_FrameWaitStackTop`, 512 bytes above the
   heap's end, it copies the bytes from the stack pointer up to
   `Ram_FrameWaitStackTop` into `gSavedStack` in EWRAM by DMA, and keeps
   their count in `gSavedStackSize`.
2. It sets the stack pointer to `Ram_FrameWaitStackTop`.
3. It runs its frames there.
4. On return, if it saved anything, it sets the stack pointer back and
   copies the saved bytes back over it.

```
0x03007fa0  IRQ-mode stack top (the dispatcher only)
0x03007f00  system stack top: the game's code starts here and grows down
            ... a deep caller's frames, copied aside while it waits
0x03007a00  Ram_FrameWaitStackTop: WaitFrames runs its frames from here
            ... 512 bytes for the frame loop, its tasks and the VBlank work
0x03007800  Ram_IwramHeapEnd: the end of the IWRAM heap
```

The frame loop, the main-loop tasks and the VBlank work therefore always
start with 512 bytes above the heap, however deep the code that waits.

`WaitFrames` must not be called from inside a task: the nested call would find
its stack below the top again and overwrite `gSavedStack` and
`gSavedStackSize`, which still hold the outer caller's frames. `WaitFrames`
itself doesn't check, but it sets `gTaskStatus` while it runs the
main-loop tasks, and code a task may reach tests it before waiting
(`BattleObject_SpawnObjectsForList`). No task waits
([Evidence and open questions](#evidence-and-open-questions)).

| Name | Value | Meaning |
| --- | --- | --- |
| `Ram_FrameWaitStackTop` | `0x03007a00` | the stack pointer `WaitFrames` runs its frames from |
| `Ram_IwramHeapEnd` | `0x03007800` | the end of the IWRAM heap and the bottom of the stack |
| `gSavedStack` | 3,152 bytes | where a deep caller's stack waits, in EWRAM |

## The heap

The heap lends blocks of memory to the rest of the game: a map's working
state, a menu's buffers, a battle animation's pictures.

The heap works like a pile. Each new block goes on top of the last, and a
single mark, the next free address, says where the next one will start.
Giving a block back moves the mark down to that block's start, which also
gives back everything taken after it. That makes taking and giving back
almost free, at the price of one rule: blocks go back in the reverse of the
order they were taken. Since the whole heap is emptied before each map, a
block can never outlive the map, battle or minigame that took it.

The heap hands out memory from two pools, one in IWRAM from `gIwramHeap` up to
`Ram_IwramHeapEnd` and one in EWRAM from `gEwramHeap` up to the end of EWRAM.
Each pool is a bump allocator: a block starts at the pool's next free address,
which moves up past it. The two next-free addresses are the first two words of
`gHeapSlots` (`union HeapState`, `games/tbs/include/heap_state.h`), and the
other 62 words are the heap slots, which hold the blocks that have to be
found again later. `AllocHeapSlot` and `Alloc` take IWRAM first
and fall back to EWRAM; `AllocHeapSlotEwram` and
`AllocEwram` take EWRAM first. Sizes are rounded up to
words, a block may not end exactly at its pool's end, and an allocation that
fits in neither pool returns `NULL`. Blocks are not cleared.

Nothing is freed one block at a time. `FreeHeapSlot(slot)` clears
the slot and sets its pool's next free address back to the block's start;
`Free(p)` does the same with `p` (both in `system/heapfree.s`). The pool is
chosen from the address with no comparison: `(address >> 22) & 4` is 0 for an
EWRAM address and 4 for an IWRAM one, the byte offsets of the two next-free
words. Releasing a block therefore releases every later block in the same
pool, and the rule is that blocks are released in the reverse of the order
they were taken, pool by pool. A block released out of order does not fail at
once: the later blocks stay in their slots, pointing at memory the next
allocation hands out again. `Game_Run` empties both pools and clears every
slot before each map.

That rule is why a battle animation's transfer task, which copies its picture
to the screen every frame, has to be removed before the animation's heap blocks are
released, and why code copied into a heap block to run from RAM
(`RamCode_Load`) is released right after use. A slot that already has a block is returned whatever size is
asked for, so `AllocHeapSlot` also means "get".

The frame's scratch block is taken after the main-loop tasks have run and
released after the VBlank that copied it to OAM. For that frame it is the top
of its pool, so anything the VBlank work allocates from the same pool in
between is released with it.

| Name | Pool first | Meaning |
| --- | --- | --- |
| `AllocHeapSlot` | IWRAM | takes, or returns the block already in, a heap slot |
| `AllocHeapSlotEwram` | EWRAM | the same, EWRAM first |
| `Alloc` | IWRAM | takes a block with no heap slot, for short-lived buffers |
| `AllocEwram` | EWRAM | the same, EWRAM first |
| `FreeHeapSlot(slot)` | | releases a slot's block and every later block in its pool |
| `Free(p)` | | releases `p` and every later block in its pool |

## The task runner

The task runner keeps the list of tasks, the small jobs that have to happen
every frame whatever the rest of the code is doing, and runs them in a fixed
order.

A task is added once and then runs by itself until it is removed. It is a
`void (*)(void)` routine (`TaskFunc`), added with
`Task_Add(callback, priority)` and removed with
`Task_Remove(callback)`; [Engine](../../engine.md#waiting-and-tasks)
shows the battle camera being added. Some
belong in the frame's work, such as scrolling the map
(`MapGrid_UpdateLayerScroll`) or updating the windows (`UiWork_ProcessAll`);
others have to happen in the blank, such as copying the windows' changed
tiles to the screen (`UiWork_UploadDirtyBlocks`). Each task carries one
number, its priority, which says both: which of the two passes runs it, and
where in that pass. Where the priority matters, it matters a lot: in a battle the
camera (`BattleCamera_Update`) is added with a lower priority than
the battle's objects and playback, so it moves only after they have. On a map, the map's scrolling task has
the higher priority, so it runs before the windows' task.

The task table holds `TASK_COUNT` tasks, each a routine, a
priority halfword and flags. The priority's high byte picks the pass that runs the
task (`TASK_PRIORITY_MAIN_LOOP` for the frame loop, `TASK_PRIORITY_VBLANK` for
VBlank) and its low byte ranks it within the pass. Every add re-sorts the
table by priority, highest first, comparing priorities as signed halfwords
(`Task_Sort`), and `Task_RunPass` runs, in table order, every
task whose high byte equals the pass's.

The sort stops one pass short: it never compares slots 0 and 1, so when the
task in slot 1 has the higher priority, the two run in the wrong order ([`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix) in the code). It swaps only on strictly greater, so
tasks of equal priority keep their table order, which depends on the free slot
each was given; code that needs an order gives its tasks different priorities.

A task is suspended by setting bit 0 of its priority's high byte
(`Task_SuspendAll`), which no pass matches, and resumed by clearing it,
so it keeps its rank. An empty slot's priority is `TASK_PRIORITY_EMPTY` and a
removed task's `TASK_PRIORITY_REMOVED`; both high bytes are odd, and as signed
halfwords a removed slot sorts first and an empty one last.
`Task_SuspendOverlayTasks` suspends the tasks whose routine is in EWRAM,
where [overlays](../../glossary.md#overlay) run, except those whose flag bit 0 is set; the Battle Arena and
the Colosso maps set it (`Task_SetFlags`) for tasks that must keep
running under a menu.

Adding, removing, suspending and resuming disable interrupts, because the
VBlank pass reads the same table. A pass walks the table by index without a
lock, so a task that adds another during a pass re-sorts the table under it,
which can shift the tasks after it by one; whether the game ever does is an
open question.

The code is `runtime/scheduler.c`, its names in
`games/tbs/include/system/callback_scheduler.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `TASK_COUNT` | 20 | tasks in the table |
| `TASK_PRIORITY_MAIN_LOOP` | `0xc80` | priority of a main-loop task (high byte `0x0c`) |
| `TASK_PRIORITY_VBLANK` | `0x480` | priority of a VBlank task (high byte `0x04`) |
| `TASK_PRIORITY_REMOVED` | `0x7fff` | a removed task's priority; sorts first |
| `TASK_PRIORITY_EMPTY` | `0xffff` | an empty slot's priority; sorts last |

## The register write queue

The queue holds changes to the hardware's settings until the blank, so that a
frame's changes reach the screen together or not at all.

The hardware's registers decide how the screen is drawn: which layers show,
how two layers blend, where a window cuts the picture. Written during the
frame, a change would take effect partway down the screen, and a frame that
ran late would show some of its changes and not others. So code adds its
register writes to a queue, and the VBlank handler carries them out in the
blank, together with the sprites and the scroll, but only for a frame whose
work has finished. A late frame keeps its writes for the next blank.

The queue (`gIoWriteQueue`, `io_write_queue.h`) holds up to 32 entries, each a
value, an address and a control word; a write to a full queue is dropped.
Bits 16 and 17 of the control word are the width (`IO_WRITE_8`,
`IO_WRITE_16`, `IO_WRITE_32`; none means the entry is a DMA whose three words
go to DMA3) and bits 18 and 19 the operation (`IO_WRITE_SET` ORs the value in,
`IO_WRITE_CLEAR` clears its bits, neither stores it). The
`IoWriteQueue_Store`, `SetBits` and `ClearBits` writers each add one entry.

`IoWriteQueue_FlushPending` copies `Resource_ExecuteTransfer`
(`system/resource/decode_01.s`), the ARM routine that carries the entries out,
onto the stack, which is in IWRAM, and runs it there. ARM code runs fastest
from IWRAM, whose 32-bit bus fetches each 4-byte ARM instruction in one read,
where the cartridge's 16-bit bus takes two and waits on each.

| Name | Value | Meaning |
| --- | --- | --- |
| `IO_WRITE_8` | `1 << 16` | writes a byte |
| `IO_WRITE_16` | `2 << 16` | writes a halfword |
| `IO_WRITE_32` | `3 << 16` | writes a word |
| `IO_WRITE_SET` | `4 << 16` | ORs the value into the register |
| `IO_WRITE_CLEAR` | `8 << 16` | clears the value's bits in the register |

## Saving

Saving writes the new copy of a save before it lets go of the old one, so
switching the console off at the wrong moment never loses a save.

The cartridge keeps saves in flash memory, which is written a whole sector at
a time. If the game wrote a save over its old copy and the power went off
halfway, both would be lost. Instead each save goes into an empty sector with
a number one higher than the old copy's, and only once it is safely written
is the old copy marked free. Whatever moment the power goes, at least one
whole copy remains, and the game keeps the one with the higher number.

The cartridge's flash has 16 sectors of 4 KB, each holding one record: a
16-byte header (signature, record id, a checksum and a sequence number) and a
payload of 4,080 bytes, of which the checksum covers the first 4,072. A save
file is 8 KB, too large for one record, so file `n` of the three on the main
menu is two records: `n` holds the first half of the save buffer
and `n + 3` the second.

`Save_WriteRecord` writes the new copy, numbered one past the record's
highest sequence, into a free sector picked at random, and only then
invalidates the old copy. A power loss between the two leaves two good copies,
and the scan that starts every save, load, copy or delete
(`Save_InitializeWork`) keeps the one with the higher sequence. The
last free sector is kept for a record that already has a copy, so replacing a
save always has room. When the sequence passes `SAVE_COUNTER_LIMIT`, the
record is written again with sequence 1 into the old copy's sector, and the
high-numbered copy is invalidated only after that write has been verified.

The code is `runtime/save.c`; its workspace is the heap slot `HEAP_SLOT_SAVE`,
taken for the length of one save screen operation.

| Name | Value | Meaning |
| --- | --- | --- |
| `SAVE_SECTOR_COUNT` | 16 | flash sectors scanned |
| `SAVE_RECORD_NONE` | 16 | the record id of a free or invalidated sector |
| `SAVE_FILE_COUNT` | 3 | save files; file `n` is records `n` and `n + 3` |
| `SAVE_COUNTER_LIMIT` | 65,000 | the highest sequence a write keeps |
| `SAVE_CHECKSUM_BYTES` | `0xfe8` | payload bytes the checksum covers |
| `SAVE_FLASH_TIMER` | 2 | the hardware timer handed to the flash library |
| `SAVE_FLASH_IDENTIFY_ATTEMPTS` | 8 | tries to identify the flash chip, a frame apart |

## The link cable

Two consoles joined by a cable trade a little data every frame, and anything
larger, such as a [party](../../glossary.md#the-party) for a link battle, is sent in numbered pieces that
are sent again when one is lost.

Each frame, in the blank, the two consoles swap a small fixed message. Bulk
data rides inside it a block at a time, each block numbered. The receiver
keeps a block only if it is the one it expects next; if one goes missing, it
asks for it again, and the sender goes back to that block and sends the rest
once more. Link battles, the Battle Arena and the main menu's
transfer use it.

While the link is up, the VBlank handler trades a 24-byte payload with the
partner (`SerialRuntime_ExchangePayloads`) in multiplayer mode, the parent
console clocking the transfer from timer 3 a halfword at a time. Bulk data
rides on the exchange in 20-byte blocks, one each way per VBlank
(`SerialRuntime_StepBlockTransfer`, started by `SerialRuntime_StartSend` and
`SerialRuntime_StartReceive`). The receiver stores a block only when its
sequence number is the one it expects; a block out of step makes it ask for
the one it expects, and a sender that sees the request goes back to that
block and sends the rest again, so a send's source must stay unchanged until
the send is over. A send's size falls by 20 a block and the send ends only at
exactly 0, so the size must be a multiple of 20.

| Name | Value | Meaning |
| --- | --- | --- |
| `SERIAL_PAYLOAD_BYTES` | 24 | one console's payload each frame |
| `SERIAL_BLOCK_BYTES` | 20 | bulk data carried in each payload |
| `SERIAL_PACKET_HALFWORDS` | 14 | halfwords in one packet on the wire |
| `SERIAL_IDLE_HALFWORD` | `0xfefe` | sent and seen between packets |
| `SERIAL_TIMER_RELOAD` | `0xc963` | timer 3's reload, which clocks the parent's transfer |

## Design

The game is written as ordinary functions that run until they are done. A
battle animation, a cutscene or a menu screen is a function with its own loop,
and when time has to pass it calls `WaitFrames` and carries on from the same
place. Nothing returns to a main loop between frames. Work that must continue
while such a function waits is a task, which `WaitFrames` runs however deep
the waiting function is: window animation, the battle's playback
(`BattlePlayback_Run`) and a map's callbacks are tasks. Most of the
runtime's shape follows from that choice: the stack switch keeps the stack
bounded however deep the caller, task priorities give tasks a known order, and
heap slots let a task's blocks outlive the call that took them.

The second choice is that a frame's changes reach the hardware together. Code
writes RAM copies (the depth slots, `gBgScroll`, the write queue) and the
VBlank handler commits them only for a finished frame, so a lag frame shows
the previous picture whole rather than a mixture of two.

Saving trades flash space for safety: two sectors per record are in use only
while a write is in progress, and the random choice of sector spreads writes
across the chip, likely to spread the flash chip's wear, since nothing else
in the code needs it.

## How it fits with the rest of the game

Every other folder runs on the runtime. The field, the battles and the menus
let time pass through `WaitFrames` and keep their per-frame work as tasks;
their memory comes from heap slots; their changes to the screen
go through the depth slots, `gBgScroll` and the write queue. The other banks
call the runtime's functions directly, and overlays reach them through the
runtime bank's far-call table
([System](../system/README.md#why-the-program-is-cut-into-banks)).
`Game_Run`, which resets the runtime before every map, is in
[Running a map](../object/README.md), the `object` folder.

## Limits

- 62 heap slots; the IWRAM heap ends at `Ram_IwramHeapEnd` and the EWRAM heap
  at the end of EWRAM.
- 20 tasks at a time.
- 32 queued register writes a frame.
- 96 sprite VRAM cache entries over 512 blocks of 64 bytes, and at most 8 KB
  (128 blocks, a quarter of sprite VRAM) for one entry.
- 256 depth slots and 32 affine matrices a frame; once all 32 are taken,
  `OamMatrix_AllocFromTransform` returns 0 and the sprite uses whatever matrix
  0 holds.
- 512 bytes of stack for the frame loop, its tasks and the VBlank work.
- 16 flash sectors for the three save files' six records.

## Files

| File | What it holds |
| --- | --- |
| `runtime.c` | the runtime object: it includes `runtime_internal.h`, then the parts in `runtime/` in ROM order; its opening comment says where each subject starts |
| `runtime_internal.h` | what the parts share: the object's includes and the one prototype two parts use |
| `runtime/*.c` | the parts, one family of functions each |
| `math_table.c` | a quarter-wave sine table with the same values as `Trig_SineTable` in `trig.s`, the copy `Trig_Sin` reads; nothing reads this one by name |
| `numtext.c` | the hexadecimal digits and the powers of ten that number text uses |
| `trig.s`, `q16_dot_product.s` | the sine and cosine, and a 16.16 dot product, in assembly; each header gives its evidence: `trig.s` branches on a shift's carry, and `q16_dot_product.s` has a multiply sequence no [compiler](../../glossary.md#the-compilers) pattern emits, which its header notes does not alone prove the whole file handwritten |

The parts are [`decompress.c`](runtime/decompress.c), [`fade.c`](runtime/fade.c),
[`frame.c`](runtime/frame.c), [`heap.c`](runtime/heap.c),
[`io_queue.c`](runtime/io_queue.c), [`math.c`](runtime/math.c),
[`render.c`](runtime/render.c), [`save.c`](runtime/save.c),
[`scheduler.c`](runtime/scheduler.c), [`serial.c`](runtime/serial.c),
[`system.c`](runtime/system.c), [`text.c`](runtime/text.c),
[`transform.c`](runtime/transform.c) and
[`vram_cache.c`](runtime/vram_cache.c); the second line of each says what it
holds.

The ROM shows the runtime as one object, so it is compiled once, through
`runtime.c`. The split into parts is ours, made where one family of functions
ends and the next begins; each part also includes the private header so that
its names resolve when it is read on its own. The object's place in the link
order is in `games/tbs/objects.tsv`, from the `trig` row on.

The prototypes the rest of the game uses are in
`games/tbs/include/unit/runtime/`. Overlays reach many of these functions
through the far-call table `games/tbs/src/system/far_call/RUNTIME.S`, under
names ending in `Far`, so renaming one touches other folders too.

## Words used here

Frame, VBlank, IWRAM, EWRAM, heap slot, task, sprite and OAM are in the
[glossary](../../glossary.md).

- **16.16 fixed point**: a word read as a whole part in its upper 16 bits and
  a fraction in its lower 16, so 65,536 is 1.0. Positions, scales and the
  transform use it.
- **angle**: a whole turn is 65,536 steps. `Trig_Sin` and `Trig_Cos` return
  16.16 values from a table of 256 entries for the first quarter turn, one per
  1/1024 of a turn; `ArcTan2` measures from the y axis, 0 along +y and a
  quarter turn along +x.
- **queued write**: a register write, or a DMA, that the VBlank handler
  carries out once the frame loop has finished a frame (`io_queue.c`).
- **depth slot**: one of 256 lists sprites are linked into each frame, which
  the OAM build walks in order (`render.c`).
- **cache entry**: one of the 96 entries of the sprite VRAM cache. Sprite tile
  memory, the last 32 KB of VRAM, is split into 512 blocks of 64 bytes, and
  each entry owns a run of them; an entry whose offset is
  `VRAM_CACHE_OFFSET_FREE` is free (`vram_cache.c`).
- **transform**: `gTransform`, twelve `s32` in 16.16: a 3x3 rotation and a
  translation, with a one-level stack (`transform.c`). The battle and field
  cameras work through it.
- **record**: one save record in one 4 KB flash sector (`save.c`).
- **payload** and **block**: each VBlank two linked consoles trade a 24-byte
  payload, and bulk data rides on it in 20-byte blocks (`serial.c`).

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `System_`, `Runtime_`, `Code_` | power-up, interrupts, the heap and code loading |
| `Input_` | key repeat and the keypad interrupt |
| `IoWriteQueue_` | the register write queue |
| `ScreenFade_` | screen fades through the blend registers |
| `Render_`, `OamMatrix_` | the frame's sprite lists and affine matrices |
| `VramCache_` | the sprite VRAM cache |
| `Task_` | tasks |
| `Math_`, `Vector_` | square roots and polar offsets beside `Random16` and `ArcTan2` |
| `FixedPoint_` | 16.16 and 8.8 arithmetic: a remainder, a product, a reciprocal and the distance between two positions |
| `Transform_`, `Projection_` | the 3D transform and projection |
| `NumberText_`, `DebugText_` | number text and the debug text screen |
| `ColorBuffer_` | shading a buffer of pixels, keeping a copy |
| `Save_` | saving |
| `SerialRuntime_`, `SerialTest_`, `LinkBattle_` | the link cable |
| `RuntimeDispatch_` | empty far-call exports |
| `Resource_`, `Palette_`, `Ui_`, `Sound_` | resources, palettes, the interface and sound, whose main code lives in other folders |

[Matching](../MATCHING.md) explains the odd spellings and the comment markers.

## Evidence and open questions

The names come from what the code does: `WaitFrames` from its loop,
`Task_` from the table it keeps, the heap's from its two pointers and its
heap slots, the save code's from the flash records it writes. Constants such as
`TASK_PRIORITY_MAIN_LOOP` are the values the callers pass.

[XXX](../../glossary.md#xxx): likely the VBlank handler's display writes come first so that they land
inside the vertical blank, and the mixer last because it is long and need
only finish within the frame; the bytes don't say so.

XXX: whether the game ever adds a task during a pass, which would shift the
tasks after it by one, isn't known.

XXX: why `IoWriteQueue_FlushPending` runs its routine from the stack rather
than a heap block isn't known; a block allocated inside the interrupt would
sit above the frame's scratch block and be released with it.

XXX: likely the save code's random choice of sector is meant to spread the
flash chip's wear, because nothing else in the code needs it; the bytes
don't say so.

`gSavedStack` holds 3,152 bytes, though a stack below the frame-wait top and
above the heap's end is at most 512. `ewram.s` lays EWRAM out in 4 KB groups
(`gMapLocalState` opens the second, `gSleepActive` the third, the music state the
fourth), and `gSavedStack` is the last cell of the third, sized to reach the
fourth. It could hold a stack that had run 2,640 bytes into the heap.

No task waits. A call graph of the tbs-en build's objects, [main image](../../glossary.md#main-image) and
overlays, from each function the code adds as a task, each object
hook and each `SC_CALL` callback, finds nine tasks with a path of calls to
`WaitFrames`, and none takes it: the map tasks turn [object events](../../glossary.md#object-event) with a wait of 0
(`Engine_ObjectEventFaceDirection`), the world map's lighthouse jitter moves the
camera with a glide, Lucky Wheels and the link countdown open instant
windows (`WINDOW_FLAG_INSTANT`), and battle playback's sprite respawn tests
`gTaskStatus`. The tenth path is the debug map selector, which only
the unused `DebugMapSelect_Schedule` would add as a task.

- XXX (no evidence): what the five empty exports after the screen fades
  (`RuntimeDispatch_ReservedNoOpA` to `E`) were for; they are empty in both
  games, nothing calls them, and each is reached only through a bare veneer,
  so no code shows what they were to do.
