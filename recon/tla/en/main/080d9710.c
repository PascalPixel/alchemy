// Near miss, differ 6 of 57, instruction order only. A one-pass block before
// the work's load (FAKEMATCH, differ 8 without it) spills the third argument
// before 0x03000000 is built, as the native does, but copies the index to r10
// one row early; the native also adds the raise to the first y after the
// target's y load and the sine's shift (sched2 ties). An empty asm there
// measured 14.
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
  struct ChainWork *work;
  struct ObjectRuntime *from;
  struct ObjectRuntime *to;

  /* FAKEMATCH: spills the third argument before the work loads (differ 8
   * without it). */
  do {
  } while (0);
  work = Ram_HeapSlots->unk84[6];
  from = ObjectTable_Get(fromId);
  to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    work->from = from;
    work->to = to;
    Func_080d9790(index, from->x, from->y + 0x80000, from->z, to->x,
                  to->y + 0x80000, to->z, Trig_Sin(gData_080f3940) * 4);
    gData_080f3940 += 0x1000;
  }
}
