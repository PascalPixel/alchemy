/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 *
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320.  A `bl` into that range
 * is an indirect call through the named register.
 *
 * This file declared FOUR phantom functions -- Func_080072f0, Func_080072f4,
 * Func_080072f8 and Func_08007300 -- which are __call_via_r3, r4, r5 and r7.
 * There are three real callees behind them, and the mapping is NOT one
 * prototype to one callee:
 *
 *   __call_via_r3  ->  0x03001388, the word copy (2 sites)
 *                  ->  0x030001d8, the square-root gateway (2 sites)
 *   __call_via_r4  ->  the renderer at 0x03001f08 (6 sites)
 *                  ->  the renderer at 0x03001f0c (2 sites)
 *   __call_via_r5  ->  the renderer at 0x03001f08 (1 site)
 *   __call_via_r7  ->  the renderer at 0x03001f0c (1 site)
 *
 * So the SAME prototype covers two different callees, and two different
 * prototypes cover the same callee.  Every site below was mapped
 * individually; none of it was pattern-matched.
 *
 * The mapping is checkable rather than assumed.  The ten renderer sites in
 * ROM order dispatch r4,r4,r4,r4,r5,r4,r7,r4,r4,r4; the ten renderer calls in
 * source order are f4,f4,f4,f4,f8,f4,7300,f4,f4,f4.  The registers agree at
 * all ten positions, which is what pins each C statement to its ROM site.
 *
 * The two renderer slots are established by the EXACT src/080cef64.c:
 * Func_080ed408(46,7,7,...) publishes at Data_03001e50+184 = 0x03001f08 and
 * (47,7,7,...) at +188 = 0x03001f0c.  This owner loads both once, at
 * 0x080d175a and 0x080d176c, and stashes them -- modelled as the two locals
 * below rather than as a re-read per call.
 *
 * On the square-root gateway: this file's own header already recorded that it
 * "only consumes its first argument", and it was right, while its call sites
 * passed three.  That reading was reached from usage; mine was reached from
 * register liveness at the branch (only r0 is ever set; r1 and r2 hold live
 * intermediates from computing r0).  Two independent methods agreeing is
 * worth more than two drafts agreeing -- which is exactly the trap that made
 * me type this routine with three arguments in batch 3.  Still not asserted
 * as a name; the type says arity and return, and the comment says the rest.
 */
#include "types.h"
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

typedef s32 (*Renderer_080d1714)(s32 context, void *source, s32 x, s32 y,
                                 u32 width, s32 height);
typedef void *(*WordCopy)(void *destination, const void *source, s32 size);
typedef s32 (*SquareRootGateway_080d1714)(s32 sumOfSquares);

/*
 * Complete owner reconstruction for the 400-frame scene renderer.  The body
 * preserves its setup, actor and particle updates, staged tile publishing,
 * sound cues, restoration pass, and teardown.  Calls through the engine's
 * r3-selected publisher gateway are written as their resolved destination
 * operations below; the square-root gateway only consumes its first argument.
 */
