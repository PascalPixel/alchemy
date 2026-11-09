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
  s32 slot;

  work = Ram_HeapSlots->unk84[6];
  /* FAKEMATCH(temporary): the index copied to slot inside the block, so its
   * move to r10 follows the constant; as index throughout, 2 of 57
   * instructions differ in tla-en. */
  slot = index;
  /* FAKEMATCH(dead): a dead store to index, so GCSE keeps slot; as no
   * store, 2 of 57 instructions differ in tla-en. */
  index = 0;
  from = ObjectTable_Get(fromId);
  to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    work->from = from;
    work->to = to;
    Func_080d9790(slot, from->x, from->y + 0x80000, from->z, to->x,
                  to->y + 0x80000, to->z, Trig_Sin(gData_080f3940) * 4);
    gData_080f3940 += 0x1000;
  }
}
