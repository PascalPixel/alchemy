#ifndef GUARD_VIDEO_DMA_FAMILY_H
#define GUARD_VIDEO_DMA_FAMILY_H

#include "DMA.H"

static __inline__ void StartDmaTransfer(
    const void *source,
    void *destination,
    u32 control)
{
    Dma_Set(source, destination, control, (volatile u32 *)0x040000d4);
}

#endif
