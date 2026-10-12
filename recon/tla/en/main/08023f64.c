#include "types.h"
#include "object_runtime.h"

s32 Object_IsTargetUnset(struct ObjectRuntime *object)
{
    s32 first;
    s32 second;

    if (object->motion_flags == 0) {
        second = object->target_x;
        if (second != (s32)0x80000000)
            return 0;
        first = object->target_y;
    } else {
        first = object->target_x;
        second = (s32)0x80000000;
    }
    if (first != second || object->target_z != first)
        return 0;
    return 1;
}
