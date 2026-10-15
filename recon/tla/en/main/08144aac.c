/* Near miss, differ 4 of 30: The Broken Seal's BattleFx_FetchRectangleBlitters
   as ⚓️ has it, Func_081963ec taking each heap slot's offset and blitter flags.
   The 3-byte local explains the native 4-byte frame. Left: in each branch the
   native sets the second argument before it finishes the heap base (movs r5,
   #192; movs r1, #19; lsls r5, r5, #18; movs r0, #104); here it comes after r0.
   Tried: the flags in a local first, the base as one local for the whole
   function (differ 29), alchemy permute for 3 minutes. */
#include "types.h"
#include "ram_buffer.h"

void Func_081963ec(s32 slot, s32 flags);

void Func_08144aac(s32 flip, void **out) {
  u8 unused[3];

  if (flip == 0) {
    void **slots = (void **)Ram_HeapSlots;

    Func_081963ec(104, 19);
    out[0] = slots[26];
    Func_081963ec(188, 3);
    out[1] = slots[47];
  } else {
    void **slots = (void **)Ram_HeapSlots;

    Func_081963ec(104, 23);
    out[0] = slots[26];
    Func_081963ec(188, 7);
    out[1] = slots[47];
  }
}
