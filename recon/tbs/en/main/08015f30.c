/* Draft, not exact: 11 differing halfwords, 136-byte candidate for the
   136-byte owner (2026-09-23). Residual:
   UiWork_InitializeWithResourceCounters. The fill word lives at sp but the
   reference addresses it through r5; the stores at 0xea3/0x12b6/0xea7 use
   r1/r2 against the reference's r3/r2. Written with a volatile fill (the
   readable form), GCC loads the halfword 99 from the pool with ldrh and
   splits the pool, 23 halfwords. */

#include "DMA.H"
#include "types.h"

#define Function Func_08015f30

s32 Func_080041d8();
s32 Func_080048f4();
void Func_08015ef4();
void Func_080173f4();
void Func_08019d0c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void Function(void)
{
    u32 i;
    s32 rec6;
    s32 record;
    s32 slot0;

    rec6 = Value2(Func_080048f4, 15, 0x12fc);
    *(s32 *)(&slot0) = 0;
    Dma_Set((const void *)(&slot0), (void *)(rec6), -0x7afffb41, (volatile u32 *)(0x40000d4));
    *(u8 *)((rec6 + 0xea3)) = 1;
    {
        s32 shown = 99;
    
        *(u16 *)((rec6 + 0x12b6)) = shown;
    }
    *(u8 *)((rec6 + 0xea7)) = 15;
    *(s32 *)(&slot0) = -0xfff1000;
    Dma_Set((const void *)(&slot0), (void *)(rec6), -0x7afffec0, (volatile u32 *)(0x40000d4));
    Func_08015ef4();
    Func_08019d0c();
    Value2(Func_080041d8, 0x80160fd, 0x480);
    Func_080173f4();
}
