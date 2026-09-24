#include "TYPES.H"

s32 Engine_RandomNext();

/* NONMATCHING: 86 of 88 bytes, 12 halfword edits (2026-09-24). The
 * reference copies the loaded wait byte to r7 (reused as the zero heading)
 * and keeps actor/timer in r5/r6; here actor, timer and wait rotate. */
s32 Func_020008e0(u8 *actor)
{
    u8 *timer = actor + 98;
    u8 wait = *timer;

    if (wait != 0) {
        *timer = wait - 1;
    } else {
        u32 roll = (u32)(Engine_RandomNext() * 300) >> 16;

        if (roll > 200) {
            s32 dir = 0xd000;

            *(u16 *)(actor + 6) = dir;
        } else if (roll > 100) {
            s32 dir = 0x5000;

            *(u16 *)(actor + 6) = dir;
        } else {
            *(u16 *)(actor + 6) = 0;
        }
        *timer = ((u32)(Engine_RandomNext() * 80) >> 16) + 80;
    }
    return 1;
}
