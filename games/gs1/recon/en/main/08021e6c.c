#include "types.h"

/* Owner-local field access until the UI workspace layout is recovered. */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
#define Ui_RunSelectionScreen Func_08021e6c

void Ui_RunSelectionScreen(s32 mode) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 *sp10;
    s32 sp14;
    s32 *sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    void *sp30;
    s32 sp34;
    u8 sp38;
    s16 *temp_r3_1104;
    s16 *temp_r3_435;
    s16 *temp_r3_571;
    s16 *var_r5_223;
    s16 temp_r7_23;
    s32 *var_r1_274;
    s32 *var_r6_224;
    u8 *temp_r0_30;
    s32 temp_r1_490;
    s32 temp_r1_631;
    s32 temp_r1_710;
    s32 temp_r1_848;
    s32 temp_r2_238;
    s32 temp_r2_281;
    s32 temp_r2_475;
    s32 temp_r2_616;
    s32 temp_r3_462;
    s32 temp_r3_603;
    s32 temp_r3_854;
    s32 temp_r3_861;
    s32 temp_r3_893;
    s32 temp_r4_171;
    s32 temp_r5_195;
    s32 var_lr_400;
    s32 var_lr_453;
    s32 var_lr_539;
    s32 var_lr_596;
    s32 var_r0_272;
    s32 var_r0_485;
    s32 var_r0_51;
    s32 var_r0_626;
    s32 var_r0_906;
    s32 var_r2_1096;
    s32 var_r2_422;
    s32 var_r2_54;
    s32 var_r2_563;
    s32 var_r2_769;
    s32 var_r3_330;
    s32 var_r3_787;
    s32 var_r3_881;
    s32 var_r4_1085;
    s32 var_r4_412;
    s32 var_r4_553;
    s32 var_r5_1086;
    s32 var_r5_158;
    s32 var_r5_482;
    s32 var_r5_623;
    s32 var_r5_842;
    s32 var_r6_481;
    s32 var_r6_622;
    s32 var_r7_845;
    s32 var_r8_1051;
    s32 var_r8_1093;
    s32 var_r8_222;
    s32 var_r8_265;
    s32 var_r8_419;
    s32 var_r8_479;
    s32 var_r8_560;
    s32 var_r8_620;
    s32 var_r8_731;
    s8 *var_r3_34;
    u16 *var_r5_1059;
    u16 temp_r1_328;
    u16 temp_r2_1137;
    u16 temp_r4_1135;
    u16 temp_r6_344;
    u8 *var_r5_161;
    u8 temp_r3_168;
    void *temp_r2_844;
    void *temp_r3_1144;
    void *temp_r3_1147;
    void *var_r0_420;
    void *var_r0_561;
    void *var_r1_1094;
    void *var_r4_480;
    void *var_r4_621;
    void *var_r5_739;

    sp34 = mode;
    sp30 = *(void **)0x03001E8C;
    temp_r7_23 = Func_080040b4(0x400);
    sp2C = 1;
    sp28 = 0;
    temp_r0_30 = (u8 *)Func_08004970(0x1E0);
    var_r3_34 = temp_r0_30 + 0xFF;
    do {
        *var_r3_34 = 0xFF;
        var_r3_34 -= 1;
    } while ((s32) var_r3_34 >= temp_r0_30);
    M2C_FIELD(temp_r0_30, s32 *, 0x100) = 1;
    if (sp34 == 0) {
        sp0 = 6;
        var_r0_51 = 0x14;
        var_r2_54 = 0xA;
    } else {
        sp0 = 6;
        var_r0_51 = 0x16;
        var_r2_54 = 8;
    }
    M2C_FIELD(temp_r0_30, s32 *, 0x1AC) = Func_080162d4(var_r0_51, 0x11, var_r2_54, 3);
    M2C_FIELD(temp_r0_30, s32 *, 0x1C4) = -1;
    if (sp34 == 0) {
        M2C_FIELD(temp_r0_30, s32 *, 0x1C8) = 0xE;
        M2C_FIELD(temp_r0_30, s32 *, 0x1CC) = 4;
        M2C_FIELD(temp_r0_30, s32 *, 0x1D0) = 7;
        M2C_FIELD(temp_r0_30, s32 *, 0x1D4) = -1;
        M2C_FIELD(temp_r0_30, s32 *, 0x1C0) = (s32) M2C_FIELD(*(void **)0x03001F34, s32 *, 0x3C);
    } else {
        M2C_FIELD(temp_r0_30, s32 *, 0x1C0) = (s32) M2C_FIELD(*(void **)0x03001F34, s32 *, 0x40);
        M2C_FIELD(temp_r0_30, s32 *, 0x1C8) = 0;
        M2C_FIELD(temp_r0_30, s32 *, 0x1CC) = 1;
        var_r5_158 = 2;
        if (*(u8 *)Func_08077000(0) != 0) {
            var_r5_161 = &sp38;
            Func_080b5130(0, var_r5_161);
            do {
                temp_r3_168 = *var_r5_161;
                var_r5_161 += 1;
                temp_r4_171 = sp28 + temp_r3_168;
                sp28 = temp_r4_171;
            } while ((s32) var_r5_161 <= (s32) (&sp0 + 0x3B));
            M2C_FIELD(temp_r0_30, s32 *, 0x1D0) = 0xF;
            var_r5_158 = 3;
            if (temp_r4_171 != 0) {
                M2C_FIELD(temp_r0_30, s32 *, 0x1D4) = 0x10;
                var_r5_158 = 4;
            }
        }
        temp_r5_195 = var_r5_158 + 1;
        *(s32 *)(temp_r0_30 + ((var_r5_158 * 4) + 0x1C8)) = 2;
        *(s32 *)(temp_r0_30 + ((temp_r5_195 * 4) + 0x1C8)) = 3;
        *(s32 *)(temp_r0_30 + (((temp_r5_195 + 1) * 4) + 0x1C8)) = -1;
    }
    M2C_FIELD(temp_r0_30, s16 *, 0x110) = temp_r7_23;
    var_r8_222 = 0;
    var_r5_223 = temp_r0_30 + 0x12C;
    var_r6_224 = temp_r0_30 + 0x1CC;
    if (M2C_FIELD(temp_r0_30, s32 *, 0x1C8) != -1) {
loop_18:
        Func_08021d88(temp_r0_30, var_r8_222);
        var_r8_222 += 1;
        if (var_r8_222 <= 5) {
            *var_r5_223 = temp_r7_23;
            temp_r2_238 = *var_r6_224;
            var_r6_224 += 4;
            var_r5_223 += 0x1C;
            if (temp_r2_238 != -1) {
                goto loop_18;
            }
        }
    }
    M2C_FIELD(temp_r0_30, s32 *, 0x1B0) = var_r8_222;
    M2C_FIELD(temp_r0_30, u16 *, 0x1B4) = 0x140U;
    M2C_FIELD(temp_r0_30, u16 *, 0x1B6) = 0x130U;
    M2C_FIELD(temp_r0_30, s16 *, 0x1B8) = 0;
    var_r8_265 = 0;
    if ((s32) M2C_FIELD(temp_r0_30, s32 *, 0x1B0) > 0) {
        var_r0_272 = 0x114;
        var_r1_274 = temp_r0_30 + 0x118;
        do {
            temp_r2_281 = (var_r8_265 - M2C_FIELD(temp_r0_30, s32 *, 0x1B0)) * 0x18;
            *(s32 *)(var_r0_272 + temp_r0_30) = temp_r2_281 + 0x9B;
            if (sp34 != 0) {
                *(s32 *)(var_r0_272 + temp_r0_30) = temp_r2_281 + 0xAB;
            }
            *var_r1_274 = 0x88;
            var_r8_265 += 1;
            var_r1_274 += 0x1C;
            var_r0_272 += 0x1C;
        } while (var_r8_265 < (s32) M2C_FIELD(temp_r0_30, s32 *, 0x1B0));
    }
    Func_080041d8(0x08021DFD, 0x480);
    Func_0800307c(2, 0x88, 0x08021E15);
    sp10 = temp_r0_30 + 0x1B0;
    sp18 = temp_r0_30 + 0x1C0;
