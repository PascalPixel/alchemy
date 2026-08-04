#include "types.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

u16 Func_080022f4(u16, s32);
u16 Func_08002304(u16, s32);
s32 Func_08015038(s32, s32, s32, s32);
s32 Func_08015048(void);
void *Func_08077008(s32);
s32 Func_08077208(u32, u32, s32);
s32 Func_08077210(u32, u32, s32);
s32 Func_080aa538(s32, s32);
void Func_080030f8(u32);
void Func_08015018(s32, s32);
void Func_08015068(void *, s32, s32, s32, s32);
void Func_08015078(s32, s32, s32, s32);
void Func_08015080(s32, void *, s32, s32);
void Func_08015090();
void Func_080150b0();
void Func_08015120(s32, s32);
void Func_08015270();
void Func_08015278(void *);
void Func_08015280();
void Func_08077010(u32);
void Func_080771b0(u32, u32, s32);
void Func_080771b8(u32, u32, s32);
void Func_080771c0(u32, u32, s32);
void Func_080771c8(u32, u32, s32);
void Func_080a1a40(s32, s32);
void Func_080a1ac0(s32, s32);
void Func_080aafb8(void *);
void Func_080ab1f4();
void Func_080ab2ec();
void Func_080ad5b4(s32, s32, s32, s32);
void Func_080ad5f4(s32, s32);
void Func_080ad608(s32, s32, s32);
void Func_080ae714(void *, s32);
void Func_080b50f8(void);
void Func_080f9010(s32);

/*
 * Interactive two-pane selection/menu owner.
 *
 * The 0x080ab834 entry is a re-entry into this function's live frame, not an
 * independent C ABI.  Calls to that address are expressed below as jumps back
 * to `redraw_entry`, which preserves the original stack-owned state.
 */
