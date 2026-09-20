#include "DMA.H"
void *Func_080048f4(s32, s32);
s32 Func_080041d8(void (*)(void), s32);
void Func_0801d94c(void);
void Menu_InitializeSelectedWorkspace(void)
{
    void *work;
    volatile u32 zero;
    work = Func_080048f4(20, 0x628);
    zero = 0;
    Dma_Set(&zero, work, 0x8500018a, (volatile u32 *)0x040000d4);
    Func_080041d8(Func_0801d94c, 3200);
}
