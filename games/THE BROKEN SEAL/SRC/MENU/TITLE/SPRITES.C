/* The title sprites: decode their tiles and palette, then load the tiles
 * into the title's VRAM block. */
#include "TYPES.H"
#include "DMA.H"
#include "RESOURCE_IDS.H"
#include "TITLE.H"

void *Runtime_BumpAllocateAlternatePool(s32 size);
void Sys_Free(void *buffer);
s32 Resource_FindFreeEntry(void);
u8 *Resource_GetTableEntry(s32 resource);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 VramBlock_LoadCached(s32 block, s32 size, const void *data);

#define DMA3 ((volatile u32 *)0x040000d4)

/* FAKEMATCH: calling through the inline passes each constant straight into
 * its argument register instead of precomputing it. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Decode the title sprites' tiles and palette and load the tiles into the
 * title's VRAM block, finding one the first time. */
void Title_LoadSprites(s32 unused)
{
    u8 *buffer;
    volatile u32 *dma;

    buffer = Runtime_BumpAllocateAlternatePool(0x520);
    if (gTitleVramBlock == -1)
        gTitleVramBlock = Resource_FindFreeEntry();
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_IntroTilesB), buffer);
    Dma_Set(buffer, (void *)0x050003e0, 0x84000008, DMA3);
    Call3((void (*)())VramBlock_LoadCached, gTitleVramBlock, 0x500, (s32)(buffer + 32));
    dma = DMA3;
    while (dma[2] & 0x80000000)
        ;
    Sys_Free(buffer);
}
