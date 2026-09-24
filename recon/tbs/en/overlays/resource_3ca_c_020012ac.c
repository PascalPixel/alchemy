#include "DMA.H"



extern u8 Data_00000030[];
extern u8 Data_020091c5[];
extern s16 Data_02009c1a;
extern u16 Data_02009c18;
s32 Func_02002602();
s32 Func_0200261a(void);
void Func_02002632();
s32 Func_02002612(void *callback, s32 priority);

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Babi Fune: fill a 256-byte scratch block with colour 1, load it into a
 * newly claimed VRAM slot, set the fade step to 48 and schedule the fade. */
/* NONMATCHING: 104 of 104 bytes, 17 halfword edits (2026-09-24). Every
 * instruction matches; the reference pools the 0x30 step first, and the
 * HImode spelling that pools it first also dumps the pool mid-function. */
void Func_020012ac(void)
{
    u8 *buf;
    volatile u32 fill;

    buf = (u8 *)Value1(Func_02002602, 0x100);
    Data_02009c1a = Func_0200261a();
    fill = 0x11111111;
    Dma_Set((const void *)&fill, buf, 0x85000040, (volatile u32 *)0x040000d4);
    Call3(Func_02002632, Data_02009c1a, 0x100, (s32)buf);
    {
        /* FAKEMATCH: the step is a HImode pool constant, placed first. */
        s32 step = (u32)Data_00000030;

        Data_02009c18 = step;
    }
    Func_02002612(Data_020091c5, 0xc80);
}
