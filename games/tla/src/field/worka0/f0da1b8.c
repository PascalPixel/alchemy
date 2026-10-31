#include "worka0.h"
#include "actor.h"

s32 Func_080da22c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);

void Func_080da1b8(s32 index, s32 from, s32 to) {
  struct WorkA0 *work = WORK_A0;
  struct Actor *a = ObjectTable_Get(from);
  struct Actor *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL) {
    struct WorkA0Slot *slot = &work->slots[index];

    slot->first = from;
    slot->second = to;
    Func_080da22c(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000,
                  b->z);
  }
}
