/* 2026-09-29: psynergy editions: the pooled 1 is the same in all five
 * matching editions, a plain constant, so it is written as 1 (score 1570,
 * was 820 with an invented symbol). The reference also keeps
 * &gMenuCtrlWork.process_state in r6 and reuses the screen register for
 * 0xea6 after RenderOutput_ClearListFar. */
/* 2026-09-29: eight minutes of permutation (--function
 * RunAssetSelectionScreen): 1530 -> 820 (11 register-only, 6 operand, 4
 * reordered, 2 inserted, 2 deleted) by taking the address of the
 * Value_00000001 symbol before the list clear and storing it after. A plain
 * 1, in any local type, gives 1570: the reference loads that 1 from the
 * pool before the call, as a link-time value does, so this draft stays
 * blocked on it. */
/* 2026-09-29: Resource_LoadPairedBlocks carries the build's name; alchemy
 * permute (--function RunAssetSelectionScreen) scores 1530, from 1550. */
/* NONMATCHING: 428/432 bytes, 195 differing halfwords, 137 aligned edits.
 * The proven menu copy/fill interface recovers per-call VRAM loads, but
 * keeps backup in r6 and hoists a zero into fp; not an exact model. */
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

/* H1 (2026-09-27): whole [080a24d0,080a2680), 432 bytes with both pools;
 * caller SELECTION_RUN_TOP_SELECTION.C and exact callee RUN_COMMANDS.C
 * audited. The earlier return-type audit only changed scheduler calls.
 * OPEN_ACTION_MENU.C, OPEN_BACKDROP_SCREEN.C and RUN_COMMANDS.C agree on
 * value-returning, destination-first resident copy/fill interfaces and
 * inline wrappers. Own-ROM resident copier stores through r0, reads r1.
 * Transfer that proven call shape, not another global-pointer spelling.
 * Prediction: per-call VRAM rematerialization and backup retention match
 * the reference r7/fp roles. Read full loops/frame/pools and normalized
 * diff. Gate: full exact extent plus compare/test/coverage/verify. Budget:
 * one model plus at most two justified follow-ups, stop by 00:30 Lisbon;
 * every result is retained in this header and its own commit.
 * H1 result: 428/432, 195 differing halfwords, 137 aligned edits; full
 * normalized diff read. Per-call VRAM loads are recovered, but the common
 * zero takes fp, size moves to sl, copy to r9, and backup remains r6.
 * The process-cell address is still missing. No further justified call
 * shape follows from these facts; stop this axis rather than repeating
 * the prior global-pointer or declaration-order attempts. Zero new DONE.
 */

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

extern struct AssetSelectionGlobals gMenuCtrlWork;

typedef s32 (*CopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*FillFn)(void *dst, s32 size, u32 value);

static __inline__ s32 CopyWords(CopyFn copy, void *dst, const void *src, s32 size)
{
    return copy(dst, src, size);
}

static __inline__ s32 FillWords(FillFn fill, void *dst, s32 size, u32 value)
{
    return fill(dst, size, value);
}

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

/* Run the modal asset-selection screen and publish an accepted selection. */
s32 RunAssetSelectionScreen(void)
{
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
    Scheduler_EnableOverlayCallbacksWithFlags();
    copy = (CopyFn)0x03001388;
    CopyWords(copy, backup, (void *)0x06004000, size);
    FillWords((FillFn)0x03000168, (void *)0x06004000, size, 0x33333333);
    Func_080153e0(1);
    Menu_CancelSoundReset();
    result = ItemMenu_RunCommands(&category, &value, &index);
    Menu_EnsureCancelSound();
    if (result == 1) {
        u8 *selection = (&gMenuCtrlWork)->selection_state;
        u16 packed = (category << 10) | (index & 0x1ff);
        s32 style;
        *(u16 *)(selection + 0x180) = packed;
        style = screen->selection_style;
        *(u16 *)(selection + 0x19a) = style;
    }
    RenderOutput_ClearListFar(screen->resource_handle);
    (&gMenuCtrlWork)->process_state[0xea6] = 1;
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    Menu_ResetTwoResourceEntries();
    Runtime_ReleaseHeapBlock(0x37);
    (&gMenuCtrlWork)->display_state->busy = 0;
    Func_080152a8();
    Func_080153e0(0);
    CopyWords(copy, (void *)0x06004000, backup, size);
    (&gMenuCtrlWork)->process_state[0xea6] = 0;
    Runtime_BumpFree(backup);
    WaitFrames(1);
    Scheduler_DisableOverlayCallbacksWithFlags();
    WaitFrames(1);
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    (&gMenuCtrlWork)->process_state[0xea6] = 0;
    Event_ClearInvalidPackedValuesFar();
    return result;
}
