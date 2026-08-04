#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct Work_80190000;
struct Work_02000000;

/*
 * m2c named several arrays after their original frame offsets.  Keep those
 * names while making the storage and the intentional overlap explicit.
 */
#define subroutine_arg0 semantic_frame_08026080[0]
#define unksp144 semantic_frame_08026080[324]
#define sp58 (*(s32 *)(semantic_frame_08026080 + 88))
#define sp60 (*(s32 *)(semantic_frame_08026080 + 96))
#define sp6C (*(s32 *)(semantic_frame_08026080 + 108))
#define sp78 (*(u16 (*)[32])(semantic_frame_08026080 + 120))
#define sp98 (*(s32 (*)[2])(semantic_frame_08026080 + 152))
#define spAC (*(u16 (*)[32])(semantic_frame_08026080 + 172))
#define spC8 (*(s32 (*)[2])(semantic_frame_08026080 + 200))
#define sp134 (*(u16 (*)[8])(semantic_frame_08026080 + 308))

s32 Func_080022fc(s32, s32);
s32 Func_08002322(s32);
void Func_080030f8(s32);
s32 Func_08003d28(void *);
void Func_08003dec(void *, s32);
void Func_08003f3c(u32);
void *Func_080040b4(s32);
s32 Func_080040d0(s32, s32);
void *Func_080162d4(s32, s32, s32, s32, s32);
void Func_08016418(void *, s32);
void Func_0801671c(void);
s32 Func_08017a64(const u16 *);
void Func_08017aa4(const s16 *, void *, s32, s32);
void Func_0801e71c(s32);
void Func_0801e8b0(s32, struct Work_02000000 *, s32, s32);
void Func_0801ea08(s32, s32, void *, s32, s32);
void Func_08026388(void);
void *Func_08077008(s32);
s32 Func_080770c0(s32);
void Func_080b50e0(u16 *, s32);
void Func_080f9010(s32);

void Func_0801965c(s32, u16 *, s32);
void Func_0801e7c0(s32, void *, s32, s32);
void Func_080b50b8(u16, s32 *);

