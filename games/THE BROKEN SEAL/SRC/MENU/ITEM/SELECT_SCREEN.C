#include "EDITION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"

struct AssetSelectionDisplay {
    u16 unknown_00;
    u16 unknown_02;
    s16 busy;
};

struct AssetSelectionGlobals {
    struct AssetSelectionDisplay *display_state;
    u8 reserved_004[0x20];
    u8 *process_state;
    u8 reserved_028[0x2c];
    struct AssetSelectionResult *selection_state;
};

/* Where the accepted selection is published. */
struct AssetSelectionResult {
    s32 unknown_000[0x60];
    u16 packed;                     /* 0x180, category above bit 10, index below */
    u8 unknown_182[0x18];
    u16 style;                      /* 0x19a */
};

struct AssetSelectionScreen {
    u8 reserved_000[0x24];
    s32 resource_handle;
    u8 reserved_028[0xe4];
    s32 window;
    u8 reserved_110[0x64];
    u16 selection_style;
    u8 reserved_176[0x92];
    u8 session[0x11];
    u8 session_mode;
};

extern struct AssetSelectionGlobals gMenuCtrlWork;

/* The byte of the process state that holds the screen while it closes. The
   localised editions also save the tile memory the screen draws over. */
#if EDITION_INTERNATIONAL
#define PROCESS_CLOSING 0xea6
#else
#define PROCESS_CLOSING 0xf36
#endif

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
struct AssetSelectionScreen *Runtime_AllocateHeapBlock(s32 id, s32 size);
void UiWindow_DrawFrameFar(s32, s32, s32, s32);
void WaitFrames(s32 frames);
void UiWindow_InitializeWork(s32);
s32 Party_ListActiveOwnersFar(void *session);
void ItemMenu_Init(s32, s32, s32, s32);
void Resource_LoadPairedBlocks(void);
void Palette_LightenBankHighlight(s32);
void Link_DrawShiftedTilePairFar(void *address);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
s32 Scheduler_EnableOverlayCallbacksWithFlags(void);
void UiWork_SetAltFlagAndClearTableFar(s32);
void Menu_CancelSoundReset(void);
s32 ItemMenu_RunCommands(s32 *category, s32 *value, s32 *index);
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
    struct AssetSelectionScreen *screen;
    s32 index;
    s32 value;
    s32 category;
    s32 result;
#if EDITION_INTERNATIONAL
    s32 size;
    CopyFn copy;
#endif
    u8 **process;

#if EDITION_INTERNATIONAL
    size = 0x2000;
    backup = Runtime_BumpAllocateAlternatePool(size);
#endif
    screen = Runtime_AllocateHeapBlock(0x37, 0xa70);
    (&gMenuCtrlWork)->display_state->busy = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);
    screen->session_mode = Party_ListActiveOwnersFar(screen->session);
    ItemMenu_Init(0, 3, 0, 7);
    Resource_LoadPairedBlocks();
    Palette_LightenBankHighlight(14);
    Link_DrawShiftedTilePairFar((void *)0x06002500);
    screen->window = UiWindow_CreateFar(13, 0, 17, 3, 2);
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
        struct AssetSelectionResult *selection = (&gMenuCtrlWork)->selection_state;

        selection->packed = (category << 10) | (index & 0x1ff);
        selection->style = screen->selection_style;
    }
    RenderOutput_ClearListFar(screen->resource_handle);
    process = &(&gMenuCtrlWork)->process_state;
    (*process)[PROCESS_CLOSING] = 1;
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    Menu_ResetTwoResourceEntries();
    Runtime_ReleaseHeapBlock(0x37);
    (&gMenuCtrlWork)->display_state->busy = 0;
#if EDITION_INTERNATIONAL
    UiWindow_MarkVisibleTileAttributesFar();
    UiWork_SetAltFlagAndClearTableFar(0);
    CopyWords(copy, (void *)0x06004000, backup, size);
    (*process)[PROCESS_CLOSING] = 0;
    Runtime_BumpFree(backup);
    WaitFrames(1);
    Scheduler_DisableOverlayCallbacksWithFlags();
#endif
    WaitFrames(1);
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    (*process)[PROCESS_CLOSING] = 0;
    Event_ClearInvalidPackedValuesFar();
    return result;
}
