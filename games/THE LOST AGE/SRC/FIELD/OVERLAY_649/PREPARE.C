#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"
#include "IO_REG.H"

extern char ResourceId_IntroTilesB;
extern s16 gTitleVramBlock;

s32 Scene_PrepareMap(void)
{
    return 0;
}

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
    dma = REG_DMA3;
    while (dma[2] < 0)
        ;
    Sys_Free(buffer);
}
