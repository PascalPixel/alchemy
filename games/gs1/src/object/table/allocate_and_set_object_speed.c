#include "object_runtime.h"

#define ObjectTable_AllocateAndSetObjectSpeed Func_080933d4

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

struct ObjectOwner_080933d4 {
    u8 unknown_000[0x1e0];
    struct ObjectRuntime *object;
};

void ObjectTable_AllocateAndSetObjectSpeed(s32 first, s32 second)
{
    struct ObjectOwner_080933d4 *owner = Runtime_AllocateBlock(0x1b, 0xccc);
    owner->object->speed_limit = first;
    owner->object->acceleration = second;
}
