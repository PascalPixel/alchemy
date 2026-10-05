#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RUNTIME_MEM.H"
#include "WORKSPACE_OPTIONS.H"
#include "TYPES.H"
#include "GAME_STATE.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "DMA.H"
#include "IO_REG.H"

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

    if (sel > 1) {
        /* FAKEMATCH: the scalar table address preserves r1/r2 choices changed by the shared frame array. */
        s32 idx = sel * 3;
        s32 adj = 0x594 + sel;
        s32 off;

        adj = ((s8 *)work)[adj];
        idx += adj;
        off = 0x5d4 + idx * 4;
        Ui_ApplyTableScaleToObject(*(void **)((u8 *)work + off));
    }
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
