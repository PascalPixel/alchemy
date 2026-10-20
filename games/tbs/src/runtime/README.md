# Runtime

Underneath everything Golden Sun puts on the screen there's a layer of code
you never see directly. It starts the game when the Game Boy Advance is
switched on, keeps time one frame at a time, reads the buttons, hands out
memory, gathers each frame's sprites, fades the screen in and out, saves your
game to the cartridge and talks to a second console over the link cable. The
field, the battles, the menus and every scene are built on it, and all of
them come back to it whenever they let time pass, through one function,
`WaitFrames`.

The memory it works in, and the resident code it copies into the console's
fast internal RAM at power-on, are described in [System](../system/README.md).

## Files

| File | What it holds |
| --- | --- |
| `runtime.c` | the runtime object itself: it includes `runtime_internal.h`, then the parts in `runtime/` in the order their code has in the ROM, and its opening comment says where each subject starts |
| `runtime_internal.h` | what the parts share: the object's includes, and the one prototype two parts use |
| `runtime/*.c` | the parts, one family of functions each |
| `math_table.c` | a quarter-wave sine table with the same values as `Trig_SineTable` in `trig.s`, which is the copy `Trig_Sin` reads; nothing reads this one by name |
| `numtext.c` | the hexadecimal digits and the powers of ten that number text uses |
| `trig.s`, `q16_dot_product.s` | the sine and cosine, and a 16.16 dot product, in assembly; each file's header gives its evidence: `trig.s` branches on a shift's carry, which the game's compiler never does, and `q16_dot_product.s` has a multiply sequence no compiler wrote, though the header says that alone doesn't prove the whole file handwritten |

The parts are [`decompress.c`](runtime/decompress.c), [`fade.c`](runtime/fade.c),
[`frame.c`](runtime/frame.c), [`heap.c`](runtime/heap.c),
[`io_queue.c`](runtime/io_queue.c), [`math.c`](runtime/math.c),
[`render.c`](runtime/render.c), [`save.c`](runtime/save.c),
[`scheduler.c`](runtime/scheduler.c), [`serial.c`](runtime/serial.c),
[`system.c`](runtime/system.c), [`text.c`](runtime/text.c),
[`transform.c`](runtime/transform.c) and
[`vram_cache.c`](runtime/vram_cache.c). The second line of each says what it
holds, so `head -2 runtime/*.c` gives you the whole list with a sentence
apiece.

The ROM shows the runtime as a single object, the code of one compiled file,
so that's how it's built: everything is compiled once, through `runtime.c`.
The split into parts is ours, made where one family of functions ends and the
next begins. Each part includes the private header as well, so its names
resolve when you open it on its own in an editor or another tool, even though
it's never compiled by itself. The object's place in the link order is in
`games/tbs/objects.tsv`, from the `trig` row on, and the resident IWRAM code
the runtime installs and calls is in `games/tbs/src/system`.

The unit headers, which hold the prototypes the rest of the game uses, are in
`games/tbs/include/unit/runtime/`. Code in other banks of the ROM reaches
many of these functions through the far-call table
`games/tbs/src/system/far_call/RUNTIME.S`, under names ending in `Far`, so
renaming one of them touches other folders too.

## Words used here

- **frame** and **VBlank**: the screen is drawn about sixty times a second,
  and each picture is a frame. After the last line of one picture there's a
  short pause before the next begins, the vertical blank or VBlank. The
  console signals it with an interrupt, and it's the moment the game uses to
  change what's on screen.
- **interrupt**: the hardware stopping whatever code is running to call a
  handler (for VBlank, the keypad, the link cable or a timer), then carrying
  on where it left off.
- **IWRAM** and **EWRAM**: the console's two work memories, 32 KB of fast
  internal RAM at addresses from 0x03000000 and 256 KB of slower external RAM
  from 0x02000000. Numbers written with 0x in front are hexadecimal, the way
  the code and the memory maps write addresses.