s32 Func_08026080(u16 arg0, s32 arg1, u32 arg2, u32 arg3) {
    u8 semantic_frame_08026080[400];
    void *sp4;
    s32 sp8;
    s32 spC;
    u8 *sp10;
    void *sp14;
    s32 sp18;
    u16 *sp1C;
    u8 *sp20;
    u8 *sp24;
    u8 *sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    s32 sp3C;
    s32 sp40;
    s32 sp44;
    s32 sp48;
    u8 *sp4C;
    u32 sp50;
    u32 sp54;
    s16 *var_r2_6;
    s16 temp_r3_26;
    s16 temp_r3_27;
    s16 temp_r3_28;
    s16 temp_r3_29;
    s16 var_r0_9;
    s32 *var_r4_3;
    u8 *var_r5_7;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_5;
    s32 temp_r0_6;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r1_6;
    s32 temp_r1_7;
    s32 temp_r2_6;
    s32 temp_r3_13;
    s32 temp_r3_15;
    s32 temp_r3_16;
    s32 temp_r3_17;
    s32 temp_r3_18;
    s32 temp_r3_19;
    s32 temp_r3_20;
    s32 temp_r3_21;
    s32 temp_r3_23;
    s32 temp_r3_25;
    s32 temp_r3_30;
    s32 temp_r3_3;
    s32 temp_r3_6;
    s32 temp_r3_8;
    s32 temp_r3_9;
    s32 temp_r4;
    s32 temp_r6;
    u8 *temp_r8;
    s32 var_r0;
    s32 var_r0_12;
    s32 var_r0_15;
    s32 var_r0_17;
    s32 var_r0_19;
    s32 var_r0_2;
    s32 var_r0_4;
    s32 var_r0_5;
    s32 var_r0_7;
    s32 var_r1_4;
    s32 var_r1_5;
    s32 var_r1_6;
    s32 var_r1_7;
    s32 var_r2_4;
    s32 var_r2_5;
    s32 var_r3;
    s32 var_r3_10;
    s32 var_r3_11;
    s32 var_r3_12;
    s32 var_r3_13;
    s32 var_r3_6;
    s32 var_r3_7;
    s32 var_r3_8;
    s32 var_r3_9;
    s32 var_r4;
    s32 var_r5;
    s32 var_r5_2;
    s32 var_r5_4;
    s32 var_r5_5;
    s32 var_r5_6;
    s32 var_r5_8;
    s32 var_r6;
    s32 var_r6_3;
    s32 var_r7;
    s32 var_r7_2;
    s32 var_r7_3;
    s32 var_r7_4;
    s32 var_r7_5;
    s32 var_r7_7;
    s32 var_r7_8;
    s32 var_r7_9;
    s32 var_r9;
    s8 *var_r3_2;
    s8 *var_r6_2;
    s8 temp_r1_4;
    s8 var_r3_3;
    s8 var_r7_6;
    u16 *var_r1;
    u16 *var_r2;
    u16 *var_r2_2;
    u16 *var_r2_3;
    u16 *var_r5_3;
    u16 temp_r2_2;
    u16 temp_r2_4;
    u16 temp_r3;
    u16 temp_r3_10;
    u16 temp_r4_2;
    u16 var_r0_10;
    u16 var_r0_11;
    u16 var_r0_13;
    u16 var_r0_14;
    u16 var_r0_16;
    u16 var_r0_6;
    u16 var_r0_8;
    u16 var_r3_5;
    u16 var_sl;
    u32 temp_r0_3;
    u32 temp_r1_5;
    u32 temp_r3_14;
    u32 temp_r3_22;
    u32 temp_r3_24;
    u32 temp_r3_2;
    u32 temp_r3_4;
    u32 temp_r3_5;
    u8 *var_r3_4;
    u8 temp_r0_4;
    u8 temp_r2;
    u8 temp_r2_3;
    u8 temp_r2_5;
    u8 temp_r3_11;
    u8 temp_r3_12;
    u8 temp_r3_7;
    u8 var_r0_18;
    u8 *temp_r1;
    void *temp_r2_7;
    u8 *var_r0_3;
    u8 *var_r1_2;
    u8 *var_r1_3;
    u8 *var_r4_2;

    sp54 = arg2;
    sp50 = arg3;
    var_sl = arg0;
    sp4C = M2C_FIELD((void *)0x03001E74, void **, 0);
    sp40 = 0;
    sp38 = 0xFFFF;
    sp34 = Func_080040b4(0x100);
    var_r9 = 0;
    if (sp54 == 0) {
        sp54 = 1;
    }
    if ((arg1 == 2) || (arg1 == 4)) {
        var_r3 = -2;
    } else {
        var_r3 = 0x10;
    }
    M2C_FIELD(M2C_FIELD((void *)0x03001E74, void **, 0xC0), s32 *, 0x28) = var_r3;
    sp24 = &subroutine_arg0 + 0xD4;
    var_r7 = 5;
    var_r3_2 = &subroutine_arg0 + 0xEA;
    do {
        var_r7 -= 1;
        *var_r3_2 = 0;
        var_r3_2 -= 4;
    } while (var_r7 >= 0);
    sp44 = -1;
    if (arg1 == 2) {
        var_r7_2 = 0;
        if (M2C_FIELD(sp4C, s16 *, 0x58) != 0xFF) {
            sp1C = &sp134[0];
            var_r2 = sp4C + 0x58;
            var_r1 = &(&sp134[0])[sp40];
loop_11:
            *var_r1 = *var_r2;
            var_r7_2 += 1;
            var_r1 += 1;
            sp40 += 1;
            var_r2 += 1;
            if (var_r7_2 <= 5) {
                if (*var_r2 == 0xFF) {

                } else {
                    goto loop_11;
                }
            }
        } else {
            sp1C = &sp134[0];
        }
    } else if (arg1 == 4) {
        sp1C = &sp134[0];
        sp134[0] = var_sl;
        sp40 = 1;
    } else {
        temp_r1 = sp4C + 2;
        var_r7_3 = 0;
        if (M2C_FIELD(temp_r1, s16 *, 0x64) != 0xFF) {
            sp1C = &sp134[0];
            var_r0 = 0x64;
            var_r2_2 = &sp1C[sp40];
loop_18:
            *var_r2_2 = *(u16 *)(temp_r1 + var_r0);
            var_r7_3 += 1;
            var_r2_2 += 1;
            sp40 += 1;
            var_r0 += 2;
            if (var_r7_3 <= 5) {
                if (*(s16 *)(temp_r1 + var_r0) == 0xFF) {

                } else {
                    goto loop_18;
                }
            }
        } else {
            sp1C = &sp134[0];
        }
    }
    sp1C[sp40] = 0xFF;
    sp3C = sp40;
    if (arg1 != 2) {

    } else {
        if ((sp54 != 0xFF) && (sp50 != 0)) {
            var_r5 = 0;
            var_r7_4 = 0;
            if (sp40 > 0) {
                var_r4 = 0xFFFF;
                var_r6 = 0;
loop_30:
                temp_r3 = sp1C[var_r5];
                if (temp_r3 != 0xFE) {
                    sp8 = var_r4;
                    temp_r1_2 = Func_08077008((s32) temp_r3);
                    switch (sp50) {
                    case 5:
                        if (M2C_FIELD(temp_r1_2, s16 *, 0x38) == 0) {
                            var_r7_4 = 1;
                            var_sl = sp1C[var_r5];
                        }
                        break;
                    case 3:
                        var_r3_3 = (s8) M2C_FIELD(temp_r1_2, u8 *, 0x131);
block_50:
                        if (var_r3_3 != 0) {
block_51:
                            var_r7_4 = 1;
                        }
                        break;
                    case 4:
                        if (!(M2C_FIELD(temp_r1_2, s32 *, 0x138) & 0xFF0000FF) && !(M2C_FIELD(temp_r1_2, u16 *, 0x13C) & var_r4)) {
                            var_r3_4 = temp_r1_2 + 0x141;
block_49:
                            var_r3_3 = (s8) *var_r3_4;
                            goto block_50;
                        }
                        goto block_51;
                    case 6:
                        if (!(M2C_FIELD(temp_r1_2, s32 *, 0x138) & 0xFF0000FF) && !(M2C_FIELD(temp_r1_2, u16 *, 0x13C) & var_r4) && (M2C_FIELD(temp_r1_2, u8 *, 0x141) == 0) && ((s8) M2C_FIELD(temp_r1_2, u8 *, 0x131) == 0)) {
                            var_r3_4 = temp_r1_2 + 0x140;
                            goto block_49;
                        }
                        goto block_51;
                    }
                    if (var_r7_4 != 0) {
                        var_sl = sp1C[var_r5];
                    } else {
                        goto block_54;
                    }
                } else {
block_54:
                    var_r5 += 1;
                    var_r6 += 2;
                    if (var_r5 < sp40) {
                        goto loop_30;
                    }
                }
            }
        }
        var_r5_2 = 0;
        if ((sp40 > 0) && (*sp1C != var_sl)) {
            var_r2_3 = sp1C;
loop_58:
            var_r5_2 += 1;
            if (var_r5_2 < sp40) {
                var_r2_3 += 1;
                if (*var_r2_3 != var_sl) {
                    goto loop_58;
                }
            }
        }
        if (var_r5_2 != sp40) {
            sp44 = var_r5_2;
        }
    }
    if (sp44 < 0) {
        temp_r3_2 = sp40 - 1;
        sp44 = (s32) (temp_r3_2 + (temp_r3_2 >> 0x1F)) >> 1;
    }
loop_65:
    sp18 = sp44 * 2;
    if (sp1C[sp44] == 0xFE) {
block_64:
        temp_r3_3 = (sp44 + sp40) - 1;
        sp44 = temp_r3_3;
        sp44 = Func_080022fc(temp_r3_3, sp40);
        goto loop_65;
    }
    if ((Func_080770c0(0x16C) != 0) && (arg1 == 1) && (M2C_FIELD((void *) Func_08077008((s32) sp1C[sp44]), s16 *, 0x38) == 0)) {
        goto block_64;
    }
    if (arg1 != 2) {
        Func_080b50b8(var_sl, &spC8);
        M2C_FIELD(sp24, u8 *, 2) = 8U;
        M2C_FIELD(sp24, u8 *, 0) = (u8)spC8[0];
        M2C_FIELD(sp24, u8 *, 1) = 0x80U;
    }
    sp48 = Func_080162d4(0U, 0xCU, 0x1EU, 4U, 0x4A);
    sp20 = &subroutine_arg0 + 0xEC;
    sp14 = &subroutine_arg0 + 0x58;
    sp30 = 0;
    Func_080b50b8(sp1C[sp44], &sp98);
    M2C_FIELD(sp20, s32 *, 4) = 0x40002000;
    M2C_FIELD(sp20, s32 *, 8) = sp30;
    M2C_FIELD(sp20, s32 *, 8) = (s16) ((0xFFFFFC00 & (u16) M2C_FIELD(sp20, s32 *, 8)) | (Func_080040d0(sp34, ((((u32) *(u32 *)0x03001E40 >> 2) & 0x1F) << 8) + 0x080346F8) & 0x3FF));
    var_r0_2 = Func_08002322(*(u32 *)0x03001E40 << 0xC);
    if (var_r0_2 < 0) {
        var_r0_2 += 0x7FFF;
    }
    temp_r0 = M2C_FIELD(&sp98, s32 *, 4) + (var_r0_2 >> 0xF);
    M2C_FIELD(&sp98, s32 *, 4) = temp_r0;
    temp_r2 = M2C_FIELD(sp24, u8 *, 2);
    if (1 & temp_r2) {
        temp_r4 = M2C_FIELD(&sp98, s32 *, 0);
        temp_r3_4 = temp_r4 + M2C_FIELD(sp24, u8 *, 0);
        temp_r1_3 = (s32) (temp_r3_4 + (temp_r3_4 >> 0x1F)) >> 1;
        temp_r3_5 = temp_r0 + M2C_FIELD(sp24, u8 *, 1);
        temp_r0_2 = (s32) (temp_r3_5 + (temp_r3_5 >> 0x1F)) >> 1;
        temp_r3_6 = temp_r4 - temp_r1_3;
        if (temp_r3_6 >= 0) {
            if (temp_r3_6 > 7) {

            } else {
                goto block_79;
            }
        } else if ((s32) (temp_r1_3 - temp_r4) <= 7) {
block_79:
            sp30 = 1;
        }
        M2C_FIELD(&sp98, s32 *, 0) = temp_r1_3;
        M2C_FIELD(&sp98, s32 *, 4) = temp_r0_2;
        M2C_FIELD(sp24, u8 *, 0) = (u8) temp_r1_3;
        M2C_FIELD(sp24, u8 *, 1) = (u8) temp_r0_2;
    } else if ((u32) (temp_r2 << 0x18) <= 0x03000000U) {
        M2C_FIELD(&sp98, s32 *, 4) = temp_r0;
        M2C_FIELD(sp24, u8 *, 0) = (u8) M2C_FIELD(&sp98, s32 *, 0);
        M2C_FIELD(sp24, u8 *, 1) = (u8) temp_r0;
        M2C_FIELD(sp24, u8 *, 2) = 1U;
    } else {
        M2C_FIELD(&sp98, s32 *, 0) = (s32) M2C_FIELD(sp24, u8 *, 0);
        M2C_FIELD(&sp98, s32 *, 4) = (s32) M2C_FIELD(sp24, u8 *, 1);
        temp_r3_7 = temp_r2 + 0xFC;
        M2C_FIELD(sp24, u8 *, 2) = temp_r3_7;
        if ((u32) (temp_r3_7 << 0x18) <= 0x03000000U) {
            M2C_FIELD(sp24, u8 *, 2) = 1U;
        }
    }
    M2C_FIELD(sp20, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(sp20, u16 *, 6)) | ((M2C_FIELD(&sp98, s32 *, 0) - 8) & 0x1FF));
    M2C_FIELD(sp20, s32 *, 4) = (s8) (M2C_FIELD(&sp98, s32 *, 4) - 0x10);
    Func_08003dec(sp20, 0xF0);
    if (sp54 == 0xFF) {
        var_r3_5 = (sp58 & 0xFFFF0000) | 0x100;
        var_r2_4 = 0x01000000;
    } else {
        var_r3_5 = (sp58 & 0xFFFF0000) | 0xB0;
        var_r2_4 = 0xB00000;
    }
    sp58 = var_r3_5 | var_r2_4;
    M2C_FIELD(sp14, s32 *, 4) = (s32) (M2C_FIELD(sp14, s32 *, 4) & 0xFFFF0000);
    sp2C = Func_08003d28(sp14);
    if (!(1 & sp38)) {

    } else {
        sp40 = 0;
        var_r1_2 = sp24;
        var_r7_5 = 5;
        do {
            var_r7_5 -= 1;
            M2C_FIELD(var_r1_2, u8 *, 2) = (u8) (0xFD & M2C_FIELD(var_r1_2, u8 *, 2));
            var_r1_2 += 4;
        } while (var_r7_5 >= 0);
        var_r7_6 = 0;
        if (sp54 > 0U) {
            var_r4_2 = sp24;
            var_r5_3 = &(&spAC[0])[sp40];
            var_r6_2 = (sp40 + &unksp144) - 0xA0;
            var_r0_3 = var_r4_2 + 0x18;
            do {
                temp_r3_8 = sp44 + var_r7_6;
                if (temp_r3_8 < sp3C) {
                    temp_r2_2 = sp1C[temp_r3_8];
                    if (temp_r2_2 != 0xFE) {
                        *var_r5_3 = temp_r2_2;
                        temp_r2_3 = 2 | M2C_FIELD(var_r4_2, u8 *, 2);
                        M2C_FIELD(var_r4_2, u8 *, 2) = temp_r2_3;
                        if (M2C_FIELD(var_r4_2, s8 *, 3) != var_r7_6) {
                            M2C_FIELD(var_r4_2, u8 *, 2) = (u8) (temp_r2_3 & 0xFE);
                            M2C_FIELD(var_r4_2, s8 *, 3) = var_r7_6;
                        }
                        *var_r6_2 = var_r7_6;
                        sp40 += 1;
                        var_r6_2 += 1;
                        var_r5_3 += 1;
                    }
                }
                if (var_r7_6 != 0) {
                    temp_r3_9 = sp44 - var_r7_6;
                    if (temp_r3_9 >= 0) {
                        temp_r2_4 = sp1C[temp_r3_9];
                        if (temp_r2_4 != 0xFE) {
                            *var_r5_3 = temp_r2_4;
                            temp_r2_5 = 2 | M2C_FIELD(var_r0_3, u8 *, 2);
                            temp_r1_4 = 0 - var_r7_6;
                            M2C_FIELD(var_r0_3, u8 *, 2) = temp_r2_5;
                            if (M2C_FIELD(var_r0_3, s8 *, 3) != temp_r1_4) {
                                M2C_FIELD(var_r0_3, u8 *, 2) = (u8) (temp_r2_5 & 0xFE);
                                M2C_FIELD(var_r0_3, s8 *, 3) = temp_r1_4;
                            }
                            *var_r6_2 = 6 - var_r7_6;
                            sp40 += 1;
                            var_r6_2 += 1;
                            var_r5_3 += 1;
                        }
                    }
                }
                var_r7_6 += 1;
                var_r4_2 += 4;
                var_r0_3 -= 4;
            } while ((u32) var_r7_6 < sp54);
        }
        var_r1_3 = sp24;
        var_r7_7 = 5;
        do {
            if (!(2 & M2C_FIELD(var_r1_3, u8 *, 2))) {
                M2C_FIELD(var_r1_3, s8 *, 3) = 6;
            }
            var_r7_7 -= 1;
            var_r1_3 += 4;
        } while (var_r7_7 >= 0);
        spAC[sp40] = 0xFF;
        Func_080b50e0(&spAC[0], 1);
        temp_r3_10 = sp1C[sp44];
        if ((u32) temp_r3_10 > 7U) {
            if (sp54 == 0xFF) {

            } else {
                temp_r8 = Func_08077008(sp1C[sp44]);
                Func_080b50b8(sp1C[sp44], &sp6C);
                var_r0_4 = Func_08002322(*(u32 *)0x03001E40 << 0xC);
                if (var_r0_4 < 0) {
                    var_r0_4 += 0x7FFF;
                }
                M2C_FIELD(&sp6C, s32 *, 4) = (s32) (M2C_FIELD(&sp6C, s32 *, 4) + (var_r0_4 >> 0xF));
                temp_r3_11 = M2C_FIELD(temp_r8, u8 *, 0x128);
                if ((temp_r3_11 != 0x7D) && (temp_r3_11 != 0x7A)) {
                    var_r7_8 = 0;
loop_236:
                    if (var_r7_8 <= 0xD) {
                        temp_r3_12 = *(temp_r8 + var_r7_8);
                        var_r2_5 = var_r7_8 * 2;
                        (&sp78[0])[var_r7_8] = (u16) temp_r3_12;
                        var_r7_8 += 1;
                        if (temp_r3_12 == 0) {

                        } else {
                            goto loop_236;
                        }
                    } else {
                        var_r2_5 = var_r7_8 * 2;
                    }
                    sp78[var_r7_8] = 0;
                } else {
                    var_r0_5 = 0x80E;
                    if (M2C_FIELD(temp_r8, u8 *, 0x128) == 0x7D) {
                        var_r0_5 = 0x80F;
                    }
                    Func_0801965c(var_r0_5, &sp78[0], 0xE);
                }
                temp_r0_3 = (u32) Func_08017a64(&sp78[0]);
                temp_r3_13 = (M2C_FIELD(&sp6C, s32 *, 0) - ((s32) (temp_r0_3 + (temp_r0_3 >> 0x1F)) >> 1)) - 8;
                M2C_FIELD(&sp6C, s32 *, 0) = temp_r3_13;
                if ((s32) (temp_r3_13 + temp_r0_3) > 0xE0) {
                    M2C_FIELD(&sp6C, s32 *, 0) = 0xE0 - temp_r0_3;
                }
                if ((s32) M2C_FIELD(&sp6C, s32 *, 0) < 0) {
                    M2C_FIELD(&sp6C, s32 *, 0) = 0;
                }
                Func_0801671c();
                Func_08017aa4((s16 *) &sp78[0], sp48, M2C_FIELD(&sp6C, s32 *, 0), 4);
            }
        } else if (sp54 == 0xFF) {

        } else if (sp50 == 0) {

        } else {
            temp_r6 = Func_08077008((s32) temp_r3_10);
            Func_080b50b8(sp1C[sp44], &sp98);
            if (0 != 0) {
                Func_08016418(0, 1);
            }
            temp_r3_14 = sp50 - 1;
            switch (temp_r3_14) {
            case 6:
                var_r3_6 = M2C_FIELD(&sp98, s32 *, 0);
                if (var_r3_6 < 0) {
                    var_r3_6 += 7;
                }
                temp_r3_15 = var_r3_6 >> 3;
                var_r0_6 = temp_r3_15 - 4;
                if ((s32) (temp_r3_15 + 4) > 0x1D) {
                    var_r0_6 = 0x16;
                }
                var_r9 = Func_080162d4(var_r0_6, 8U, 9U, 3U, 6);
                Func_0801e71c(2);
                var_r0_7 = 0x8AC;
block_225:
                Func_0801e7c0(var_r0_7, var_r9, 0, 0);
                Func_0801e71c(0xF);
                break;
            case 0:
                var_r3_7 = M2C_FIELD(&sp98, s32 *, 0);
                if (var_r3_7 < 0) {
                    var_r3_7 += 7;
                }
                temp_r3_16 = var_r3_7 >> 3;
                var_r0_8 = temp_r3_16 - 7;
                if ((s32) (temp_r3_16 + 6) > 0x1D) {
                    var_r0_8 = 0x11;
                }
                var_r9 = Func_080162d4(var_r0_8, 8U, 0xDU, 3U, 6);
                Func_0801e8b0(0x080373DC, (struct Work_02000000 *) var_r9, 0, 0);
                Func_0801ea08((s32) M2C_FIELD(temp_r6, s16 *, 0x38), 4, var_r9, 0x10, 0);
                Func_0801e8b0(0x080373E0, (struct Work_02000000 *) var_r9, 0x30, 0);
                var_r0_9 = M2C_FIELD(temp_r6, s16 *, 0x34);
block_140:
                Func_0801ea08((s32) var_r0_9, 4, var_r9, 0x38, 0);
                break;
            case 1:
                var_r3_8 = M2C_FIELD(&sp98, s32 *, 0);
                if (var_r3_8 < 0) {
                    var_r3_8 += 7;
                }
                temp_r3_17 = var_r3_8 >> 3;
                var_r0_10 = temp_r3_17 - 7;
                if ((s32) (temp_r3_17 + 6) > 0x1D) {
                    var_r0_10 = 0x11;
                }
                var_r9 = Func_080162d4(var_r0_10, 8U, 0xDU, 3U, 6);
                Func_0801e8b0(0x080373E4, (struct Work_02000000 *) var_r9, 0, 0);
                Func_0801ea08((s32) M2C_FIELD(temp_r6, s16 *, 0x3A), 4, var_r9, 0x10, 0);
                Func_0801e8b0(0x080373E0, (struct Work_02000000 *) var_r9, 0x30, 0);
                var_r0_9 = M2C_FIELD(temp_r6, s16 *, 0x36);
                goto block_140;
            case 4:
                var_r3_9 = M2C_FIELD(&sp98, s32 *, 0);
                if (var_r3_9 < 0) {
                    var_r3_9 += 7;
                }
                temp_r3_18 = var_r3_9 >> 3;
                var_r0_11 = temp_r3_18 - 7;
                if ((s32) (temp_r3_18 + 5) > 0x1D) {
                    var_r0_11 = 0x12;
                }
                var_r9 = Func_080162d4(var_r0_11, 8U, 0xCU, 3U, 6);
                if (M2C_FIELD(temp_r6, s16 *, 0x38) != 0) {
block_224:
                    Func_0801e71c(2);
                    var_r0_7 = 0x8A3;
                    goto block_225;
                }
                var_r0_12 = 0x8AB;
block_155:
                Func_0801e7c0(var_r0_12, var_r9, 0, 0);
                break;
            case 2:
                var_r3_10 = M2C_FIELD(&sp98, s32 *, 0);
                if (var_r3_10 < 0) {
                    var_r3_10 += 7;
                }
                temp_r3_19 = var_r3_10 >> 3;
                var_r0_13 = temp_r3_19 - 7;
                if ((s32) (temp_r3_19 + 5) > 0x1D) {
                    var_r0_13 = 0x12;
                }
                var_r9 = Func_080162d4(var_r0_13, 8U, 0xCU, 3U, 6);
                if ((s8) M2C_FIELD(temp_r6, u8 *, 0x131) == 0) {
                    goto block_224;
                }
                var_r0_12 = 0x8A4;
                goto block_155;
            case 3:
                var_r5_4 = 0;
                if (M2C_FIELD(temp_r6, u8 *, 0x138) != 0) {
                    var_r5_4 = 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x13B) != 0) {
                    var_r5_4 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x13C) != 0) {
                    var_r5_4 += 1;
                }
                sp28 = temp_r6 + 0x13D;
                if (M2C_FIELD(temp_r6, u8 *, 0x13D) != 0) {
                    var_r5_4 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x141) != 0) {
                    var_r5_4 += 1;
                }
                if (var_r5_4 == 0) {
                    var_r5_4 = 1;
                }
                var_r1_4 = 9 - var_r5_4;
                if (var_r1_4 <= 3) {
                    var_r1_4 = 4;
                }
                var_r3_11 = M2C_FIELD(&sp98, s32 *, 0);
                if (var_r3_11 < 0) {
                    var_r3_11 += 7;
                }
                temp_r3_20 = var_r3_11 >> 3;
                var_r0_14 = temp_r3_20 - 7;
                if ((s32) (temp_r3_20 + 9) > 0x1D) {
                    var_r0_14 = 0xE;
                }
                var_r9 = Func_080162d4(var_r0_14, (u16) var_r1_4, 0x10U, var_r5_4 + 2, 6);
                var_r5_5 = 0;
                if (M2C_FIELD(temp_r6, u8 *, 0x138) != 0) {
                    Func_0801e7c0(0x8A5, var_r9, 0, 0);
                    var_r5_5 = 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x13B) != 0) {
                    Func_0801e7c0(0x8A6, var_r9, 0, var_r5_5 * 8);
                    var_r5_5 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x13C) != 0) {
                    Func_0801e7c0(0x8A7, var_r9, 0, var_r5_5 * 8);
                    var_r5_5 += 1;
                }
                if (*sp28 != 0) {
                    Func_0801e7c0(0x8A8, var_r9, 0, var_r5_5 * 8);
                    var_r5_5 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x141) == 0) {

                } else {
                    var_r3_12 = var_r5_5 * 8;
                    var_r0_15 = 0x8A9;
block_221:
                    Func_0801e7c0(var_r0_15, var_r9, 0, var_r3_12);
                    var_r5_5 += 1;
                }
