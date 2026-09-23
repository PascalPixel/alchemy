#include "types.h"

#define Func_080ae2f4 Func_080ae2f4

extern u8 Data_000000c8[];
s32 Func_080022ec();
s32 Func_080022fc();
void Func_08002df0();
void Func_080030f8();
void Func_0800352c();
s32 Func_080041d8();
s32 Func_08004970();
void Func_080072f0();
void Func_08015060();
void Func_08015080();
void Func_08015270();
void Func_08015278();
void Func_08015280();
s32 Func_08077008();
void Func_080a10d0();
s32 Func_080a1114();
s32 Func_080aa538();
s32 Func_080aae14();
void Func_080acab8();
void Func_080ad5b4();
void Func_080f9010();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ void Call9(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8);
}

s32 Func_080ae2f4(void)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p9;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 r13;
    s32 v2;
    s32 v3;
    s32 v7;
    s32 v0;
    s32 base5_baa;
    s32 none;
    s32 base5_0;
    s32 v1;
    s32 slot52;
    s32 slot56;
    s32 slot48;
    s32 slot20;
    s32 slot76;
    u8 slot68[16];
    s32 slot44;
    s32 slot40;
    u8 *slot24;
    s32 slot36;
    s32 slot32;
    s32 slot28;
    u8 *p6;
    u8 slot64[4];
    u8 slot60[4];

    slot52 = 1;
    slot56 = 0;
    slot48 = 0;
    p9 = *(s32 *)0x03001f2c;
    *(u8 *)(*(s32 *)(*(s32 *)0x03001f2c + 20) + 5) = 13;
    slot20 = (r13 + 76);
    (*(s32 *)((*(s32 *)slot68) + 8)) = 0;
    *(s32 *)(slot20 + 4) = (*(s32 *)((*(s32 *)slot68) + 8));
    v2 = 3;
    v3 = (0x14a + p9);
    do {
        v2 = (v2 - 1);
        *(u16 *)(v3) = (s32)Data_000000c8;
        v3 = (v3 - 2);
    } while (v2 >= 0);
    Func_08015278(*(s32 *)(p9 + 48), p9, v2, v3);
    v7 = 1;
    Func_080030f8(1);
    (*(s32 *)slot68) = v7;
    *(s32 *)(slot68 + 4) = (*(s32 *)slot68);
    p11 = slot68;
    rec7 = Value1(Func_08004970, 96);
    rec8 = Value1(Func_08004970, 0x14c);
    record = Value1(Func_08077008, *(u8 *)((0x21a + p9)));
    record = Func_080aae14((record + 88), (record + 88), rec7, slot64, slot60);
    (*(s32 *)slot68) = record;
    *(s32 *)(p11 + 4) = (*(s32 *)slot68);
    Func_08002df0(rec8);
    Func_08002df0(rec7);
    (*(s32 *)slot68) = (Func_080022ec(((*(s32 *)slot68) - 1), 6) + 1);
    if ((*(s32 *)slot68) == 0) {
    }
    v0 = (Func_080022ec((*(s32 *)(p11 + 4) - 1), 6) + 1);
    *(s32 *)(p11 + 4) = (Func_080022ec((*(s32 *)(p11 + 4) - 1), 6) + 1);
    if (v0 == 0) {
        *(s32 *)(p11 + 4) = (*(s32 *)slot68);
    }
    slot44 = (p9 + 36);
    Call6(Func_080a10d0, slot44, 0, 5, 15, 15, 2);
    slot40 = (p9 + 52);
    Call6(Func_080a10d0, slot40, 15, 5, 15, 15, 2);
    Func_08015270(*(s32 *)((0x10c + p9)));
    Func_08015270(*(s32 *)(p9 + 16));
    base5_baa = 0xbaa;
    Func_08015080(base5_baa, *(s32 *)(p9 + 16), 0, 0);
    Func_08015080((base5_baa + 2), *(s32 *)(p9 + 16), 0, 16);
    slot24 = (0x258 + p9);
    none = 0;
    p10 = slot20;
    L_080ae43c:;
    slot36 = *(s32 *)0x03001e8c;
    slot32 = *(s32 *)0x03001c94;
    slot28 = *(s32 *)0x03001b04;
    if (slot52 != 0) {
        *(u8 *)((slot36 + 0xea6)) = 1;
        Func_08015060(*(s32 *)(p9 + 36));
        Func_08015060(*(s32 *)(p9 + 52));
        Call9(Func_080acab8, *(s32 *)(p9 + 36), 0, 0, slot24[0], none, none, 3, none, 1);
        Call9(Func_080acab8, *(s32 *)(p9 + 52), 0, 0, slot24[0], none, none, 3, ((*(s32 *)((*(s32 *)slot68) + 8)) + 1), 1);
        *(u8 *)((slot36 + 0xea6)) = none;
    }
    if (*(s32 *)(0x00000000 + (s32)p11) > 1) {
            base5_0 = 0;
        p6 = *(s32 *)(p9 + 52);
        if (0 < *(s32 *)(0x00000000 + (s32)p11)) {
            do {
                v1 = (base5_0 + 0xf031);
                if (base5_0 > 9) {
                    v1 = 0xf030;
                }
                if (base5_0 == *(s32 *)(0x00000000 + slot20)) {
                }
                Func_08015280((s32)p6, (v1 + -0x1000), (((s32)(*(u16 *)((s32)p6 + 8) - *(s32 *)p11) + base5_0) - 2), -1, none);
                base5_0 = (base5_0 + 1);
            } while (base5_0 < *(s32 *)p11);
        }
        Func_08015280((s32)p6, 0xf128, ((*(u16 *)((s32)p6 + 8) - *(s32 *)(0x00000000 + (s32)p11)) - 3), -1, none);
        Func_08015280((s32)p6, 0xf129, (*(u16 *)((s32)p6 + 8) - 2), -1, none);
        *(u8 *)((slot36 + 0xea3)) |= (s32)(2 << (s32)((u32)*(u16 *)((s32)p6 + 14) >> 2));
    }
    slot48 = (slot48 + 1);
    record = Func_080022fc((slot48 + 1), 60);
    Func_080ad5b4(0, 32, 200, 0);
    if (slot52 != 0) {
        slot52 = 0;
        record = Value2(Func_080aa538, slot56, 2);
        slot56 = record;
    }
    if ((slot48 & 3) == 0) {
        if ((4 & slot48) != 0) {
            Call4(Func_080072f0, 0x60052c0, 0x80af26c, 32, 0x3001388);
        } else {
            Call4(Func_080072f0, 0x60052c0, 32, 0x44444444, 0x3000168);
        }
    }
    if ((8 & slot32) != 0) {
        v7 = 2;
    } else {
        if ((0x303 & slot32) == 0) {
            goto L_080ae5ce;
        }
        v7 = 1;
    }
    Func_080f9010(113);
    v7 = -v7;
    goto L_080ae638;
    L_080ae5ce:;
    if ((32 & slot28) != 0) {
        *(s32 *)p10 = (*(s32 *)p10 - 1);
        record = Func_080aa538((*(s32 *)p10 - 1), *(s32 *)(none + (s32)p11));
        *(s32 *)p10 = record;
        Func_080f9010(111);
        Func_0800352c();
        slot52 = 1;
    } else {
        if ((16 & slot28) != 0) {
            *(s32 *)p10 += 1;
            Func_080f9010(111);
            Func_0800352c();
            slot52 = 1;
            record = Func_080aa538(*(s32 *)p10, *(s32 *)(none + (s32)p11));
            *(s32 *)p10 = record;
        }
    }
    Func_080030f8(1);
    goto L_080ae43c;
    L_080ae638:;
    v7 = (0 + p11);
    Value2(Func_080041d8, 0x80a19a1, 0xc80);
    *(u8 *)(*(s32 *)0x03001e8c + 0xea6) = 1;
    none = 0;
    ((void (*)())Func_080a1114)((0x10c + p9), 1);
    Func_080030f8(1);
    Call6(Func_080a10d0, (0x10c + p9), 13, 0, 17, 5, 2);
    Value2(Func_080a1114, slot44, 1);
    Value2(Func_080a1114, slot40, 1);
    Func_08015270(*(s32 *)(p9 + 48));
    Func_08015270(*(s32 *)(p9 + 40));
    Func_08015270(*(s32 *)(p9 + 16));
    *(u8 *)((*(s32 *)0x03001e8c + 0xea6)) = none;
    Func_080030f8(1);
    return v7;
}
