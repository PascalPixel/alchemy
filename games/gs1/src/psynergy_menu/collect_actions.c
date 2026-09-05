#include "types.h"

#define PsynergyMenu_CollectActions Func_080a68ec
#include "owner_state.h"

u8 *Ability_GetData(s32 action);

#define ACTION_ID_MASK 0x3FFF

/* PsynergyMenu_CollectActions per games/gs1/include/psynergy_menu.h. */
s32 PsynergyMenu_CollectActions(struct OwnerActionState *owner, u16 *actions, s32 mode)
{
    s32 n;
    u16 *out;
    s32 outerCount;
    s32 count;
    s32 i;
    s32 j;
    s32 off;

    outerCount = (mode != 2) ? 4 : 3;
    for (n = 62; n >= 0; n -= 2) {
        u16 *q = (u16 *)((u8 *)actions + n);
        *q = 0;
        *q = 0;
    }
    count = 0;

    if (mode == 1) {
        for (i = 0, off = 88, out = actions; i <= 31; i++, off += 4) {
            if (*(u16 *)(off + (s32)owner) != 0) {
                if (Ability_GetData(*(u16 *)(off + (s32)owner) & ACTION_ID_MASK)[12] != 0) {
                    *out = *(u16 *)((s32)owner + off);
                    out++;
                    count++;
                }
            }
        }
    } else {
        for (j = 0; j < outerCount; j++) {
            out = (u16 *)(count * 2 + (s32)actions);

            for (i = 0; i < 32; i++) {
                if (owner->action_slots[i].encoded_action != 0) {
                    u8 *ability = Ability_GetData(owner->action_slots[i].encoded_action & ACTION_ID_MASK);

                    if (j == 0 && (ability[12] != 0 || (ability[1] & 0x40) != 0)) {
                        *out = owner->action_slots[i].encoded_action;
                        out++;
                        count++;
                    } else if (j == 1) {
                    } else if (j == 2) {
                    } else if (j == 3 && ability[12] == 0 && (ability[1] & 0x40) == 0) {
                        *out = owner->action_slots[i].encoded_action;
                        out++;
                        count++;
                    }
                }
            }
        }
    }

    return count;
}
