#include "types.h"

#include "owner_state.h"

#define ACTION_ID_MASK 0x3FFF

s32 PsynergyMenu_CollectActions(struct BtlUnit *owner, u16 *actions, s32 mode)
{
    /* FAKEMATCH: the doubled zero store through the byte-offset cursor makes loop.c replace the counter with the pointer (274 bytes); a single indexed clear keeps the counter (266). */
    s32 n;
    s32 passes;
    s32 count;
    s32 i;
    s32 j;

    passes = (mode != 2) ? 4 : 3;
    for (n = 62; n >= 0; n -= 2) {
        u16 *q = (u16 *)((u8 *)actions + n);
        *q = 0;
        *q = 0;
    }
    count = 0;

    if (mode == 1) {
        for (i = 0; i < 32; i++) {
            if (owner->action_slots[i].encoded_action != 0) {
                if (BtlActGet(owner->action_slots[i].encoded_action & ACTION_ID_MASK)->type_0c != 0) {
                    actions[count] = owner->action_slots[i].encoded_action;
                    count++;
                }
            }
        }
    } else {
        for (j = 0; j < passes; j++) {
            for (i = 0; i < 32; i++) {
                if (owner->action_slots[i].encoded_action != 0) {
                    struct BtlAct *ability = BtlActGet(owner->action_slots[i].encoded_action & ACTION_ID_MASK);

                    if (j == 0 && (ability->type_0c != 0 || (ability->tgt_flg & 0x40) != 0)) {
                        actions[count] = owner->action_slots[i].encoded_action;
                        count++;
                    } else if (j == 1) {
                    } else if (j == 2) {
                    } else if (j == 3 && ability->type_0c == 0 && (ability->tgt_flg & 0x40) == 0) {
                        actions[count] = owner->action_slots[i].encoded_action;
                        count++;
                    }
                }
            }
        }
    }

    return count;
}
