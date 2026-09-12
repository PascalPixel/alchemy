#include "types.h"

#define FieldScene_RunScene3a5SequenceA Func_020018a4

extern u8 Data_00000000[];
extern u8 Data_00000002[];
extern u8 Data_0000001f[];
extern u8 Data_00000077[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][2];
void Func_02003606();
s32 Func_02003624();
s32 Func_0200363e();
s32 Func_02003660();
s32 Func_02003688();
s32 Func_020036aa();
s32 Func_02003724();
s32 Func_02003752();
void Func_0200375e();
s32 Func_02003766();
void Func_0200384a();
void Func_02003896();
void Func_020038a4();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene3a5SequenceA(void)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p11b;
    s32 p5;
    s32 p8;
    s32 p9;
    s32 rec3;
    s32 record;
    s32 r10;
    s32 r8;
    s32 base4_200a6be;
    s32 v3;
    s32 v4;
    u8 *v1;
    s32 v10;
    s32 v5;
    s32 v8;
    s32 v7;
    s32 base5_50003cc;
    s32 base3_2000240;
    s32 base5_200a6bc;
    u8 *v2;
    s32 base5_200a6e0;
    s32 base6_8;
    s32 base7_0;
    s32 v6;
    s32 slot8;
    s32 slot4;
    s32 slot0;

    slot8 = ((u32)*(u16 *)(((*(s16 *)(0x200a6d0) << 2) + 0x3001b10) + 2) >> 5);
    if (*(s16 *)(0x200b030) != 0) {
        *(u16 *)0x0200a6be = (s32)Data_00000002;
        v4 = 0x200a6be;
    } else {
        if (Value1(Func_02003624, 0x104) != 0) {
            base4_200a6be = 0x200a6be;
            if (*(s16 *)(0x200a6be) <= 0) {
                v4 = base4_200a6be;
                goto L_02001932;
            }
            *(u16 *)base4_200a6be = (*(u16 *)base4_200a6be - 1);
        } else {
            base4_200a6be = 0x200a6be;
            if (*(s16 *)(0x200a6be) <= 1) {
                v3 = (*(u16 *)base4_200a6be + 1);
                *(u16 *)base4_200a6be += 1;
                if ((v3 << 16) == 0x10000) {
                    *(s32 *)(0x40000d4) = 0x2009f80;
                    *(s32 *)(0x40000d4 + 4) = 0x50003c0;
                    *(s32 *)(0x40000d4 + 8) = -0x7ffffff0;
                }
            }
        }
        v4 = base4_200a6be;
    }
    L_02001932:;
    if (*(s16 *)(v4) == 0) {
        Func_02003606(*(s16 *)(0x200a6d0));
        v5 = 0;
        v6 = r10;
        v7 = r8;
    } else {
        if (*(s32 *)0x03001ecc != 0) {
            v1 = ((*(s32 *)0x03001ecc + (((((*(u8 *)((*(s32 *)0x03001ecc + 0x539)) << 2) + *(u8 *)((*(s32 *)0x03001ecc + 0x539))) << 5) + *(u8 *)((*(s32 *)0x03001ecc + 0x539))) << 2)) + 38);
            v4 = 0;
            do {
                v4 = (v4 + 1);
                *(u16 *)((s32)v1) = ((*(s16 *)(v4) << 19) >> 16);
                v1 = ((s32)v1 + 4);
            } while ((u32)v4 <= 143);
        }
        rec3 = Value2(Func_0200363e, 0x900, (s32)v1);
        *(s32 *)(0x40000d4) = 0x2009f80;
        *(s32 *)(0x40000d4 + 4) = rec3;
        *(s32 *)(0x40000d4 + 8) = -0x7ffffff0;
        slot4 = (rec3 + 12);
        p10 = rec3 + 12;
        p11 = (s32)Data_0000001f;
        v4 = 6;
        v10 = p10;
        do {
            p8 = ((u32)(0x1f0000 & (*(s16 *)(v10) << 16)) >> 16);
            v5 = (((u32)(*(s16 *)(v10) << 16) >> 21) & (s32)p11);
            slot0 = v4;
            record = Func_02003660(*(s16 *)(0x200a6bc), 3);
            v8 = (p8 + record);
            record = Func_02003688(*(s16 *)(0x200a6bc), 6);
            v7 = (((s32)((((u32)(*(s16 *)(v10) << 16) >> 26) & (s32)p11) - 20) - record) + 20);
            v4 = slot0;
            if (*(s16 *)(0x200a6bc) > 60) {
                v5 = (((u32)(*(s16 *)(v10) << 16) >> 21) & (s32)p11);
                if ((*(s32 *)0x03001e40 & 1) != 0) {
                    record = Func_020036aa((*(s16 *)(0x200a6bc) << 6), 120);
                    v4 = slot0;
                    v5 = (((s32)v5 + record) - 32);
                }
            }
            if ((u32)(p8 + record) > 31) {
                v8 = 31;
            }
            if ((u32)v5 > 31) {
                v5 = 31;
            }
            if ((u32)(((s32)((((u32)(*(s16 *)(v10) << 16) >> 26) & (s32)p11) - 20) - record) + 20) > 31) {
                v7 = 31;
            }
            p5 = v8;
            v4 = (v4 + 1);
            *(u16 *)(v10) = (((v7 << 10) | (v5 << 5)) | (s32)p5);
            v10 = (v10 + 2);
        } while ((u32)v4 <= 11);
        base5_50003cc = 0x50003cc;
        Value2(Func_02003752, base5_50003cc, *(s32 *)(slot4));
        Func_0200375e((base5_50003cc + 4), *(s32 *)(rec3 + 16));
        Value2(Func_02003766, ((base5_50003cc + 4) + 4), *(s32 *)((rec3 + 16) + 4));
        base3_2000240 = (s32)Data_02000240;
        record = Func_02003724((((*(s16 *)((base3_2000240 + 0x232)) << 4) - *(s16 *)((base3_2000240 + 0x232))) << 3), Data_02000240_t[139][0]);
        base5_200a6bc = 0x200a6bc;
        *(u16 *)base5_200a6bc = record;
        if ((record << 16) > 0x760000) {
            *(u16 *)0x0200a6c0 = (s32)Data_00000077;
        }
        if (*(s16 *)(0x200a6c0) != 0) {
            *(u16 *)base5_200a6bc = *(u16 *)0x0200a6c0;
            v3 = (*(u16 *)0x0200a6c0 - 8);
            *(u16 *)0x0200a6c0 = (*(u16 *)0x0200a6c0 - 8);
            if ((v3 << 16) <= 0) {
                *(u16 *)0x0200a6c0 = (s32)Data_00000000;
            }
        }
        *(s32 *)(0x40000d4) = 0x200a730;
        *(s32 *)(0x40000d4 + 4) = rec3;
        *(s32 *)(0x40000d4 + 8) = -0x7bfffdc0;
        if (*(s16 *)(0x200a6c0) <= 118) {
            v1 = (rec3 + 80);
            if ((u32)12 < (128 - *(s16 *)(0x200a6bc))) {
                v1 = (rec3 + 80);
                v4 = 12;
                do {
                    *(s32 *)((s32)v1 + 32) = -0x11111112;
                    *(s32 *)((s32)v1) = -0x11111112;
                    v1 = ((s32)v1 + 4);
                    if ((v4 & 7) == 7) {
                        v1 = ((s32)v1 + 32);
                    }
                    v4 = (v4 + 1);
                } while ((u32)v4 < (128 - *(s16 *)(0x200a6bc)));
            }
            *(s32 *)((s32)v1) = *(s32 *)(rec3);
            *(s32 *)((s32)v1 + 32) = *(s32 *)(rec3 + 32);
        }
        v1 = rec3;
        v2 = (0x480 + rec3);
        v4 = 0;
        do {
            v2 = ((s32)v2 + 1);
            if (v2[0] != 0) {
                v1[0] = v2[0];
            }
            v4 = (v4 + 1);
            v1 = ((s32)v1 + 1);
        } while ((u32)v4 <= 0x47f);
        Call3(Func_0200384a, *(s16 *)(0x200a6d0), 0x480, rec3);
        base5_200a6e0 = 0x200a6e0;
        base6_8 = 8;
        base7_0 = 0;
        v4 = 0;
        do {
            v8 = -0x7fff8000;
            if (v4 == 4) {
                v8 = 0x40000000;
            }
            *(s32 *)(base5_200a6e0) = 0;
            *(s32 *)(base5_200a6e0 + 4) = ((((((*(s16 *)(0x200a6be) << 3) - 16) & 0x1ff) << 16) | base6_8) | v8);
            *(s32 *)(base5_200a6e0 + 8) = (0xe400 | slot8);
            slot0 = v4;
            Func_020038a4((base7_0 + 0x200a6e0), 255);
            v4 = slot0;
            v4 = (v4 + 1);
            base5_200a6e0 = (base5_200a6e0 + 12);
            slot8 = (slot8 + 8);
            base7_0 = (base7_0 + 12);
            base6_8 = (base6_8 + 32);
        v6 = base6_8;
        } while ((u32)v4 <= 4);
        Func_02003896(rec3);
        v5 = base5_200a6e0;
        v7 = base7_0;
    }
    p9 = v5;
    p10b = v6;
    p11b = v7;
}
