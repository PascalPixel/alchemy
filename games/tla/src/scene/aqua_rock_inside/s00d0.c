#include "local.h"

struct ObjectRuntime *Func_020000d0(s32 x, s32 y, s32 z, s32 kind)
{
    struct ObjectRuntime *object = Engine_ObjectCreate(kind, x, y, z);

    if (object != 0) {
        Object_SetSpritePriority(object, 1);
        object->motion_flags = 0;
        object->collision_flags = 8;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Object_SetPartAttribute(object, 15);
        object->priority_flags |= 0x22;
        return object;
    }
    return 0;
}
