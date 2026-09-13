#include "types.h"

/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 Data_00000b20[];
void Func_08015080();
void Func_080150a8();
void Func_080ae99c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call4(void (*f)(), s32 alt, s32 base, s32 work, s32 a3)
{
    f(alt, base, work, a3);
}

void UiText_DrawStatComparison(s32 alt, s32 base, s32 work)
{
    u32 i;
    s32 tmp2;
    s32 p;
    s32 tmp;
    s32 rec;

    p = alt;
    Call4(Func_08015080, 0xb1c, work, 0, 32);
    Func_080150a8(*(u16 *)(base + 60), 3, work, 16, 40);
    if (*(u16 *)(p + 60) != *(u16 *)(base + 60)) {
        Func_080150a8(*(u16 *)(p + 60), 3, work, 64, 40);
        if (*(u16 *)(p + 60) > *(u16 *)(base + 60)) {
            Func_080ae99c(work, 44, 36, 0);
        } else {
            Func_080ae99c(work, 44, 36, 1);
        }
    }
    Call4(Func_08015080, 0xb1d, work, 0, 48);
    Func_080150a8(*(u16 *)(base + 62), 3, work, 16, 56);
    if (*(u16 *)(p + 62) != *(u16 *)(base + 62)) {
        Func_080150a8(*(u16 *)(p + 62), 3, work, 64, 56);
        if (*(u16 *)(p + 62) > *(u16 *)(base + 62)) {
            Func_080ae99c(work, 44, 52, 0);
        } else {
            Func_080ae99c(work, 44, 52, 1);
        }
    }
    Func_08015080((s32)Data_00000b20, work, 0, 64);
    Func_080150a8(*(u16 *)(base + 64), 3, work, 16, 72);
    if (*(u16 *)(p + 64) != *(u16 *)(base + 64)) {
        Func_080150a8(*(u16 *)(p + 64), 3, work, 64, 72);
        if (*(u16 *)(p + 64) > *(u16 *)(base + 64)) {
            Func_080ae99c(work, 44, 68, 0);
        } else {
            Func_080ae99c(work, 44, 68, 1);
        }
    }
    tmp = *(u16 *)(base + 64);
    tmp2 = p + 64;
}
