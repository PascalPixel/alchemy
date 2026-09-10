#include "types.h"

#define FieldScene_RunScene370SequenceA Func_02000de4

s32 Func_020022c2();
s32 Func_020022da();
s32 Func_02002410();
void Func_0200242e();
s32 Func_020024a6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

s32 Func_02000de4(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    u8 *p11;
    s32 p4;
    s32 p4b;
    s32 p5;
    s32 p6;
    s32 rec2;
    s32 rec6;
    u8 *record;
    s32 r13;
    s32 none;
    u8 *v3;
    s32 v9;
    s32 a1;
    s32 base5_20096d0;
    s32 v7;
    s32 v2;
    s32 v1;
    s32 base5_0;
    u8 *v0;
    s32 v4;
    s32 v6;
    s32 base10_27;
    u8 *v5;
    s32 base7_0;
    s32 base8_d8;
    s32 v10;
    s32 base6_ffffffff;
    s32 base10_6b;
    s32 v12;
    s32 slot28;
    s32 slot24;
    u8 slot20[44];
    s32 slot16;
    u8 slot12[8];
    u8 slot8[4];
    s32 slot4;
    s32 slot0;
    u8 *p4c;

    (*(s32 *)((*(s32 *)slot20) + 8)) = a1;
    p11 = a2;
    (*(s32 *)((*(s32 *)slot20) + 4)) = 11;
    switch (a1) {
    case 0:
        (*(s32 *)((*(s32 *)slot20) + 4)) = 173;
        break;
    case 1:
        (*(s32 *)((*(s32 *)slot20) + 4)) = 39;
        break;
    }
    none = 0;
    if ((*(s32 *)((*(s32 *)slot20) + 4)) != 0) {
        v3 = (s32)p11;
        v9 = none;
        do {
            v3[0] = 0;
            a1 = 1;
            v9 = (v9 + 1);
            v3 = ((s32)v3 + 1);
        } while (v9 != (*(s32 *)((*(s32 *)slot20) + 4)));
    }
    (*(s32 *)slot20) = 0;
    (*(s32 *)((*(s32 *)slot12) + 4)) = 0;
    (*(s32 *)slot12) = 0;
    (*(s32 *)slot8) = 0;
    slot4 = (r13 + 32);
    none = 0;
    v9 = none;
    do {
        v9 = (v9 + 1);
        p4 = v9;
        *(s32 *)((r13 + 32)) = 0;
        v9 = p4;
    } while (p4 != 8);
    base5_20096d0 = 0x20096d0;
    none = 0;
    v9 = none;
    for (i = 0; (i >> 16) != 6; i += 0x10000) {
        base5_20096d0 = (base5_20096d0 + 2);
        if (Value4(Func_020022c2, *(u16 *)(base5_20096d0), 1, 0, i) != 0) {
            (*(s32 *)slot8) = ((u32)(((*(s32 *)slot8) | (1 << v9)) << 24) >> 24);
        }
        v9 = (v9 + 1);
    }
    none = 0;
    v7 = slot4;
    v9 = none;
    L_02000e94:;
    p6 = v9;
    record = Value1(Func_020022da, *(s32 *)(0x020096c0 + (s32)((s32)p6 << 2)));
    v2 = *(u16 *)((s32)record + 16);
    if (*(s16 *)((s32)record + 16) > 0x7cf) {
        *(u16 *)((s32)record + 16) = 0x7cf;
        v2 = 0x7cf;
    }
    if ((v2 << 16) < 0) {
        {
            s32 shown = 0;
        
            *(u16 *)((s32)record + 16) = shown;
        }
    }
    v2 = *(u16 *)((s32)record + 18);
    if (*(s16 *)((s32)record + 18) > 0x7cf) {
        *(u16 *)((s32)record + 18) = 0x7cf;
        v2 = 0x7cf;
    }
    if ((v2 << 16) < 0) {
        {
            s32 shown = 0;
        
            *(u16 *)((s32)record + 18) = shown;
        }
    }
    if ((u32)*(u16 *)((s32)record + 24) > 0x3e7) {
        *(u16 *)((s32)record + 24) = 0x3e7;
    }
    if ((u32)*(u16 *)((s32)record + 26) > 0x3e7) {
        *(u16 *)((s32)record + 26) = 0x3e7;
    }
    if ((u32)*(u16 *)((s32)record + 28) > 0x3e7) {
        *(u16 *)((s32)record + 28) = 0x3e7;
    }
    if ((u32)record[30] > 99) {
        record[30] = 99;
    }
    *(s32 *)(v7) = (((*(s16 *)((s32)record + 16) << 21) | (*(s16 *)((s32)record + 18) << 10)) | *(u16 *)((s32)record + 24));
    *(s32 *)(v7 + 4) = (((*(u16 *)((s32)record + 26) << 22) | (*(u16 *)((s32)record + 28) << 12)) | (record[30] << 4));
    v2 = record[15];
    if ((u32)record[15] > 99) {
        record[15] = 99;
        v2 = 99;
    }
    if (v2 == 0) {
        record[15] = 1;
    }
    (*(s32 *)slot20) = ((*(s32 *)slot20) | (s32)(record[15] << (s32)((s32)((s32)p6 << 3) - (s32)p6)));
    base5_0 = 0;
    v1 = 0;
    do {
        v3 = *(s32 *)(((s32)record + 248));
        base5_0 = (base5_0 + 1);
        (*(s32 *)((*(s32 *)slot12) + 4)) = ((*(s32 *)((*(s32 *)slot12) + 4)) + ((s32)v3 << v1));
        v1 = (v1 + 7);
    } while (base5_0 != 4);
    base5_0 = 0;
    v0 = ((s32)record + 216);
    do {
        v2 = 0x20096dc;
        for (i = 0; i != 8; i++) {
            v2 = (v2 + 2);
            if ((0x1ff & *(u16 *)((s32)v0)) == *(u16 *)(v2)) {
                (*(s32 *)slot12) = ((u32)(((*(s32 *)slot12) | (1 << i)) << 24) >> 24);
            }
            v4 = i;
        }
        base5_0 = (base5_0 + 1);
        v0 = ((s32)v0 + 2);
    } while (base5_0 != 15);
    v9 = (p6 + 1);
    v7 = (v7 + 8);
    if (v9 != 4) {
        goto L_02000e94;
    }
    if ((*(s32 *)((*(s32 *)slot20) + 8)) != 0) {
        v4 = p6;
        v5 = base5_0;
    } else {
        base10_27 = 39;
        none = 0;
        v9 = none;
        v6 = none;
        do {
            p4b = v9;
            rec6 = Func_02002410(*(s32 *)(0x020096c0 + (s32)((s32)p4b << 2)));
            p5 = base10_27;
            base7_0 = 0;
            base8_d8 = 216;
            v4 = rec6;
            v5 = (p5 + (s32)p11);
            v10 = p5;
            do {
                slot0 = v4;
                Func_0200242e(*(u16 *)(base8_d8 + v4));
                v4 = slot0;
                v6 = (v6 + 1);
                v5[0] += ((*(u16 *)(base8_d8 + v4) & 0x1ff) >> (v6 + 1));
                v5[1] += ((*(u16 *)(base8_d8 + v4) & 0x1ff) << (7 - v6));
                v5 = ((s32)v5 + 1);
                v10 = (v10 + 1);
                if ((v6 + 1) == 7) {
                    v5 = ((s32)v5 + 1);
                    v10 = (v10 + 1);
                    v6 = 0;
                }
                base7_0 = (base7_0 + 1);
                base8_d8 = (base8_d8 + 2);
            } while (base7_0 != 15);
            v7 = base7_0;
            v9 = (p4b + 1);
        } while ((p4b + 1) != 4);
        base6_ffffffff = -1;
        base10_6b = 107;
        none = 0;
        v9 = none;
        do {
            rec2 = Func_020024a6(*(s32 *)(0x020096c0 + (v9 << 2)));
            v0 = (base10_6b + (s32)p11);
            v6 = base6_ffffffff;
            v10 = base10_6b;
            v12 = 0x20096ec;
            do {
                p4c = *(u16 *)(v12);
                base7_0 = 0;
                v1 = (rec2 + 216);
                do {
                    v3 = (0x1ff & *(u16 *)(v1));
                    v1 = (v1 + 2);
                    v5 = 0;
                    if ((0x1ff & *(u16 *)(v1)) == (s32)p4c) {
                        v3 = (0xf800 & *(u16 *)(v1));
                        v5 = ((u32)(0xf800 & *(u16 *)(v1)) >> 11);
                    }
                    base7_0 = (base7_0 + 1);
                v7 = base7_0;
                } while (base7_0 != 15);
                if (v6 < 0) {
                    v0[0] += (((u32)((s32)v5 << 16) >> 16) >> -v6);
                    v10 = (v10 + 1);
                    v0 = ((s32)v0 + 1);
                    v6 = (v6 + 8);
                } else {
                }
                v6 = (v6 - 5);
                v0[0] += (((u32)((s32)v5 << 16) >> 16) << v6);
                if (v6 == -5) {
                    v0 = ((s32)v0 + 1);
                    v10 = (v10 + 1);
                    v6 = 3;
                }
                v4 = 1;
                v12 = (v12 + 2);
            } while (1 != 23);
            v9 = (v9 + v4);
        } while ((v9 + v4) != 4);
        p11[165] = *(u16 *)0x02000252;
        *(u8 *)(((s32)p11 + 165) + 1) = ((u32)*(s32 *)0x02000250 >> 8);
        *(u8 *)((((s32)p11 + 165) + 1) + 1) = *(s32 *)0x02000250;
    }
    if ((*(s32 *)((*(s32 *)slot20) + 8)) != 2) {
        none = 0;
        v9 = none;
        v0 = ((((u32)(-(*(s32 *)((*(s32 *)slot20) + 8)) | (*(s32 *)((*(s32 *)slot20) + 8))) >> 31) + 8) + (s32)p11);
        v4 = slot4;
        for (i = 0; (i >> 16) != 2; i += 0x10000) {
            v0[0] = ((u32)*(s32 *)(v4) >> 24);
            v0[1] = ((u32)*(s32 *)(v4) >> 16);
            v0[2] = ((u32)*(s32 *)(v4) >> 8);
            v0[3] = *(s32 *)(v4);
            v0[4] = ((u32)*(s32 *)(v4 + 4) >> 24);
            v0[5] = ((u32)*(s32 *)(v4 + 4) >> 16);
            v0[7] = *(s32 *)(v4 + 4);
            v0[6] = ((u32)*(s32 *)(v4 + 4) >> 8);
            v0[8] = ((u32)*(s32 *)(v4 + 8) >> 20);
            v0[9] = ((u32)*(s32 *)(v4 + 8) >> 12);
            v0[11] = (*(s32 *)(v4 + 8) << 4);
            v0[10] = ((u32)*(s32 *)(v4 + 8) >> 4);
            v0[7] = (*(s32 *)(v4 + 4) | ((u32)*(s32 *)(v4 + 8) >> 28));
            v0[11] = ((*(s32 *)(v4 + 8) << 4) | ((u32)*(s32 *)(v4 + 12) >> 28));
            v0[12] = ((u32)*(s32 *)(v4 + 12) >> 20);
            v9 = (v9 + 1);
            v0[13] = ((u32)*(s32 *)(v4 + 12) >> 12);
            v0[14] = ((u32)*(s32 *)(v4 + 12) >> 4);
            v4 = (v4 + 16);
            v0 = ((s32)v0 + 15);
        }
    }
    *p11 = (*(s32 *)slot20);
    p11[1] = ((u32)(*(s32 *)slot20) >> 8);
    p11[2] = ((u32)(*(s32 *)slot20) >> 16);
    p11[3] = ((((u32)(*(s32 *)slot20) >> 20) & 240) | ((*(s32 *)((*(s32 *)slot12) + 4)) & 15));
    p11[4] = ((u32)(*(s32 *)((*(s32 *)slot12) + 4)) >> 4);
    p11[5] = ((u32)(*(s32 *)((*(s32 *)slot12) + 4)) >> 12);
    p11[6] = ((u32)(*(s32 *)((*(s32 *)slot12) + 4)) >> 20);
    p11[7] = (*(s32 *)slot8);
    if ((*(s32 *)((*(s32 *)slot20) + 8)) != 0) {
        p11[8] = (*(s32 *)slot12);
    }
    return (*(s32 *)((*(s32 *)slot20) + 4));
    /* unlifted: 0x02000e1e..0x02000e22 (2) */
}
