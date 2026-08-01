/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * Callee signatures here are established, not guessed: 0x03001388 is the
 * word copy declared in the EXACT src/080d40ec.c, and 0x03000168 is the fill
 * documented in semantic/main/080e15e8.c as (destination, size, value).
 */
#include "types.h"

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);
typedef void (*ArmFill)(void *destination, u32 size, u32 value);

#define FIELD_AT(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct Work_80190000;

void *Func_080040b4(s32);
void *Func_08004970(s32);
void *Func_080162d4(s32, s32, s32, s32, s32);
void Func_08016418(void *, s32);
void Func_08016498(void *);
void Func_080164ac(void *);
void Func_08016738(void);
void Func_08017aa4(void *, void *, s32, s32);
void Func_08019000(struct Work_80190000 *, s32, s32, s32, s32);
void Func_0801965c(s32, s16 *, s32);
void Func_080198dc(void);
void Func_08019908(s32, s32);
void Func_0801e318(void);
void Func_0801e3c8(s32);
void Func_0801e71c(s32);
void Func_0801e7c0(s32, void *, s32, s32);
void Func_0801e9d4(s32, s32, void *, s32, s32);
void Func_08022768(s32, s32, s32, s32, s32);
void *Func_08022b44(void *, s32, s32, s32, s32 *);
void Func_080030f8(u32);
void Func_08002df0(void *);
void Func_08003dec(void *, s32);
void Func_08003f3c(u32);
s32 Func_080040d0(void *, const void *);
s32 Func_080022ec(s32, s32);
void Func_0800352c(void);
void Func_080f9010(s32);
void *Func_08077000(s32);
void *Func_08077008(s32);

