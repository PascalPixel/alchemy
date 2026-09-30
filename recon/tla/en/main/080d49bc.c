/*
 * Draft: ObjectMotion_MoveTowardTarget does not yet match; 9 halfwords differ from ☀️'s C, first at +0xc (beq +8c).
 * Links as recon/tla/raw/080d49bc.s.
 */
#include "IWRAM_CALL.H"

s32 Object_SetPosition(s32, s32, s32, s32);
s32 Object_SetMode(s32, s32);

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
                Math_Div(cellX << 20, arg0);
            Object_SetPosition(object, newX, *(s32 *)(object + 0x0c),
                          *(s32 *)(object + 0x10) +
                              Math_Div(cellY << 20, arg0));
            Object_SetMode(object, 2);
        } else {
            Object_SetMode(object, 1);
        }
    }
    return 1;
}
