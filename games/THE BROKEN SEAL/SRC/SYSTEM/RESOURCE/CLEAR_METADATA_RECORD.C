#include "DMA.H"

void ResourceMetadata_ClearRecord(void *destination)
{
    if (destination != 0) {
        volatile u32 clear_value;

        clear_value = 0;
        Dma_Set(&clear_value, destination, 0x85000006, (volatile u32 *)0x040000d4);
    }
}
