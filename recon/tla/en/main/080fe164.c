/*
 * Draft: PsynergyMenu_IsActionRestricted does not yet match; 2 halfwords differ from ☀️'s C, first at +0xa (ldrb r3, [r0, #6]).
 * Links as recon/tla/raw/080fcf14.s.
 */
#include "TYPES.H"

u8 *BattleAction_Get(u32 action);

s32 PsynergyMenu_IsActionRestricted(s32 no)
{
    u8 *action = BattleAction_Get((u32)(no << 18) >> 18);
    u32 flags;

    if (action[12] != 0)
        goto restricted;
    flags = action[1] & 0xc0;
    no = 1;
    if (flags != 0xc0)
        goto done;
restricted:
    no = 0;
done:
    return no;
}
