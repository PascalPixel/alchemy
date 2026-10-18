// Near miss, differ 8 of 57, instruction order only: the native spills the
// third argument before building 0x03000000, and adds the raise to the first
// y after the target's y load and the sine's shift (sched2 ties).
// Records two objects as the ends of the heap slot 0x9c work, draws
// the slot's chain between them through Func_080d9790 (each end raised by
// 0x80000, wobbling by the sine of a shared angle) and advances that angle.
// A sibling of Func_080d9104 and Func_080d92d4.
#include "types.h"
#include "fixed_math.h"
#include "object_runtime.h"
#include "ram_buffer.h"

struct ChainWork {
  struct ObjectRuntime *from;
  struct ObjectRuntime *to;
};

extern s32 gData_080f3940;

void Func_080d9790(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                   s32 wobble);

void Func_080d9710(s32 index, s32 fromId, s32 toId) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  struct ObjectRuntime *from = ObjectTable_Get(fromId);
  struct ObjectRuntime *to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    work->from = from;
    work->to = to;
    Func_080d9790(index, from->x, from->y + 0x80000, from->z, to->x,
                  to->y + 0x80000, to->z, Trig_Sin(gData_080f3940) * 4);
    gData_080f3940 += 0x1000;
  }
}
