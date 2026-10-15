#include "caverns.h"

struct FieldActor *Func_020000d0(s32 x, s32 y, s32 z, s32 type)
{
    struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

    if (object != NULL) {
        Object_SetSpritePriority(object, 1);
        object->motionFlags = 0;
        object->collisionFlags = 8;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Object_SetPartAttribute(object, 15);
        object->priorityFlags |= 0x22;
        return object;
    }
    return NULL;
}
