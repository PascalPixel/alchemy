#include "types.h"

#define BattlePresentation_WaitForLinkedState Func_080b60a0

s32 BattlePresentation_WaitForLinkedState(void)
{
    u8 *battle = *(u8 **)0x03001e74;
    u16 *remote;
    u16 *local;
    s32 misses = 0;
    s32 attempt;

    if (battle[0x44] != 0) {
        u32 side = battle[0x50];
        u32 opposite_side = 1;

        opposite_side ^= side;
        side = opposite_side << 1;
        side += opposite_side;
        side <<= 3;
        remote = (u16 *)(0x02002024 + side);
        local = (u16 *)0x02002224;
        if (battle[0x52] == 0) {
            local[0] = 0x45;
            local[1] = 0x58;
            local[2] = 0x45;
            local[3] = 0x43;

            for (attempt = 0; attempt <= 29; attempt++) {
                if ((*(u16 *)0x03001f64 & 3) != 3) {
                    misses++;
                    if (misses > 24) {
                        return -1;
                    }
                } else {
                    misses = 0;
                    if (local[2] == remote[2] && local[3] == remote[3]) {
                        return 0;
                    }
                }
                Func_080030f8(1);
            }
        }
        return -1;
    }
    return 0;
}
