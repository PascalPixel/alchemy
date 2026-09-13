#include "psynergy_menu.h"

u8 *Ability_GetData(u32 action);

s32 PsynergyMenu_IsActionRestricted(s32 no)
{
    u8 *action = Ability_GetData((u32)(no << 18) >> 18);
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
