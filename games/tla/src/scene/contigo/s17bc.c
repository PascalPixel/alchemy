#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);

void Func_020017bc(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    ObjectMotion_SetActionVariant(id, 3);
    object->collision_flags = 8;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native stores the collision flags before it copies the object for the priority flags (2 differ without it; a pointer to the flags 6). */
    object->priority_flags |= 2;
    object->motion_flags = 0;
    object->y = 0x400000;
}
