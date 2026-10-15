#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "object_dispatch.h"

s32 __divsi3(s32, s32);

s32 Object_ClearChildAndCollisionFlags(struct ObjectRuntime *obj)
{
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)obj, 0);
    obj->collisionFlags = 0;
    return 0;
}

s32 Object_ClearChildFlags(struct ObjectRuntime *obj)
{
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)obj, 0);
    return 0;
}

struct ObjectFlags23 {
    u8 unk00[0x23];
    u8 flags23;
};

s32 Object_SetPriorityBit5(struct ObjectFlags23 *obj)
{
    obj->flags23 |= 0x20;
    return 0;
}

s32 ObjectMotion_MoveTowardTarget(struct ObjectRuntime *object)
{
    struct ObjectRuntime *target;
    s32 deltaX;
    s32 deltaY;
    s32 cellX;
    s32 cellY;
    s32 newX;
    s32 distance;
    s32 step;

    target = object->linkedObject;
    if (target != 0) {
        deltaX = target->x - object->x;
        if (deltaX < 0)
            deltaX += 0xffff;
        cellX = deltaX >> 16;
        deltaY = target->z - object->z;
        if (deltaY < 0)
            deltaY += 0xffff;
        cellY = deltaY >> 16;
        distance = Iwram_Sqrt(cellX * cellX + cellY * cellY);
        step = object->action;
        if (distance >= step) {
            newX = object->x + __divsi3(cellX << 20, step);
            Object_SetPosition(object, newX, object->y,
                object->z + __divsi3(cellY << 20, step));
            Object_SetMode(object, 2);
        } else {
            Object_SetMode(object, 1);
        }
    }
    return 1;
}
