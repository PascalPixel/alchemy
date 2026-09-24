#include "TYPES.H"

s32 Engine_RandomNext();

/* Counts down the wait timer; when it runs out, turns to a random heading and waits again. */
s32 FuneKanpan_Func020008e0(u8 *actor)
{
    if (actor[98] != 0) {
        actor[98]--;
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
        actor[98] = ((u32)(Engine_RandomNext() * 80) >> 16) + 80;
    }
    return 1;
}
