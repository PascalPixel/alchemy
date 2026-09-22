#include "types.h"

#define Func_080b0aac Func_080b0aac

extern u8 Data_00000075[];
s32 Func_080022ec();
s32 Func_080022fc();
void Func_080030f8();
s32 Func_08015010();
s32 Func_08015018();
void Func_08015120();
void Func_08015270();
s32 Func_08077018();
s32 Func_08077028();
void Func_08077058();
s32 Func_08077220();
s32 Func_08077238();
void Func_08077240();
s32 Func_08077248();
void Func_080a1028();
void Func_080a1030();
s32 Func_080b0070();
void Func_080b04dc();
void Func_080b0574();
s32 Func_080b0634();
void Func_080b0a6c();
void Func_080b0fa4();
void Func_080b10cc();
void Func_080b110c();
void Func_080b11a4();
void Func_080b11c4();
void Func_080b1260();
void Func_080b1470();
s32 Func_080b153c();
void Func_080b17e4();
void Func_080b24e4();
void Func_080f9010();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

s32 Func_080b0aac(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 p10;
    s32 p8;
    s32 p8b;
    s32 p9;
    s32 rec;
    s32 rec3;
    s32 rec8;
    s32 record;
    s32 value;
    s32 v7;
    s32 base11_1;
    s32 none;
    s32 v8;
    s32 v6;
    s32 base5_3001b04;
    s32 v5;
    s32 slot32;
    s32 slot16;
    s32 slot28;
    s32 slot24;
    s32 slot8;
    s32 slot12;
    s32 slot4;
    s32 slot20;
    u8 *p5;
    u8 *p6;

    slot32 = 0;
    slot16 = slot32;
    p10 = *(s32 *)0x03001f2c;
    *(s32 *)(p10 + 32) = 0;
    record = Func_08015010(18, 7, 12, 4, 2);
    *(s32 *)(p10 + 12) = record;
    Func_080b10cc();
    record = Func_08015010(0, 8, 15, 4, 2);
    slot32 = record;
    L_080b0af4:;
    record = Func_08015010(0, 12, 30, 4, 2);
    slot28 = record;
    *(u8 *)(*(s32 *)((0x380 + p10)) + 5) = 18;
    *(u8 *)((0x3a8 + p10)) = 12;
    record = Func_08015010(0, 17, 30, 3, 2);
    slot24 = record;
    base11_1 = 1;
    v7 = slot16;
    L_080b0b32:;
    p9 = ((s32)(*(u8 *)((0x3a6 + p10)) << 24) >> 24);
    if (base11_1 != 0) {
        p5 = *(s16 *)((v7 << 1) + (s32)(0x26c + p10));
        rec8 = Func_08077018((s32)p5);
        none = 0;
        base11_1 = none;
        value = Value2(Func_080022fc, v7, 7);
        Func_080b0a6c(slot28, ((value << 5) - 8), 8);
        *(u8 *)((0x3a8 + p10)) = 4;
        Func_080b0fa4(slot28, v7);
        Func_080b11a4(slot24, ((s32)p5 + (s32)Data_00000075));
        Func_08015270(slot32);
        Func_080b110c(slot32, (s32)p5, *(s16 *)(rec8), 0);
    }
    if ((*(s32 *)0x03001c94 & 1) != 0) {
    } else {
        if ((*(s32 *)0x03001c94 & 2) != 0) {
        } else {
            if ((*(s32 *)0x03001b04 & 32) != 0) {
                p8 = v7;
                v7 = (p8 - 1);
                rec = Value1(Func_080022fc, (v7 + (s32)p9));
                v7 = rec;
                if (p8 != v7) {
                    Func_080f9010(111);
                    base11_1 = 1;
                }
            }
            if ((*(s32 *)0x03001b04 & 16) != 0) {
                p8b = v7;
                v7 = (p8b + 1);
                rec = Value1(Func_080022fc, (v7 + (s32)p9));
                v7 = rec;
                if (p8b != v7) {
                    Func_080f9010(111);
                    base11_1 = 1;
                }
            }
            if ((*(s32 *)0x03001b04 & 64) != 0) {
                if ((v7 - 7) >= 0) {
                    v7 = (v7 - 7);
                    base11_1 = 1;
                }
            }
            if ((*(s32 *)0x03001b04 & 128) != 0) {
                value = Value2(Func_080022ec, (p9 + 6), 7);
                if ((v7 + 7) < ((value << 3) - value)) {
                    v7 = (v7 + 7);
                    base11_1 = 1;
                }
                if (v7 > (p9 - 1)) {
                    v7 = (p9 - 1);
                }
            }
            Func_080030f8(1);
            goto L_080b0b32;
            v8 = p8b;
            L_080b0c5a:;
            ((void (*)())Func_08015018)(slot24, 2);
            Value2(Func_08015018, slot28, 2);
            Func_080030f8(1);
            if (v8 != 0) {
                goto L_080b0f56;
            }
            *(u16 *)((0x39e + p10)) = *(u16 *)((s32)p10 + ((slot16 << 1) + 0x26c));
            Call1(Func_080b04dc, 0xc9d);
            record = Func_08077018(*(u16 *)((0x39e + p10)));
            slot8 = record;
            slot12 = 1;
            slot4 = 0;
            record = Func_08015010(0, 14, 13, 3, 2);
            slot20 = record;
            *(u8 *)(*(s32 *)((0x380 + p10)) + 5) = 4;
            *(u8 *)((0x3a8 + p10)) = 12;
            Func_080a1028(slot20, 2, 0, 8, v8);
            rec3 = Func_08015010(16, 11, 14, 9, 2);
            v7 = 0;
            base11_1 = 1;
            L_080b0cf0:;
            if (slot4 != 0) {
                slot4 = 0;
                Call1(Func_080b04dc, 0xc9d);
                base11_1 = 1;
            }
            v6 = slot4;
            if (base11_1 != 0) {
                base11_1 = none;
                none = 0;
                rec = Func_080022fc((v7 + *(s8 *)((0x3a7 + p10))));
                v7 = rec;
                p6 = *(s16 *)((s32)(p10 + 2) + ((v7 << 1) + 0x36c));
                v6 = (s32)p6;
                Func_080b0a6c(slot20, ((((v7 << 1) + v7) << 3) - 12), 0);
                *(u8 *)((0x3a8 + p10)) = 3;
                Func_080b11c4(slot20, v7, *(u16 *)((0x39e + p10)));
                if (Value1(Func_08077238, *(u16 *)((0x39e + p10))) == 0) {
                    Func_080b1470(rec3, (s32)p6, *(u16 *)((0x39e + p10)));
                } else {
                    Func_080b1260(rec3, (s32)p6, *(u16 *)((0x39e + p10)));
                }
            }
            if ((*(s32 *)0x03001c94 & 1) != 0) {
                record = Value2(Func_08077028, v6, *(u16 *)((0x39e + p10)));
                if (record < 0) {
                    Func_080f9010(113);
                    Func_08015120(v6, 1);
                    Func_08015120(*(u16 *)((0x39e + p10)), 2);
                    if (Value1(Func_08077248, v6) == 15) {
                        Call1(Func_080b04dc, 0xc9e);
                        goto L_080b0cf0;
                    }
                    Call1(Func_080b04dc, 0xca6);
                    goto L_080b0cf0;
                }
                Func_08077058(v6);
                if ((u32)*(s16 *)(slot8) > *(s32 *)0x02000250) {
                    goto L_080b0f26;
                }
                if (Value2(Func_08077220, v6, *(u16 *)((0x39e + p10))) == 0) {
                    Func_08015120(v6, 1);
                    Call1(Func_080b04dc, 0xc9f);
                    record = Func_080b0634(0);
                    slot4 = 1;
                    if (record != 0) {
                        goto L_080b0cf0;
                    }
                }
                Func_080f9010(112);
                Func_080030f8(1);
                record = Value2(Func_080b153c, v6, *(u16 *)((0x39e + p10)));
                slot12 = record;
                slot4 = 1;
                if (slot12 == -1) {
                    goto L_080b0cf0;
                }
                Func_080b17e4(v6, *(u16 *)((0x39e + p10)), slot12);
                Func_080b24e4(slot20, rec3);
                none = 0;
                v8 = none;
            } else {
                if ((*(s32 *)0x03001c94 & 2) != 0) {
                    goto L_080b0f18;
                }
                base5_3001b04 = 0x3001b04;
                if ((*(s32 *)base5_3001b04 & 32) != 0) {
                    Func_080f9010(111);
                    v7 = (v7 - 1);
                    base11_1 = 1;
                }
                if ((*(s32 *)base5_3001b04 & 16) != 0) {
                    Func_080f9010(111);
                    v7 = (v7 + 1);
                    base11_1 = 1;
                }
                Func_080030f8(1);
                goto L_080b0cf0;
            }
            L_080b0eaa:;
            Func_080a1030();
            ((void (*)())Func_08015018)(rec3, 2);
            Value2(Func_08015018, slot20, 2);
            Func_080030f8(1);
            if (v8 == 0) {
                if (((s32)(*(u8 *)((0x3aa + p10)) << 24) >> 24) == 2) {
                    if (v8 < slot12) {
                        v5 = slot12;
                        do {
                            v5 = (v5 - 1);
                            Call2(Func_08077240, *(u16 *)((0x39e + p10)), -1);
                        } while (v5 != 0);
                    }
                    record = Value0(Func_080b0070);
                    if (record == 0) {
                        goto L_080b0f56;
                    }
                    if (slot16 > (((s32)(*(u8 *)((0x3a6 + p10)) << 24) >> 24) - 1)) {
                        slot16 = (((s32)(*(u8 *)((0x3a6 + p10)) << 24) >> 24) - 1);
                    }
                }
            }
            Call1(Func_080b04dc, 0xca8);
            goto L_080b0af4;
            L_080b0f18:;
            Func_080f9010(113);
            v8 = -1;
            goto L_080b0eaa;
            L_080b0f26:;
            Func_080f9010(113);
            Call1(Func_080b0574, 0xc9c);
            v8 = -1;
            goto L_080b0eaa;
        }
        Func_080f9010(113);
        v8 = -1;
        goto L_080b0c5a;
    }
    slot16 = v7;
    Func_080f9010(112);
    none = 0;
    v8 = none;
    goto L_080b0c5a;
    L_080b0f56:;
    ((void (*)())Func_08015018)(slot32, 2);
    ((void (*)())Func_08015018)(*(s32 *)(p10 + 12), 2);
    Func_080030f8(1);
    return 0;
}
