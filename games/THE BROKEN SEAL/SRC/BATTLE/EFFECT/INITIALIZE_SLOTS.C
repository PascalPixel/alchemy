#include "DMA.H"
void *Func_080048b0(s32, s32);
s32 Func_080041d8(void (*)(void), s32);
void Func_08095884(void);
void BattleFx_InitializeSlots(void)
{
    void *work;
    volatile u32 zero;
    work = Func_080048b0(56, 0x720);
    zero = 0;
    Dma_Set(&zero, work, 0x850001c8, (volatile u32 *)0x040000d4);
    Func_080041d8(Func_08095884, 3200);
}
