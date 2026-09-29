#include "DMA.H"

extern u8 gDisp[];

/* Eight 4bpp tiles of a bar that narrows by a column from one to the next. */
extern const u8 BattlePres_TileVariants[];

void BattlePresentation_UploadTileVariant(void)
{
    u32 variant = **(u32 **)gDisp - 1;
    if (variant <= 31)
        Dma_Set(BattlePres_TileVariants + (variant >> 2) * 32, (void *)0x06005000, 0x84000008, (volatile u32 *)0x040000d4);
}
