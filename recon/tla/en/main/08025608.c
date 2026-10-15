/* Draft, differ 2 of 65: The Broken Seal's Object_ApproachLinkedObject with
 * the square root through the IWRAM entry. The native loads object->x before
 * link->x (ldr r3, [r6, #8]; ldr r1, [r2, #8]), this loads link->x first.
 * Measured: the operands either way round (2), dz before dx (16), object->x
 * in a temporary, behind a barrier or in a one-pass block (9 to 15, the
 * temporary stays live for the move target); two minutes of permuter found
 * nothing. */
#include "types.h"
#include "object_runtime.h"
#include "iwram_call.h"

void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void ObjectDispatch_ApplyArgumentToChildren(struct ObjectRuntime *object, s32 argument);

s32 Object_ApproachLinkedObject(struct ObjectRuntime *object)
{
    s32 len;
    s32 mz;
    s32 dx;
    s32 dz;
    s32 dzh;
    s32 dxh;
    s32 n;
    s32 mx;
    s32 dx2;
    struct ObjectRuntime *link;

    link = object->linked_object;
    object->speed_limit = link->speed_limit;
    object->acceleration = link->acceleration;
    dx = link->x - object->x;
    dz = link->z - object->z;
    dxh = dx >> 0x10;
    dzh = dz >> 0x10;
    len = Iwram_Sqrt((dxh * dxh) + (dzh * dzh));
    if (len > 0x10) {
        dx2 = dx;
        n = len - 0x10;
        mx = dx2 * n / len;
        mz = dz * n / len;
        Object_SetMoveTarget(object, object->x + mx, object->y, object->z + mz);
        ObjectDispatch_ApplyArgumentToChildren(object, 2);
        object->step = (object->step + 1);
        return 1;
    }
    ObjectDispatch_ApplyArgumentToChildren(object, 1);
    return 0;
}
