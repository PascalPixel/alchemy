#include "types.h"
#include "object_runtime.h"

s32 Object_IsTargetUnset(struct ObjectRuntime *object)
{
    s32 first;
    s32 second;

    if (object->motionFlags == 0) {
        /* FAKEMATCH: the one-pass block's end is a scheduling barrier: the native loads targetX before it builds 0x80000000, here and in the other arm (two reordered pairs, differ 4 without; an empty asm on the loaded value does not move them). */
        do {
            second = object->targetX;
        } while (0);
        if (second != (s32)0x80000000)
            return 0;
        first = object->targetY;
    } else {
        /* FAKEMATCH: as above. */
        do {
            first = object->targetX;
        } while (0);
        second = (s32)0x80000000;
    }
    if (first != second || object->targetZ != first)
        return 0;
    return 1;
}

s32 Func_08023f94(struct ObjectRuntime *object)
{
    return Object_IsTargetUnset(object);
}
