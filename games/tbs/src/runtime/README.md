# Runtime

This folder holds The Broken Seal's runtime: the code the rest of the game
stands on. Power-up and the interrupt handlers, the frame wait and the VBlank
handler, input, the register write queue, fades, the sprite lists and the
sprite VRAM cache, the task scheduler, maths, number and debug text, the heap,
the 3D transform, saving and the link cable.

See also: [System](../system/README.md), for the memory map and the resident
IWRAM code this folder installs.

## Files

| File | What it holds |
| --- | --- |
| `runtime.c` | the guide to the runtime object: it includes `runtime_internal.h`, then the parts in `runtime/` in ROM order |
| `runtime_internal.h` | the object's includes, and the one prototype two parts share |
| `math_table.c` | the quarter-wave sine table the trigonometry reads |
| `numtext.c` | the hexadecimal digits and the powers of ten number text uses |
| `trig.s`, `q16_dot_product.s` | handwritten sine and 16.16 dot product routines, each proven in its header |

The guide's comment says where to start, and each part's second line says
what it holds: `head -2 runtime/*.c` lists them. A part includes the private
header, so it resolves when opened on its own, but it is compiled only through
`runtime.c`. The ROM shows the runtime as one object; the parts are ours, cut
where one family ends and the next begins. Link order is in
`games/tbs/objects.tsv`, from `trig`. The resident IWRAM code the runtime
installs and calls is in `games/tbs/src/system`.

The unit headers are in `games/tbs/include/unit/runtime/`. Code in other banks
reaches many of these functions through the far-call table
`games/tbs/src/system/far_call/RUNTIME.S` (the `...Far` names), so a rename
crosses folders.

## Words

- **heap slot**: one of the named long-lived blocks (`enum HeapSlot`,
  `HEAP_SLOT_*`) in `gWorkSlot`. The heap is two bump allocators, one in IWRAM
  up to `Ram_IwramHeapEnd` and one in EWRAM; nothing is freed one block at a
  time (`heap.c`, and the design below).
- **task**: a routine the scheduler runs once a frame from `WaitFrames`, or
  once a VBlank, in the order of its order value; up to 20 at a time
  (`scheduler.c`).
- **queued write**: a register write, or a DMA, that the VBlank handler
  carries out once the frame loop has finished a frame (`IoWriteQueue_Store16`,
  `IoWriteQueue_SetBits16` and their kin, `io_queue.c`).
- **cache entry**: one of the 96 entries of the sprite VRAM cache, which owns a
  run of the 512 blocks of 64 bytes of sprite tile memory (`vram_cache.c`).
  An entry whose offset is `0xffff` is free.
- **depth slot**: one of 256 lists sprites are linked into each frame, which
  the OAM build walks in order (`render.c`).
- **transform**: `gTransform`, twelve `s32` in 16.16 fixed point: a 3x3
  rotation and a translation, with a one-level stack (`transform.c`).
- **record**: one save record in one 4 KB flash sector, with a 16-byte header
  (signature, record id, checksum, sequence number); a save file is records
  `n` and `n + 3` ([`runtime/save.c`](runtime/save.c)).
- **payload** and **block**: each VBlank two linked consoles trade a 24-byte
  payload; bulk data rides on it in 20-byte blocks (`serial.c`).

## Prefixes

| Prefix | What it acts on |
| --- | --- |
| `System_`, `Runtime_`, `Code_` | power-up, interrupts, the heap and code loading |
| `Input_` | key repeat and the keypad interrupt |
| `IoWriteQueue_` | the register write queue |
| `Fade_` | screen fades through the blend registers |
| `Render_`, `AffineMatrix_` | the frame's sprite lists and affine matrices |
| `VramCache_` | the sprite VRAM cache |
| `Scheduler_` | the task scheduler |
| `Transform_`, `SceneTransform_`, `Projection_` | the 3D transform and projection |
| `NumberText_`, `DebugText_` | number text and the debug text screen |
| `ColorBuffer_` | shading a buffer of pixels, keeping a copy |
| `SaveState_` | saving |
| `SerialRuntime_`, `SerialTest_`, `BattleLink_` | the link cable |
| `RuntimeDispatch_` | empty far-call exports nothing calls |
| `Resource_`, `Palette_`, `Ui_`, `Sound_` | things the engine names elsewhere |

