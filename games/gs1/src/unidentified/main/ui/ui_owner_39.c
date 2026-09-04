#include "types.h"

#define Func_080a15f0 Func_080a15f0

/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 Data_00000b20[];
void Func_08015080();
void Func_080150a8();
void Func_080ae99c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Func_080a15f0(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 p10;
    s32 p8;
    s32 p8b;
    s32 record;

    p8 = a0;
    Call4(Func_08015080, 0xb1c, a2, 0, 32);
    Func_080150a8(*(u16 *)(a1 + 60), 3, a2, 16, 40);
    if (*(u16 *)(p8 + 60) != *(u16 *)(a1 + 60)) {
        Func_080150a8(*(u16 *)(p8 + 60), 3, a2, 64, 40);
        if (*(u16 *)(p8 + 60) > *(u16 *)(a1 + 60)) {
            Func_080ae99c(a2, 44, 36, 0);
        } else {
            Func_080ae99c(a2, 44, 36, 1);
        }
    }
    Call4(Func_08015080, 0xb1d, a2, 0, 48);
    Func_080150a8(*(u16 *)(a1 + 62), 3, a2, 16, 56);
    if (*(u16 *)(p8 + 62) != *(u16 *)(a1 + 62)) {
        Func_080150a8(*(u16 *)(p8 + 62), 3, a2, 64, 56);
        if (*(u16 *)(p8 + 62) > *(u16 *)(a1 + 62)) {
            Func_080ae99c(a2, 44, 52, 0);
        } else {
            Func_080ae99c(a2, 44, 52, 1);
        }
    }
    Func_08015080((s32)Data_00000b20, a2, 0, 64);
    Func_080150a8(*(u16 *)(a1 + 64), 3, a2, 16, 72);
    if (*(u16 *)(p8 + 64) != *(u16 *)(a1 + 64)) {
        Func_080150a8(*(u16 *)(p8 + 64), 3, a2, 64, 72);
        if (*(u16 *)(p8 + 64) > *(u16 *)(a1 + 64)) {
            Func_080ae99c(a2, 44, 68, 0);
        } else {
            Func_080ae99c(a2, 44, 68, 1);
        }
    }
    p8b = *(u16 *)(a1 + 64);
    p10 = p8 + 64;
}
