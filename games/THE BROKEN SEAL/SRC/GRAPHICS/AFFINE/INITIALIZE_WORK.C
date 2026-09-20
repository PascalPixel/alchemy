#include "DMA.H"
void *Func_080048f4(s32, s32);
s32 Func_080041d8(void (*)(void), s32);
void Func_08028194(void);
void *AffineEffect_InitializeWork(void)
{
    void *work;
    volatile u32 zero;
    work = Func_080048f4(58, 152);
    zero = 0;
    Dma_Set(&zero, work, 0x85000026, (volatile u32 *)0x040000d4);
    Func_080041d8(Func_08028194, 0xc76);
    return work;
}