- **byte**, **halfword** and **word**: 8, 16 and 32 bits. A halfword holds 0
  to 65,535 (0xffff); its high byte is its upper eight bits.
- **16.16 fixed point**: a word read as a whole part in its upper 16 bits and
  a fraction in its lower 16, so 0x10000 (65,536) means 1.0. Positions,
  scales and the transform are kept this way.
- **angle**: a whole turn is 65,536 steps, so 0x4000 is a quarter turn and
  0x8000 a half. `Trig_Sin` and `Trig_Cos` return 16.16 values from a table
  of 256 entries for the first quarter turn, each 1/1024 of a turn after the
  last; `ArcTan2` measures from the y axis, 0 along +y and 0x4000 along +x.
- **heap slot**: one of the named long-lived blocks (`enum HeapSlot`,
  `HEAP_SLOT_*`) in `gWorkSlot`. The heap is two bump allocators, one in IWRAM
  up to `Ram_IwramHeapEnd` and one in EWRAM, and nothing in it is freed one
  block at a time (`heap.c`; the heap section below explains the rest).
- **task**: a routine the scheduler runs once every frame from `WaitFrames`,
  or once every VBlank, in the order of its order value; up to 20 at a time
  (`scheduler.c`).
- **register** and **DMA**: the hardware's settings, such as the scroll
  positions and the blend, live at fixed addresses called registers. DMA is
  the console's copy hardware: the game writes a source, a destination and a
  count to a DMA channel's registers, and the hardware does the copy.
- **queued write**: a register write, or a DMA, that the VBlank handler
  carries out once the frame loop has finished a frame (`IoWriteQueue_Store16`,
  `IoWriteQueue_SetBits16` and their kin, `io_queue.c`).
- **sprite** and **OAM**: sprites are the small pictures the hardware draws
  over the backgrounds; OAM is the hardware's table of them, which the game
  rebuilds every frame.
- **cache entry**: one of the 96 entries of the sprite VRAM cache. Sprite tile
  memory, the 32 KB of video memory from 0x06010000, is split into 512 blocks
  of 64 bytes, and each entry owns a run of them. An entry whose offset is
  `0xffff` is free (`vram_cache.c`).
- **depth slot**: one of 256 lists sprites are linked into each frame, which
  the OAM build walks in order (`render.c`).
- **transform**: `gTransform`, twelve `s32` in 16.16 fixed point: a 3x3
  rotation and a translation, with a one-level stack (`transform.c`). The
  battle and field cameras work through it.
- **record**: one save record in one 4 KB flash sector, with a 16-byte header
  (signature, record id, checksum, sequence number); a save file is records
  `n` and `n + 3` ([`runtime/save.c`](runtime/save.c)).
- **payload** and **block**: each VBlank, two linked consoles trade a 24-byte
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
| `Transform_`, `SceneTransform_`, `Projection_` | the 3D transform and projection |
| `NumberText_`, `DebugText_` | number text and the debug text screen |
| `ColorBuffer_` | shading a buffer of pixels, keeping a copy |
| `SaveState_` | saving |
| `SerialRuntime_`, `SerialTest_`, `BattleLink_` | the link cable |
| `RuntimeDispatch_` | empty far-call exports nothing calls |
| `Resource_`, `Palette_`, `Ui_`, `Sound_` | resources, palettes, the interface and sound, whose main code lives in other folders |

[Matching](../MATCHING.md) explains the odd spellings and the comment markers.

## How the game boots and runs one frame

When the console starts, the cartridge header jumps to `System_Entry`
(`games/tbs/src/system/entry.s`). It sets up the interrupt stack at
0x03007fa0 and the system stack at 0x03007f00, then calls
`System_Initialize`.

