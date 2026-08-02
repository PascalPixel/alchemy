#include "types.h"
#include "item.h"
#include "owner_state.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_08079ad8(s32 index);

/*
 * Refresh the owner's 32 sparse effect slots.  Stale flagged entries are
 * removed, the survivors are compacted, eligible table and inventory effects
 * are inserted once, and the list is compacted again.  Each slot is four
 * bytes wide; only its leading halfword is owned by this routine.
 */
s32 Func_08078bf0(s32 owner)
{
    u8 *effectTable;
    u16 *var_r2_4;
    u16 *var_r3;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r4_3;
    s32 var_r4_4;
    s32 var_r4_5;
    s32 var_r4_6;
    s32 var_r5;
    s32 var_r5_2;
    s32 var_r5_3;
    s32 var_r5_4;
    s32 var_r5_5;
    s32 var_r5_6;
    s32 var_r5_7;
    s32 var_sl;
    s32 var_sl_2;
    u16 *slots;
    u16 *temp_r2_3;
    u16 *var_r1;
    u16 *var_r1_2;
    u16 *var_r1_3;
    u16 *var_r1_4;
    u16 *var_r2;
    u16 *var_r2_2;
    u16 *var_r2_3;
    u16 temp_r0_4;
    u16 temp_r2;
    u16 temp_r2_2;
    u16 temp_r2_4;
    u16 temp_r6;
    u16 temp_r6_2;
    u16 var_r3_2;
    u8 *var_lr;
    u8 *var_r7;
    u8 *state;
    u8 *itemDefinition;

    state = Func_08077394(owner);
    slots = (u16 *)(state + 0x58);
    effectTable = (u8 *)(unsigned long)Func_08079ad8(M2C_FIELD(state, u8 *, 0x129));
    var_r2 = slots;
    var_r5 = 0x1F;
    if (M2C_FIELD(state, u8 *, 0x129) == 0) {
        return 0;
    }
    do {
        if (*var_r2 & 0x8000) {
            *var_r2 = 0;
        }
        var_r5 -= 1;
        var_r2 += 2;
    } while (var_r5 >= 0);
    var_r2_2 = slots;
    var_r5_2 = 0x1F;
    do {
        if (*var_r2_2 & 0x4000) {
            *var_r2_2 = 0;
        }
        var_r5_2 -= 1;
        var_r2_2 += 2;
    } while (var_r5_2 >= 0);
    var_r4 = 0x1F;
    var_r5_3 = 0x1F;
    var_r1 = (u16 *)((u8 *)slots + 0x7c);
    do {
        temp_r2 = *((var_r4 * 2) + slots);
        if (temp_r2 == 0) {
            var_r4 -= 1;
        } else {
            *var_r1 = temp_r2;
            var_r4 -= 1;
            var_r1 -= 2;
            var_r5_3 -= 1;
        }
    } while (var_r4 >= 0);
    if (var_r5_3 >= 0) {
        var_r3 = (var_r5_3 * 2) + slots;
        do {
            var_r5_3 -= 1;
            *var_r3 = 0;
            var_r3 -= 2;
        } while (var_r5_3 >= 0);
    }
    var_lr = effectTable + 0x10;
    var_r5_4 = 0;
    var_sl = 0x10;
    var_r7 = var_lr;
loop_19:
    if ((*var_r7 != 0) && ((u32) M2C_FIELD(state, u8 *, 0xF) >= (u32) M2C_FIELD(var_lr, u8 *, 1))) {
        temp_r6 = M2C_FIELD(state, u16 *, 0x58);
        var_r4_2 = 0;
        if (temp_r6 != M2C_FIELD(var_lr, u8 *, 0)) {
            var_r1_2 = slots;
loop_23:
            var_r4_2 += 1;
            if (var_r4_2 <= 0x1F) {
                var_r1_2 += 2;
                if (*var_r1_2 != effectTable[var_sl]) {
                    goto loop_23;
                }
            }
        }
        if (var_r4_2 == 0x20) {
            var_r4_3 = 0;
            if (temp_r6 == 0) {
                var_r3_2 = *var_r7 | 0x8000;
                var_r2_3 = slots;
                goto block_31;
            }
loop_28:
            var_r4_3 += 1;
            if (var_r4_3 <= 0x1F) {
                var_r2_3 = (var_r4_3 * 2) + slots;
                if (*var_r2_3 == 0) {
                    var_r3_2 = *var_r7 | 0x8000;
block_31:
                    *var_r2_3 = var_r3_2;
                } else {
                    goto loop_28;
                }
            }
            if (var_r4_3 != 0x20) {
                goto block_33;
            }
        } else {
            goto block_33;
        }
    } else {
block_33:
        var_r5_4 += 1;
        var_lr += 4;
        var_r7 += 4;
        var_sl += 4;
        if (var_r5_4 <= 0xF) {
            goto loop_19;
        }
    }
    var_r5_5 = 0;
    var_sl_2 = 0xD8;
loop_35:
    temp_r2_2 = *(u16 *)(state + var_sl_2);
    if ((temp_r2_2 != 0) && (0x200 & temp_r2_2) &&
        (itemDefinition = (u8 *)Func_08078414(*(u16 *)(state + var_sl_2)),
         M2C_FIELD(itemDefinition, u8 *, 0xC) == 3)) {
        temp_r6_2 = M2C_FIELD(state, u16 *, 0x58);
        temp_r0_4 = M2C_FIELD(itemDefinition, u16 *, 0x28);
        var_r4_4 = 0;
        if ((0x3FFF & temp_r6_2) != temp_r0_4) {
            var_r1_3 = slots;
loop_41:
            var_r4_4 += 1;
            if (var_r4_4 <= 0x1F) {
                var_r1_3 += 2;
                if ((0x3FFF & *var_r1_3) != temp_r0_4) {
                    goto loop_41;
                }
            }
        }
        if (var_r4_4 == 0x20) {
            var_r4_5 = 0;
            if (temp_r6_2 == 0) {
                M2C_FIELD(state, u16 *, 0x58) = (u16) (0x4000 | temp_r0_4);
            } else {
loop_46:
                var_r4_5 += 1;
                if (var_r4_5 <= 0x1F) {
                    temp_r2_3 = (var_r4_5 * 2) + slots;
                    if (*temp_r2_3 == 0) {
                        *temp_r2_3 = 0x4000 | temp_r0_4;
                    } else {
                        goto loop_46;
                    }
                }
            }
            if (var_r4_5 != 0x20) {
                goto block_50;
            }
        } else {
            goto block_50;
        }
    } else {
block_50:
        var_r5_5 += 1;
        var_sl_2 += 2;
        if (var_r5_5 <= 0xE) {
            goto loop_35;
        }
    }
    var_r4_6 = 0;
    var_r5_6 = 0;
    var_r1_4 = slots;
    do {
        temp_r2_4 = *((var_r4_6 * 2) + slots);
        if (temp_r2_4 == 0) {
            var_r4_6 += 1;
        } else {
            *var_r1_4 = temp_r2_4;
            var_r4_6 += 1;
            var_r1_4 += 2;
            var_r5_6 += 1;
        }
    } while (var_r4_6 <= 0x1F);
    if (var_r5_6 <= 0x1F) {
        var_r2_4 = (var_r5_6 * 2) + slots;
        var_r5_7 = 0x20 - var_r5_6;
        do {
            var_r5_7 -= 1;
            *var_r2_4 = 0;
            var_r2_4 += 2;
        } while (var_r5_7 != 0);
    }
    return 0;
}
