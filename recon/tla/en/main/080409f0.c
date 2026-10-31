// Near miss, differ 2 of 39 (tla-en, 2026-10-31): one reordered row, a
// scheduler cost the C cannot move. The native copies the cursor's address
// to r0 right after the store; this copies it after the coordinates' loads.
// Written from the listing. It opens a 15x9 window at (7, 3), creates an
// output there from fixed block B, keeps it as the cursor at 0x5a4 in the
// work (the slot debug_warp_window.c calls cursor) and places it with
// Func_08108048 at the window's corner.
// The native's store comes before the loads, so it may alias the window's
// fields: stored through a union member (alias set 0) it does, with no
// device. Plain, the store is independent of the loads and sinks after them
// (differ 4). The empty asm reading the stored output and rewriting window
// (a barrier) also measures differ 2. What remains: in sched2 the loads are
// ready one cycle after the store (store to ldrsh costs 1; -mthumb-slow-loads
// charges 2 only for a plain-MEM ldr), and at that cycle their priority (70)
// beats the copy's (66). With a 2-cycle store-to-ldrsh cost the native's
// whole order follows (store, copy, both loads, shifts, subs, adds).
// Earlier device search (2026-10-26): no single empty asm, one-pass block,
// wrapping block or swap of neighbouring statements brings it closer, nor
// any pair. With the address pinned to r0 and read by the asm the copy
// comes first, but x's subs r1, #4 then follows y's adds r2, #12 (differ 2);
// computing y first differs 8, the address as an asm output 18, and an
// empty volatile asm after the store moves WindowSprite_Create's argument
// set-up instead (differ 4). The other editions' scaffold holds this
// function in one piece with the next.

#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct RenderInput *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                    s32 style);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
void Func_08108048(void *cursor, s32 x, s32 y);

struct RenderInput *Func_080409f0(void) {
  u8 *work = Ram_HeapSlots->unkd0[0];
  struct RenderInput *window;
  s32 no;

  window = Window_Create(7, 3, 15, 9, 2);
  no = Resource_LoadFixedBlockBIntoFreeSlot();
  ((union { struct WindowSprite *cursor; } *)(work + 0x5a4))->cursor =
      WindowSprite_Create(no, 0x40000000, window, 0, 0);
  Func_08108048(work + 0x5a4, ((s16)window->x << 3) - 4,
                ((s16)window->y << 3) + 12);
  return window;
}
