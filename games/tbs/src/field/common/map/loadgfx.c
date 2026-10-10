#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "global_cells.h"
#include "dma.h"
#include "map.h"
#include "types.h"
#include "callback_scheduler.h"
#include "resource_ids.h"
#include "ram_buffer.h"
#include "system.h"
#include "map_scroll.h"
#include "mapframe.h"
#include "frame.h"
#include "io_reg.h"

extern u8 gMapCellBuffer[];
extern u8 gDecodeBuffer[];


s32 Resource_DecodeByteLz(const void *source, void *destination);

void MapAnimation_PresentFrame(void)
{
    s32 control = 0x682;

    /* FAKEMATCH: needed to match: the do-while keeps the BG1CNT store before the DMA source and destination loads. */
    do {
        REG_BG1CNT = control;
    } while (0);
    Dma_Set((const void *)gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00, DMA_ENABLE32 | DMA_32BIT | 0x2580, REG_DMA3);
}

void MapAnimation_Start(void)
{
    /* FAKEMATCH: needed to match: the adjacent slot-cell walk avoids the four bytes added by separate gMapWork loads. */
    u8 **pointers = &gMapAnimationPages;
    struct MapFrameWork *pages = (struct MapFrameWork *)*pointers++;
    struct PerspectiveWork *work = *(struct PerspectiveWork **)pointers;
    s32 one = 1;

    work->animation_active = one;
    Scheduler_EnableCallbacks(MapAnimation_Update);
    Dma_Set(BG_CHAR_BLOCK(1), (void *)gDecodeBuffer, DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
    WaitFrames(1);
    Resource_RunCopiedDecoder((s32)pages->pages[gFrameCount & one], (s32)gMapCellBuffer);
    work->window_top = 200;
    work->window_bottom = 255;
    gVBlankHook = MapAnimation_PresentFrame;
}

void Map_ShowBg1FromBuffer(void)
{
    u32 mode = 0x501;
    u16 *bg1cnt = (u16 *)&REG_BG1CNT;

    do { *bg1cnt = mode; } while (0); /* FAKEMATCH: needed to match: the wrap orders the store before the DMA operands. */
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
    buffer = gMapCellBuffer;
    resources = state->resources;
    value = *(s16 *)BG_PLTT;
    Resource_DecodeByteLz(Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = value;
    Dma_Set(buffer, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry(resources[1]), gBgTileBuffer);
    Resource_DecodeType01(Resource_GetTableEntry(resources[2]), Ram_BgTileBuffer + 0x2000);
    Resource_DecodeType01(Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer + 0x4000);
    Resource_DecodeType01(Resource_GetTableEntry(resources[4]), Ram_BgTileBuffer + 0x6000);
    gVBlankHook = Map_ShowBg1FromBuffer;
    state->window_top = 0;
    state->window_bottom = 159;
    WaitFrames(1);
    Resource_DecodeType01(Resource_GetTableEntry((u32)&ResourceId_DefaultMapCells), buffer);
    Map_UpdateCurrentTileBlock();
    state->animation_active = 0;
    Scheduler_DisableCallbacks(MapAnimation_Update);
    WaitFrames(1);
}

void Map_LoadDefaultCellsAndUpdateBlock(void)
{
    struct PerspectiveWork *work = gMapWork[0];
    gVBlankHook = Map_ShowBg1FromBuffer;
    work->window_top = 0;
    work->window_bottom = 0x9f;
    WaitFrames(1);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), gMapCellBuffer);
    Map_UpdateCurrentTileBlock();
    Scheduler_DisableCallbacks(MapAnimation_Update);
    WaitFrames(1);
}
