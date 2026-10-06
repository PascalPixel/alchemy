#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "GLOBAL_CELLS.H"
#include "DMA.H"
#include "MAP.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE_IDS.H"
#include "RAM_BUFFER.H"
#include "SYSTEM.H"
#include "MAP_SCROLL.H"
#include "MAPFRAME.H"
#include "FRAME.H"
#include "IO_REG.H"

extern u8 gMapCellBuffer[];
extern u8 gDecodeBuffer[];


s32 Resource_DecodeByteLz(const void *source, void *destination);
#define BG_PALETTE ((s16 *)0x05000000)

void MapAnimation_PresentFrame(void)
{
    s32 control = 0x682;

    /* FAKEMATCH: the do-while keeps the BG1CNT store before the DMA source and destination loads. */
    do {
        REG_BG1CNT = control;
    } while (0);
    Dma_Set((const void *)gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00, DMA_ENABLE32 | DMA_32BIT | 0x2580, REG_DMA3);
}

void MapAnimation_Start(void)
{
    /* FAKEMATCH: the adjacent slot-cell walk avoids the four bytes added by separate gMapWork loads. */
    u8 **pointers = &gMapAnimationPages;
    struct MapFrameWork *pages = (struct MapFrameWork *)*pointers++;
    struct PerspectiveWork *work = *(struct PerspectiveWork **)pointers;
    s32 one = 1;

    work->animation_active = one;
    Scheduler_EnableCallbacks((u32)(MapAnimation_Update));
    Dma_Set(BG_CHAR_BLOCK(1), (void *)gDecodeBuffer, DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
    WaitFrames(1);
    Resource_RunCopiedDecoder((s32)pages->pages[*(u32 *)&gFrameCount & one], (s32)gMapCellBuffer);
    work->window_top = 200;
    work->window_bottom = 255;
    Data_03001cfc = MapAnimation_PresentFrame;
}

void Map_ShowBg1FromBuffer(void)
{
    u32 mode = 0x501;
    u16 *bg1cnt = (u16 *)&REG_BG1CNT;

    do { *bg1cnt = mode; } while (0); /* FAKEMATCH: the wrap orders the store before the DMA operands. */
    Dma_Set((const void *)gBgTileBuffer, BG_CHAR_BLOCK(2), 0x84002000,
            REG_DMA3);
}

void Map_LoadAreaGraphics(void)
{
    struct PerspectiveWork *state;
    u32 *resources;
    u8 *buffer;
    s16 value;

    state = gMapWork[0];
    buffer = (u8 *)gMapCellBuffer;
    resources = state->resources;
    value = BG_PALETTE[0];
    Resource_DecodeByteLz((const void *)Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = value;
    Dma_Set(buffer, BG_PALETTE, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[1]), (void *)gBgTileBuffer);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[2]), Ram_BgTileBuffer + 0x2000);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer + 0x4000);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[4]), Ram_BgTileBuffer + 0x6000);
    Data_03001cfc = Map_ShowBg1FromBuffer;
    state->window_top = 0;
    state->window_bottom = 159;
    WaitFrames(1);
    Resource_DecodeType01((const void *)Resource_GetTableEntry((u32)&ResourceId_DefaultMapCells), buffer);
    /* FAKEMATCH: the halfword local keeps zero in r5 and the short-range pool before the epilogue. */
    value = 0;
    Map_UpdateCurrentTileBlock();
    state->animation_active = value;
    Scheduler_DisableCallbacks((u32)(MapAnimation_Update));
    WaitFrames(1);
}

void Map_LoadDefaultCellsAndUpdateBlock(void)
{
    struct PerspectiveWork *work = gMapWork[0];
    Data_03001cfc = Map_ShowBg1FromBuffer;
    work->window_top = 0;
    work->window_bottom = 0x9f;
    WaitFrames(1U);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), gMapCellBuffer);
    Map_UpdateCurrentTileBlock();
    Scheduler_DisableCallbacks((u32)MapAnimation_Update);
    WaitFrames(1U);
}
