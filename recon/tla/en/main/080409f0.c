// Draft, differ 2 of 39 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Near miss, differ 2 of 39 (one reordered row; 4 before the barrier):
// written from the listing. It opens a 15x9 window at (7, 3), creates an
// output there from fixed block B, keeps it in the work at 0x5a4 and places
// it with Func_08108048 at the window's corner. The empty asm reading the
// stored output and rewriting window makes the coordinates' loads wait for
// the store, as the native's do (to be tagged FAKEMATCH(barrier)). Left: the
// native copies the entry's address to r0 right after the store; this
// copies it after the loads. With the address also pinned to r0 and read by
// the asm the copy comes first, but x's subs r1, #4 then follows y's adds
// r2, #12 (differ 2); computing y first differs 8, and the address as an asm
// output 18. An empty volatile asm after the store moves the argument set-up
// of RenderOutput_Create instead (differ 4). The other editions' scaffold
// holds this function in one piece with the next.

#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                    s32 style);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
void Func_08108048(void *entry, s32 x, s32 y);

struct RenderInput *Func_080409f0(void) {
  u8 *work = Ram_HeapSlots->unkd0[0];
  struct RenderInput *window;
  s32 no;

  window = UiWindow_Create(7, 3, 15, 9, 2);
  no = Resource_LoadFixedBlockBIntoFreeSlot();
  *(struct RenderOutput **)(work + 0x5a4) =
      RenderOutput_Create(no, 0x40000000, window, 0, 0);
  __asm__("" : "+l"(window) : "m"(*(struct RenderOutput **)(work + 0x5a4)));
  Func_08108048(work + 0x5a4, ((s16)window->x << 3) - 4,
                ((s16)window->y << 3) + 12);
  return window;
}
