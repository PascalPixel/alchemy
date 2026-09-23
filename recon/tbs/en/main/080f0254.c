/* Draft, not exact (2026-09-24): 9 differing halfwords, 92 of 92 bytes.
   Residual: the reference sets r0 = &fill again before the second DMA;
   this candidate reuses r0. Block-scoped fills give two stack slots. */

#include "DMA.H"

void Func_080f0254(s32 alternate)
{
    u32 value;
    u32 vram;
    u32 palette;
    volatile u32 fill;

    if (alternate == 0) {
        value = 0x01010101;
        vram = 0x06000000;
        palette = 0x05000000;
    } else {
        value = 0x81818181;
        vram = 0x06008000;
        palette = 0x05000100;
    }
    fill = value;
    Dma_Set((const void *)&fill, (void *)vram, 0x85001e00, (volatile u32 *)0x040000d4);
    fill = 0;
    Dma_Set((const void *)&fill, (void *)palette, 0x85000040, (volatile u32 *)0x040000d4);
}
