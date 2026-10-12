/* Near miss of Func_02002bd0 of overlay 692 (35 instructions, 2 differ): for the y step the
 * native loads the object's y before the target's, the draft after; x and z match. Measured:
 * a one-pass block before the y difference moves the object's load one row too far (2 differ);
 * the object's y in its own statement first swaps the two registers (6 differ). The step count
 * lives in r8 only as a variable set after the x difference. */
#include "local.h"

s32 Func_02002bd0(struct ObjectRuntime *object)
{
    struct ObjectRuntime *target = object->linked_object;
    s32 dx = target->x - object->x;
    s32 steps = 10;

    object->bounce = dx / steps;
    object->unknown_4c = (target->y - object->y + 0x100000) / steps;
    object->gravity = (target->z - object->z) / steps;
    *((u8 *)object + 98) = steps;
    return 0;
}
