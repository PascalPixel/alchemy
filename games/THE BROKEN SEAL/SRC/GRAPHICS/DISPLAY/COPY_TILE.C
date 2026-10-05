#include "DMA.H"
#include "IO_REG.H"
void Vram_CopyTile(u32 source_tile, u32 destination_tile)
{
    Dma_Set((u8 *)0x06000000 + (source_tile & 0x3ff) * 32,
            (u8 *)0x06000000 + (destination_tile & 0x3ff) * 32,
            0x84000008, REG_DMA3);
}
