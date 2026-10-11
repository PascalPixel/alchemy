/* Near miss of Func_020017bc (25 instructions, 2 differ): the native stores the collision flags
 * before it copies the object for the priority flags, the draft after. A pointer to the flags
 * measured worse (6 differ). */
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);

void Func_020017bc(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    ObjectMotion_SetActionVariant(id, 3);
    object->collision_flags = 8;
    object->priority_flags |= 2;
    object->motion_flags = 0;
    object->y = 0x400000;
}
