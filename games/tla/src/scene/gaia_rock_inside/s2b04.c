#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(u32 id);

void Func_02002b04(u32 id, s8 mode)
{
    struct ObjectRuntime *object = Object_GetById(id);

    object->motion_flags = 0;
    object->priority_flags |= 0x80;
    object->acceleration = 0x1000;
    object->speed_limit = 0x10000;
    object->y = 0x100000;
    object->action = object->x >> 16;
    object->unknown_66 = object->z >> 16;
    Object_SetModeById(id, mode);
}
