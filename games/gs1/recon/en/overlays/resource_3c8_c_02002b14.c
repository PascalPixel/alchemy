#include "types.h"

#define FieldScene_RunScene3c8SequenceA Func_02002b14

u8 *Func_02002cfa();
void Func_02003632();
s32 Func_02003a58();
s32 Func_02003a94();
void Func_0200797a();
void Func_0200798e();
void Func_020079be();
void Func_020079c6();
s32 Func_020079d8();
void Func_020079da();
u8 *Func_02007a10();
u8 *Func_02007ab8();
void Func_02007af2();
void Func_02007b14();
u8 *Func_02007b20();
u8 * Func_02007b54();
u8 * Func_02007b54_a();
void Func_02007b72();
void Func_02007bd0();
s32 Func_02007bda();
u8 *Func_02007bec();
void Func_02007c00();
s32 Func_02007c02();
void Func_02007c08();
s32 Func_02007c1c();
void Func_02007c20();
void Func_02007c2e();
void Func_02007c32();
s32 Func_02007c4a();
s32 Func_02007c52();
s32 Func_02007c5a();
u8 *Func_02007c62();
void Func_02007c68();
void Func_02007c7a();
void Func_02007c8c();
void Func_02007c90();
void Func_02007cae();
void Func_02007cce();
void Func_02007ce8();
void Func_02007ce8_a();
void Func_02007cfe();
void Func_02007d42();
void Func_02007d60();
void Func_02007d7c();
void Func_02007d80();
void Func_02007d88();
void Func_02007da2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunScene3c8SequenceA(void)
{
    u32 i;
    s32 p10;
    u8 *p11;
    s32 p11b;
    u8 *p8;
    s32 p9;
    s32 p9b;
    u8 *rec2;
    u8 *rec7;
    s32 rec8;
    u8 *record;
    s32 r10;
    s32 r8;
    s32 base10_f;
    s32 base4_200d164;
    u8 *v6;
    s32 v4;
    s32 v1;
    s32 v5;
    s32 v0;
    s32 v7;
    s32 slot12;
    s32 slot8;

    base6_0 = r10;
    slot12 = 0;
    record = Value1(Func_020079d8, 0);
    slot8 = (s32)record;
    Func_020079c6();
    Call6(Func_0200797a, 69, 48, 4, 2, 5, 48);
    Call6(Func_0200798e, 73, 37, 9, 13, 9, 37);
    base10_f = 15;
    L_02002b60:;
    rec2 = Func_02007a10(base10_f);
    p11 = (s32)rec2 + 35;
    if (*p11 != 2) {
        Call6(Func_020079be, 72, 48, 1, 1, (*(s32 *)((s32)rec2 + 8) >> 20), (*(s32 *)((s32)rec2 + 16) >> 20));
    } else {
        Call6(Func_020079da, 73, 48, 1, 1, (*(s32 *)((s32)rec2 + 8) >> 20), (*(s32 *)((s32)rec2 + 16) >> 20));
    }
        v4 = 0x200d164;
        base4_200d164 = 0x200d164;
    base6_0 = 0;
    if ((*(s32 *)((s32)rec2 + 8) >> 20) == *(s32 *)(0x0200d164 + base6_0)) {
        if ((*(s32 *)((s32)rec2 + 16) >> 20) != *(s32 *)(base4_200d164 + 4)) {
            v4 = base4_200d164;
            v6 = base6_0;
            goto L_02002bd0;
        }
        v5 = 0;
        v6 = base6_0;
        if (*(s32 *)((s32)rec2 + 12) < 0) {
            v4 = base4_200d164;
            v6 = base6_0;
            goto L_02002bd0;
        }
    } else {
        v6 = base6_0;
        L_02002bd0:;
        do {
            if ((u32)((s32)v6 + 1) > 7) {
                v5 = 8;
                v6 = ((s32)v6 + 1);
                goto L_02002bf8;
            }
            v1 = (((s32)v6 + 1) << 3);
            v6 = ((s32)v6 + 1);
        } while ((*(s32 *)((s32)rec2 + 8) >> 20) != *(s32 *)(v4 + (((s32)v6 + 1) << 3)));
        if ((*(s32 *)((s32)rec2 + 16) >> 20) != *(s32 *)(v4 + (v1 + 4))) {
            goto L_02002bd0;
        }
        if (*(s32 *)((s32)rec2 + 12) < 0) {
            goto L_02002bd0;
        }
        v5 = (s32)v6;
    }
    L_02002bf8:;
    if (v5 == 8) {
        v7 = r8;
    } else {
        v6 = 15;
        for (;;) {
            if ((u32)(s32)v6 > 18) {
                goto L_02002c30;
            }
            record = Func_02007ab8((s32)v6);
            if (!(base10_f == (s32)v6)) break;
            L_02002c02:;
            v0 = (s32)record;
            v6 = ((s32)v6 + 1);
        }
        if ((*(s32 *)((s32)rec2 + 8) >> 20) != (*(s32 *)(v0 + 8) >> 20)) {
            goto L_02002c02;
        }
        if ((*(s32 *)((s32)rec2 + 16) >> 20) != (*(s32 *)(v0 + 16) >> 20)) {
            goto L_02002c02;
        }
        v5 = 8;
        L_02002c30:;
        if (v5 == 8) {
            v7 = r8;
        } else {
            p9 = ((u32)(*(u8 *)(*(s32 *)(slot8 + 80) + 9) << 28) >> 30);
            if ((u32)(*(s32 *)(slot8 + 16) >> 20) <= *(s32 *)(0x0200d164 + ((v5 << 3) + 4))) {
                record = Func_02002cfa(*(s32 *)((s32)rec2 + 8), *(s32 *)((s32)rec2 + 12), (*(s32 *)((s32)rec2 + 16) + -0x40000), 20);
                slot12 = (s32)record;
                Func_02007bd0(0, 3);
            }
            v6 = 15;
            for (i = 0; i < 19; i++) {
                record = Func_02007b20(i);
                if (base10_f != i) {
                    if ((*(s32 *)((s32)rec2 + 8) >> 20) == (*(s32 *)((s32)record + 8) >> 20)) {
                        if (((*(s32 *)((s32)rec2 + 16) >> 20) - 1) == (*(s32 *)((s32)record + 16) >> 20)) {
                            Func_02007c00(i, 3);
                        }
                    }
                }
                v6 = i;
            }
            record = Func_02007b54(base10_f);
            Func_02007af2((s32)record, 0);
            v6 = (s32)rec2;
            rec2[34] = 0;
            v6 = ((s32)v6 + 85);
            v6[0] = 3;
            *(s32 *)((s32)rec2 + 72) = 0x1999;
            *(s32 *)((s32)rec2 + 68) = 0;
            Call6(Func_02007b14, 6, 44, 1, 1, *(s32 *)(0x0200d164 + (v5 << 3)), *(s32 *)(0x0200d164 + ((v5 << 3) + 4)));
            Func_02003632((s32)rec2);
            Func_02007ce8(188);
            rec2[89] = 0;
            v6[0] = 0;
            *(s32 *)((s32)rec2 + 12) = -0x100000;
            Func_02007c68(base10_f, 3);
            *p11 = 2;
            Call6((void (*)())Func_02007b54_a, 73, 48, 1, 1, *(s32 *)(0x0200d164 + (v5 << 3)), *(s32 *)(0x0200d164 + ((v5 << 3) + 4)));
            Func_02007c8c(0, p9);
            v6 = 15;
            *(u8 *)(Func_02007bda(0) + 35) |= 1;
            for (i = 0; i < 19; i++) {
                record = Func_02007bec(i);
                if (base10_f != i) {
                    if ((*(s32 *)((s32)rec2 + 8) >> 20) == (*(s32 *)((s32)record + 8) >> 20)) {
                        if (((*(s32 *)((s32)rec2 + 16) >> 20) - 1) == (*(s32 *)((s32)record + 16) >> 20)) {
                            Func_02007cce(i, 1);
                            *(u8 *)(Func_02007c1c(i) + 35) |= 1;
                        }
                    }
                }
                v6 = i;
            }
            Func_02007b72(slot12);
            if (Value1(Func_02007c02, 0x308) != 0) {
                Func_02007c32();
                v7 = (v5 << 3);
                goto L_02002ea8;
            }
            rec2 = Value1(Func_02007c4a, 15);
            rec7 = Value1(Func_02007c52, 16);
            rec8 = Value1(Func_02007c5a, 17);
            v6 = rec8;
            record = Func_02007c62(18);
            v6 = ((s32)v6 + 35);
            if (((((rec7[35] & rec2[35]) & v6[0]) & record[35]) & 2) != 0) {
                Call2(Func_02007d60, 0x10000, 0x2000);
                Func_02007d80(14, 1);
                Func_02007d7c();
                rec8 = Value3(Func_02003a58, 136, 0x308, 0x200d77c);
                v6 = rec8;
                Func_02007c90(30);
                Call2(Func_02007d88, 0x6666, 0xccc);
                Call4(Func_02007da2, 0xd80000, -1, 0x2780000, 1);
                Func_02007c20((s32)v6);
                Call2(Func_02007c08, (s32)v6, 0x200d7c8);
                rec7 = Value3(Func_02003a94, 216, 0x2f8, 0x200dac8);
                p8 = (s32)v6 + 99;
                for (;;) {
                    if (!(*(s32 *)((s32)v6) != 0)) break;
                    L_02002e46:;
                    if (*p8 == 0) {
                        if (rec7[99] == 0) {
                            goto L_02002e86;
                        }
                    }
                    Func_02007ce8_a(30);
                    Call3(Func_02007c7a, 0x200dd3c, 77, 35);
                    Call6(Func_02007cae, 13, 35, 1, 1, 13, 36);
                    Call1(Func_02007cfe, 0x308);
                    v7 = (v5 << 3);
                    goto L_02002e98;
                    L_02002e86:;
                    Func_02007c2e(1);
                }
                if (*(s32 *)((s32)rec7) != 0) {
                    goto L_02002e46;
                }
            }
            v7 = (v5 << 3);
        }
    }
    L_02002e98:;
    base10_f = (base10_f + 1);
    if ((u32)base10_f <= 18) {
        goto L_02002b60;
    }
    Func_02007d42();
    L_02002ea8:;
    p9b = ((v5 << 3) + 4);
    p10 = (s32)v6;
    base10_f = p10;
    p11b = v7;
}