block_222:
                if (var_r5_5 != 0) {

                } else {
                    goto block_224;
                }
                break;
            case 5:
                var_r5_6 = 0;
                if ((s8) M2C_FIELD(temp_r6, u8 *, 0x131) != 0) {
                    var_r5_6 = 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x138) != 0) {
                    var_r5_6 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x13B) != 0) {
                    var_r5_6 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x13C) != 0) {
                    var_r5_6 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x13D) != 0) {
                    var_r5_6 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x141) != 0) {
                    var_r5_6 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x140) != 0) {
                    var_r5_6 += 1;
                }
                if (var_r5_6 == 0) {
                    var_r5_6 = 1;
                }
                var_r1_5 = 9 - var_r5_6;
                if (var_r1_5 <= 3) {
                    var_r1_5 = 4;
                }
                var_r3_13 = M2C_FIELD(&sp98, s32 *, 0);
                if (var_r3_13 < 0) {
                    var_r3_13 += 7;
                }
                temp_r3_21 = var_r3_13 >> 3;
                var_r0_16 = temp_r3_21 - 7;
                if ((s32) (temp_r3_21 + 9) > 0x1D) {
                    var_r0_16 = 0xE;
                }
                var_r9 = Func_080162d4(var_r0_16, (u16) var_r1_5, 0x10U, var_r5_6 + 2, 6);
                var_r5_5 = 0;
                if ((s8) M2C_FIELD(temp_r6, u8 *, 0x131) != 0) {
                    Func_0801e7c0(0x8A4, var_r9, 0, 0);
                    var_r5_5 = 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x138) != 0) {
                    Func_0801e7c0(0x8A5, var_r9, 0, var_r5_5 * 8);
                    var_r5_5 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x13B) != 0) {
                    Func_0801e7c0(0x8A6, var_r9, 0, var_r5_5 * 8);
                    var_r5_5 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x13C) != 0) {
                    Func_0801e7c0(0x8A7, var_r9, 0, var_r5_5 * 8);
                    var_r5_5 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x13D) != 0) {
                    Func_0801e7c0(0x8A8, var_r9, 0, var_r5_5 * 8);
                    var_r5_5 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x141) != 0) {
                    Func_0801e7c0(0x8A9, var_r9, 0, var_r5_5 * 8);
                    var_r5_5 += 1;
                }
                if (M2C_FIELD(temp_r6, u8 *, 0x140) != 0) {
                    var_r3_12 = var_r5_5 * 8;
                    var_r0_15 = 0x8AA;
                    goto block_221;
                }
                goto block_222;
            }
        }
        sp38 &= ~1;
    }
    if (sp30 == 0) {

    } else {
        var_r7_9 = 1;
        if (sp40 <= 1) {

        } else {
            sp10 = &subroutine_arg0 + 0xA4;
            spC = 2;
            var_r5_7 = sp20 + 0xC;
            var_r4_3 = &sp60;
loop_252:
            sp4 = sp24 + (*(sp10 + var_r7_9) * 4);
            sp8 = var_r4_3;
            Func_080b50b8(spAC[spC / 2], var_r4_3);
            var_r0_17 = Func_08002322(*(u32 *)0x03001E40 << 0xC);
            if (var_r0_17 < 0) {
                var_r0_17 += 0x7FFF;
            }
            M2C_FIELD(var_r4_3, s32 *, 4) = (s32) (M2C_FIELD(var_r4_3, s32 *, 4) + (var_r0_17 >> 0xF));
            M2C_FIELD(var_r5_7, s32 *, 0) = M2C_FIELD(sp20, s32 *, 0);
            M2C_FIELD(var_r5_7, s32 *, 4) = (s32) M2C_FIELD(sp20, s32 *, 4);
            M2C_FIELD(var_r5_7, s32 *, 8) = (s32) M2C_FIELD(sp20, s32 *, 8);
            if (1 & M2C_FIELD(sp4, u8 *, 2)) {
                temp_r1_5 = M2C_FIELD(var_r4_3, s32 *, 0) + M2C_FIELD(sp4, u8 *, 0);
                temp_r3_22 = M2C_FIELD(var_r4_3, s32 *, 4) + M2C_FIELD(sp4, u8 *, 1);
                temp_r1_6 = (s32) (temp_r1_5 + (temp_r1_5 >> 0x1F)) >> 1;
                temp_r3_23 = (s32) (temp_r3_22 + (temp_r3_22 >> 0x1F)) >> 1;
                M2C_FIELD(var_r4_3, s32 *, 0) = temp_r1_6;
                M2C_FIELD(sp4, u8 *, 0) = (u8) temp_r1_6;
                M2C_FIELD(var_r4_3, s32 *, 4) = temp_r3_23;
                M2C_FIELD(sp4, u8 *, 1) = (u8) temp_r3_23;
            } else {
                temp_r3_24 = (u32) (M2C_FIELD(var_r5_7, u16 *, 6) << 0x17) >> 0x17;
                temp_r2_6 = (u8) M2C_FIELD(var_r5_7, s32 *, 4) + 8;
                M2C_FIELD(sp4, u8 *, 2) = 1U;
                M2C_FIELD(var_r4_3, s32 *, 0) = (s32) temp_r3_24;
                M2C_FIELD(sp4, u8 *, 0) = (u8) temp_r3_24;
                M2C_FIELD(var_r4_3, s32 *, 4) = temp_r2_6;
                M2C_FIELD(sp4, u8 *, 1) = (u8) temp_r2_6;
            }
            temp_r0_4 = (M2C_FIELD(var_r5_7, u8 *, 5) & ~0xC) | 4;
            M2C_FIELD(var_r5_7, u16 *, 6) = (u16) ((M2C_FIELD(var_r5_7, u16 *, 6) & 0xFFFFFE00) | ((M2C_FIELD(&sp60, s32 *, 0) - 8) & 0x1FF));
            M2C_FIELD(var_r5_7, u8 *, 5) = temp_r0_4;
            M2C_FIELD(var_r5_7, s32 *, 4) = (s8) (M2C_FIELD(&sp60, s32 *, 4) - 0xC);
            if (sp54 == 0xFF) {
                var_r0_18 = temp_r0_4 & ~3;
            } else {
                var_r0_18 = (temp_r0_4 & ~3) | 1;
            }
            M2C_FIELD(var_r5_7, u8 *, 5) = var_r0_18;
            M2C_FIELD(var_r5_7, u8 *, 7) = (u8) ((M2C_FIELD(var_r5_7, u8 *, 7) & ~0x3E) | ((sp2C & 0x1F) * 2));
            sp8 = var_r4_3;
            Func_08003dec(var_r5_7, 0xF0);
            var_r7_9 += 1;
            var_r5_7 += 0xC;
            spC += 2;
            if (var_r7_9 < sp40) {
                goto loop_252;
            }
        }
    }
    var_r6_3 = *(s32 *)0x03001C94;
    var_r5_8 = *(s32 *)0x03001B04;
    temp_r2_7 = *(void **)0x03001F34;
    if (M2C_FIELD(temp_r2_7, s32 *, 0xD8) != 0) {
        temp_r3_25 = M2C_FIELD(temp_r2_7, s32 *, 0xDC);
        var_r5_8 = 0;
        var_r6_3 = 0;
        if (temp_r3_25 == 0) {
            M2C_FIELD(temp_r2_7, s32 *, 0xDC) = 0x3C;
            var_r5_8 = 1;
            var_r6_3 = 1;
        } else {
            M2C_FIELD(temp_r2_7, s32 *, 0xDC) = (s32) (temp_r3_25 - 1);
        }
    }
    if (1 & var_r6_3) {
        temp_r4_2 = sp1C[sp44];
        sp38 = 0;
        temp_r3_26 = M2C_FIELD(sp4C, s16 *, 0x58);
        var_r0_19 = -1;
        var_r1_6 = 0;
        if (temp_r3_26 != 0xFF) {
            if (temp_r3_26 == temp_r4_2) {
                var_r0_19 = 0x100;
            } else {
loop_270:
                var_r1_6 += 1;
                if (var_r1_6 <= 5) {
                    temp_r3_27 =
                        *(s16 *)(sp4C + ((var_r1_6 * 2) + 0x58));
                    if (temp_r3_27 != 0xFF) {
                        if (temp_r3_27 == temp_r4_2) {
                            var_r0_19 = 0x100 | var_r1_6;
                        } else {
                            goto loop_270;
                        }
                    }
                }
            }
        }
        if (var_r0_19 < 0) {
            var_r2_6 = sp4C + 0x66;
            temp_r3_28 = M2C_FIELD(sp4C, s16 *, 0x66);
            var_r1_7 = 0;
            if (temp_r3_28 != 0xFF) {
                if (temp_r3_28 == temp_r4_2) {
                    var_r0_19 = 0x180;
                } else {
loop_278:
                    var_r1_7 += 1;
                    var_r2_6 += 1;
                    if (var_r1_7 <= 5) {
                        temp_r3_29 = *var_r2_6;
                        if (temp_r3_29 != 0xFF) {
                            if (temp_r3_29 == temp_r4_2) {
                                var_r0_19 = 0x180 | var_r1_7;
                            } else {
                                goto loop_278;
                            }
                        }
                    }
                }
            }
        }
        sp44 = var_r0_19;
    } else if (sp54 != 0xFF) {
        if (0x90 & var_r5_8) {
            Func_080f9010(0x6F);
            do {
                temp_r1_7 = sp44 + 1;
                sp44 = temp_r1_7;
                temp_r0_5 = Func_080022fc(temp_r1_7, sp3C);
                sp44 = temp_r0_5;
            } while (sp1C[temp_r0_5] == 0xFE);
            sp38 |= 1;
        }
        if (0x60 & var_r5_8) {
            Func_080f9010(0x6F);
            do {
                temp_r3_30 = (sp44 + sp3C) - 1;
                sp44 = temp_r3_30;
                temp_r0_6 = Func_080022fc(temp_r3_30, sp3C);
                sp44 = temp_r0_6;
            } while (sp1C[temp_r0_6] == 0xFE);
            sp38 |= 1;
        }
    }
    if ((M2C_FIELD(*(u8 **)0x03001F34, s32 *, 0x4C) == 0) ||
        (2 & var_r6_3)) {
        Func_080f9010(0x71);
        sp44 = -1;
    } else {
        Func_080030f8(1U);
        if (sp38 != 0) {
            Func_08026388();
        }
    }
    Func_080030f8(1U);
    Func_08003f3c((u32) sp34);
    if (var_r9 != 0) {
        Func_08016418(var_r9, 1);
    }
    Func_08016418(sp48, 1);
    Func_080b50e0(sp1C, 0);
    M2C_FIELD(*(u8 **)0x03001F34, s32 *, 0x28) = 0;
    Func_080030f8(1U);
    return sp44;
}
