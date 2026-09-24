#include "DMA.H"

void *Runtime_AllocateBlock(s32 kind, s32 size);


void *DisplayTransition_AllocateAndClearState(void)
{
    volatile u32 clear_value;
    void *destination;

    destination = Runtime_AllocateBlock(31, 0x540);
    clear_value = 0;
    Dma_Set(&clear_value, destination, 0x85000150, (volatile u32 *)0x040000d4);
    return destination;
}
