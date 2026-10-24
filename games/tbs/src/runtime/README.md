# Runtime

When a cutscene pauses for half a second, the code that runs it simply waits,
yet the screen keeps moving: windows still type, the [map](../../glossary.md#map)
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
once, then hands over to the loop of maps for as long as the console is on.

The cartridge header jumps to `System_Entry` (`games/tbs/src/system/entry.s`),
which sets the IRQ and system stacks at the top of
[IWRAM](../../glossary.md#iwram-and-ewram) and calls `System_Initialize`
(`runtime/system.c`). In order, that:

1. stops any HBlank DMA (a copy the console makes after each line) left
   running and sets the cartridge wait states;
2. clears IWRAM up to `Ram_IwramHeapEnd`, leaving the stacks above it, and
   empties the heap (`InitHeap`);
3. copies the resident runtime into IWRAM and points the interrupt vector at
   its dispatcher (`System_InstallIwramAndIrqs`);
4. copies the build stamp and loads the window graphics;
5. installs `System_VBlankHandler`, arms the soft-reset keys, starts the sound
   driver, and empties the sprite VRAM cache and the task table;
6. waits ten frames and calls `Game_Run` through its
   [far-call](../../glossary.md#far-call) entry, `Game_RunFar`.

The German, French, Spanish and Italian [editions](../../glossary.md#editions)
(`TBS_PATCH_2`) also install `Iwram_Halt` for the cartridge-removed interrupt.
The soft-reset keys raise the keypad interrupt; `Input_HandleKeyInterrupt`
sets `gResetRequested`, and the frame loop jumps to `Cart_Restart`. `Game_Run`
([Running a map](../overworld/README.md)) never returns.

## The frame loop

The screen is redrawn about sixty times a second, and each frame has two
halves: the game's work, then a wait for the vertical blank (VBlank), the
pause after the last line, in which everything lands on screen at once.
Changing the picture while it is drawn would show half the change, so code
writes new positions, scroll and colours to RAM during the frame and the
VBlank handler copies them to the hardware together.

`WaitFrames(n)` (`runtime/frame.c`) runs `n` frames. During a frame, code links
each sprite it wants shown into one of 256 depth slots (`gOamDepthLists`).
Each frame `WaitFrames`:

1. runs the main-loop tasks (`Task_RunPass(TASK_PRIORITY_MAIN_LOOP)`);
2. builds the OAM list in the [heap slot](../../glossary.md#heap-slot)
   `HEAP_SLOT_FRAME_SCRATCH` by walking the depth slots (`Oam_BuildBuffer`) and
   sets `gFrameRenderPending`, marking the frame finished;
3. measures CPU load for the debug display when it is on, and counts frames
   toward sleep;
4. halts the CPU until VBlank (`swi 2`, the BIOS's Halt);
5. releases the scratch block, resets the depth slots, counts the frame and
   updates key repeat (`Input_UpdateKeyRepeatAndDirection`);
6. sleeps, or restarts the game, if either was asked for.

`System_VBlankHandler`, reached through the IWRAM dispatcher and
`gIrqHandlers`, stops any HBlank DMA, trades the link payloads and steps the
music's fades and the screen fade; then, only if `gFrameRenderPending` is set,
copies the OAM list, the four [background layers](../../glossary.md#background-layer)'
scroll from `gBgScroll` and the queued register writes, and clears the flag;
then runs the one-shot `gVBlankCallback` and the VBlank tasks; and last reads
the keypad, sets `gVBlankDone` and mixes the next sound buffer.

So a frame whose work runs late is not committed, and the screen keeps the
previous frame's sprites, scroll and register writes together. Sprites placed
by a VBlank task show a frame later, since the OAM list is built after the
main pass, and keys read in VBlank reach code at the start of its next frame.

| Name | Value | Meaning |
| --- | --- | --- |
| `TASK_PRIORITY_MAIN_LOOP` | `0xc80` | the pass `WaitFrames` runs each frame |
| `TASK_PRIORITY_VBLANK` | `0x480` | the pass the VBlank handler runs |
| `HEAP_SLOT_FRAME_SCRATCH` | 52 | the heap slot the frame's OAM list is built in |
| `RENDER_DEPTH_SLOT_COUNT` | 256 | depth slots a sprite is linked into |
| `OBJ_AFFINE_MATRIX_COUNT` | 32 | rotation-scaling matrices a frame |
| `CPU_LOAD_PEAK_FRAMES` | 30 | frames the debug display holds a CPU load peak |

### Buttons and sleep

![Isaac stands at the Sanctum's door in Vale while only L and R are held; after three seconds the screen turns white as the console goes to sleep](https://alchemy.pascalpixel.com/guides/runtime/sleep.gif)

Holding L and R alone for three seconds puts the console to sleep, with the
screen off and the battery spared until L and R wake it. The frame loop
counts the frames each button combination has been held, and also repeats a
held button, as when the player holds Down in a menu.

A held key repeats after `INPUT_KEY_REPEAT_FIRST` frames and then every
`INPUT_KEY_REPEAT_NEXT`. `gKeysRepeatDirected` is the repeat with at most one
direction: of several held directions it keeps the one added last, and with
all four none. Holding only L and R for `SLEEP_COMBO_FRAMES`, or no key for
`SLEEP_IDLE_FRAMES` while `gIdleSleepEnabled` is set, turns the screen white
for `SLEEP_WHITE_FRAMES`, stops the CPU (`swi 3`, the BIOS's Stop) with the
keypad set to wake it on L and R, then restores the display and waits
`SLEEP_WAKE_FRAMES`. Nothing sleeps while `gSleepDisabled` is set. In debug
mode (`gDebugMode`), Start and Select held pause the loop, and A, B, Select or
the D-pad step one frame.

| Name | Value | Meaning |
| --- | --- | --- |
| `INPUT_KEY_REPEAT_FIRST` | 19 | frames a key is held before its first repeat |
| `INPUT_KEY_REPEAT_NEXT` | 6 | frames between later repeats |
| `SLEEP_COMBO_FRAMES` | `3 * 60` | three seconds holding only L and R |
| `SLEEP_IDLE_FRAMES` | `3 * 60 * 60` | three minutes with no key held |
| `SLEEP_WHITE_FRAMES` | 60 | the white screen before the Stop |
| `SLEEP_WAKE_FRAMES` | 10 | frames waited after waking |

## The stack switch in `WaitFrames`

The stack is the scratch memory every function call takes a little of. It
grows down from the top of IWRAM towards the heap, 1,792 bytes of it, and
interrupts use it too: the dispatcher, `IwramIrqMain` (`system/runtime.s`),
switches to system mode before calling a handler, so the VBlank handler and
the sound mixer run below whatever they interrupted. Code can call
`WaitFrames` from deep inside its own calls, and the tasks it runs need stack
of their own, so `WaitFrames` sets the deepest part aside:

1. If the stack pointer is below `Ram_FrameWaitStackTop`, 512 bytes above the
   heap's end, it copies the bytes from there up to `Ram_FrameWaitStackTop`
   into `gSavedStack` in EWRAM by DMA and keeps their count in
   `gSavedStackSize`.
2. It sets the stack pointer to `Ram_FrameWaitStackTop` and runs its frames.
3. On return it puts the saved bytes back and the stack pointer with them.

The frame loop, the main-loop tasks and the VBlank work therefore always
start with 512 bytes, however deep the waiting code. `WaitFrames` must not be
called from a task: the nested call would overwrite `gSavedStack`, which still
holds the outer caller's frames. `WaitFrames` doesn't check, but it sets
`gTasksRunning` while it runs the main-loop tasks, and code a task may reach
tests it before waiting (`BattleObject_SpawnObjectsForList`).

| Name | Value | Meaning |
| --- | --- | --- |
| `Ram_FrameWaitStackTop` | `0x03007a00` | the stack pointer `WaitFrames` runs its frames from |
| `Ram_IwramHeapEnd` | `0x03007800` | the end of the IWRAM heap and the bottom of the stack |
| `gSavedStack` | 3,152 bytes | where a deep caller's stack waits, in EWRAM |

## The heap

The heap lends blocks of memory to the rest of the game: a map's working
state, a menu's buffers, a battle animation's pictures. It works like a pile:
each block goes on top of the last, and giving one back gives back everything
taken after it. That makes both almost free, at the price of one rule: blocks
go back in the reverse of the order they were taken. The whole heap is
emptied before each map, so no block outlives the map that took it.

There are two pools, one in IWRAM from `gIwramHeap` up to `Ram_IwramHeapEnd`
and one in EWRAM from `gEwramHeap` to the end of EWRAM, each a bump
allocator. Their next-free addresses are the first two words of `gHeapSlots`
(`union HeapState`, `games/tbs/include/heap.h`); the other 62 words are the
heap slots, which hold blocks that have to be found again. A slot that already
has a block returns it whatever size is asked for, so `AllocHeapSlot` also
means "get". Sizes round up to words, blocks are not cleared, and an
allocation that fits in neither pool returns `NULL`.

`FreeHeapSlot` and `Free` (`system/heapfree.s`) set the pool's next free
address back to the block's start; the pool comes from the address with no
comparison, as `(address >> 22) & 4`, 0 for EWRAM and 4 for IWRAM, the offsets
of the two next-free words. A block released out of order doesn't fail at
once: later blocks stay in their slots, pointing at memory the next
allocation hands out again. That is why a battle animation's transfer task is
removed before its blocks are released, and why code copied into a block to
run from RAM (`RamCode_Load`) is released right after use.

| Name | Pool first | Meaning |
| --- | --- | --- |
| `AllocHeapSlot` | IWRAM | takes, or returns the block already in, a heap slot |
| `AllocHeapSlotEwram` | EWRAM | the same, EWRAM first |
| `Alloc` | IWRAM | takes a block with no heap slot, for short-lived buffers |
| `AllocEwram` | EWRAM | the same, EWRAM first |
| `FreeHeapSlot(slot)` | | releases a slot's block and every later block in its pool |
| `Free(p)` | | releases `p` and every later block in its pool |

## The task runner

Some work has to happen every frame whatever else is going on: scrolling the
map (`MapGrid_UpdateLayerScroll`), updating the windows (`WindowSystem_Update`),
copying their changed tiles in the blank (`WindowSystem_UploadTilemap`). Each
is a task, added once with `Task_Add(callback, priority)` and run until
`Task_Remove(callback)` or the next map. The priority says which of the two
passes runs it and where in that pass, which matters: a battle's camera
(`BattleCamera_Update`) has a lower priority than the battle's objects, so it
moves after they have.

The table holds `TASK_COUNT` tasks (`runtime/scheduler.c`, names in
`games/tbs/include/system/task.h`). The priority's high byte picks the pass and
its low byte ranks the task within it. Every add re-sorts the table, highest
first, comparing priorities as signed halfwords (`Task_Sort`), and
`Task_RunPass` runs every task whose high byte matches. The sort never
compares slots 0 and 1, so when slot 1 has the higher priority the two run in
the wrong order ([`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix)); equal
priorities keep their table order.

Setting bit 0 of a priority's high byte suspends a task (`Task_SuspendAll`),
since no pass matches an odd high byte, and clearing it resumes the task at
its old rank. `Task_SuspendOverlayTasks` suspends the tasks whose routine is
in EWRAM, where [overlays](../../glossary.md#overlay) run, except those whose
flag bit 0 is set (`Task_SetFlags`), which the Battle Arena and the Colosso
maps use for tasks that must keep running under a menu. Adding, removing,
suspending and resuming disable interrupts, because the VBlank pass reads the
same table.

| Name | Value | Meaning |
| --- | --- | --- |
| `TASK_COUNT` | 20 | tasks in the table |
| `TASK_PRIORITY_MAIN_LOOP` | `0xc80` | a main-loop task (high byte `0x0c`) |
| `TASK_PRIORITY_VBLANK` | `0x480` | a VBlank task (high byte `0x04`) |
| `TASK_PRIORITY_REMOVED` | `0x7fff` | a removed task's priority; sorts first |
| `TASK_PRIORITY_EMPTY` | `0xffff` | an empty slot's priority; sorts last |

## The register write queue

The hardware's registers decide how the screen is drawn: which layers show,
how two blend, where a window cuts the picture. Code adds its register writes
to a queue, and the VBlank handler carries them out in the blank with the
sprites and the scroll, only for a frame whose work has finished, so a frame's
changes reach the screen together or not at all.

The queue (`gIoWriteQueue`, `io_write_queue.h`) holds 32 entries of a value,
an address and a control word; a write to a full queue is dropped. Bits 16 and
17 of the control word are the width (none means a DMA whose three words go to
DMA3) and bits 18 and 19 the operation. `IoWriteQueue_FlushPending` copies the
ARM routine that carries the entries out, `Resource_ExecuteTransfer`
(`system/resource/decode_tagged_lz.s`), onto the stack, which is in IWRAM,
and runs it there: IWRAM's 32-bit bus fetches an ARM instruction in one read,
the cartridge's 16-bit bus in two.

| Name | Value | Meaning |
| --- | --- | --- |
| `IO_WRITE_8` | `1 << 16` | writes a byte |
| `IO_WRITE_16` | `2 << 16` | writes a halfword |
| `IO_WRITE_32` | `3 << 16` | writes a word |
| `IO_WRITE_SET` | `4 << 16` | ORs the value into the register |
| `IO_WRITE_CLEAR` | `8 << 16` | clears the value's bits in the register |

## Saving

![In Vale, Start opens the pause menu; Save your quest is chosen, then the first of three empty files, and the game says "Saving..." and then "Your game was saved."](https://alchemy.pascalpixel.com/guides/runtime/save.gif)

Saving writes the new copy of a save before it lets go of the old one, so
switching off at the wrong moment never loses a save. The cartridge's flash
memory is written a sector at a time; each save goes into an empty sector
numbered one higher than the old copy, and only once it is written is the old
copy marked free. Whenever the power goes, one whole copy remains, and the
game keeps the higher number.

The flash has 16 sectors of 4 KB, each holding one record: a 16-byte header
(signature, record id, checksum, sequence number) and a 4,080-byte payload. A
save file is 8 KB, so file `n` of the three is two records, `n` and `n + 3`.
`Save_WriteRecord` writes the new copy into a free sector picked at random and
only then invalidates the old one; the scan that starts every save, load, copy
or delete (`Save_InitializeWork`) keeps the copy with the higher sequence. The
last free sector is kept for a record that already has a copy, so replacing a
save always has room. Past `SAVE_COUNTER_LIMIT` the record is written again
with sequence 1, and the old copy is invalidated only once that write is
verified. The code is `runtime/save.c`, its workspace the heap slot
`HEAP_SLOT_SAVE`; the Save command itself is `Save_ToChosenFile`
([Windows](../window/README.md)).

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

Two consoles joined by a cable trade a small fixed message every frame, and
anything larger, such as a [party](../../glossary.md#the-party) for a link
battle, rides inside it in numbered blocks that are sent again when one goes
missing. Link battles, the Battle Arena and the main menu's transfer use it.

While the link is up, the VBlank handler trades a 24-byte payload with the
partner (`Serial_ExchangePayloads`) in multiplayer mode, the parent console
clocking the transfer from timer 3. Bulk data rides on it in 20-byte blocks,
one each way per VBlank (`Serial_StepBlockTransfer`, started by
`Serial_StartSend` and `Serial_StartReceive`). The receiver keeps a block only
when its sequence number is the one it expects, and otherwise asks for that
one; the sender goes back to it and sends the rest again, so a send's source
must not change until the send is over. A send ends only when its size reaches
exactly 0, falling by 20 a block, so the size must be a multiple of 20.

| Name | Value | Meaning |
| --- | --- | --- |
| `SERIAL_PAYLOAD_BYTES` | 24 | one console's payload each frame |
| `SERIAL_BLOCK_BYTES` | 20 | bulk data carried in each payload |
| `SERIAL_PACKET_HALFWORDS` | 14 | halfwords in one packet on the wire |
| `SERIAL_IDLE_HALFWORD` | `0xfefe` | sent and seen between packets |
| `SERIAL_TIMER_RELOAD` | `0xc963` | timer 3's reload, which clocks the parent's transfer |

## Design

The game is written as ordinary functions that run until they are done. A
battle animation, a cutscene or a menu screen is a function with its own loop
that calls `WaitFrames` when time has to pass and carries on from the same
place; nothing returns to a main loop between frames. Most of the runtime's
shape follows: tasks keep the world moving while such a function waits, the
stack switch keeps the stack bounded however deep it waits, task priorities
give tasks a known order, and heap slots let a task's blocks outlive the call
that took them.

The second choice is that a frame's changes reach the hardware together: code
writes RAM copies (the depth slots, `gBgScroll`, the write queue) and the
VBlank handler commits them only for a finished frame, so a lag frame shows
the previous picture whole rather than a mixture of two.

The ROM shows the runtime as one object, compiled once through `runtime.c`;
the parts are our cut of it. Overlays reach many of its functions through the
far-call table `system/far_call/RUNTIME.S`, so renaming one touches other
folders too. `math_table.c` holds a quarter-wave sine table with the same
values as `Trig_SineTable` in `trig.s`, the copy `Trig_Sin` reads; nothing
reads this one by name.

## How it fits with the rest of the game

Every other folder runs on the runtime: the field, the battles and the menus
let time pass through `WaitFrames`, keep their per-frame work as tasks, take
their memory from heap slots and reach the screen through the depth slots,
`gBgScroll` and the write queue. Other banks call the runtime directly, and
overlays through its far-call table
([System](../system/README.md#why-the-program-is-cut-into-banks)).

## Limits

- 62 heap slots; the IWRAM heap ends at `Ram_IwramHeapEnd`, the EWRAM heap at
  the end of EWRAM.
- 20 tasks at a time.
- 32 queued register writes a frame.
- 96 sprite VRAM cache entries over 512 blocks of 64 bytes, and at most 8 KB
  (128 blocks) for one entry.
- 256 depth slots and 32 affine matrices a frame; once all 32 are taken,
  `OamMatrix_AllocFromTransform` returns 0 and the sprite uses whatever matrix
  0 holds.
- 512 bytes of stack for the frame loop, its tasks and the VBlank work.
- 16 flash sectors for the three save files' six records.

## Words used here

Frame, VBlank, IWRAM, EWRAM, heap slot, task, sprite and OAM are in the
[glossary](../../glossary.md).

- **16.16 fixed point**: a word read as a whole part in its upper 16 bits and
  a fraction in its lower 16, so 65,536 is 1.0. Positions, scales and the
  transform use it.
- **angle**: a whole turn is 65,536 steps. `Trig_Sin` and `Trig_Cos` return
  16.16 values from a table of 256 entries for the first quarter turn;
  `ArcTan2` measures from the y axis, 0 along +y and a quarter turn along +x.
- **queued write**: a register write, or a DMA, that the VBlank handler
  carries out once the frame loop has finished a frame (`io_queue.c`).
- **depth slot**: one of 256 lists sprites are linked into each frame, which
  the OAM build walks in order (`render.c`).
- **cache entry**: one of the 96 entries of the sprite VRAM cache, each owning
  a run of the 512 blocks of 64 bytes sprite tile memory is split into; an
  entry whose offset is `VRAM_CACHE_OFFSET_FREE` is free (`vram_cache.c`).
- **transform**: `gTransform`, twelve `s32` in 16.16, a 3x3 rotation and a
  translation, with a one-level stack (`transform.c`). The battle and field
  cameras work through it.
- **record**: one save record in one 4 KB flash sector (`save.c`).
- **payload** and **block**: each VBlank two linked consoles trade a 24-byte
  payload, and bulk data rides on it in 20-byte blocks (`serial.c`).

## Evidence

The names come from what the code does: `WaitFrames` from its loop, `Task_`
from the table it keeps, the heap's from its two pointers and its heap slots,
the save code's from the flash records it writes. Constants such as
`TASK_PRIORITY_MAIN_LOOP` are the values the callers pass.

`gSavedStack` holds 3,152 bytes, though a stack below the frame-wait top and
above the heap's end is at most 512. `ewram.s` lays EWRAM out in 4 KB groups
(`gMapLocalState` opens the second, `gSleepActive` the third, the music state
the fourth), and `gSavedStack` is the last cell of the third, sized to reach
the fourth.

No task waits. A call graph of the tbs-en build's objects, main image and
overlays, from each function the code adds as a task, each object hook and
each `SC_CALL` callback, finds nine tasks with a path of calls to
`WaitFrames`, and none takes it: the map tasks turn object events with a wait
of 0 (`ObjectMotion_ArmCallback`), the world map's lighthouse jitter moves the
camera with a glide, Lucky Wheels and the link countdown open instant windows
(`WINDOW_FLAG_INSTANT`), and battle playback's sprite respawn tests
`gTasksRunning`. The tenth path is the debug map selector, which only the
unused `DebugMapSelect_Schedule` would add as a task.
