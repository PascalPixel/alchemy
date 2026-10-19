# Runtime

This folder holds The Broken Seal's runtime: the code the rest of the game
stands on. Power-up and the interrupt handlers, the frame wait and the VBlank
handler, input, the register write queue, fades, the sprite lists and the
sprite VRAM cache, the task scheduler, maths, number and debug text, the heap,
the 3D transform, saving and the link cable.

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
  time (`heap.c`).
- **task**: a routine the scheduler runs once a frame from `WaitFrames`, or
  once a VBlank, in the order of its order value; up to 20 at a time
  (`scheduler.c`).
- **queued write**: a register write that the VBlank handler carries out a
  fixed number of frames later (`QueueIoWriteDelay1` and its kin,
  `io_queue.c`).
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
| `IoWriteQueue_`, `QueueIoWriteDelay` | the register write queue |
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

## Spellings

Each site keeps the form it matches with. These are one idea and can compile
differently:

- A register write as `Io_Write16(value, &REG_X)`, which builds the value in a
  word first and keeps the store where it is written, or as `REG_X = value`.
- Code copied to RAM is called through a cast function pointer, or through
  `_call_via_r3`, the compiler library's call through r3, with the code's
  address as its fourth argument.

A DMA control word is written with its flags above its count:
`(DMA_ENABLE | DMA_32BIT) << 16 | words`, or `DMA_ENABLE << 16 | halfwords`.

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
   VBlank (`swi 2`) and updates key repeat.
4. **VBlank** (`System_VBlankHandler`, `runtime/frame.c`), reached through
   the IWRAM dispatcher and `gIrqHandlers`: trade the link cable payloads,
   step the music's fades and the screen fade, and if the frame loop finished
   a frame, copy its OAM list and BG scroll registers and flush the queued
   register writes. Then run the one-shot VBlank hook and the VBlank tasks
   (`SCHEDULER_KEY_VBLANK`), read the keypad, count the frame and mix the
   next sound buffer.

## Limits

- 62 heap slots (`games/tbs/include/heap_state.h`); the IWRAM heap ends at
  0x03007800 and the EWRAM heap at 0x02040000.
- 20 scheduled tasks at a time (`runtime/scheduler.c`).
- 96 sprite VRAM cache entries over 512 blocks of 64 bytes.
- 256 sprite depth slots.
