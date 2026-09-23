/* Draft, not exact: 2 differing halfwords, 60-byte candidate for the 60-byte
   owner (2026-09-23). Residual: the BG0CNT address load is scheduled before
   the 0x400 synthesis instead of between movs and lsls; storing 0x400 as a
   plain u16 constant pools it (ldrh), so the value is held in an int. */

#include "DMA.H"

/* Fills the BG0 map at 0x06002000 with blank tiles of palette 15 and points
   BG0 at it. */
void Bg0_ClearTilemap(void)
{
    volatile u32 fill;
    u32 control;

    fill = 0xf000f000;
    Dma_Set((const void *)&fill, (void *)0x06002000, 0x85000140, (volatile u32 *)0x040000d4);
    *(u32 *)0x03001cbc = 0x06002000;
    control = 4 << 8;
    *(volatile u16 *)0x04000008 = control;
}
