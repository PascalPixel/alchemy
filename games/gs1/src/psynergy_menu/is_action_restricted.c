#include "psynergy_menu.h"

u8 *Func_08077080(u32 action);

s32 PsynergyMenu_IsActionRestricted(s32 encoded_action)
{
    u8 *action = Func_08077080((u32)(encoded_action << 18) >> 18);
    u32 masked_flags;

    if (action[12] != 0)
        goto restricted;
    masked_flags = action[1] & 0xc0;
    encoded_action = 1;
    if (masked_flags != 0xc0)
        goto done;
restricted:
    encoded_action = 0;
done:
    return encoded_action;
}
