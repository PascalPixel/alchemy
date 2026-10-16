// Near miss, differ 10 of 60: scheduling only. The native loads the work
// slot before it copies the second argument, and orders the seven argument
// loads and the 0x80000 constant differently; a barrier after the work load
// moves the frame setup instead (differ 14).
#include "worka0.h"
#include "object_runtime.h"

void Func_080d9f1c(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2);

void Func_080d9e98(s32 slotNo, s32 firstId, s32 secondId) {
  struct WorkA0 *work = WORK_A0;
  struct ObjectRuntime *first = ObjectTable_Get(firstId);
  struct ObjectRuntime *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    struct WorkA0Slot *slot = &work->slots[slotNo];

    slot->first = firstId;
    slot->second = secondId;
    Func_080d9f1c(slotNo, first->x, first->y + 0x80000, first->z, second->x,
                  second->y + 0x80000, second->z);
    Object_SetMode(first, 4);
    Object_SetMode(second, 4);
  }
}
