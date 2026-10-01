#include "TYPES.H"
#include "IWRAM_CALL.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 ObjectDispatch_SetSingleChildField26Far(void *, s32);
s32 __divsi3(s32, s32);
s32 Object_SetPosition(s32, s32, s32, s32);
s32 Object_SetMode(s32, s32);

s32 Object_ResetAndClearField59(void *obj)
{
    ObjectDispatch_SetSingleChildField26Far(obj, 0);
    FIELD_AT_OFFSET(obj, s8 *, 0x59) = 0;
    return 0;
}

/* ⚓️ splits ☀️'s reset in two: the child field alone, and a flag bit. */
s32 Object_ResetChildField26(void *obj)
{
    ObjectDispatch_SetSingleChildField26Far(obj, 0);
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

s32 ObjectMotion_MoveTowardTarget(s32 arg0)
{
    s32 object;
    void *target;
    s32 deltaX;
    s32 deltaY;
    s32 cellX;
    s32 cellY;
    s32 newX;
    s32 distance;

    object = arg0;
    target = *(void **)(object + 0x68);
    if (target != 0) {
        deltaX = *(s32 *)(target + 8) - *(s32 *)(object + 8);
        if (deltaX < 0)
            deltaX += 0xffff;
        cellX = deltaX >> 16;
        deltaY = *(s32 *)(target + 0x10) - *(s32 *)(object + 0x10);
        if (deltaY < 0)
            deltaY += 0xffff;
        cellY = deltaY >> 16;
        distance = Iwram_Sqrt(cellX * cellX + cellY * cellY);
        arg0 = *(s16 *)(object + 0x64);
        if (distance >= arg0) {
            newX = *(s32 *)(object + 8) +
                __divsi3(cellX << 20, arg0);
            Object_SetPosition(object, newX, *(s32 *)(object + 0x0c),
                          *(s32 *)(object + 0x10) +
                              __divsi3(cellY << 20, arg0));
            Object_SetMode(object, 2);
        } else {
            Object_SetMode(object, 1);
        }
    }
    return 1;
}
