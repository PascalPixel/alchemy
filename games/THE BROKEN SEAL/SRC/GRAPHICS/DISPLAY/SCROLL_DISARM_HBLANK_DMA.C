#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "SCROLL.H"

void DisplayScroll_DisarmHBlankDma(void)
{
    volatile u16 *dma0;

    Scheduler_RemoveCallback((u32)DisplayScroll_ArmHBlankDma);
    Scheduler_RemoveCallback((u32)DisplayScroll_BuildAndSwapHBlankPage);
    dma0 = (volatile u16 *)0x040000B0;
    dma0[5] = (u16)(0xC5FF & dma0[5]);
    dma0[5] = (u16)(0x7FFF & dma0[5]);
    (void)dma0[5];
}
