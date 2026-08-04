/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * 0x030001d8 takes ONE argument and returns one.  This was got WRONG in the
 * first pass of this audit (batch 3), which typed it as three because two
 * independent drafts agreed on three -- and agreement between drafts is not
 * evidence, it is a shared inheritance.  Checked properly against the ROM at
 * every site: only r0 is ever set for the call.  The values the drafts had
 * passed as second and third arguments are the compiler's live intermediates
 * from computing r0, which happen to sit in r1 and r2 at the branch.
 *
 * What it DOES is still not asserted, though the evidence is now strong and
 * consistent across eight call sites in four files: the argument is always a
 * sum of squares, the result is always consumed as a length -- a distance
 * comparison, or shifted right by 8 as a 16.16 magnitude.  That reads as a
 * square root.  Left as a comment for the exact reconstruction to settle, not a name.
 *
 * 0x03000380 is NOT established.  Its two call sites in the tree (here and
 * semantic/main/08090658.c) are byte-identical instruction sequences.  r0 and
 * r1 are unambiguously deliberate -- r1 is freshly loaded with
 * `movs r1,#0; ldrsb r1,[r4,r1]` immediately before the branch.  r2 is live
 * but was computed as an input to r0, so it may be an argument or may be a
 * leftover; typed as three because three is the safer reproduction, with the
 * doubt recorded here rather than resolved by guess.
 */
#include "types.h"

typedef s32 (*Resident_030001D8)(s32 value);
typedef s32 (*Resident_03000380)(s32 arg0, s32 arg1, s32 arg2);

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);
s32 Func_08004458(void);
void Func_08004278(void *);
void Func_08015268(void *);
void *Func_0808ba1c(u32);

/*
 * Rebuild the inactive scanline-effect buffer for the active field transition.
 *
 * The state selects one of several gradient, iris, and perspective profiles.
 * Each profile emits 160 horizontal-window pairs, then the completed buffer is
 * handed to the display scheduler by toggling the active-buffer index.
 */
