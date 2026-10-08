#include "types.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "scroll.h"
#include "io_reg.h"

void DisplayScroll_DisarmHBlankDma(void)
{
    volatile u16 *dma0;

    Scheduler_RemoveCallback(DisplayScroll_ArmHBlankDma);
    Scheduler_RemoveCallback(DisplayScroll_BuildAndSwapHBlankPage);
    dma0 = REG_DMA0;
    dma0[5] = 0xC5FF & dma0[5];
    dma0[5] = 0x7FFF & dma0[5];
    (void)dma0[5];
}
