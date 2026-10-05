#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"
#include "IO_REG.H"

extern char ResourceId_IntroTilesB;
extern s16 gTitleVramBlock;

/* The scene's last loader hook; this scene needs nothing. */
s32 Scene_PrepareMap(void)
{
    return 0;
}

/* Loads the title's sprite tiles: decodes them into a scratch buffer, takes
   its first 32 bytes as object palette 15 and caches the rest in the
   title's VRAM block. */
void Title_LoadSprites(void)
{
    u8 *buffer;
    volatile s32 *dma;

    buffer = Runtime_BumpAllocateAlternatePool(0x520);
    if (gTitleVramBlock == -1)
        gTitleVramBlock = Resource_FindFreeEntry();
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_IntroTilesB), buffer);
    Dma_Set(buffer, (void *)0x050003e0, 0x84000008, REG_DMA3);
    VramBlock_LoadCached(gTitleVramBlock, 0x500, buffer + 32);
    dma = (volatile s32 *)REG_DMA3;
    while (dma[2] < 0)
        ;
    Sys_Free(buffer);
}
