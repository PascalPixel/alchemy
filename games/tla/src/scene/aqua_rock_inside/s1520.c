#include "local.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

s32 Func_02001520(struct ObjectRuntime *object)
{
    struct ObjectRuntime *target = object->linked_object;
    s32 steps;
    s32 from;

    object->bounce = (target->x - object->x) / (steps = 10);
    asm volatile(""); /* FAKEMATCH: ⚓️ stores the x step, then reads the object height, then the target height; unfenced, the target height is read first */
    from = object->y;
    asm volatile(""); /* FAKEMATCH: second fence of the pair above */
    object->unknown_4c = (target->y - from + 0x100000) / steps;
    object->gravity = (target->z - object->z) / steps;
    BYTE_AT(object, 98) = steps;
    return 0;
}
