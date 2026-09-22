#include "types.h"

#define Function Func_080f9f6c

void Func_080f9ee8();
void Func_080f9f3c();
void Func_080fa1ac();
s32 Func_080fa1fc();
void Func_080fa678();
void Func_080fac44();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void Function(s32 a0, s32 a1, u8 *a2, s32 a3)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p4b;
    u8 *p6;
    s32 p8;
    u8 *p9;
    s32 p9b;
    s32 p9c;
    s32 record;
    s32 r0;
    s32 v3;
    s32 v0;
    s32 v9;
    s32 none;
    s32 v2;
    u8 *v4;
    s32 v7;
    s32 v6;
    s32 v8;
    s32 v1;
    u8 *slot0;
    u8 *slot4;
    s32 slot20;
    s32 slot8;
    s32 slot16;
    s32 slot12;
    u8 *p4;
    u8 *p6b;
    u8 *p7;

    slot0 = a1;
    slot4 = *(s32 *)0x03007ff0;
    a2[4] = *(u8 *)((a0 + 0x80fba14));
    if ((u32)*(u8 *)(*(s32 *)((s32)a2 + 64)) < 128) {
        a2[5] = *(u8 *)(*(s32 *)((s32)a2 + 64));
        v3 = (*(s32 *)((s32)a2 + 64) + 1);
        if ((u32)*(u8 *)(*(s32 *)((s32)a2 + 64) + 1) < 128) {
            a2[6] = *(u8 *)(*(s32 *)((s32)a2 + 64) + 1);
            v3 = ((*(s32 *)((s32)a2 + 64) + 1) + 1);
            if ((u32)*(u8 *)((*(s32 *)((s32)a2 + 64) + 1) + 1) < 128) {
                a2[4] += *(u8 *)((*(s32 *)((s32)a2 + 64) + 1) + 1);
                v3 = (v3 + 1);
            }
        }
        *(s32 *)((s32)a2 + 64) = v3;
    }
    slot20 = 0;
    if ((192 & a2[36]) != 0) {
        if ((64 & a2[36]) != 0) {
            v0 = *(u8 *)((*(s32 *)((s32)a2 + 44) + v0));
        } else {
            v0 = a2[5];
        }
        p9 = ((((v0 << 1) + v0) << 2) + *(s32 *)((s32)a2 + 40));
        v9 = (s32)p9;
        if ((192 & *p9) != 0) {
            goto L_080fa152;
        }
        if ((128 & a2[36]) == 0) {
            v3 = a2[5];
            v9 = (s32)p9;
            goto L_080fa008;
        }
        if ((128 & p9[3]) != 0) {
            slot20 = ((s32)(p9[3] - 192) << 1);
        }
        v3 = p9[1];
    } else {
        p9b = (s32)a2 + 36;
        v3 = a2[5];
        v9 = p9b;
    }
    L_080fa008:;
    slot8 = v3;
    v0 = (a2[29] + slot0[9]);
    if ((u32)(a2[29] + slot0[9]) > 255) {
        v0 = 255;
    }
    slot16 = v0;
    p6 = v9;
    slot12 = (7 & *p6);
    if ((7 & *p6) != 0) {
        p4 = *(s32 *)((s32)slot4 + 28);
        if ((s32)p4 == 0) {
            goto L_080fa152;
        }
        if ((199 & *(u8 *)(((s32)p4 + ((slot12 - 1) << 6)))) == 0) {
            v4 = ((s32)p4 + ((slot12 - 1) << 6));
            goto L_080fa0ac;
        }
        if ((64 & *(u8 *)(((s32)p4 + ((slot12 - 1) << 6)))) != 0) {
            v4 = ((s32)p4 + ((slot12 - 1) << 6));
            goto L_080fa0ac;
        }
        if ((u32)*(u8 *)(((s32)p4 + ((slot12 - 1) << 6)) + 19) < slot16) {
            v4 = ((s32)p4 + ((slot12 - 1) << 6));
            goto L_080fa0ac;
        }
        if (*(u8 *)(((s32)p4 + ((slot12 - 1) << 6)) + 19) != slot16) {
            goto L_080fa152;
        }
        if ((u32)*(s32 *)(((s32)p4 + ((slot12 - 1) << 6)) + 44) >= (s32)a2) {
            v4 = ((s32)p4 + ((slot12 - 1) << 6));
            goto L_080fa0ac;
        }
    } else {
        none = 0;
        v2 = none;
        v3 = slot4[6];
        v4 = ((s32)slot4 + 80);
        for (i = (u32)v3; i != 0; --i) {
            if ((199 & v4[0]) == 0) {
                goto L_080fa0ac;
            }
            if ((64 & v4[0]) != 0) {
                if (v2 != 0) {
                    goto L_080fa084;
                }
                v2 = (v2 + 1);
                p6b = v4[19];
                v6 = (s32)p6b;
                p7 = *(s32 *)((s32)v4 + 44);
                v7 = (s32)p7;
            } else {
                if (v2 != 0) {
                    v6 = slot16;
                    v7 = (s32)a2;
                    v8 = none;
                    goto L_080fa0a0;
                }
                L_080fa084:;
                if ((u32)v4[19] < slot16) {
                    p7 = *(s32 *)((s32)v4 + 44);
                    v6 = v4[19];
                    v7 = (s32)p7;
                } else {
                    if ((u32)v4[19] > 0) {
                        v6 = slot16;
                        v7 = (s32)a2;
                        v8 = none;
                        goto L_080fa0a0;
                    }
                    if ((u32)*(s32 *)((s32)v4 + 44) > (s32)a2) {
                        v7 = *(s32 *)((s32)v4 + 44);
                    } else {
                        if ((u32)*(s32 *)((s32)v4 + 44) < 0) {
                            v6 = slot16;
                            v7 = (s32)a2;
                            v8 = none;
                            goto L_080fa0a0;
                        }
                        v7 = (s32)a2;
                    }
                    v6 = slot16;
                }
            }
            p8 = (s32)v4;
            v8 = p8;
            L_080fa0a0:;
            v4 = p8;
            v4 = ((s32)v4 + 64);
            a2 = v7;
        }
        p4b = v8;
        if (p4b != 0) {
            L_080fa0ac:;
            v4 = p4b;
            Func_080fa678((s32)v4, *(u8 *)(((s32)p4 + ((slot12 - 1) << 6))), a2[36], slot8);
            *(s32 *)((s32)v4 + 48) = 0;
            *(s32 *)((s32)v4 + 52) = *(s32 *)((s32)a2 + 32);
            if (*(s32 *)((s32)a2 + 32) != 0) {
                *(s32 *)(*(s32 *)((s32)a2 + 32) + 48) = (s32)v4;
            }
            *(s32 *)((s32)a2 + 32) = (s32)v4;
            *(s32 *)((s32)v4 + 44) = (s32)a2;
            a2[28] = a2[27];
            if (a2[27] != 0) {
                Func_080fa1ac(a2[27], (s32)a2);
            }
            Func_080fac44((s32)slot0, (s32)a2);
            *(s32 *)((s32)v4 + 16) = *(s32 *)((s32)a2 + 4);
            v4[19] = slot16;
            v4[8] = slot8;
            v4[20] = slot20;
            v4[1] = *p6;
            p7 = *(s32 *)((s32)p6 + 4);
            *(s32 *)((s32)v4 + 36) = (s32)p7;
            *(s32 *)((s32)v4 + 4) = *(s32 *)((s32)p6 + 8);
            *(u16 *)((s32)v4 + 12) = *(u16 *)((s32)a2 + 30);
            Func_080f9f3c();
            v3 = (v4[8] + *(s8 *)((s32)a2 + 8));
            if ((v4[8] + *(s8 *)((s32)a2 + 8)) < 0) {
                v3 = 0;
            }
            if (slot12 != 0) {
                v4[30] = p6[2];
                if ((128 & p6[3]) == 0) {
                    if ((112 & p6[3]) != 0) {
                        v1 = p6[3];
                        goto L_080fa128;
                    }
                }
                v1 = 8;
                L_080fa128:;
                v4[31] = v1;
                Func_080f9ee8(slot12, v3, a2[9], *(s32 *)((s32)slot4 + 48));
            } else {
                Value3(Func_080fa1fc, (s32)p7, v3, a2[9]);
            }
            *(s32 *)((s32)v4 + 32) = r0;
            v4[0] = 128;
            a2[0] &= 240;
        }
    }
    L_080fa152:;
    p9c = *p9;
    p10 = a2[36];
    p11 = a2[5];
}
