#include "types.h"
#include "item.h"
#include "owner_state.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32 value, s32 shift);
s32 Func_08002df0(s16 *allocation);
s16 *Func_08004970(s32 size);
s32 Func_08079338(s32 flag);
s32 Func_08079ad8(s32 index);
const u16 *Func_0807a0cc(u32 group, u32 index);

/*
 * Rebuild the owner's derived values in a 0x60-byte work record.  The routine
 * applies the active inventory-entry adjustments, learned-entry adjustments,
 * table multipliers, and signed state modifiers before clamping and writing
 * the values back to the owner state.  Field widths below follow the reference
 * byte, signed-byte, halfword, and signed-halfword loads and stores.
 */
s32 Func_08077428(s32 owner)
{
    u8 *values;
    u8 *var_r1_4;
    u8 *var_r2;
    u8 *var_r2_4;
    u8 *var_r2_5;
    s16 temp_r1;
    s16 temp_r1_2;
    s16 temp_r2_6;
    s16 temp_r2_7;
    u8 *var_r8_2;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r1_3;
    s32 temp_r1_6;
    s32 temp_r2;
    s32 temp_r2_2;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r3_4;
    s32 temp_sl;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r1_3;
    s32 var_r1_6;
    s32 var_r2_2;
    s32 var_r2_3;
    s32 var_r2_6;
    s32 var_r2_7;
    s32 var_r2_8;
    s32 var_r2_9;
    s32 var_r3_10;
    s32 var_r3_11;
    s32 var_r3_12;
    s32 var_r3_13;
    s32 var_r3_14;
    s32 var_r3_15;
    s32 var_r3_2;
    s32 var_r3_3;
    s32 var_r3_4;
    s32 var_r3_5;
    s32 var_r3_6;
    s32 var_r3_7;
    s32 var_r3_8;
    s32 var_r3_9;
    s32 var_r4_2;
    s32 var_r4_3;
    s32 var_r5;
    s32 var_r5_2;
    s32 var_r5_4;
    s32 var_r5_5;
    s32 var_r5_6;
    s32 var_r5_7;
    s32 var_r5_8;
    s32 var_r8;
    s32 var_r8_3;
    u8 *var_r1_2;
    s8 temp_r3_5;
    u32 temp_r1_8;
    u32 var_r4;
    u32 var_r5_3;
    u8 *var_r0;
    u8 temp_r0_4;
    u8 temp_r1_5;
    u8 temp_r1_7;
    u8 temp_r2_4;
    u8 temp_r3_6;
    u8 var_r3;
    u8 *state;
    u8 *temp_r0_5;
    u8 *temp_r0_6;
    u8 *temp_r0_7;
    u8 *temp_r1_4;
    u8 *temp_r2_3;
    u8 *temp_r2_5;
    u8 *var_r1;
    u8 *var_r1_5;

    values = (u8 *)Func_08004970(0x60);
    state = Func_08077394(owner);
    M2C_FIELD(values, s32 *, 0) = (s32) M2C_FIELD(state, s16 *, 0x10);
    M2C_FIELD(values, s32 *, 4) = (s32) M2C_FIELD(state, s16 *, 0x12);
    M2C_FIELD(values, s32 *, 8) = (s32) M2C_FIELD(state, u16 *, 0x18);
    M2C_FIELD(values, s32 *, 0xC) = (s32) M2C_FIELD(state, u16 *, 0x1A);
    M2C_FIELD(values, s32 *, 0x10) = (s32) M2C_FIELD(state, u16 *, 0x1C);
    M2C_FIELD(values, s32 *, 0x18) = (s32) M2C_FIELD(state, u8 *, 0x1E);
    M2C_FIELD(values, s32 *, 0x1C) = (s32) (0xF & M2C_FIELD(state, u8 *, 0x1F));
    M2C_FIELD(values, s32 *, 0x20) = (s32) M2C_FIELD(state, u8 *, 0x20);
    M2C_FIELD(values, s32 *, 0x24) = (s32) M2C_FIELD(state, u8 *, 0x21);
    var_r1 = state + 0x24;
    var_r2 = values + 0x28;
    var_r5 = 3;
    do {
        M2C_FIELD(var_r2, s32 *, 0) = (s32) M2C_FIELD(var_r1, s16 *, 0);
        var_r5 -= 1;
        M2C_FIELD(var_r2, s32 *, 4) = (s32) M2C_FIELD(var_r1, s16 *, 2);
        var_r1 += 4;
        var_r2 += 8;
    } while (var_r5 >= 0);
    temp_r3 = M2C_FIELD(state, s16 *, 0x14) * M2C_FIELD(state, s16 *, 0x34);
    var_r2_2 = temp_r3;
    if (temp_r3 < 0) {
        var_r2_2 = temp_r3 + 0x3FFF;
    }
    temp_r0_2 = var_r2_2 >> 0xE;
    temp_r1 = M2C_FIELD(state, s16 *, 0x38);
    temp_r2 = temp_r0_2 - temp_r1;
    if (temp_r2 >= 0) {
        if (temp_r2 <= 1) {
            goto block_8;
        }
        goto block_14;
    }
    if ((s32) (temp_r1 - temp_r0_2) <= 1) {
block_8:
        temp_r3_2 = M2C_FIELD(state, s16 *, 0x16) * M2C_FIELD(state, s16 *, 0x36);
        var_r2_3 = temp_r3_2;
        if (temp_r3_2 < 0) {
            var_r2_3 = temp_r3_2 + 0x3FFF;
        }
        temp_r0_3 = var_r2_3 >> 0xE;
        temp_r1_2 = M2C_FIELD(state, s16 *, 0x3A);
        temp_r2_2 = temp_r0_3 - temp_r1_2;
        if (temp_r2_2 >= 0) {
            if (temp_r2_2 <= 1) {

            } else {
                goto block_14;
            }
        } else if ((s32) (temp_r1_2 - temp_r0_3) > 1) {
            goto block_14;
        }
    } else {
block_14:
        M2C_FIELD(state, s16 *, 0x14) = 0x4000;
        M2C_FIELD(state, s16 *, 0x16) = 0x4000;
        M2C_FIELD(state, s16 *, 0x38) = (s16) (u16) M2C_FIELD(state, s16 *, 0x34);
        M2C_FIELD(state, s16 *, 0x3A) = (s16) (u16) M2C_FIELD(state, s16 *, 0x36);
    }
    temp_r0_4 = -4 & M2C_FIELD(state, u8 *, 0x130);
    M2C_FIELD(state, u8 *, 0x130) = temp_r0_4;
    if (4 & temp_r0_4) {
        M2C_FIELD(state, u8 *, 0x130) = (u8) (temp_r0_4 | 1);
    }
    if (M2C_FIELD(state, u8 *, 0x144) != 0) {
        M2C_FIELD(values, s32 *, 0x1C) = (s32) (M2C_FIELD(values, s32 *, 0x1C) + 1);
    }
    M2C_FIELD(state, u8 *, 0x142) = 0U;
    M2C_FIELD(state, u8 *, 0x143) = 0U;
    if (M2C_FIELD(state, u8 *, 0x129) == 0) {

    } else {
        var_r5_2 = 0;
loop_22:
        temp_r1_3 = (var_r5_2 * 2) + 0xD8;
        if (!(0x200 & *(u16 *)(state + temp_r1_3))) {

        } else {
            temp_r0_5 = (u8 *)Func_08078414(*(u16 *)(state + temp_r1_3));
            M2C_FIELD(values, void **, 0x58) = temp_r0_5;
            if (1 & M2C_FIELD(temp_r0_5, u8 *, 3)) {
                M2C_FIELD(state, u8 *, 0x130) = (u8) (3 | M2C_FIELD(state, u8 *, 0x130));
            }
            temp_r1_4 = M2C_FIELD(values, void **, 0x58);
            M2C_FIELD(values, s32 *, 8) = (s32) (M2C_FIELD(values, s32 *, 8) + M2C_FIELD(temp_r1_4, s16 *, 8));
            M2C_FIELD(values, s32 *, 0xC) = (s32) (M2C_FIELD(values, s32 *, 0xC) + M2C_FIELD(temp_r1_4, s8 *, 0xA));
            var_r8 = 0;
loop_27:
            temp_r2_3 = M2C_FIELD(values, void **, 0x58);
            temp_r3_3 = (var_r8 * 4) + 0x18;
            temp_r1_5 = *(temp_r2_3 + temp_r3_3);
            M2C_FIELD(values, s32 *, 0x48) = (s32) temp_r1_5;
            M2C_FIELD(values, s32 *, 0x54) = (s32) M2C_FIELD((temp_r2_3 + temp_r3_3), s8 *, 1);
            switch ((u32) temp_r1_5) {
            case 1:
                M2C_FIELD(values, s32 *, 0) = (s32) (M2C_FIELD(values, s32 *, 0) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 2:
                M2C_FIELD(values, s32 *, 0x20) = (s32) (M2C_FIELD(values, s32 *, 0x20) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 3:
                M2C_FIELD(values, s32 *, 4) = (s32) (M2C_FIELD(values, s32 *, 4) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 4:
                M2C_FIELD(values, s32 *, 0x24) = (s32) (M2C_FIELD(values, s32 *, 0x24) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 5:
                M2C_FIELD(values, s32 *, 0x10) = (s32) (M2C_FIELD(values, s32 *, 0x10) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 6:
                M2C_FIELD(values, s32 *, 0x18) = (s32) (M2C_FIELD(values, s32 *, 0x18) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 15:
                M2C_FIELD(values, s32 *, 0x28) = (s32) (M2C_FIELD(values, s32 *, 0x28) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 16:
                M2C_FIELD(values, s32 *, 0x30) = (s32) (M2C_FIELD(values, s32 *, 0x30) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 17:
                M2C_FIELD(values, s32 *, 0x38) = (s32) (M2C_FIELD(values, s32 *, 0x38) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 18:
                M2C_FIELD(values, s32 *, 0x40) = (s32) (M2C_FIELD(values, s32 *, 0x40) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 19:
                M2C_FIELD(values, s32 *, 0x2C) = (s32) (M2C_FIELD(values, s32 *, 0x2C) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 20:
                M2C_FIELD(values, s32 *, 0x34) = (s32) (M2C_FIELD(values, s32 *, 0x34) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 21:
                M2C_FIELD(values, s32 *, 0x3C) = (s32) (M2C_FIELD(values, s32 *, 0x3C) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 22:
                M2C_FIELD(values, s32 *, 0x44) = (s32) (M2C_FIELD(values, s32 *, 0x44) + M2C_FIELD(values, s32 *, 0x54));
                break;
            case 23:
                var_r1_2 = state + 0x142;
                var_r3 = M2C_FIELD(state, u8 *, 0x142);
block_46:
                *var_r1_2 = var_r3 + M2C_FIELD(values, s32 *, 0x54);
                break;
            case 24:
                var_r1_2 = state + 0x143;
                var_r3 = M2C_FIELD(state, u8 *, 0x143);
                goto block_46;
            case 25:
                M2C_FIELD(state, u8 *, 0x130) = (u8) (8 | M2C_FIELD(state, u8 *, 0x130));
                break;
            case 26:
                M2C_FIELD(values, s32 *, 0x1C) = (s32) (M2C_FIELD(values, s32 *, 0x1C) + M2C_FIELD(values, s32 *, 0x54));
                break;
            }
            var_r8 += 1;
            if (var_r8 <= 3) {
                goto loop_27;
            }
        }
        var_r5_2 += 1;
        if (var_r5_2 <= 0xE) {
            goto loop_22;
        }
        temp_r2_4 = M2C_FIELD(state, u8 *, 0x130);
        if (8 & temp_r2_4) {
            M2C_FIELD(state, u8 *, 0x130) = (u8) (-0xA & temp_r2_4);
        }
        var_r4 = 0;
        var_r8_2 = state + 0x108;
        do {
            var_r5_3 = 0;
            temp_sl = *(s32 *)var_r8_2;
loop_57:
            if ((1 << var_r5_3) & temp_sl) {
                temp_r0_6 = (u8 *)Func_0807a0cc(var_r4, var_r5_3);
                M2C_FIELD(values, s32 *, 0) = (s32) (M2C_FIELD(values, s32 *, 0) + M2C_FIELD(temp_r0_6, s8 *, 4));
                M2C_FIELD(values, s32 *, 4) = (s32) (M2C_FIELD(values, s32 *, 4) + M2C_FIELD(temp_r0_6, s8 *, 5));
                M2C_FIELD(values, s32 *, 8) = (s32) (M2C_FIELD(values, s32 *, 8) + M2C_FIELD(temp_r0_6, s8 *, 6));
                M2C_FIELD(values, s32 *, 0xC) = (s32) (M2C_FIELD(values, s32 *, 0xC) + M2C_FIELD(temp_r0_6, s8 *, 7));
                M2C_FIELD(values, s32 *, 0x10) = (s32) (M2C_FIELD(values, s32 *, 0x10) + M2C_FIELD(temp_r0_6, s8 *, 8));
                M2C_FIELD(values, s32 *, 0x18) = (s32) (M2C_FIELD(values, s32 *, 0x18) + M2C_FIELD(temp_r0_6, s8 *, 9));
            }
            var_r5_3 += 1;
            if ((s32) var_r5_3 <= 0x13) {
                goto loop_57;
            }
            var_r4 += 1;
            var_r8_2 += 4;
        } while ((s32) var_r4 <= 3);
        temp_r0_7 = (u8 *)(unsigned long)Func_08079ad8(M2C_FIELD(state, u8 *, 0x129));
        M2C_FIELD(values, s32 *, 0) = Func_080022ec(M2C_FIELD(temp_r0_7, u8 *, 8) * M2C_FIELD(values, s32 *, 0), 0xA);
        M2C_FIELD(values, s32 *, 4) = Func_080022ec(M2C_FIELD(temp_r0_7, u8 *, 9) * M2C_FIELD(values, s32 *, 4), 0xA);
        M2C_FIELD(values, s32 *, 8) = Func_080022ec(M2C_FIELD(temp_r0_7, u8 *, 0xA) * M2C_FIELD(values, s32 *, 8), 0xA);
        M2C_FIELD(values, s32 *, 0xC) = Func_080022ec(M2C_FIELD(temp_r0_7, u8 *, 0xB) * M2C_FIELD(values, s32 *, 0xC), 0xA);
        M2C_FIELD(values, s32 *, 0x10) = Func_080022ec(M2C_FIELD(temp_r0_7, u8 *, 0xC) * M2C_FIELD(values, s32 *, 0x10), 0xA);
        M2C_FIELD(values, s32 *, 0x18) = Func_080022ec(M2C_FIELD(temp_r0_7, u8 *, 0xD) * M2C_FIELD(values, s32 *, 0x18), 0xA);
        var_r5_4 = 0;
loop_62:
        temp_r1_6 = (var_r5_4 * 2) + 0xD8;
        if (0x200 & *(u16 *)(state + temp_r1_6)) {
            M2C_FIELD(values, void **, 0x58) = Func_08078414(*(u16 *)(state + temp_r1_6));
            var_r8_3 = 0;
            do {
                temp_r2_5 = M2C_FIELD(values, void **, 0x58);
                temp_r3_4 = (var_r8_3 * 4) + 0x18;
                temp_r1_7 = *(temp_r2_5 + temp_r3_4);
                M2C_FIELD(values, s32 *, 0x48) = (s32) temp_r1_7;
                temp_r1_8 = temp_r1_7 - 7;
                M2C_FIELD(values, s32 *, 0x54) = (s32) M2C_FIELD((temp_r2_5 + temp_r3_4), s8 *, 1);
                switch (temp_r1_8) {
                case 0:
                    M2C_FIELD(values, s32 *, 0) = Func_080022ec(M2C_FIELD(values, s32 *, 0x54) * M2C_FIELD(values, s32 *, 0), 0xA);
                    break;
                case 1:
                    M2C_FIELD(values, s32 *, 0x20) = Func_080022ec(M2C_FIELD(values, s32 *, 0x54) * M2C_FIELD(values, s32 *, 0x20), 0xA);
                    break;
                case 2:
                    M2C_FIELD(values, s32 *, 4) = Func_080022ec(M2C_FIELD(values, s32 *, 0x54) * M2C_FIELD(values, s32 *, 4), 0xA);
                    break;
                case 3:
                    M2C_FIELD(values, s32 *, 0x24) = Func_080022ec(M2C_FIELD(values, s32 *, 0x54) * M2C_FIELD(values, s32 *, 0x24), 0xA);
                    break;
                case 4:
                    M2C_FIELD(values, s32 *, 8) = Func_080022ec(M2C_FIELD(values, s32 *, 0x54) * M2C_FIELD(values, s32 *, 8), 0xA);
                    break;
                case 5:
                    M2C_FIELD(values, s32 *, 0xC) = Func_080022ec(M2C_FIELD(values, s32 *, 0x54) * M2C_FIELD(values, s32 *, 0xC), 0xA);
                    break;
                case 6:
                    M2C_FIELD(values, s32 *, 0x10) = Func_080022ec(M2C_FIELD(values, s32 *, 0x54) * M2C_FIELD(values, s32 *, 0x10), 0xA);
                    break;
                case 7:
                    M2C_FIELD(values, s32 *, 0x18) = Func_080022ec(M2C_FIELD(values, s32 *, 0x54) * M2C_FIELD(values, s32 *, 0x18), 0xA);
                    break;
                }
                var_r8_3 += 1;
            } while (var_r8_3 <= 3);
        }
        var_r5_4 += 1;
        if (var_r5_4 <= 0xE) {
            goto loop_62;
        }
    }
    var_r3_2 = ((s8) M2C_FIELD(state, u8 *, 0x133) + 8) * M2C_FIELD(values, s32 *, 8);
    if (var_r3_2 < 0) {
        var_r3_2 += 7;
    }
    M2C_FIELD(values, s32 *, 8) = (s32) (var_r3_2 >> 3);
    var_r3_3 = ((s8) M2C_FIELD(state, u8 *, 0x135) + 8) * M2C_FIELD(values, s32 *, 0xC);
    if (var_r3_3 < 0) {
        var_r3_3 += 7;
    }
    M2C_FIELD(values, s32 *, 0xC) = (s32) (var_r3_3 >> 3);
    var_r3_4 = ((s8) M2C_FIELD(state, u8 *, 0x147) + 8) * M2C_FIELD(values, s32 *, 0x10);
    if (var_r3_4 < 0) {
        var_r3_4 += 7;
    }
    M2C_FIELD(values, s32 *, 0x10) = (s32) (var_r3_4 >> 3);
    var_r4_2 = 0x28;
    var_r5_5 = 3;
    var_r0 = state + 0x12C;
    do {
        temp_r3_5 = (s8) *var_r0;
        var_r5_5 -= 1;
        *(s32 *)(values + var_r4_2) += ((temp_r3_5 * temp_r3_5) + temp_r3_5) * 5;
        var_r0 += 1;
        var_r4_2 += 8;
    } while (var_r5_5 >= 0);
    var_r5_6 = 3;
    var_r1_3 = 0x2C;
    do {
        var_r5_6 -= 1;
        *(s32 *)(values + var_r1_3) += M2C_FIELD(state, s8 *, 0x137) * 0x14;
        var_r1_3 += 8;
    } while (var_r5_6 >= 0);
    if (M2C_FIELD(state, u8 *, 0x129) != 0) {
        temp_r3_6 = M2C_FIELD(state, u8 *, 0x128);
        var_r0_2 = 0;
        switch (temp_r3_6) {
        case 0:
            var_r0_3 = 0x88;
block_92:
            var_r0_2 = Func_08079338(var_r0_3 * 2);
            break;
        case 1:
            var_r0_3 = 0x89;
            goto block_92;
        case 2:
            var_r0_2 = Func_08079338(0x113);
            break;
        case 3:
            var_r0_2 = Func_08079338(0x111);
            break;
        case 5:
            var_r0_2 = Func_08079338(0x112);
            break;
        }
        if (var_r0_2 != 0) {
            M2C_FIELD(values, s32 *, 0x24) = (s32) (M2C_FIELD(values, s32 *, 0x24) + 4);
        }
    }
    var_r3_5 = M2C_FIELD(values, s32 *, 8);
    if (var_r3_5 < 0) {
        var_r3_5 = 0;
        M2C_FIELD(values, s32 *, 8) = 0;
    }
    if (var_r3_5 > 0x3E7) {
        M2C_FIELD(values, s32 *, 8) = 0x3E7;
    }
    var_r3_6 = M2C_FIELD(values, s32 *, 0xC);
    if (var_r3_6 < 0) {
        var_r3_6 = 0;
        M2C_FIELD(values, s32 *, 0xC) = 0;
    }
    if (var_r3_6 > 0x3E7) {
        M2C_FIELD(values, s32 *, 0xC) = 0x3E7;
    }
    var_r3_7 = M2C_FIELD(values, s32 *, 0x10);
    if (var_r3_7 < 0) {
        var_r3_7 = 0;
        M2C_FIELD(values, s32 *, 0x10) = 0;
    }
    if (var_r3_7 > 0x3E7) {
        M2C_FIELD(values, s32 *, 0x10) = 0x3E7;
    }
    var_r3_8 = M2C_FIELD(values, s32 *, 0x18);
    if (var_r3_8 < 0) {
        var_r3_8 = 0;
        M2C_FIELD(values, s32 *, 0x18) = 0;
    }
    if (var_r3_8 > 0x63) {
        M2C_FIELD(values, s32 *, 0x18) = 0x63;
    }
    var_r3_9 = M2C_FIELD(values, s32 *, 0x1C);
    if (var_r3_9 < 0) {
        var_r3_9 = 0;
        M2C_FIELD(values, s32 *, 0x1C) = 0;
    }
    if (var_r3_9 > 2) {
        M2C_FIELD(values, s32 *, 0x1C) = 2;
    }
    var_r3_10 = M2C_FIELD(values, s32 *, 0x20);
    if (var_r3_10 < 0) {
        var_r3_10 = 0;
        M2C_FIELD(values, s32 *, 0x20) = 0;
    }
    if (var_r3_10 > 0x2710) {
        M2C_FIELD(values, s32 *, 0x20) = 0x2710;
    }
    var_r3_11 = M2C_FIELD(values, s32 *, 0x24);
    if (var_r3_11 < 0) {
        var_r3_11 = 0;
        M2C_FIELD(values, s32 *, 0x24) = 0;
    }
    if (var_r3_11 > 0xC8) {
        M2C_FIELD(values, s32 *, 0x24) = 0xC8;
    }
    var_r5_7 = 0;
    var_r4_3 = 0x2C;
    var_r2_4 = values + 0x28;
    var_r1_4 = values + 0x2C;
    do {
        var_r3_12 = *(s32 *)var_r2_4;
        if (var_r3_12 < 0) {
            *(s32 *)var_r2_4 = 0;
            var_r3_12 = 0;
        }
        if (var_r3_12 > 0xC8) {
            *(s32 *)var_r2_4 = 0xC8;
        }
        var_r3_13 = *(s32 *)var_r1_4;
        if (var_r3_13 < 0) {
            *(s32 *)var_r1_4 = 0;
            var_r3_13 = 0;
        }
        if (var_r3_13 > 0xC8) {
            *(s32 *)(values + var_r4_3) = 0xC8;
        }
        var_r5_7 += 1;
        var_r1_4 += 8;
        var_r4_3 += 8;
        var_r2_4 += 8;
    } while (var_r5_7 <= 3);
    M2C_FIELD(state, s16 *, 0x3C) = (s16) M2C_FIELD(values, s32 *, 8);
    M2C_FIELD(state, s16 *, 0x3E) = (s16) M2C_FIELD(values, s32 *, 0xC);
    M2C_FIELD(state, s16 *, 0x40) = (s16) M2C_FIELD(values, s32 *, 0x10);
    M2C_FIELD(state, s8 *, 0x42) = (s8) M2C_FIELD(values, s32 *, 0x18);
    M2C_FIELD((state + 0x42), s8 *, 1) = (s8) M2C_FIELD(values, s32 *, 0x1C);
    M2C_FIELD(state, s8 *, 0x44) = (s8) M2C_FIELD(values, s32 *, 0x20);
    M2C_FIELD(state, s8 *, 0x45) = (s8) M2C_FIELD(values, s32 *, 0x24);
    var_r1_5 = state + 0x48;
    var_r2_5 = values + 0x28;
    var_r5_8 = 3;
    do {
        M2C_FIELD(var_r1_5, s16 *, 0) = (s16) M2C_FIELD(var_r2_5, s32 *, 0);
        var_r5_8 -= 1;
        M2C_FIELD(var_r1_5, s16 *, 2) = (s16) M2C_FIELD(var_r2_5, s32 *, 4);
        var_r2_5 += 8;
        var_r1_5 += 4;
    } while (var_r5_8 >= 0);
    var_r1_6 = 0x270F;
    if (M2C_FIELD(state, u8 *, 0x129) != 0) {
        var_r1_6 = 0x7CF;
    }
    temp_r2_6 = M2C_FIELD(state, s16 *, 0x34);
    var_r3_14 = M2C_FIELD(values, s32 *, 0);
    if (var_r3_14 < 0) {
        var_r3_14 = 0;
        M2C_FIELD(values, s32 *, 0) = 0;
    }
    if (var_r3_14 > var_r1_6) {
        M2C_FIELD(values, s32 *, 0) = var_r1_6;
        var_r3_14 = var_r1_6;
    }
    M2C_FIELD(state, s16 *, 0x34) = (s16) var_r3_14;
    if (temp_r2_6 != (s16) var_r3_14) {
        var_r2_6 = M2C_FIELD(state, s16 *, 0x14) * M2C_FIELD(values, s32 *, 0);
        if (var_r2_6 < 0) {
            var_r2_6 += 0x3FFF;
        }
        var_r2_7 = var_r2_6 >> 0xE;
        if (var_r2_7 < 0) {
            var_r2_7 = 0;
        }
        if (var_r2_7 > var_r1_6) {
            var_r2_7 = var_r1_6;
        }
        if ((M2C_FIELD(state, s16 *, 0x38) != 0) && (var_r2_7 == 0)) {
            var_r2_7 = 1;
        }
        M2C_FIELD(state, s16 *, 0x38) = (s16) var_r2_7;
    }
    temp_r2_7 = M2C_FIELD(state, s16 *, 0x36);
    var_r3_15 = M2C_FIELD(values, s32 *, 4);
    if (var_r3_15 < 0) {
        var_r3_15 = 0;
        M2C_FIELD(values, s32 *, 4) = 0;
    }
    if (var_r3_15 > var_r1_6) {
        M2C_FIELD(values, s32 *, 4) = var_r1_6;
        var_r3_15 = var_r1_6;
    }
    M2C_FIELD(state, s16 *, 0x36) = (s16) var_r3_15;
    if (temp_r2_7 != (s16) var_r3_15) {
        var_r2_8 = M2C_FIELD(state, s16 *, 0x16) * M2C_FIELD(values, s32 *, 4);
        if (var_r2_8 < 0) {
            var_r2_8 += 0x3FFF;
        }
        var_r2_9 = var_r2_8 >> 0xE;
        if (var_r2_9 < 0) {
            var_r2_9 = 0;
        }
        if (var_r2_9 > var_r1_6) {
            var_r2_9 = var_r1_6;
        }
        if ((M2C_FIELD(state, s16 *, 0x3A) != 0) && (var_r2_9 == 0)) {
            var_r2_9 = 1;
        }
        M2C_FIELD(state, s16 *, 0x3A) = (s16) var_r2_9;
    }
    return Func_08002df0((s16 *)values);
}
