#include "types.h"

#define Scene_RunScene3bbSequenceC Func_02003638

s32 Func_020074bc();
void Func_020074d6();
void Func_020074e2();
void Func_020074fc();
void Func_0200750e();
s32 Func_02007574();
void Func_02007584();
void Func_020075c6();
void Func_02007606();
void Func_02007636();
s32 Func_0200767c();
s32 Func_0200767c_a();
s32 Func_02007692();
void Func_020076d2();
s32 Func_02007704();
s32 Func_0200771a();
void Func_0200776e();
void Func_020077f0();
s32 Func_02007942();
s32 Func_020079ca();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void Scene_RunScene3bbSequenceC(s32 a0)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p11b;
    s32 p8;
    s32 p8b;
    s32 p9;
    s32 rec6;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 r9;
    s32 v3;
    s32 v5;
    s32 base6_0;
    s32 v8;
    s32 v4;
    s32 base10_800000;
    s32 v6;
    s32 none;
    s32 v7;
    s32 slot12;
    s32 slot16;
    s32 slot8;
    s32 slot4;
    s32 slot0;

    p8 = *(s32 *)0x03001f3c;
    slot12 = *(s32 *)0x03001f3c;
    slot16 = *(s32 *)0x03001f3c;
    slot8 = ((u32)*(u16 *)(((s32)(*(s16 *)(p8 + 216) << 2) + 0x3001b10) + 2) >> 5);
    p11 = *(s16 *)(p8 + 230);
    if (*(s16 *)(((p8 + 230) - 10)) != 0) {
        {
            s32 shown = 2;
        
            *(u16 *)(p8 + 218) = shown;
        }
    } else {
        if (Value1(Func_02007574, 0x106) != 0) {
            if (*(s16 *)(p8 + 218) <= 0) {
                goto L_020036f4;
            }
            *(u16 *)(p8 + 218) = (*(u16 *)(p8 + 218) - 1);
        } else {
            if (*(s16 *)(p8 + 218) <= 1) {
                v3 = (*(u16 *)(p8 + 218) + 1);
                *(u16 *)(p8 + 218) += 1;
                if ((v3 << 16) == 0x10000) {
                    *(s32 *)(0x40000d4) = 0x200c174;
                    *(s32 *)(0x40000d4 + 4) = 0x50003c0;
                    *(s32 *)(0x40000d4 + 8) = -0x7ffffff0;
                    rec7 = Value4(Func_020074bc, 0x200, 0x50003c0, -0x7ffffff0, ((0x40000d4 + 12) - 12));
                    Call2(Func_020074d6, 0x200c194, rec7);
                    Call3(Func_020074fc, *(s16 *)(p8 + 216), 0x200, rec7);
                    Func_020074e2(rec7);
                }
            }
        }
    }
    L_020036f4:;
    if (*(s16 *)(p8 + 218) == 0) {
        Func_0200750e(*(s16 *)(slot12 + 216));
        v5 = r9;
        v7 = (p8 + 216);
    } else {
        slot4 = ((s32)p11 << 4);
        *(s32 *)(slot12) = 0;
        slot12 = (slot12 + 4);
        *(s32 *)((slot12 + 4)) = ((s32)(((104 - slot4) << 16) | (s32)((s32)(((s32)((s32)(*(s16 *)(p8 + 218) << 1) + *(s16 *)(p8 + 218)) << 1) - 8) & 255)) | 0x8000);
        slot12 = ((slot12 + 4) + 4);
        *(s32 *)(((slot12 + 4) + 4)) = (slot8 | 0xe400);
        slot12 = (((slot12 + 4) + 4) + 4);
            base6_0 = 0;
        v8 = (p8 + 12);
        Func_02007584(p8, 255);
        if ((u32)0 < p11) {
            p10 = ((slot8 + 2) | 0xe400);
            v5 = slot12;
            v8 = (p8 + 12);
            do {
                *(s32 *)(v5) = 0;
                *(s32 *)(v5 + 4) = ((s32)(((96 - (base6_0 << 4)) << 16) | (s32)((s32)(((s32)((s32)(*(s16 *)(p8 + 218) << 1) + *(s16 *)(p8 + 218)) << 1) - 8) & 255)) | 0x40000000);
                *(s32 *)(v5 + 8) = p10;
                slot12 = (slot12 + 12);
                base6_0 = (base6_0 + 1);
                v8 = (v8 + 12);
                v5 = (v5 + 12);
                Func_020075c6(v8, 255);
            } while ((u32)base6_0 < p11);
        }
            base6_0 = 0;
        *(s32 *)(slot12) = 0;
        slot12 = (slot12 + 4);
        v5 = ((slot8 + 6) | 0xe400);
        *(s32 *)((slot12 + 4)) = ((s32)(0x700000 | (s32)((s32)(((s32)((s32)(*(s16 *)(p8 + 218) << 1) + *(s16 *)(p8 + 218)) << 1) - 8) & 255)) | 0x8000);
        *(s32 *)(((slot12 + 4) + 4)) = ((slot8 + 6) | 0xe400);
        slot12 = (((slot12 + 4) + 4) + 4);
        Func_02007606(v8, 255);
        *(s32 *)(slot12) = 0;
        slot12 = (slot12 + 4);
        *(s32 *)((slot12 + 4)) = ((s32)((s32)(0x780000 | (s32)((s32)(((s32)((s32)(*(s16 *)(p8 + 218) << 1) + *(s16 *)(p8 + 218)) << 1) - 8) & 255)) | 0x8000) | 0x10000000);
        slot12 = ((slot12 + 4) + 4);
        v8 = ((v8 + 12) + 12);
        *(s32 *)(((slot12 + 4) + 4)) = ((slot8 + 6) | 0xe400);
        slot12 = (((slot12 + 4) + 4) + 4);
        v6 = 0;
        Func_02007636((v8 + 12), 255);
        if ((u32)0 < p11) {
            base10_800000 = 0x800000;
            v4 = ((slot8 + 2) | 0xe400);
            v5 = slot12;
            v8 = ((v8 + 12) + 12);
            do {
                *(s32 *)(v5) = 0;
                *(s32 *)(v5 + 4) = ((s32)((s32)((s32)((s32)(((s32)((s32)(*(s16 *)(p8 + 218) << 1) + *(s16 *)(p8 + 218)) << 1) - 8) & 255) | base10_800000) | 0x40000000) | 0x10000000);
                *(s32 *)(v5 + 8) = v4;
                slot0 = v4;
                slot12 = (slot12 + 12);
                v8 = (v8 + 12);
                Call2((void (*)())Func_0200767c, v8, 255);
                base6_0 = (base6_0 + 1);
                v5 = (v5 + 12);
                base10_800000 = (base10_800000 + 0x100000);
                v4 = slot0;
            } while ((u32)base6_0 < p11);
            v6 = base6_0;
        }
        *(s32 *)(slot12) = 0;
        none = 0;
        slot12 = (slot12 + 4);
        *(s32 *)((slot12 + 4)) = ((s32)((s32)((s32)((s32)(((s32)((s32)(*(s16 *)(p8 + 218) << 1) + *(s16 *)(p8 + 218)) << 1) - 8) & 255) | ((slot4 + 128) << 16)) | 0x8000) | 0x10000000);
        *(s32 *)(((slot12 + 4) + 4)) = (slot8 | 0xe400);
        slot12 = (((slot12 + 4) + 4) + 4);
        Func_020076d2(v8, 255);
        if ((u32)(*(s32 *)0x03001e40 & 15) <= 4) {
            v7 = ((s32)((s32)((s32)((s32)(((s32)((s32)(*(s16 *)(p8 + 218) << 1) + *(s16 *)(p8 + 218)) << 1) - 8) & 255) | ((slot4 + 128) << 16)) | 0x8000) | 0x10000000);
        } else {
            rec8 = Value1(Func_02007942, *(s16 *)(slot16 + 224));
            v7 = ((s32)((s32)((s32)((s32)(((s32)((s32)(*(s16 *)(p8 + 218) << 1) + *(s16 *)(p8 + 218)) << 1) - 8) & 255) | ((slot4 + 128) << 16)) | 0x8000) | 0x10000000);
            v8 = (v8 + 12);
            if (rec8 != 0) {
                rec6 = Func_0200767c_a((*(s32 *)(rec8 + 8) - *(s32 *)(slot16 + 232)), 0xe0000);
                slot0 = (rec6 + 112);
                record = Value2(Func_02007692, (*(s32 *)(rec8 + 16) - *(s32 *)(slot16 + 236)), 0xe0000);
                *(s32 *)(slot12) = none;
                slot12 = (slot12 + 4);
                v7 = (((((record + (((*(s16 *)(slot16 + 218) << 1) + *(s16 *)(slot16 + 218)) << 1)) - 4) & 255) | (slot0 << 16)) | 0x40000000);
                *(s32 *)((slot12 + 4)) = (((((record + (((*(s16 *)(slot16 + 218) << 1) + *(s16 *)(slot16 + 218)) << 1)) - 4) & 255) | (slot0 << 16)) | 0x40000000);
                slot12 = ((slot12 + 4) + 4);
                *(s32 *)(slot12) = ((slot8 + 12) | 0xe400);
                slot12 = (slot12 + 4);
                v8 = (v8 + 12);
                Func_0200776e((v8 + 12), 255);
                v5 = 0xe0000;
            }
            rec8 = Value1(Func_020079ca, *(s16 *)(slot16 + 222));
            if (rec8 != 0) {
                rec6 = Func_02007704((*(s32 *)(rec8 + 8) - *(s32 *)(slot16 + 232)), 0xe0000);
                slot0 = (rec6 + 112);
                record = Value2(Func_0200771a, (*(s32 *)(rec8 + 16) - *(s32 *)(slot16 + 236)), 0xe0000);
                *(s32 *)(slot12) = none;
                slot12 = (slot12 + 4);
                v7 = (((((record + (((*(s16 *)(slot16 + 218) << 1) + *(s16 *)(slot16 + 218)) << 1)) - 4) & 255) | (slot0 << 16)) | 0x40000000);
                *(s32 *)((slot12 + 4)) = (((((record + (((*(s16 *)(slot16 + 218) << 1) + *(s16 *)(slot16 + 218)) << 1)) - 4) & 255) | (slot0 << 16)) | 0x40000000);
                slot12 = ((slot12 + 4) + 4);
                *(s32 *)(slot12) = ((slot8 + 8) | 0xe400);
                Func_020077f0(v8, 255);
                v5 = 0xe0000;
            }
        }
    }
    p8b = (*(s32 *)0x03001e40 & 15);
    p9 = v5;
    p10b = p8 + 218;
    p11b = v7;
}
