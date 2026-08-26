#ifndef GUARD_VIDEO_DMA_FAMILY_H
#define GUARD_VIDEO_DMA_FAMILY_H

#include "types.h"

struct DmaChannel {
    const void *source;
    void *destination;
    u32 control;
};

static __inline__ void StartDmaTransfer(
    const void *source,
    void *destination,
    u32 control)
{
    volatile struct DmaChannel *dma =
        (volatile struct DmaChannel *)0x040000d4;

    dma->source = source;
    dma->destination = destination;
    dma->control = control;
}

#endif
