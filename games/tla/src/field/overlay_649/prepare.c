#include "types.h"
#include "field_scene.h"
#include "resource.h"
#include "dma.h"
#include "mem.h"
#include "io_reg.h"

extern char ResourceId_IntroTilesB;
extern s16 gTitleVramBlock;

s32 Scene_PrepareMap(void)
{
    return 0;
}

void Title_LoadSprites(s32 unused)
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
    MemFree(buffer);
}