`System_Initialize` (`runtime/system.c`) powers the game up. It stops any
HBlank DMA left running (the copy some screen effects repeat after every line
of the picture), sets how fast the cartridge is read (its wait states) and
clears IWRAM up to 0x03007800, leaving the stacks above that alone. It
empties the heap (`Runtime_InitializeHeap`), copies the resident runtime into
IWRAM and points the interrupt vector at its dispatcher
(`Runtime_InstallIwramAndIrqs`), and loads the window graphics. Then it
installs `System_VBlankHandler` for
VBlank, arms the soft-reset keys, starts the sound driver, resets the sprite
VRAM cache and the task table, waits ten frames and starts the game with
`Game_ResetForNewGameFar`. It never returns: `Game_ResetForNewGame`
(`games/tbs/src/object/object/scene.c`) goes from scene to scene for as long
as the game runs, starting at the title. The German, French, Spanish and
Italian editions also take the cartridge-removed interrupt, whose handler
halts the game.

The soft-reset keys are A, B, Select and Start together. When they're
pressed, the keypad interrupt (`Input_HandleKeyInterrupt`) sets
`gResetRequested`, and the frame loop restarts the game from the cartridge's
entry point.

From then on, time passes in calls to `WaitFrames(n)` (`runtime/frame.c`),
made from however deep in its own calls the waiting code is. If the caller's
stack has grown below `Ram_FrameWaitStackTop` (0x03007a00), it's copied aside
and the loop runs from that top instead (why is explained below). Each
frame the loop runs the main-loop tasks
(`Scheduler_RunPass(SCHEDULER_KEY_MAIN_LOOP)`), builds the OAM list into the
frame's scratch heap block (`Render_BuildOamList`), measures CPU load for the
debug display and counts the frames that lead to sleep. Then it halts the CPU
until VBlank (`swi 2`, the BIOS's Halt), releases the scratch block and
updates key repeat.

The game's sleep mode lives in that loop. Holding only L and R for three
seconds, or with idle sleep switched on (`gIdleSleepEnabled`) touching no key
for three minutes, turns the screen white for a second and then stops the
console (`swi 3`, the BIOS's Stop) until L and R are pressed again. On
waking, the screen is put back and the game waits ten frames before carrying
on. Nothing sleeps while `gSleepDisabled` is set.

A key held down first repeats after 19 frames and then every 6
(`INPUT_KEY_REPEAT_FIRST` and `INPUT_KEY_REPEAT_NEXT`), which is what lets a
held direction run down a list. `gKeysRepeatDirected` is the same keys with
at most one direction of the D-pad: of several held directions it keeps the
one added last, and with all four held it keeps none. In debug mode
(`gDebugMode`), holding Start and Select pauses the game; A, B, Select or the
D-pad then step it a frame at a time, and Start lets it run again.

VBlank interrupts the halt. `System_VBlankHandler` (`runtime/frame.c`), reached
through the IWRAM dispatcher and `gIrqHandlers`, trades the link cable
payloads and steps the music's fades and the screen fade. If the frame loop
finished a frame, it copies that frame's OAM list and BG scroll registers to
the hardware and carries out the queued register writes. Then it runs the
one-shot VBlank hook and the VBlank tasks (`SCHEDULER_KEY_VBLANK`), reads the
keypad, counts the frame and mixes the next sound buffer.

## Why it works this way

### Code that waits, and tasks that keep running

The game is written as ordinary functions that run until they're done. A
battle effect, a cutscene or a menu screen is a function with its own loop,
and when time has to pass it calls `WaitFrames(n)` and carries on from the
same place when the frames are over. It doesn't return to a main loop
between frames: `System_Initialize` never returns, and `Game_ResetForNewGame`
runs one scene after another.

So anything that has to keep happening while such a function waits is a
task, a routine scheduled with `Scheduler_AddOrUpdateCallback`, which
`WaitFrames` runs every frame however deep the waiting function is. Window
animation, the battle's event playback (`BattleEvent_Playback`) and a scene's
callbacks all work this way. Most of what follows comes from that choice: the
stack has to stay bounded however deep the caller, tasks have to run in a
known order, and the blocks tasks use have to outlive them.

### Why `WaitFrames` moves the stack

Every function call keeps its return address and its local values on the
stack, which grows downward in memory as calls nest. The system stack starts
at 0x03007f00 and grows down towards the IWRAM heap, which ends at
0x03007800: 1,792 bytes in all. Interrupts use that same stack. The
dispatcher, `IwramIrqMain` (`system/runtime.s`), switches the CPU to system
mode before it calls a handler, so the VBlank handler, the VBlank tasks and
the sound mixer all run on the system stack, below whatever they interrupted.

A caller deep in its own calls could leave the tasks too little room before
the heap. `WaitFrames` prevents that. When the caller's stack pointer is
below `Ram_FrameWaitStackTop` (0x03007a00), it copies everything from the
stack pointer up to that top into `gSavedStack` in EWRAM by DMA, sets the
stack pointer to the top and runs its frames from there; on return it copies
the frames back and restores the stack pointer. The frame loop, the main-loop
tasks and the VBlank work that interrupts the frame loop's halt therefore
always start from the same place, with 512 bytes above the heap's end,
however deep the code that waits.

It follows that `WaitFrames` can't be called from inside a task. The nested
call would find its stack below the top again and copy over `gSavedStack` and
`gSavedStackSize`, which still hold the outer caller's frames. XXX: nothing
guards against this, and whether any task does it isn't known.

### The heap: two stacks of blocks

Scenes, battles and menus need memory for their work while they run: a
battle's state, a menu's windows, a decoded picture. The heap hands it out
from two pools, one in IWRAM from `gIwramHeap` up to 0x03007800 and one in
EWRAM from `gEwramHeap` up to 0x02040000. Each pool is a bump allocator: a
new block starts at the pool's next free address, and that address moves up
past it. The two next-free addresses are the first two words of `gWorkSlot`
(`union HeapState`), and the other 62 words are the named slots.
`Runtime_AllocateHeapBlock` and `MemAlloc` take IWRAM first, the faster
memory, and fall back to EWRAM; `Runtime_AllocateBlock` and
`Runtime_BumpAllocateAlternatePool` take EWRAM first. Every size is rounded
up to whole words, and an allocation that fits in neither pool returns
`NULL`.

Nothing is freed one block at a time. `Runtime_ReleaseHeapBlock(slot)` clears
the slot and sets its pool's next free address back to the block's start, and
`MemFree(p)` does the same with `p` (both in `system/heapfree.s`). The pool
is chosen from the address itself, with no comparison: `(address >> 22) & 4`
is 0 for an EWRAM address (0x02...) and 4 for an IWRAM one (0x03...), and
those are the byte offsets of the two next-free words. So releasing a block
also releases every block allocated after it in the same pool. The heap is
two stacks, and the lifetime rule is that blocks are released in the reverse
of the order they were taken, pool by pool. A block released out of order
doesn't fail straight away: the later blocks stay in their slots, pointing at
memory the next allocation hands out again. Every scene change empties both
pools and clears every slot (`Runtime_InitializeHeap`, from the scene loop),
so no block outlives the scene, battle or minigame that took it.

That rule is why an effect's transfer callback has to be removed before its
heap blocks are released, and why `RamCode_Load` copies of code are freed
right after use. A slot also makes allocation idempotent:
`Runtime_AllocateHeapBlock` returns the slot's block when it already has one,
whatever size is asked for, so "allocate" also means "get".

The frame's scratch block follows the same rule. `WaitFrames` allocates
`HEAP_SLOT_FRAME_SCRATCH` (0x400 bytes) after the main-loop tasks have run,
builds the OAM list in it and releases it after the VBlank that copied it to
OAM. For that frame it's the top of its pool, so anything the VBlank work
allocates from the same pool in between is released with it.

### The order tasks run in

A task's order is a halfword. Its high byte picks the pass that runs it
(`SCHEDULER_KEY_MAIN_LOOP`, 0xc80, for the frame loop and
`SCHEDULER_KEY_VBLANK`, 0x480, for VBlank) and its low byte ranks it within
the pass. The table is bubble-sorted, highest order first, every time a task
is added (`Scheduler_SortTasks`), and `Scheduler_RunPass` runs, in table
order, every task whose high byte equals the pass's. So within a pass a task
with a higher order runs before a lower one: 0xc94 before 0xc80 before 0xc76.

The sort has a bug, marked `// BUG:` in the code: it stops one pass short, so
the tasks in the first two slots of the table can stay out of order. Tasks of
equal order run in table order, which the sort keeps, since it swaps only on
strictly greater, but which depends on the free slot each task was given. Code
that needs an order gives its tasks different orders.

A task is suspended by making the high byte odd (`Scheduler_SuspendTasks`
sets bit 8 of the order), which no pass matches, and resumed by clearing that
bit again, so it keeps its rank. A free slot's order is 0xffff, or 0x7fff once
its task has been removed, and both are odd too. `Scheduler_SuspendOverlayTasks`
tells an overlay's tasks from the main image's by the routine's address
(0x02... is EWRAM, where overlays run), and spares a task whose flag bit 0
says to keep it running; the link lobby and the Colosso scenes set that flag
(`Scheduler_SetTaskFlags`) for tasks that must keep going under a menu.

Adding, removing, suspending and resuming all disable interrupts, because the
VBlank pass reads the same table from inside the interrupt. A pass itself
walks the table by index without a lock, so a task that adds another during a
pass re-sorts the table under the pass, which can shift the tasks after it by
one. XXX: whether the game relies on this, or ever triggers it, isn't known.

### The frame order and what depends on it

Each frame has two halves. In the frame loop, the main-loop tasks run, then
`Render_BuildOamList` builds the sprite list from what they left in the depth
slots, then `gFrameRenderPending` is set, then the loop halts. In VBlank, the
link payloads are traded and the music and screen fades step, and only if
`gFrameRenderPending` is set are the OAM list, the four BG scroll registers
(from `gBgScroll`) and the queued register writes copied to the hardware.
Then the one-shot VBlank hook runs, then the VBlank tasks, then the keypad is
read, and the mixer runs last.

A few things depend on this order. Tasks never write sprite or scroll
registers directly: they write RAM, and the VBlank commits a whole frame at
once. A frame whose work doesn't finish before VBlank (a lag frame) isn't
committed at all, so the screen keeps the previous frame's sprites, scroll
and register writes together rather than a mixture of two frames.

Sprites have to be placed by main-loop tasks, or by the waiting code before
it calls `WaitFrames`, because the OAM list is built right after the main
pass; a VBlank task's changes show a frame later. The keys are read in VBlank
and turned into repeats after the halt (`Input_UpdateKeyRepeatAndDirection`),
so code sees them at the start of its next frame.

XXX: that the display writes come first so that they land inside the
vertical blank, and the mixer last because it's long and only needs to finish
within the frame, is the natural reading of the order; the bytes don't say
so.

### Register writes wait for VBlank

A fade, a window opening or a change of scroll often means writing to the
display registers, and the game wants all of a frame's changes to reach the
screen together, in the pause between two pictures. So code that runs during
the frame can queue such a write instead of making it, and the VBlank handler
carries out the whole queue at once, after the frame loop has finished that
frame.

The register write queue (`io_queue.c`) holds up to 32 entries, each a value,
an address and a control word, and each flush empties it.
`Resource_ExecuteTransfer` (`system/resource/decode_01.s`), the routine that
carries the queue out, reads bits 16 and 17 of the control word as the width
(1 a byte, 2 a halfword, 3 a word; 0 means the entry is a DMA, whose three
words go to DMA3) and bits 18 and 19 as the operation (4 sets the value's
bits in the register, 8 clears them, neither stores the value).
`io_write_queue.h` names them `IO_WRITE_8` to `IO_WRITE_32`, `IO_WRITE_SET`
and `IO_WRITE_CLEAR`. So `IoWriteQueue_SetBits16(&REG_WINOUT, 16)` sets one
bit of a halfword register and `IoWriteQueue_ClearBits16(&REG_WINOUT, 4)`
clears one.

The flush copies that routine onto the stack and runs it there: it's ARM
code, and the stack is in IWRAM, where ARM code runs fastest. XXX: why the
stack rather than a heap block isn't known; a heap block allocated inside the
interrupt would sit above the frame's scratch block, which the heap's rule
would then release with it.

### Saving next to the old copy

The cartridge keeps your saves in flash memory, 16 sectors of 4 KB. Each
sector holds one record: a 16-byte header (signature, record id, a checksum
of the payload and a sequence number) and a payload of 0xff0 bytes, of which
the checksum covers the first 0xfe8. The three save files on the file select
screen are six records, file `n` being records `n` and `n + 3`.

A save never writes over the copy it replaces. `SaveState_WriteRecord` writes
the new copy, numbered one past the record's highest sequence number, into a
free sector picked at random, and only then marks the old copy invalid. If
the power goes in between, two good copies are left, and the scan of every
sector that starts each save, load, copy or delete
(`SaveState_InitializeWorkspace`) keeps the one with the higher sequence.
The last free sector is kept for a record that already has a copy, so
replacing a save always has room. When the sequence
passes 65,000 it starts again at 1: the record is written once more, with
sequence 1, into the old copy's sector, and the high-numbered copy is
invalidated only after that write has been verified.

### The link cable rides on VBlank

Two linked consoles talk once a frame. While the link is up, the VBlank
handler trades a 24-byte payload with the partner
(`SerialRuntime_ExchangePayloads`) in the serial port's multiplayer mode,
with the parent console clocking the transfer from timer 3 a halfword at a
time. Anything bigger rides on that exchange in 20-byte blocks, one block
each way per VBlank (`SerialRuntime_StepBlockTransfer`). The receiver
acknowledges each block's sequence number, and a sender that sees an
acknowledgement behind its own count rewinds and sends the missed blocks
again, so a send's source has to stay unchanged until the send is over. A
send's size falls by 20 a block and the send ends only when it reaches
exactly 0, so the size has to be a multiple of 20. The link battles, the
Battle Arena's lobby and the file select screen's data transfer use it.

## Limits

- 62 heap slots (`games/tbs/include/heap_state.h`); the IWRAM heap ends at
  0x03007800 and the EWRAM heap at 0x02040000.
- 20 scheduled tasks at a time (`runtime/scheduler.c`).
- 32 queued register writes a frame; a write to a full queue is dropped.
- 96 sprite VRAM cache entries over 512 blocks of 64 bytes, and at most 8 KB
  (128 blocks, a quarter of sprite VRAM) for one entry.
- 256 sprite depth slots, and 32 affine matrices a frame; once all 32 are
  taken, `AffineMatrix_BuildForEffect` returns 0 and the sprite is drawn with
  whatever matrix 0 holds.
- 512 bytes of stack for the frame loop, its tasks and the VBlank work.
- 16 flash sectors, holding the three save files' six records.

## Evidence and open questions

`gSavedStack` holds 0xc50 bytes (3,152), though a caller's stack below the
frame-wait top and above the heap's end is at most 0x200 (512). Its size is
what's left of the EWRAM block it ends. `ewram.s` lays EWRAM out in 4 KB
blocks, each holding a new group of variables (`gSceneState` at 0x02001000, `gSleepActive`
and the link cells at 0x02002000, the music state at 0x02003000), and
`gSavedStack` is the last cell of the 0x02002000 block, running up to
0x02003000. It could hold a stack that had run 0xa50 bytes into the heap.

The names come from what the code does: `WaitFrames` from its loop,
`Scheduler_` from the table it keeps, the heap's names from its two pointers
and its slots, the save code's from the flash records it writes. Constants
such as `SCHEDULER_KEY_MAIN_LOOP` are the values the callers pass.

Still unknown:

- XXX: whether any task calls `WaitFrames`, which would overwrite the saved
  stack.
- XXX: what `RuntimeDispatch_` and the reserved empty exports were for.
