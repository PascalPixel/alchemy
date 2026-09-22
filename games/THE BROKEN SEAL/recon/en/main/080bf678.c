#include "types.h"

#define Func_080bf678 Func_080bf678

extern u8 Data_00000820[];
void Func_08009080();
s32 Func_08009088();
void Func_08015120();
void Func_080151c8();
s32 Func_08077000();
u8 *Func_08077008();
void Func_08077010();
s32 Func_08077118();
s32 Func_08077120();
void Func_080771b0();
void Func_080771c0();
s32 Func_080b6c08();
s32 Func_080b78e4();
void Func_080b7aac();
s32 Func_080b7dd0();
void Func_080bb65c();
s32 Func_080bbabc();
void Func_080bd808();
void Func_080bdfec();
void Func_080be02c();
s32 Func_080bf250();
s32 Func_080bf2b4();
s32 Func_080bf318();
s32 Func_080bf37c();
s32 Func_080bf3bc();
s32 Func_080bf400();
s32 Func_080bf440();
s32 Func_080bf484();
s32 Func_080bf4c4();
s32 Func_080bf524();
s32 Func_080bf54c();
s32 Func_080bf574();
void Func_080c0774();
s32 Func_080c1798();
void Func_080f9010();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void Func_080bf678(void)
{
    u32 i;
    s32 p10;
    s32 p11;
    u8 *rec;
    s32 rec4;
    s32 record;
    s32 r13;
    s32 none;
    s32 v9;
    u8 *v5;
    s32 v8;
    u8 *v6;
    u8 *slot8;
    u8 *slot4;
    s32 slot0;
    u8 *p7;
    u8 *p5;
    u8 *p4;
    u8 *p5b;
    u8 *p6;

    slot8 = *(s32 *)0x03001e74;
    slot4 = ((s32)slot8 + 68);
    p11 = ((u32)(-slot4[0] | slot4[0]) >> 31);
    none = 0;
    if (none >= (p11 + 1)) {
    } else {
        v9 = none;
        L_080bf6aa:;
        record = Func_08077000(v9);
        p10 = record + 8;
        if (none < *(s32 *)((record + 0x108))) {
            v5 = p10;
            none = 0;
            v8 = none;
            do {
                if (*(s8 *)((s32)v5 + 3) > 0) {
                    if (Value1(Func_080b7dd0, v5[2]) != 0) {
                        record = Func_08077008(v5[2]);
                        if (*(s16 *)(record + 56) != 0) {
                            v5[3] = (v5[3] - 1);
                        }
                    }
                }
                v8 = (v8 + 1);
                v5 = ((s32)v5 + 4);
            } while (v8 < *(s32 *)((0x100 + p10)));
        }
        if (none < *(s32 *)((0x100 + p10))) {
            v6 = p10;
            none = 0;
            v8 = none;
            do {
                if (*(s8 *)((s32)v6 + 3) == 0) {
                    p7 = v6[2];
                    record = Value1(Func_080b7dd0, (s32)p7);
                    if (record == 0) {
                        goto L_080bf7c6;
                    }
                    Func_080bdfec();
                    Func_080bd808(30);
                    Value2(Func_080bbabc, 0, (s32)p7);
                    Value2(Func_080bbabc, 3, (((((v6[0] << 2) + v6[0]) << 2) + v6[1]) + 0x12c));
                    Value2(Func_080bbabc, 14, 175);
                    Value2(Func_080bbabc, 10, 0);
                    Value2(Func_080bbabc, 4, 0x897);
                    ((void (*)())Func_080bbabc)(11, (s32)p7);
                    Func_080f9010(212);
                    record = Func_080b7dd0((s32)p7);
                    Func_08009080(*(s32 *)(record), 3);
                    record = Func_080b7dd0((s32)p7);
                    Value2(Func_08009088, *(s32 *)(record), 32);
                    p5 = v6[0];
                    Func_080771b0((s32)p7, (s32)p5, v6[1]);
                    Func_080771c0((s32)p7, v6[0], v6[1]);
                    Func_08077010((s32)p7);
                    Value4(Func_080c1798, (s32)p7, (s32)p5, 3, 0);
                    Func_080be02c();
                } else {
                    v6 = ((s32)v6 + 4);
                    v8 = (v8 + 1);
                }
                L_080bf7c6:;
            } while (v8 < *(s32 *)((0x100 + p10)));
        }
        v9 = (v9 + 1);
        if (v9 < (p11 + 1)) {
            goto L_080bf6aa;
        }
    }
    Func_080c0774(2, *(u16 *)((*(s32 *)0x03001e74 + 0x648)), 0);
    p4 = *(s32 *)0x080c35c0;
    if (slot4[0] != 0) {
        if (slot8[80] == 0) {
            goto L_080bf824;
        }
        slot0 = (r13 + 12);
        *(s32 *)(slot0 + 4) = 1;
    } else {
        slot0 = (r13 + 12);
        goto L_080bf82a;
        L_080bf824:;
        slot0 = (r13 + 12);
    }
    L_080bf82a:;
    none = 0;
    v9 = none;
    L_080bf834:;
    rec4 = Func_080b6c08(*(s32 *)((v9 << 2) + slot0), 20, 2, (s32)p4);
    none = 0;
    if (none >= rec4) {
    } else {
        v8 = none;
        L_080bf84e:;
        p5b = *(u16 *)(0x00000014 + (v8 << 1));
        rec = Func_08077008((s32)p5b);
        if (*(u8 *)(((s32)rec + 0x144)) != 0) {
            *(u8 *)(((s32)rec + 0x144)) += 255;
        }
        if (*(s16 *)((s32)rec + 56) != 0) {
            if (rec[68] != 0) {
                if (*(s16 *)((s32)rec + 56) != *(s16 *)((s32)rec + 52)) {
                    p6 = rec[68];
                    v6 = (s32)p6;
                    if ((*(s16 *)((s32)rec + 56) + (s32)p6) > *(s16 *)((s32)rec + 52)) {
                        v6 = (*(s16 *)((s32)rec + 52) - *(s16 *)((s32)rec + 56));
                    }
                    Value2(Func_08077118, (s32)p5b, (s32)v6);
                    Func_08015120((s32)p5b, 1);
                    Func_08015120((s32)v6, 5);
                    if (*(s16 *)((s32)rec + 56) == *(s16 *)((s32)rec + 52)) {
                        Func_080151c8((s32)Data_00000820);
                    } else {
                        Call1(Func_080151c8, 0x81d);
                    }
                    Func_080f9010(175);
                    Func_080bb65c();
                }
            }
            if (rec[69] != 0) {
                if (*(s16 *)((s32)rec + 58) != *(s16 *)((s32)rec + 54)) {
                    p6 = rec[69];
                    v6 = (s32)p6;
                    if ((*(s16 *)((s32)rec + 58) + (s32)p6) > *(s16 *)((s32)rec + 54)) {
                        v6 = (*(s16 *)((s32)rec + 54) - *(s16 *)((s32)rec + 58));
                    }
                    Value2(Func_08077120, (s32)p5b, (s32)v6);
                    Func_08015120((s32)p5b, 1);
                    Func_08015120((s32)v6, 5);
                    if (*(s16 *)((s32)rec + 58) == *(s16 *)((s32)rec + 54)) {
                        Call1(Func_080151c8, 0x821);
                    } else {
                        Call1(Func_080151c8, 0x81e);
                    }
                    Func_080f9010(175);
                    Func_080bb65c();
                }
            }
        }
        if (Value1(Func_080bf574, (s32)p5b) != 0) {
            record = Value1(Func_080b7dd0, (s32)p5b);
            Value2(Func_080b78e4, (s32)p5b, record);
            Func_08015120((s32)p5b, 1);
            Call1(Func_080151c8, 0x889);
            Func_080bb65c();
        }
        if (Value1(Func_080bf250, (s32)p5b) != 0) {
            record = Value1(Func_080b7dd0, (s32)p5b);
            Value2(Func_080b78e4, (s32)p5b, record);
            Func_08015120((s32)p5b, 1);
            Call1(Func_080151c8, 0x887);
            Func_080bb65c();
        }
        if (Value1(Func_080bf2b4, (s32)p5b) != 0) {
            record = Value1(Func_080b7dd0, (s32)p5b);
            Value2(Func_080b78e4, (s32)p5b, record);
            Func_08015120((s32)p5b, 1);
            Call1(Func_080151c8, 0x888);
            Func_080bb65c();
        }
        if (Value1(Func_080bf318, (s32)p5b) != 0) {
            record = Value1(Func_080b7dd0, (s32)p5b);
            Value2(Func_080b78e4, (s32)p5b, record);
            Func_08015120((s32)p5b, 1);
            Call1(Func_080151c8, 0x886);
            Func_080bb65c();
        }
        if (Value1(Func_080bf37c, (s32)p5b) != 0) {
            record = Value1(Func_080b7dd0, (s32)p5b);
            Value2(Func_080b78e4, (s32)p5b, record);
            Func_08015120((s32)p5b, 1);
            Call1(Func_080151c8, 0x88b);
            Func_080bb65c();
        }
        if (Value1(Func_080bf3bc, (s32)p5b) != 0) {
            record = Value1(Func_080b7dd0, (s32)p5b);
            Value2(Func_080b78e4, (s32)p5b, record);
            Func_08015120((s32)p5b, 1);
            Call1(Func_080151c8, 0x88a);
            Func_080bb65c();
        }
        if (Value1(Func_080bf400, (s32)p5b) != 0) {
            record = Value1(Func_080b7dd0, (s32)p5b);
            Value2(Func_080b78e4, (s32)p5b, record);
            Func_08015120((s32)p5b, 1);
            Call1(Func_080151c8, 0x88e);
            Func_080bb65c();
        }
        if (Value1(Func_080bf440, (s32)p5b) != 0) {
            record = Value1(Func_080b7dd0, (s32)p5b);
            Value2(Func_080b78e4, (s32)p5b, record);
            Func_08015120((s32)p5b, 1);
            Func_080b7aac((s32)p5b);
            Call1(Func_080151c8, 0x88d);
            Func_080bb65c();
        }
        if (Value1(Func_080bf484, (s32)p5b) != 0) {
            record = Value1(Func_080b7dd0, (s32)p5b);
            Value2(Func_080b78e4, (s32)p5b, record);
            Func_08015120((s32)p5b, 1);
            Func_080b7aac((s32)p5b);
            Call1(Func_080151c8, 0x883);
            Func_080bb65c();
        }
        if (Value1(Func_080bf4c4, (s32)p5b) != 0) {
            record = Value1(Func_080b7dd0, (s32)p5b);
            Value2(Func_080b78e4, (s32)p5b, record);
            Func_08015120((s32)p5b, 1);
            Call1(Func_080151c8, 0x88c);
            Func_080bb65c();
        }
        if (Value1(Func_080bf524, (s32)p5b) != 0) {
            record = Value1(Func_080b7dd0, (s32)p5b);
            Value2(Func_080b78e4, (s32)p5b, record);
            Func_08015120((s32)p5b, 1);
            Call1(Func_080151c8, 0x891);
            Func_080bb65c();
        }
        if (Value1(Func_080bf54c, (s32)p5b) != 0) {
            record = Value1(Func_080b7dd0, (s32)p5b);
            Value2(Func_080b78e4, (s32)p5b, record);
            Func_08015120((s32)p5b, 1);
            Call1(Func_080151c8, 0x892);
            Func_080bb65c();
        }
        v8 = (v8 + 1);
        if (v8 < rec4) {
            goto L_080bf84e;
        }
    }
    v9 = (v9 + 1);
    if (v9 <= 1) {
        goto L_080bf834;
    }
}
