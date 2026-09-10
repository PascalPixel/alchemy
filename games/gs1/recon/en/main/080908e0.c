#include "types.h"

#define Function Func_080908e0

extern u8 Data_0000001f[];
extern u8 Data_000003e0[];
extern u8 Data_00002a01[];
extern u8 Data_02002090[];
extern u8 Data_04000208[];
void Func_080072f0();
s32 Func_080770c0();

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

void Function(void)
{
    s32 v2;
    s32 v0;
    s32 v1;
    s32 v5;
    s32 v4;
    s32 base4_4000208;
    s32 base5_2002090;
    s32 v3;
    s32 v6;
    s32 idx;
    s32 f;
    s32 base;
    s32 dest1;
    s32 *q;
    u8 *p6;

    p6 = *(s32 *)0x03001ed0;
    v5 = ((s32)p6 + 0x1880);
    if (Func_080770c0(0x152) != 0) {
    } else {
        if (*(s8 *)((s32)p6 + (s32)Data_00002a01) == 0) {
        } else {
            v2 = (*(u8 *)((s32)p6 + (s32)Data_00002a01 + 1) + 1);
            *(u8 *)((s32)p6 + (s32)Data_00002a01 + 1) += 1;
            if (((v2 << 24) >> 24) < *(s8 *)((s32)p6 + (s32)Data_00002a01)) {
                v0 = 0;
                v1 = ((s32)p6 + 0x380);
                do {
                    v0 = (v0 + 1);
                    *(u16 *)(v1) += *(u16 *)(v5);
                    v5 = (v5 + 2);
                    v1 = (v1 + 2);
                } while (v0 <= 0x53f);
            } else {
                Call4(Func_080072f0, ((s32)p6 + 0x380), ((s32)p6 + 0xe00), 0xa80, 0x3001388);
                *(u8 *)((s32)p6 + (s32)Data_00002a01) = 0;
            }
            v0 = 0x1c0;
            v1 = ((s32)p6 + 0x380);
            v4 = ((s32)((s32)p6 + (s32)((s32)((s32)((s32)(1 ^ *(u8 *)(((s32)p6 + 0x2a00))) << 3) - (s32)(1 ^ *(u8 *)(((s32)p6 + 0x2a00)))) << 7)) + 0x2300);
            do {
                v0 = (v0 - 1);
                *(u16 *)(v4) = (((0x7c00 & *(u16 *)(v1)) | (((*(u16 *)(v1 + 2) << 16) >> 21) & (s32)Data_000003e0)) | (((*(u16 *)(v1 + 4) << 16) >> 26) & (s32)Data_0000001f));
                v1 = (v1 + 6);
                v4 = (v4 + 2);
            } while (v0 != 0);
            *(u8 *)(((s32)p6 + 0x2a00)) ^= 1;
            f = *(u8 *)(((s32)p6 + 0x2a00));
            base = (s32)p6 + (((f << 3) - f) << 7);
            base4_4000208 = (s32)Data_04000208;
            base5_2002090 = (s32)Data_02002090;
            dest1 = base + 0x2300;
            v1 = *(u16 *)base4_4000208;
            *(u16 *)base4_4000208 = base4_4000208;
            idx = *(u16 *)base5_2002090;
            if (idx <= 31) {
                v3 = ((((idx << 1) + idx) << 2) + base5_2002090) + 4;
                *(u16 *)base5_2002090 = idx + 1;
                q = (s32 *)v3;
                *q++ = dest1;
                *q++ = 0x5000000;
                *q = -0x7bffff90;
            }
            *(u16 *)base4_4000208 = v1;
            v6 = *(u16 *)base4_4000208;
            *(u16 *)base4_4000208 = base4_4000208;
            idx = *(u16 *)base5_2002090;
            if (idx <= 31) {
                v3 = ((((idx << 1) + idx) << 2) + base5_2002090) + 4;
                *(u16 *)base5_2002090 = idx + 1;
                q = (s32 *)v3;
                *q++ = base + 0x24c0;
                *q++ = 0x5000200;
                *q = -0x7bffff90;
            }
            *(u16 *)base4_4000208 = v6;
        }
    }
}
