#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "character_menu.h"
#include "heap_state.h"
#include "window.h"
#include "edition.h"
#include "mem.h"
#include "types.h"
#include "callback_scheduler.h"
#include "iwram_call.h"
#include "dma.h"
#include "system.h"
#include "ui.h"
#include "tbs_edition.h"
#include "io_reg.h"
#include "inventory_menu.h"
#include "menu_list.h"

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

#if EDITION_INTERNATIONAL
#define SAVED_TILE_BYTES 0x2000
#else
#define SAVED_TILE_BYTES 0x800
#endif

void FourObjectMotion_InitializeTopRow();

s32 ActionMenu_Open(void)
{
    struct CharacterMenuState *state = (struct CharacterMenuState *)Runtime_AllocateHeapBlock(55, 0x0a70);
    void *palette = Runtime_BumpAllocateAlternatePool(64);
    void *tiles = Runtime_BumpAllocateAlternatePool(SAVED_TILE_BYTES);
    s32 result;
    s32 index;

    ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    Scheduler_EnableOverlayCallbacksWithFlags();
    UiWindow_InitializeWork(0);
    state->page = 0;
    state->party_count = Party_ListActiveOwnersFar(state->owner_ids);
    Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
    Iwram_CopyWords(palette, (void *)0x05000000, 64);
#if !EDITION_INTERNATIONAL
    Iwram_CopyWords(tiles, (void *)0x06004000, SAVED_TILE_BYTES);
#endif
    Palette_LightenBankHighlight(14);
    Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, REG_DMA3);
    Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, REG_DMA3);
    Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, REG_DMA3);
    Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, REG_DMA3);
#if EDITION_INTERNATIONAL
    Iwram_CopyWords(tiles, (void *)0x06004000, SAVED_TILE_BYTES);
    Iwram_FillWords((void *)0x06004000, SAVED_TILE_BYTES, 0x33333333);
    UiWork_SetAltFlagAndClearTableFar(1);
#endif
    state->selector_window = (struct UiWindow *)UiWindow_CreateFar(13, 0, 17, 5, 2);
    for (index = 0; index < MENU_ROW_COUNT; index++)
        state->owner_y[index] = 30;
    if (Party_SumDjinnCountsFar(-1) != 0)
        FourObjectMotion_InitializeTopRow(state->selector_window, 0);
    for (index = 0; index < 4; index++) {
        state->row_x[index] = 130 + index * 32;
        state->row_y[index] = 0x80;
    }
    Link_DrawShiftedTilePairFar(0x06002500);
    Menu_CancelSoundReset();
    state->page = 0;

    result = Menu_RunActionFlow();

    Menu_EnsureCancelSound();
    RenderOutput_ClearListFar(state->status_window);
    FourObjectMotion_ClearSlotsAndSchedule();
    Scheduler_DisableOverlayCallbacksWithFlags();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
#if EDITION_INTERNATIONAL
    UiWindow_MarkVisibleTileAttributesFar();
    UiWork_SetAltFlagAndClearTableFar(0);
    WaitFrames(1);
#endif
    Iwram_CopyWords((void *)0x05000000, palette, 64);
    Iwram_CopyWords((void *)0x06004000, tiles, SAVED_TILE_BYTES);
    MemFree(tiles);
    MemFree(palette);
    ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menu_busy = 1;
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    Runtime_ReleaseHeapBlock(55);
    ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 0;
    WaitFrames(1);
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menu_busy = 0;
    return result;
}