loop_26:
    temp_r1_328 = *(u16 *)(0x080366F8 + (((*(s32 *)0x03001E40 * 2) & 0x1F) * 2));
    var_r3_330 = temp_r1_328 + 0xFFFFFF00;
    sp24 = (s32) temp_r1_328;
    if (var_r3_330 < 0) {
        var_r3_330 = temp_r1_328 - 0xFD;
    }
    sp24 = (var_r3_330 >> 2) + 0x130;
    temp_r6_344 = sp24;
    M2C_FIELD(temp_r0_30, u16 *, 0x1B4) = temp_r6_344;
    M2C_FIELD(temp_r0_30, u16 *, 0x1B6) = temp_r6_344;
    M2C_FIELD((void *)0x03001AD0, s16 *, 4) = 0;
    M2C_FIELD((void *)0x03001AD0, s16 *, 6) = 0x20;
    if (sp2C == 0) {

    } else {
        sp2C = 0;
        M2C_FIELD(sp30, s8 *, 0xEA6) = 1;
        if (sp34 == 0) {
            var_lr_539 = 0;
            if ((s32) (6 - *sp10) > 0) {
                var_r4_553 = 0;
                do {
                    var_r8_560 = 0;
                    var_r0_561 = ((var_r4_553 + var_lr_539) * 2) + sp30;
loop_54:
                    var_r2_563 = 0;
loop_55:
                    temp_r3_571 = ((var_r2_563 & 3) * 2) + var_r0_561 + 0x444;
                    var_r2_563 += 1;
                    *temp_r3_571 = 0xF07F;
                    if (var_r2_563 <= 2) {
                        goto loop_55;
                    }
                    var_r8_560 += 1;
                    var_r0_561 += 0x40;
                    if (var_r8_560 <= 2) {
                        goto loop_54;
                    }
                    var_lr_539 += 1;
                    var_r4_553 += 2;
                } while (var_lr_539 < (s32) (6 - M2C_FIELD(temp_r0_30, s32 *, 0x1B0)));
            }
            var_lr_596 = 0;
            if ((s32) *sp10 > 0) {
                temp_r3_603 = M2C_FIELD(temp_r0_30, s32 *, 0x1B0);
                sp1C = temp_r3_603 * 6;
                spC = 0;
                do {
                    temp_r2_616 = spC + var_lr_596;
                    var_r8_620 = 0;
                    var_r4_621 = (temp_r2_616 * 2) + sp30;
                    var_r6_622 = temp_r2_616;
                    var_r5_623 = var_lr_596 * 0x10;
loop_63:
                    sp4 = var_r5_623;
                    var_r0_626 = 0;
loop_64:
                    temp_r1_631 = var_r0_626 & 3;
                    *(s16 *)(0x0600FD68 + (((var_r6_622 + temp_r1_631) - (temp_r3_603 * 3)) * 2)) = (s16) (sp4 + var_r0_626 + 0x100);
                    var_r0_626 += 1;
                    M2C_FIELD(((var_r4_621 + (temp_r1_631 * 2)) - sp1C), s16 *, 0x468) = 0;
                    if (var_r0_626 <= 2) {
                        goto loop_64;
                    }
                    var_r8_620 += 1;
                    var_r4_621 += 0x40;
                    var_r6_622 += 0x20;
                    var_r5_623 += 4;
                    if (var_r8_620 <= 2) {
                        goto loop_63;
                    }
                    var_lr_596 += 1;
                    spC += 2;
                } while (var_lr_596 < temp_r3_603);
            }
        } else {
            var_lr_400 = 0;
            if ((s32) (6 - *sp10) > 0) {
                var_r4_412 = 0;
                do {
                    var_r8_419 = 0;
                    var_r0_420 = ((var_r4_412 + var_lr_400) * 2) + sp30;
loop_37:
                    var_r2_422 = 0;
loop_39:
                    temp_r3_435 = ((var_r2_422 & 3) * 2) + var_r0_420 + 0x448;
                    var_r2_422 += 1;
                    *temp_r3_435 = 0xF07F;
                    if (var_r2_422 <= 2) {
                        goto loop_39;
                    }
                    var_r8_419 += 1;
                    var_r0_420 += 0x40;
                    if (var_r8_419 <= 2) {
                        goto loop_37;
                    }
                    var_lr_400 += 1;
                    var_r4_412 += 2;
                } while (var_lr_400 < (s32) (6 - M2C_FIELD(temp_r0_30, s32 *, 0x1B0)));
            }
            var_lr_453 = 0;
            if ((s32) *sp10 <= 0) {

            } else {
                temp_r3_462 = M2C_FIELD(temp_r0_30, s32 *, 0x1B0);
                sp20 = temp_r3_462 * 6;
                sp8 = 0;
                do {
                    temp_r2_475 = sp8 + var_lr_453;
                    var_r8_479 = 0;
                    var_r4_480 = (temp_r2_475 * 2) + sp30;
                    var_r6_481 = temp_r2_475;
                    var_r5_482 = var_lr_453 * 0x10;
loop_46:
                    sp4 = var_r5_482;
                    var_r0_485 = 0;
loop_47:
                    temp_r1_490 = var_r0_485 & 3;
                    *(s16 *)(0x0600FD6C + (((var_r6_481 + temp_r1_490) - (temp_r3_462 * 3)) * 2)) = (s16) (sp4 + var_r0_485 + 0x100);
                    var_r0_485 += 1;
                    M2C_FIELD(((var_r4_480 + (temp_r1_490 * 2)) - sp20), s16 *, 0x46C) = 0;
                    if (var_r0_485 <= 2) {
                        goto loop_47;
                    }
                    var_r8_479 += 1;
                    var_r4_480 += 0x40;
                    var_r6_481 += 0x20;
                    var_r5_482 += 4;
                    if (var_r8_479 <= 2) {
                        goto loop_46;
                    }
                    var_lr_453 += 1;
                    sp8 += 2;
                } while (var_lr_453 < temp_r3_462);
            }
        }
    }
    if (M2C_FIELD(temp_r0_30, s32 *, 0x1C4) != M2C_FIELD(temp_r0_30, s32 *, 0x1C0)) {
        Func_08016478(M2C_FIELD(temp_r0_30, s32 *, 0x1AC));
        Func_0801e7c0(*(s32 *)(temp_r0_30 + ((M2C_FIELD(temp_r0_30, s32 *, 0x1C0) * 0x1C) + 0x11C)) + 0x1F, M2C_FIELD(temp_r0_30, s32 *, 0x1AC), 0, 0);
        M2C_FIELD(temp_r0_30, s32 *, 0x1C4) = (s32) M2C_FIELD(temp_r0_30, s32 *, 0x1C0);
        temp_r1_710 = M2C_FIELD(temp_r0_30, s32 *, 0x1C0);
        Func_08021d88(temp_r0_30, temp_r1_710, *(s32 *)(temp_r0_30 + ((temp_r1_710 * 4) + 0x1C8)));
    }
    M2C_FIELD(temp_r0_30, s32 *, 0x1BC) = Func_08003d28(temp_r0_30 + 0x1B4);
    var_r8_731 = 0;
    if ((s32) *sp10 > 0) {
        var_r5_739 = temp_r0_30 + 0x104;
        do {
            if (var_r8_731 == *sp18) {
                M2C_FIELD(var_r5_739, u8 *, 7) = (u8) ((M2C_FIELD(var_r5_739, u8 *, 7) & ~0x3E) | (((u8) M2C_FIELD(temp_r0_30, s32 *, 0x1BC) & 0x1F) * 2));
                M2C_FIELD(var_r5_739, u8 *, 5) = (u8) (M2C_FIELD(var_r5_739, u8 *, 5) | 3);
                var_r2_769 = sp24 * 7;
                if (var_r2_769 < 0) {
                    var_r2_769 += 0x1FF;
                }
                M2C_FIELD(var_r5_739, u16 *, 6) = (u16) ((M2C_FIELD(var_r5_739, u16 *, 6) & 0xFFFFFE00) | (((M2C_FIELD(var_r5_739, s32 *, 0x10) + (var_r2_769 >> 9)) - 0xE) & 0x1FF));
                var_r3_787 = sp24 * 3;
                if (var_r3_787 < 0) {
                    var_r3_787 += 0xFF;
                }
                M2C_FIELD(var_r5_739, s8 *, 4) = (s8) ((M2C_FIELD(var_r5_739, s32 *, 0x14) + (var_r3_787 >> 8)) - 0x14);
                Func_08003dec(var_r5_739, 0xF1);
            } else {
                M2C_FIELD(var_r5_739, u16 *, 6) = (u16) ((M2C_FIELD(var_r5_739, u16 *, 6) & 0xFFFFFE00) | (M2C_FIELD(var_r5_739, s32 *, 0x10) & 0x1FF));
                M2C_FIELD(var_r5_739, s8 *, 4) = (s8) M2C_FIELD(var_r5_739, s32 *, 0x14);
                M2C_FIELD(var_r5_739, u8 *, 7) = (u8) (M2C_FIELD(var_r5_739, u8 *, 7) & ~0x3E);
                M2C_FIELD(var_r5_739, u8 *, 5) = (u8) (M2C_FIELD(var_r5_739, u8 *, 5) & ~3);
            }
            var_r8_731 += 1;
            var_r5_739 += 0x1C;
        } while (var_r8_731 < (s32) *sp10);
    }
    var_r5_842 = *(s32 *)0x03001C94;
    temp_r2_844 = *(void **)0x03001F34;
    var_r7_845 = *(s32 *)0x03001B04;
    temp_r1_848 = M2C_FIELD(temp_r2_844, s32 *, 0xD8);
    if (temp_r1_848 != 0) {
        temp_r3_854 = M2C_FIELD(temp_r2_844, s32 *, 0xDC);
        var_r7_845 = 0;
        var_r5_842 = 0;
        if (temp_r3_854 == 0) {
            temp_r3_861 = M2C_FIELD(temp_r2_844, s32 *, 0xE0);
            if (temp_r3_861 == 1) {
                if (*(s32 *)(temp_r0_30 + ((*sp18 * 4) + 0x1C8)) == 3) {
                    var_r7_845 = 1;
                    var_r5_842 = 1;
                } else {
                    var_r7_845 = 0x20;
                    var_r5_842 = 0x20;
                }
                var_r3_881 = 0x1E;
                goto block_106;
            }
            if (temp_r3_861 == 0) {
                temp_r3_893 = *(s32 *)(temp_r0_30 + ((*sp18 * 4) + 0x1C8));
                if ((temp_r3_893 == 0x10) || ((sp28 == 0) && (temp_r3_893 == 0xF))) {
                    if (temp_r1_848 == 1) {
                        if (temp_r3_893 == 0xF) {
                            var_r0_906 = 0xC4A;
                            goto block_100;
                        }
                        if (temp_r3_893 == 0x10) {
                            var_r0_906 = 0xC49;
block_100:
                            sp14 = Func_08021e48(var_r0_906, 0xF, 8);
                        }
                        Func_080b5128(0x66, 0x9B);
                        Func_08016418(sp14, 1);
                        M2C_FIELD(temp_r2_844, s32 *, 0xD8) = (s32) (M2C_FIELD(temp_r2_844, s32 *, 0xD8) + 1);
                        var_r3_881 = 0x2D;
                    } else {
                        var_r3_881 = 0xC8;
                        var_r7_845 = 1;
                        var_r5_842 = 1;
                    }
                } else {
                    var_r3_881 = 0x28;
                    var_r7_845 = 0x10;
                    var_r5_842 = 0x10;
                }
                goto block_106;
            }
            M2C_FIELD(temp_r2_844, s32 *, 0xDC) = 0x3C;
            var_r7_845 = 1;
            var_r5_842 = 1;
        } else {
            var_r3_881 = temp_r3_854 - 1;
block_106:
            M2C_FIELD(temp_r2_844, s32 *, 0xDC) = var_r3_881;
        }
    }
    if ((0x300 & var_r5_842) && (sp34 != 0)) {

    } else if (1 & var_r5_842) {

    } else if ((sp34 != 0) && (2 & var_r5_842)) {
        Func_080f9010(0x71);
    } else {
        if (0x90 & var_r7_845) {
            Func_080f9010(0x6F);
            *sp18 = Func_080022fc(*sp18 + 1, *sp10);
            goto block_121;
        }
        if (0x60 & var_r7_845) {
            Func_080f9010(0x6F);
            *sp18 = Func_080022fc((*sp18 + *sp10) - 1);
            goto block_121;
        }
        if (M2C_FIELD(*(void **)0x03001F34, s32 *, 0x4C) == 0) {

        } else {
block_121:
            Func_0800387c(0x04000000, 0x1741);
            M2C_FIELD(sp30, s8 *, 0xEA6) = 0;
            Func_080030f8(1);
            goto loop_26;
        }
    }
    if (sp34 != 0) {
        M2C_FIELD(*(void **)0x03001F34, s32 *, 0x40) = (s32) M2C_FIELD(temp_r0_30, s32 *, 0x1C0);
    } else {
        M2C_FIELD(*(void **)0x03001F34, s32 *, 0x3C) = (s32) M2C_FIELD(temp_r0_30, s32 *, 0x1C0);
    }
    var_r8_1051 = 0;
    if ((s32) M2C_FIELD(temp_r0_30, s32 *, 0x1B0) > 0) {
        var_r5_1059 = temp_r0_30 + 0x110;
        do {
            Func_08003f3c(*var_r5_1059);
            var_r8_1051 += 1;
            var_r5_1059 += 0x1C;
        } while (var_r8_1051 < (s32) M2C_FIELD(temp_r0_30, s32 *, 0x1B0));
    }
    M2C_FIELD(sp30, s8 *, 0xEA6) = 1;
    Func_08016418(M2C_FIELD(temp_r0_30, s32 *, 0x1AC), 1);
    var_r4_1085 = 0;
    var_r5_1086 = 0;
    do {
        var_r8_1093 = 0;
        var_r1_1094 = ((var_r5_1086 + var_r4_1085) * 2) + sp30;
loop_130:
        var_r2_1096 = 0;
loop_131:
        temp_r3_1104 = ((var_r2_1096 & 3) * 2) + var_r1_1094 + 0x44A;
        var_r2_1096 += 1;
        *temp_r3_1104 = 0xF07F;
        if (var_r2_1096 <= 2) {
            goto loop_131;
        }
        var_r8_1093 += 1;
        var_r1_1094 += 0x40;
        if (var_r8_1093 <= 2) {
            goto loop_130;
        }
        var_r4_1085 += 1;
        var_r5_1086 += 2;
    } while (var_r4_1085 <= 6);
    M2C_FIELD(sp30, s8 *, 0xEA3) = 1;
    Func_080030f8();
    Func_08004278(0x08021DFD);
    Func_0800307c(2, 0, 0);
    temp_r4_1135 = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0x04000208;
    temp_r2_1137 = *(u16 *)0x02002090;
    if ((s32) temp_r2_1137 <= 0x1F) {
        temp_r3_1144 = (temp_r2_1137 * 0xC) + 0x02002090;
        *(u16 *)0x02002090 = temp_r2_1137 + 1;
        temp_r3_1147 = temp_r3_1144 + 4;
        M2C_FIELD(temp_r3_1144, s32 *, 4) = 0x1541;
        M2C_FIELD(temp_r3_1147, s32 *, 4) = 0x04000000;
        M2C_FIELD((temp_r3_1147 + 4), s32 *, 4) = 0x20000;
    }
    *(u16 *)0x04000208 = temp_r4_1135;
    Func_08002df0(temp_r0_30);
    M2C_FIELD(sp30, s8 *, 0xEA6) = 0;
    Func_080030f8(1);
}