The spellings some sites keep to match, and the comment markers, are in
[Matching](../MATCHING.md).

## How the game boots and runs one frame

The memory these steps use is mapped in `../system/README.md`.

1. **Entry.** The cartridge header jumps to `System_Entry`
   (`games/tbs/src/system/entry.s`), which sets the IRQ-mode and system-mode
   stacks (0x03007fa0 and 0x03007f00) and calls `System_Initialize`.
2. **Power-up** (`System_Initialize`, `runtime/system.c`): stop any HBlank
   DMA, set the cartridge wait states, clear IWRAM up to 0x03007800, empty
   the heap (`Runtime_InitializeHeap`), copy the resident runtime into IWRAM
   and point the interrupt vector at its dispatcher
   (`Runtime_InstallIwramAndIrqs`), load the window graphics, install
   `System_VBlankHandler` for VBlank, arm the soft-reset keys, start the
   sound driver, reset the sprite VRAM cache and the task table, wait ten
   frames, and start the game (`Game_ResetForNewGameFar`). It never returns:
   `Game_ResetForNewGame` (`games/tbs/src/object/object/scene.c`) loops from
   scene to scene, starting at the title.
3. **A frame** (`WaitFrames`, `runtime/frame.c`). Code that wants time to pass
   calls `WaitFrames(n)`, from however deep in its own calls. If the caller's
   stack is below `Ram_FrameWaitStackTop` (0x03007a00), it is copied aside and
   the loop runs on that top. Each frame it runs the main-loop tasks
   (`Scheduler_RunPass(SCHEDULER_KEY_MAIN_LOOP)`), builds the OAM list into
   the frame scratch heap block (`Render_BuildOamList`), measures CPU load for
   the debug display, counts idle and L+R frames for sleep, then halts until
   VBlank (`swi 2`), releases the scratch block and updates key repeat.
4. **VBlank** (`System_VBlankHandler`, `runtime/frame.c`), reached through
   the IWRAM dispatcher and `gIrqHandlers`: trade the link cable payloads,
   step the music's fades and the screen fade, and if the frame loop finished
   a frame, copy its OAM list and BG scroll registers and flush the queued
   register writes. Then run the one-shot VBlank hook and the VBlank tasks
   (`SCHEDULER_KEY_VBLANK`), read the keypad, count the frame and mix the
   next sound buffer.

## Design

### Code that waits, and tasks that keep running

The game is written as ordinary functions that run until they are done. A
battle effect, a cutscene or a menu screen is a function with its own loop,
and when time must pass it calls `WaitFrames(n)` and continues where it was
when the frames are over. It does not return to a main loop between frames;
`System_Initialize` never returns, and `Game_ResetForNewGame` runs one scene
after another.

So anything that must keep happening while such a function waits is a task:
a routine scheduled with `Scheduler_AddOrUpdateCallback`, which `WaitFrames`
runs every frame however deep the waiting function is. Window animation,
the battle's event playback (`BattleEvent_Playback`) and a scene's
callbacks all work this way. The rest of this design follows from that
choice: the stack must be bounded however deep the caller, tasks must run
in a known order, and blocks that tasks use must outlive them.

### Why `WaitFrames` moves the stack

The system stack starts at 0x03007f00 and grows down towards the IWRAM heap,
which ends at 0x03007800: 1,792 bytes in all. Interrupts use that same
stack. The dispatcher, `IwramIrqMain` (`system/runtime.s`), switches the CPU
to system mode before it calls a handler, so the VBlank handler, the VBlank
tasks and the sound mixer all run on the system stack, below whatever they
interrupted.

