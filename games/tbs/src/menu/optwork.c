#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "workspace_options.h"
#include "types.h"
#include "game_state.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "resource_ids.h"
#include "dma.h"
#include "io_reg.h"

#define ShopCursor_AdvanceFar ((void (*)(void*))ShopCursor_AdvanceFar)
#define ShopCursor_MoveTowardTargetFar ((void (*)(void*))ShopCursor_MoveTowardTargetFar)
#define Ui_ApplyTableScaleToObject ((void (*)(void*))Ui_ApplyTableScaleToObject)



void GraphicsPalette_LoadSelectionResourcesAndAdvance(void)
{
    s32 src0;
    s32 src1;
    s32 sel;
    struct WorkspaceWork *work;

    work = gSelectionWork;
    sel = work->page;
    ShopCursor_AdvanceFar(&work->cursor);
    ShopCursor_MoveTowardTargetFar(&work->marker[0]);
    ShopCursor_MoveTowardTargetFar(&work->marker[1]);

    if (sel == 0) {
        src0 = (work->cursor_frame & 7) +
            (s32)&ResourceId_ShopCursorFrames;
    } else {
        src0 = (s32)&ResourceId_ShopCursorFrames;
    }
    VramBlock_LoadCached(
        (u8)work->marker[0].output->index,
        0x100,
        Resource_GetTableEntry(src0));

    if (sel == 1) {
        src1 = (work->cursor_frame & 7) +
            (s32)&ResourceId_ShopCursorFrames;
    } else {
        src1 = (s32)&ResourceId_ShopCursorFrames;
    }
    VramBlock_LoadCached(
        (u8)work->marker[1].output->index,
        0x100,
        Resource_GetTableEntry(src1));

    if (sel > 1)
        Ui_ApplyTableScaleToObject(work->frame[sel - 2][work->option[sel]]);
    work->cursor_frame++;
}

void OptionMenu_InitializeWork(void)
{
    struct WorkspaceWork *work;
    volatile u32 zero;

    work = (struct WorkspaceWork *)Runtime_AllocateBlock(20, 0x628);
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x8500018a, REG_DMA3);
    work->option[0] = gGameState.palette_glow[0];
    work->option_count[0] = 24;
    work->option[1] = gGameState.palette_glow[1];
    work->option_count[1] = 15;
    work->option[2] = gGameState.message_speed;
    work->option_count[2] = 3;
    work->option[3] = gGameState.speech;
    work->option_count[3] = 2;
    work->option[4] = gGameState.saved_options;
    work->option_count[4] = 2;
    Scheduler_AddOrUpdateCallback(GraphicsPalette_LoadSelectionResourcesAndAdvance, 0xc80);
}

void Runtime_ScheduleCallbackAndReleaseBlock20B(void)
{
    Scheduler_RemoveCallback(GraphicsPalette_LoadSelectionResourcesAndAdvance);
    Runtime_ReleaseHeapBlock(0x14);
}
