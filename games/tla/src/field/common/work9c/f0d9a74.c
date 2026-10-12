#include "work9c.h"

s32 Func_080d9a74(s32 id, s32 *value, s32 *index)
{
    struct Work9c *work = WORK_9C;
    struct Work9cSlot *slot = work->slots;
    s32 i;

    for (i = 0; i <= 7; i++) {
        if (slot->head != NULL && slot->id == id) {
            *value = slot->value;
            *index = i;
            return 0;
        }
        slot++;
    }
    return -1;
}
