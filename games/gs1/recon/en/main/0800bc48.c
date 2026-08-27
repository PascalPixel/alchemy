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
        const void *source = &clear_value;
        u32 control = 0x85000006;
        DmaTransfer *dma = (DmaTransfer *)0x040000d4;

        dma->control = control;
        dma->destination = destination;
        dma->source = source;
    }
}
