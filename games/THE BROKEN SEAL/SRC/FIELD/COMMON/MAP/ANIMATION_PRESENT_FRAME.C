#include "DMA.H"

/* Display hook installed by MapAnimation_Start: selects the animation
   layout for BG1 and copies the decoded frame into character VRAM. */
void MapAnimation_PresentFrame(void)
{
    s32 control = 0x682;

    /* FAKEMATCH: the do-while keeps the BG1CNT store ahead of the DMA
       source and destination loads. */
    do {
        *(volatile u16 *)0x0400000a = control;
    } while (0);
    Dma_Set((const void *)0x02010000, (void *)0x06006a00, 0x84002580, (volatile u32 *)0x040000d4);
}
