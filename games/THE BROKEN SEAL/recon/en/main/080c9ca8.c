#include "types.h"

#define Func_080c9ca8 Func_080c9ca8

extern u8 Data_00000058[];
extern u8 Data_000000b4[];
extern u8 Data_00001000[];
s32 Func_080022ec();
s32 Func_080022fc();
s32 Func_08002322();
void Func_08002dd8();
s32 Func_08002f40();
void Func_080030f8();
s32 Func_080041d8();
void Func_08004278();
void Func_080049ac();
void Func_080051d8();
void Func_080072f0();
s32 Func_080072f4();
void Func_080b5088();
s32 Func_080b5098();
void Func_080b50e8();
void Func_080cd52c();
void Func_080cd594();
void Func_080cdbc0();
void Func_080d6888();
void Func_080e0524();
void Func_080e155c();
void Func_080e38b8();
void Func_080e3944();
void Func_080ed408();
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Func_080c9ca8(s32 a0, s32 a1)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p4b;
    s32 p8;
    s32 p8b;
    s32 p9;
    s32 p9b;
    s32 p9c;
    s32 record;
    s32 value;
    s32 v1;
    s32 base6_3001eec;
    s32 v9;
    s32 none;
    s32 v7;
    s32 v11;
    s32 v10;
    s32 v5;
    s32 v6;
    s32 v2;
    s32 v3;
    s32 slot68;
    s32 slot64;
    s32 slot60;
    s32 slot44;
    s32 slot48;
    s32 slot52;
    s32 slot40;
    s32 slot56;
    s32 slot24;
    s32 slot28;
    s32 slot36;
    s32 slot20;
    s32 slot16;
    s32 slot12;
    s32 slot8;
    s32 slot32;
    u8 *p4;
    u8 *p6;
    u8 *p5;
    u8 slot72[12];

        base6_3001eec = 0x3001eec;
        base6_3001eec = 0x3001eec;
    slot68 = a1;
    v1 = *(s32 *)(0x3001eec);
    slot64 = v1;
    slot60 = *(s32 *)(0x3001eec + 4);
    slot44 = *(s32 *)0x03001e80;
    *(s32 *)((slot64 + 0x7828)) = a0;
    Func_080cd594(1);
    if (*(s32 *)(*(s32 *)((slot64 + 0x7828)) + 4) == 0) {
        Func_080ed408(46, 7, 7, 11, 2);
        slot48 = *(s32 *)(base6_3001eec + 28);
    } else {
        Func_080ed408(46, 7, 7, 15, 2);
        p4 = *(s32 *)(base6_3001eec + 28);
        slot48 = (s32)p4;
    }
    Func_080ed408(47, 7, 7, 3, 2);
    p6 = *(s32 *)(base6_3001eec + 32);
    slot52 = (s32)p6;
    Func_080e0524((s32)Data_00000058, slot64, 0, 0);
    if ((u32)slot68 <= 4) {
    } else {
    }
    record = Value1(Func_08002f40, (s32)Data_000000b4);
    Call4(Func_080072f0, 0x5000000, record, 128, 0x3001388);
    *(s32 *)((slot64 + 0x7780)) = 2;
    *(s32 *)((slot64 + 0x7784)) = 50;
    Value2(Func_080041d8, 0x80cd261, 0x480);
    record = Func_080b5098(*(s32 *)(*(s32 *)((slot64 + 0x7828)) + 8));
    p10 = *(s32 *)(record);
    slot40 = (((((*(s32 *)(*(s32 *)((slot64 + 0x7828)) + 20) * 3) << 1) + (*(s32 *)(*(s32 *)((slot64 + 0x7828)) + 20) * 3)) << 1) + 48);
    slot56 = 0;
    if (*(s32 *)(*(s32 *)((slot64 + 0x7828)) + 20) != 0) {
        p9 = slot56;
        v9 = p9;
        do {
            record = Func_080b5098(*(s16 *)(*(s32 *)((slot64 + 0x7828)) + ((slot56 << 1) + 36)));
            p4b = v9;
            p8 = *(s32 *)(record);
            none = 0;
            v7 = ((s32)((s32)((s32)p4b << 2) + slot64) + 0x7080);
            v11 = none;
            do {
                *(s32 *)(v7) = *(s32 *)(p10 + 8);
                p5 = *(s32 *)(p10 + 12);
                *(s32 *)(v7 + 4) = ((s32)p5 + 0x140000);
                p6 = *(s32 *)(p10 + 16);
                *(s32 *)(v7 + 8) = (s32)p6;
                record = Value2(Func_080022ec, (*(s32 *)(p8 + 8) - *(s32 *)(p10 + 8)), 24);
                *(s32 *)(v7 + 12) = record;
                record = Value2(Func_080022ec, ((s32)(*(s32 *)(p8 + 12) + 0x140000) - (s32)((s32)p5 + 0x140000)), 24);
                *(s32 *)(v7 + 16) = record;
                record = Value2(Func_080022ec, (*(s32 *)(p8 + 16) - (s32)p6), 24);
                *(s32 *)(v7 + 20) = record;
                v11 = (v11 + 1);
                *(s32 *)(v7 + 24) = 0;
                v7 = (v7 + 28);
            } while (v11 != 3);
            v9 = (p4b + (24 - 3));
            slot56 = (slot56 + 1);
        } while (slot56 != *(s32 *)(*(s32 *)((slot64 + 0x7828)) + 20));
    }
    none = 0;
    if (slot40 == 0) {
    } else {
        slot24 = (slot40 - 16);
        slot28 = (slot44 + 12);
        v10 = none;
        L_080c9e9a:;
        slot56 = 0;
        v5 = (v10 << 12);
        do {
            value = Func_08002322(v5);
            *(s32 *)((slot64 + 0x6980)) = ((0x80000 - (value << 1)) >> 10);
            v6 = ((slot64 + 0x6980) + 4);
            v5 = (v5 + 0x1000);
            slot56 = (slot56 + 1);
        } while (slot56 != 160);
        if (v10 > slot24) {
            *(u16 *)0x04000052 = ((slot40 - v10) | (s32)Data_00001000);
        }
        Func_080049ac();
        Func_080051d8(slot44, slot28);
        slot56 = 0;
        if (*(s32 *)(*(s32 *)((slot64 + 0x7828)) + 20) == 0) {
        } else {
            slot36 = (slot64 + 0x7828);
            slot20 = 36;
            slot16 = (v10 - 30);
            slot12 = 0;
            slot8 = slot12;
            L_080c9f32:;
            if (v10 < slot12) {
            } else {
                slot32 = (slot68 << 2);
                none = 0;
                p9b = slot12;
                v9 = p9b;
                v11 = none;
                L_080c9f46:;
                if (v10 >= v9) {
                    p8b = (0x7080 + (slot64 + ((((slot8 + v11) << 3) - (slot8 + v11)) << 2)));
                    v6 = slot72;
                    Func_080e3944(p8b, slot72);
                    v7 = (*(s32 *)(slot72) >> 1);
                    *(s32 *)(slot72) = (*(s32 *)(slot72) >> 1);
                    p5 = *(s32 *)(p8b + 24);
                    v5 = (s32)p5;
                    if ((s32)p5 < 0) {
                        v5 = ((s32)p5 + 7);
                    }
                    v2 = (v5 >> 3);
                    if ((v5 >> 3) > 5) {
                        v2 = 5;
                    }
                    if (*(s8 *)(0x080edee8 + slot32) != 0) {
                        value = Value2(Func_080022fc, ((((u32)v10 >> 31) + v10) >> 1), 3);
                        Call6(Func_080072f4, slot60, (slot64 + ((((((v2 << 1) + v2) << 3) + v2) << 5) + (((((value << 2) + value) << 4) - ((value << 2) + value)) << 6))), (v7 - 10), (*(s32 *)(slot72 + 4) - 40), 20, 40);
                        Call6(Func_080072f4, slot60, (slot64 + ((((((v2 << 1) + v2) << 3) + v2) << 5) + (((((value << 2) + value) << 4) - ((value << 2) + value)) << 6))), (*(s32 *)(slot72) - 10), *(s32 *)(slot72 + 4), 20, 40);
                    } else {
                        (void)Func_080072f4(slot60, ((slot64 + (((((v2 << 1) + v2) << 3) + v2) << 5)) + 0x2580), (v7 - 10), (*(s32 *)(slot72 + 4) - 40), 20, 40);
                        Call6(Func_080072f4, slot60, ((slot64 + (((((v2 << 1) + v2) << 3) + v2) << 5)) + 0x2580), (*(s32 *)(slot72) - 10), *(s32 *)(slot72 + 4), 20, 40);
                    }
                    Func_080e38b8(p8b, 64, 0);
                    *(s32 *)(p8b + 24) += 1;
                }
                v11 = (v11 + 1);
                v9 = (v9 + 6);
                if (v11 != 3) {
                    goto L_080c9f46;
                }
                if (*(s8 *)(0x080edee8 + (slot32 + 3)) != 0) {
                    if (v10 >= (slot12 + 30)) {
                        if (v10 < (slot12 + 62)) {
                            record = Func_080b5098(*(s16 *)(*(s32 *)(slot36) + slot20));
                            v3 = slot16;
                            if (slot16 < 0) {
                                v3 = ((v10 - slot12) - 23);
                            }
                            v2 = (*(s32 *)(*(s32 *)(record) + 8) + (*(s8 *)(0x080edefc + (slot16 - ((v3 >> 3) << 3))) << 16));
                            *(s32 *)(*(s32 *)(record) + 8) += (*(s8 *)(0x080edefc + (slot16 - ((v3 >> 3) << 3))) << 16);
                            if (v2 > 0) {
                                v3 = (v2 + 0x8000);
                            } else {
                                v3 = (v2 + -0x8000);
                            }
                            *(s32 *)(*(s32 *)(record) + 8) = v3;
                            Func_080d6888(*(s16 *)(*(s32 *)(slot36) + slot20), -1, 5, -1, 0);
                        }
                    }
                }
                if (*(s8 *)(0x080edee8 + (slot32 + 1)) != 0) {
                    if (v10 == (slot12 + 24)) {
                        Func_080f9010(133);
                        if (slot56 == 0) {
                            Call1(Func_080b50e8, -1);
                        }
                        Func_080d6888(*(s16 *)(*(s32 *)(slot36) + slot20), 7, 5, slot56, 8);
                    }
                    if (v10 == (slot12 + 40)) {
                        Func_080d6888(*(s16 *)(*(s32 *)(slot36) + slot20), 7, 5, slot56, 8);
                    }
                }
                if (*(s8 *)(0x080edee8 + (slot32 + 2)) != -1) {
                    if (v10 == (slot12 + 24)) {
                        *(s32 *)((slot64 + 0x77a8)) = 4;
                        Func_080b5088(*(s16 *)(*(s32 *)(slot36) + slot20));
                    }
                }
            }
            slot16 = (slot16 - 32);
            slot20 = (slot20 + 2);
            slot12 = (slot12 + 32);
            slot8 = (slot8 + 3);
            slot56 = (slot56 + 1);
            if (slot56 != *(s32 *)(*(s32 *)(slot36) + 20)) {
                goto L_080c9f32;
            }
        }
        Func_080e155c(8, 8);
        Func_080cd52c();
        *(s32 *)((slot64 + 0x7824)) = 1;
        Func_080030f8(1);
        v10 = (v10 + 1);
        if (v10 != slot40) {
            goto L_080c9e9a;
        }
    }
    Call1(Func_08004278, 0x80cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
    p9c = ((s32)p5 + 0x140000);
    v9 = p9c;
    v10 = (s32)p6;
    p11 = v7;
    v11 = p11;
}
