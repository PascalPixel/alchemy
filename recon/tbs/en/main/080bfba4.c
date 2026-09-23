#include "types.h"

#define Func_080bfba4 Func_080bfba4

s32 Func_080022ec();
void Func_08002df0();
s32 Func_08004970();
void Func_080072f0();
void Func_08009080();
s32 Func_08009088();
u8 *Func_08077000();
s32 Func_08077008();
s32 Func_08077010();
s32 Func_08077118();
void Func_080771b8();
s32 Func_080b6b40();
s32 Func_080b7dd0();
void Func_080bb938();
s32 Func_080bbabc();
void Func_080bd808();
void Func_080bdfec();
void Func_080be02c();
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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

s32 Func_080bfba4(s32 a0)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p8;
    s32 rec;
    s32 rec3;
    u8 *rec8;
    s32 record;
    s32 r13;
    u8 *v5;
    s32 base7_0;
    s32 v3;
    s32 v4;
    u8 *v2;
    s32 v12;
    s32 base5_ffffffff;
    u8 *v1;
    s32 none;
    s32 base6_0;
    s32 v10;
    s32 v7;
    s32 base5_828;
    u8 *slot12;
    s32 slot4;
    s32 slot8;
    s32 slot0;
    u8 *p4;
    u8 *p5;
    u8 slot28[4];

    slot12 = a0;
    p8 = slot12[0];
    slot4 = 0;
    record = Func_08077008();
    slot8 = record;
    if ((u32)p8 > 7) {
    }
    rec8 = Value1(Func_08077000, 1);
        base7_0 = 0;
    if (slot4 < *(s32 *)(((s32)rec8 + 0x108))) {
        v5 = ((s32)rec8 + 8);
        do {
            if (v5[2] == p8) {
                if (*(s8 *)((s32)v5 + 3) == -1) {
                    Func_080771b8(p8, v5[0], v5[1]);
                }
            }
            base7_0 = (base7_0 + 1);
            v5 = ((s32)v5 + 4);
        } while (base7_0 < *(s32 *)((((s32)rec8 + 8) + 0x100)));
    }
    if (Value2(Func_080b6b40, 1, 0) != 0) {
        if (Value2(Func_080b6b40, 2, 0) != 0) {
            slot4 = 1;
        }
    }
    if ((u32)p8 > 7) {
    }
    rec8 = Func_08077000(1);
    slot0 = (r13 + 16);
    v3 = slot28;
    do {
        *(s32 *)(v3) = 0;
        v3 = (v3 - 4);
    } while (v3 >= slot0);
    p11 = (((s32)rec8 + 8) + 0x100);
    L_080bfc6a:;
    do {
        v12 = -1;
        if (0 < *(s32 *)p11) {
            if (*(s8 *)((s32)rec8 + 11) == -2) {
                v12 = rec8[10];
            } else {
                v4 = 0;
                do {
                    v4 = (v4 + 1);
                    if (v4 >= *(s32 *)p11) {
                        v12 = -1;
                        goto L_080bfc9e;
                    }
                    v2 = (((s32)rec8 + 8) + (v4 << 2));
                } while (*(s8 *)((((s32)rec8 + 8) + (v4 << 2)) + 3) != -2);
                v12 = v2[2];
            }
        }
        L_080bfc9e:;
        if (v12 == -1) {
            goto L_080bfd18;
        }
        v5 = -1;
            base5_ffffffff = -1;
        if (*(s32 *)p11 > 0) {
            p4 = *(s32 *)p11;
            v2 = ((s32)rec8 + 8);
            v4 = (s32)p4;
            do {
                if (v2[2] == v12) {
                    if (*(s8 *)((s32)v2 + 3) > base5_ffffffff) {
                        base5_ffffffff = *(s8 *)((s32)v2 + 3);
                    }
                }
                v4 = (v4 - 1);
                v2 = ((s32)v2 + 4);
            } while (v4 != 0);
            v5 = base5_ffffffff;
        }
        v5 = ((s32)v5 + 1);
        if (((s32)v5 + 1) <= 1) {
            v5 = 2;
        }
        v4 = 0;
    } while (v4 >= *(s32 *)p11);
    v1 = ((s32)rec8 + 8);
    do {
        if (v1[2] == v12) {
            if (*(s8 *)((s32)v1 + 3) == -2) {
                v1[3] = (s32)v5;
                *(s32 *)(slot0 + (v1[0] << 2)) = (*(s32 *)(slot0 + (v1[0] << 2)) + 1);
                v5 = ((s32)v5 + 1);
            }
        }
        v4 = (v4 + 1);
        v1 = ((s32)v1 + 4);
    } while (v4 < *(s32 *)((0x100 + ((s32)rec8 + 8))));
    goto L_080bfc6a;
    L_080bfd18:;
    if (slot4 == 0) {
    } else {
        none = 0;
        rec3 = Func_08004970(0x14c, -2);
        Call4(Func_080072f0, rec3, slot8, 0x14c, 0x3001388);
        base6_0 = 0;
        v10 = none;
        do {
            v3 = *(s32 *)(slot0);
            v7 = -1;
            if (v3 > v10) {
                p10 = v3;
                v10 = p10;
                v7 = base6_0;
            }
            base6_0 = (base6_0 + 1);
        } while (base6_0 <= 3);
        if (v7 >= 0) {
            if (*(s8 *)(slot8 + (v7 + 0x12c)) < v10) {
                *(u8 *)(slot8 + (v7 + 0x12c)) = v10;
            }
        }
        v7 = 72;
        Call1((void (*)())Func_08077010, p8);
        base6_0 = 0;
        do {
            if ((*(s16 *)(v7 + slot8) - *(s16 *)(v7 + rec3)) > 0) {
                Func_080bdfec();
                Func_080bd808(25);
                Value2(Func_080bbabc, 0, p8);
                Value2(Func_080bbabc, 1, (*(s16 *)(v7 + slot8) - *(s16 *)(v7 + rec3)));
                Value2(Func_080bbabc, 14, 175);
                ((void (*)())Func_080bbabc)(4, (base6_0 + 0x879));
                ((void (*)())Func_080bbabc)(11, p8);
                Func_080f9010(212);
                record = Func_080b7dd0(p8);
                Func_08009080(*(s32 *)(record), 3);
                record = Func_080b7dd0(p8);
                Value2(Func_08009088, *(s32 *)(record), 32);
                Value4(Func_080c1798, p8, base6_0, 2, (v10 - 1));
                Func_080be02c();
            }
            base6_0 = (base6_0 + 1);
            v7 = (v7 + 4);
        } while (base6_0 <= 3);
        Func_08002df0(rec3);
        if (slot4 == 0) {
        } else {
            Func_080bdfec();
            if (*(s32 *)((s32)slot12 + 96) != 0) {
                Value2(Func_080bbabc, 8, p8);
                Value2(Func_080bbabc, 0, p8);
                ((void (*)())Func_080bbabc)(1, *(s32 *)((s32)slot12 + 96));
                Value2(Func_080bbabc, 4, 0x84b);
                if (Value2(Func_08077118, p8, -*(s32 *)((s32)slot12 + 96)) == 0) {
                    Value2(Func_080bbabc, 9, p8);
                    ((void (*)())Func_080bbabc)(0, p8);
                    if ((u32)p8 <= 7) {
                    } else {
                    }
                    Call2(Func_080bbabc, 4, 0x825);
                } else {
                    ((void (*)())Func_080bbabc)(11, p8);
                }
            }
            Func_080bb938();
            Func_080bdfec();
            if (*(s8 *)((slot8 + 0x131)) != 0) {
                rec = Value2(Func_080022ec, (*(s8 *)((slot8 + 0x131)) * *(s16 *)(slot8 + 52)), 10);
                p5 = *(s32 *)0x03001e74;
                ((void (*)())Func_080bbabc)(8, p8);
                Value2(Func_080bbabc, 0, p8);
                Value2(Func_080bbabc, 1, rec);
                ((void (*)())Func_080bbabc)(4, 0x851);
                if (*(s8 *)((slot8 + 0x131)) != 0) {
                    v3 = 134;
                } else {
                    v3 = 133;
                }
                *(s32 *)(((s32)p5 + 0x820)) = v3;
                if (Value2(Func_08077118, p8, -rec) == 0) {
                    Value2(Func_080bbabc, 9, p8);
                    ((void (*)())Func_080bbabc)(0, p8);
                    if ((u32)p8 <= 7) {
                    } else {
                    }
                    Call2(Func_080bbabc, 4, 0x825);
                } else {
                    ((void (*)())Func_080bbabc)(11, p8);
                }
            }
            Func_080bb938();
            Func_080bdfec();
            if (*(u8 *)((slot8 + 0x141)) != 0) {
                v3 = (*(u8 *)((slot8 + 0x141)) + 255);
                *(u8 *)((slot8 + 0x141)) += 255;
                if ((v3 << 24) == 0) {
                    if (Func_08077118(p8, -0x40000000) == 0) {
                        Value2(Func_080bbabc, 0, p8);
                        base5_828 = 0x828;
                        ((void (*)())Func_080bbabc)(4, base5_828);
                        Value2(Func_080bbabc, 8, p8);
                        Value2(Func_080bbabc, 9, p8);
                        ((void (*)())Func_080bbabc)(0, p8);
                        if ((u32)p8 <= 7) {
                            Call2(Func_080bbabc, 4, 0x825);
                        } else {
                            ((void (*)())Func_080bbabc)(4, (base5_828 + 3));
                        }
                    }
                }
            }
            Func_080bb938();
        }
    }
    record = Func_08077010(p8);
    return record;
}
