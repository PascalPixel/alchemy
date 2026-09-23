#include "types.h"

#define Function Func_080a4924

void Func_08015080();
void Func_08015120();
s32 Func_08077018();
void Func_080a4db4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Function(s32 a0, s32 a1)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p11b;
    s32 p8;
    s32 p8b;
    s32 p9;
    u8 *record;
    s32 r10;
    s32 none;
    s32 v2;
    s32 v9;
    s32 v1;
    s32 v5;
    s32 base6_b73;
    s32 base6_b71;
    s32 v6;
    s32 v0;
    s32 slot8;

    p8 = a0;
    p11 = a1;
    slot8 = 0;
    record = Value1(Func_08077018, (0x1ff & a1));
    v5 = slot8;
    if (record[2] != 0) {
        if ((*(s32 *)((s32)record + 8) & 0xffffff) == 0) {
            v2 = ((s32)record + 24);
            none = 0;
            v9 = none;
            for (i = 0; (i >> 16) != 3; i += 0x10000) {
                if (v9 > 3) {
                    v1 = none;
                    goto L_080a497a;
                }
                if (v9 != 3) {
                    break;
                }
                v2 = (v2 + 4);
                v9 = (v9 + 1);
            }
        }
        v1 = 1;
        L_080a497a:;
        v5 = slot8;
        if (v1 == 1) {
            Call4(Func_08015080, 0xb6d, p8, 16, 0);
            v5 = 1;
        }
        if (*(s16 *)((s32)record + 8) != 0) {
            Call4(Func_08015080, 0xaf7, p8, 0, (((v5 << 24) >> 24) << 3));
            Func_080a4db4(*(s16 *)((s32)record + 8), 3, p8, 64, (((v5 << 24) >> 24) << 3));
            v5 = ((u32)((((v5 << 24) >> 24) + 1) << 24) >> 24);
        }
        if (*(s8 *)((s32)record + 10) != 0) {
            Call4(Func_08015080, 0xaf8, p8, 0, (((v5 << 24) >> 24) << 3));
            Func_080a4db4(*(s8 *)((s32)record + 10), 3, p8, 64, (((v5 << 24) >> 24) << 3));
            v5 = ((u32)((((v5 << 24) >> 24) + 1) << 24) >> 24);
        }
    }
    none = 0;
    v9 = none;
    L_080a49fc:;
    if (*(u8 *)((s32)record + ((v9 << 2) + 24)) == 0) {
    } else {
        if ((u32)*(u8 *)(((s32)record + ((v9 << 2) + 24))) > 27) {
        } else {
            Func_08015080((*(u8 *)((s32)record + ((v9 << 2) + 24)) + 0xb3b), p8, 0, ((v5 << 24) >> 21));
            Func_080a4db4(*(s8 *)(((s32)record + ((v9 << 2) + 24)) + 1), 3, p8, 64, ((v5 << 24) >> 21));
            goto L_080a4b96;
            v5 = ((v5 << 24) >> 21);
        }
        L_080a4b96:;
        v5 = ((u32)((v5 << 24) + 0x1000000) >> 24);
    }
    v9 = (v9 + 1);
    if (v9 <= 3) {
        goto L_080a49fc;
    }
    if ((1 & record[3]) != 0) {
        Call4(Func_08015080, 0xb76, p8, 0, (((v5 << 24) >> 24) << 3));
        v5 = ((u32)((((v5 << 24) >> 24) + 1) << 24) >> 24);
    }
    if (record[12] == 3) {
        Call4(Func_08015080, 0xb65, p8, 0, (((v5 << 24) >> 24) << 3));
        slot8 = 1;
        v5 = ((u32)((((v5 << 24) >> 24) + 1) << 24) >> 24);
    }
    if (record[12] != 4) {
        if (record[12] != 0) {
            if (slot8 == 0) {
                Call4(Func_08015080, 0xb6e, p8, 16, (((v5 << 24) >> 24) << 3));
                v5 = ((u32)((((v5 << 24) >> 24) + 1) << 24) >> 24);
            }
            if (record[12] != 1) {
                if (record[12] <= 1) {
                    goto L_080a4cdc;
                }
                if (record[12] == 2) {
                    goto L_080a4c40;
                }
            } else {
                v5 = ((((v5 << 24) >> 24) + 1) << 24);
                Call4(Func_08015080, 0xb63, p8, 0, (((v5 << 24) >> 24) << 3));
                goto L_080a4cda;
                L_080a4c40:;
                if ((0x400 & (s32)p11) != 0) {
                    base6_b73 = 0xb73;
                    Func_08015080(base6_b73, p8, 0, (((v5 << 24) >> 24) << 3));
                    Func_08015080((base6_b73 + 1), p8, 0, ((((((v5 << 24) >> 24) + 1) << 24) >> 24) << 3));
                } else {
                    base6_b71 = 0xb71;
                    Func_08015080(base6_b71, p8, 0, (((v5 << 24) >> 24) << 3));
                    Func_08015080((base6_b71 + 1), p8, 0, ((((((v5 << 24) >> 24) + 1) << 24) >> 24) << 3));
                }
                L_080a4cda:;
                v5 = (((v5 >> 24) + 1) << 24);
                v5 = ((u32)v5 >> 24);
            }
        }
    }
    L_080a4cdc:;
    if ((16 & *(u8 *)(r10 + 3)) != 0) {
        v6 = (v5 << 24);
        if (((v5 << 24) >> 24) != 0) {
            v6 = (((v6 >> 24) + 1) << 24);
        }
        v9 = 0xb6f;
        Func_08015080(0xb6f, p8, 16, ((v6 >> 24) << 3));
        v0 = (0xf800 & (s32)p11);
        if ((0xf800 & (s32)p11) < 0) {
            v0 = ((s32)v0 + 0x7ff);
        }
        Func_08015120(((v0 >> 11) + 1), 5);
        Func_08015080((v9 + 1), p8, 0, (((((u32)(((v6 >> 24) + 1) << 24) >> 24) << 24) >> 24) << 3));
        v5 = ((u32)((((((u32)(((v6 >> 24) + 1) << 24) >> 24) << 24) >> 24) + 1) << 24) >> 24);
    }
    if (v5 == 0) {
        v1 = 0;
        if ((4 & *(u8 *)(r10 + 3)) != 0) {
            Call4(Func_08015080, 0xb69, p8, 0, 0);
            v1 = 1;
        }
        if (v1 == 0) {
            if ((8 & *(u8 *)(r10 + 3)) != 0) {
                Call4(Func_08015080, 0xb6a, p8, 0, 0);
                v1 = 1;
            }
            if (v1 == 0) {
                Call4(Func_08015080, 0xb6c, p8, 0, 0);
            }
        }
    }
    p8b = (8 & *(u8 *)(r10 + 3));
    p9 = v5;
    v9 = p9;
    p10 = v6;
    p11b = (v0 >> 11);
}
