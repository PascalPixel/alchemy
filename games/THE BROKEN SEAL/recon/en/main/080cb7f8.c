#include "types.h"

#define Func_080cb7f8 Func_080cb7f8

extern u8 Data_00000057[];
extern u8 Data_00000070[];
extern u8 Data_00000076[];
extern u8 Data_00000100[];
extern u8 Data_00001000[];
s32 Func_080022fc();
s32 Func_0800231c();
s32 Func_08002322();
void Func_08002dd8();
s32 Func_08002f40();
void Func_080030f8();
s32 Func_080041d8();
void Func_08004278();
s32 Func_08004458();
s32 Func_080048b0();
void Func_080049ac();
void Func_080051d8();
void Func_080072f0();
void Func_080072f4();
void Func_080072fc();
void Func_080b5098();
void Func_080cd52c();
void Func_080cd594();
void Func_080cdbc0();
void Func_080d6888();
void Func_080e0524();
s32 Func_080e3944();
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

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

void Func_080cb7f8(s32 a0)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p11b;
    s32 p4;
    s32 p4b;
    s32 p4c;
    s32 p8;
    s32 p9;
    s32 rec;
    s32 rec3;
    s32 record;
    s32 r9;
    s32 r13;
    s32 r8;
    s32 none;
    s32 base5_2010000;
    s32 v8;
    s32 v3;
    s32 v10;
    s32 v0;
    s32 v6;
    s32 v7;
    s32 v2;
    s32 slot36;
    s32 slot24;
    s32 slot20;
    s32 slot28;
    s32 slot8;
    s32 slot12;
    s32 slot16;
    u8 *p6;
    u8 slot40[12];
    u8 slot52[24];

    rec3 = Value2(Func_080048b0, 39, 0x782c);
    record = Value2(Func_080048b0, 40, 0x4000);
    slot36 = record;
    record = Value2(Func_080048b0, 41, 0x60e);
    slot24 = record;
    slot20 = *(s32 *)0x03001e80;
    *(s32 *)((0x7828 + rec3)) = a0;
    Func_080cd594(0);
    *(s32 *)((0x77b4 + rec3)) = 24;
    *(s32 *)((0x77b8 + rec3)) = 0;
    v6 = (s32)Data_00000057;
    *(u16 *)0x04000052 = 0x100c;
    *(u16 *)0x04000020 = (s32)Data_00000100;
    Func_080e0524((s32)Data_00000057, rec3, 1, 0);
    Func_080e0524((s32)Data_00000076, slot24, 0, 0);
    if (*(s32 *)(*(s32 *)((0x7828 + rec3))) != 1) {
        if (*(s32 *)(*(s32 *)((0x7828 + rec3))) <= 1) {
            if (*(s32 *)(*(s32 *)((0x7828 + rec3))) == 0) {
                goto L_080cb8b8;
            }
            goto L_080cb8cc;
        }
        if (*(s32 *)(*(s32 *)((0x7828 + rec3))) == 2) {
            goto L_080cb8c0;
        }
        goto L_080cb8cc;
        L_080cb8b8:;
    } else {
        goto L_080cb8ce;
        L_080cb8c0:;
        goto L_080cb8ce;
        L_080cb8cc:;
    }
    L_080cb8ce:;
    record = Value1(Func_08002f40, (s32)Data_00000057);
    Call4(Func_080072f0, 0x5000000, record, 128, 0x3001388);
    base5_2010000 = 0x2010000;
    none = 0;
    v8 = none;
    do {
        *(s32 *)(base5_2010000 + 4) = 0;
        record = Func_08004458();
        *(s32 *)(base5_2010000) = (0xffff & record);
        record = Func_08004458();
        p4 = v8;
        v8 = (p4 + 1);
        *(s32 *)(base5_2010000 + 8) = ((0x1ff & record) + (s32)((s32)p4 << 1));
        *(s32 *)(base5_2010000 + 24) = -p4;
        base5_2010000 = (base5_2010000 + 28);
    } while (v8 != 128);
    *(s32 *)((0x7780 + r9)) = 2;
    *(s32 *)((0x7784 + r9)) = 75;
    Value2(Func_080041d8, 0x80cd261, 0x480);
    ((void (*)())Func_080ed408)(46, 7, 7, 3);
    slot28 = *(s32 *)0x03001f08;
    v3 = (*(s32 *)(*(s32 *)((0x7828 + r9)) + 24) + 1);
    *(s32 *)(*(s32 *)((0x7828 + r9)) + 24) += 1;
    if (v3 <= 0) {
        *(s32 *)(*(s32 *)((0x7828 + r9)) + 24) = 1;
    }
    if (*(s32 *)(*(s32 *)((0x7828 + r9)) + 24) > 4) {
        *(s32 *)(*(s32 *)((0x7828 + r9)) + 24) = 4;
    }
    Func_080f9010(212);
    slot8 = (r13 + 64);
    slot12 = (slot20 + 12);
    slot16 = (0x7828 + r9);
    none = 0;
    v10 = none;
    L_080cb982:;
    Func_080e3980(*(s32 *)(*(s32 *)(slot16) + 8), slot8);
    *(s32 *)0x04000028 = ((64 - *(s32 *)(slot8)) << 8);
    if (v10 > 49) {
        *(u16 *)0x04000052 = (((s32)Data_00000070 - (v10 << 1)) | (s32)Data_00001000);
    }
    p4b = v10;
    if (p4b == 16) {
        Func_080d6888(*(s16 *)(*(s32 *)(slot16) + 36), 7, -1, 0, 20);
    }
    v7 = r8;
    if (p4b <= 55) {
        p8 = ((s32)(((u32)(s32)p4b >> 31) + p4b) >> 1);
        v0 = p8;
        if (p8 < 0) {
            v0 = (p8 + 3);
        }
        p11 = (v0 >> 2);
        ((void (*)())Func_080ed408)(47, 7, 7, 3, 2);
        p6 = *(s32 *)0x03001f0c;
        Call7(Func_080072fc, slot36, (((s32)((s32)((s32)((s32)p8 - (s32)((s32)p11 << 2)) << 4) + (s32)((s32)p8 - (s32)((s32)p11 << 2))) << 6) + r9), 47, (*(s32 *)(slot8 + 4) - 64), 17, 64, (s32)p6);
        rec = Value2(Func_080022fc, ((s32)p4b / 4), 3);
        Call6(Func_080072fc, slot36, (((((rec << 7) + rec) << 3) + r9) + 0x1100), 40, (*(s32 *)(slot8 + 4) - 36), 24, 43);
        Func_08002dd8(47);
        ((void (*)())Func_080ed408)(47, 7, 7, 7, 2);
        p6 = *(s32 *)0x03001f0c;
        v6 = (s32)p6;
        v7 = ((s32)p8 - (s32)((s32)p11 << 2));
        Call7(Func_080072fc, slot36, (((s32)((s32)((s32)((s32)p8 - (s32)((s32)p11 << 2)) << 4) + (s32)((s32)p8 - (s32)((s32)p11 << 2))) << 6) + r9), 64, (*(s32 *)(slot8 + 4) - 64), 17, 64, (s32)p6);
        Func_080072fc(slot36, (((((rec << 7) + rec) << 3) + r9) + 0x1100), 64, (*(s32 *)(slot8 + 4) - 36), 24, 43);
        Func_08002dd8(47);
    }
    Func_080b5098(*(s32 *)(*(s32 *)(slot16) + 8));
    Func_080049ac();
    Func_080051d8(slot20, slot12);
    base5_2010000 = 0x2010000;
    none = 0;
    v8 = none;
    do {
        if (*(s32 *)(base5_2010000 + 24) >= 0) {
            record = Func_08002322(*(s32 *)(base5_2010000));
            *(s32 *)(slot40) = ((*(s32 *)(base5_2010000 + 8) * record) >> 4);
            record = Func_0800231c(*(s32 *)(base5_2010000));
            *(s32 *)(slot40 + 8) = -((*(s32 *)(base5_2010000 + 8) * record) >> 4);
            *(s32 *)(slot40 + 4) = *(s32 *)(base5_2010000 + 4);
            *(s32 *)(base5_2010000) += 0x400;
            *(s32 *)(base5_2010000 + 4) += 0x50000;
            *(s32 *)(base5_2010000 + 8) += 64;
            Value2(Func_080e3944, slot40, slot52);
            v2 = ((*(s32 *)(slot52) + ((u32)*(s32 *)(slot52) >> 31)) >> 1);
            *(s32 *)(slot52) = ((*(s32 *)(slot52) + ((u32)*(s32 *)(slot52) >> 31)) >> 1);
            Call6(Func_080072f4, slot36, (slot24 + *(u16 *)(0x080ede5c + ((((1 & v8) + *(s32 *)(*(s32 *)(slot16) + 24)) << 1) - 2))), (v2 - ((1 & v8) + *(s32 *)(*(s32 *)(slot16) + 24))), (*(s32 *)(slot52 + 4) - ((1 & v8) + *(s32 *)(*(s32 *)(slot16) + 24))), (((1 & v8) + *(s32 *)(*(s32 *)(slot16) + 24)) << 1), (((1 & v8) + *(s32 *)(*(s32 *)(slot16) + 24)) << 1));
        }
        v8 = (v8 + 1);
        *(s32 *)(base5_2010000 + 24) += 1;
        base5_2010000 = (base5_2010000 + 28);
    } while (v8 != 32);
    Func_080cd52c();
    *(s32 *)((0x7824 + r9)) = 1;
    Func_080030f8(1);
    v10 = (p4b + 1);
    p4c = v10;
    if (p4c != 56) {
        goto L_080cb982;
    }
    Func_08002dd8(46);
    Call1(Func_08004278, 0x80cd261);
    Func_080cdbc0();
    Func_08002dd8(41);
    Func_08002dd8(40);
    Func_08002dd8(39);
    p9 = base5_2010000;
    p10 = slot52;
    v10 = p10;
    p11b = slot40;
}
