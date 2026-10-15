#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);

void Func_020017bc(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    ObjectMotion_SetActionVariant(id, 3);
    object->collisionFlags = 8;
    /* FAKEMATCH: the loop note ends the scheduling block: the native stores the collision flags before it copies the object for the priority flags (2 differ without it; a pointer to the flags 6). */
    do
    {
    } while (0);
    object->priorityFlags |= 2;
    object->motionFlags = 0;
    object->y = 0x400000;
}
