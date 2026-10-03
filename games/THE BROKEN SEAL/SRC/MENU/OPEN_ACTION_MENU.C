#include "CHARACTER_MENU.H"
#include "HEAP_STATE.H"
#include "WINDOW.H"
#include "EDITION.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "UI.H"
#include "TBS_EDITION.H"

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: direct calls change saved-pointer allocation and argument scheduling during menu setup. */
    return copy(dst, src, size);
}

static __inline__ s32 FillWords(WordFillFn fill, void *dst, s32 size, u32 value)
{
    /* FAKEMATCH: a direct call loads the fill value before the routine, reversing their pool words. */
    return fill(dst, size, value);
}

extern struct ObjectSystemWork *gMenuCtrlWork;


/* The bytes of BG character block 1 the menu saves and restores: the
   Japanese menu keeps 0x800 and leaves the block as it was. */
#if EDITION_INTERNATIONAL
#define SAVED_TILE_BYTES 0x2000
#else
#define SAVED_TILE_BYTES 0x800
#endif

void UiWindow_DrawFrameFar(s32, s32, s32, s32);
void UiWindow_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void UiWindow_InitializeWork(s32);
s32 Party_ListActiveOwnersFar(u16 *);
void Palette_LightenBankHighlight(s32 index);
void Func_080153e0(s32);
void Func_080152a8(void);
s32 Party_SumDjinnCountsFar(s32);
/* The zero-argument definition ignores the extra words at this call boundary. */
void FourObjectMotion_InitializeTopRow();
void Link_DrawShiftedTilePairFar(s32 addr);
void Menu_CancelSoundReset(void);
s32 Menu_RunActionFlow(void);
void Menu_EnsureCancelSound(void);
void RenderOutput_ClearListFar(s32);
void FourObjectMotion_ClearSlotsAndSchedule(void);
void ItemMenu_Close(void);

/* Open the action menu: save palette bank 0-1 and BG character block 1, tint
   the highlight bank, blank the block, lay out the selector window, row anchors
   and the four djinn icon slots, run Menu_RunActionFlow, then restore the
   saved palette and tiles and close the screen. Returns the flow result. */
s32 ActionMenu_Open(void)
{
    /* FAKEMATCH: the existing field-address slice names heap slot 15 relative
       to slot 6. A whole-bank base changes register and pool reuse on exit. */
    struct CharacterMenuState *state = (struct CharacterMenuState *)Runtime_AllocateHeapBlock(55, 0x0a70);
    void *palette = Runtime_BumpAllocateAlternatePool(64);
    void *tiles = Runtime_BumpAllocateAlternatePool(SAVED_TILE_BYTES);
    s32 result;
    s32 index;

    gMenuCtrlWork->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    Scheduler_EnableOverlayCallbacksWithFlags();
    UiWindow_InitializeWork(0);
    state->page = 0;
    state->party_count = Party_ListActiveOwnersFar(state->owner_ids);
    Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
    CopyWords(Iwram_CopyWords, palette, (void *)0x05000000, 64);
#if !EDITION_INTERNATIONAL
    CopyWords(Iwram_CopyWords, tiles, (void *)0x06004000, SAVED_TILE_BYTES);
#endif
    Palette_LightenBankHighlight(14);
    Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, (volatile u32 *)0x040000d4);
#if EDITION_INTERNATIONAL
    CopyWords(Iwram_CopyWords, tiles, (void *)0x06004000, SAVED_TILE_BYTES);
    FillWords(Iwram_FillWords, (void *)0x06004000, SAVED_TILE_BYTES, 0x33333333);
    Func_080153e0(1);
#endif
    state->selector_window = (struct UiWindow *)UiWindow_CreateFar(13, 0, 17, 5, 2);
    for (index = 0; index < MENU_ROW_COUNT; index++)
        state->owner_y[index] = 30;
    if (Party_SumDjinnCountsFar(-1) != 0)
        FourObjectMotion_InitializeTopRow((s32)state->selector_window, 0);
    for (index = 0; index < 4; index++) {
        state->row_x[index] = 130 + index * 32;
        state->row_y[index] = 0x80;
    }
    Link_DrawShiftedTilePairFar(0x06002500);
    Menu_CancelSoundReset();
    state->page = 0;

    result = Menu_RunActionFlow();

    Menu_EnsureCancelSound();
    RenderOutput_ClearListFar((s32)state->status_window);
    FourObjectMotion_ClearSlotsAndSchedule();
    Scheduler_DisableOverlayCallbacksWithFlags();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
#if EDITION_INTERNATIONAL
    Func_080152a8();
    Func_080153e0(0);
    WaitFrames(1);
#endif
    CopyWords(Iwram_CopyWords, (void *)0x05000000, palette, 64);
    CopyWords(Iwram_CopyWords, (void *)0x06004000, tiles, SAVED_TILE_BYTES);
    Runtime_BumpFree(tiles);
    Runtime_BumpFree(palette);
    ((struct UiRenderWork *)((void **)&gMenuCtrlWork)[HEAP_SLOT_WINDOW - HEAP_SLOT_MENU_CONTROL])->menu_busy = 1;
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    Runtime_ReleaseHeapBlock(55);
    gMenuCtrlWork->suspended = 0;
    WaitFrames(1);
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    ((struct UiRenderWork *)((void **)&gMenuCtrlWork)[HEAP_SLOT_WINDOW - HEAP_SLOT_MENU_CONTROL])->menu_busy = 0;
    return result;
}
