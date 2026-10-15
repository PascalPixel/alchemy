#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(u32 id);

void Func_02002b04(u32 id, s8 mode)
{
    struct ObjectRuntime *object = Object_GetById(id);

    object->motionFlags = 0;
    object->priorityFlags |= 0x80;
    object->acceleration = 0x1000;
    object->speedLimit = 0x10000;
    object->y = 0x100000;
    object->action = object->x >> 16;
    object->unk66 = object->z >> 16;
    Object_SetModeById(id, mode);
}
