#include "types.h"

typedef struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
} DmaTransfer;

#define ResourceMetadata_ClearRecord Func_0800bc48

void ResourceMetadata_ClearRecord(void *destination)
{
    if (destination != 0) {
        u32 clear_value = 0;
        DmaTransfer *dma = (DmaTransfer *)0x040000d4;

        dma->source = &clear_value;
        dma->destination = destination;
        dma->control = 0x85000006;
    }
}
