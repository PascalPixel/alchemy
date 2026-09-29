#include "TYPES.H"
#include "DMA.H"
#include "MAP.H"
#include "RESOURCE_IDS.H"
#include "RAM_BUFFER.H"
extern u8 gMapCellBuffer[];
extern u8 Data_03001cfc[];

u32 Resource_GetTableEntry(u32 index);
s32 Resource_DecodeByteLz(const void *source, void *destination);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 WaitFrames(s32 frames);
void Map_UpdateCurrentTileBlock(void);
void Scheduler_DisableCallbacks(void *keep);
void Map_ShowBg1FromBuffer(void);
void MapAnimation_Update(void);

extern u8 *Data_03001e70;

#define BG_PALETTE ((s16 *)0x05000000)

struct MapWindow {
    u8 unk_000[0x100];
    u16 top;
    u16 bottom;
};

/* Decodes the area's palette (keeping the backdrop colour) and its four tile
   banks, shows BG1 over the full window, decodes the default cells and
   leaves only the map animation running. */
void Map_LoadAreaGraphics(void)
{
    u8 *state;
    u32 *resources;
    u8 *buffer;
    s16 value;

    state = Data_03001e70;
    buffer = (u8 *)gMapCellBuffer;
    resources = *(u32 **)(state + 0x11c);
    value = BG_PALETTE[0];
    Resource_DecodeByteLz((const void *)Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = value;
    Dma_Set(buffer, BG_PALETTE, 0x84000070, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[1]), (void *)gBgTileBuffer);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[2]), Ram_BgTileBuffer + 0x2000);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer + 0x4000);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[4]), Ram_BgTileBuffer + 0x6000);
    *(s32 *)((u32)&Data_03001cfc) = (s32)Map_ShowBg1FromBuffer;
    ((struct MapWindow *)state)->top = 0;
    ((struct MapWindow *)state)->bottom = 159;
    WaitFrames(1);
    Resource_DecodeType01((const void *)Resource_GetTableEntry((u32)&ResourceId_DefaultMapCells), buffer);
    /* FAKEMATCH: the byte flag is written from the halfword local cleared
       before the call; the reference keeps that zero in r5 and loads it
       from a short-range halfword pool placed before the epilogue. */
    value = 0;
    Map_UpdateCurrentTileBlock();
    state[252] = value;
    Scheduler_DisableCallbacks(MapAnimation_Update);
    WaitFrames(1);
}
