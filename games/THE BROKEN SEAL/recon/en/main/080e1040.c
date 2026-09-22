#include "types.h"
#include "BATTLE_EFX.H"

#define Func_080e1040 Func_080e1040

extern u8 Data_00000094[];
extern u8 Data_000000a7[];
s32 Func_08002304();
s32 Func_0800231c();
s32 Func_08002322();
void Func_08002dd8();
void Func_080030f8();
void Func_080041d8();
void Func_08004278();
s32 Func_08004458();
void Func_080049ac();
void Func_080051d8();
void Func_080072f4();
void Func_080072f8();
void Func_080b5088();
s32 Func_080b5098();
void Func_080cd52c();
void Func_080cd594();
void Func_080cdbc0();
void Func_080d6888();
void Func_080de2f8();
void Func_080e155c();
s32 Func_080e3944();
void Func_080e46f0();
void BattleEffect_LoadWork();
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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Func_080e1040(s32 a0, s32 a1)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p8;
    s32 p9;
    s32 p9b;
    s32 record;
    s32 value;
    s32 v0;
    s32 v5;
    s32 base7_0;
    s32 v6;
    s32 v3;
    s32 base4_80eeca1;
    s32 base4_80eec74;
    s32 none;
    s32 v7;
    s32 v11;
    s32 v1;
    s32 v4;
    s32 v2;
    s32 slot48;
    s32 slot28;
    s32 slot32;
    s32 slot24;
    s32 slot44;
    s32 slot40;
    s32 slot16;
    s32 slot12;
    s32 slot8;
    s32 slot20;
    s32 slot36;
    u8 *p6;
    u8 *p4;
    u8 slot56[4];
    u8 slot52[4];
    u8 slot60[12];

    v0 = *(s32 *)(0x3001eec);
    slot48 = *(s32 *)(0x3001eec + 4);
    p9 = v0;
    slot28 = *(s32 *)0x03001e80;
    *(s32 *)((0x7828 + p9)) = a0;
    Func_080cd594(0);
    Call6(Func_080de2f8, a0, 0, *(s32 *)(*(s32 *)((0x7828 + p9)) + 4), 2, slot56, slot52);
    *(u16 *)0x04000052 = 0x1010;
    if (*(s32 *)(*(s32 *)((0x7828 + p9)) + 4) == 1) {
        BattleEffect_LoadWork(46, 7, 7, 7, 2);
    } else {
        BattleEffect_LoadWork(46, 7, 7, 3, 2);
    }
    slot32 = *(s32 *)0x03001f08;
    Resource_LoadAndDecompress((s32)Data_000000a7, p9, 1, 0);
    Resource_LoadAndDecompress((s32)Data_00000094, (0x65c0 + p9), 1, 1);
    *(s32 *)((0x7780 + p9)) = 2;
    *(s32 *)((0x7784 + p9)) = 75;
    Call2(Func_080041d8, 0x80cd261, 0x480);
    record = Func_080b5098(*(s32 *)(*(s32 *)((0x7828 + p9)) + 8));
    p6 = *(s32 *)(record);
    v5 = 225;
    record = Func_080b5098(*(s16 *)(*(s32 *)((0x7828 + p9)) + 36));
    slot24 = *(s32 *)(record);
    slot44 = 0;
    base7_0 = 0;
    v5 = ((v5 << 7) + p9);
    do {
        *(s32 *)(v5) = *(s32 *)((s32)p6 + 8);
        *(s32 *)(v5 + 4) = 0x420000;
        *(s32 *)(v5 + 8) = *(s32 *)((s32)p6 + 16);
        *(s32 *)(v5 + 12) = (base7_0 >> 5);
        record = Func_08004458();
        *(s32 *)(v5 + 16) = ((((127 & record) - 64) << 16) >> 6);
        record = Func_08004458();
        *(s32 *)(v5 + 20) = ((((255 & record) - 127) << 16) >> 5);
        if (*(s32 *)(v5) > 0) {
            *(s32 *)(v5 + 12) = -*(s32 *)(v5 + 12);
        }
        *(s32 *)(v5 + 24) = 1;
        base7_0 = (base7_0 + 0x500000);
        v5 = (v5 + 28);
        slot44 = (slot44 + 1);
    } while (slot44 != 8);
    slot40 = 0;
    slot16 = (slot28 + 12);
    L_080e118c:;
    if (slot40 > 16) {
        Func_080e46f0((s32)Data_000000a7);
    }
    if (*(s32 *)(*(s32 *)((0x7828 + p9)) + 28) == 1) {
        record = Func_08002322((slot40 << 11));
        value = Func_0800231c((slot40 << 11));
        v5 = ((((value << 1) >> 16) + (*(s32 *)slot52)) - 22);
        if (slot40 > 16) {
            v5 = ((v5 - (slot40 << 1)) + 32);
        }
        if (*(s32 *)(*(s32 *)((0x7828 + p9)) + 4) == 1) {
            BattleEffect_LoadWork(47, 7, 7, 7, 3);
        } else {
            BattleEffect_LoadWork(47, 7, 7, 3);
        }
        if (slot40 <= 3) {
            p4 = *(s32 *)0x03001f0c;
            Call6(Func_080072f4, slot48, (0x65c0 + p9), ((((-record << 2) >> 16) + (((*(s32 *)slot56) + ((u32)(*(s32 *)slot56) >> 31)) >> 1)) - 10), v5, 20, 40);
        }
        Func_08002dd8(47);
        Call6(Func_080072f4, slot48, (0x65c0 + p9), ((((-record << 2) >> 16) + (((*(s32 *)slot56) + ((u32)(*(s32 *)slot56) >> 31)) >> 1)) - 10), v5, 20, 40);
    }
    if ((1 & slot40) == 0) {
        slot44 = 0;
        v5 = (0x7400 + p9);
        do {
            record = Func_08004458();
            *(s32 *)(v5 + 12) = (Func_08002304(record, 6) + 3);
            record = Func_08004458();
            *(s32 *)(v5 + 16) = *(u8 *)(0x080eec70 + (3 & record));
            v5 = (v5 + 28);
            slot44 = (slot44 + 1);
        } while (slot44 != 32);
    }
    Func_080049ac();
    v6 = 225;
    Func_080051d8(slot28, slot16);
    slot44 = 0;
    slot12 = slot44;
    slot8 = p9;
    v6 = ((v6 << 7) + p9);
    L_080e12a0:;
    if (*(s32 *)(v6 + 24) != 1) {
    } else {
        slot20 = slot12;
        if (slot40 <= slot20) {
        } else {
            Value2(Func_080e3944, v6, slot60);
            v3 = (*(s32 *)(slot60) >> 1);
            *(s32 *)(slot60) = (*(s32 *)(slot60) >> 1);
            p10 = (v3 - 12);
            p8 = (*(s32 *)(slot60 + 4) - 24);
            Call6(Func_080072f8, slot48, p9, p10, p8, 24, 48);
            if ((3 & slot40) <= 1) {
                base4_80eeca1 = 0x80eeca1;
                base4_80eec74 = 0x80eec74;
                Call6(Func_080072f8, slot48, (*(u16 *)0x080eec88 + p9), (*(u8 *)0x080eec99 + p10), (*(u8 *)(base4_80eeca1 + 1) + p8), *(u8 *)(base4_80eec74 + 1), *(u8 *)0x080eec7e);
            } else {
                base4_80eec74 = 0x80eec74;
                Call6(Func_080072f8, slot48, (*(u16 *)0x080eec8a + p9), (*(u8 *)0x080eec9a + p10), (*(u8 *)0x080eeca3 + p8), *(u8 *)(base4_80eec74 + 2), *(u8 *)0x080eec7f);
            }
            none = 0;
            v7 = (slot8 + 0x7400);
            v11 = none;
            do {
                BattleEffect_LoadWork(47, 7, 7, *(s32 *)(v7 + 16), 2);
                slot36 = *(s32 *)0x03001f0c;
                if ((4 & *(s32 *)(v7 + 16)) != 0) {
                    v3 = (((s32)((s32)p10 - *(u8 *)(0x080eec74 + *(s32 *)(v7 + 12))) - *(u8 *)(0x080eec98 + *(s32 *)(v7 + 12))) + 24);
                } else {
                    v3 = (*(u8 *)(0x080eec98 + *(s32 *)(v7 + 12)) + p10);
                }
                if ((8 & *(s32 *)(v7 + 16)) != 0) {
                    v5 = (((s32)((s32)p8 - *(u8 *)(0x080eec7d + *(s32 *)(v7 + 12))) - *(u8 *)(0x080eeca1 + *(s32 *)(v7 + 12))) + 48);
                } else {
                    v5 = ((s32)p8 + *(u8 *)(0x080eeca1 + *(s32 *)(v7 + 12)));
                }
                Call6(Func_080072f8, slot48, (*(u16 *)(0x080eec86 + (*(s32 *)(v7 + 12) << 1)) + p9), v3, v5, *(u8 *)(0x080eec74 + *(s32 *)(v7 + 12)), *(u8 *)(0x080eec7d + *(s32 *)(v7 + 12)));
                Func_08002dd8(47);
                v11 = (v11 + 1);
                v7 = (v7 + 28);
            } while (v11 != 4);
            *(s32 *)(v6) += *(s32 *)(v6 + 12);
            *(s32 *)(v6 + 4) += *(s32 *)(v6 + 16);
            *(s32 *)(v6 + 8) += *(s32 *)(v6 + 20);
        }
        if (slot40 > (slot12 + 16)) {
            v1 = (*(s32 *)(v6 + 12) + ((*(s32 *)(slot24 + 8) - *(s32 *)(v6)) >> 8));
            *(s32 *)(v6 + 12) += ((*(s32 *)(slot24 + 8) - *(s32 *)(v6)) >> 8);
            v0 = (*(s32 *)(v6 + 16) + ((0x140000 - *(s32 *)(v6 + 4)) >> 8));
            *(s32 *)(v6 + 16) += ((0x140000 - *(s32 *)(v6 + 4)) >> 8);
            v4 = (*(s32 *)(v6 + 20) + ((*(s32 *)(slot24 + 16) - *(s32 *)(v6 + 8)) >> 8));
            *(s32 *)(v6 + 20) += ((*(s32 *)(slot24 + 16) - *(s32 *)(v6 + 8)) >> 8);
            if (slot40 < (slot20 + 85)) {
                v2 = (((v1 << 4) - v1) << 2);
                if ((((v1 << 4) - v1) << 2) < 0) {
                    v2 = (v2 + 63);
                }
                *(s32 *)(v6 + 12) = (v2 >> 6);
                v2 = (((v0 << 4) - v0) << 2);
                if ((((v0 << 4) - v0) << 2) < 0) {
                    v2 = (v2 + 63);
                }
                *(s32 *)(v6 + 16) = (v2 >> 6);
                v2 = (((v4 << 4) - v4) << 2);
                if ((((v4 << 4) - v4) << 2) < 0) {
                    v2 = (v2 + 63);
                }
                *(s32 *)(v6 + 20) = (v2 >> 6);
            }
            if (*(s32 *)(v6 + 4) <= 0x13ffff) {
                *(s32 *)((0x77a8 + p9)) = 8;
                *(s32 *)(v6 + 24) = 0;
                Func_080f9010(134);
                Func_080d6888(*(s16 *)(*(s32 *)((0x7828 + p9)) + 36), 7, 5, 0, 4);
                Func_080b5088(*(s16 *)(*(s32 *)((0x7828 + p9)) + 36), 4);
            }
        }
    }
    slot12 = (slot12 + 2);
    v6 = (v6 + 28);
    slot8 = (slot8 + 112);
    slot44 = (slot44 + 1);
    if (slot44 != 6) {
        goto L_080e12a0;
    }
    Func_080e155c(16, 16);
    Func_080cd52c();
    *(s32 *)((0x7824 + p9)) = 1;
    Func_080030f8(1);
    slot40 = (slot40 + 1);
    if (slot40 != 96) {
        goto L_080e118c;
    }
    Call1(Func_08004278, 0x80cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
    p9b = slot44;
    p10b = v6;
    p11 = ((((-record << 2) >> 16) + (((*(s32 *)slot56) + ((u32)(*(s32 *)slot56) >> 31)) >> 1)) - 10);
}
