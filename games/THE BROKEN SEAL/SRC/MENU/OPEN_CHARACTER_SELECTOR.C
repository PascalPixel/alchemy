#include "LAYOUT_GUARD.H"
#include "TYPES.H"
#include "TBS_EDITION.H"
#include "INVENTORY_MENU.H"
#include "CHARACTER_MENU.H"
#include "SYSTEM.H"
#include "UI.H"

#if defined(TBS_EDITION_JA)
#define ROW_CNT 4
#else
#define ROW_CNT 8
#endif

struct MenuObjectControl {
    u8 padding00[4];
    u16 suspended;
};

struct CharacterSelectorState {
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
};

LAYOUT_OFFSET_GUARD(
    CharacterMenu080a7380_ScreenHandle,
    struct CharacterSelectorState,
    screen_handle,
    0x24);
LAYOUT_OFFSET_GUARD(
    CharacterMenu080a7380_SelectorWindow,
    struct CharacterSelectorState,
    selector_window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    CharacterMenu080a7380_RowPositions,
    struct CharacterSelectorState,
    row_positions,
    0x144);
LAYOUT_OFFSET_GUARD(
    CharacterMenu080a7380_CharacterIds,
    struct CharacterSelectorState,
    character_ids,
    0x208);
LAYOUT_OFFSET_GUARD(
    CharacterMenu080a7380_SelectorFlags,
    struct CharacterSelectorState,
    flags,
    0x220);

extern struct MenuObjectControl *gMenuCtrlWork;

struct CharacterSelectorState *Runtime_AllocateHeapBlock(s32, s32);
void RenderOutput_ClearListFar(s32);
void UiWindow_DrawFrameFar(s32, s32, s32, s32);
s32 Party_ListActiveOwnersFar(const u16 *);
void UiWindow_InitializeWork(s32);
void Menu_InitSelectorCursorAndEntries(s32, s32, s32, s32);
s32 CharacterSelector_Run(void);

/*
 * Open the compact character selector, run its blocking interaction body,
 * and tear the screen down.  The shared object list is suspended while this
 * modal owns the display.  Its eight row anchors all begin at x=30.
 */
s32 Menu_OpenCharacterSelector(void)
{
    struct CharacterSelectorState *state =
        Runtime_AllocateHeapBlock(55, 0x0a70);
    s32 result;
    s32 index;

    gMenuCtrlWork->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);

    state->character_count =
        (u8)Party_ListActiveOwnersFar(state->character_ids);
    Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
    state->selector_window = UiWindow_CreateFar(13, 0, 17, 5, 2);
    for (index = 0; index < ROW_CNT; index++)
        state->row_positions[index] = 30;
    state->flags = 3;

    result = CharacterSelector_Run();

    RenderOutput_ClearListFar(state->screen_handle);
    InventoryMenu_CloseWindows();
    gMenuCtrlWork->suspended = 0;
    WaitFrames(1);
    Runtime_ReleaseHeapBlock(55);
    return result;
}
