#include "DMA.H"
void *Func_080048f4(s32, s32);
extern u32 *Data_03001f00;
void BattlePresentation_InitializeWorkAndResetState(void)
{
    void *work;
    u32 *state;
    volatile u32 zero;
    work = Func_080048f4(10, 0x2a0);
    state = Data_03001f00;
    zero = 0;
    Dma_Set(&zero, work, 0x850000a8, (volatile u32 *)0x040000d4);
    state[2] = 0;
}
