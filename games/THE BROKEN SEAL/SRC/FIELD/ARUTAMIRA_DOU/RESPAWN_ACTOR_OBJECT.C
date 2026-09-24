#include "DMA.H"
#include "TYPES.H"

#define ArutamiraDou_RespawnActorObject Func_02000c98

s32 Func_020048f6();
void Func_0200492c();
void Func_02004962();
void Func_02004a24();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void ArutamiraDou_RespawnActorObject(void)
{
    s32 rec6;
    s32 record;
    u8 *p6;
    u8 *p5;

    p6 = *(s32 *)0x03001f30;
    p5 = *(s32 *)((s32)p6 + 16);
    Call3(Func_02004a24, *(s16 *)((s32)p6 + 24), 0x4000, 0);
    Func_0200492c((s32)p5, 0);
    Func_02004962(20);
    rec6 = Value4(Func_020048f6, 0, *(s32 *)((s32)p5 + 8), *(s32 *)((s32)p5 + 12), *(s32 *)((s32)p5 + 16));
    if (rec6 != 0) {
        Dma_Set((const void *)((s32)p5), (void *)(rec6), -0x7bffffe4, (volatile u32 *)(0x40000d4));
        *(s32 *)((s32)p5 + 108) = 0;
        *(s32 *)((s32)p6 + 16) = rec6;
        p5[84] = 0;
    }
}
