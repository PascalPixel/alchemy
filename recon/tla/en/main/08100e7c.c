#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"

struct ItemMenuModeState {
    u8 reserved_000[0x20c];
    u8 mode;
};

struct ItemMenuDisplayState {
    u16 reserved_00[2];
    u16 busy;
};

/* The pointer at 0x2128 is cleared beside the variant; its pointer type
   keeps the two stores apart for the scheduler. */
struct ItemMenuSavedGraphics {
    u8 reserved_000[0xa8];
    u8 tiles[0x2000];
    u16 palette[64];
    void *reserved_2128;
    s32 variant;
};

struct ItemMenuWork {
    u8 reserved_000[0x1c];
    u8 selection;
    u8 target;
    u8 reserved_01e[0xee];
    s32 window;
    u8 reserved_110[0x64];
    u16 slot;
    u16 target_slot;
    u16 item;
    u8 reserved_17a[10];
    struct ItemMenuSavedGraphics *saved;
    u8 reserved_188[0x80];
    u16 owners[8];
    u8 reserved_218;
    u8 count;
};

extern struct ItemMenuModeState gGameState;
extern struct ItemMenuDisplayState *gMenuCtrlWork;
s32 GameFlag_TestFar(s32 flag);
void UiWindow_DrawFrameFar(s32 x, s32 y, s32 width, s32 height);
void UiWindow_InitializeWork(s32 mode);
void Scheduler_EnableOverlayCallbacksWithFlags(void);
void Scheduler_DisableOverlayCallbacksWithFlags(void);
void Func_080153e0(s32 value);
void Func_080152a8(void);
void Link_DrawShiftedTilePairFar(void *tiles);
s32 Party_ListActiveOwnersFar(u16 *owners);
void Resource_LoadPairedBlocksIfAvailable(void);
void ItemMenu_Init(s32 x, s32 y, s32 mode, s32 columns);
void Menu_SetFirstObjectRowCoordinates(s32 mode);
void Palette_LightenBankHighlight(s32 palette);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void FourObjectMotion_InitializeBottomRow(s32 window, s32 mode);
void Func_080aa768(void);
void FourObjectMotion_ClearSlotsAndScheduleAlt(void);
void Menu_ResetTwoResourceEntries(void);
void ItemMenu_Close(void);
void UiWindow_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void Event_ClearInvalidPackedValuesFar(void);

/* Open the item menu as a modal view, selecting its story variant, then
   restore the saved graphics and the caller's menu mode. */

s32 ItemMenu_Run(void)
{
    struct ItemMenuWork *menu;
    u32 old_mode;
    s32 state;
    struct ItemMenuSavedGraphics *saved;
    s32 zero;

    menu = Runtime_AllocateHeapBlock(55, 0xa70);
    old_mode = gGameState.mode;
    gGameState.mode = 2;
    state = gMenuCtrlWork->busy = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);
    saved = Runtime_BumpAllocateAlternatePool(0x2130);
    menu->saved = saved;
    saved->reserved_2128 = 0;
    saved->variant = 0;
    if (GameFlag_TestFar(0x16e)) {
        if (!GameFlag_TestFar(0x16f)) {
            if (!GameFlag_TestFar(0x171))
                saved->variant = state;
            else
                saved->variant = 14;
        } else {
            if (!GameFlag_TestFar(0x171))
                saved->variant = 27;
            else
                saved->variant = 28;
        }
    }
    Scheduler_EnableOverlayCallbacksWithFlags();
    Func_080153e0(1);
    Link_DrawShiftedTilePairFar((void *)0x06002500);
    menu->count = Party_ListActiveOwnersFar(menu->owners);
    Resource_LoadPairedBlocksIfAvailable();
    ItemMenu_Init(0, 3, 0, 7);
    Menu_SetFirstObjectRowCoordinates(0);
    Palette_LightenBankHighlight(14);
    menu->window = UiWindow_CreateFar(13, 0, 17, 5, 2);
    menu->item = 255;
    zero = 0;
    menu->selection = zero;
    menu->target = zero;
    menu->slot = 0;
    menu->target_slot = 0;
    FourObjectMotion_InitializeBottomRow(menu->window, 0);
    Func_080aa768();
    FourObjectMotion_ClearSlotsAndScheduleAlt();
    Menu_ResetTwoResourceEntries();
    WaitFrames(1);
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    gMenuCtrlWork->busy = 0;
    Func_080152a8();
    Func_080153e0(0);
    Iwram_CopyWords((void *)0x06004000, saved->tiles, 0x2000);
    Iwram_CopyWords((void *)0x05000080, saved->palette, 128);
    WaitFrames(1);
    Scheduler_DisableOverlayCallbacksWithFlags();
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    Runtime_BumpFree(menu->saved);
    Runtime_ReleaseHeapBlock(55);
    Event_ClearInvalidPackedValuesFar();
    gGameState.mode = old_mode;
    return 1;
}
