#include "worka0.h"
#include "actor.h"

s32 Func_080d9f1c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);

void Func_080d9e98(s32 slotNo, s32 firstId, s32 secondId) {
  struct WorkA0 *work = WORK_A0;
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    struct WorkA0Slot *slot = &work->slots[slotNo];

    slot->first = firstId;
    slot->second = secondId;
    Func_080d9f1c(slotNo, first->x, first->y + 0x80000, first->z, second->x,
                  second->y + 0x80000, second->z);
    Actor_SetMode(first, 4);
    Actor_SetMode(second, 4);
  }
}
