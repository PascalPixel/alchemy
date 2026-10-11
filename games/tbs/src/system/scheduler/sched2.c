#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "inventory_menu.h"
#include "io_reg.h"

s32 Scheduler_DisableOverlayCallbacksWithFlags(void)
{
    Scheduler_DisableOverlayCallbacks();
    Map_DisableUpdateCallbackFar();
    GameFlag_ClearBitFar(0x166);
    return GameFlag_ClearBitFar(0x152);
}

s32 Scheduler_EnableOverlayCallbacksWithFlags(void)
{
    GameFlag_SetBitFar(0x152);
    GameFlag_SetBitFar(0x166);
    Map_EnableUpdateCallbackFar();
    return Scheduler_EnableUnmaskedOverlayCallbacks();
}

void UiWindow_InitializeWork(s32 unused)
{
    struct InventoryMenuState *work = gMenuWork;
    volatile u32 zero = 0;
    Dma_Set(&zero, work, 0x8500029c, REG_DMA3);
    work->pane_index[0] = -1;
    work->pane_count[0] = 1;
    work->pane_count[1] = 1;
    work->column_count = 1;
    work->row_count = 1;
}
