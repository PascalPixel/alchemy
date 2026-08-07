#include "types.h"

extern u8 *Data_03001ebc;
extern u8 *Func_0808a080(s32 actorId);
extern s32 Func_02000408(u8 *actor, u8 *target, s32 distanceLimit, s32 force);

/* Complete 156-byte actor-18 proximity counter owner. */
s32 Func_02000500(u8 *actor)
{
    s32 active = 0;
    u8 *leader;

    if (*(s32 *)(actor + 0x38) == (s32)0x80000000 &&
        *(s32 *)(actor + 0x40) == (s32)0x80000000)
        return 0;

    leader = Func_0808a080(0);
    if ((unsigned)((*(s32 *)(leader + 8) >> 20) - 17) <= 1 &&
        (*(s32 *)(leader + 16) >> 20) == 14 &&
        (*(s32 *)(actor + 8) >> 20) <= 19 &&
        *(s32 *)(actor + 0x24) <= 0) {
        if (*(s32 *)(leader + 8) <= *(s32 *)(actor + 8)) {
            actor[0x62]++;
            active = 1;
        }
    } else {
        actor[0x62] = 0;
    }

    if (active != 0 && actor[0x62] > 119) {
        *(u16 *)(*(u8 **)Data_03001ebc + 0x182) = 200;
        actor[0x62] = 0;
    }
    Func_02000408(actor, leader, 18, active);
    return 0;
}
