/* Draft, differ 32 of 32 (the same instructions in other registers): the listing reloads position[0] each pass where this draft hoists it out of the loop (loop.c). Tried: comparisons either way round (36), the shared struct EventWork; alchemy permute reaches 12 only by adding position++; position--; to the loop, no source form. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "eventwrk.h"

struct ObjectRuntime *Func_0200034c(s32 *position)
{
    struct EventWork *work = Ram_HeapSlots->event_work;
    u32 i;

    for (i = 8; i < 64; i++)
    {
        struct ObjectRuntime *object = (struct ObjectRuntime *)work->objects[i];

        if (position[0] >> 20 == object->x >> 20 && position[1] >> 20 == object->y >> 20
            && position[2] >> 20 == object->z >> 20)
            return object;
    }
    return 0;
}