s32 Func_08023e70(s32 arg0, s32 arg1) {
    s32 *sp4;
    u8 *sp8;
    s16 *spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    s16 *sp38;
    s32 sp3C;
    s32 sp40;
    s32 sp44;
    void *sp48;
    s32 sp4C;
    s32 sp50;
    s32 *temp_r3_3;
    s32 *var_r5;
    s32 *var_r8_2;
    s32 temp_fp;
    s32 temp_r0_5;
    s32 temp_r0_6;
    s32 temp_r0_7;
    s32 temp_r0_8;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r2;
    s32 temp_r2_3;
    s32 temp_r3_4;
    s32 temp_r3_5;
    s32 temp_r3_6;
    s32 temp_r3_7;
    s32 temp_r3_8;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 temp_r5_4;
    s32 temp_r5_5;
    s32 temp_r9;
    s32 var_r0;
    s32 var_r1;
    s32 var_r1_2;
    s32 var_r1_3;
    s32 var_r1_4;
    s32 var_r2;
    s32 var_r2_2;
    s32 var_r3;
    s32 var_r4;
    s32 var_r5_2;
    s32 var_r5_3;
    s32 var_r6;
    s32 var_r6_2;
    s32 var_r6_3;
    s32 var_r6_4;
    s32 var_r6_5;
    s32 var_r7;
    s32 var_r7_2;
    s32 var_r7_3;
    s32 var_r7_4;
    s32 var_r7_5;
    s32 var_r8_3;
    s32 var_sl;
    s8 temp_r3_2;
    u16 temp_r5_3;
    u32 temp_r0_3;
    u32 temp_r0_4;
    void *temp_r0;
    void *temp_r0_2;
    void *temp_r1_4;
    void *temp_r2_2;
    void *temp_r2_4;
    void *temp_r2_5;
    void *temp_r3;
    void *temp_r3_10;
    void *temp_r3_9;
    void *temp_r7;
    void *var_r8;

    s32 cursorSprite[3];
    s16 description[26];

    (void)arg1;
    sp4C = arg0;
    sp48 = FIELD_AT((void *)0x03001E8C, void **, 0);
    sp44 = 0;
    sp40 = -1;
    sp3C = Func_080040b4(0x80);
    sp38 = Func_08004970(0x150);
    sp34 = 0;
    sp30 = sp40;
    sp28 = 0;
    sp20 = 0;
    sp50 = 0;
    sp1C = 0;
    sp18 = 0;
    sp2C = Func_080162d4(0U, 4U, 0x1EU, 4U, 0x2A);
    Func_0801e3c8(1);
    temp_r9 = Func_080162d4(0x15U, 9U, 9U, 0xBU, 6);
    temp_r3 = FIELD_AT((void *)0x03001E8C, void **, 0xA8);
    temp_fp = FIELD_AT(temp_r3, s32 *, 0x34);
    var_sl = FIELD_AT(temp_r3, s32 *, 0x30);
    sp24 = FIELD_AT(temp_r3, s32 *, 0x38);
    var_r7 = 0;
    var_r8 = Func_08077008(arg0) + 0xF8;
    do {
        var_r6_2 = 0;
        var_r5 = (sp34 * 4) + sp38;
loop_2:
        temp_r2 = 1 << var_r6_2;
        if (FIELD_AT(var_r8, s32 *, 0x10) & temp_r2) {
            *var_r5 = (var_r7 << 8) | var_r6_2;
            var_r5 += 4;
            sp34 += 1;
        } else if (FIELD_AT(var_r8, s32 *, 0) & temp_r2) {
            var_r0 = 0;
            if ((u32) sp4C > 7U) {
                var_r0 = 1;
            }
            temp_r0 = Func_08077000(var_r0);
            var_r1 = 0;
            temp_r0_2 = temp_r0 + 8;
            var_r4 = 0;
            if ((s32) FIELD_AT(temp_r0, s32 *, 0x108) > 0) {
                if ((FIELD_AT(temp_r0_2, u8 *, 2) != sp4C) || (FIELD_AT(temp_r0, u8 *, 8) != var_r7) || (FIELD_AT(temp_r0_2, u8 *, 1) != var_r6_2)) {
loop_11:
                    var_r1 += 1;
                    if (var_r1 < (s32) FIELD_AT(temp_r0_2, s32 *, 0x100)) {
                        var_r4 = var_r1 * 4;
                        temp_r2_2 = temp_r0_2 + var_r4;
                        if ((FIELD_AT(temp_r2_2, u8 *, 2) == sp4C) && (FIELD_AT(temp_r2_2, u8 *, 0) == var_r7) && (FIELD_AT(temp_r2_2, u8 *, 1) == var_r6_2)) {

                        } else {
                            goto loop_11;
                        }
                    } else {
                        var_r4 = var_r1 * 4;
                    }
                } else {
                    var_r4 = 0;
                }
            }
            temp_r2_3 = (var_r7 << 8) | var_r6_2 | 0x10000;
            *var_r5 = temp_r2_3;
            temp_r3_2 = (s8) FIELD_AT((temp_r0_2 + var_r4), u8 *, 3);
            if ((s32) temp_r3_2 > 0) {
                *var_r5 = temp_r2_3 | (temp_r3_2 << 0x11);
            }
            sp34 += 1;
            var_r5 += 4;
        }
        var_r6_2 += 1;
        if (var_r6_2 <= 0x13) {
            goto loop_2;
        }
        var_r7 += 1;
        var_r8 += 4;
    } while (var_r7 <= 3);
    *((sp34 * 4) + sp38) = 0x80000000;
    FIELD_AT(sp48, s8 *, 0xEA3) = 1;
    sp4 = cursorSprite;
    spC = description;
    sp8 = sp48 + 0xEA3;
main_loop:
    if ((temp_fp == sp30) && (var_sl == sp40) && (sp1C == 0)) {

    } else {
        temp_r5 = *(((temp_fp + var_sl) * 4) + sp38);
        sp28 = 0;
        FIELD_AT(sp48, u8 *, 0xEA6) = 1U;
        Func_08022768(FIELD_AT(temp_r9, u16 *, 0xC) + 1, FIELD_AT(temp_r9, u16 *, 0xE) + (sp40 * 2) + 1, FIELD_AT(temp_r9, u16 *, 8) - 2, 1, 0xF);
        if (sp20 != 0) {
            Func_08016418(sp2C, 1);
            sp2C = Func_080162d4(0U, 4U, 0x1EU, 4U, 0x2A);
            Func_08016738();
        }
        sp1C = 0;
        if (sp34 == 0) {
            Func_0801965c(0x8ED, spC, 0x34);
        } else {
            Func_080198dc();
            sp18 = 0;
            var_r1_2 = sp1C;
            if (FIELD_AT(*(void **)0x03001F34, s32 *, 0xE4) == temp_r5) {
                sp18 = 1;
            } else {
loop_33:
                var_r1_2 += 1;
                if (var_r1_2 <= 7) {
                    if (*(s32 *)((u8 *)*(void **)0x03001F34 +
                            (var_r1_2 * 4) + 0xE4) == temp_r5) {
                        sp18 = 1;
                    } else {
                        goto loop_33;
                    }
                }
            }
            if (sp18 != 0) {
                Func_0801965c(0x8EF, spC, 0x34);
                if (sp44 != 0) {
                    Func_08016418(sp44, 1);
                    sp44 = 0;
                    sp20 = 0;
                }
            } else if (0x10000 & temp_r5) {
                temp_r0_3 = 0x3E0000 & temp_r5;
                if (temp_r0_3 != 0) {
                    Func_08019908(temp_r0_3 >> 0x11, 5U);
                    Func_0801965c((((u32) (0xF00 & temp_r5) >> 8) * 0x14) + (u8) temp_r5 + 0x666, spC, 0x34);
                    if (sp44 != 0) {
                        Func_08016418(sp44, 1);
                        sp44 = 0;
                        sp20 = 0;
                    }
                } else {
                    sp44 = Func_08022b44(sp44, sp4C, temp_r5, sp20, &sp50);
                    Func_0801965c(0x899, spC, 0x34);
                    sp28 = 1 << ((u32) (temp_r5 & 0xF00) >> 8);
                }
            } else {
                sp44 = Func_08022b44(sp44, sp4C, temp_r5, sp20, &sp50);
                Func_0801965c((((u32) (0xF00 & temp_r5) >> 8) * 0x14) + (u8) temp_r5 + 0x666, spC, 0x34);
            }
        }
        FIELD_AT(sp48, u8 *, 0xEA6) = 0U;
        if (sp20 == 0) {
            FIELD_AT(sp48, u8 *, 0xEA6) = 1U;
            Func_08016418(sp2C, 1);
            sp2C = Func_080162d4(0U, 4U, 0x1EU, 4U, 0x2A);
            FIELD_AT(sp48, u8 *, 0xEA6) = (u8) sp20;
        }
        Func_08017aa4(spC, sp2C, 0, 4);
        sp40 = var_sl;
        if (temp_fp != sp30) {
            Func_08016498((void *) temp_r9);
            temp_r3_3 = (temp_fp * 4) + sp38;
            var_r6_3 = *temp_r3_3;
            var_r7_2 = 0;
            if (var_r6_3 != 0x80000000) {
                var_r8_2 = temp_r3_3;
loop_51:
                Func_08019000((struct Work_80190000 *) temp_r9, ((u32) (var_r6_3 & 0xF00) >> 8) + 0x5001, 0, var_r7_2 * 2, 0);
                if (0x3E0000 & var_r6_3) {
                    Func_0801e71c(4);
                } else if (0x10000 & var_r6_3) {
                    Func_0801e71c(2);
                }
                temp_r5_2 = var_r7_2 * 0x10;
                Func_0801e7c0((((u32) (var_r6_3 & 0xF00) >> 8) * 0x14) + (0xFF & var_r6_3) + 0x45F, temp_r9, 8, temp_r5_2);
                temp_r0_4 = 0x3E0000 & var_r6_3;
                if (temp_r0_4 != 0) {
                    Func_0801e9d4((s32) (temp_r0_4 >> 0x11), 1, temp_r9, 0x30, temp_r5_2);
                }
                var_r7_2 += 1;
                Func_0801e71c(0xF);
                if (var_r7_2 <= 4) {
                    var_r8_2 += 4;
                    var_r6_3 = *var_r8_2;
                    if (var_r6_3 != 0x80000000) {
                        goto loop_51;
                    }
                }
            }
            sp30 = temp_fp;
        }
        if (sp34 > 5) {
            var_r7_3 = 0;
loop_65:
            temp_r0_5 = Func_080022ec(sp34 + 4, 5);
            if (var_r7_3 < temp_r0_5) {
                var_r6_4 = var_r7_3 + 0xF301;
                if (var_r7_3 == Func_080022ec(temp_fp, 5)) {
                    var_r6_4 = var_r7_3 + 0xF30B;
                }
                Func_08019000((struct Work_80190000 *) temp_r9, var_r6_4, ((FIELD_AT(temp_r9, u16 *, 8) - temp_r0_5) + var_r7_3) - 2, -1, 0);
                var_r7_3 += 1;
                goto loop_65;
            }
        }
        Func_08022768(FIELD_AT(temp_r9, u16 *, 0xC) + 1, FIELD_AT(temp_r9, u16 *, 0xE) + (var_sl * 2) + 1, FIELD_AT(temp_r9, u16 *, 8) - 2, 1, 0xE);
        *sp8 = 1;
        FIELD_AT(sp48, u8 *, 0xEA6) = 0U;
    }
    if (sp34 > 5) {
        var_r7_4 = 0;
        temp_r3_4 = sp34 + 4;
loop_74:
        temp_r0_6 = Func_080022ec(temp_r3_4, 5);
        if (var_r7_4 < temp_r0_6) {
            var_r6_5 = var_r7_4 + 0xF301;
            if (((*(s32 *)0x03001AE8 & 0x100) || ((u32) (*(s32 *)0x03001E40 & 0xF) <= 0xBU)) && (var_r7_4 == Func_080022ec(temp_fp, 5))) {
                var_r6_5 = var_r7_4 + 0xF30B;
            }
            temp_r5_3 = FIELD_AT(temp_r9, u16 *, 8);
            Func_08019000((struct Work_80190000 *) temp_r9, var_r6_5, ((temp_r5_3 - Func_080022ec(temp_r3_4, 5)) + var_r7_4) - 2, -1, 0);
            var_r7_4 += 1;
            goto loop_74;
        }
        temp_r5_4 = *(u32 *)0x03001AE8 & 0x100;
        if (temp_r5_4 == 0) {
            Func_08019000((struct Work_80190000 *) temp_r9, 0xF334, (FIELD_AT(temp_r9, u16 *, 8) - temp_r0_6) - 3, -1, temp_r5_4);
            Func_08019000((struct Work_80190000 *) temp_r9, 0xF335, FIELD_AT(temp_r9, u16 *, 8) - 2, -1, temp_r5_4);
        } else {
            Func_08019000((struct Work_80190000 *) temp_r9, 0xF011, (FIELD_AT(temp_r9, u16 *, 8) - temp_r0_6) - 3, -1, 0);
            Func_08019000((struct Work_80190000 *) temp_r9, 0xF012, FIELD_AT(temp_r9, u16 *, 8) - 2, -1, 0);
        }
        *sp8 |= 2 << ((u32) (FIELD_AT(temp_r9, u16 *, 0xE) - 1) >> 2);
    }
    var_r1_3 = *(s32 *)0x03001C94;
    var_r7_5 = *(s32 *)0x03001B04;
    temp_r2_4 = *(void **)0x03001F34;
    var_r8_3 = *(u32 *)0x03001AE8;
    if (FIELD_AT(temp_r2_4, s32 *, 0xD8) != 0) {
        temp_r3_5 = FIELD_AT(temp_r2_4, s32 *, 0xDC);
        var_r1_3 = 0;
        var_r7_5 = 0;
        var_r8_3 = 0;
        if (temp_r3_5 == 0) {
            FIELD_AT(temp_r2_4, s32 *, 0xDC) = 0x3C;
            var_r7_5 = 1;
            var_r1_3 = 1;
        } else {
            FIELD_AT(temp_r2_4, s32 *, 0xDC) = (s32) (temp_r3_5 - 1);
        }
    }
    temp_r2_5 = *(void **)0x03001F34;
    if ((FIELD_AT(temp_r2_5, s32 *, 0x4C) == 0) || (2 & var_r1_3)) {
        Func_080f9010(0x71);
        var_r6 = -1;
    } else if (1 & var_r1_3) {
        if (sp34 != 0) {
            temp_r0_7 = *(((temp_fp + var_sl) * 4) + sp38);
            temp_r5_5 = temp_r0_7 & 0x3E0000;
            if (temp_r5_5 == 0) {
                if (sp18 == 0) {
                    FIELD_AT(temp_r2_5, s32 *, 0x34) = temp_fp;
                    FIELD_AT(temp_r2_5, s32 *, 0x30) = var_sl;
                    var_r6 = temp_r0_7;
                    FIELD_AT(temp_r2_5, s32 *, 0x38) = sp24;
                } else {
                    goto block_93;
                }
            } else {
                if (sp18 == 0) {
                    Func_08016738();
                    Func_080198dc();
                    Func_08019908((u32) (temp_r5_5 & 0x3E0000) >> 0x11, 5U);
                    Func_0801965c(0x898, spC, 0x34);
                    Func_08017aa4(spC, sp2C, 0, 4);
                }
block_93:
                Func_080f9010(0x72);
                goto block_95;
            }
        } else {
            var_r6 = -1;
        }
    } else {
block_95:
        if (sp34 == 0) {

        } else if (0x80 & var_r7_5) {
            Func_080f9010(0x6F);
            var_sl += 1;
            if ((var_sl == 5) || ((temp_fp + var_sl) == sp34)) {
                var_sl = 0;
            }
            sp24 = var_sl;
        } else if (0x40 & var_r7_5) {
            Func_080f9010(0x6F);
            var_sl -= 1;
            if (var_sl < 0) {
                if (temp_fp == (Func_080022ec(sp34 - 1, 5) * 5)) {
                    var_r3 = (sp34 - temp_fp) - 1;
                } else {
                    var_r3 = 4;
                }
                var_sl = var_r3;
            }
            sp24 = var_sl;
        } else if (0x100 & var_r8_3) {
            if (sp44 != 0) {
                var_r5_2 = 0;
                if (sp50 > 0) {
                    do {
                        var_r1_4 = var_r5_2 + 0xF301;
                        if (((*(u32 *)0x03001E40 & 0xF) <= 0xBU) &&
                                (var_r5_2 == (sp20 - 1))) {
                            var_r1_4 = sp20 + 0xF30A;
                        }
                        Func_08019000((struct Work_80190000 *) sp44, var_r1_4, ((FIELD_AT(sp44, u16 *, 8) - sp50) + var_r5_2) - 2, -1, 0);
                        var_r5_2 += 1;
                    } while (var_r5_2 < sp50);
                }
                Func_08019000((struct Work_80190000 *) sp44, 0xF334, (FIELD_AT(sp44, u16 *, 8) - sp50) - 3, -1, 0);
                Func_08019000((struct Work_80190000 *) sp44, 0xF335, FIELD_AT(sp44, u16 *, 8) - 2, -1, 0);
                *sp8 |= 2 << ((u16) FIELD_AT(sp44, u16 *, 0xE) >> 2);
            }
            if (sp20 == 0) {
                if (sp50 != 0) {
                    if (sp44 != 0) {
                        Func_080164ac((void *) sp44);
                    }
                    sp20 = 1;
                    sp1C = 1;
                } else {
                    goto block_123;
                }
            } else {
block_123:
                if (sp20 > sp50) {
                    sp20 = sp50;
                }
                if (sp20 == 0) {

                } else {
                    if (0x10 & var_r7_5) {
                        Func_080f9010(0x6F);
                        temp_r1 = sp20 + 1;
                        sp20 = temp_r1;
                        if (temp_r1 > sp50) {
                            var_r2 = 1;
                            goto block_133;
                        }
                        goto block_134;
                    }
                    if (0x20 & var_r7_5) {
                        Func_080f9010(0x6F);
                        temp_r1_2 = sp20 - 1;
                        sp20 = temp_r1_2;
                        if (temp_r1_2 <= 0) {
                            var_r2 = sp50;
block_133:
                            sp20 = var_r2;
                        }
block_134:
                        sp1C = 1;
                    }
                }
            }
        } else if (sp20 != 0) {
            if (sp44 != 0) {
                Func_080164ac((void *) sp44);
            }
            sp20 = 0;
            sp1C = 1;
        } else if (0x10 & var_r7_5) {
            Func_080f9010(0x6F);
            Func_0800352c();
            temp_r3_6 = temp_fp + 5;
            if (temp_r3_6 >= sp34) {
                if (temp_fp != 0) {
                    var_sl = sp24;
                }
            } else {
                var_sl = sp24;
                if (temp_r3_6 == (Func_080022ec(sp34 - 1, 5) * 5)) {
                    var_sl = (sp34 - temp_r3_6) - 1;
                    if (var_sl > sp24) {
                        var_sl = sp24;
                    }
                }
            }
        } else if (0x20 & var_r7_5) {
            Func_080f9010(0x6F);
            Func_0800352c();
            if (temp_fp != 0) {
                var_sl = sp24;
            } else {
                temp_r3_7 = Func_080022ec(sp34 - 1, 5) * 5;
                var_sl = sp24;
                if (temp_r3_7 != 0) {
                    var_sl = (sp34 - temp_r3_7) - 1;
                    if (var_sl > sp24) {
                        var_sl = sp24;
                    }
                }
            }
        }
        temp_r3_8 = (FIELD_AT(temp_r9, u16 *, 0xC) * 8) - 2;
        sp10 = temp_r3_8;
        sp14 = (((var_sl * 2) + FIELD_AT(temp_r9, u16 *, 0xE)) * 8) + 0x14;
        FIELD_AT(sp4, s32 *, 4) = 0x40000000;
        FIELD_AT(sp4, s32 *, 8) = 0;
        FIELD_AT(sp4, s32 *, 8) = (s16) (((u16) FIELD_AT(sp4, s32 *, 8) & 0xFFFFFC00) | (Func_080040d0(sp3C, 0x080310A4) & 0x3FF));
        FIELD_AT(sp4, u16 *, 6) = (u16) ((FIELD_AT(sp4, u16 *, 6) & 0xFFFFFE00) | ((temp_r3_8 + ((*(u32 *)0x03001E40 & 4) >> 1) + 0xFFFA) & 0x1FF));
        FIELD_AT(sp4, s32 *, 4) = (s8) ((sp14 - ((*(u32 *)0x03001E40 & 4) >> 2)) + 0xF8);
        if (sp34 != 0) {
            Func_08003dec(sp4, 0xF2);
        }
        temp_r3_9 = *(void **)0x03001E90;
        temp_r7 = FIELD_AT(temp_r3_9, void **, 0);
        var_r6 = *(u32 *)0x03001E40 & 4;
        if (2 & FIELD_AT(temp_r3_9, u16 *, 0xC)) {
            var_r5_3 = 0;
            do {
                var_r2_2 = 0xF - ((u32) ((0 - var_r6) | var_r6) >> 0x1F);
                if (!((1 << var_r5_3) & sp28)) {
                    var_r2_2 = 0xF;
                }
                temp_r0_8 = FIELD_AT(temp_r7, u16 *, 0xC) + FIELD_AT(var_r5_3, u8 *, 0x080373E7) + 1;
                temp_r1_3 = FIELD_AT(temp_r7, u16 *, 0xE) + FIELD_AT(var_r5_3, u8 *, 0x080373EB) + 1;
                var_r5_3 += 1;
                Func_08022768(temp_r0_8, temp_r1_3, 2, 2, var_r2_2);
            } while (var_r5_3 <= 3);
        }
        if (*(u32 *)0x03001E40 & 4) {
            var_r6 = 0x03001388;
            ((WordCopy)0x03001388)((void *)0x06006500, (const void *)0x08037308, 0x20);
            ((WordCopy)0x03001388)((void *)0x06006520, (const void *)0x08037308, 0x20);
        } else {
            ((ArmFill)0x03000168)((void *)0x06006500, 0x20, 0x44444444);
            ((WordCopy)0x03001388)((void *)0x06006520, (const void *)0x08037308, 0x20);
        }
        Func_080030f8(1U);
        goto main_loop;
    }
    temp_r1_4 = *(void **)0x03001E90;
    if (2 & FIELD_AT(temp_r1_4, u16 *, 0xC)) {
        temp_r3_10 = FIELD_AT(temp_r1_4, void **, 0);
        Func_08022768(FIELD_AT(temp_r3_10, u16 *, 0xC) + 1, FIELD_AT(temp_r3_10, u16 *, 0xE) + 1, 4, 4, 0xF);
    }
    Func_08003f3c((u32) sp3C);
    Func_08016418(sp2C, 1);
    Func_08016418(sp44, 1);
    Func_08016418(temp_r9, 1);
    Func_0801e318();
    Func_0801e3c8(0);
    Func_08002df0(sp38);
    Func_080030f8(1U);
    return var_r6;
}
