#include "ITEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "INVENTORY_MENU.H"
#include "WINDOW.H"
#include "HEAP_STATE.H"
#include "ANIMSPR.H"

/* Where the accepted selection is published. */
struct AssetSelectionResult {
    s32 unknown_000[0x60];
    u16 unknown_180;
    u8 unknown_182[0x18];
    u16 unknown_19a;
};

#if EDITION_INTERNATIONAL
typedef s32 (*CopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*FillFn)(void *dst, s32 size, u32 value);

static __inline__ s32 CopyWords(CopyFn copy, void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: the wrapper keeps the copier's address in a register for
     * both copies and the tile address in the pool; called directly the
     * tile address takes the register instead. */
    return copy(dst, src, size);
}

static __inline__ s32 FillWords(FillFn fill, void *dst, s32 size, u32 value)
{
    /* FAKEMATCH: as CopyWords; the direct call loads the tile address once. */
    return fill(dst, size, value);
}
#endif

void *Runtime_BumpAllocateAlternatePool(s32 size);
s32 Runtime_AllocateHeapBlock(s32 id, s32 size);
void UiWindow_DrawFrameFar(s32, s32, s32, s32);
void WaitFrames(s32 frames);
void UiWindow_InitializeWork(s32);
s32 Party_ListActiveOwnersFar(void *session);
void Resource_LoadPairedBlocks(void);
void Palette_LightenBankHighlight(s32);
void Link_DrawShiftedTilePairFar(void *address);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
s32 Scheduler_EnableOverlayCallbacksWithFlags(void);
void UiWork_SetAltFlagAndClearTableFar(s32);
void Menu_CancelSoundReset(void);
void Menu_EnsureCancelSound(void);
void RenderOutput_ClearListFar(s32);
void ItemMenu_Close(void);
void Menu_ResetTwoResourceEntries(void);
void Runtime_ReleaseHeapBlock(s32);
void UiWindow_MarkVisibleTileAttributesFar(void);
void Runtime_BumpFree(void *);
s32 Scheduler_DisableOverlayCallbacksWithFlags(void);
void UiWindow_EraseBorderRectFar(s32, s32, s32, s32);
void Event_ClearInvalidPackedValuesFar(void);

/* Runs the modal item selection screen over the saved tile memory and
 * publishes the accepted selection. */
s32 RunAssetSelectionScreen(void)
{
#if EDITION_INTERNATIONAL
    void *backup;
#endif
    struct InventoryMenuState *screen;
    s32 index;
    s32 value;
    s32 category;
    s32 result;
#if EDITION_INTERNATIONAL
    s32 size;
    CopyFn copy;
#endif
    void **process;
    void **cache;

#if EDITION_INTERNATIONAL
    size = 0x2000;
    backup = Runtime_BumpAllocateAlternatePool(size);
#endif
    screen = (struct InventoryMenuState *)Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0xa70);
    cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_MENU_CONTROL];
    ((struct ObjectSystemWork *)cache[0])->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);
    screen->party_count = Party_ListActiveOwnersFar(screen->owner_ids);
    ItemMenu_Init(0, 3, 0, 7);
    Resource_LoadPairedBlocks();
    Palette_LightenBankHighlight(14);
    Link_DrawShiftedTilePairFar((void *)0x06002500);
    screen->message_window = (struct UiWindow *)UiWindow_CreateFar(13, 0, 17, 3, 2);
#if EDITION_INTERNATIONAL
    Scheduler_EnableOverlayCallbacksWithFlags();
    copy = Iwram_CopyWords;
    CopyWords(copy, backup, (void *)0x06004000, size);
    FillWords(Iwram_FillWords, (void *)0x06004000, size, 0x33333333);
    UiWork_SetAltFlagAndClearTableFar(1);
#endif
    Menu_CancelSoundReset();
    result = ItemMenu_RunCommands(&category, &value, &index);
    Menu_EnsureCancelSound();
    if (result == 1) {
        struct AssetSelectionResult *selection = cache[HEAP_SLOT_EVENT - HEAP_SLOT_MENU_CONTROL];

        selection->unknown_180 = (category << 10) | (index & ITEM_ID_MASK);
        selection->unknown_19a = screen->selected_slots[0];
    }
    RenderOutput_ClearListFar((s32)screen->status_window);
    process = &cache[HEAP_SLOT_WINDOW - HEAP_SLOT_MENU_CONTROL];
    ((struct UiRenderWork *)*process)->menu_busy = 1;
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    Menu_ResetTwoResourceEntries();
    Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
    ((struct ObjectSystemWork *)cache[0])->suspended = 0;
#if EDITION_INTERNATIONAL
    UiWindow_MarkVisibleTileAttributesFar();
    UiWork_SetAltFlagAndClearTableFar(0);
    CopyWords(copy, (void *)0x06004000, backup, size);
    ((struct UiRenderWork *)*process)->menu_busy = 0;
    Runtime_BumpFree(backup);
    WaitFrames(1);
    Scheduler_DisableOverlayCallbacksWithFlags();
#endif
    WaitFrames(1);
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    ((struct UiRenderWork *)*process)->menu_busy = 0;
    Event_ClearInvalidPackedValuesFar();
    return result;
}
