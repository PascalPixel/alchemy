/* NONMATCHING: shared callee return types audited on 2026-09-26.
 * 432 of 432 bytes, 122 differing halfwords, 87 aligned edits.
 * Canonical declarations are retained; the remaining source model is not exact. */
/*
 * RunAssetSelectionScreen (main:080a24d0, 432 bytes)
 *
 * Draft, not exact (2026-09-24): candidate=432 reference=432
 * differing_halfwords=122.  Split from ItemMenu_RunCommands (main:080a2680),
 * which the old listing bundled.  Every call and argument lines up; the two
 * IWRAM routines are called through function pointers (_call_via_fp for the
 * word copy at 0x03001388, _call_via_r3 for the fill at 0x03000168).
 * Remaining: the reference computes &globals->process_state once into r6
 * (r8 + 36) and reuses it for the three 0xea6 stores, which spends every
 * callee-saved register and makes it rematerialise 0x06004000 from the pool
 * at each copy; here the field is addressed as [r8 + 36] each time, the VRAM
 * address stays in r7 and the backup moves to r6.  A pointer local for the
 * field folds back to a separate constant (constant and extern-struct
 * spellings alike).
 */
#include "TYPES.H"

struct AssetSelectionGlobals {
    struct { u16 unk0; u16 unk2; s16 busy; } *display_state;
    u8 reserved_004[0x20];
    u8 *process_state;
    u8 reserved_028[0x2c];
    u8 *selection_state;
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

extern struct AssetSelectionGlobals Data_03001e68;

typedef void (*CopyFn)(const void *src, void *dst, s32 size);
typedef void (*FillFn)(void *dst, s32 size, u32 value);

void *Runtime_BumpAllocateAlternatePool(s32 size);
struct AssetSelectionScreen *Runtime_AllocateHeapBlock(s32 id, s32 size);
void UiWindow_DrawFrameFar(s32, s32, s32, s32);
void WaitFrames(s32 frames);
void UiWindow_InitializeWork(s32);
s32 Party_ListActiveOwnersFar(void *session);
void ItemMenu_Init(s32, s32, s32, s32);
void Func_080a5534(void);
void Palette_LightenBankHighlight(s32);
void Link_DrawShiftedTilePairFar(void *address);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
s32 Scheduler_EnableOverlayCallbacksWithFlags(void);
void Func_080153e0(s32);
void Menu_CancelSoundReset(void);
s32 ItemMenu_RunCommands(s32 *category, s32 *value, s32 *index);
void Menu_EnsureCancelSound(void);
void RenderOutput_ClearListFar(s32);
void ItemMenu_Close(void);
void Menu_ResetTwoResourceEntries(void);
void Runtime_ReleaseHeapBlock(s32);
void Func_080152a8(void);
void Runtime_BumpFree(void *);
s32 Scheduler_DisableOverlayCallbacksWithFlags(void);
void UiWindow_EraseBorderRectFar(s32, s32, s32, s32);
void Event_ClearInvalidPackedValuesFar(void);

extern u8 Value_00000001;

/* Run the modal asset-selection screen and publish an accepted selection. */
s32 RunAssetSelectionScreen(void)
{
#define globals (&Data_03001e68)
    void *backup;
    struct AssetSelectionScreen *screen;
    s32 index;
    s32 value;
    s32 category;
    s32 result;
    s32 size;
    CopyFn copy;

    size = 0x2000;
    backup = Runtime_BumpAllocateAlternatePool(size);
    screen = Runtime_AllocateHeapBlock(0x37, 0xa70);
    globals->display_state->busy = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);
    screen->session_mode = Party_ListActiveOwnersFar(screen->session);
    ItemMenu_Init(0, 3, 0, 7);
    Func_080a5534();
    Palette_LightenBankHighlight(14);
    Link_DrawShiftedTilePairFar((void *)0x06002500);
    screen->window = UiWindow_CreateFar(13, 0, 17, 3, 2);
    Scheduler_EnableOverlayCallbacksWithFlags();
    copy = (CopyFn)0x03001388;
    copy(backup, (void *)0x06004000, size);
    ((FillFn)0x03000168)((void *)0x06004000, size, 0x33333333);
    Func_080153e0(1);
    Menu_CancelSoundReset();
    result = ItemMenu_RunCommands(&category, &value, &index);
    Menu_EnsureCancelSound();

    if (result == 1) {
        u8 *selection = globals->selection_state;

        u16 packed = (category << 10) | (index & 0x1ff);
        s32 style;

        *(u16 *)(selection + 0x180) = packed;
        style = screen->selection_style;
        *(u16 *)(selection + 0x19a) = style;
    }

    RenderOutput_ClearListFar(screen->resource_handle);
    globals->process_state[0xea6] = (s32)&Value_00000001;
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    Menu_ResetTwoResourceEntries();
    Runtime_ReleaseHeapBlock(0x37);
    globals->display_state->busy = 0;
    Func_080152a8();
    Func_080153e0(0);
    copy((void *)0x06004000, backup, size);
    globals->process_state[0xea6] = 0;
    Runtime_BumpFree(backup);
    WaitFrames(1);
    Scheduler_DisableOverlayCallbacksWithFlags();
    WaitFrames(1);
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    globals->process_state[0xea6] = 0;
    Event_ClearInvalidPackedValuesFar();
    return result;
}
