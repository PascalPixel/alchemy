#include "worka0.h"
#include "object_runtime.h"

void Func_080d9f1c(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2);

void Func_080d9e98(s32 slot_no, s32 first_id, s32 second_id)
{
    struct WorkA0 *work = WORK_A0;
    struct ObjectRuntime *first = ObjectTable_Get(first_id);
    struct ObjectRuntime *second = ObjectTable_Get(second_id);

    if (first != NULL && second != NULL) {
        struct WorkA0Slot *slot = &work->slots[slot_no];

        slot->first = first_id;
        slot->second = second_id;
        Func_080d9f1c(slot_no, first->x, first->y + 0x80000, first->z, second->x, second->y + 0x80000, second->z);
        Object_SetMode(first, 4);
        Object_SetMode(second, 4);
    }
}
