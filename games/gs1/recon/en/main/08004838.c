#include "video_dma_family.h"

#define PaletteDma_LoadBlock Func_08004838

void PaletteDma_LoadBlock(void)
{
    const void *src = (const void *)0x0800779c;
    void *dst = (void *)0x05000200;
    u32 ctrl = 0x800000e0;
    struct DmaChannel *dma = (struct DmaChannel *)0x040000d4;

    dma->control = ctrl;
    dma->destination = dst;
    dma->source = src;
}
