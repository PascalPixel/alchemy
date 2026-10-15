#include "sea_of_time.h"

struct SeaOfTimeObject *Func_020000d0(s32 x, s32 y, s32 z, s32 type)
{
    struct SeaOfTimeObject *object = Engine_ObjectCreate(type, x, y, z);

    if (object != NULL) {
        Object_SetSpritePriority(object, 1);
        object->motion_flags = 0;
        object->collision_flags = 8;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Object_SetPartAttribute(object, 15);
        object->priority_flags |= 34;
        return object;
    }
    return NULL;
}
