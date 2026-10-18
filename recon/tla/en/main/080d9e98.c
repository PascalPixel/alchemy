// Near miss, differ 6 of 60, instruction order only (8 without the tagged
// barrier). As in Func_080da1b8: the native copies the first id to r8 after
// the work's load, loads the second end's y after building 0x80000, and adds
// the raise to the first y straight after; this draft loads that y first and
// adds the first raise last. A barrier after the work load moves the frame
// setup instead (differ 14).
#include "worka0.h"
#include "object_runtime.h"

void Func_080d9f1c(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2);

void Func_080d9e98(s32 slotNo, s32 firstId, s32 secondId) {
  struct WorkA0 *work = WORK_A0;
  struct ObjectRuntime *first = ObjectTable_Get(firstId);
  struct ObjectRuntime *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    struct WorkA0Slot *slot = &work->slots[slotNo];
    s32 lift;

    slot->first = firstId;
    slot->second = secondId;
    lift = 128;
    /* FAKEMATCH: starts 0x80000 before the stack arguments load, as the
     * native does (differ 8 without it). */
    asm volatile("" : "+l"(lift));
    lift <<= 12;
    Func_080d9f1c(slotNo, first->x, first->y + lift, first->z, second->x,
                  second->y + lift, second->z);
    Object_SetMode(first, 4);
    Object_SetMode(second, 4);
  }
}
