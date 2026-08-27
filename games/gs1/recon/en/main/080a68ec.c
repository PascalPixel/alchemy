#include "types.h"
#include "owner_state.h"

u8 *Ability_GetData(s32 action);

#define ACTION_ID_MASK 0x3FFF

/* PsynergyMenu_CollectActions per games/gs1/include/psynergy_menu.h. */
s32 Func_080a68ec(struct OwnerActionState *owner, u16 *actions, s32 mode)
{
    u16 *p;
    s32 outerCount;
    s32 count;

    outerCount = (mode != 2) ? 4 : 3;
    p = actions + 31;
    do {
        *p = 0;
        p--;
    } while ((s32)p >= (s32)actions);
    count = 0;

    if (mode == 1) {
        u16 *out;
        s32 idx;

        out = actions;
        for (idx = 0; idx <= 31; idx++) {
            if (owner->action_slots[idx].encoded_action != 0) {
                if (Ability_GetData(owner->action_slots[idx].encoded_action & ACTION_ID_MASK)[12] != 0) {
                    *out = owner->action_slots[idx].encoded_action;
                    out++;
                    count++;
                }
            }
        }
    } else {
        s32 j;

        for (j = 0; j < outerCount; j++) {
            struct OwnerActionSlot *slot;
            u16 *out;
            s32 k;

            slot = owner->action_slots;
            out = actions + count;

            for (k = 32; k > 0; k--) {
                if (slot->encoded_action != 0) {
                    u8 *ability = Ability_GetData(slot->encoded_action & ACTION_ID_MASK);

                    if (j == 0) {
                        if (ability[12] != 0 || (ability[1] & 0x40) != 0) {
                            *out = slot->encoded_action;
                            out++;
                            count++;
                        }
                    } else if (j == 1) {
                    } else if (j == 2) {
                    } else if (j == 3) {
                        if (ability[12] == 0 && (ability[1] & 0x40) == 0) {
                            *out = slot->encoded_action;
                            out++;
                            count++;
                        }
                    }
                }

                slot++;
            }
        }
    }

    return count;
}
