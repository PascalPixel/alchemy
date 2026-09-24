#include "DMA.H"

void BattleFx_ApplyColorToTargetBufferFar(s32, s32);
void BattleFx_StartBufferInterpolationFar(s32);


/* Copies the saved tile block at work + 0xe00 back into the scene's map and
   the work area, then refreshes the scene layer. */
void Shop_RestoreSceneTiles(s32 layer)
{
    u32 *scene = (u32 *)0x03001ebc;
    u8 *work = (u8 *)scene[5];
    u8 *map = (u8 *)scene[0];
    u8 *saved = work + 0xe00;

    Dma_Set(saved, map + 0x236, 0x84000150, (volatile u32 *)0x040000d4);
    Dma_Set(saved, work + 0x380, 0x840002a0, (volatile u32 *)0x040000d4);
    BattleFx_ApplyColorToTargetBufferFar(layer, 1);
    BattleFx_StartBufferInterpolationFar(16);
}
