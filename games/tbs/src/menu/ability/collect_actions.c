#include "types.h"

#include "owner_state.h"

#define ACTION_ID_MASK 0x3FFF

s32 PsynergyMenu_CollectActions(struct BtlUnit *owner, u16 *actions, s32 mode)
{
    /* FAKEMATCH: the scalar cursor, offset clears and empty branches preserve operand order and 274 bytes; indexed clearing yields 266. */
    s32 n;
    u16 *out;
    s32 passes;
    s32 count;
    s32 i;
    s32 j;
    s32 off;

    passes = (mode != 2) ? 4 : 3;
    for (n = 62; n >= 0; n -= 2) {
        u16 *q = (u16 *)((u8 *)actions + n);
        *q = 0;
        *q = 0;
    }
    count = 0;

    if (mode == 1) {
        for (i = 0, off = (u8 *)owner->action_slots - (u8 *)owner, out = actions; i <= 31; i++, off += 4) {
            if (*(u16 *)(off + (s32)owner) != 0) {
                if (BtlActGet(*(u16 *)(off + (s32)owner) & ACTION_ID_MASK)->type_0c != 0) {
                    *out = *(u16 *)((s32)owner + off);
                    out++;
                    count++;
                }
            }
        }
    } else {
        for (j = 0; j < passes; j++) {
            out = (u16 *)(count * sizeof(actions[0]) + (s32)actions);

            for (i = 0; i < 32; i++) {
                if (owner->action_slots[i].encoded_action != 0) {
                    struct BtlAct *ability = BtlActGet(owner->action_slots[i].encoded_action & ACTION_ID_MASK);

                    if (j == 0 && (ability->type_0c != 0 || (ability->tgt_flg & 0x40) != 0)) {
                        *out = owner->action_slots[i].encoded_action;
                        out++;
                        count++;
                    } else if (j == 1) {
                    } else if (j == 2) {
                    } else if (j == 3 && ability->type_0c == 0 && (ability->tgt_flg & 0x40) == 0) {
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
