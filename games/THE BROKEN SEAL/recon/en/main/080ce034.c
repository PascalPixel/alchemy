#include "types.h"

#define Function Func_080ce034

extern u8 Data_00000073[];
extern u8 Data_0000007d[];
extern u8 Data_00000089[];
s32 Func_0800231c();
s32 Func_08002322();
void Func_08002dd8();
s32 Func_08002f40();
void Func_080030f8();
void Func_080041d8();
void Func_08004278();
s32 Func_08004458();
void Func_080049ac();
void Func_080051d8();
s32 Func_08005340();
void Func_080072f0();
void Func_080072f4();
void Func_08009080();
void Func_08009140();
void Func_08009150();
void Func_080b5088();
s32 Func_080b5098();
void Func_080b50e8();
void Func_080cd52c();
void Func_080cd594();
void Func_080cdbc0();
void Func_080d6888();
s32 Func_080e155c();
void Func_080e3980();
s32 Func_080ed408();
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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Function(s32 a0)
{
    u32 i;
    u8 *p10;
    s32 p10b;
    s32 p11;
    s32 p9;
    s32 p9b;
    s32 p9c;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 r13;
    s32 r8;
    s32 v1;
    s32 base6_3001eec;
    s32 none;
    s32 v3;
    s32 v8;
    s32 v7;
    s32 v11;
    s32 base6_2010000;
    s32 v12;
    s32 v0;
    s32 slot36;
    s32 slot32;
    s32 slot28;
    s32 slot40;
    s32 slot8;
    s32 slot24;
    s32 slot20;
    s32 slot16;
    s32 slot12;
    u8 *p5;
    u8 *p4;
    u8 slot48[12];

    base6_3001eec = 0x3001eec;
    v1 = *(s32 *)(0x3001eec);
    slot36 = v1;
    slot32 = *(s32 *)(0x3001eec + 4);
    slot28 = *(s32 *)(base6_3001eec + 8);
    *(s32 *)((slot36 + 0x7828)) = a0;
    Func_080cd594(0);
    record = Func_08002f40((s32)Data_00000073);
    ((void (*)())Func_08005340)(record, slot28);
    rec7 = Value1(Func_08002f40, (s32)Data_0000007d);
    Call4(Func_080072f0, 0x5000000, rec7, 128, 0x3001388);
    Value2(Func_08005340, (rec7 + 128), slot36);
    ((void (*)())Func_080ed408)(46, 7, 7, 3, 2);
    slot40 = *(s32 *)(base6_3001eec + 28);
    ((void (*)())Func_080ed408)(47, 7, 7, 7, 2);
    slot8 = (r13 + 40);
    *(s32 *)(slot8 + 4) = *(s32 *)(base6_3001eec + 32);
    *(s32 *)((slot36 + 0x7780)) = 2;
    *(s32 *)((slot36 + 0x7784)) = 75;
    Call2(Func_080041d8, 0x80cd261, 0x480);
    none = 0;
    v3 = 0x2010018;
    v8 = none;
    do {
        v8 = (v8 + 1);
        *(s32 *)(v3) = 0;
        v3 = (v3 + 28);
    } while (v8 != 0x400);
    record = Func_080b5098(*(s32 *)(*(s32 *)((slot36 + 0x7828)) + 8));
    p10 = *(s32 *)(record);
    record = Func_080b5098(*(s16 *)(*(s32 *)((slot36 + 0x7828)) + 36));
    slot24 = *(s32 *)(record);
    slot20 = -0xf0000;
    if (*(s32 *)((s32)p10 + 8) <= 0) {
        slot20 = 0xf0000;
    }
    slot16 = (slot36 + 0x7828);
    slot12 = (r13 + 48);
    none = 0;
    v7 = r8;
    v11 = none;
    L_080ce130:;
    p5 = *(s32 *)0x03001e80;
    Func_080049ac();
    Func_080051d8((s32)p5, ((s32)p5 + 12));
    if (v11 <= 17) {
        if (v11 != 0) {
            goto L_080ce164;
        }
    }
    Func_080e3980(*(s32 *)(*(s32 *)(slot16) + 8), slot12);
    *(s32 *)(slot12) = ((*(s32 *)(slot12) + ((u32)*(s32 *)(slot12) >> 31)) >> 1);
    L_080ce164:;
    if ((u32)(v11 - 2) <= 1) {
        Func_080072f4(slot32, slot36, (*(s32 *)(slot12) - 16), (*(s32 *)(slot12 + 4) - 64), 32, 64);
    }
    if ((u32)(v11 - 4) <= 11) {
        none = 0;
        v7 = slot48;
        p9 = ((((v11 - 4) + ((u32)(v11 - 4) >> 31)) >> 1) << 11);
        v8 = none;
        do {
            record = Func_08002322((v8 << 12));
            p5 = *(s32 *)(v7);
            record = Func_0800231c((v8 << 12));
            Call6(Func_080072f4, slot32, (slot36 + p9), (((s32)p5 + ((v11 * record) >> 16)) - 16), (((*(s32 *)(v7 + 4) + ((v11 * record) >> 16)) - v11) - 64), 32, 64);
            v8 = (v8 + 1);
        } while (v8 != 16);
    }
    if (v11 == 4) {
        *(s32 *)((s32)p10 + 40) = 0x140000;
        *(s32 *)((s32)p10 + 52) = 0x10000;
        *(s32 *)((s32)p10 + 48) = 0x30000;
        *(s32 *)((s32)p10 + 72) = 0xab85;
        p10[90] = 0;
        *(u8 *)((((s32)p10 + 90) - 2)) = 0;
        Func_08009150((s32)p10, ((s32)(*(s32 *)((s32)p10 + 8) << 1) + *(s32 *)((s32)p10 + 8)), 0, *(s32 *)((s32)p10 + 16));
        Func_08009080((s32)p10, 2);
        *(s32 *)((slot36 + 0x77a8)) = v11;
        Func_080f9010(136);
    }
    if (v11 == 16) {
        rec7 = Value1(Func_08002f40, (s32)Data_00000089);
        Call4(Func_080072f0, 0x5000000, rec7, 128, 0x3001388);
        ((void (*)())Func_08005340)((rec7 + 128), slot36);
        *(s32 *)((s32)p10 + 72) = 0;
        *(s32 *)((s32)p10 + 36) = 0;
        *(s32 *)((s32)p10 + 40) = 0;
        *(s32 *)((s32)p10 + 16) = *(s32 *)(slot24 + 16);
        Func_08009140((s32)p10);
    }
    if (v11 <= 17) {
    } else {
        if (*(s32 *)((s32)p10 + 12) > 0) {
            *(s32 *)((s32)p10 + 8) += slot20;
            *(s32 *)((s32)p10 + 12) += -0x80000;
            if (*(s32 *)(*(s32 *)(slot16) + 4) == 0) {
                Func_080072f4(slot32, slot36, (*(s32 *)(slot12) - 20), (*(s32 *)(slot12 + 4) - 52), 40, 64);
                *(s32 *)(slot12) = (*(s32 *)(slot12) - 8);
            } else {
                p4 = *(s32 *)(slot8 + 4);
                Call6(Func_080072f4, slot32, slot36, (*(s32 *)(slot12) - 26), (*(s32 *)(slot12 + 4) - 52), 40, 64);
                *(s32 *)(slot12 + 4) += 8;
            }
        }
        if (*(s32 *)((s32)p10 + 12) < 0) {
            *(s32 *)((s32)p10 + 12) = 0;
            none = 0;
            v7 = 0x2010000;
            p9b = slot48;
            v8 = none;
            do {
                record = Func_08004458();
                rec8 = Func_08004458();
                *(s32 *)(v7) = (*(s32 *)p9b << 16);
                *(s32 *)(v7 + 4) = ((s32)(*(s32 *)(p9b + 4) - 24) << 16);
                record = Func_08002322((rec8 & 0xffff));
                *(s32 *)(v7 + 8) = ((((0x3ff & record) + 32) * record) >> 6);
                record = Func_0800231c((rec8 & 0xffff));
                *(s32 *)(v7 + 16) = (-((((0x3ff & record) + 32) * record) << 1) >> 6);
                record = Func_08004458();
                v8 = (v8 + 1);
                *(s32 *)(v7 + 24) = ((7 & record) + 32);
                v7 = (v7 + 28);
            } while (v8 != 0x100);
            *(s32 *)((slot36 + 0x77a8)) = 8;
            Func_080b50e8(145);
            Func_080b5088(*(s16 *)(*(s32 *)(slot16) + 36), 4);
            Func_080d6888(*(s16 *)(*(s32 *)(slot16) + 36), 7, 5, 0, 8);
        }
    }
    base6_2010000 = 0x2010000;
    none = 0;
    v8 = none;
    do {
        p4 = *(s32 *)(base6_2010000 + 24);
        if ((s32)p4 > 0) {
            v12 = (*(s32 *)(base6_2010000) + *(s32 *)(base6_2010000 + 8));
            *(s32 *)(base6_2010000) += *(s32 *)(base6_2010000 + 8);
            *(s32 *)(base6_2010000 + 24) = ((s32)p4 - 1);
            v7 = (*(s32 *)(base6_2010000 + 4) + *(s32 *)(base6_2010000 + 16));
            *(s32 *)(base6_2010000 + 4) += *(s32 *)(base6_2010000 + 16);
            *(s32 *)(base6_2010000 + 8) = ((((*(s32 *)(base6_2010000 + 8) << 3) - *(s32 *)(base6_2010000 + 8)) << 3) / 64);
            v3 = (((((*(s32 *)(base6_2010000 + 16) << 3) - *(s32 *)(base6_2010000 + 16)) << 3) / 64) + 0x2000);
            *(s32 *)(base6_2010000 + 16) = (((((*(s32 *)(base6_2010000 + 16) << 3) - *(s32 *)(base6_2010000 + 16)) << 3) / 64) + 0x2000);
            if (v7 > 0x700000) {
                *(s32 *)(base6_2010000 + 16) = ((-v3 + ((u32)-v3 >> 31)) >> 1);
            } else {
                if ((u32)v12 <= 0x7effff) {
                    if (v7 >= 0) {
                        v0 = ((s32)p4 - 1);
                        if (((s32)p4 - 1) < 0) {
                            v0 = ((s32)p4 + 6);
                        }
                        p4 = *(s32 *)(((1 & v8) << 2) + slot8);
                        Call6(Func_080072f4, slot32, (slot28 + *(u16 *)(0x080ede48 + ((((v0 >> 3) + 1) << 1) - 2))), ((v12 >> 16) - ((((v0 >> 3) + 1) + ((u32)((v0 >> 3) + 1) >> 31)) >> 1)), ((v7 >> 16) - ((v0 >> 3) + 1)), ((v0 >> 3) + 1), (((v0 >> 3) + 1) << 1));
                    }
                }
            }
        }
        v8 = (v8 + 1);
        base6_2010000 = (base6_2010000 + 28);
    } while (v8 != 0x100);
    Value2(Func_080e155c, 16, 16);
    Func_080cd52c();
    *(s32 *)((slot36 + 0x7824)) = 1;
    Func_080030f8(1);
    v11 = (v11 + 1);
    if (v11 != 88) {
        goto L_080ce130;
    }
    Call1(Func_08004278, 0x80cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
    p9c = rec7 + 128;
    p10b = base6_2010000;
    p11 = v7;
    v11 = p11;
}
