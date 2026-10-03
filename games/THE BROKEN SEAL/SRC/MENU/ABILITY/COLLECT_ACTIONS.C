#include "TYPES.H"

#include "OWNER_STATE.H"

#define ACTION_ID_MASK 0x3FFF

/* PsynergyMenu_CollectActions per games/THE BROKEN SEAL/INCLUDE/PSYNERGY_MENU.H. */
s32 PsynergyMenu_CollectActions(struct BattleUnit *owner, u16 *actions, s32 mode)
{
    /* FAKEMATCH: retain the existing scalar action-slot cursor in mode 1; an indexed slot loop keeps the 274-byte extent but changes native operand order. */
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
        for (i = 0, off = (u8 *)owner->action_slots - (u8 *)owner, out = actions; i <= 31; i++, off += 4) {
            if (*(u16 *)(off + (s32)owner) != 0) {
                if (BattleAction_Get(*(u16 *)(off + (s32)owner) & ACTION_ID_MASK)->type_0c != 0) {
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
                    struct BattleAction *ability = BattleAction_Get(owner->action_slots[i].encoded_action & ACTION_ID_MASK);

                    if (j == 0 && (ability->type_0c != 0 || (ability->target_flags & 0x40) != 0)) {
                        *out = owner->action_slots[i].encoded_action;
                        out++;
                        count++;
                    } else if (j == 1) {
                    } else if (j == 2) {
                    } else if (j == 3 && ability->type_0c == 0 && (ability->target_flags & 0x40) == 0) {
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