A caller that is deep in its own calls could leave the tasks too little room
before the heap. `WaitFrames` prevents that: when the caller's stack pointer
is below `Ram_FrameWaitStackTop` (0x03007a00), it copies everything from the
stack pointer up to that top into `gSavedStack` in EWRAM by DMA, sets the
stack pointer to the top, and runs its frames from there; on return it copies
the frames back and restores the stack pointer. The frame loop, the
main-loop tasks and the VBlank work that interrupts the frame loop's halt
therefore always start from the same place, with 512 bytes above the heap's
end, however deep the code that waits.

It follows that `WaitFrames` cannot be called from inside a task. The nested
call would find its stack below the top again and copy over `gSavedStack`
and `gSavedStackSize`, which still hold the outer caller's frames. XXX:
nothing guards against this; whether any task does it is not known.

### The heap: two stacks of blocks

The heap is two bump allocators, one in IWRAM from `gIwramHeap` up to
0x03007800 and one in EWRAM from `gEwramHeap` up to 0x02040000. Their next
free addresses are the first two words of `gWorkSlot` (`union HeapState`), and
the other 62 words are the named slots. `Runtime_AllocateHeapBlock` and
`MemAlloc` take IWRAM first, the faster memory, and fall back to EWRAM;
`Runtime_AllocateBlock` and `Runtime_BumpAllocateAlternatePool` take EWRAM
first. Every size is rounded up to words.

Nothing is freed one block at a time. `Runtime_ReleaseHeapBlock(slot)` clears
the slot and sets its pool's next free address back to the block's start, and
`MemFree(p)` does the same with `p` (both in `system/heapfree.s`). The pool is
chosen from the address itself, with no compare: `(address >> 22) & 4` is 0
for an EWRAM address (0x02...) and 4 for an IWRAM one (0x03...), which are
the byte offsets of the two next-free words. So releasing a block also
releases every block allocated after it in the same pool. The heap is two
stacks, and the lifetime rule is that blocks are released in the reverse
order they were taken, pool by pool. A block released out of order does not
fail at once: the later blocks stay in their slots, pointing at memory that
the next allocation hands out again.

That rule is why an effect's transfer callback must be removed before its
heap blocks are released, and why `RamCode_Load` copies of code are freed
right after use. A slot also makes allocation idempotent:
`Runtime_AllocateHeapBlock` returns the slot's block when it already has one,
whatever size is asked for, so "allocate" also means "get".

The frame's scratch block follows the same rule. `WaitFrames` allocates
`HEAP_SLOT_FRAME_SCRATCH` (0x400 bytes) after the main-loop tasks have run,
builds the OAM list in it, and releases it after the VBlank that copied it to
OAM. For that frame it is the top of its pool, so anything the VBlank work
allocates from the same pool in between is released with it.

### The scheduler's ordering guarantees

A task's order is a halfword. Its high byte picks the pass that runs it
(`SCHEDULER_KEY_MAIN_LOOP` 0xc80 for the frame loop, `SCHEDULER_KEY_VBLANK`
0x480 for VBlank) and its low byte ranks it within the pass. The table is
bubble-sorted, highest order first, every time a task is added
(`Scheduler_SortTasks`), and `Scheduler_RunPass` runs, in table order, every
task whose high byte equals the pass's. So:

- within a pass, a task with a higher order runs before a lower one: 0xc94
  before 0xc80 before 0xc76;
- tasks of equal order run in table order, which the sort keeps (it swaps
  only on strictly greater) but which depends on the free slot each task was
  given, so code that needs an order gives its tasks different orders;
- a task is suspended by making the high byte odd (`Scheduler_SuspendTasks`
  sets bit 8), which no pass matches, and resumed by clearing it, so it keeps
  its rank; a free slot is 0xffff, or 0x7fff once removed, also odd;
- `Scheduler_SuspendOverlayTasks` tells an overlay's tasks from the main
  image's by the routine's address (0x02... is EWRAM, where overlays run),
  unless the task's flag bit 0 says to keep it running.

