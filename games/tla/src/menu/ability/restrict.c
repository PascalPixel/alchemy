#include "types.h"
#include "battle_system.h"

s32 PsynergyMenu_IsActionRestricted(s32 no)
{
    u8 *action = BattleActGet((u32)(no << 18) >> 18);
    u32 flags;

    if (action[6] == 0) {
        flags = action[1] & 0xc0;
        no = 1;
        if (flags != 0xc0)
            return no;
    }
    no = 0;
    return no;
}
