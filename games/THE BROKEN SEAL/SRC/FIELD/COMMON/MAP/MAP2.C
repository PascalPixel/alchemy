#include "DMA.H"
#include "MAP.H"
#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "RAM_BUFFER.H"
#include "SYSTEM.H"

extern u8 gMapCellBuffer[];
extern u32 gFrameCount;
extern u8 Data_03001cfc[];
extern u8 gDecodeBuffer[];
void MapAnimation_Update(void);
void MapAnimation_PresentFrame(void);

struct MapAnimationWork {
    u8 unknown_000[0xfc];
    u8 active;
    u8 unknown_0fd[3];
    u16 timer;
    u16 limit;
};

s32 Scheduler_EnableCallbacks(void (*callback)(void));
void WaitFrames(s32 frames);
void Resource_RunCopiedDecoder(void *source, void *destination);

u32 Resource_GetTableEntry(u32 index);
s32 Resource_DecodeByteLz(const void *source, void *destination);
s32 Resource_DecodeType01(const void *source, void *destination);
void Map_UpdateCurrentTileBlock(void);
void Scheduler_DisableCallbacks(void *keep);
void Map_ShowBg1FromBuffer(void);
extern u8 *gCam;
#define BG_PALETTE ((s16 *)0x05000000)

struct MapWindow {
    u8 unk_000[0x100];
    u16 top;
    u16 bottom;
};

struct MapInitWork {
    u8 unknown_000[0x100];
    s16 first;
    s16 second;
};

/* Display hook installed by MapAnimation_Start: selects the animation
   layout for BG1 and copies the decoded frame into character VRAM. */
void MapAnimation_PresentFrame(void)
{
    s32 control = 0x682;

    /* FAKEMATCH: the do-while keeps the BG1CNT store ahead of the DMA
       source and destination loads. */
    do {
        *(volatile u16 *)0x0400000a = control;
    } while (0);
    Dma_Set((const void *)gMapCellBuffer, (void *)0x06006a00, 0x84002580, (volatile u32 *)0x040000d4);
}

/* Starts the map animation task, saves the second character block, and
   decodes this frame's animation page into the buffer. */
void MapAnimation_Start(void)
{
    /* FAKEMATCH: the map work pointer, gMapWork, is read as the word after
       the pages through one base; the reference loads one address for both. */
    u8 **pointers = &gMapAnimationPages;
    u8 *pages = *pointers++;
    struct MapAnimationWork *work = *(struct MapAnimationWork **)pointers;
    s32 one = 1;

    work->active = one;
    Scheduler_EnableCallbacks(MapAnimation_Update);
    Dma_Set((const void *)0x06004000, (void *)gDecodeBuffer, 0x84000800, (volatile u32 *)0x040000d4);
    WaitFrames(1);
    Resource_RunCopiedDecoder(pages + (*(u32 *)&gFrameCount & one) * 0x1400 + 0xc80, (void *)gMapCellBuffer);
    work->timer = 200;
    work->limit = 255;
    *(u32 *)Data_03001cfc = (u32)MapAnimation_PresentFrame;
}

/* Points BG1 at character block 2 and copies the 32 KB of buffered
   characters into it. */
void Map_ShowBg1FromBuffer(void)
{
    u32 mode = 0x501;
    u16 *bg1cnt = (u16 *)0x0400000a;

    do { *bg1cnt = mode; } while (0); /* FAKEMATCH: the wrap orders the store before the DMA operands. */
    Dma_Set((const void *)gBgTileBuffer, (void *)0x06008000, 0x84002000,
            (volatile u32 *)((u8 *)bg1cnt + 0xca));
}

/* Decodes the area's palette (keeping the backdrop colour) and its four tile
   banks, shows BG1 over the full window, decodes the default cells and
   leaves only the map animation running. */
void Map_LoadAreaGraphics(void)
{
    u8 *state;
    u32 *resources;
    u8 *buffer;
    s16 value;

    state = gCam;
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

void Map_LoadDefaultCellsAndUpdateBlock(void)
{
    struct MapInitWork *work = *(struct MapInitWork **)((u32)&gCam);
    *(s32 *)((u32)&Data_03001cfc) = (s32)Map_ShowBg1FromBuffer;
    work->first = 0;
    work->second = 0x9f;
    WaitFrames(1U);
    Resource_DecodeType01((s32)Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), (u32)gMapCellBuffer);
    Map_UpdateCurrentTileBlock();
    Scheduler_DisableCallbacks((u32)MapAnimation_Update);
    WaitFrames(1U);
}
