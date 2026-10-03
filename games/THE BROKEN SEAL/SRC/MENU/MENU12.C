#include "EDITION.H"
#include "LAYOUT_GUARD.H"
#include "TYPES.H"
#include "TBS_EDITION.H"
#include "CHARACTER_MENU.H"
#include "SYSTEM.H"
#include "RUNTIME_MEM.H"
#include "UI.H"

s32 Runtime_AllocateHeapBlock(s32 slot, s32 size);
extern struct ObjectSystemWork *gMenuCtrlWork;
extern struct CharacterMenuState *gMenuWork;
void ItemMenu_Close(void);
void RenderOutput_ClearListFar(s32);
void UiWindow_DrawFrameFar(s32, s32, s32, s32);
s32 Party_ListActiveOwnersFar(u16 *);
void UiWindow_InitializeWork(s32);

#if EDITION_INTERNATIONAL
#define ROW_CNT 8
#else
#define ROW_CNT 4
#endif

/*
 * Open the compact character selector, run its blocking interaction body,
 * and tear the screen down.  The shared object list is suspended while this
 * modal owns the display.  Its eight row anchors all begin at x=30.
 */
s32 Menu_OpenCharacterSelector(void)
{
    struct CharacterMenuState *state =
        (struct CharacterMenuState *)Runtime_AllocateHeapBlock(55, 0x0a70);
    s32 result;
    s32 index;

    gMenuCtrlWork->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);

    state->party_count =
        (u8)Party_ListActiveOwnersFar(state->owner_ids);
    Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
    state->selector_window = (struct UiWindow *)UiWindow_CreateFar(13, 0, 17, 5, 2);
    for (index = 0; index < ROW_CNT; index++)
        state->owner_y[index] = 30;
    state->page = 3;

    result = CharacterSelector_Run();

    RenderOutput_ClearListFar((s32)state->status_window);
    ItemMenu_Close();
    gMenuCtrlWork->suspended = 0;
    WaitFrames(1);
    Runtime_ReleaseHeapBlock(55);
    return result;
}

/* Run the character selector and return the chosen owner id, or -1 when it was
   cancelled. */
s32 CharacterSelector_Run(void)
{
    struct CharacterMenuState *work = gMenuWork;
    s32 result = 0;

    /* FAKEMATCH: the do-while keeps the cursor store in source order */
    do {
        work->selected_slots[0] = result;
    } while (0);
    if (CharacterMenu_SelectOwner(0) == -1)
        result = -1;
    else
        result = work->selected_ids[0];
    return result;
}
