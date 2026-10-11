#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "menu_result.h"
#include "system.h"
#include "ui.h"
#include "tbs_edition.h"
#include "owner_state.h"
#include "psynergy_menu.h"
#include "btlsys.h"
#include "text/msg_ids.h"

#define RenderOutput_RedrawSavedRectFar ((void (*)(s32))RenderOutput_RedrawSavedRectFar)
#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)

struct BtlUnit;
void Menu_SpawnIconEntries(struct PsynergyMenuState *, s32);
void AudioCommand_PlayFar(s32 cue);
void ItemMenu_HideAllIcons(void);

s32 PsynergyMenu_SelectAction(void)
{

    struct PsynergyMenuState *menu;
    s32 result;
    s32 done;
    s32 redraw;
    s32 first;
    s32 nav;
    s32 tab;
    s32 i;
    struct MenuResult state;

    menu = ((struct PsynergyMenuState *)gMenuWork);
    result = 0;
    done = 0;
    Menu_BuildPatternTiles();
    RenderOutput_RedrawSavedRectFar(menu->message_window);
    UiWindow_UpdateOrCreate((s32 *)&menu->info_window, 0, 0, 30, 5, 2);
    for (i = 3; i >= 0; i--)
        menu->slot_y[i] = -16;
    {
        s32 priority = 245;
        struct RenderOutput **icons = menu->entry_icons;

        for (i = 31; i >= 0; i--) {
            struct RenderOutput *icon = *icons++;

            if (icon != 0)
                icon->sentinel = priority;
        }
    }
    Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
    Menu_SpawnIconEntries(menu, menu->message_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgSwitchCharacterHelp, menu->status_window, SWITCH_HELP_X, -24);
    UiText_DrawCharacterAtOffsetFar((s32)MsgSwitchCharacterHelp + 2, menu->status_window, 0, -24);

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        ItemMenu_PosCategory();
        RenderOutput_RedrawSavedRectFar(menu->status_window);
        menu->psynergy_count = PsynergyMenu_CollectActions(
            (struct BtlUnit *)UnitGetFar(menu->owner_ids[0]), menu->psynergies, 0);
        WaitFrames(1);
        Menu_BuildPageResult(&state, 0);
        PsynergyMenu_DrawPreparedPsynergyIcons(menu->status_window, menu->owner_ids[0]);
        redraw = 1;
        first = 1;

        while (GameFlag_TestFar(0x150) == 0) {
            if (redraw != 0) {
                redraw = 0;
                if (first != 0) {
                    first = 0;
                    PsynergyMenu_DrawListPage(menu->status_window, 0, &state);
                }
                PsynergyMenu_DrawRangePage(menu->status_window, 0, &state);
                WaitFrames(1);
            }
            WaitFrames(1);
            nav = Menu_HandlePageInput(0, state.entry_count, PAGE_ROWS, &state.row, &state.page);
            menu->pane_icon[0]->active = 1;
#if EDITION_INTERNATIONAL
            UiMenu_PositionCursor(55, state.row * 16 + 60);
#else
            UiMenu_PositionCursor(58, state.row * 16 + 52);
#endif
            if (nav == 1) {
                first = 1;
                redraw = 1;
            }
            if (nav == 0)
                redraw = 1;
            if (nav == -1)
                redraw = 0;
            if (gKeyState & KEY_A) {
                AudioCommand_PlayFar(112);
                result = 1;
                done = 1;
                break;
            }
            if (gKeyState & KEY_B) {
                AudioCommand_PlayFar(113);
                result = -1;
                done = 1;
                Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
                break;
            }
            if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
                AudioCommand_PlayFar(111);
                tab = menu->tab_index[0];
                menu->selected_index_by_owner[menu->owner_table[tab]] = state.selected_index;
                if (gKeysRepeat & KEY_R)
                    tab++;
                else
                    tab--;
                tab = (tab + menu->owner_count) % menu->owner_count;
                menu->selected_owner = menu->owner_table[tab];
                menu->owner_ids[0] = menu->owner_table[tab];
                menu->tab_index[0] = tab;
                PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->owner_table[tab]);
                break;
            }
        }
    }
    RenderOutput_ClearListFar((s32)menu->info_window);
    RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
    ItemMenu_HideAllIcons();
    RenderOutput_ClearListFar(menu->message_window);
    RenderOutput_RedrawSavedRectFar(menu->status_window);
    return result;
}

s32 PsynergyMenu_ActionReturnTrue(void)
{
    return 1;
}

void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
    struct PsynergyMenuState *menu = gMenuWork;

    UnitGetFar(owner_id);
    ItemMenu_HideAllIcons();
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void PsynergyMenu_PreparedIconsNoOp(void)
{
}

void PsynergyMenu_PreparedIconsSecondNoOp(void)
{
}
