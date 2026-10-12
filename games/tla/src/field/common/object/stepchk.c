#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

u32 Func_080202f0(s32 terrain, s32 x, s32 z);

s32 Func_080d4ccc(struct ObjectRuntime *object, s32 *position)
{
    struct MapWork *map = Ram_HeapSlots->map_work;
    s8 *event = Ram_HeapSlots->event_work;

    if (map == 0 || event[0x18a] == 3
        || Func_080202f0(object->terrain_id, position[0], position[2]) <= 2)
        return 0;
    return 1;
}