Adding, removing, suspending and resuming disable interrupts, because the
VBlank pass reads the same table from inside the interrupt. A pass itself
walks the table by index without a lock, so a task that adds another during
a pass re-sorts the table under the pass, which can shift the tasks after it
by one. XXX: whether the game relies on this, or ever triggers it, is not
known.

### The frame order and what depends on it

Each frame has two halves. In the frame loop: the main-loop tasks run, then
`Render_BuildOamList` builds the sprite list from what they left in the depth
slots, then `gFrameRenderPending` is set, then the loop halts. In VBlank: the
link payloads are traded, the music and screen fades step, and only if
`gFrameRenderPending` is set are the OAM list, the four BG scroll registers
(from `gBgScroll`) and the queued register writes copied to the hardware.
Then the one-shot VBlank hook runs, then the VBlank tasks, then the keypad is
read, and the mixer runs last.

What depends on this order:

- Tasks never write sprite or scroll registers directly; they write RAM, and
  the VBlank commits a whole frame at once. A frame that runs long (a lag
  frame) is not committed at all, so the screen keeps the previous frame's
  sprites, scroll and register writes together rather than a mixture.
- Sprites must be placed by main-loop tasks or by the waiting code before it
  calls `WaitFrames`: the OAM list is built right after the main pass, so a
  VBlank task's changes show a frame later.
- The keys are read in VBlank and turned into repeats after the halt
  (`Input_UpdateKeyRepeatAndDirection`), so code sees them at the start of
  its next frame.
- XXX: that the display writes come first so that they land inside the
  vertical blank, and the mixer last because it is long and only needs to
  finish within the frame, is the natural reading of the order; the bytes do
  not say so.

The register write queue (`io_queue.c`) holds up to 32 entries of a value,
an address and a control word, and is emptied by each flush.
`Resource_ExecuteTransfer` (`system/resource/decode_01.s`), the routine that
carries the queue out, reads bits 16 and 17 of the control word as the width
(1 a byte, 2 a halfword, 3 a word; 0 means the entry is a DMA, whose three
words go to DMA3) and bits 18 and 19 as the operation (4 sets the value's
bits, 8 clears them, neither stores it); `io_write_queue.h` names them
`IO_WRITE_8` to `IO_WRITE_32`, `IO_WRITE_SET` and `IO_WRITE_CLEAR`. So
`IoWriteQueue_SetBits16(&REG_WINOUT, 16)` sets a bit of a halfword register
and `IoWriteQueue_ClearBits16(&REG_WINOUT, 4)` clears one. The flush copies
that routine onto the stack and runs it there: it is ARM code, and the stack
is in IWRAM, where ARM code runs fastest. XXX: why the stack rather than a
heap block is not known; a heap block allocated inside the interrupt would
sit above the frame's scratch block, which the rule above would then release
with it.

## Limits

- 62 heap slots (`games/tbs/include/heap_state.h`); the IWRAM heap ends at
  0x03007800 and the EWRAM heap at 0x02040000.
- 20 scheduled tasks at a time (`runtime/scheduler.c`).
- 32 queued register writes a frame; a write to a full queue is dropped.
- 96 sprite VRAM cache entries over 512 blocks of 64 bytes.
- 256 sprite depth slots.
- 512 bytes of stack for the frame loop, its tasks and the VBlank work.

## Evidence and open questions

The names come from what the code does: `WaitFrames` from its loop,
`Scheduler_` from the table it keeps, the heap's from its two pointers and its
slots, the save code's from the flash records it writes. Constants such as
`SCHEDULER_KEY_MAIN_LOOP` are the values the callers pass.

Still unknown:

- XXX: whether any task calls `WaitFrames`, which would overwrite the saved
  stack.
- XXX: `gSavedStack` holds 0xc50 bytes, though a caller's stack below the
  frame-wait top and above the heap's end is at most 0x200; why it is that
  large is not known, unless the stack was expected to run into the heap.
- XXX: what `RuntimeDispatch_` and the reserved empty exports were for.