s32 Func_080ab5e4(s32 arg0) {
    u32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s8 *sp1C;
    s32 sp20;
    s32 sp24;
    u32 sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    s32 sp3C;
    s32 sp40;
    s32 sp44;
    s32 sp48;
    void *sp4C;
    s32 sp50;
    s8 sp54;
    s32 *var_r2_3;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r0_5;
    s32 temp_r0_6;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r1_4;
    s32 temp_r1_5;
    s32 temp_r1_6;
    s32 temp_r2;
    s32 temp_r3_2;
    s32 temp_r3_4;
    s32 temp_r3_5;
    s32 temp_r3_6;
    s32 temp_r3_7;
    s32 temp_r3_8;
    s32 temp_r3_9;
    s32 temp_r5_11;
    s32 temp_r5_12;
    s32 temp_r5_15;
    s32 temp_r5_2;
    s32 temp_r5_3;
    void *temp_r5_4;
    s32 temp_r6_2;
    s32 temp_r6_3;
    s32 temp_r6_6;
    s32 temp_r6_7;
    s32 temp_r7;
    s32 temp_r7_2;
    s32 temp_r7_3;
    s32 var_fp;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r3_3;
    s32 var_r3_4;
    s32 var_r5_2;
    s32 var_r5_3;
    s32 var_r6;
    s32 var_r8;
    s8 *var_r0;
    s8 *var_r1;
    s8 *var_r1_2;
    s8 *var_r2_2;
    s8 *var_r3;
    s8 var_r1_3;
    s8 var_r1_4;
    s8 var_r1_5;
    u16 *var_sl;
    u16 temp_r0;
    u16 temp_r2_2;
    u16 temp_r2_5;
    u16 temp_r5;
    u16 temp_r6;
    u16 temp_r7_6;
    u32 temp_r3_3;
    u32 temp_r4;
    u32 temp_r5_10;
    u32 temp_r5_13;
    u32 temp_r5_14;
    u32 temp_r5_5;
    u32 temp_r5_6;
    u32 temp_r5_7;
    u32 temp_r5_8;
    u32 temp_r5_9;
    u32 temp_r6_10;
    u32 temp_r6_11;
    u32 temp_r6_12;
    u32 temp_r6_13;
    u32 temp_r6_4;
    u32 temp_r6_5;
    u32 temp_r6_8;
    u32 temp_r6_9;
    u32 temp_r7_4;
    u32 temp_r7_5;
    u32 temp_r7_7;
    u32 var_r3_2;
    u32 var_r4;
    u32 var_r4_2;
    u32 var_r4_3;
    u32 var_r4_4;
    u32 var_r4_5;
    u32 var_r4_6;
    u8 *var_r2;
    u8 temp_r2_3;
    u8 temp_r2_4;
    void *temp_r3;
    void *temp_r9;
    s32 var_r5;
    s32 dialog;
    s8 slotFlags[8];

    sp50 = arg0;
    temp_r3 = *(void **)0x03001F2C;
    sp4C = temp_r3;
    temp_r2 = sp50 * 2;
    sp48 = 1;
    sp34 = temp_r2;
    temp_r9 = FIELD(temp_r3, void **, 0x184);
    temp_r5 = FIELD(sp4C, u16 *, temp_r2 + 0x174);
    sp38 = (s32) (u16) Func_08002304((s32) temp_r5, 0xA);
    temp_r0 = Func_080022f4(temp_r5, 0xA);
    sp1C = slotFlags;
    sp30 = (s32) temp_r0;
    sp2C = 0;
    sp28 = 0;
    sp24 = 0;
    sp20 = 0;
    sp3C = -1;
    var_r3 = slotFlags + 7;
    do {
        *var_r3 = 0;
        var_r3 -= 1;
    } while ((s32) var_r3 >= (s32) sp1C);
    if (sp50 == 0) {
        Func_080aafb8(temp_r9);
        sp44 = 0;
        if (sp50 < (s32) FIELD(sp4C, u8 *, 0x219)) {
            var_r1 = sp1C;
            var_r2 = temp_r9 + 0xA0;
            do {
                temp_r3_2 = *var_r2 << 0x18;
                var_r2 += 1;
                if (temp_r3_2 == 0) {
                    *var_r1 = 4;
                }
                temp_r5_2 = sp44 + 1;
                sp44 = temp_r5_2;
                var_r1 += 1;
            } while (temp_r5_2 < (s32) FIELD(sp4C, u8 *, 0x219));
        }
        sp2C = sp30;
        sp44 = 0;
        if ((s32) FIELD(sp4C, u8 *, 0x219) > 0) {
            do {
                if (sp1C[sp38] == 4) {
                    temp_r0_2 = sp38 + 1;
                    sp38 = temp_r0_2;
                    sp38 = Func_080aa538(temp_r0_2, (s32) FIELD(sp4C, u8 *, 0x219));
                }
                temp_r1 = sp44 + 1;
                sp44 = temp_r1;
            } while (temp_r1 < (s32) FIELD(sp4C, u8 *, 0x219));
        }
    } else {
        Func_080ae714(&sp54, (s32) FIELD(sp4C, s8 *, 0x1C));
        sp44 = 0;
        if ((s32) FIELD(sp4C, u8 *, 0x219) > 0) {
            var_r0 = sp1C;
            var_r1_2 = temp_r9 + 0xA0;
            var_r2_2 = var_r0;
            do {
                if (sp44 == FIELD(sp4C, s8 *, 0x1C)) {
                    *var_r2_2 = 7;
                } else if ((u8) (&sp54)[sp44] != 0) {
                    *var_r2_2 = 0;
                } else {
                    *var_r2_2 = 3;
                    if (*var_r1_2 == 0) {
                        *var_r0 = 7;
                    }
                }
                sp44 += 1;
                var_r0 += 1;
                var_r1_2 += 1;
                var_r2_2 += 1;
            } while (sp44 < (s32) FIELD(sp4C, u8 *, 0x219));
        }
    }
    if (sp50 == 1) {
        temp_r6 = FIELD(sp4C, u16 *, 0x174);
        temp_r5_3 = Func_08002304((s32) temp_r6, 0xA);
        temp_r6_2 = ((u16) temp_r5_3 * 7) + 1;
        Func_080ab1f4(FIELD(sp4C, void **, 0x30), temp_r6_2, Func_080022f4(temp_r6, 0xA) + 2, 6, sp50, 0xE);
        Func_080ab2ec(FIELD(sp4C, void **, 0x30), temp_r6_2, 2, 6, 7, 6);
        var_r5_2 = 0;
        if ((s32) FIELD(sp4C, u8 *, 0x219) > 0) {
            var_r6 = 8;
            do {
                if (var_r5_2 == FIELD(sp4C, s8 *, 0x1C)) {
                    if (FIELD(sp4C, u16 *, 0x178) & 0x8000) {
                        var_r0_2 = 0xBB0;
                    } else {
                        var_r0_2 = 0xBAF;
                    }
                } else if (2 & (u8) sp1C[var_r5_2]) {
                    var_r0_2 = 0xBAE;
                } else {
                    var_r0_2 = 0xBB1;
                }
                Func_08015080(var_r0_2, FIELD(sp4C, void **, 0x30), var_r6, 8);
                var_r5_2 += 1;
                var_r6 += 0x38;
            } while (var_r5_2 < (s32) FIELD(sp4C, u8 *, 0x219));
        }
    }
    Func_08015270(FIELD(sp4C, s32 *, 0x10C));
    FIELD(FIELD(sp4C, void **, 0x14), s8 *, 5) = 1;
    sp18 = sp38 * 8;
redraw_entry:
    temp_r7 = sp48;
    if (temp_r7 == 0) {

    } else {
        sp48 = 0;
        sp3C = -1;
        if (!(1 & (u8) sp1C[sp38])) {
            sp3C = sp30;
        }
        temp_r6_3 = FIELD(sp4C, s32 *, 0x10);
        temp_r5_4 = Func_08077008(
            (s32)FIELD(sp4C, u16 *, (sp38 * 2) + 0x208)
        );
        Func_08015270(temp_r6_3);
        Func_08015090(temp_r5_4, temp_r6_3, 0, 0);
        Func_08015080(FIELD(temp_r5_4, u8 *, 0x129) + 0x741, (void *) temp_r6_3, 0, 8);
        Func_08015090(0x080AF28C, temp_r6_3, 0x30, 0);
        Func_080150b0((s32) FIELD(temp_r5_4, u8 *, 0xF), 2, temp_r6_3, 0x48, sp48);
        if (sp50 == 0) {
            Func_08015080(0xBA9, (void *) temp_r6_3, 0, 0x10);
        }
        if (sp3C != -1) {
            sp28 = FIELD(
                temp_r9,
                u16 *,
                ((sp38 * 0xA) + sp3C) * 2
            );
        }
        Func_08015270(FIELD(sp4C, s32 *, 0x10C));
        if (sp50 == 1) {
            Func_08015120((s32) FIELD(sp4C, u8 *, 0x21A), 1);
            sp8 = 0xBB2;
            Func_08015080(0xBB2, (void *) FIELD(sp4C, s32 *, 0x10C), 0, 0);
            temp_r2_2 = FIELD(sp4C, u16 *, 0x178);
            Func_08015120((((u32) (0xE0 & temp_r2_2) >> 5) * 0x14) + (0x1F & temp_r2_2) + 0x12C, 4);
            Func_08015280(FIELD(sp4C, s32 *, 0x10C), ((u32) (0xE0 & FIELD(sp4C, u16 *, 0x178)) >> 5) + 0x5001, 6, 0, sp48);
            Func_08015080(0xBB3, (void *) FIELD(sp4C, s32 *, 0x10C), 0x38, 0);
            Func_08015080(0xBB4, (void *) FIELD(sp4C, s32 *, 0x10C), 0, 8);
        }
        if (sp3C == -1) {
            Func_080ad5b4(sp50, 0, 0xC8, 0);
        } else if (sp50 != 0) {
            temp_r6_4 = (u32) (0xF00 & sp28) >> 8;
            temp_r7_2 = 0x1F & sp28;
            temp_r5_5 = (u32) (0xE0 & sp28) >> 5;
            if ((Func_08077210(temp_r6_4, temp_r5_5, temp_r7_2) != 0) || (Func_08077208(temp_r6_4, temp_r5_5, temp_r7_2) != 0)) {
                if (Func_08077210(temp_r6_4, temp_r5_5, temp_r7_2) != 0) {
                    Func_080ad608(sp50, (s32) temp_r5_5, 1);
                } else {
                    Func_080ad608(sp50, (s32) temp_r5_5, 2);
                }
                Func_080ad5b4(sp50, ((sp18 - sp38) * 8) + 0x30, 0x36, 0);
            } else {
                Func_080ad608(sp50, (s32) temp_r5_5, 1);
                Func_080ad5b4(sp50, ((sp18 - sp38) * 8) + 0x30, 0x36, 1);
            }
            if (((u32) temp_r7 >> 1) != 0) {
                Func_080ad5f4(sp50, 0);
            }
        } else {
            if (sp24 != 0) {
                if (sp20 == 0) {
                    Func_08015080(0xB98, (void *) FIELD(sp4C, s32 *, 0x10C), 0, 0);
                } else {
                    Func_08015080(0xB99, (void *) FIELD(sp4C, s32 *, 0x10C), 0, 0);
                }
                temp_r6_5 = (u32) (0xF00 & sp28) >> 8;
                temp_r7_3 = 0x1F & sp28;
                temp_r5_6 = (u32) (0xE0 & sp28) >> 5;
                if ((Func_08077210(temp_r6_5, temp_r5_6, temp_r7_3) != 0) || (Func_08077208(temp_r6_5, temp_r5_6, temp_r7_3) != 0)) {
                    if (Func_08077210(temp_r6_5, temp_r5_6, temp_r7_3) != 0) {
                        Func_080ad608(sp50, (s32) temp_r5_6, 1);
                    } else {
                        Func_080ad608(sp50, (s32) temp_r5_6, 2);
                    }
                    Func_080ad5b4(sp50, ((sp18 - sp38) * 8) + 0x30, 0x3E, 0);
                } else {
                    Func_08015080(0xB9E, (void *) FIELD(sp4C, s32 *, 0x10C), 0, 0x10);
                    Func_080ad608(sp50, (s32) temp_r5_6, 1);
                    Func_080ad5b4(sp50, ((sp18 - sp38) * 8) + 0x30, 0x3E, 1);
                }
                var_r3_2 = (u32) temp_r7 >> 1;
            } else {
                sp8 = 0xB9A;
                Func_08015080(0xB9A, (void *) FIELD(sp4C, s32 *, 0x10C), 0, 0);
                temp_r5_7 = (u32) (0xF00 & sp28) >> 8;
                temp_r7_4 = (u32) (0xE0 & sp28) >> 5;
                temp_r6_6 = 0x1F & sp28;
                var_r4 = 0xB9A;
                if ((Func_08077210(temp_r5_7, temp_r7_4, temp_r6_6) != 0) || (var_r4 = sp8, (Func_08077208(temp_r5_7, temp_r7_4, temp_r6_6) != 0))) {
                    sp8 = var_r4;
                    if (Func_08077210(temp_r5_7, temp_r7_4, temp_r6_6) != 0) {
                        Func_08015080(var_r4 + 3, (void *) FIELD(sp4C, s32 *, 0x10C), 0, 0x10);
                        Func_080ad608(0, (s32) temp_r7_4, 1);
                    } else {
                        Func_08015080(var_r4 + 2, (void *) FIELD(sp4C, s32 *, 0x10C), 0, 0x10);
                        Func_080ad608(0, (s32) temp_r7_4, 2);
                    }
                    Func_080ad5b4(sp50, ((sp18 - sp38) * 8) + 0x30, 0x3E, 0);
                } else {
                    Func_08015080(var_r4 + 4, (void *) FIELD(sp4C, s32 *, 0x10C), 0, 0x10);
                    Func_080ad608(0, (s32) temp_r7_4, 1);
                    Func_080ad5b4(0, ((sp18 - sp38) * 8) + 0x30, 0x3E, 1);
                }
                var_r3_2 = (u32) temp_r7 >> 1;
            }
            if (var_r3_2 != 0) {
                Func_080ad5f4(sp50, 0);
            }
        }
        Func_08015278(FIELD(sp4C, void **, 0x30));
        if (sp3C != -1) {
            Func_08015080(0xBAD, FIELD(sp4C, void **, 0x30), 0, 0x50);
            Func_08015068(FIELD(sp4C, void **, 0x30), 0, 0x60, 0xE0, 0x68);
            Func_08015080((((u32) (0xE0 & sp28) >> 5) * 0x14) + (0x1F & sp28) + 0x666, FIELD(sp4C, void **, 0x30), 0, 0x60);
        }
        if (!(1 & (u8) sp1C[sp38])) {
            Func_080ab1f4(FIELD(sp4C, void **, 0x30), (sp18 - sp38) + 1, sp30 + 2, 6, 1, 0xE);
        }
        FIELD(*(void **)0x03001E8C, s8 *, 0xEA3) = 1;
    }
    if (1 & (u8) sp1C[sp38]) {
        Func_080a1a40(((sp18 - sp38) * 8) - 8, 0x34);
    } else {
        Func_080a1a40(((sp18 - sp38) * 8) - 8, (sp30 * 8) + 0x3C);
    }
    Func_080030f8(1U);
    if (!(*(s32 *)0x03001AE8 & 0x100) || (*(s32 *)0x03001AF8 & 0x100)) {
        if (sp24 != 0) {
            sp48 = 1;
        }
        sp24 = 0;
        sp20 = 0;
    }
    var_fp = *(s32 *)0x03001B04;
    var_r4_2 = *(u32 *)0x03001C94;
    temp_r1_2 = FIELD(temp_r9, s32 *, 0x212C);
    if (temp_r1_2 == 0) {

    } else {
        var_r4_2 = 0;
        FIELD(temp_r9, s32 *, 0x2128) = (s32) (FIELD(temp_r9, s32 *, 0x2128) + 1);
        temp_r3_3 = temp_r1_2 - 1;
        var_fp = 0;
        switch (temp_r3_3) {
        case 27:
            if (!(*(volatile u32 *)0x03001C94 & 1)) {
                do {
                    Func_080a1a40(0x96, 0x1A);
                    Func_080030f8(1U);
                } while (!(*(volatile u32 *)0x03001C94 & 1));
            }
            var_r4_2 = 2;
            var_fp = 2;
            break;
        case 26:
            if (FIELD(temp_r9, s32 *, 0x2128) != 0x3C) {

            } else {
                dialog = Func_08015038(0xC4C, 9, 9, 1);
                *(s8 *)0x0200044C = 1;
loop_105:
                if (Func_08015048() == 0) {
                    Func_080030f8(1U);
                    goto loop_105;
                }
                Func_08015018(dialog, 1);
                Func_080aafb8(temp_r9);
                Func_080030f8(1U);
                dialog = Func_08015038(0xC4D, 9, 9, 1);
                *(s8 *)0x0200044C = 1;
loop_108:
                if (Func_08015048() == 0) {
                    Func_080030f8(1U);
                    goto loop_108;
                }
                Func_08015018(dialog, 1);
                Func_080aafb8(temp_r9);
                FIELD(temp_r9, s32 *, 0x2128) = 0;
                Func_080b50f8();
                Func_080b50f8();
                Func_080b50f8();
                Func_080771b8(0U, 0U, 0);
                Func_080771c8(0U, 0U, 0);
                Func_08077010(0U);
                var_fp = 2;
block_163:
                var_r4_2 = 2;
            }
            break;
        case 0:
            if (FIELD(temp_r9, s32 *, 0x2128) != 0x3C) {

            } else {
                sp8 = 0;
                dialog = Func_08015038(0xC40, 9, 9, 1);
                *(s8 *)0x0200044C = 1;
loop_114:
                if (Func_08015048() == 0) {
                    Func_080030f8(1U);
                    goto loop_114;
                }
                Func_08015018(dialog, 1);
                Func_080aafb8(temp_r9);
                FIELD(temp_r9, s32 *, 0x2128) = 0;
                var_r3_3 = 2;
block_155:
                FIELD(temp_r9, s32 *, 0x212C) = var_r3_3;
                var_r4_2 = sp8;
            }
            break;
        case 1:
        case 3:
        case 7:
        case 8:
        case 21:
            if (FIELD(temp_r9, s32 *, 0x2128) != 0x5A) {

            } else {
                var_fp = 1;
                FIELD(temp_r9, s32 *, 0x2128) = 0;
                var_r2_3 = temp_r9 + 0x212C;
                var_r3_4 = FIELD(temp_r9, s32 *, 0x212C) + 1;
block_158:
                var_r4_2 = 1;
                *var_r2_3 = var_r3_4;
            }
            break;
        case 2:
            if (FIELD(temp_r9, s32 *, 0x2128) != 0x5A) {

            } else {
                FIELD(temp_r9, s32 *, 0x2128) = 0;
                var_fp = 0x10;
                var_r4_2 = 0x10;
                FIELD(temp_r9, s32 *, 0x212C) = 4;
            }
            break;
        case 5:
        case 6:
            if (FIELD(temp_r9, s32 *, 0x2128) != 0x3C) {

            } else {
                sp8 = 0;
                dialog = Func_08015038(0xC41, 9, 9, 1);
                *(s8 *)0x0200044C = 1;
loop_126:
                if (Func_08015048() == 0) {
                    Func_080030f8(1U);
                    goto loop_126;
                }
                Func_08015018(dialog, 1);
                Func_080aafb8(temp_r9);
                FIELD(temp_r9, s32 *, 0x2128) = 0;
                var_r3_3 = 8;
                goto block_155;
            }
            break;
        case 12:
            if (FIELD(temp_r9, s32 *, 0x2128) != 0x28) {

            } else {
                FIELD(temp_r9, s32 *, 0x2128) = 0;
                var_fp = 2;
                var_r4_2 = 2;
                FIELD(temp_r9, s32 *, 0x212C) = (s32) (FIELD(temp_r9, s32 *, 0x212C) + 1);
            }
            break;
        case 13:
        case 15:
        case 16:
            if (FIELD(temp_r9, s32 *, 0x2128) != 0x28) {

            } else {
                FIELD(temp_r9, s32 *, 0x2128) = 0;
                FIELD(temp_r9, s32 *, 0x212C) = (s32) (FIELD(temp_r9, s32 *, 0x212C) + 1);
            }
            break;
        case 14:
            if (FIELD(temp_r9, s32 *, 0x2128) != 0x3C) {

            } else {
                *(s8 *)0x0200044C = 1;
                sp8 = 0;
                dialog = Func_08015038(0xC44, 9, 9, 1);
                Func_080a1ac0(2, 0x92);
loop_138:
                var_r4_3 = sp8;
                if (Func_08015048() == 0) {
                    Func_080030f8(1U);
                    goto loop_138;
                }
                if (!(*(volatile u32 *)0x03001C94 & 1)) {
                    do {
                        sp8 = var_r4_3;
                        Func_080a1a40(2, 0x92);
                        Func_080030f8(1U);
                    } while (!(*(volatile u32 *)0x03001C94 & 1));
                }
                sp8 = var_r4_3;
                Func_08015018(dialog, 1);
                Func_080aafb8(temp_r9);
                Func_080030f8(1U);
                dialog = Func_08015038(0xC45, 9, 9, 1);
loop_144:
                var_r4_4 = sp8;
                if (Func_08015048() == 0) {
                    Func_080030f8(1U);
                    goto loop_144;
                }
                if (!(*(volatile u32 *)0x03001C94 & 1)) {
                    do {
                        sp8 = var_r4_4;
                        Func_080a1a40(2, 0x92);
                        Func_080030f8(1U);
                    } while (!(*(volatile u32 *)0x03001C94 & 1));
                }
                sp8 = var_r4_4;
                Func_08015018(dialog, 1);
                Func_080aafb8(temp_r9);
                Func_080030f8(1U);
                dialog = Func_08015038(0xC46, 9, 9, 1);
loop_150:
                var_r4_5 = sp8;
                if (Func_08015048() == 0) {
                    Func_080030f8(1U);
                    goto loop_150;
                }
                if (!(*(volatile u32 *)0x03001C94 & 1)) {
                    do {
                        sp8 = var_r4_5;
                        Func_080a1a40(2, 0x92);
                        Func_080030f8(1U);
                    } while (!(*(volatile u32 *)0x03001C94 & 1));
                }
                sp8 = var_r4_5;
                Func_08015018(dialog, 1);
                Func_080aafb8(temp_r9);
                Func_080030f8(1U);
                FIELD(temp_r9, s32 *, 0x2128) = 0;
                var_r3_3 = 0x10;
                goto block_155;
            }
            break;
        case 17:
        case 19:
            if (FIELD(temp_r9, s32 *, 0x2128) == 0x5A) {
                var_fp = 1;
                FIELD(temp_r9, s32 *, 0x2128) = 0;
                var_r3_4 = 0x15;
                var_r2_3 = temp_r9 + 0x212C;
                goto block_158;
            }
            break;
        case 20:
            if (FIELD(temp_r9, s32 *, 0x2128) == 0x5A) {
                FIELD(temp_r9, s32 *, 0x2128) = 0;
                var_fp = 0x20;
                var_r4_2 = 0x20;
                FIELD(temp_r9, s32 *, 0x212C) = 0x16;
            }
            break;
        case 23:
            if (FIELD(temp_r9, s32 *, 0x2128) == 0x3C) {
                FIELD(temp_r9, s32 *, 0x2128) = 0;
                FIELD(temp_r9, s32 *, 0x212C) = 0x19;
                var_fp = 2;
                goto block_163;
            }
            break;
        }
    }
    if (sp50 != 0) {
        goto block_182;
    }
    if (!(0x100 & var_r4_2)) {
        goto block_179;
    }
    if (sp3C == -1) {
        goto block_189;
    }
    sp44 = 0;
    temp_r7_5 = (u32) (0xF00 & sp28) >> 8;
    temp_r5_8 = (u32) (0xE0 & sp28) >> 5;
    temp_r6_7 = 0x1F & sp28;
    sp8 = var_r4_2;
    var_r4_6 = var_r4_2;
    if ((Func_08077210(temp_r7_5, temp_r5_8, temp_r6_7) != 0) || (var_r4_6 = sp8, (Func_08077208(temp_r7_5, temp_r5_8, temp_r6_7) != 0))) {
        sp44 = 1;
    }
    sp24 = 1;
    *(volatile s32 *)0x03001AF8 = 0;
    if (sp44 == 0) {
        Func_080f9010(0x72);
        Func_08015278(FIELD(sp4C, void **, 0x30));
        Func_08015068(FIELD(sp4C, void **, 0x30), 0, 0x50, 0xD8, 0x68);
        Func_08015078(0xBBE, (s32) FIELD(sp4C, void **, 0x30), 0, 0x60);
        sp48 = 1;
        goto redraw_entry;
    }
    if ((sp28 >> 0xF) != 0) {
        sp8 = var_r4_6;
        Func_080f9010(0xAF);
        temp_r3_4 = 0x1F & sp28;
        temp_r5_9 = (u32) (0xF00 & sp28) >> 8;
        temp_r6_8 = (u32) (0xE0 & sp28) >> 5;
        spC = temp_r3_4;
        Func_080771b8(temp_r5_9, temp_r6_8, temp_r3_4);
        Func_080771c8(temp_r5_9, temp_r6_8, temp_r3_4);
    } else {
        sp8 = var_r4_6;
        Func_080f9010(0x8B);
        temp_r3_5 = 0x1F & sp28;
        temp_r5_10 = (u32) (0xF00 & sp28) >> 8;
        temp_r6_9 = (u32) (0xE0 & sp28) >> 5;
        spC = temp_r3_5;
        Func_080771b0(temp_r5_10, temp_r6_9, temp_r3_5);
        Func_080771c0(temp_r5_10, temp_r6_9, temp_r3_5);
    }
    Func_08077010((u32) (0xF00 & sp28) >> 8);
    Func_080aafb8(temp_r9);
    sp48 = 1;
    var_r4_2 = sp8;
block_179:
    if ((sp50 == 0) && (0x200 & var_r4_2)) {
        var_r5 = 7;
        Func_080f9010(0x70);
    } else {
block_182:
        if (!(1 & var_r4_2)) {
            if (sp50 != 1) {
                goto block_203;
            }
            if (!(0x100 & var_r4_2)) {
block_203:
                if (8 & var_r4_2) {
                    var_r5_3 = 2;
                    goto block_207;
                }
                if (2 & var_r4_2) {
                    var_r5_3 = 1;
block_207:
                    Func_080f9010(0x71);
                    var_r5 = 0 - var_r5_3;
                } else {
                    if (sp50 != 0) {
                        goto block_236;
                    }
                    if (!(4 & var_r4_2)) {
                        goto block_236;
                    }
                    if (sp24 == 0) {
                        Func_080ab1f4(FIELD(sp4C, void **, 0x30), (sp18 - sp38) + 1, sp30 + 2, 6, 1, 0xF);
                        var_r5 = 10;
                        Func_080f9010(0x70);
                    } else {
                        temp_r5_11 = sp20 ^ 1;
                        sp20 = temp_r5_11;
                        if (temp_r5_11 != 0) {
                            Func_080f9010(0x8B);
                        } else {
                            Func_080f9010(0xAF);
                        }
                        sp44 = 0;
                        if ((s32) FIELD(sp4C, u8 *, 0x219) <= 0) {

                        } else {
                            sp14 = 0xA0;
                            sp10 = 0;
loop_220:
                            sp40 = 0;
                            if ((s32)FIELD(temp_r9, s8 *, sp14) > 0) {
                                var_sl = (u16 *)((u8 *)temp_r9 + (sp10 * 2));
                                do {
                                    temp_r7_6 = *var_sl;
                                    var_sl += 2;
                                    temp_r4 = (u32) (temp_r7_6 & 0xF00) >> 8;
                                    var_r8 = 0;
                                    temp_r6_10 = (u32) (temp_r7_6 & 0xE0) >> 5;
                                    temp_r5_12 = temp_r7_6 & 0x1F;
                                    sp8 = temp_r4;
                                    if ((Func_08077210(temp_r4, temp_r6_10, temp_r5_12) != 0) || (Func_08077208(temp_r4, temp_r6_10, temp_r5_12) != 0)) {
                                        var_r8 = 1;
                                    }
                                    if (var_r8 != 0) {
                                        if (sp20 != 0) {
                                            if ((temp_r7_6 >> 0xF) == 0) {
                                                temp_r3_6 = 0x1F & temp_r7_6;
                                                temp_r5_13 = (u32) (temp_r7_6 & 0xF00) >> 8;
                                                temp_r6_11 = (u32) (temp_r7_6 & 0xE0) >> 5;
                                                spC = temp_r3_6;
                                                Func_080771b0(temp_r5_13, temp_r6_11, temp_r3_6);
                                                Func_080771c0(temp_r5_13, temp_r6_11, temp_r3_6);
                                                Func_08077010(temp_r5_13);
                                            }
                                        } else if ((temp_r7_6 >> 0xF) != 0) {
                                            temp_r3_7 = 0x1F & temp_r7_6;
                                            temp_r5_14 = (u32) (temp_r7_6 & 0xF00) >> 8;
                                            temp_r6_12 = (u32) (temp_r7_6 & 0xE0) >> 5;
                                            spC = temp_r3_7;
                                            Func_080771b8(temp_r5_14, temp_r6_12, temp_r3_7);
                                            Func_080771c8(temp_r5_14, temp_r6_12, temp_r3_7);
                                            Func_08077010(temp_r5_14);
                                        }
                                    }
                                    temp_r1_3 = sp40 + 1;
                                    sp40 = temp_r1_3;
                                } while (
                                    temp_r1_3 <
                                    (s32)FIELD(temp_r9, s8 *, sp14)
                                );
                            }
                            temp_r1_4 = sp44 + 1;
                            sp14 += 1;
                            sp10 += 0xA;
                            sp44 = temp_r1_4;
                            if (temp_r1_4 < (s32) FIELD(sp4C, u8 *, 0x219)) {
                                goto loop_220;
                            }
                        }
                        Func_080aafb8(temp_r9);
                        sp48 = 1;
block_236:
                        if (0x40 & var_fp) {
                            Func_080f9010(0x6F);
                            if (4 & (u8) sp1C[sp38]) {
                                goto redraw_entry;
                            }
                            Func_080ab1f4(FIELD(sp4C, void **, 0x30), (sp18 - sp38) + 1, sp30 + 2, 6, 1, 0xF);
                            var_r0_3 = sp38;
                            temp_r2_3 = (u8)sp1C[var_r0_3];
                            if (4 & temp_r2_3) {
                                goto redraw_entry;
                            }
                            if (1 & temp_r2_3) {
                                sp1C[var_r0_3] = (s8)(temp_r2_3 & ~1);
                                sp30 = 0;
                            } else if ((sp30 == 0) && (2 & temp_r2_3)) {
                                sp1C[sp38] = (s8)(temp_r2_3 | 1);
                                sp48 = 2;
                                goto redraw_entry;
                            }
                            sp30 -= 1;
                            var_r1_3 = FIELD(temp_r9, s8 *, sp38 + 0xA0);
                            if (var_r1_3 == 0) {
                                var_r1_3 = 1;
                            }
                            temp_r0_3 = Func_080aa538(sp30, (s32) var_r1_3);
                            sp30 = temp_r0_3;
                            sp2C = temp_r0_3;
                            sp48 = 2;
                            goto redraw_entry;
                        }
                        if (0x80 & var_fp) {
                            Func_080f9010(0x6F);
                            if (4 & (u8) sp1C[sp38]) {
                                goto redraw_entry;
                            }
                            Func_080ab1f4(FIELD(sp4C, void **, 0x30), (sp18 - sp38) + 1, sp30 + 2, 6, 1, 0xF);
                            sp30 += 1;
                            var_r1_4 = FIELD(temp_r9, s8 *, sp38 + 0xA0);
                            if (var_r1_4 == 0) {
                                var_r1_4 = 1;
                            }
                            sp30 = Func_080aa538(sp30, (s32) var_r1_4);
                            temp_r2_3 = (u8) sp1C[sp38];
                            if ((1 & temp_r2_3) && !(4 & temp_r2_3)) {
                                sp1C[sp38] = -2 & temp_r2_3;
                                sp30 = 0;
                            } else if (sp30 == 0) {
                                temp_r2_4 = (u8) sp1C[sp38];
                                if (2 & temp_r2_4) {
                                    sp1C[sp38] = 1 | temp_r2_4;
                                }
                            }
                            sp2C = sp30;
                            sp48 = 2;
                            goto redraw_entry;
                        }
                        if (0x20 & var_fp) {
                            Func_080f9010(0x6F);
                            if (!(4 & (u8) sp1C[sp38])) {
                                Func_080ab1f4(FIELD(sp4C, void **, 0x30), (sp18 - sp38) + 1, sp30 + 2, 6, 1, 0xF);
                            }
                            temp_r3_8 = sp38 - 1;
                            sp38 = temp_r3_8;
                            sp38 = Func_080aa538(temp_r3_8, (s32) FIELD(sp4C, u8 *, 0x219));
                            if (sp50 == 0) {
                                sp44 = 0;
                                if (sp50 < (s32) FIELD(sp4C, u8 *, 0x219)) {
                                    do {
                                        if (sp1C[sp38] == 4) {
                                            temp_r0_4 = sp38 - 1;
                                            sp38 = temp_r0_4;
                                            sp38 = Func_080aa538(temp_r0_4, (s32) FIELD(sp4C, u8 *, 0x219));
                                        }
                                        temp_r1_5 = sp44 + 1;
                                        sp44 = temp_r1_5;
                                    } while (temp_r1_5 < (s32) FIELD(sp4C, u8 *, 0x219));
                                }
                            }
                        } else {
                            if (!(0x10 & var_fp)) {
                                goto redraw_entry;
                            }
                            Func_080f9010(0x6F);
                            if (!(4 & (u8) sp1C[sp38])) {
                                Func_080ab1f4(FIELD(sp4C, void **, 0x30), (sp18 - sp38) + 1, sp30 + 2, 6, 1, 0xF);
                            }
                            temp_r3_9 = sp38 + 1;
                            sp38 = temp_r3_9;
                            sp38 = Func_080aa538(temp_r3_9, (s32) FIELD(sp4C, u8 *, 0x219));
                            if (sp50 == 0) {
                                sp44 = 0;
                                if (sp50 < (s32) FIELD(sp4C, u8 *, 0x219)) {
                                    do {
                                        if (sp1C[sp38] == 4) {
                                            temp_r0_5 = sp38 + 1;
                                            sp38 = temp_r0_5;
                                            sp38 = Func_080aa538(temp_r0_5, (s32) FIELD(sp4C, u8 *, 0x219));
                                        }
                                        temp_r1_6 = sp44 + 1;
                                        sp44 = temp_r1_6;
                                    } while (temp_r1_6 < (s32) FIELD(sp4C, u8 *, 0x219));
                                }
                            }
                        }
                        sp30 = sp2C;
                        var_r1_5 = FIELD(temp_r9, s8 *, sp38 + 0xA0);
                        if (var_r1_5 == 0) {
                            var_r1_5 = 1;
                        }
                        sp30 = Func_080aa538(sp30, (s32) var_r1_5);
                        sp48 = 2;
                        sp18 = sp38 * 8;
                        goto redraw_entry;
                    }
                }
            } else {
                goto block_187;
            }
        } else {
block_187:
            sp44 = 1;
            if (!(1 & (u8) sp1C[sp38])) {
                if (sp3C == -1) {
block_189:
                    Func_080f9010(0x72);
                    goto redraw_entry;
                }
                temp_r7_7 = (u32) (0xF00 & sp28) >> 8;
                temp_r6_13 = (u32) (0xE0 & sp28) >> 5;
                temp_r5_15 = 0x1F & sp28;
                sp44 = 0;
                if ((Func_08077210(temp_r7_7, temp_r6_13, temp_r5_15) != 0) || (Func_08077208(temp_r7_7, temp_r6_13, temp_r5_15) != 0)) {
                    sp44 = 1;
                }
            }
            if (sp44 == 0) {
                Func_080f9010(0x72);
                Func_08015278(FIELD(sp4C, void **, 0x30));
                Func_08015068(FIELD(sp4C, void **, 0x30), 0, 0x50, 0xD8, 0x68);
                Func_08015078(0xBBE, (s32) FIELD(sp4C, void **, 0x30), 0, 0x60);
                goto redraw_entry;
            }
            if (sp50 == 1) {
                var_r5 = 4;
                if (sp50 & (u8) sp1C[sp38]) {
                    if (sp38 == FIELD(sp4C, s8 *, 0x1C)) {
                        var_r5 = 2;
                        if (!(0x8000 & FIELD(sp4C, u16 *, 0x178))) {
                            goto block_201;
                        }
                    } else {
                        var_r5 = 3;
                    }
                }
            } else {
block_201:
                var_r5 = 1;
            }
            Func_080f9010(0x70);
        }
    }
    FIELD(sp4C, s8 *, sp50 + 0x1C) = (s8)sp38;
    if (sp3C != -1) {
        temp_r2_5 = FIELD(
            temp_r9,
            u16 *,
            ((sp38 * 0xA) + sp3C) * 2
        );
        FIELD(sp4C, u16 *, sp34 + 0x178) = temp_r2_5;
        temp_r0_6 = sp50 + 0x254;
        FIELD(sp4C, u8 *, temp_r0_6) = (u8)(0x1F & temp_r2_5);
        FIELD(sp4C, u8 *, temp_r0_6 + 2) =
            (u8)((u32)(0xE0 & temp_r2_5) >> 5);
        FIELD(sp4C, u8 *, sp50 + 0x258) =
            (u8)((u32)(0xF00 & temp_r2_5) >> 8);
    }
    FIELD(sp4C, u16 *, sp34 + 0x174) =
        (u16)(sp38 + (sp30 * 0xA));
    return var_r5;
}
