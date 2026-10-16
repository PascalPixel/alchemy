// Near miss, differ 6 of 18: the native computes active != 0 without a branch
// (negs r0, r3; orrs r0, r3; lsrs r0, r0, #31) after its early return of 0;
// every form tried here tests and branches (cmp r3, #0; beq; movs r0, #1): the
// early return, a result set in both arms, an empty asm between the two, the
// value returned from inside the if (differ 12), and the spelled-out (u32)(-x |
// x) >> 31 (differ 5, registers).
#include "types.h"
#include "ram_buffer.h"

struct Work080cf158 {
  u8 unk000[0xca4];
  s8 active;
};

s32 Func_080cf158(void) {
  struct Work080cf158 *work = Ram_HeapSlots->eventWork;

  if (work == NULL)
    return 0;
  return work->active != 0;
}
