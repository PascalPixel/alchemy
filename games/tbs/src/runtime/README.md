# Runtime

![One frame of Vale with each of its nine sprite list entries outlined in its own colour, and beside it the same entries with the backgrounds hidden: the stone pillar is one tall sprite, and each villager a square sprite with a small one for its shadow.](https://alchemy.pascalpixel.com/guides/runtime/oam.png)

The runtime is the layer every other part of the game runs on: power-up, the
frame loop and the VBlank handler, input, the task scheduler, the heap, the
register write queue, fades, the frame's sprite lists and the sprite VRAM
cache, the 3D transform, saving and the link cable. Code everywhere else lets
time pass through one function, `WaitFrames`. The memory map it works in, and
the resident IWRAM code it installs, are in [System](../system/README.md).

## Files

| File | What it holds |
| --- | --- |
| `runtime.c` | the runtime object: it includes `runtime_internal.h`, then the parts in `runtime/` in ROM order; its opening comment says where each subject starts |
| `runtime_internal.h` | what the parts share: the object's includes and the one prototype two parts use |
| `runtime/*.c` | the parts, one family of functions each |
| `math_table.c` | a quarter-wave sine table with the same values as `Trig_SineTable` in `trig.s`, the copy `Trig_Sin` reads; nothing reads this one by name |
| `numtext.c` | the hexadecimal digits and the powers of ten that number text uses |
| `trig.s`, `q16_dot_product.s` | the sine and cosine, and a 16.16 dot product, in assembly; each header gives its evidence: `trig.s` branches on a shift's carry, and `q16_dot_product.s` has a multiply sequence no compiler pattern emits, which its header notes does not alone prove the whole file handwritten |

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
`games/tbs/include/unit/runtime/`. Other banks reach many of these functions
through the far-call table `games/tbs/src/system/far_call/RUNTIME.S`, under
names ending in `Far`, so renaming one touches other folders too.

## Words used here

- **frame** and **VBlank**: the screen is redrawn about sixty times a second,
  each picture a frame. The vertical blank (VBlank) is the pause after the
  last line of one picture, signalled by an interrupt; it is when the game
  changes what is on screen.
- **IWRAM** and **EWRAM**: the console's 32 KB of fast internal RAM and 256 KB
  of slower external RAM.
- **16.16 fixed point**: a word read as a whole part in its upper 16 bits and
  a fraction in its lower 16, so 65,536 is 1.0. Positions, scales and the
  transform use it.
- **angle**: a whole turn is 65,536 steps. `Trig_Sin` and `Trig_Cos` return
  16.16 values from a table of 256 entries for the first quarter turn, one per
  1/1024 of a turn; `ArcTan2` measures from the y axis, 0 along +y and a
  quarter turn along +x.
- **heap slot**: one of the 62 named long-lived blocks (`enum HeapSlot`,
  `HEAP_SLOT_*`) in `gWorkSlot`.
- **task**: a routine the scheduler runs once every frame from `WaitFrames`,
  or once every VBlank, in the order of its order value (`scheduler.c`).
- **queued write**: a register write, or a DMA, that the VBlank handler
  carries out once the frame loop has finished a frame (`io_queue.c`).
- **sprite** and **OAM**: sprites are the small pictures the hardware draws
  over the backgrounds; OAM is the hardware's table of them, rebuilt every
  frame.
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
| `Fade_` | screen fades through the blend registers |
| `Render_`, `AffineMatrix_` | the frame's sprite lists and affine matrices |
| `VramCache_` | the sprite VRAM cache |
| `Scheduler_` | the task scheduler |
| `Math_`, `Vector_` | square roots and polar offsets beside `Random16` and `ArcTan2` |
| `Transform_`, `SceneTransform_`, `Projection_` | the 3D transform and projection |
| `NumberText_`, `DebugText_` | number text and the debug text screen |
| `ColorBuffer_` | shading a buffer of pixels, keeping a copy |
| `SaveState_` | saving |
| `SerialRuntime_`, `SerialTest_`, `LinkBattle_` | the link cable |
| `RuntimeDispatch_` | empty far-call exports |
| `Resource_`, `Palette_`, `Ui_`, `Sound_` | resources, palettes, the interface and sound, whose main code lives in other folders |

[Matching](../MATCHING.md) explains the odd spellings and the comment markers.

## Start-up

The cartridge header jumps to `System_Entry`
(`games/tbs/src/system/entry.s`), which sets the IRQ and system stacks at the
top of IWRAM and calls `System_Initialize` (`runtime/system.c`). That
function stops any HBlank DMA left running, sets the cartridge wait states and
clears IWRAM up to `Ram_IwramHeapEnd`, leaving the stacks above it alone. It
empties the heap (`Runtime_InitializeHeap`), copies the resident runtime into
IWRAM and points the interrupt vector at its dispatcher
(`Runtime_InstallIwramAndIrqs`), copies the build stamp and loads the window
graphics. It then installs `System_VBlankHandler`, arms the soft-reset keys,
starts the sound driver, resets the sprite VRAM cache and the task table,
waits ten frames and calls `Game_RunFar`. The German, French, Spanish and
Italian editions (`TBS_PATCH_2`) also install `IwramHalt` for the
cartridge-removed interrupt.

Neither function returns. `Game_Run` (`games/tbs/src/object/object/scene.c`)
runs one scene after another, starting at the title, and before each one it
resets the task table, removes the HBlank and VCount handlers, empties the heap
and resets the VRAM cache.

## The frame loop

`WaitFrames(n)` (`runtime/frame.c`) runs `n` frames and returns. If the
caller's stack has grown below `Ram_FrameWaitStackTop`, the stack is first
moved (see below). Each frame then:

1. runs the main-loop tasks (`Scheduler_RunPass(SCHEDULER_KEY_MAIN_LOOP)`);
2. allocates `HEAP_SLOT_FRAME_SCRATCH` (1 KB), builds the OAM list in it from
   the depth slots (`Render_BuildOamList`) and sets `gFrameRenderPending`;
3. measures CPU load for the debug display when it is on, and counts the
   frames toward sleep;
4. halts the CPU until the VBlank handler has run (`swi 2`, the BIOS's Halt);
5. releases the scratch block, resets the depth slots, counts the frame and
   updates key repeat (`Input_UpdateKeyRepeatAndDirection`);
6. sleeps, or restarts the game, if either was requested.

VBlank interrupts the halt. `System_VBlankHandler`, reached through the IWRAM
dispatcher and `gIrqHandlers`, stops any HBlank DMA, trades the link payloads
and steps the music's fades and the screen fade. Only if
`gFrameRenderPending` is set does it copy the frame's OAM list (when
`gOamCopyEnabled`), the four BG scroll registers from `gBgScroll` and the
queued register writes to the hardware, and clear the flag. It then runs and
clears the one-shot `gVBlankHook`, runs the VBlank tasks
(`SCHEDULER_KEY_VBLANK`), reads the keypad, counts the frame, sets
`gVBlankDone` and mixes the next sound buffer.

The order has consequences for the code that runs in a frame:

- Tasks never write sprite or scroll registers directly. They write RAM, and
  VBlank commits a whole frame at once. A frame whose work does not finish
  before VBlank (a lag frame) is not committed, so the screen keeps the
  previous frame's sprites, scroll and register writes together.
- Sprites must be placed by main-loop tasks, or by the waiting code before it
  calls `WaitFrames`; the OAM list is built right after the main pass, so a
  VBlank task's sprites show a frame later.
- The keys are read in VBlank and turned into repeats after the halt, so code
  sees them at the start of its next frame.

XXX: likely the display writes come first so that they land inside the
vertical blank, and the mixer last because it is long and need only finish
within the frame; the bytes don't say so.

### Input, soft reset and sleep

A held key first repeats after `INPUT_KEY_REPEAT_FIRST` (19) frames and then
every `INPUT_KEY_REPEAT_NEXT` (6). `gKeysRepeatDirected` is the repeat with at
most one D-pad direction: of several held directions it keeps the one added
last, and with all four it keeps none.

A, B, Select and Start pressed together raise the keypad interrupt;
`Input_HandleKeyInterrupt` sets `gResetRequested`, and the frame loop jumps to
`Cart_Restart`, the cartridge entry. In debug mode (`gDebugMode`), holding
Start and Select pauses the loop; A, B, Select or the D-pad then advance one
frame and Start resumes.

Holding only L and R for `SLEEP_COMBO_FRAMES` (three seconds), or touching no
key for `SLEEP_IDLE_FRAMES` (three minutes) while `gIdleSleepEnabled` is set,
requests sleep. The loop turns the screen white for `SLEEP_WHITE_FRAMES`,
stops the CPU (`swi 3`, the BIOS's Stop) with the keypad set to wake on L and
R, then restores the display and waits `SLEEP_WAKE_FRAMES`. Nothing sleeps
while `gSleepDisabled` is set.

## The stack switch in `WaitFrames`

The system stack runs from the top of IWRAM down to the end of the IWRAM heap,
1,792 bytes. Interrupts use it too: the dispatcher, `IwramIrqMain`
(`system/runtime.s`), switches to system mode before calling a handler, so the
VBlank handler, the VBlank tasks and the sound mixer run below whatever they
interrupted.

When the caller's stack pointer is below `Ram_FrameWaitStackTop`, 512 bytes
above the heap's end, `WaitFrames` copies everything from the stack pointer up
to that top into `gSavedStack` in EWRAM by DMA, sets the stack pointer to the
top and runs its frames there; on return it copies the stack back and
restores the pointer. The frame loop, the main-loop tasks and the VBlank work
therefore always start with 512 bytes above the heap, however deep the code
that waits.

`WaitFrames` must not be called from inside a task: the nested call would find
its stack below the top again and overwrite `gSavedStack` and
`gSavedStackSize`, which still hold the outer caller's frames. `WaitFrames`
itself doesn't check, but it sets `gSchedulerStatus` while it runs the
main-loop tasks, and code a task may reach tests it before waiting
(`BattleObj_SpawnObjectsForList`). No task waits (below).

## The heap

The heap hands out memory from two pools, one in IWRAM from `gIwramHeap` up to
`Ram_IwramHeapEnd` and one in EWRAM from `gEwramHeap` up to the end of EWRAM.
Each pool is a bump allocator: a block starts at the pool's next free address,
which moves up past it. The two next-free addresses are the first two words of
`gWorkSlot` (`union HeapState`, `games/tbs/include/heap_state.h`), and the
other 62 words are the named slots. `Runtime_AllocateHeapBlock` and `MemAlloc`
take IWRAM first and fall back to EWRAM; `Runtime_AllocateBlock` and
`Runtime_BumpAllocateAlternatePool` take EWRAM first. Sizes are rounded up to
words, a block may not end exactly at its pool's end, and an allocation that
fits in neither pool returns `NULL`. Blocks are not cleared.

Nothing is freed one block at a time. `Runtime_ReleaseHeapBlock(slot)` clears
the slot and sets its pool's next free address back to the block's start;
`MemFree(p)` does the same with `p` (both in `system/heapfree.s`). The pool is
chosen from the address with no comparison: `(address >> 22) & 4` is 0 for an
EWRAM address and 4 for an IWRAM one, the byte offsets of the two next-free
words. Releasing a block therefore releases every later block in the same
pool, and the rule is that blocks are released in the reverse of the order
they were taken, pool by pool. A block released out of order does not fail at
once: the later blocks stay in their slots, pointing at memory the next
allocation hands out again. `Game_Run` empties both pools and clears every
slot before each scene, so no block outlives the scene, battle or minigame
that took it.

That rule is why an effect's transfer callback has to be removed before its
heap blocks are released, and why `RamCode_Load` copies of code are freed
right after use. A slot that already has a block is returned whatever size is
asked for, so `Runtime_AllocateHeapBlock` also means "get".

The frame's scratch block is taken after the main-loop tasks have run and
released after the VBlank that copied it to OAM. For that frame it is the top
of its pool, so anything the VBlank work allocates from the same pool in
between is released with it.

## The task scheduler

The task table holds 20 tasks (`SCHEDULER_TBS_TASK_COUNT`), each a routine, an
order halfword and flags. The order's high byte picks the pass that runs the
task (`SCHEDULER_KEY_MAIN_LOOP` for the frame loop, `SCHEDULER_KEY_VBLANK` for
VBlank) and its low byte ranks it within the pass. Every add re-sorts the
table by order, highest first, comparing orders as signed halfwords
(`Scheduler_SortTasks`), and `Scheduler_RunPass` runs, in table order, every
task whose high byte equals the pass's.

The sort stops one pass short, so the tasks in the first two slots can stay
out of order (`// BUG:` in the code). It swaps only on strictly greater, so
tasks of equal order keep their table order, which depends on the free slot
each was given; code that needs an order gives its tasks different orders.

A task is suspended by setting bit 0 of its order's high byte
(`Scheduler_SuspendTasks`), which no pass matches, and resumed by clearing it,
so it keeps its rank. An empty slot's order is `SCHEDULER_KEY_EMPTY` and a
removed task's `SCHEDULER_KEY_REMOVED`; both high bytes are odd, and as signed
halfwords a removed slot sorts first and an empty one last.
`Scheduler_SuspendOverlayTasks` suspends the tasks whose routine is in EWRAM,
where overlays run, except those whose flag bit 0 is set; the link lobby and
the Colosso scenes set it (`Scheduler_SetTaskFlags`) for tasks that must keep
running under a menu.

Adding, removing, suspending and resuming disable interrupts, because the
VBlank pass reads the same table. A pass walks the table by index without a
lock, so a task that adds another during a pass re-sorts the table under it,
which can shift the tasks after it by one. XXX: whether the game ever does
this isn't known.

## The register write queue

The queue (`gIoWriteQueue`, `io_write_queue.h`) holds up to 32 entries, each a
value, an address and a control word; a write to a full queue is dropped, and
a frame that is not finished keeps its writes for the next. Bits 16 and 17 of
the control word are the width (`IO_WRITE_8`, `IO_WRITE_16`, `IO_WRITE_32`;
none means the entry is a DMA whose three words go to DMA3) and bits 18 and 19
the operation (`IO_WRITE_SET` ORs the value in, `IO_WRITE_CLEAR` clears its
bits, neither stores it). The `IoWriteQueue_Store`, `SetBits` and `ClearBits`
writers each add one entry.

`IoWriteQueue_FlushPending` copies `Resource_ExecuteTransfer`
(`system/resource/decode_01.s`), the ARM routine that carries the entries out,
onto the stack, which is in IWRAM, and runs it there. XXX: why the stack
rather than a heap block isn't known; a block allocated inside the interrupt
would sit above the frame's scratch block and be released with it.

## Saving

The cartridge's flash has 16 sectors of 4 KB, each holding one record: a
16-byte header (signature, record id, a checksum and a sequence number) and a
payload of 4,080 bytes, of which the checksum covers the first 4,072. A save
file is records `n` and `n + 3`, for the three files of the file select
screen.

A save never writes over the copy it replaces. `SaveState_WriteRecord` writes
the new copy, numbered one past the record's highest sequence, into a free
sector picked at random, and only then invalidates the old copy. A power loss
between the two leaves two good copies, and the scan that starts every save,
load, copy or delete (`SaveState_InitializeWorkspace`) keeps the one with the
higher sequence. The last free sector is kept for a record that already has a
copy, so replacing a save always has room. When the sequence passes
`SAVE_SEQUENCE_LIMIT` (65,000), the record is written again with sequence 1
into the old copy's sector, and the high-numbered copy is invalidated only
after that write has been verified.

## The link cable

While the link is up, the VBlank handler trades a 24-byte payload with the
partner (`SerialRuntime_ExchangePayloads`) in multiplayer mode, the parent
console clocking the transfer from timer 3 a halfword at a time. Bulk data
rides on the exchange in 20-byte blocks, one each way per VBlank
(`SerialRuntime_StepBlockTransfer`). The receiver acknowledges each block's
sequence number, and a sender that sees an acknowledgement behind its own
count rewinds and sends the missed blocks again, so a send's source must stay
unchanged until the send is over. A send's size falls by 20 a block and the
send ends only at exactly 0, so the size must be a multiple of 20. Link
battles, the Battle Arena's lobby and the file select screen's transfer use
it.

## Design

The game is written as ordinary functions that run until they are done. A
battle effect, a cutscene or a menu screen is a function with its own loop,
and when time has to pass it calls `WaitFrames` and carries on from the same
place. Nothing returns to a main loop between frames. Work that must continue
while such a function waits is a task, which `WaitFrames` runs however deep
the waiting function is: window animation, the battle's event playback
(`BattleEvent_Playback`) and a scene's callbacks are tasks. Most of the
runtime's shape follows from that choice: the stack switch keeps the stack
bounded however deep the caller, the scheduler gives tasks a known order, and
the heap's slots let a task's blocks outlive the call that took them.

The second choice is that a frame's changes reach the hardware together. Code
writes RAM copies (the depth slots, `gBgScroll`, the write queue) and the
VBlank handler commits them only for a finished frame, so a lag frame shows
the previous picture whole rather than a mixture of two.

Saving trades flash space for safety: two sectors per record are in use only
while a write is in progress, and the random choice of sector spreads writes
across the chip. XXX: likely the random choice is meant to spread wear,
because nothing else in the code needs it; the bytes don't say so.

## Limits

- 62 heap slots; the IWRAM heap ends at `Ram_IwramHeapEnd` and the EWRAM heap
  at the end of EWRAM.
- 20 scheduled tasks at a time.
- 32 queued register writes a frame.
- 96 sprite VRAM cache entries over 512 blocks of 64 bytes, and at most 8 KB
  (128 blocks, a quarter of sprite VRAM) for one entry.
- 256 depth slots and 32 affine matrices a frame; once all 32 are taken,
  `AffineMatrix_BuildForEffect` returns 0 and the sprite uses whatever matrix
  0 holds.
- 512 bytes of stack for the frame loop, its tasks and the VBlank work.
- 16 flash sectors for the three save files' six records.

## Evidence and open questions

The names come from what the code does: `WaitFrames` from its loop,
`Scheduler_` from the table it keeps, the heap's from its two pointers and its
slots, the save code's from the flash records it writes. Constants such as
`SCHEDULER_KEY_MAIN_LOOP` are the values the callers pass.

`gSavedStack` holds 3,152 bytes, though a stack below the frame-wait top and
above the heap's end is at most 512. `ewram.s` lays EWRAM out in 4 KB groups
(`gSceneState` opens the second, `gSleepActive` the third, the music state the
fourth), and `gSavedStack` is the last cell of the third, sized to reach the
fourth. It could hold a stack that had run 2,640 bytes into the heap.

No task waits. A call graph of the tbs-en build's objects, main image and
overlays, from each function the code hands to the scheduler, each object
hook and each `SC_CALL` callback, finds nine tasks with a path of calls to
`WaitFrames`, and none takes it: the scene tasks turn actors with a wait of 0
(`Engine_ActorFaceDirection`), the world map's lighthouse jitter moves the
camera with a glide, the slot machine and the link countdown open instant
windows (`UI_WINDOW_INSTANT`), and battle playback's sprite respawn tests
`gSchedulerStatus`. The tenth path is the debug scene selector, which only
the unused `DebugMenu_ScheduleSceneSelector` would schedule as a task.

- XXX (no evidence): what the five empty exports after the fades
  (`RuntimeDispatch_ReservedNoOpA` to `E`) were for; they are empty in both
  games, nothing calls them, and each is reached only through a bare veneer,
  so no code shows what they were to do.
