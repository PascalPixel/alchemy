/* Draft, differ 22 of 32: the native reloads position[0] on every pass and keeps the slot pointer in r5, the count in r4; this form (a goto loop) still hoists the first word into r6. Tried: a for loop over work->objects[i] (differ 32), a do-while over a slot pointer (differ 24). */
#include "../../../../games/tla/src/field/overlay_692/local.h"
#include "eventwrk.h"

struct ObjectRuntime *Func_02000374(s32 *position)
{
    struct EventWork *work = Ram_HeapSlots->event_work;
    struct ObjectRuntime **slot = &work->objects[8];
    u32 i = 8;
    struct ObjectRuntime *object;

loop:
    object = *slot++;
    if (position[0] >> 20 == object->x >> 20 && position[1] >> 20 == object->y >> 20
        && position[2] >> 20 == object->z >> 20)
        return object;
    i++;
    if (i <= 63)
        goto loop;
    return NULL;
}
