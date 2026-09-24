#include "TYPES.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "UI.H"

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    return copy(dst, src, size);
}

static __inline__ s32 FillWords(WordFillFn fill, void *dst, s32 size, u32 value)
{
    return fill(dst, size, value);
}

struct MenuObjectControl {
    u8 padding00[4];
    u16 suspended;
};

extern struct MenuObjectControl *gMenuCtrlWork;

#define FIELD(ptr, type, offset) (*(type *)((u8 *)(ptr) + (offset)))

struct ActionMenuState {
    u8 padding000[0x24];
    s32 screen_handle;
    u8 padding028[0x0e4];
    s32 selector_window;
    u8 padding110[0x34];
    u16 row_positions[8];
    u8 padding154[0x0b4];
    u16 character_ids[8];
    u8 padding218;
    u8 character_count;
    u8 padding21a[6];
    u16 flags;
    u8 padding222[0x12];
    u16 icon_x[4];
    u16 icon_y[4];
};

struct ActionMenuState *Runtime_AllocateHeapBlock(s32, s32);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(void *block);
void UiWindow_DrawFrameFar(s32, s32, s32, s32);
void UiWindow_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
s32 Scheduler_EnableOverlayCallbacksWithFlags(void);
s32 Scheduler_DisableOverlayCallbacksWithFlags(void);
void UiWindow_InitializeWork(s32);
s32 Party_ListActiveOwnersFar(const u16 *);
void Menu_InitSelectorCursorAndEntries(s32, s32, s32, s32);
void Palette_LightenBankHighlight(s32 index);
void Func_080153e0(s32);
void Func_080152a8(void);
s32 Party_SumDjinnCountsFar(s32);
void FourObjectMotion_InitializeTopRow(s32, s32);
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
    struct ActionMenuState *state = Runtime_AllocateHeapBlock(55, 0x0a70);
    void *palette = Runtime_BumpAllocateAlternatePool(64);
    void *tiles = Runtime_BumpAllocateAlternatePool(0x2000);
    s32 result;
    s32 index;

    gMenuCtrlWork->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    Scheduler_EnableOverlayCallbacksWithFlags();
    UiWindow_InitializeWork(0);
    state->flags = 0;
    state->character_count = Party_ListActiveOwnersFar(state->character_ids);
    Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
    CopyWords((WordCopyFn)0x03001388, palette, (void *)0x05000000, 64);
    Palette_LightenBankHighlight(14);
    Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, (volatile u32 *)0x040000d4);
    CopyWords((WordCopyFn)0x03001388, tiles, (void *)0x06004000, 0x2000);
    FillWords((WordFillFn)0x03000168, (void *)0x06004000, 0x2000, 0x33333333);
    Func_080153e0(1);
    state->selector_window = UiWindow_CreateFar(13, 0, 17, 5, 2);
    for (index = 0; index < 8; index++)
        state->row_positions[index] = 30;
    if (Party_SumDjinnCountsFar(-1) != 0)
        FourObjectMotion_InitializeTopRow(state->selector_window, 0);
    for (index = 0; index < 4; index++) {
        state->icon_x[index] = 130 + index * 32;
        state->icon_y[index] = 0x80;
    }
    Link_DrawShiftedTilePairFar(0x06002500);
    Menu_CancelSoundReset();
    state->flags = 0;

    result = Menu_RunActionFlow();

    Menu_EnsureCancelSound();
    RenderOutput_ClearListFar(state->screen_handle);
    FourObjectMotion_ClearSlotsAndSchedule();
    Scheduler_DisableOverlayCallbacksWithFlags();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    Func_080152a8();
    Func_080153e0(0);
    WaitFrames(1);
    CopyWords((WordCopyFn)0x03001388, (void *)0x05000000, palette, 64);
    CopyWords((WordCopyFn)0x03001388, (void *)0x06004000, tiles, 0x2000);
    Runtime_BumpFree(tiles);
    Runtime_BumpFree(palette);
    FIELD(FIELD(&gMenuCtrlWork, void *, 0x24), u8, 0xea6) = 1;
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    Runtime_ReleaseHeapBlock(55);
    gMenuCtrlWork->suspended = 0;
    WaitFrames(1);
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    FIELD(FIELD(&gMenuCtrlWork, void *, 0x24), u8, 0xea6) = 0;
    return result;
}
