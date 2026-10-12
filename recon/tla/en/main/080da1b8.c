/* Near miss, differ 12: as Func_080d8fa8, the stack arguments are scheduled apart; the first parameter is saved a
 * row early. */
#include "worka0.h"
#include "object_runtime.h"

s32 Func_080da22c(s32 index, s32 x, s32 y, s32 z, s32 to_x, s32 to_y, s32 to_z);

void Func_080da1b8(s32 index, s32 from, s32 to)
{
    struct WorkA0 *work = WORK_A0;
    struct ObjectRuntime *a = ObjectTable_Get(from);
    struct ObjectRuntime *b = ObjectTable_Get(to);

    if (a != NULL && b != NULL) {
        struct WorkA0Slot *slot = &work->slots[index];

        slot->first = from;
        slot->second = to;
        Func_080da22c(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000, b->z);
    }
}
