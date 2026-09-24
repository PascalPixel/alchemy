#include "TYPES.H"
#include "DMA.H"

extern u16 *gDebugTextCursor;

/* Fill the BG0 screen block at 0x06002000 with blank tiles and home the cursor. */
void Bg0_ClearTilemap(void)
{
    volatile u32 fill;

    fill = 0xf000f000;
    Dma_Set((const void *)&fill, (void *)0x06002000, 0x85000140, (volatile u32 *)0x040000d4);
    /* FAKEMATCH: the cursor store sits in a do-while(0) to hold the BG0CNT constant load order */
    do {
        gDebugTextCursor = (u16 *)0x06002000;
    } while (0);
    {
        s32 control = 0x400;

        *(volatile u16 *)0x04000008 = control;
    }
}
