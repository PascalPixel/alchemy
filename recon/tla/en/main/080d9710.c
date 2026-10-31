// Near miss, differ 2 of 57 (tla-en, 2026-10-31): one swapped pair. The
// native spills the third argument (str r2, [sp, #16]) before building
// 0x0300009c; here the constant's first movs goes first. In sched2 that movs
// starts a chain into the work's load and the first call, priority 69, against
// the spill's 68, so no statement order moves it (scheduler order). The
// one-pass block this draft used before moved the spill up but also the
// index's copy to r10: differ 2 as well, so it is gone. An empty asm before
// the work's load measured 14; u32 ids, a literal 0x0300009c, early returns,
// reordered locals and four permuter minutes changed nothing.
// Records two objects as the ends of the heap slot 0x9c work, draws
// the slot's chain between them through Func_080d9790 (each end raised by
// 0x80000, wobbling by the sine of a shared angle) and advances that angle.
// A sibling of Func_080d9104 and Func_080d92d4.
#include "types.h"
#include "fixed_math.h"
#include "actor.h"
#include "ram_buffer.h"

struct ChainWork {
  struct Actor *from;
  struct Actor *to;
};

extern s32 gData_080f3940;

void Func_080d9790(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                   s32 wobble);

void Func_080d9710(s32 index, s32 fromId, s32 toId) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  struct Actor *from = ObjectTable_Get(fromId);
  struct Actor *to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    work->from = from;
    work->to = to;
    Func_080d9790(index, from->x, from->y + 0x80000, from->z, to->x,
                  to->y + 0x80000, to->z, Trig_Sin(gData_080f3940) * 4);
    gData_080f3940 += 0x1000;
  }
}
