#include "types.h"

#define Shop_DrawPartyItemPage Func_080a9598

/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 Data_00000182[];
extern u8 Data_00000af7[];
extern u8 Data_03001f2c[];
void Func_08015068();
void Func_08015080();
void Func_08015090();
void Func_080150b0();
s32 Func_08077008();
void Func_080a21b0();
void Func_080a2324();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

s32 Func_080a9598(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p8;
    s32 p9;
    s32 p9b;
    s32 rec5;
    s32 record;
    s32 r10;
    s32 v10;
    s32 v5;
    s32 base6_0;
    s32 v6;
    s32 base5_af7;

    p9 = *(s32 *)Data_03001f2c;
    rec5 = Func_08077008(*(u8 *)((0x21a + p9)));
    Func_08015068(a0, 128, 8, 224, 96);
    p8 = ((*(s32 *)(a2 + 8) << 2) + *(s32 *)(a2 + 8));
    p10 = ((u32)(s32)((s32)(*(s32 *)(a2 + 20) - (s32)p8) << 24) >> 24);
    v10 = p10;
    if ((u32)p10 > 5) {
        v10 = 5;
    }
    Func_080a2324(5, p8, a0, 119, 52);
    Func_080a21b0(a0, *(s32 *)(a2 + 20), 5, *(s32 *)(a2 + 8), 28);
    if (*(u8 *)((0x218 + p9)) == 0) {
        Call4(Func_08015080, 0xad7, a0, 120, 8);
        v6 = r10;
    } else {
        v6 = 0;
        base6_0 = 0;
        /* The entry guard compares the count variable, not a literal zero:
           folded to a literal it becomes `!= 0` and a `beq`, where the
           original keeps the unsigned `bls`. */
        if ((u32)v10 > base6_0) {
            v5 = ((s32)(((s32)p8 << 1) + p9) + 0x1c8);
            do {
                Func_08015080(((0x1ff & *(u16 *)(v5)) + (s32)Data_00000182), a0, 128, ((base6_0 << 4) + 8));
                base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
                v5 = (v5 + 2);
            } while ((u32)v10 > base6_0);
            v6 = base6_0;
        }
    }
    Func_08015090(rec5, a0, 40, 0);
    base5_af7 = (s32)Data_00000af7;
    Func_08015080(base5_af7, a0, 32, 16);
    Func_08015080((base5_af7 + 1), a0, 32, 24);
    Func_080150b0(*(u16 *)(rec5 + 60), 3, a0, 80, 16);
    Func_080150b0(*(u16 *)(rec5 + 62), 3, a0, 80, 24);
    p9b = base5_af7 + 1;
    p10b = v6;
    p11 = a0;
    return 1;
}
