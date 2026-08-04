#include "object_runtime.h"

s32 Func_0800ca98(struct ObjectRuntime *object)
{
    s32 first;
    s32 second;

    if (object->flags == 0) {
        second = object->target_x;
        if (second == 0x80000000) {
            first = object->target_y;
            goto block_4;
        }
        goto block_6;
    }
    first = object->target_x;
    second = 0x80000000;
block_4:
    if ((first != second) || (object->target_z != first)) {
block_6:
        return 0;
    }
    return 1;
}
