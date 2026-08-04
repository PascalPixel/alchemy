#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080022fc(s32, s32);
s32 Func_08002322(s32);
u32 Func_08004458(void);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_080150b0(s32, s32, s32, s32, s32);
void Func_08077048(s32);
s32 Func_080772e0(s32);
void Func_080f9010(s32);

/*
 * Update the menu state machine and rebuild its complete 128-entry OAM list.
 *
 * Input handling advances the active view, creates or closes message windows,
 * and updates the five animated records in the shared state.  The common
 * renderer emits every remaining hidden OAM entry before DMA-copying the list
 * to hardware OAM.
 *
 * Owner span: 0x080f6440-0x080f731c.  Long branches within that span are
 * continuations of this owner, not separately callable C functions.
 */
void Func_080f6440(void) {
    s32 *spC;
    s32 sp10;
    s32 *sp14;
    s32 *sp18;
    s32 sp1C;
    s32 sp20;
    u8 *sp24;
    s32 *temp_r2;
    s32 *temp_r4_2;
    u8 *temp_r7;
    s32 *var_fp;
    s32 *var_r1;
    u8 *var_r1_3;
    s32 *var_r2_5;
    s32 var_r4;
    s32 var_r4_6;
    u8 *var_r5_11;
    s32 *var_r6_3;
    s32 *var_r8;
    s32 *var_r8_2;
    s32 *var_r8_3;
    s32 temp_r0;
    s32 temp_r0_11;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r0_5;
    s32 temp_r0_6;
    s32 temp_r0_7;
    s32 temp_r0_8;
    s32 temp_r0_9;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r1_4;
    s32 temp_r1_5;
    s32 temp_r1_6;
    s32 temp_r2_10;
    s32 temp_r2_11;
    s32 temp_r2_2;
    s32 temp_r2_3;
    s32 temp_r2_4;
    s32 temp_r2_5;
    s32 temp_r2_6;
    s32 temp_r2_8;
    s32 temp_r2_9;
    s32 temp_r3_10;
    s32 temp_r3_12;
    s32 temp_r3_13;
    s32 temp_r3_14;
    s32 temp_r3_15;
    s32 temp_r3_17;
    s32 temp_r3_18;
    s32 temp_r3_19;
    s32 temp_r3_20;
    s32 temp_r3_21;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r3_5;
    s32 temp_r3_6;
    s32 temp_r3_8;
    s32 temp_r3_9;
    s32 temp_r4;
    s32 temp_r4_3;
    s32 temp_r4_4;
    s32 temp_r5;
    s32 var_fp_2;
    s32 var_ip;
    s32 var_ip_4;
    s32 var_lr_2;
    s32 var_r0;
    s32 var_r0_10;
    s32 var_r0_11;
    s32 var_r0_12;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r0_4;
    s32 var_r0_5;
    s32 var_r0_6;
    s32 var_r0_7;
    s32 var_r0_8;
    s32 var_r0_9;
    s32 var_r1_2;
    s32 var_r1_4;
    s32 var_r1_5;
    s32 var_r1_6;
    s32 var_r1_7;
    s32 var_r2_10;
    s32 var_r2_11;
    s32 var_r2_12;
    s32 var_r2_13;
    s32 var_r2_9;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r3_3;
    s32 var_r3_4;
    s32 var_r4_2;
    s32 var_r4_3;
    s32 var_r4_4;
    s32 var_r4_5;
    s32 var_r4_7;
    s32 var_r4_8;
    s32 var_r5;
    s32 var_r5_10;
    s32 var_r5_12;
    s32 var_r5_2;
    s32 var_r5_4;
    s32 var_r5_5;
    s32 var_r5_6;
    s32 var_r5_7;
    s32 var_r5_8;
    s32 var_r5_9;
    s32 var_r6;
    s32 var_r6_10;
    s32 var_r6_13;
    s32 var_r6_14;
    s32 var_r6_15;
    s32 var_r6_16;
    s32 var_r6_2;
    s32 var_r6_4;
    s32 var_r6_5;
    s32 var_r6_6;
    s32 var_r6_7;
    s32 var_r6_8;
    s32 var_r6_9;
    s32 var_r8_4;
    s32 var_r8_5;
    s32 var_r9;
    s32 var_r9_2;
    s32 var_r9_3;
    s32 var_r9_4;
    s32 var_sl;
    s32 var_sl_2;
    s32 var_sl_3;
    u16 temp_r1;
    u16 var_r2;
    u32 temp_r3_11;
    u8 *var_ip_2;
    u8 *var_ip_3;
    u8 *var_r2_2;
    u8 *var_r6_12;
    u8 temp_r2_7;
    u8 temp_r3;
    u8 temp_r3_16;
    u8 temp_r3_7;
    void *temp_r0_10;
    void *temp_r3_4;
    u8 *var_lr;
    u8 *var_r2_3;
    u8 *var_r2_4;
    u8 *var_r2_6;
    u8 *var_r2_7;
    u8 *var_r2_8;
    u8 *var_r5_3;
    s32 var_r6_11;

    temp_r7 = *(u8 **)0x03001F04;
    sp24 = *(u8 **)0x03001EEC;
    sp1C = 0x400;
    sp20 = 0;
    Func_08004458();
    M2C_FIELD((void *)0x040000B0, u16 *, 0xA) = (u16) (0xC5FF & M2C_FIELD((void *)0x040000B0, u16 *, 0xA));
    M2C_FIELD((void *)0x040000B0, u16 *, 0xA) = (u16) (0x7FFF & M2C_FIELD((void *)0x040000B0, u16 *, 0xA));
    M2C_FIELD((void *)0x040000B0, s32 *, 0) = (s32) (temp_r7 + 0x4D8);
    M2C_FIELD((void *)0x040000B0, s32 *, 4) = 0x04000054;
    M2C_FIELD((void *)0x040000B0, s32 *, 8) = 0xA2600001;
    temp_r4 = *(s32 *)0x03001AE8;
    M2C_FIELD(temp_r7, u16 *, 0xA0) = (u16) ((u16) temp_r4 & ~M2C_FIELD(temp_r7, u16 *, 0x9C));
    temp_r1 = *(s32 *)0x03001AE8 & 0xF0;
    M2C_FIELD(temp_r7, u16 *, 0x9E) = temp_r1;
    if ((0xF0 & M2C_FIELD(temp_r7, u16 *, 0x9C)) == temp_r1) {
        var_r2 = M2C_FIELD(temp_r7, u16 *, 0xA2);
        if ((u32) var_r2 > 0xCU) {
            M2C_FIELD(temp_r7, u16 *, 0xA2) = 0xCU;
            var_r2 = 0xC;
        }
        if (var_r2 == 0) {
            M2C_FIELD(temp_r7, u16 *, 0xA2) = 4U;
        } else {
            M2C_FIELD(temp_r7, u16 *, 0xA2) = (u16) (var_r2 + 0xFFFF);
            M2C_FIELD(temp_r7, u16 *, 0x9E) = (u16) sp20;
        }
    } else {
        M2C_FIELD(temp_r7, u16 *, 0xA2) = 0xCU;
    }
    M2C_FIELD(temp_r7, u16 *, 0x9C) = (u16) temp_r4;
    temp_r3 = *(u8 *)0x03001D20;
    if (temp_r3 != 0) {
        spC = (s32 *)(temp_r7 + 0x8C);
        sp18 = spC;
        var_fp = (s32 *)(temp_r7 + 0x98);
        var_r8_2 = (s32 *)(temp_r7 + 0x94);
        goto common_render;
    }
    temp_r2 = (s32 *)(temp_r7 + 0x8C);
    spC = temp_r2;
    sp18 = temp_r2;
    temp_r5 = M2C_FIELD(temp_r7, s32 *, 0x8C);
    switch (temp_r5) {
    case 0:
        temp_r0 = Func_080772e0(0xE4);
        var_fp = (s32 *)(temp_r7 + 0x98);
        Func_080150b0(temp_r0 - M2C_FIELD(temp_r7, s32 *, 0x98), 2, M2C_FIELD(temp_r7, s32 *, 0x4CC), 0x40, (s32) temp_r3);
        Func_080150b0(M2C_FIELD(temp_r7, s32 *, 0x98), 2, M2C_FIELD(temp_r7, s32 *, 0x4CC), 0x40, 8);
        var_r8 = var_fp;
        if (2 & M2C_FIELD(temp_r7, u16 *, 0xA0)) {
            *spC = 0xA;
            *(s8 *)0x0200036C = 0xFE;
            Func_08015018(M2C_FIELD(temp_r7, s32 *, 0x4C8), 1);
            var_r8 = (s32 *)(temp_r7 + 0x94);
            var_r8_2 = var_r8;
            goto common_render;
        }
        if (0x40 & M2C_FIELD(temp_r7, u16 *, 0xA0)) {
            temp_r3_2 = M2C_FIELD(temp_r7, s32 *, 0x98);
            if ((temp_r3_2 <= 3) && (temp_r0 > temp_r3_2)) {
                M2C_FIELD(temp_r7, s32 *, 0x98) = (s32) (temp_r3_2 + 1);
                Func_080f9010(0x6F);
            } else {
                Func_080f9010(0x71);
            }
        }
        if (0x80 & M2C_FIELD(temp_r7, u16 *, 0xA0)) {
            temp_r3_3 = *var_r8;
            if (temp_r3_3 > 1) {
                *var_r8 = temp_r3_3 - 1;
                Func_080f9010(0x6F);
            } else {
                Func_080f9010(0x71);
            }
        }
        M2C_FIELD((void *)0x04000050, s16 *, 0) = 0x3FD0;
        M2C_FIELD((void *)0x04000050, s16 *, 2) = 0x10;
        if (!(1 & M2C_FIELD(temp_r7, u16 *, 0xA0))) {

        } else {
            *sp18 = 1;
            M2C_FIELD(sp24, s32 *, 0x778C) = 0;
            Func_08015018(M2C_FIELD(temp_r7, s32 *, 0x4C8), 1);
            var_r5 = 0;
            if (*var_r8 != 0) {
                do {
                    Func_08077048(0xE4);
                    var_r5 += 1;
                } while (var_r5 != *var_r8);
            }
            Func_08015018(M2C_FIELD(temp_r7, s32 *, 0x4CC), 1);
            Func_080f9010(0x130);
        }
        var_r8_2 = (s32 *)(temp_r7 + 0x94);
        break;
    case 5:
        var_r9 = 0;
        M2C_FIELD(temp_r7, s32 *, 0xA8) = (s32) (M2C_FIELD(temp_r7, s32 *, 0xA8) + 1);
        var_r5_2 = 0;
        if (M2C_FIELD(temp_r7, u8 *, 0x19) != 0) {
            var_r2_2 = temp_r7 + 0x19;
loop_32:
            var_r5_2 += 1;
            if (var_r5_2 != 5) {
                var_r2_2 += 0x1C;
                if (*var_r2_2 == 0) {
                    goto block_34;
                }
                goto loop_32;
            }
            goto block_35;
        }
block_34:
        if (var_r5_2 == 5) {
block_35:
            var_r9 = 1;
        }
        if (1 & M2C_FIELD(temp_r7, u16 *, 0xA0)) {
            M2C_FIELD(temp_r7, s32 *, 0xA8) = 0;
            M2C_FIELD(sp24, s32 *, 0x778C) = 0;
            var_r8_2 = (s32 *)(temp_r7 + 0x94);
            if (M2C_FIELD(temp_r7, s32 *, 0x94) == 4) {
                M2C_FIELD(temp_r7, s32 *, 0x94) = 0;
                M2C_FIELD(temp_r7, s32 *, 0x90) = 0;
                *sp18 = 0;
                var_r6 = 0;
                var_r2_3 = temp_r7 + 0x18;
                do {
                    var_r6 += 1;
                    M2C_FIELD(var_r2_3, s8 *, 1) = 0;
                    M2C_FIELD(var_r2_3, u8 *, 2) = (u8) (M2C_FIELD(var_r2_3, u8 *, 2) | 0xFF);
                    var_r2_3 += 0x1C;
                } while (var_r6 != 5);
            } else if ((s32) M2C_FIELD(temp_r7, s32 *, 0x90) <= 4) {
                Func_080f9010(0x131);
                temp_r3_4 = temp_r7 + ((M2C_FIELD(temp_r7, s32 *, 0x90) * 0x1C) + 0x18);
                M2C_FIELD(temp_r3_4, u8 *, 1) = (u8) (M2C_FIELD(temp_r3_4, u8 *, 1) ^ 1);
            } else if (var_r9 == 0) {
                Func_080f9010(0x130);
                *sp18 = 1;
                var_r6_2 = 0;
                M2C_FIELD(temp_r7, s32 *, 0x90) = var_r9;
                var_r2_4 = temp_r7 + 0x18;
                do {
                    var_r6_2 += 1;
                    M2C_FIELD(var_r2_4, u8 *, 2) = (u8) (M2C_FIELD(var_r2_4, u8 *, 2) | 0xFF);
                    var_r2_4 += 0x1C;
                } while (var_r6_2 != 5);
                M2C_FIELD(temp_r7, s32 *, 0x94) = (s32) (M2C_FIELD(temp_r7, s32 *, 0x94) + 1);
            } else {
                Func_080f9010(0x71);
            }
        } else {
            if (0x10 & M2C_FIELD(temp_r7, u16 *, 0x9E)) {
                M2C_FIELD(temp_r7, s32 *, 0x90) = Func_080022fc(M2C_FIELD(temp_r7, s32 *, 0x90) + 1, 6);
                Func_080f9010(0x6F);
            }
            if (0x20 & M2C_FIELD(temp_r7, u16 *, 0x9E)) {
                M2C_FIELD(temp_r7, s32 *, 0x90) = Func_080022fc(M2C_FIELD(temp_r7, s32 *, 0x90) + 5, 6);
                Func_080f9010(0x6F);
                var_r8_2 = (s32 *)(temp_r7 + 0x94);
            } else {
                var_r8_2 = (s32 *)(temp_r7 + 0x94);
            }
        }
        temp_r2_2 = *sp18;
        if (temp_r2_2 != 5) {
            Func_08015018(M2C_FIELD(temp_r7, s32 *, 0x4C8), 1);
            var_fp = (s32 *)(temp_r7 + 0x98);
        } else {
            temp_r2_3 = M2C_FIELD(temp_r7, s32 *, 0x90);
            if (temp_r2_3 == 5) {
                if (var_r9 != 0) {
                    var_r6_3 = (s32 *)(temp_r7 + 0x618);
                    temp_r2_4 = M2C_FIELD(temp_r7, s32 *, 0x618);
                    if ((u32) (temp_r2_4 - 1) > 1U) {
                        Func_08015018(M2C_FIELD(temp_r7, s32 *, 0x4C8), 1);
                        temp_r0_2 = Func_08015010(0xB, 0, 0x13, 4, 6);
                        M2C_FIELD(temp_r7, s32 *, 0x4C8) = temp_r0_2;
                        Func_08015080(0x912, temp_r0_2, 0, 0);
                        var_r3 = 1;
                        goto block_65;
                    }
                    if (temp_r2_4 != 1) {
                        var_fp = (s32 *)(temp_r7 + 0x98);
                    } else {
                        Func_08015080(0x913, M2C_FIELD(temp_r7, s32 *, 0x4C8), 0, 8);
                        M2C_FIELD(temp_r7, s32 *, 0x618) = 2;
                        var_fp = (s32 *)(temp_r7 + 0x98);
                    }
                } else {
                    var_r6_3 = (s32 *)(temp_r7 + 0x618);
                    if (M2C_FIELD(temp_r7, s32 *, 0x618) != 3) {
                        Func_08015018(M2C_FIELD(temp_r7, s32 *, 0x4C8), 1);
                        temp_r0_3 = Func_08015010(0x10, 0, 0xE, 3, 6);
                        M2C_FIELD(temp_r7, s32 *, 0x4C8) = temp_r0_3;
                        Func_08015080(0x90F, temp_r0_3, 0, 0);
                    }
                    var_r3 = 3;
block_65:
                    *var_r6_3 = var_r3;
                    var_fp = (s32 *)(temp_r7 + 0x98);
                }
            } else if (M2C_FIELD(((temp_r2_3 * 0x1C) + temp_r7), u8 *, 0x19) == 0) {
                if (M2C_FIELD(temp_r7, s32 *, 0x618) != 4) {
                    Func_08015018(M2C_FIELD(temp_r7, s32 *, 0x4C8), 1);
                    temp_r0_4 = Func_08015010(0x17, 0, 7, 3, 6);
                    M2C_FIELD(temp_r7, s32 *, 0x4C8) = temp_r0_4;
                    Func_08015080(0x90D, temp_r0_4, 0, 0);
                }
                M2C_FIELD(temp_r7, s32 *, 0x618) = 4;
                var_fp = (s32 *)(temp_r7 + 0x98);
            } else {
                if (M2C_FIELD(temp_r7, s32 *, 0x618) != 5) {
                    Func_08015018(M2C_FIELD(temp_r7, s32 *, 0x4C8), 1);
                    temp_r0_5 = Func_08015010(0x17, 0, 7, 3, 6);
                    M2C_FIELD(temp_r7, s32 *, 0x4C8) = temp_r0_5;
                    Func_08015080(0x90E, temp_r0_5, 0, 0);
                }
                M2C_FIELD(temp_r7, s32 *, 0x618) = temp_r2_2;
                var_fp = (s32 *)(temp_r7 + 0x98);
            }
        }
        break;
    case 2:
        temp_r3_5 = M2C_FIELD(temp_r7, s32 *, 0xA8) + 1;
        M2C_FIELD(temp_r7, s32 *, 0xA8) = temp_r3_5;
        sp1C = 0;
        if (temp_r3_5 != 0x3C) {

        } else {
            *spC = 3;
            Func_080f9010(0x5D);
            M2C_FIELD(temp_r7, s32 *, 0xA8) = sp1C;
            M2C_FIELD((void *)0x04000050, s16 *, 0) = 0x3F44;
            M2C_FIELD((void *)0x04000050, s16 *, 2) = 0x1010;
            M2C_FIELD(sp24, s32 *, 0x7780) = temp_r5;
            M2C_FIELD(sp24, s32 *, 0x7784) = 0x4B;
        }
        var_fp = (s32 *)(temp_r7 + 0x98);
        var_r8_2 = (s32 *)(temp_r7 + 0x94);
        break;
    case 3:
        M2C_FIELD(temp_r7, s32 *, 0xA8) = (s32) (M2C_FIELD(temp_r7, s32 *, 0xA8) + 1);
        sp1C = 0;
        if (!(1 & M2C_FIELD(temp_r7, u16 *, 0xA0))) {
            var_fp = (s32 *)(temp_r7 + 0x98);
            var_r8_2 = (s32 *)(temp_r7 + 0x94);
        } else {
            *spC = 0xA;
            Func_080f9010(0x70);
block_170:
            var_r1 = (s32 *)(temp_r7 + 0x98);
            var_r2_5 = (s32 *)(temp_r7 + 0x94);
block_171:
            var_fp = var_r1;
            var_r8_2 = var_r2_5;
        }
        break;
    case 11:
        if (M2C_FIELD(temp_r7, s32 *, 0x618) == 0) {
            M2C_FIELD(temp_r7, s32 *, 0x618) = 1;
            Func_08015080(0x90C, M2C_FIELD(temp_r7, s32 *, 0x4C8), 0, 8);
        }
        if (!(1 & M2C_FIELD(temp_r7, u16 *, 0xA0))) {
            goto block_170;
        }
        *spC = 5;
        M2C_FIELD(temp_r7, s32 *, 0x618) = (s32) temp_r3;
        Func_080f9010(0x70);
        Func_08015018(M2C_FIELD(temp_r7, s32 *, 0x4C8), 1);
block_172:
        var_fp = (s32 *)(temp_r7 + 0x98);
        var_r8_2 = (s32 *)(temp_r7 + 0x94);
        break;
    case 20:
        temp_r3_6 = M2C_FIELD(temp_r7, s32 *, 0xA8) + 1;
        M2C_FIELD(temp_r7, s32 *, 0xA8) = temp_r3_6;
        if (temp_r3_6 != 0x2D) {
            goto block_172;
        }
        var_r1 = (s32 *)(temp_r7 + 0x98);
        var_r2_5 = (s32 *)(temp_r7 + 0x94);
        *spC = 0xA;
        goto block_171;
    case 10:
        var_fp = (s32 *)(temp_r7 + 0x98);
        var_r8_2 = (s32 *)(temp_r7 + 0x94);
        break;
    default:
        temp_r4_2 = (s32 *)(temp_r7 + 0xA8);
        sp14 = temp_r4_2;
        var_r3_2 = M2C_FIELD(temp_r7, s32 *, 0xA8);
        if (var_r3_2 == 4) {
            temp_r0_6 = Func_08015010(0x12, 0x11, 0xC, 3, 6);
            M2C_FIELD(temp_r7, s32 *, 0x4C8) = temp_r0_6;
            Func_08015080(0x90A, temp_r0_6, 0, 0);
            var_r3_2 = M2C_FIELD(temp_r7, s32 *, 0xA8);
        }
        if (var_r3_2 == 0x10) {
            Func_080f9010(0x132);
            var_r3_2 = M2C_FIELD(temp_r7, s32 *, 0xA8);
        }
        if ((var_r3_2 > 0x38) && (((s32) M2C_FIELD(sp24, s32 *, 0x778C) > 0x1F) || (0x100 & M2C_FIELD(temp_r7, u16 *, 0xA0)))) {
            M2C_FIELD(sp24, s32 *, 0x778C) = (s32) temp_r3;
            var_r6_4 = 0;
            var_r5_3 = temp_r7 + 0x18;
loop_103:
            if ((M2C_FIELD(var_r5_3, u8 *, 1) != 0) || (M2C_FIELD(var_r5_3, s8 *, 2) != -1)) {
                var_r6_4 += 1;
                var_r5_3 += 0x1C;
                if (var_r6_4 != 5) {
                    goto loop_103;
                }
            } else {
                M2C_FIELD(var_r5_3, s8 *, 2) = (s8) ((Func_08004458() & 3) + 4);
                Func_080f9010(0x133);
            }
        }
        var_r6_5 = 0;
        var_r2_6 = temp_r7 + 0x18;
        do {
            if ((s32) M2C_FIELD(var_r2_6, s8 *, 2) > 0) {
                M2C_FIELD(var_r2_6, s8 *, 2) = (s8) ((u8) M2C_FIELD(var_r2_6, s8 *, 2) - 1);
            }
            var_r6_5 += 1;
            var_r2_6 += 0x1C;
        } while (var_r6_5 != 5);
        var_r1_2 = 0;
        var_r6_6 = 0;
        var_r2_7 = temp_r7 + 0x18;
        var_r0 = 0;
        do {
            if ((M2C_FIELD(var_r2_7, u8 *, 1) == 1) || ((M2C_FIELD(var_r2_7, s8 *, 2) == 0) && ((M2C_FIELD(temp_r7, s32 *, var_r0) & 0xF) == 8))) {
                var_r1_2 += 1;
            }
            var_r6_6 += 1;
            var_r2_7 += 0x1C;
            var_r0 += 0x1C;
        } while (var_r6_6 != 5);
        if (var_r1_2 != 5) {
            var_fp = (s32 *)(temp_r7 + 0x98);
            var_r8_2 = (s32 *)(temp_r7 + 0x94);
        } else {
            sp10 = 0;
            var_sl = 0;
            var_fp = (s32 *)(temp_r7 + 0x98);
            var_r8_3 = (s32 *)(temp_r7 + 0xAC);
            do {
                *var_r8_3 = 0;
                temp_r2_5 = M2C_FIELD(temp_r7, s32 *, 0x98);
                var_r9_2 = 0;
                var_r4 = -1;
                if ((var_sl > (s32) (3 - temp_r2_5)) && (var_sl < (s32) (temp_r2_5 + 3))) {
                    var_r6_7 = 0;
                    var_r5_4 = 0;
                    do {
                        if (var_sl == 0) {
                            var_r0_2 = M2C_FIELD(temp_r7, s32 *, var_r5_4);
                            if (var_r0_2 < 0) {
                                var_r0_2 += 0xF;
                            }
                            var_r0_3 = (var_r6_7 - (var_r0_2 >> 4)) + 0x16;
                        } else if (var_sl == 6) {
                            var_r0_4 = M2C_FIELD(temp_r7, s32 *, var_r5_4);
                            if (var_r0_4 < 0) {
                                var_r0_4 += 0xF;
                            }
                            var_r0_3 = ((0 - var_r6_7) - (var_r0_4 >> 4)) + 0x1A;
                        } else {
                            var_r0_5 = M2C_FIELD(temp_r7, s32 *, var_r5_4);
                            if (var_r0_5 < 0) {
                                var_r0_5 += 0xF;
                            }
                            var_r0_3 = (var_sl - (var_r0_5 >> 4)) + 0x15;
                        }
                        temp_r3_7 = M2C_FIELD(temp_r7, u8 *, Func_080022fc(var_r0_3, 0x15) + var_r5_4 + 4);
                        if (temp_r3_7 != 5) {
                            if (var_r4 == -1) {
                                var_r4 = temp_r3_7;
                            } else if (var_r4 != temp_r3_7) {
                                var_r9_2 = 1;
                            }
                        }
                        var_r6_7 += 1;
                        var_r5_4 += 0x1C;
                    } while (var_r6_7 != 5);
                    if (var_r9_2 == 0) {
                        *var_r8_3 = 1;
                        M2C_FIELD((void *)0x0200024C, u8 *, sp10 + 0x120) =
                            M2C_FIELD((void *)0x080F870C, u8 *, var_r4);
                        sp10 += 1;
                    }
                }
                var_sl += 1;
                var_r8_3 += 1;
            } while (var_sl != 7);
            *sp14 = 0;
            if (sp10 != 0) {
                M2C_FIELD((void *)0x0200024C, s8 *, sp10 + 0x120) = -1;
                *sp18 = 2;
                Func_080f9010(0xAB);
                M2C_FIELD(sp24, s32 *, 0x7780) = 1;
                M2C_FIELD(sp24, s32 *, 0x7784) = 0;
                M2C_FIELD((void *)0x04000050, s16 *, 0) = 0;
                Func_08015018(M2C_FIELD(temp_r7, s32 *, 0x4C8), 1);
                var_r8_2 = (s32 *)(temp_r7 + 0x94);
            } else {
                *sp18 = 0xB;
                M2C_FIELD(temp_r7, s32 *, 0x618) = sp10;
                Func_08015018(M2C_FIELD(temp_r7, s32 *, 0x4C8), 1);
                temp_r0_7 = Func_08015010(3, 0x10, 0x18, 4, 6);
                M2C_FIELD(temp_r7, s32 *, 0x4C8) = temp_r0_7;
                Func_08015080(0x90B, temp_r0_7, 0, 0);
                var_r8_2 = (s32 *)(temp_r7 + 0x94);
                if (M2C_FIELD(temp_r7, s32 *, 0x94) == 4) {
                    temp_r0_8 = Func_080772e0(0xE4);
                    if (temp_r0_8 > 0) {
                        *sp18 = 0x14;
                    } else {
                        *sp18 = 0x14;
                    }
                    if ((s32) M2C_FIELD(temp_r7, s32 *, 0x98) > temp_r0_8) {
                        M2C_FIELD(temp_r7, s32 *, 0x98) = temp_r0_8;
                    }
                    M2C_FIELD(temp_r7, s32 *, 0x94) = 0;
                    M2C_FIELD(temp_r7, s32 *, 0x90) = 0;
                    var_r6_8 = 0;
                    var_r2_8 = temp_r7 + 0x18;
                    do {
                        var_r6_8 += 1;
                        M2C_FIELD(var_r2_8, s8 *, 1) = 0;
                        M2C_FIELD(var_r2_8, u8 *, 2) = (u8) (M2C_FIELD(var_r2_8, u8 *, 2) | 0xFF);
                        var_r2_8 += 0x1C;
                    } while (var_r6_8 != 5);
                    M2C_FIELD(sp24, s32 *, 0x7780) = 1;
                    M2C_FIELD(sp24, s32 *, 0x7784) = 0;
                    M2C_FIELD((void *)0x04000050, s16 *, 0) = 0;
                    temp_r0_9 = Func_08015010(0x12, 0, 0xC, 4, 6);
                    M2C_FIELD(temp_r7, s32 *, 0x4CC) = temp_r0_9;
                    Func_08015080(0x905, temp_r0_9, 0, 8);
                    Func_08015080(0x904, M2C_FIELD(temp_r7, s32 *, 0x4CC), 0, 0);
                }
            }
        }
        if (*sp18 == 1) {
            var_r6_9 = 0;
            var_r1_3 = temp_r7;
            do {
                if (M2C_FIELD(var_r1_3, u8 *, 0x19) == 0) {
                    if (M2C_FIELD(var_r1_3, s8 *, 0x1A) != 0) {
                        goto block_161;
                    }
                    var_r2_9 = M2C_FIELD(var_r1_3, s32 *, 0);
                    if ((0xF & var_r2_9) != 8) {
block_161:
                        temp_r3_8 = M2C_FIELD(var_r1_3, s32 *, 0) + 8;
                        M2C_FIELD(var_r1_3, s32 *, 0) = temp_r3_8;
                        var_r2_9 = temp_r3_8;
                    }
                    if (var_r2_9 == 0x150) {
                        M2C_FIELD(var_r1_3, s32 *, 0) = 0;
                    }
                }
                var_r6_9 += 1;
                var_r1_3 += 0x1C;
            } while (var_r6_9 != 5);
        }
        M2C_FIELD(sp24, s32 *, 0x778C) = (s32) (M2C_FIELD(sp24, s32 *, 0x778C) + 1);
        *sp14 += 1;
        break;
    }
common_render:
    if (*spC == 5) {
        temp_r1_2 = M2C_FIELD(temp_r7, s32 *, 0x90);
        var_r5_5 = (temp_r1_2 * 0x24) + 0x24;
        var_r6_10 = 0x80;
        var_r0_6 = 0;
        if ((s32) (M2C_FIELD(temp_r7, s32 *, 0xA8) & 0xF) <= 7) {
            var_r0_6 = 1;
        }
        if (temp_r1_2 == 5) {
            var_r5_5 = 0xD0;
            var_r6_10 = 0x20;
        }
        temp_r4_3 = var_r6_10 + 8;
        M2C_FIELD(temp_r7, s32 *, 0xC8) = (s32) (((var_r5_5 - 0xC) << 0x10) | sp1C | temp_r4_3 | 0x80006000);
        temp_r1_3 = (var_r0_6 * 0x10) + 0x2B0;
        M2C_FIELD(temp_r7, s32 *, 0xCC) = temp_r1_3;
        M2C_FIELD(temp_r7, s32 *, 0xD0) = (s32) (((var_r5_5 + 0xC) << 0x10) | sp1C | temp_r4_3 | 0x90006000);
        M2C_FIELD(temp_r7, s32 *, 0xD4) = temp_r1_3;
        M2C_FIELD(temp_r7, s32 *, 0xD8) = (s32) ((var_r5_5 << 0x10) | sp1C | var_r6_10 | 0x80002000);
        M2C_FIELD(temp_r7, s32 *, 0xDC) = 0x1F0;
        sp20 = 3;
    }
    if (*spC == 3) {
        var_lr = sp24;
        temp_r3_9 = sp20 * 8;
        var_r6_11 = 0;
        var_ip = temp_r3_9 + 0xCC;
        var_r5_6 = temp_r3_9 + 0xC8;
        do {
            temp_r0_10 = var_lr + 0x7080;
            M2C_FIELD(temp_r7, s32 *, var_r5_6) = (M2C_FIELD(temp_r0_10, s16 *, 2) << 0x10) | sp1C | (u8) (M2C_FIELD(temp_r0_10, s16 *, 6) + 0x100) | 0x80000000;
            M2C_FIELD(temp_r7, s32 *, var_ip) = ((M2C_FIELD((void *)0x080F8712, u8 *, var_r6_11) * 0x10) + 0x370) | 0xF000;
            temp_r2_6 = M2C_FIELD(temp_r0_10, s32 *, 0x10);
            M2C_FIELD(temp_r0_10, s32 *, 4) = (s32) (M2C_FIELD(temp_r0_10, s32 *, 4) + temp_r2_6);
            M2C_FIELD(temp_r0_10, s32 *, 0x10) = (s32) (temp_r2_6 + 0x4000);
            temp_r3_10 = M2C_FIELD(temp_r7, s32 *, 0xA8);
            var_r2_10 = temp_r3_10;
            if (temp_r3_10 < 0) {
                var_r2_10 += 0xFF;
            }
            if ((temp_r3_10 - ((var_r2_10 >> 8) << 8)) == (((s32) var_r6_11 * 4) + 0xC8)) {
                M2C_FIELD(temp_r0_10, s32 *, 0x10) = 0x60000;
                M2C_FIELD(temp_r0_10, s32 *, 0x18) = 0;
            }
            if ((s32) M2C_FIELD(temp_r0_10, s32 *, 4) > 0x400000) {
                temp_r1_4 = M2C_FIELD(temp_r0_10, s32 *, 0x18);
                M2C_FIELD(temp_r0_10, s32 *, 4) = 0x400000;
                if (temp_r1_4 <= 1) {
                    temp_r3_11 = 0 - M2C_FIELD(temp_r0_10, s32 *, 0x10);
                    M2C_FIELD(temp_r0_10, s32 *, 0x10) = (s32) ((s32) (temp_r3_11 + (temp_r3_11 >> 0x1F)) >> 1);
                }
                M2C_FIELD(temp_r0_10, s32 *, 0x18) = (s32) (temp_r1_4 + 1);
            }
            var_r6_11 += 1;
            var_ip += 8;
            var_r5_6 += 8;
            sp20 += 1;
            var_lr += 0x1C;
        } while (var_r6_11 != 8);
    }
    temp_r3_12 = sp20 * 8;
    var_r1_4 = temp_r3_12 + 0xCC;
    var_r6_12 = (u8 *)0x080F871A;
    var_ip_2 = (u8 *)0x080F8728;
    var_r5_7 = 0;
    var_r4_2 = var_r1_4;
    var_r0_7 = temp_r3_12 + 0xC8;
    do {
        temp_r2_7 = *var_ip_2;
        var_ip_2 += 1;
        temp_r3_13 = (*var_r6_12 << 0x10) | sp1C | temp_r2_7 | 0x80006000;
        var_r6_12 += 1;
        M2C_FIELD(temp_r7, s32 *, var_r0_7) = temp_r3_13;
        if (var_r5_7 <= 3) {
            M2C_FIELD(temp_r7, s32 *, var_r4_2) = 0x4E0;
        } else {
            M2C_FIELD(temp_r7, s32 *, var_r1_4) = 0x4E8;
        }
        var_r5_7 += 1;
        temp_r3_14 = sp20 + 1;
        var_r1_4 += 8;
        var_r4_2 += 8;
        var_r0_7 += 8;
        sp20 = temp_r3_14;
    } while (var_r5_7 != 0xE);
    temp_r3_15 = temp_r3_14 * 8;
    var_r1_5 = temp_r3_15 + 0xCC;
    var_r2_11 = temp_r3_15 + 0xC8;
    var_r6_13 = 0;
    var_r5_8 = 0x200000;
    var_ip_3 = temp_r7 + 0x19;
    var_r4_3 = var_r1_5;
    var_r9_3 = var_r2_11;
    do {
        temp_r3_16 = *var_ip_3;
        var_ip_3 += 0x1C;
        if (temp_r3_16 == 0) {
            M2C_FIELD(temp_r7, s32 *, var_r9_3) = sp1C | var_r5_8 | 0x8000207C;
            M2C_FIELD(temp_r7, s32 *, var_r4_3) = 0x460;
        } else {
            M2C_FIELD(temp_r7, s32 *, var_r2_11) = sp1C | var_r5_8 | 0x8000207C;
            M2C_FIELD(temp_r7, s32 *, var_r1_5) = 0x480;
        }
        var_r9_3 += 8;
        var_r6_13 += 1;
        var_r1_5 += 8;
        var_r2_11 += 8;
        var_r4_3 += 8;
        sp20 += 1;
        var_r5_8 += 0x240000;
    } while (var_r6_13 != 5);
    temp_r3_17 = sp20 * 8;
    var_r2_12 = temp_r3_17 + 0xCC;
    var_r6_14 = 0;
    var_r5_9 = 0x200000;
    var_r0_8 = var_r2_12;
    var_r9_4 = temp_r3_17 + 0xC8;
    var_r4_4 = 0x210;
    do {
        M2C_FIELD(temp_r7, s32 *, var_r9_4) = sp1C | var_r5_9 | 0x80006003;
        if (var_r6_14 == *var_r8_2) {
            M2C_FIELD(temp_r7, s32 *, var_r0_8) = var_r4_4 | 0x400;
        } else {
            M2C_FIELD(temp_r7, s32 *, var_r2_12) = ((var_r6_14 << 5) + 0x220) | 0x400;
        }
        var_r9_4 += 8;
        temp_r1_5 = sp20 + 1;
        var_r6_14 += 1;
        var_r2_12 += 8;
        var_r0_8 += 8;
        sp20 = temp_r1_5;
        var_r5_9 += 0x100000;
        var_r4_4 += 0x20;
    } while (var_r6_14 != 5);
    temp_r3_18 = temp_r1_5 * 8;
    var_sl_2 = 0;
    var_r5_10 = 5;
    var_r4_5 = temp_r3_18 + 0xCC;
    var_r0_9 = temp_r3_18 + 0xC8;
    var_r1_6 = temp_r3_18;
    do {
        M2C_FIELD(temp_r7, s32 *, var_r0_9) = (((0x204 - ((1 & var_sl_2) * 8)) & 0x1FF) << 0x10) | sp1C | var_r5_10 | 0x80002000;
        temp_r2_8 = *var_fp;
        if ((var_sl_2 > (s32) (3 - temp_r2_8)) && (var_sl_2 < (s32) (temp_r2_8 + 3))) {
            M2C_FIELD(temp_r7, s32 *, var_r4_5) = 0x5D0;
        } else {
            M2C_FIELD(temp_r7, s32 *, var_r1_6 + 0xCC) = 0x510;
        }
        var_sl_2 += 1;
        sp20 += 1;
        var_r4_5 += 8;
        var_r0_9 += 8;
        var_r1_6 += 8;
        var_r5_10 += 0x10;
    } while (var_sl_2 != 7);
    var_r6_15 = 0;
    var_fp_2 = 0x280000;
    var_r5_11 = temp_r7;
    do {
        temp_r3_19 = sp20 * 8;
        var_sl_3 = 0;
        var_r8_4 = temp_r3_19 + 0xCC;
        var_r4_6 = temp_r3_19 + 0xC8;
loop_214:
        temp_r2_9 = M2C_FIELD(var_r5_11, s32 *, 0);
        var_r3_3 = temp_r2_9;
        if (temp_r2_9 < 0) {
            var_r3_3 += 0xF;
        }
        M2C_FIELD(temp_r7, s32 *, var_r4_6) = ((var_sl_3 * 0x10) + (temp_r2_9 - ((var_r3_3 >> 4) * 0x10)) + 4) | var_fp_2 | 0x80006000;
        var_r0_10 = M2C_FIELD(var_r5_11, s32 *, 0);
        if (var_r0_10 < 0) {
            var_r0_10 += 0xF;
        }
        M2C_FIELD(temp_r7, s32 *, var_r8_4) = (M2C_FIELD(var_r5_11, u8 *, Func_080022fc((var_sl_3 - (var_r0_10 >> 4)) + 0x15, 0x15) + 4) * 0x10) | 0x800;
        var_sl_3 += 1;
        var_r8_4 += 8;
        var_r4_6 += 8;
        sp20 += 1;
        if (var_sl_3 != 7) {
            goto loop_214;
        }
        var_r6_15 += 1;
        var_fp_2 += 0x200000;
        var_r5_11 += 0x1C;
    } while (var_r6_15 != 5);
    var_r4_7 = 0x28;
    if (*spC == 1) {
        temp_r0_11 = M2C_FIELD(temp_r7, s32 *, 0xA8);
        if (temp_r0_11 <= 0x2F) {
            var_r0_11 = Func_08002322(temp_r0_11 * 0x2AA) << 6;
            goto block_225;
        }
        if (temp_r0_11 <= 0x37) {
            var_r0_11 = Func_08002322((temp_r0_11 << 0xC) + 0xFFFD0000) * 4;
block_225:
            var_r4_7 = (var_r0_11 >> 0x10) + 0x28;
        }
    }
    temp_r2_10 = sp20 * 8;
    M2C_FIELD(temp_r7, s32 *, temp_r2_10 + 0xC8) = var_r4_7 | sp1C | 0x80D06000;
    M2C_FIELD(temp_r7, s32 *, temp_r2_10 + 0xCC) = 0x500;
    temp_r1_6 = sp20 + 1;
    temp_r3_20 = temp_r1_6 * 8;
    var_r6_16 = temp_r3_20 + 0xCC;
    var_r4_8 = var_r6_16;
    var_r0_12 = var_r4_8;
    sp20 = temp_r1_6;
    var_r1_7 = var_r0_12;
    var_r5_12 = 0;
    var_r8_5 = 0xC;
    var_lr_2 = var_r1_7;
    var_ip_4 = temp_r3_20 + 0xC8;
    do {
        M2C_FIELD(temp_r7, s32 *, var_ip_4) = sp1C | var_r8_5 | 0x80CE6000;
        switch (var_r5_12) {
        case 0:
            M2C_FIELD(temp_r7, s32 *, var_lr_2) = 0x540;
            break;
        case 1:
            M2C_FIELD(temp_r7, s32 *, var_r1_7) = 0x550;
            break;
        case 6:
            M2C_FIELD(temp_r7, s32 *, var_r0_12) = 0x570;
            break;
        case 7:
            M2C_FIELD(temp_r7, s32 *, var_r4_8) = 0x580;
            break;
        default:
            M2C_FIELD(temp_r7, s32 *, var_r6_16) = 0x560;
            break;
        }
        var_lr_2 += 8;
        var_ip_4 += 8;
        temp_r2_11 = sp20 + 1;
        var_r5_12 += 1;
        var_r6_16 += 8;
        var_r4_8 += 8;
        var_r0_12 += 8;
        var_r1_7 += 8;
        sp20 = temp_r2_11;
        var_r8_5 += 0x10;
    } while (var_r5_12 != 8);
    if (temp_r2_11 != 0x80) {
        temp_r3_21 = temp_r2_11 * 8;
        var_r2_13 = temp_r3_21 + 0xCC;
        var_r3_4 = temp_r3_21 + 0xC8;
        do {
            M2C_FIELD(temp_r7, s32 *, var_r3_4) = 0x40F02000;
            M2C_FIELD(temp_r7, s32 *, var_r2_13) = 0;
            temp_r4_4 = sp20 + 1;
            var_r2_13 += 8;
            var_r3_4 += 8;
            sp20 = temp_r4_4;
        } while (temp_r4_4 != 0x80);
    }
    M2C_FIELD((void *)0x040000D4, void **, 0) = (void *) (temp_r7 + 0xC8);
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x07000000;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = (s32) ((sp20 * 2) | 0x84000000);
}
