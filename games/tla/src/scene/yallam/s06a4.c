#include "types.h"
#include "object_runtime.h"

s32 Func_020006a4(struct ObjectRuntime *object)
{
    s32 x = object->target_x;

    /* FAKEMATCH: the one-pass block keeps the target load before the constant; as a bare if the constant is built first (differ 2) */
    do {
        if (x == 0x80000000 && object->target_z == 0x80000000)
            return 1;
    } while (0);
    return 0;
}
