#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "OBJECT_RUNTIME.H"
#include "OBJECT_DISPATCH.H"

s32 __divsi3(s32, s32);

s32 Object_ResetAndClearField59(struct ObjectRuntime *obj)
{
    ObjectDispatch_SetSingleChildField26Far((struct DispatchObject *)obj, 0);
    obj->unknown_59 = 0;
    return 0;
}

/* ⚓️ splits ☀️'s reset in two: the child field alone, and a flag bit. */
s32 Object_ResetChildField26(struct ObjectRuntime *obj)
{
    ObjectDispatch_SetSingleChildField26Far((struct DispatchObject *)obj, 0);
    return 0;
}

struct ObjectFlags23 {
    u8 unknown_00[0x23];
    u8 flags_23;
};

s32 Object_SetField23Bit5(struct ObjectFlags23 *obj)
{
    obj->flags_23 |= 0x20;
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

    target = object->linked_object;
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
