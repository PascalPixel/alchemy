/* Draft of Func_02000374 of overlay 692 (32 instructions, 32 differ): the native reloads the
 * three position words on every pass and walks the object slots with a pointer; the draft
 * moves the first position word out of the loop. */
#include "local.h"
#include "eventwrk.h"

struct ObjectRuntime *Func_02000374(s32 *position)
{
    struct EventWork *work = Ram_HeapSlots->event_work;
    u32 i;

    for (i = 8; i < 64; i++) {
        struct ObjectRuntime *object = work->objects[i];

        if (position[0] >> 20 == object->x >> 20 && position[1] >> 20 == object->y >> 20
            && position[2] >> 20 == object->z >> 20)
            return object;
    }
    return NULL;
}
