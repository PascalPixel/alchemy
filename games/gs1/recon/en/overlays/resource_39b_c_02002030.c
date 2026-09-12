#include "types.h"

#define Scene_RunScene39bSequenceA Func_02002030

extern u8 Data_00000000[];
void Func_02003ec6();
void Func_020044fa();
s32 Func_02004588();
s32 Func_02004588_a();
s32 Func_020045a8();
s32 Func_020045be();
void Func_020045ca();
void Func_020045f8();
void Func_020045fa();
void Func_0200461c();
void Func_0200462c();
void Func_02004638();
void Func_0200466e();
void Func_02004676();
void Func_0200467e();
void Func_0200468a();
void Func_020046a0();
void Func_020046ba();
s32 Func_020046c8();
void Func_020046d8();
void Func_02004700();
void Func_02004702();
s32 Func_02004712();
void Func_02004732();
void Func_02004788();
void Func_020047b4();
void Func_02004816();
void Func_0200485a();
void Func_020048c2();
void Func_020048f0();
void Func_02004924();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void Scene_RunScene39bSequenceA(void)
{
    u32 i;
    s32 p10;
    s32 p8;
    s32 p8b;
    u8 *p9;
    s32 p9b;
    s32 rec4;
    u8 *rec7;
    s32 record;
    s32 r9;
    s32 v2;
    s32 v3;
    s32 v5;
    s32 base5_0;
    s32 base6_3001c94;
    s32 v6;
    s32 slot12;
    u8 *slot0;
    u8 slot4[12];
    s32 slot8;
    u8 *p7;
    u8 *p6;
    u8 *p4;
    u8 *p5;
    u8 *p6b;
    u8 slot16[12];

    (*(s32 *)((*(s32 *)slot4) + 8)) = *(s32 *)0x03001ebc;
    p7 = *(s32 *)((*(s32 *)((*(s32 *)slot4) + 8)) + ((*(s32 *)0x02000434 << 2) + 20));
    p8 = *(s32 *)(*(s32 *)0x03001edc);
    slot0 = ((s32)p7 + 85);
    (*(s32 *)slot4) = p7[85];
    p6 = *(u16 *)(0x0200a7f4 + ((((u32)*(s32 *)0x03001ae8 >> 4) & 15) << 1));
    if (*(s16 *)(0x0200a7f4 + ((((u32)*(s32 *)0x03001ae8 >> 4) & 15) << 1)) == -1) {
        v3 = *(s16 *)(0x0200a7f4 + ((((u32)*(s32 *)0x03001ae8 >> 4) & 15) << 1));
        v5 = r9;
        v6 = (s32)p6;
    } else {
        p4 = *(s32 *)((s32)p7 + 8);
        v3 = ((s32)((s32)p4 & -0x100000) + 0x80000);
        *(s32 *)0x00000010 = ((s32)((s32)p4 & -0x100000) + 0x80000);
        *(s32 *)0x00000014 = *(s32 *)((s32)p7 + 20);
        v2 = ((s32)(*(s32 *)((s32)p7 + 16) & -0x100000) + 0x80000);
        *(s32 *)0x00000018 = ((s32)(*(s32 *)((s32)p7 + 16) & -0x100000) + 0x80000);
        if (((s32)(*(s32 *)((s32)p7 + 16) & -0x100000) + 0x80000) < 0) {
            v2 = ((s32)(*(s32 *)((s32)p7 + 16) & -0x100000) + 0x17ffff);
        }
        if (((s32)((s32)p4 & -0x100000) + 0x80000) < 0) {
            v3 = ((s32)((s32)p4 & -0x100000) + 0x17ffff);
        }
        Call3(Func_020044fa, 0x200000, (s32)p6, 16);
        (*(s32 *)((*(s32 *)slot4) + 4)) = (((((*(s32 *)0x00000018 / 0x100000) << 7) + (*(s32 *)0x00000010 / 0x100000)) << 2) + 0x2010000);
        if (*(u8 *)((((((v2 >> 20) << 7) + (v3 >> 20)) << 2) + 0x2010000) + 2) != *(s32 *)(p8 + 4)) {
            if ((*(s32 *)((*(s32 *)slot4) + 4))[2] != *(s32 *)(p8 + 4)) {
                goto L_0200211a;
            }
            v3 = *(s32 *)p8;
            if (*(s32 *)p8 != 0) {
                goto L_0200211a;
            }
            v5 = (((((v2 >> 20) << 7) + (v3 >> 20)) << 2) + 0x2010000);
            v6 = (s32)p6;
        } else {
            L_0200211a:;
            Func_020045f8();
            rec4 = Value2(Func_020045a8, (s32)p7, slot16);
            if (rec4 != 0) {
                v5 = (((((v2 >> 20) << 7) + (v3 >> 20)) << 2) + 0x2010000);
                v6 = (s32)p6;
            } else {
                p5 = *(s32 *)(p8 + 24);
                if ((s32)p5 != 0) {
                    *(u16 *)((s32)p5 + 100) = rec4;
                    Value2(Func_02004588, (s32)p5, 0x200a7dc);
                    ((void (*)())Func_02004588_a)((s32)p5, 7);
                    *(s32 *)(p8 + 24) = rec4;
                }
                if ((*(s32 *)((*(s32 *)slot4) + 4))[2] == *(s32 *)(p8 + 4)) {
                    if (*(s32 *)p8 != 0) {
                        p6b = *(s32 *)(p8 + 20);
                        rec7 = Value4(Func_020045be, 26, *(s32 *)((s32)p6b + 8), *(s32 *)((s32)p6b + 12), *(s32 *)((s32)p6b + 16));
                        if ((s32)rec7 != 0) {
                            p9 = *(s32 *)((s32)rec7 + 80);
                            *(s32 *)((s32)rec7 + 20) = *(s32 *)((s32)p6b + 20);
                            Call2(Func_020045ca, (s32)rec7, 0x200a7d0);
                            rec7[85] = rec4;
                            *(u16 *)(((s32)rec7 + 85) + 15) = rec4;
                            rec7[35] = 2;
                            *(s32 *)((s32)rec7 + 48) = 0x40000;
                            *(s32 *)((s32)rec7 + 52) = 0x20000;
                            Func_0200461c((s32)rec7, *(s32 *)0x00000010, *(s32 *)0x00000014, *(s32 *)0x00000018);
                            if ((s32)p9 != 0) {
                                Func_020045fa((s32)p9, 6);
                                p9[38] = (s32)Data_00000000;
                            }
                            *(s32 *)(p8 + 24) = (s32)rec7;
                        }
                        v5 = (*(s32 *)p8 - 1);
                        *(s32 *)p8 = (*(s32 *)p8 - 1);
                        if (v5 == 0) {
                            Func_02004638(*(s32 *)(p8 + 20));
                            *(s32 *)(p8 + 20) = v5;
                            Call1(Func_020046ba, 0x161);
                        } else {
                            if (*(s32 *)(p8 + 20) != 0) {
                                Func_0200466e(*(s32 *)(p8 + 20), (6 - v5));
                            }
                        }
                    }
                }
                Func_02004676((s32)p7, 6);
                Func_0200462c(3);
                Func_0200485a(152);
                Func_0200468a((s32)p7, 7);
                *(s32 *)((s32)p7 + 48) = 0x30000;
                *(s32 *)((s32)p7 + 52) = 0x20000;
                *(s32 *)((s32)p7 + 40) = 0x40000;
                slot0[0] &= 126;
                Func_02004700((s32)p7, 0);
                Func_02004788(0, *(s16 *)(16 + 2), *(s16 *)(16 + 10));
                Value2(Func_020046c8, (s32)p7, 6);
                Func_0200467e(2);
                if ((*(s32 *)((*(s32 *)slot4) + 4))[2] != *(s32 *)(p8 + 4)) {
                    Func_02004732((s32)p7, 1);
                } else {
                    Func_020048c2(215);
                }
                Func_020046a0(1);
                slot0[0] = (*(s32 *)slot4);
                if ((*(s32 *)((*(s32 *)slot4) + 4))[2] == *(s32 *)(p8 + 4)) {
                    if (*(s32 *)(p8 + 24) == 0) {
                        Value2(Func_02004712, (s32)p7, 18);
                        Func_020048f0(241);
                        base5_0 = 0;
                        base6_3001c94 = 0x3001c94;
                        for (;;) {
                            if ((base5_0 & 15) == 0) {
                                Func_02003ec6((s32)p7);
                            }
                            if (!(base5_0 <= 31)) break;
                            v5 = base5_0;
                            L_020022e8:;
                            Func_020046d8(1);
                            v5 = (v5 + 1);
                        }
                        if (*(s32 *)base6_3001c94 == 0) {
                            goto L_020022e8;
                        }
                        Call1(Func_02004924, 0x120);
                        Func_02004702(1);
                        *(s32 *)((s32)p7 + 8) = *(s32 *)(p8 + 12);
                        *(s32 *)((s32)p7 + 16) = *(s32 *)(p8 + 16);
                        Func_020047b4((s32)p7, 1);
                    }
                }
                *(s32 *)(p8 + 8) = 0;
                Func_02004816();
                v6 = base6_3001c94;
            v3 = ((*(s32 *)((*(s32 *)slot4) + 8)) + 0x1b0);
            }
        }
    }
    p8b = v3;
    p9b = v5;
    p10 = v6;
    /* unlifted: 0x020020e0..0x020020e4 (2), 0x020020f0..0x020020f4 (2) */
}
