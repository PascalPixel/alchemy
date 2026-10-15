#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

s32 EventRuntime_GetControlledOwner(void);

void Func_080ec16c(s32 x, s32 z)
{
    s32 owner = EventRuntime_GetControlledOwner();
    s32 *camera = *(s32 **)Ram_HeapSlots->mapWork;
    struct ObjectRuntime *object = ObjectTable_Get(owner);

    if (object != NULL) {
        x >>= 16;
        x = ((x << 14) + 0x434000) / 853;
        z >>= 16;
        object->x = x << 16;
        object->z = (((z << 14) + 0x1c0000) / 640) << 16;
        camera[0] = object->x;
        camera[2] = object->z;
    }
}
