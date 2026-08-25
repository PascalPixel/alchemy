#include "types.h"

typedef struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
} DmaTransfer;

void Func_08004838(void)
{
    volatile DmaTransfer *dma = (volatile DmaTransfer *)0x040000d4;

    dma->source = (const void *)0x0800779c;
    dma->destination = (void *)0x05000200;
    dma->control = 0x800000e0;
}