s32 Func_08002dd8(s32);
s32 Func_08004c1c(s32);
s32 Func_08004c6c(s32);
s32 Func_08004cb4(s32 *);
s32 Func_08009008(s32, s32 *, s32, s32);
s32 Func_080b5040(s32, s32, s32);
s32 Func_080b50e8(s32);
s32 Func_080cd52c();
s32 Func_080cd594(s32);
s32 Func_080d6750(void *);
s32 Func_080d67dc();
s32 Func_080d6888(s16, s32, s32, s32, s32);
s32 *Func_080b5098(s32);
s32 Func_08002f40(s32);
s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_08002304(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
s32 Func_080030f8(s32);
s32 Func_080041d8();
s32 Func_08004278();
s32 Func_08004458(void);
s32 Func_080044d0(s32, s32);
s32 Func_080049ac(void);
s32 Func_080051d8();
s32 Func_08005340();
s32 Func_08009038();
s32 Func_080b5038();
s32 Func_080dbb24();
s32 Func_080cdbc0(void);
s32 Func_080e3944();
s32 Func_080ed408();
s32 Func_080f9010(s32);

void Func_080d1714(s32 *arg0) {
    s32 sp8;
    s32 spC;
    s32 sp10;
    u32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    void *sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    u32 sp3C;
    u32 sp40;
    s32 sp44;
    s32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 sp54;
    s32 sp58;
    void *sp5C;
    s32 sp60;
    s32 sp64;
    s32 sp68;
    s32 sp6C;
    s32 sp70;
    s32 sp80;
    s32 sp8C;
    s32 sp98;
    s32 spA4;
    s32 spC4;
    s32 sp104;
    s32 sp124;
    s32 sp144;
    s32 *var_r5_8;
    s32 *var_r6;
    s16 temp_r3_13;
    s16 temp_r3_14;
    s16 temp_r3_16;
    s32 *var_r3_2;
    s32 *var_r3_3;
    s32 *var_r4_3;
    s32 *var_r5_5;
    s32 *var_r7_5;
    s32 temp_r0;
    s32 temp_r0_11;
    s32 temp_r0_12;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r0_6;
    s32 temp_r0_7;
    s32 temp_r0_8;
    s32 temp_r0_9;
    s32 temp_r1;
    s32 temp_r1_3;
    s32 temp_r2;
    s32 temp_r2_2;
    s32 temp_r2_3;
    s32 temp_r2_4;
    s32 temp_r2_5;
    s32 temp_r2_6;
    s32 temp_r3_10;
    s32 temp_r3_11;
    s32 temp_r3_12;
    s32 temp_r3_15;
    s32 temp_r3_17;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r3_5;
    s32 temp_r3_6;
    s32 temp_r3_7;
    s32 temp_r3_8;
    s32 temp_r3_9;
    s32 temp_r4;
    s32 temp_r5_10;
    s32 temp_r5_11;
    s32 temp_r5_12;
    s32 temp_r5_2;
    s32 temp_r5_7;
    s32 temp_r5_9;
    s32 temp_r6;
    s32 temp_r6_2;
    s32 temp_r6_3;
    s32 temp_r7;
    s32 temp_sl;
    s32 var_fp;
    s32 var_fp_2;
    s32 var_fp_3;
    s32 var_fp_4;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r0_4;
    s32 var_r0_5;
    s32 var_r0_6;
    s32 var_r1;
    s32 var_r2;
    s32 var_r2_2;
    s32 var_r3;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r4_4;
    s32 var_r5_2;
    s32 var_r5_3;
    s32 var_r5_4;
    s32 var_r7;
    s32 var_r7_3;
    s32 var_r9;
    s32 var_r9_10;
    s32 var_r9_11;
    s32 var_r9_12;
    s32 var_r9_13;
    s32 var_r9_14;
    s32 var_r9_15;
    s32 var_r9_2;
    s32 var_r9_3;
    s32 var_r9_4;
    s32 var_r9_5;
    s32 var_r9_6;
    s32 var_r9_7;
    s32 var_r9_8;
    s32 var_sl;
    s32 var_sl_2;
    u16 *var_r6_2;
    u16 temp_r0_10;
    u16 temp_r3_4;
    u32 temp_r0_5;
    u32 temp_r4_2;
    u32 temp_r4_3;
    u32 temp_r4_4;
    u32 temp_r4_5;
    u32 temp_r5_4;
    u32 temp_r5_8;
    void **var_r5_9;
    void *temp_r0_13;
    void *temp_r1_2;
    void *temp_r2_7;
    void *temp_r3;
    void *temp_r5;
    void *temp_r5_3;
    void *temp_r5_5;
    void *temp_r5_6;
    void *var_r5;
    void *var_r5_6;
    void *var_r5_7;
    void *var_r6_3;
    void *var_r6_4;
    void *var_r6_5;
    void *var_r7_2;
    void *var_r7_4;
    void *var_r9_9;
    /* Loaded once at 0x080d175a and 0x080d176c, as the ROM does. */
    Renderer_080d1714 publish_46 = (Renderer_080d1714)*(u32 *)0x03001f08;
    Renderer_080d1714 publish_47 = (Renderer_080d1714)*(u32 *)0x03001f0c;

    sp64 = M2C_FIELD((void *)0x03001EF0, s32 *, 0);
    sp60 = *(s32 *)0x03001E80;
    temp_r3 = *(void **)0x03001EEC;
    sp5C = temp_r3;
    sp4C = M2C_FIELD((void *)0x03001EF0, s32 *, 4);
    M2C_FIELD(temp_r3, s32 **, 0x7828) = arg0;
    Func_080cd594(0);
    Func_080ed408(0x2E, 7, 7, 3, 2);
    sp54 = M2C_FIELD((void *)0x03001EF0, s32 *, 0x18);
    Func_080ed408(0x2F, 7, 7, 3, 3);
    sp58 = M2C_FIELD((void *)0x03001EF0, s32 *, 0x1C);
    temp_r0 = Func_08002f40(0x82);
    ((WordCopy)0x03001388)((void *)0x05000000, (const void *)temp_r0, 0x80);
    Func_08005340(temp_r0 + 0x80, (s32) sp5C);
    Func_08005340(Func_08002f40(0x73), sp4C);
    M2C_FIELD(sp5C, s32 *, 0x7780) = 2;
    M2C_FIELD(sp5C, s32 *, 0x7784) = 0x32;
    Func_080041d8(0x080CD261, 0x480);
    sp44 = 0x800000;
    sp48 = 0x280000;
    sp3C = 0;
    sp40 = 0;
    var_r9 = 0;
    if (M2C_FIELD(M2C_FIELD(temp_r3, s32 **, 0x7828), s32 *, 0x14) != 0) {
        sp28 = &spA4;
        var_r4 = 0;
        var_r6 = &spC4;
        do {
            sp8 = var_r4;
            temp_r5 = (void *)*Func_080b5098((s32) *((u8 *)M2C_FIELD(sp5C, void **, 0x7828) + ((var_r9 * 2) + 0x24)));
            M2C_FIELD(var_r6, s32 *, 0) = (s32) M2C_FIELD(temp_r5, s32 *, 8);
            M2C_FIELD(var_r6, s32 *, 4) = (s32) M2C_FIELD(temp_r5, s32 *, 0x10);
            *(s32 *)((u8 *)sp28 + var_r4) = (s32) M2C_FIELD(temp_r5, u16 *, 6);
            *(var_r4 + &sp144) = (s32) (u16) Func_080044d0(M2C_FIELD(temp_r5, s32 *, 8), M2C_FIELD(temp_r5, s32 *, 0x10));
            temp_r3_2 = (s32) M2C_FIELD(temp_r5, s32 *, 8) >> 8;
            temp_r3_3 = (s32) M2C_FIELD(temp_r5, s32 *, 0x10) >> 8;
            temp_r1 = temp_r3_3 * temp_r3_3;
            *(var_r4 + &sp124) = ((SquareRootGateway_080d1714)0x030001d8)((temp_r3_2 * temp_r3_2) + temp_r1) >> 7;
            *(var_r4 + &sp104) = 0;
            M2C_FIELD(temp_r5, s32 *, 0x48) = 0;
            var_r9 += 1;
            var_r4 += 4;
            var_r6 += 8;
        } while (var_r9 != M2C_FIELD(M2C_FIELD(sp5C, void **, 0x7828), s32 *, 0x14));
    }
    var_r9_2 = 0;
    var_r5 = sp5C + 0x7080;
    do {
        M2C_FIELD(var_r5, s32 *, 8) = 0x78;
        M2C_FIELD(var_r5, s32 *, 4) = 0;
        Func_08004458();
        M2C_FIELD(var_r5, s32 *, 0x10) = 0;
        M2C_FIELD(var_r5, s32 *, 0xC) = 0;
        var_r9_2 += 1;
        M2C_FIELD(var_r5, s32 *, 0x18) = (s32) (0x3F & Func_08004458());
        var_r5 += 0x1C;
    } while (var_r9_2 != 0x40);
    sp50 = 0;
    sp24 = sp60 + 0xC;
frame_loop:
    if (*(s32 *)0x03001B04 & 3) {
        if (sp50 <= 0x9F) {
            sp50 = 0xA0;
        } else if (sp50 <= 0x18A) {
            sp50 = 0x18B;
        }
    }
    Func_080049ac();
    Func_080051d8(sp60, sp24);
    if (sp50 == 0x10) {
        Func_080f9010(0x8D);
    }
    if (sp50 == 0x100) {
        Func_080f9010(0x8C);
    }
    if (sp50 == 0x14E) {
        Func_080f9010(0xD4);
    }
    if (sp50 == 0x15B) {
        Func_080f9010(0xD4);
    }
    if (sp50 == 0x167) {
        Func_080f9010(0xD4);
    }
    if (sp50 == 0x174) {
        Func_080f9010(0xD4);
    }
    var_r9_3 = 0;
    if (M2C_FIELD(M2C_FIELD(sp5C, void **, 0x7828), s32 *, 0x14) == 0) {

    } else {
        var_r4_2 = 0;
loop_25:
        temp_r7 = var_r9_3 * 0x10;
        if (sp50 <= temp_r7) {

        } else {
            temp_r5_2 = var_r9_3 * 2;
            sp38 = temp_r5_2;
            sp8 = var_r4_2;
            temp_r5_3 = (void *)*Func_080b5098((s32) *((u8 *)M2C_FIELD(sp5C, void **, 0x7828) + (temp_r5_2 + 0x24)));
            temp_sl = var_r4_2;
            M2C_FIELD(temp_r5_3, s32 *, 8) = (s32) ((s32) (*(&sp124 + var_r4_2) * Func_08002322(*(&sp144 + var_r4_2))) >> 1);
            temp_r0_2 = Func_0800231c(*(&sp144 + var_r4_2));
            M2C_FIELD(temp_r5_3, s32 *, 0x10) = (s32) ((s32) (*(&sp124 + var_r4_2) * temp_r0_2) >> 1);
            if (sp50 <= 0x9F) {
                if (sp50 > (s32) (temp_r7 + 0x10)) {
                    *(&sp104 + var_r4_2) = *(&sp104 + var_r4_2) + 0x30;
                }
                if ((s32) *(&sp124 + var_r4_2) <= 0x1F) {
                    var_r3 = M2C_FIELD(temp_r5_3, s32 *, 0xC) + 0x60000;
                } else {
                    var_r3 = M2C_FIELD(temp_r5_3, s32 *, 0xC) + 0x8000;
                }
                M2C_FIELD(temp_r5_3, s32 *, 0xC) = var_r3;
                if ((s32) M2C_FIELD(temp_r5_3, s32 *, 0xC) > 0x7C0000) {
                    M2C_FIELD(temp_r5_3, s32 *, 0xC) = 0x7C0000;
                }
            }
            if (sp50 <= 0x1EA) {
                temp_r2 = *(&sp144 + var_r4_2) + *(&sp104 + var_r4_2);
                *(&sp144 + var_r4_2) = temp_r2;
                if (temp_r2 > 0x10000) {
                    *(&sp144 + temp_sl) = temp_r2 + 0xFFFF0000;
                }
                var_r2 = *(&sp104 + temp_sl);
                if (var_r2 < 0) {
                    var_r2 += 3;
                }
                M2C_FIELD(temp_r5_3, u16 *, 6) = (u16) (M2C_FIELD(temp_r5_3, u16 *, 6) + (var_r2 >> 2));
            }
            if (sp50 == 0x18B) {
                M2C_FIELD(temp_r5_3, s32 *, 0x48) = 0xAB85;
            }
            var_r6_2 = (u16 *)0x080EE16C;
            var_fp = 0;
            do {
                temp_r3_4 = *var_r6_2;
                var_r6_2 += 2;
                if (sp50 == temp_r3_4) {
                    M2C_FIELD(temp_r5_3, s32 *, 0x28) = 0;
                    sp8 = var_r4_2;
                    Func_080d6888(*(s16 *)((u8 *)M2C_FIELD(sp5C, void **, 0x7828) + (sp38 + 0x24)), 7, 5, var_r9_3, 8);
                }
                var_fp += 1;
            } while (var_fp != 4);
            temp_r3_5 = *(&sp124 + var_r4_2);
            if (temp_r3_5 > 0x10) {
                *(&sp124 + temp_sl) = temp_r3_5 - 2;
            }
        }
        var_r9_3 += 1;
        var_r4_2 += 4;
        if (var_r9_3 != M2C_FIELD(M2C_FIELD(sp5C, void **, 0x7828), s32 *, 0x14)) {
            goto loop_25;
        }
    }
    if ((u32) (sp50 - 0x10) <= 0x8FU) {
        var_r5_2 = (sp50 * 2) - 0x20;
        if (var_r5_2 > 0x30) {
            var_r5_2 = 0x30;
        }
        var_r0 = sp50;
        if (var_r0 < 0) {
            var_r0 += 3;
        }
        publish_46(sp64, sp5C + ((Func_080022fc(var_r0 >> 2, 3) * 0xC00) + ((0x30 - var_r5_2) * 0x30)), 0x20, 0x70 - var_r5_2, 0x30U, var_r5_2);
    }
    if ((u32) (sp50 - 0x30) <= 0x6FU) {
        var_r5_3 = (sp50 * 2) - 0x60;
        if (var_r5_3 > 0x40) {
            var_r5_3 = 0x40;
        }
        var_r0_2 = sp50;
        if (var_r0_2 < 0) {
            var_r0_2 += 3;
        }
        publish_46(sp64, sp5C + ((Func_080022fc(var_r0_2 >> 2, 3) * 0xC00) + ((0x40 - var_r5_3) * 0x30)), 0x20, 0x30, 0x30U, var_r5_3);
    }
    temp_r5_4 = sp50 - 0xA0;
    if (temp_r5_4 <= 0xEFU) {
        var_r0_3 = sp50;
        if (var_r0_3 < 0) {
            var_r0_3 += 3;
        }
        temp_r5_5 = sp5C + (Func_080022fc(var_r0_3 >> 2, 3) * 0xC00);
        temp_r1_2 = temp_r5_5;
        sp8 = sp54;
        publish_46(sp64, temp_r1_2, 8, 0, 0x30U, 0x40);
        publish_46(sp64, temp_r5_5, 8, 0x40, 0x30U, 0x40);
    }
    if (sp50 <= 0x9F) {
        var_r9_4 = 0;
        var_r6_3 = sp5C + 0x7080;
        do {
            temp_r4 = M2C_FIELD(var_r6_3, s32 *, 0x18);
            if (temp_r4 == 0) {
                sp8 = temp_r4;
                M2C_FIELD(&sp98, s32 *, 0) = (s32) (M2C_FIELD(var_r6_3, s32 *, 8) * Func_08002322(M2C_FIELD(var_r6_3, s32 *, 0x10)));
                M2C_FIELD(&sp98, s32 *, 4) = (s32) M2C_FIELD(var_r6_3, s32 *, 4);
                M2C_FIELD(&sp98, s32 *, 8) = (s32) (M2C_FIELD(var_r6_3, s32 *, 8) * Func_0800231c(M2C_FIELD(var_r6_3, s32 *, 0x10)));
                Func_080e3944((s32) &sp98, &sp8C);
                temp_r2_2 = (s32) M2C_FIELD(&sp8C, s32 *, 0) >> 1;
                M2C_FIELD(&sp8C, s32 *, 0) = temp_r2_2;
                if ((s32) M2C_FIELD(var_r6_3, s32 *, 4) <= 0x3FFFFF) {
                    publish_46(sp64, sp5C + 0x2400, temp_r2_2 - 0xC, M2C_FIELD(&sp8C, s32 *, 4) - 0xC, 0x10, 0x10);
                }
                temp_r3_6 = M2C_FIELD(var_r6_3, s32 *, 8);
                if (temp_r3_6 > 0x18) {
                    M2C_FIELD(var_r6_3, s32 *, 8) = (s32) (temp_r3_6 - 4);
                }
                temp_r2_3 = M2C_FIELD(var_r6_3, s32 *, 0x10) + (M2C_FIELD(var_r6_3, s32 *, 0xC) * 2);
                M2C_FIELD(var_r6_3, s32 *, 0x10) = temp_r2_3;
                if (temp_r2_3 > 0x10000) {
                    M2C_FIELD(var_r6_3, s32 *, 0x10) = (s32) (temp_r2_3 + 0xFFFF0000);
                }
                temp_r3_7 = M2C_FIELD(var_r6_3, s32 *, 0xC) + 0x32;
                M2C_FIELD(var_r6_3, s32 *, 0xC) = temp_r3_7;
                if (temp_r3_7 > 0x1000) {
                    M2C_FIELD(var_r6_3, s32 *, 0xC) = 0x1000;
                }
                temp_r3_8 = M2C_FIELD(var_r6_3, s32 *, 0x10) + 0x800;
                M2C_FIELD(var_r6_3, s32 *, 0x10) = temp_r3_8;
                temp_r2_4 = M2C_FIELD(var_r6_3, s32 *, 4) + (temp_r3_8 * 2);
                M2C_FIELD(var_r6_3, s32 *, 4) = temp_r2_4;
                if (temp_r2_4 > 0x300000) {
                    M2C_FIELD(var_r6_3, s32 *, 4) = sp8;
                    M2C_FIELD(var_r6_3, s32 *, 8) = 0x64;
                    M2C_FIELD(var_r6_3, s32 *, 0x10) = sp8;
                    M2C_FIELD(var_r6_3, s32 *, 0xC) = sp8;
                }
            } else {
                M2C_FIELD(var_r6_3, s32 *, 0x18) = (s32) (temp_r4 - 1);
            }
            var_r9_4 += 1;
            var_r6_3 += 0x1C;
        } while (var_r9_4 != 0x18);
    }
    if (sp50 != 0xA0) {

    } else {
        Func_080b5040(1, 0x3B, 0);
        Func_080b5038(1, 0x3BU, 8);
        Func_080d6750(M2C_FIELD(sp5C, void **, 0x7828));
        Func_080dbb24(9, 0x178, 2U);
        temp_r0_3 = Func_08002f40(0x88);
        ((WordCopy)0x03001388)((void *)0x05000000, (const void *)temp_r0_3, 0x80);
        Func_08005340(temp_r0_3 + 0x80, (s32) (sp5C + 0x3600));
        var_r3_2 = (s32 *)0x02010018;
        var_r9_5 = 0;
        do {
            var_r9_5 += 1;
            *var_r3_2 = 0;
            var_r3_2 += 0x1C;
        } while (var_r9_5 != 0x200);
        var_r9_6 = 0;
        if (M2C_FIELD(M2C_FIELD(sp5C, void **, 0x7828), s32 *, 0x14) != 0) {
            var_r7 = 0x24;
            do {
                temp_r5_6 = (void *)*Func_080b5098((s32) *((u8 *)M2C_FIELD(sp5C, void **, 0x7828) + var_r7));
                M2C_FIELD(temp_r5_6, s32 *, 0xC) = (s32) (((0xF & Func_08004458()) + 8) << 0x10);
                var_r9_6 += 1;
                var_r7 += 2;
            } while (var_r9_6 != M2C_FIELD(M2C_FIELD(sp5C, void **, 0x7828), s32 *, 0x14));
        }
        M2C_FIELD((void *)0x03001CE0, s32 *, 0xC) = 0x48;
        var_r9_7 = 0;
        var_r7_2 = sp5C + 0x7080;
        do {
            temp_r6 = Func_08004458() & 0x7F;
            temp_r5_7 = (0x7FFF & Func_08004458()) + 0x9FFF;
            M2C_FIELD(var_r7_2, s32 *, 0) = (s32) (temp_r6 * Func_08002322(temp_r5_7));
            M2C_FIELD(var_r7_2, s32 *, 4) = (s32) (temp_r6 * Func_0800231c(temp_r5_7));
            temp_r0_4 = Func_08002304((s32) Func_08004458(), 0xC8) - 0x64;
            var_r9_7 += 1;
            M2C_FIELD(var_r7_2, s32 *, 0x18) = 0;
            M2C_FIELD(var_r7_2, s32 *, 8) = (s32) (temp_r0_4 << 0x10);
            var_r7_2 += 0x1C;
        } while (var_r9_7 != 0x20);
        var_r9_8 = 0;
        if (M2C_FIELD(M2C_FIELD(sp5C, void **, 0x7828), s32 *, 0x14) != 0) {
            var_r2_2 = 0;
            do {
                *(var_r2_2 + &sp124) = 0x10;
                *(var_r2_2 + &sp104) = 0x1770;
                var_r9_8 += 1;
                var_r2_2 += 4;
            } while (var_r9_8 != M2C_FIELD(M2C_FIELD(sp5C, void **, 0x7828), s32 *, 0x14));
        }
    }
    if (sp50 <= 0x9F) {

    } else {
        sp68 = M2C_FIELD((void *)0x080EDA78, s32 *, 0);
        sp6C = M2C_FIELD((void *)0x080EDA78, s32 *, 4);
        var_r5_4 = 0x10;
        if ((s32) temp_r5_4 <= 0x40) {
            var_r5_4 = 0x20;
        }
        sp44 -= (s32) (Func_08002322(temp_r5_4 << 7) << 5) >> 6;
        temp_r3_9 = sp48 + ((s32) (var_r5_4 * Func_08002322(temp_r5_4 << 9)) >> 6);
        sp48 = temp_r3_9;
        temp_r5_8 = sp3C;
        sp3C = (s32) ((sp3C >> 0x1F) + sp3C) >> 1;
        temp_r0_5 = sp40;
        sp40 = (s32) ((sp40 >> 0x1F) + sp40) >> 1;
        var_r4_3 = &sp68;
        sp68 = 0x10000;
        M2C_FIELD(var_r4_3, s32 *, 4) = 0x10000;
        M2C_FIELD(&sp70, s32 *, 0xC) = 0;
        sp48 = temp_r0_5 + temp_r3_9;
        sp44 += temp_r5_8;
        var_r5_5 = sp5C + 0x77D8;
        var_r9_9 = NULL;
        var_r7_3 = 0xFF0000;
        do {
            M2C_FIELD(&sp70, s32 *, 0) = (s32) ((M2C_FIELD(var_r9_9, u8 *, 0x080EE15A) << 0x10) + sp44 + 0x280000);
            M2C_FIELD(&sp70, s32 *, 4) = var_r7_3;
            M2C_FIELD(&sp70, s32 *, 8) = (s32) (((M2C_FIELD(var_r9_9, u8 *, 0x080EE163) + ((s32) var_r9_9 << 6)) << 0x10) + sp48);
            temp_r0_6 = *var_r5_5;
            var_r5_5 += 4;
            sp8 = (s32)var_r4_3;
            Func_08009008(temp_r0_6, &sp70, (s32) var_r4_3, 0);
            var_r9_9 += 1;
            var_r7_3 += 0x400000;
            var_r4_3 = (s32 *)sp8;
        } while (var_r9_9 != (void *)9);
        if (sp50 <= 0xFF) {

        } else {
            M2C_FIELD(&sp80, s32 *, 8) = 0x01000000;
            M2C_FIELD(&sp80, s32 *, 0) = 0;
            M2C_FIELD(&sp80, s32 *, 4) = 0;
            var_sl = 0;
            Func_080049ac();
            Func_08004cb4(&sp80);
            if (sp50 > 0x149) {

            } else {
                var_r9_10 = 0;
                var_r6_4 = sp5C + 0x7080;
loop_105:
                temp_r3_10 = (s32) M2C_FIELD(var_r6_4, s32 *, 0) >> 8;
                temp_r3_11 = (s32) M2C_FIELD(var_r6_4, s32 *, 4) >> 8;
                temp_r2_5 = temp_r3_11 * temp_r3_11;
                temp_r3_12 = (s32) M2C_FIELD(var_r6_4, s32 *, 8) >> 8;
                temp_r1_3 = temp_r3_12 * temp_r3_12;
                temp_r0_7 = ((SquareRootGateway_080d1714)0x030001d8)((temp_r3_10 * temp_r3_10) + temp_r2_5 + temp_r1_3) >> 9;
                if (temp_r0_7 != 0) {
                    Func_080e3944((s32) var_r6_4, &sp8C);
                    M2C_FIELD(&sp8C, s32 *, 0) = (s32) (((s32) M2C_FIELD(&sp8C, s32 *, 0) >> 0x11) + (sp44 >> 0x11) + 0x20);
                    M2C_FIELD(&sp8C, s32 *, 4) = (s32) ((M2C_FIELD(&sp8C, s16 *, 6) + (sp48 >> 0x10)) - 4);
                    temp_r3_13 = M2C_FIELD(&sp8C, s16 *, 0xA);
                    M2C_FIELD(&sp8C, s32 *, 8) = (s32) temp_r3_13;
                    if ((s32) temp_r3_13 <= 0xA9) {
                        M2C_FIELD(&sp8C, s32 *, 8) = 0xAA;
                    }
                    var_r0_4 = M2C_FIELD(&sp8C, s32 *, 8);
                    if (var_r0_4 > 0x15E) {
                        M2C_FIELD(&sp8C, s32 *, 8) = 0x15E;
                        var_r0_4 = 0x15E;
                    }
                    temp_r4_2 = 6 - Func_080022ec(var_r0_4 - 0xAA, 0x24);
                    temp_r0_8 = temp_r4_2 * 2;
                    publish_47(sp64, sp4C + M2C_FIELD((temp_r0_8 - 2), u16 *, 0x080EDE48), M2C_FIELD(&sp8C, s32 *, 0) - ((s32) (temp_r4_2 + (temp_r4_2 >> 0x1F)) >> 1), M2C_FIELD(&sp8C, s32 *, 4) - temp_r4_2, temp_r4_2, temp_r0_8);
                    temp_r5_9 = M2C_FIELD(var_r6_4, s32 *, 0);
                    M2C_FIELD(var_r6_4, s32 *, 0) = (s32) (temp_r5_9 - Func_080022ec(temp_r5_9, temp_r0_7));
                    temp_r5_10 = M2C_FIELD(var_r6_4, s32 *, 4);
                    M2C_FIELD(var_r6_4, s32 *, 4) = (s32) (temp_r5_10 - Func_080022ec(temp_r5_10, temp_r0_7));
                    temp_r5_11 = M2C_FIELD(var_r6_4, s32 *, 8);
                    M2C_FIELD(var_r6_4, s32 *, 8) = (s32) (temp_r5_11 - Func_080022ec(temp_r5_11, temp_r0_7));
                } else {
                    var_sl += 1;
                }
                var_r9_10 += 1;
                var_r6_4 += 0x1C;
                if (var_r9_10 != 0x20) {
                    goto loop_105;
                }
                if (var_sl > 0) {
                    temp_r4_3 = Func_080022ec(var_sl, 0xA) + 1;
                    temp_r0_9 = temp_r4_3 * 2;
                    publish_47(sp64, sp4C + M2C_FIELD((temp_r0_9 - 2), u16 *, 0x080EDE48), ((sp44 >> 0x11) - ((s32) (temp_r4_3 + (temp_r4_3 >> 0x1F)) >> 1)) + 0x20, ((sp48 >> 0x10) - temp_r4_3) - 4, temp_r4_3, temp_r0_9);
                }
            }
            var_fp_2 = 0;
            var_sl_2 = 0;
loop_117:
            if (sp50 == M2C_FIELD((var_fp_2 * 2), u16 *, 0x080EE16C)) {
                var_r9_11 = 0;
                var_r7_4 = (void *)(var_sl_2 + 0x02010000);
                do {
                    temp_r6_2 = 0x7F & Func_08004458();
                    temp_r0_10 = (u16) Func_08004458();
                    temp_r6_3 = temp_r6_2 + 0x10;
                    M2C_FIELD(var_r7_4, s32 *, 0xC) = (s32) ((s32) (temp_r6_3 * Func_08002322((s32) temp_r0_10)) >> 6);
                    M2C_FIELD(var_r7_4, s32 *, 0x10) = (s32) ((s32) (0 - (temp_r6_3 * Func_0800231c((s32) temp_r0_10))) >> 6);
                    M2C_FIELD(var_r7_4, s32 *, 0x10) = (s32) (((0xFF & Func_08004458()) - 0x80) << 0xA);
                    M2C_FIELD(var_r7_4, s32 *, 0) = 0;
                    M2C_FIELD(var_r7_4, s32 *, 4) = 0;
                    M2C_FIELD(var_r7_4, s32 *, 8) = 0;
                    var_r9_11 += 1;
                    M2C_FIELD(var_r7_4, s32 *, 0x18) = (s32) ((0xF & Func_08004458()) + 0x40);
                    var_r7_4 += 0x1C;
                } while (var_r9_11 != 0x60);
            }
            if (sp50 >= (s32) *(u16 *)0x080EE16C) {
                var_r9_12 = 0;
                var_r5_6 = (void *)0x02010000;
                do {
                    if ((s32) M2C_FIELD(var_r5_6, s32 *, 0x18) > 0) {
                        Func_080e3944((s32) var_r5_6, &sp8C);
                        M2C_FIELD(&sp8C, s32 *, 0) = (s32) (((s32) M2C_FIELD(&sp8C, s32 *, 0) >> 0x11) + 0x20);
                        M2C_FIELD(&sp8C, s32 *, 4) = (s32) (M2C_FIELD(&sp8C, s16 *, 6) + 0x38);
                        temp_r3_14 = M2C_FIELD(&sp8C, s16 *, 0xA);
                        M2C_FIELD(&sp8C, s32 *, 8) = (s32) temp_r3_14;
                        if ((s32) temp_r3_14 <= 0xA9) {
                            M2C_FIELD(&sp8C, s32 *, 8) = 0xAA;
                        }
                        var_r0_5 = M2C_FIELD(&sp8C, s32 *, 8);
                        if (var_r0_5 > 0x15E) {
                            M2C_FIELD(&sp8C, s32 *, 8) = 0x15E;
                            var_r0_5 = 0x15E;
                        }
                        temp_r4_4 = 3 - Func_080022ec(var_r0_5 - 0xAA, 0x5A);
                        temp_r0_11 = temp_r4_4 * 2;
                        publish_46(sp64, sp4C + M2C_FIELD((temp_r0_11 - 2), u16 *, 0x080EDE48), M2C_FIELD(&sp8C, s32 *, 0) - ((s32) (temp_r4_4 + (temp_r4_4 >> 0x1F)) >> 1), M2C_FIELD(&sp8C, s32 *, 4) - temp_r4_4, temp_r4_4, temp_r0_11);
                        M2C_FIELD(var_r5_6, s32 *, 0) = (s32) (M2C_FIELD(var_r5_6, s32 *, 0) + M2C_FIELD(var_r5_6, s32 *, 0xC));
                        temp_r2_6 = M2C_FIELD(var_r5_6, s32 *, 0x10);
                        M2C_FIELD(var_r5_6, s32 *, 4) = (s32) (M2C_FIELD(var_r5_6, s32 *, 4) + temp_r2_6);
                        M2C_FIELD(var_r5_6, s32 *, 8) = (s32) (M2C_FIELD(var_r5_6, s32 *, 8) + temp_r2_6);
                        M2C_FIELD(var_r5_6, s32 *, 0x18) = (s32) (M2C_FIELD(var_r5_6, s32 *, 0x18) - 1);
                    }
                    var_r9_12 += 1;
                    var_r5_6 += 0x1C;
                } while (var_r9_12 != 0x200);
            }
            var_fp_2 += 1;
            var_sl_2 += 0xE00;
            if (var_fp_2 != 4) {
                goto loop_117;
            }
            var_r9_13 = 0;
            sp18 = 0;
            var_r6_5 = sp5C + 0x7710;
            sp1C = 0x7720;
            sp14 = sp50 + 0xFFFFFEB6;
            sp10 = 0x14A;
            sp34 = (sp44 >> 0x11) + 0x20;
loop_133:
            if (sp50 == sp10) {
                temp_r3_15 = (sp48 >> 0x10) - 4;
                M2C_FIELD(var_r6_5, s32 *, 4) = temp_r3_15;
                M2C_FIELD(var_r6_5, s32 *, 0x10) = temp_r3_15;
                M2C_FIELD(var_r6_5, s32 *, 0) = sp34;
                M2C_FIELD(var_r6_5, s32 *, 0xC) = sp34;
                var_fp_3 = 0;
                var_r3_3 = sp5C + 0x7418;
                do {
                    var_fp_3 += 1;
                    *var_r3_3 = 4;
                    var_r3_3 += 0x1C;
                } while (var_fp_3 != 0x1C);
                sp40 = 0xFFFE0000;
                sp3C = 0x80000;
                M2C_FIELD(sp5C, s32 *, 0x77A8) = 8;
            }
            if (sp50 < sp10) {

            } else {
                var_r4_4 = (s32) (sp14 + (sp14 >> 0x1F)) >> 1;
                if (var_r4_4 > 2) {
                    var_r4_4 = 2;
                }
                publish_47(sp64, sp5C + M2C_FIELD((var_r4_4 * 2), u16 *, 0x080EE17E), M2C_FIELD(var_r6_5, s32 *, 0), M2C_FIELD(var_r6_5, s32 *, 4) - M2C_FIELD(var_r4_4, u8 *, 0x080EE17A), (u32) M2C_FIELD(var_r4_4, u8 *, 0x080EE174), (s32) M2C_FIELD(var_r4_4, u8 *, 0x080EE177));
                M2C_FIELD(var_r6_5, s32 *, 0) = (s32) (M2C_FIELD(var_r6_5, s32 *, 0) - 8);
                M2C_FIELD(var_r6_5, s32 *, 4) = (s32) (M2C_FIELD(var_r6_5, s32 *, 4) + 2);
                if (sp50 < (s32) (sp10 + 8)) {
                    Func_08004c6c(0xFFFFF800);
                    Func_08004c1c(0xFFFFF000);
                    var_fp_4 = 0;
                    sp30 = sp18;
                    sp2C = sp1C;
                    sp20 = 0;
                    var_r5_7 = sp5C + 0x7400;
                    do {
                        M2C_FIELD(&sp98, s32 *, 0) = 0;
                        M2C_FIELD(&sp98, s32 *, 4) = (s32) (M2C_FIELD(var_r5_7, s32 *, 0x18) * Func_0800231c(sp20));
                        M2C_FIELD(&sp98, s32 *, 8) = (s32) (M2C_FIELD(var_r5_7, s32 *, 0x18) * Func_08002322(sp20));
                        M2C_FIELD(var_r5_7, s32 *, 0x18) = (s32) (M2C_FIELD(var_r5_7, s32 *, 0x18) + 2);
                        Func_080e3944((s32) &sp98, &sp8C);
                        M2C_FIELD(&sp8C, s32 *, 0) = (s32) (((s32) M2C_FIELD(&sp8C, s32 *, 0) >> 0x11) + *(s32 *)((u8 *)sp5C + sp30 + 0x771C));
                        temp_r3_16 = M2C_FIELD(&sp8C, s16 *, 0xA);
                        M2C_FIELD(&sp8C, s32 *, 4) = (s32) (M2C_FIELD(&sp8C, s16 *, 6) + *(s32 *)((u8 *)sp5C + sp2C));
                        M2C_FIELD(&sp8C, s32 *, 8) = (s32) temp_r3_16;
                        if ((s32) temp_r3_16 <= 0xA9) {
                            M2C_FIELD(&sp8C, s32 *, 8) = 0xAA;
                        }
                        var_r0_6 = M2C_FIELD(&sp8C, s32 *, 8);
                        if (var_r0_6 > 0x15E) {
                            M2C_FIELD(&sp8C, s32 *, 8) = 0x15E;
                            var_r0_6 = 0x15E;
                        }
                        temp_r4_5 = 3 - Func_080022ec(var_r0_6 - 0xAA, 0x5A);
                        temp_r0_12 = temp_r4_5 * 2;
                        publish_46(sp64, sp4C + M2C_FIELD((temp_r0_12 - 2), u16 *, 0x080EDE48), M2C_FIELD(&sp8C, s32 *, 0) - ((s32) (temp_r4_5 + (temp_r4_5 >> 0x1F)) >> 1), M2C_FIELD(&sp8C, s32 *, 4) - temp_r4_5, temp_r4_5, temp_r0_12);
                        var_fp_4 += 1;
                        sp20 += 0x924;
                        var_r5_7 += 0x1C;
                    } while (var_fp_4 != 0x1C);
                }
            }
            var_r9_13 += 1;
            sp1C += 0x1C;
            sp18 += 0x1C;
            sp14 -= 0xC;
            sp10 += 0xC;
            var_r6_5 += 0x1C;
            if (var_r9_13 != 4) {
                goto loop_133;
            }
        }
    }
    Func_080cd52c();
    M2C_FIELD(sp5C, s32 *, 0x7824) = 1;
    Func_080030f8(1U);
    temp_r5_12 = sp50 + 1;
    sp50 = temp_r5_12;
    if (temp_r5_12 != 0x190) {
        /*
         * The binary spells this edge `bl .L81`, but .L81 is inside this
         * owner's already-established stack frame.  The common epilogue
         * restores the entry LR, so it is a loop edge rather than a callable
         * external function.
         */
        goto frame_loop;
    }
    Func_080b50e8(0x86);
    var_r9_14 = 0;
    if (M2C_FIELD(M2C_FIELD(sp5C, void **, 0x7828), s32 *, 0x14) != 0) {
        var_r7_5 = &spA4;
        var_r5_8 = &spC4;
        var_r1 = 0x24;
        do {
            spC = var_r1;
            temp_r2_7 = (void *)*Func_080b5098((s32) *((u8 *)M2C_FIELD(sp5C, void **, 0x7828) + var_r1));
            M2C_FIELD(temp_r2_7, s32 *, 8) = (s32) M2C_FIELD(var_r5_8, s32 *, 0);
            M2C_FIELD(temp_r2_7, s32 *, 0x10) = (s32) M2C_FIELD(var_r5_8, s32 *, 4);
            temp_r3_17 = *var_r7_5;
            var_r7_5 += 4;
            M2C_FIELD(temp_r2_7, s16 *, 6) = (s16) temp_r3_17;
            var_r9_14 += 1;
            var_r5_8 += 8;
            var_r1 += 2;
        } while (var_r9_14 != M2C_FIELD(M2C_FIELD(sp5C, void **, 0x7828), s32 *, 0x14));
    }
    M2C_FIELD((void *)0x03001CE0, s32 *, 0xC) = 0x78;
    Func_080d67dc();
    var_r9_15 = 0;
    var_r5_9 = sp5C + 0x77D8;
    do {
        temp_r0_13 = *var_r5_9;
        var_r5_9 += 4;
        Func_08009038(temp_r0_13);
        var_r9_15 += 1;
    } while (var_r9_15 != 9);
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_080cdbc0();
}
