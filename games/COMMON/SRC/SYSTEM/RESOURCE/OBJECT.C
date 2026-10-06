#include "DMA.H"
#include "ANIMSPR.H"
#include "IO_REG.H"

void ResourceMetadata_ClearRecord(void *destination)
{
    if (destination != 0) {
        volatile u32 clear_value;

        clear_value = 0;
        Dma_Set(&clear_value, destination, 0x85000000 | (sizeof(struct AnimationEntry) / 4), REG_DMA3);
    }
}
