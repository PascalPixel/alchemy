#include "DMA.H"
#include "TYPES.H"

s32 Main_08000170();
s32 Main_080001d0();
s32 Main_08000290();
void Main_080001a8();
void Engine_VramLoad();
void Main_08000178();


extern u8 Data_0000001c[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Title_Func020001c0(void)
{
    s32 buf;
    s16 *slot;

    buf = Value1(Main_08000170, 0x520);
    slot = (s16 *)0x2008650;
    if (*slot == -1)
        *slot = Main_080001d0();
    Main_080001a8(Main_08000290((s32)Data_0000001c), buf);
    Dma_Set((const void *)buf, (void *)0x050003e0, 0x84000008, (volatile u32 *)0x040000d4);
    Call3(Engine_VramLoad, *slot, 0x500, buf + 32);
    {
        volatile u32 *dma = (volatile u32 *)0x040000d4;

        while (dma[2] & 0x80000000)
            ;
    }
    Main_08000178(buf);
}
