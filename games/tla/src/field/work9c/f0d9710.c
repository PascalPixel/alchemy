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
  struct ChainWork *work;
  struct Actor *from;
  struct Actor *to;

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