void Func_0808f52c(void) {
    u16 *sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    void *sp14;
    s32 temp_r0_2;
    s32 temp_r0_4;
    s32 temp_r0_5;
    s32 temp_r0_7;
    s32 temp_r0_9;
    s32 temp_r2_2;
    s32 temp_r2_3;
    s32 temp_r3_5;
    s32 temp_r3_6;
    s32 temp_r5_10;
    s32 temp_r5_12;
    s32 temp_r5_13;
    s32 temp_r5_15;
    s32 temp_r5_16;
    s32 temp_r5_17;
    s32 temp_r5_19;
    s32 temp_r5_20;
    s32 temp_r5_21;
    s32 temp_r5_23;
    s32 temp_r5_24;
    s32 temp_r5_4;
    s32 temp_r5_5;
    s32 temp_r5_6;
    s32 temp_r5_7;
    s32 temp_r6_2;
    s32 temp_r6_4;
    s32 temp_r6_5;
    s32 temp_r6_6;
    s32 temp_r7_2;
    s32 temp_r7_3;
    s32 temp_r7_4;
    s32 temp_r7_5;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r0_4;
    s32 var_r0_5;
    s32 var_r0_6;
    s32 var_r0_7;
    s32 var_r0_8;
    s32 var_r0_9;
    s32 var_r1;
    s32 var_r2;
    s32 var_r3;
    s32 var_r3_3;
    s32 var_r3_6;
    s32 var_r3_7;
    s32 var_r3_8;
    s32 var_r3_9;
    s32 var_r5;
    s32 var_r5_3;
    s32 var_r5_4;
    s32 var_r5_5;
    s32 var_r5_6;
    s32 var_r6;
    s32 var_r6_2;
    s32 var_r6_3;
    s32 var_r6_4;
    s32 var_r6_5;
    s32 var_r9;
    s32 var_r9_3;
    s8 temp_r2;
    s8 temp_r3_2;
    s8 temp_r3_7;
    u16 *temp_r4;
    u16 *temp_r4_2;
    u16 *var_r4;
    u16 *var_r4_2;
    u16 *var_r4_3;
    u16 *var_r4_4;
    u16 *var_r4_5;
    u16 *var_r4_6;
    u16 *var_r4_7;
    u16 *var_r4_8;
    u16 *var_r4_9;
    u16 temp_r0;
    u16 temp_r5;
    u16 temp_r5_11;
    u16 temp_r5_14;
    u16 temp_r5_18;
    u16 temp_r5_22;
    u16 temp_r5_3;
    u16 temp_r5_8;
    u16 temp_r7;
    u16 var_r3_2;
    u16 var_r3_4;
    u16 var_r3_5;
    u16 var_r5_2;
    u16 var_r9_2;
    u32 temp_r3_3;
    u32 temp_r5_9;
    u32 var_r0;
    u32 var_r8;
    u32 var_r8_2;
    u32 var_r8_3;
    u32 var_r8_4;
    u32 var_r8_5;
    u32 var_r8_6;
    u32 var_r8_7;
    u32 var_r8_8;
    u32 var_r8_9;
    u8 temp_r3_4;
    u8 temp_r5_2;
    u8 temp_r6_3;
    void *temp_r0_3;
    void *temp_r0_6;
    void *temp_r0_8;
    void *temp_r2_4;
    void *temp_r3;
    void *temp_r6;
    void *var_sl;

    temp_r6 = M2C_FIELD((void *)0x03001E70, void **, 0);
    temp_r3 = M2C_FIELD((void *)0x03001E70, void **, 0x5C);
    sp14 = temp_r3;
    temp_r2 = M2C_FIELD(temp_r3, s8 *, 0x53C);
    if (temp_r2 != 0) {
        if ((s32) M2C_FIELD(temp_r3, s8 *, 0x53D) >= (s32) temp_r2) {
            M2C_FIELD(temp_r3, s8 *, 0x53C) = 0;
            if (M2C_FIELD(sp14, s8 *, 0x53E) == 0) {
                if ((s8) M2C_FIELD(sp14, u8 *, 0x53B) == 0x40) {
                    var_r3 = 0x81FF;
                } else {
                    var_r3 = 0x9FFF;
                }
                *(u16 *)0x04000000 &= var_r3;
                Func_08004278((void *)0x0808F499);
                Func_08004278((void *)0x0808F52D);
                M2C_FIELD((void *)0x040000B0, u16 *, 0xA) = (u16) (0xC5FF & M2C_FIELD((void *)0x040000B0, u16 *, 0xA));
                M2C_FIELD((void *)0x040000B0, u16 *, 0xA) = (u16) (0x7FFF & M2C_FIELD((void *)0x040000B0, u16 *, 0xA));
                return;
            }
            M2C_FIELD(sp14, u16 *, 0x528) = (u16) temp_r2;
        } else {
            temp_r2_2 = (s8) M2C_FIELD(sp14, u8 *, 0x53B) - M2C_FIELD(sp14, s8 *, 0x53A);
            temp_r3_2 = (u8) M2C_FIELD(temp_r3, s8 *, 0x53D) + 1;
            M2C_FIELD(temp_r3, s8 *, 0x53D) = temp_r3_2;
            M2C_FIELD(sp14, u16 *, 0x52A) = (u16) (M2C_FIELD(sp14, s8 *, 0x53A) + ((Resident_03000380)0x03000380)(temp_r3_2 * temp_r2_2, (s32) M2C_FIELD(temp_r3, s8 *, 0x53C), temp_r2_2));
        }
    }
    temp_r4 = sp14 + ((1 ^ M2C_FIELD(sp14, u8 *, 0x539)) * 0x284);
    sp0 = temp_r4;
    Func_08015268(temp_r4 + 4);
    if ((u32) M2C_FIELD(sp14, u16 *, 0x528) > 0x4DU) {
        return;
    }
    switch (M2C_FIELD(sp14, u16 *, 0x528)) {
    case 0x0:
        M2C_FIELD(temp_r4, u16 *, 0) = 0x7F7F;
        M2C_FIELD(temp_r4, u16 *, 2) = 1U;
        temp_r5 = M2C_FIELD(sp14, u16 *, 0x52A);
        var_r4 = temp_r4 + 2 + 2;
        if (temp_r5 & 0x20) {
            var_r5 = 0x20 - (0x1F & temp_r5);
        } else {
            var_r5 = temp_r5 & 0x1F;
        }
        temp_r5_2 = M2C_FIELD(var_r5, u8 *, 0x0809E8AC);
        var_r8 = 0;
        temp_r6_2 = 0xF1 - temp_r5_2;
        do {
            sp0 = var_r4;
            temp_r3_3 = (u32) (temp_r6_2 * Func_08004458()) >> 0x10;
            var_r8 += 1;
            *var_r4 = (temp_r3_3 << 8) | (temp_r3_3 + temp_r5_2);
            var_r4 += 4;
        } while (var_r8 <= 0x9FU);
        break;
    case 0x1:
        temp_r5_3 = M2C_FIELD(sp14, u16 *, 0x52A);
        temp_r2_3 = (0x1F & temp_r5_3) * 0x1C;
        temp_r2_4 = (void *)(temp_r2_3 + 0x0809F840);
        if (0x20 & temp_r5_3) {
            M2C_FIELD(temp_r4, u16 *, 0) = M2C_FIELD(temp_r2_3, u16 *, 0x0809F840);
            var_r3_2 = M2C_FIELD(temp_r2_4, u16 *, 2);
        } else {
            M2C_FIELD(temp_r4, u16 *, 0) = M2C_FIELD(temp_r2_4, u16 *, 2);
            var_r3_2 = M2C_FIELD(temp_r2_3, u16 *, 0x0809F840);
        }
        M2C_FIELD(temp_r4, u16 *, 2) = var_r3_2;
        var_r4_2 = temp_r4 + 2 + 2;
        var_r8_2 = 0;
        var_sl = temp_r2_4 + 4;
        do {
            temp_r7 = M2C_FIELD(var_sl, u16 *, 0);
            temp_r0 = M2C_FIELD(var_sl, u16 *, 2);
            if (temp_r7 != 0) {
                if (temp_r0 != 0) {
                    var_r9 = 0;
                    if ((s32) temp_r7 > 0) {
                        do {
                            var_r9 += 1;
                            *var_r4_2 = temp_r0;
                            var_r4_2 += 4;
                        } while (var_r9 < (s32) temp_r7);
                    }
                } else {
                    temp_r3_4 = M2C_FIELD(var_sl, u8 *, 4);
                    temp_r6_3 = M2C_FIELD(var_sl, u8 *, 6);
                    if (temp_r7 != 0) {
                        sp10 = M2C_FIELD(var_sl, u8 *, 5) - temp_r3_4;
                        spC = M2C_FIELD(var_sl, u8 *, 7) - temp_r6_3;
                        var_r2 = 0;
                        var_r3_3 = 0;
                        var_r9_2 = temp_r7;
                        do {
                            sp8 = var_r2;
                            sp4 = var_r3_3;
                            sp0 = var_r4_2;
                            temp_r5_4 = Func_080022ec(var_r3_3, (s32) temp_r7);
                            *var_r4_2 = ((temp_r5_4 + temp_r3_4) << 8) + (temp_r6_3 + Func_080022ec(var_r2, (s32) temp_r7));
                            var_r9_2 -= 1;
                            var_r2 += spC;
                            var_r4_2 += 4;
                            var_r3_3 += sp10;
                        } while (var_r9_2 != 0);
                    }
                }
            }
            var_r8_2 += 1;
            var_sl += 8;
        } while (var_r8_2 <= 2U);
        break;
    case 0x2:
        temp_r5_5 = M2C_FIELD(sp14, u16 *, 0x52A) - 1;
        if (0x20 & temp_r5_5) {
            M2C_FIELD(temp_r4, u16 *, 0) = 1;
            var_r3_4 = 0x7F7F;
        } else {
            M2C_FIELD(temp_r4, u16 *, 0) = 0x7F7F;
            var_r3_4 = 1;
        }
        M2C_FIELD(temp_r4, u16 *, 2) = var_r3_4;
        var_r4_3 = temp_r4 + 2 + 2;
        var_r8_3 = 0;
        do {
            sp0 = var_r4_3;
            var_r0 = ((temp_r5_5 & 0x1F) * 0x10) + ((u32) (Func_08004458() * 0x10) >> 0x10);
            if (var_r0 > 0xFFU) {
                var_r0 = 0xFF;
            }
            var_r8_3 += 1;
            *var_r4_3 = (u16) var_r0;
            var_r4_3 += 4;
        } while (var_r8_3 <= 0x9FU);
        break;
    case 0x3:
        var_r5_2 = M2C_FIELD(sp14, u16 *, 0x52A);
        M2C_FIELD(temp_r4, u16 *, 0) = 0x7F7F;
        temp_r4_2 = temp_r4 + 2;
        M2C_FIELD(temp_r4, u16 *, 2) = 1U;
        var_r4_4 = temp_r4_2 + 2;
        if ((u32) var_r5_2 > 0x20U) {
            M2C_FIELD(temp_r4_2, s16 *, 2) = 1;
            M2C_FIELD(var_r4_4, s16 *, 2) = 0x7F7F;
            var_r5_2 = 0x40 - var_r5_2;
            var_r4_4 = var_r4_4 + 2 + 2;
        }
        temp_r5_6 = var_r5_2 * 5;
        var_r8_4 = 0;
        do {
            temp_r5_7 = var_r8_4 - 0x50;
            sp0 = var_r4_4;
            temp_r0_2 = ((Resident_030001D8)0x030001d8)(((temp_r5_6 * temp_r5_6) << 0x10) - ((temp_r5_7 * temp_r5_7) << 0x10)) >> 8;
            var_r6 = 0x78 - temp_r0_2;
            var_r0_2 = temp_r0_2 + 0x78;
            if (var_r6 < 0) {
                var_r6 = 0;
            }
            if (var_r0_2 < 0) {
                var_r0_2 = 0;
            }
            if (var_r6 > 0xF0) {
                var_r6 = 0xF0;
            }
            if (var_r0_2 > 0xF0) {
                var_r0_2 = 0xF0;
            }
            var_r8_4 += 1;
            *var_r4_4 = (var_r6 << 8) + var_r0_2;
            var_r4_4 += 4;
        } while (var_r8_4 <= 0x9FU);
        break;
    case 0x4:
        temp_r5_8 = M2C_FIELD(sp14, u16 *, 0x52A);
        if (0x20 & temp_r5_8) {
            M2C_FIELD(temp_r4, u16 *, 0) = 1;
            var_r3_5 = 0x7F7F;
        } else {
            M2C_FIELD(temp_r4, u16 *, 0) = 0x7F7F;
            var_r3_5 = 1;
        }
        M2C_FIELD(temp_r4, u16 *, 2) = var_r3_5;
        var_r4_5 = temp_r4 + 2 + 2;
        temp_r5_9 = (u32) ((0x1F & temp_r5_8) * 0xF0) >> 5;
        var_r8_5 = 0;
        do {
            var_r8_5 += 2;
            M2C_FIELD(var_r4_5, u16 *, 0) = (u16) temp_r5_9;
            M2C_FIELD(var_r4_5, s16 *, 4) = (s16) (((0xF0 - temp_r5_9) << 8) + 0xF0);
            var_r4_5 += 8;
        } while (var_r8_5 <= 0x9FU);
        break;
    case 0x7:
        temp_r5_10 = M2C_FIELD(temp_r6, s32 *, 0xE4) & 0xFFFF0000;
        temp_r6_4 = M2C_FIELD((temp_r6 + 0xE4), s32 *, 4) & 0xFFFF0000;
        sp0 = temp_r4;
        temp_r0_3 = (void *) Func_0808ba1c(*(u32 *)0x02000434);
        var_r3_6 = M2C_FIELD(temp_r0_3, s32 *, 8) - temp_r5_10;
        if (var_r3_6 < 0) {
            var_r3_6 += 0xFFFF;
        }
        temp_r7_2 = var_r3_6 >> 0x10;
        var_r0_3 = (M2C_FIELD(temp_r0_3, s32 *, 0x10) - M2C_FIELD(temp_r0_3, s32 *, 0xC)) - temp_r6_4;
        if (var_r0_3 < 0) {
            var_r0_3 += 0xFFFF;
        }
        M2C_FIELD(temp_r4, u16 *, 0) = M2C_FIELD(sp14, u16 *, 0x536);
        M2C_FIELD(temp_r4, u16 *, 2) = (u16) M2C_FIELD(sp14, u16 *, 0x534);
        temp_r5_11 = M2C_FIELD(sp14, u16 *, 0x52A);
        var_r4_6 = temp_r4 + 2 + 2;
        if (!(temp_r5_11 & 0x20)) {
            var_r5_3 = 0x20 - (0x1F & temp_r5_11);
        } else {
            var_r5_3 = temp_r5_11 & 0x1F;
        }
        if (*(u32 *)0x03001E40 & 1) {
            var_r5_3 = 0;
        }
        temp_r5_12 = var_r5_3 * 5;
        var_r8_6 = 0;
        do {
            temp_r5_13 = var_r8_6 - ((var_r0_3 >> 0x10) - 0x10);
            sp0 = var_r4_6;
            temp_r0_4 = ((Resident_030001D8)0x030001d8)(((temp_r5_12 * temp_r5_12) << 0x10) - (temp_r5_13 * temp_r5_13 * 0x18000)) >> 8;
            var_r6_2 = temp_r7_2 - temp_r0_4;
            var_r0_4 = temp_r7_2 + temp_r0_4;
            if (var_r6_2 < 0) {
                var_r6_2 = 0;
            }
            if (var_r0_4 < 0) {
                var_r0_4 = 0;
            }
            if (var_r6_2 > 0xF0) {
                var_r6_2 = 0xF0;
            }
            if (var_r0_4 > 0xF0) {
                var_r0_4 = 0xF0;
            }
            *var_r4_6 = (var_r6_2 << 8) + var_r0_4;
            var_r8_6 += 1;
            var_r4_6 += 4;
        } while (var_r8_6 <= 0x9FU);
        break;
    case 0x8:
        var_r3_7 = M2C_FIELD(sp14, s32 *, 0x52C) - (M2C_FIELD(temp_r6, s32 *, 0xE4) & 0xFFFF0000);
        if (var_r3_7 < 0) {
            var_r3_7 += 0xFFFF;
        }
        temp_r7_3 = var_r3_7 >> 0x10;
        var_r1 = M2C_FIELD(sp14, s32 *, 0x530) - (M2C_FIELD((temp_r6 + 0xE4), s32 *, 4) & 0xFFFF0000);
        if (var_r1 < 0) {
            var_r1 += 0xFFFF;
        }
        temp_r3_5 = (var_r1 >> 0x10) - 0x10;
        var_r9_3 = *(u32 *)0x03001E40 - (temp_r3_5 * 2);
        M2C_FIELD(temp_r4, u16 *, 0) = M2C_FIELD(sp14, u16 *, 0x534);
        M2C_FIELD(temp_r4, u16 *, 2) = (u16) M2C_FIELD(sp14, u16 *, 0x536);
        temp_r5_14 = M2C_FIELD(sp14, u16 *, 0x52A);
        var_r4_7 = temp_r4 + 2 + 2;
        if (temp_r5_14 & 0x20) {
            var_r5_4 = 0x20 - (0x1F & temp_r5_14);
        } else {
            var_r5_4 = temp_r5_14 & 0x1F;
        }
        temp_r5_15 = var_r5_4 * 5;
        temp_r3_6 = (temp_r5_15 * temp_r5_15) << 0x10;
        var_r8_7 = 0;
        do {
            temp_r5_16 = var_r8_7 - temp_r3_5;
            sp0 = var_r4_7;
            temp_r0_5 = ((Resident_030001D8)0x030001d8)(temp_r3_6 - (temp_r5_16 * temp_r5_16 * 0x18000)) >> 8;
            var_r6_3 = temp_r7_3 - temp_r0_5;
            var_r0_5 = temp_r7_3 + temp_r0_5;
            if (var_r6_3 < var_r0_5) {
                temp_r3_7 = M2C_FIELD((0x1F & var_r9_3), s8 *, 0x0809E8CE);
                var_r6_3 -= temp_r3_7;
                var_r0_5 += temp_r3_7;
                if (var_r6_3 >= var_r0_5) {
                    var_r6_3 = 0xF0;
                    var_r0_5 = 0xF0;
                }
            }
            if (var_r6_3 < 0) {
                var_r6_3 = 0;
            }
            if (var_r0_5 < 0) {
                var_r0_5 = 0;
            }
            if (var_r6_3 > 0xF0) {
                var_r6_3 = 0xF0;
            }
            if (var_r0_5 > 0xF0) {
                var_r0_5 = 0xF0;
            }
            *var_r4_7 = (var_r6_3 << 8) + var_r0_5;
            var_r8_7 += 1;
            var_r4_7 += 4;
            var_r9_3 += 2;
        } while (var_r8_7 <= 0x9FU);
        break;
    case 0x9:
        temp_r5_17 = M2C_FIELD(temp_r6, s32 *, 0xE4) & 0xFFFF0000;
        temp_r6_5 = M2C_FIELD((temp_r6 + 0xE4), s32 *, 4) & 0xFFFF0000;
        sp0 = temp_r4;
        temp_r0_6 = Func_0808ba1c(*(u32 *)0x02000434);
        var_r3_8 = M2C_FIELD(temp_r0_6, s32 *, 8) - temp_r5_17;
        if (var_r3_8 < 0) {
            var_r3_8 += 0xFFFF;
        }
        temp_r7_4 = var_r3_8 >> 0x10;
        var_r0_6 = (M2C_FIELD(temp_r0_6, s32 *, 0x10) - M2C_FIELD(temp_r0_6, s32 *, 0xC)) - temp_r6_5;
        if (var_r0_6 < 0) {
            var_r0_6 += 0xFFFF;
        }
        M2C_FIELD(temp_r4, u16 *, 0) = M2C_FIELD(sp14, u16 *, 0x534);
        M2C_FIELD(temp_r4, u16 *, 2) = (u16) M2C_FIELD(sp14, u16 *, 0x536);
        temp_r5_18 = M2C_FIELD(sp14, u16 *, 0x52A);
        var_r4_8 = temp_r4 + 2 + 2;
        if (temp_r5_18 & 0x20) {
            var_r5_5 = 0x20 - (0x1F & temp_r5_18);
        } else {
            var_r5_5 = temp_r5_18 & 0x1F;
        }
        temp_r5_19 = var_r5_5 * 5;
        var_r8_8 = 0;
        do {
            temp_r5_20 = var_r8_8 - ((var_r0_6 >> 0x10) - 0x10);
            sp0 = var_r4_8;
            temp_r0_7 = ((Resident_030001D8)0x030001d8)(((temp_r5_19 * temp_r5_19) << 0x10) - (temp_r5_20 * temp_r5_20 * 0x18000)) >> 8;
            var_r6_4 = temp_r7_4 - temp_r0_7;
            var_r0_7 = temp_r7_4 + temp_r0_7;
            if (var_r6_4 < 0) {
                var_r6_4 = 0;
            }
            if (var_r0_7 < 0) {
                var_r0_7 = 0;
            }
            if (var_r6_4 > 0xF0) {
                var_r6_4 = 0xF0;
            }
            if (var_r0_7 > 0xF0) {
                var_r0_7 = 0xF0;
            }
            *var_r4_8 = (var_r6_4 << 8) + var_r0_7;
            var_r8_8 += 1;
            var_r4_8 += 4;
        } while (var_r8_8 <= 0x9FU);
        break;
    case 0x4D:
        temp_r5_21 = M2C_FIELD(temp_r6, s32 *, 0xE4) & 0xFFFF0000;
        temp_r6_6 = M2C_FIELD((temp_r6 + 0xE4), s32 *, 4) & 0xFFFF0000;
        sp0 = temp_r4;
        temp_r0_8 = Func_0808ba1c(*(u32 *)0x02000434);
        var_r3_9 = M2C_FIELD(temp_r0_8, s32 *, 8) - temp_r5_21;
        if (var_r3_9 < 0) {
            var_r3_9 += 0xFFFF;
        }
        temp_r7_5 = var_r3_9 >> 0x10;
        var_r0_8 = (M2C_FIELD(temp_r0_8, s32 *, 0x10) - M2C_FIELD(temp_r0_8, s32 *, 0xC)) - temp_r6_6;
        if (var_r0_8 < 0) {
            var_r0_8 += 0xFFFF;
        }
        M2C_FIELD(temp_r4, u16 *, 0) = M2C_FIELD(sp14, u16 *, 0x534);
        M2C_FIELD(temp_r4, u16 *, 2) = (u16) M2C_FIELD(sp14, u16 *, 0x536);
        temp_r5_22 = M2C_FIELD(sp14, u16 *, 0x52A);
        var_r4_9 = temp_r4 + 2 + 2;
        if (temp_r5_22 & 0x20) {
            var_r5_6 = 0x20 - (0x1F & temp_r5_22);
        } else {
            var_r5_6 = temp_r5_22 & 0x1F;
        }
        temp_r5_23 = var_r5_6 * 5;
        var_r8_9 = 0;
        do {
            temp_r5_24 = var_r8_9 - ((var_r0_8 >> 0x10) - 8);
            sp0 = var_r4_9;
            temp_r0_9 = ((Resident_030001D8)0x030001d8)(((temp_r5_23 * temp_r5_23) << 0x10) - (temp_r5_24 * temp_r5_24 * 0xC000)) >> 8;
            var_r6_5 = temp_r7_5 - temp_r0_9;
            var_r0_9 = temp_r7_5 + temp_r0_9;
            if (var_r6_5 < 0) {
                var_r6_5 = 0;
            }
            if (var_r0_9 < 0) {
                var_r0_9 = 0;
            }
            if (var_r6_5 > 0xF0) {
                var_r6_5 = 0xF0;
            }
            if (var_r0_9 > 0xF0) {
                var_r0_9 = 0xF0;
            }
            *var_r4_9 = (var_r6_5 << 8) + var_r0_9;
            var_r8_9 += 1;
            var_r4_9 += 4;
        } while (var_r8_9 <= 0x9FU);
        break;
    }
    M2C_FIELD(sp14, u8 *, 0x539) = (u8) (M2C_FIELD(sp14, u8 *, 0x539) ^ 1);
}
