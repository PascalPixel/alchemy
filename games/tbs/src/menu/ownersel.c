#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "layout_guard.h"
#include "types.h"
#include "tbs_edition.h"
#include "character_menu.h"
#include "system.h"
#include "mem.h"
#include "ui.h"
#include "menu_list.h"

void ItemMenu_Close(void);

#if EDITION_INTERNATIONAL
#define ROW_CNT 8
#else
#define ROW_CNT 4
#endif

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
        Party_ListActiveOwnersFar(state->owner_ids);
    Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
    state->selector_window = (struct UiWindow *)UiWindow_CreateFar(13, 0, 17, 5, 2);
    for (index = 0; index < ROW_CNT; index++)
        state->owner_y[index] = 30;
    state->page = 3;

    result = CharacterSelector_Run();

    RenderOutput_ClearListFar(state->status_window);
    ItemMenu_Close();
    gMenuCtrlWork->suspended = 0;
    WaitFrames(1);
    Runtime_ReleaseHeapBlock(55);
    return result;
}

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
