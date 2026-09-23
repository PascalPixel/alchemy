#include "types.h"
#include "BATTLE_EFX.H"

#define Func_080e94b8 Func_080e94b8

extern u8 Data_00000055[];
extern u8 Data_00000073[];
extern u8 Data_0000007d[];
extern u8 Data_000000c0[];
s32 Func_080022ec();
s32 Func_0800231c();
s32 Func_08002322();
void Func_08002dd8();
void Func_080030f8();
void Func_080041d8();
void Func_08004278();
s32 Func_08004458();
void Func_080072f4();
s32 Func_08009080();
s32 Func_08009088();
void Func_080b5088();
s32 Func_080b5098();
void Func_080b50e8();
void Func_080cd52c();
void Func_080cd594();
void Func_080cdbc0();
void Func_080cef64();
void Func_080d6888();
void Func_080e155c();
void Func_080e3908();
void Func_080e46f0();
void Func_080f9010();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Func_080e94b8(s32 a0)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p11b;
    s32 p8;
    s32 p8b;
    s32 p8c;
    s32 p9;
    s32 p9b;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 r13;
    s32 v0;
    s32 v8;
    s32 v9;
    s32 none;
    s32 v7;
    s32 v10;
    s32 a0;
    s32 v3;
    s32 v2;
    s32 v5;
    s32 v1;
    s32 base6_24;
    s32 slot40;
    s32 slot32;
    s32 slot28;
    s32 slot24;
    s32 slot36;
    s32 slot20;
    s32 slot16;
    s32 slot12;
    s32 slot8;
    u8 *p4;

    v0 = *(s32 *)(0x3001eec);
    slot40 = *(s32 *)(0x3001eec + 4);
    slot32 = *(s32 *)0x03001ef4;
    p11 = v0;
    record = Func_080b5098(*(s32 *)(a0 + 8));
    slot28 = *(s32 *)(record);
    *(s32 *)(a0 + 24) = 1;
    *(s32 *)((0x7828 + p11)) = a0;
    Func_080cd594(1);
    *(u16 *)0x04000052 = 0x1010;
    slot24 = (r13 + 44);
    Func_080cef64(*(s32 *)(*(s32 *)((0x7828 + p11)) + 4));
    Value2(Func_08009080, slot28, 2);
    Value2(Func_08009088, slot28, 48);
    Resource_LoadAndDecompress((s32)Data_00000055, p11, 1, 1);
    Resource_LoadAndDecompress((s32)Data_0000007d, (0x2000 + p11), 1, 0);
    Resource_LoadAndDecompress((s32)Data_00000073, slot32, 0, 0);
    slot36 = 0;
    p9 = slot36;
    v8 = p11;
    v9 = p9;
    do {
        none = 0;
        v7 = (0x7080 + v8);
        v10 = none;
        for (i = 0; (i >> 16) != 16; i += 0x10000) {
            rec7 = Func_08004458();
            a0 = (rec7 & 0xffff);
            record = Func_08002322((rec7 & 0xffff));
            *(s32 *)(v7) = ((v10 << 1) * record);
            record = Func_0800231c((rec7 & 0xffff));
            *(s32 *)(v7 + 4) = -((v10 << 1) * record);
            v10 = (v10 + 1);
            *(s32 *)(v7 + 24) = ((((u32)v10 >> 31) + v10) >> 1);
            v7 = (v7 + 28);
        }
        none = 0;
        v7 = ((((v9 << 3) - v9) << 2) + 0x2010000);
        v10 = none;
        do {
            record = Func_08004458();
            rec8 = Func_08004458();
            *(s32 *)(v7) = (*(u8 *)(0x080eef06 + (s32)(slot36 + (s32)((s32)(*(s32 *)(*(s32 *)((0x7828 + p11)) + 4) << 1) + *(s32 *)(*(s32 *)((0x7828 + p11)) + 4)))) << 16);
            *(s32 *)(v7 + 4) = 0x580000;
            record = Func_08002322((rec8 & 0xffff));
            *(s32 *)(v7 + 12) = ((((0x1ff & record) + 32) * record) >> 6);
            record = Func_0800231c((rec8 & 0xffff));
            *(s32 *)(v7 + 16) = (-((((0x1ff & record) + 32) * record) << 1) >> 6);
            record = Func_08004458();
            *(s32 *)(v7 + 24) = ((7 & record) + 32);
            v10 = (v10 + 1);
            v3 = 0x154;
            v7 = (v7 + 28);
        } while (v10 != v3);
        v9 = (v9 + v3);
        v8 = (v8 + 0x1c0);
        slot36 = (slot36 + 1);
        a0 = ((0x1ff & record) + 32);
    } while (slot36 != 3);
    *(s32 *)((0x7780 + p11)) = 2;
    *(s32 *)((0x7784 + p11)) = 75;
    Call2(Func_080041d8, 0x80cd261, 0x480);
    none = 0;
    v9 = none;
    L_080e965c:;
    if (v9 == 4) {
        Func_080f9010(212);
    }
    if (v9 == 8) {
        *(s32 *)((0x77a8 + p11)) = v9;
    }
    if (v9 == 18) {
        Func_080f9010(145);
    }
    if (v9 == 40) {
        Func_080b50e8(134);
    }
    if (v9 <= 39) {
        v1 = 128;
        if (*(s32 *)(*(s32 *)((0x7828 + p11)) + 4) == 1) {
            if (v9 <= 9) {
                v2 = ((((v9 << 2) + v9) << 1) - 8);
                v5 = ((v9 << 4) - 128);
                goto L_080e96fe;
            }
            if (v9 > 20) {
                v2 = (v9 + 62);
                v5 = ((v9 << 1) - 24);
                goto L_080e96fe;
            }
            v2 = 82;
        } else {
            if (v9 <= 9) {
                v2 = (128 - (((v9 << 2) + v9) << 1));
                v5 = ((v9 << 4) - 128);
                goto L_080e96fe;
            }
            if (v9 > 20) {
                v2 = (58 - v9);
                v5 = ((v9 << 1) - 24);
                goto L_080e96fe;
            }
            v2 = 38;
        }
        v5 = 16;
        L_080e96fe:;
        if ((v5 + 128) > 104) {
            v1 = ((v1 - v5) - 24);
        }
        if (v1 > 0) {
            Call6(Func_080072f4, slot40, p11, (v2 - 32), v5, 64, v1);
        }
    }
    if (v9 > 16) {
        Func_080e46f0((s32)Data_000000c0);
    }
    slot36 = 0;
    slot20 = 22;
    slot16 = slot36;
    slot12 = 16;
    slot8 = p11;
    L_080e9742:;
    if (v9 == slot12) {
        *(s32 *)((0x77a8 + p11)) = 12;
    }
    if (v9 >= slot12) {
        if (v9 < ((slot36 << 3) + 18)) {
            Func_080072f4(slot40, (0x2000 + p11), (*(u8 *)(0x080eef06 + (s32)(slot36 + (s32)((s32)(*(s32 *)(*(s32 *)((0x7828 + p11)) + 4) << 1) + *(s32 *)(*(s32 *)((0x7828 + p11)) + 4)))) - 16), 56, 32, 64);
        }
        none = 0;
        v5 = (slot8 + 0x7080);
        v10 = none;
        do {
            if ((u32)*(s32 *)(v5 + 24) <= 17) {
                record = Func_080022ec(*(s32 *)(v5 + 24), 3);
                Call6(Func_080072f4, slot40, ((s32)((*(u8 *)(0x080eef0c + record) << 11) + p11) + 0x2000), ((*(s16 *)(v5 + 2) + *(u8 *)(0x080eef06 + (s32)(slot36 + (s32)((s32)(*(s32 *)(*(s32 *)((0x7828 + p11)) + 4) << 1) + *(s32 *)(*(s32 *)((0x7828 + p11)) + 4))))) - 16), (*(s16 *)(v5 + 6) + 56), 32, 64);
            }
            if (*(s32 *)(v5 + 24) > 0) {
                v3 = (*(s32 *)(v5 + 24) - 1);
            } else {
                v3 = -1;
            }
            *(s32 *)(v5 + 24) = v3;
            v10 = (v10 + 1);
            v5 = (v5 + 28);
            v7 = *(s16 *)(v5 + 6);
        } while (v10 != 12);
    }
    if (v9 > (slot12 + 5)) {
        none = 0;
        v10 = none;
        do {
            if (*(s32 *)((((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000) + 24) > 0) {
                Call3(Func_080e3908, (((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000), 64, 0x1000);
                v3 = (*(s32 *)((((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000) + 24) - 1);
                *(s32 *)((((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000) + 24) = (*(s32 *)((((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000) + 24) - 1);
                if (*(s32 *)((((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000) + 4) > 0x6c0000) {
                    *(s32 *)((((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000) + 16) = ((-*(s32 *)((((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000) + 16) + ((u32)-*(s32 *)((((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000) + 16) >> 31)) >> 1);
                } else {
                    if ((u32)*(s32 *)((((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000)) <= 0x7effff) {
                        if (*(s32 *)((((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000) + 4) >= 0) {
                            p8 = (s32)*(u8 **)((((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000) + 4) >> 16;
                            p4 = *(s32 *)(((1 & v10) << 2) + slot24);
                            p8b = ((s32)p8 - (Value2(Func_080022ec, v3, 5) + 1));
                            Func_080072f4(slot40, (slot32 + *(u16 *)(0x080ede48 + (((Value2(Func_080022ec, v3, 5) + 1) << 1) - 2))), ((*(s32 *)((((((((slot16 + (slot16 << 4)) << 2) + v10) << 3) - (((slot16 + (slot16 << 4)) << 2) + v10)) << 2) + 0x2010000)) >> 16) - (((Value2(Func_080022ec, v3, 5) + 1) + ((u32)(Value2(Func_080022ec, v3, 5) + 1) >> 31)) >> 1)), (Value2(Func_080022ec, v3, 5) + 1), ((Value2(Func_080022ec, v3, 5) + 1) << 1));
                        }
                    }
                }
            }
            v10 = (v10 + 1);
        } while (v10 != 0x100);
    }
    none = 0;
    if (*(s32 *)(*(s32 *)(p11 + 30760) + 20) != 0) {
        base6_24 = 36;
        p8c = slot20;
        v2 = 0x7828;
        v10 = none;
        do {
            if (v9 == p8c) {
                v2 = 5;
                Func_080d6888(*(s16 *)(*(s32 *)(((s32)p11 + v2)) + base6_24), 7, v2, v10, 10);
                Func_080b5088(*(s16 *)(*(s32 *)(((s32)p11 + v2)) + base6_24), 4);
            }
            v2 = 0x7828;
            v10 = (v10 + 1);
            base6_24 = (base6_24 + 2);
            v5 = ((s32)p11 + v2);
        } while (v10 != *(s32 *)(*(s32 *)((s32)p11 + v2) + 20));
    }
    slot20 = (slot20 + 8);
    slot16 = (slot16 + 5);
    slot12 = (slot12 + 8);
    slot8 = (slot8 + 0x1c0);
    slot36 = (slot36 + 1);
    if (slot36 != 2) {
        goto L_080e9742;
    }
    Func_080e155c(16, 16);
    v5 = 1;
    Func_080cd52c();
    *(s32 *)((0x7824 + p11)) = v5;
    Func_080030f8(1);
    v9 = (v9 + 1);
    if (v9 != 80) {
        goto L_080e965c;
    }
    ((void (*)())Func_08009088)(slot28, 16);
    Call1(Func_08004278, 0x80cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
    p9b = v5;
    v9 = p9b;
    p10 = base6_24;
    p11b = slot16;
}
