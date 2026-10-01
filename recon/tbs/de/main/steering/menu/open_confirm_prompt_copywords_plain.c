/* NONMATCHING: 2026-10-01 brief Wave2 CopyWords plain-source attempt.
 * Removing this one source device changes Menu_OpenConfirmPrompt.
 * First remaining difference: Menu_OpenConfirmPrompt: mov	r9, r1 => mov	fp, r1 (179/181 assembly lines).
 * Measured with the existing TBS agscc option set, DE edition; no option changes.
 * This reduced draft preserves the affected function and its declarations.
 * Production retains the measured device with its FAKEMATCH reason.
 */
#include "TYPES.H"
#include "SYSTEM.H"
#include "UI.H"
#include "TBS_EDITION.H"
#include "IWRAM_CALL.H"

#define FIELD(ptr, type, offset) (*(type *)((u8 *)(ptr) + (offset)))

struct MenuObjectControl {
    u8 padding00[4];
    u16 suspended;
};

extern struct MenuObjectControl *gMenuCtrlWork;

s32 Runtime_AllocateHeapBlock(s32 kind, s32 size);
void UiWindow_DrawFrameFar(s32 x, s32 y, s32 width, s32 height);
void UiWindow_InitializeWork(s32 unused);
s32 Party_ListActiveOwnersFar(const u16 *ids);
void ItemMenu_Init(s32, s32, s32, s32);
void Palette_LightenBankHighlight(s32 index);
void Link_DrawShiftedTilePairFar(s32 addr);
void Menu_CancelSoundReset(void);
s32 Menu_ResolveSelectedAction(s32 *, s32 *, s32 *);
void Menu_EnsureCancelSound(void);
s32 BattleAction_Get(s32 flags);
void RenderOutput_ClearListFar(s32 screen_handle);
void ItemMenu_Close(void);
void UiWindow_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void Event_ClearInvalidPackedValuesFar(void);

/* The European editions keep the 8 KB of background tiles at 0x06004000
   aside while the prompt is open and fill that area with colour 3. */
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define PROMPT_SAVES_TILES 1
#define PROMPT_TILES ((void *)0x06004000)
#define PROMPT_TILES_SIZE 0x2000
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(void *buffer);
void Scheduler_EnableOverlayCallbacksWithFlags(void);
void Scheduler_DisableOverlayCallbacksWithFlags(void);
void UiWork_SetAltFlagAndClearTableFar(s32 enable);
void UiWindow_MarkVisibleTileAttributesFar(void);

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);


static __inline__ s32 FillWords(WordFillFn fill, void *dst, s32 size, u32 value)
{
    /* FAKEMATCH: a direct fill call changes the preceding copy's fp call register and adds moves. */
    return fill(dst, size, value);
}
#endif

/*
 * Open a modal menu screen and run its blocking interaction body.
 *
 * The address of gMenuCtrlWork, not the pointer it holds, is the base for the
 * two fixed-address fields at +0x24 and +0x54; only "suspended" is reached
 * through the pointer itself.  The field read at +0x178 is named by position
 * only and is not otherwise confirmed.
 */
s32 Menu_OpenConfirmPrompt(void)
{
#if defined(PROMPT_SAVES_TILES)
    void *saved = Runtime_BumpAllocateAlternatePool(PROMPT_TILES_SIZE);
#endif
    void *state = (void *)Runtime_AllocateHeapBlock(0x37, 0xa70);
    s32 high;
    s32 unused;
    s32 low;
    s32 result;

    gMenuCtrlWork->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);
    FIELD(state, u8, 0x219) = (u8)Party_ListActiveOwnersFar((const u16 *)((u8 *)state + 0x208));
    ItemMenu_Init(0, 3, 0, 7);
    FIELD(state, s32, 0x10c) = UiWindow_CreateFar(13, 0, 17, 3, 2);
    Palette_LightenBankHighlight(14);
    Link_DrawShiftedTilePairFar(0x06002500);
#if defined(PROMPT_SAVES_TILES)
    Scheduler_EnableOverlayCallbacksWithFlags();
    Iwram_CopyWords(saved, PROMPT_TILES, PROMPT_TILES_SIZE);
    FillWords(Iwram_FillWords, PROMPT_TILES, PROMPT_TILES_SIZE, 0x33333333);
    UiWork_SetAltFlagAndClearTableFar(1);
#endif
    Menu_CancelSoundReset();
    result = Menu_ResolveSelectedAction(
        &high, &unused, &low);
    Menu_EnsureCancelSound();
    if (result == 1) {
        void *target = FIELD(&gMenuCtrlWork, void *, 0x54);
        u16 flags;
        BattleAction_Get(0x3fff & FIELD(state, u16, 0x178));
        flags = (u16)(low | (high << 10));
        FIELD(target, u16, 0x17e) = flags;
    }
    RenderOutput_ClearListFar(FIELD(state, s32, 0x24));
    FIELD(FIELD(&gMenuCtrlWork, void *, 0x24), u8, RENDER_MENU_BUSY_OFS) = 1;
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    Runtime_ReleaseHeapBlock(0x37);
    gMenuCtrlWork->suspended = 0;
#if defined(PROMPT_SAVES_TILES)
    UiWindow_MarkVisibleTileAttributesFar();
    UiWork_SetAltFlagAndClearTableFar(0);
    Iwram_CopyWords(PROMPT_TILES, saved, PROMPT_TILES_SIZE);
    FIELD(FIELD(&gMenuCtrlWork, void *, 0x24), u8, RENDER_MENU_BUSY_OFS) = 0;
    Runtime_BumpFree(saved);
    WaitFrames(1);
    Scheduler_DisableOverlayCallbacksWithFlags();
#endif
    WaitFrames(1);
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    FIELD(FIELD(&gMenuCtrlWork, void *, 0x24), u8, RENDER_MENU_BUSY_OFS) = 0;
    Event_ClearInvalidPackedValuesFar();
    return result;
}
