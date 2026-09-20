#include "DMA.H"
void DisplayTransition_FillTilemapAndSolidTile(s32 color)
{
    u8 *work = *(u8 **)0x03001ecc;
    volatile u32 fill = 0xf000f000;
    Dma_Set(&fill, (void *)0x06002000, 0x85000140, (volatile u32 *)0x040000d4);
    if (color != -1) {
        u32 pattern = 0;
        s32 cnt;
        u32 *tile;
        for (cnt = 7; cnt >= 0; --cnt) pattern = (pattern << 4) | color;
        tile = (u32 *)(work + 1288);
        for (cnt = 7; cnt >= 0; --cnt) *tile++ = pattern;
        Dma_Set(work + 1288, (void *)0x06000000, 0x84000008, (volatile u32 *)0x040000d4);
    }
}
