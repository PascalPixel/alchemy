/*
 * Step an object toward its linked target, or change its mode when the
 * remaining distance is too small.
 */
#include "types.h"
#include "scene.h"
#include "abi/object/motion/aim/move_toward_target.h"

/*
 * Obj_SetMode names a `bx rN` slot, so the call is indirect through the
 * register that slot selects; the trailing argument is the callee address
 * loaded into that register, not a parameter of the callee. The callee
 * takes one argument and returns one; it is fed a sum of squares and its
 * result used as a length, which reads as a square root but is not
 * established.
 */

s32 FixedPoint_Ratio(s32, s32);
s32 Object_SetPosition(s32, s32, s32, s32);
s32 Object_SetMode(s32, s32);

/*
 * The routine address is held in a local and passed as the trailing
 * argument -- that local is the callee load. The parameter is then reused
 * to hold the halfword at +0x64.
 */
s32 Motion_MoveTowardTarget(s32 arg0)
{
    s32 squareRoot = 0x030001d8;
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
        distance = Obj_SetMode(cellX *cellX + cellY *cellY,
                                 (s32)target, cellY, squareRoot);
        arg0 = *(s16 *)(object + 0x64);
        if (distance >= arg0) {
            newX = *(s32 *)(object + 8) +
                FixedPoint_Ratio(cellX << 20, arg0);
            Object_SetPosition(object, newX, *(s32 *)(object + 0x0c),
                          *(s32 *)(object + 0x10) +
                              FixedPoint_Ratio(cellY << 20, arg0));
            Object_SetMode(object, 2);
        } else {
            Object_SetMode(object, 1);
        }
    }
    return 1;
}
