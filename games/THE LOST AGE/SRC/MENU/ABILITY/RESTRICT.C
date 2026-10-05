#include "TYPES.H"
#include "BATTLE_RUNTIME.H"

/* ⚓️'s ability records keep the restriction byte at 6; ☀️'s at 12. */
s32 PsynergyMenu_IsActionRestricted(s32 no)
{
    u8 *action = BattleAction_Get((u32)(no << 18) >> 18);
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
