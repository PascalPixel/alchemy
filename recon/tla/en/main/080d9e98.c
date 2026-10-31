// Matches: differ 0 of 60 (tla-en, 2026-10-31), with no device: the plain
// constant 0x80000 now schedules as the native, and the earlier lift built
// behind an empty asm is no longer needed.
#include "worka0.h"
#include "actor.h"

void Func_080d9f1c(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2);

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
