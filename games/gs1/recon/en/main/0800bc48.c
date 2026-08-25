#include "types.h"

typedef struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
} DmaTransfer;

void Func_0800bc48(void *destination)
{
    if (destination != 0) {
        u32 clear_value = 0;
        DmaTransfer *dma = (DmaTransfer *)0x040000d4;

        dma->source = &clear_value;
        dma->destination = destination;
        dma->control = 0x85000006;
    }
}
