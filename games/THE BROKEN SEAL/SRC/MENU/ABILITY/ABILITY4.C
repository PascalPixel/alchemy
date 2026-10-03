#include "EDITION.H"
/* Browse the current owner's Psynergy from the status menu. L/R changes owners;
 * A advances to Item and B returns to character selection. The row coordinates
 * are signed because this view hides the four owner slots above the screen. */
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "MENU_RESULT.H"
#include "SYSTEM.H"
#include "UI.H"
#include "TBS_EDITION.H"
#include "OWNER_STATE.H"
#include "PSYNERGY_MENU.H"

struct BattleUnit;
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
extern u8 MsgSwitchCharacterHelp;
void Menu_BuildPatternTiles(void);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
s32 UiWindow_UpdateOrCreate(s32 *, s32, s32, s32, s32, s32);
void Menu_UpdateEntryObjectTransforms(void);
void Menu_SpawnIconEntries(struct PsynergyMenuState *, s32);
s32 GameFlag_TestFar(s32 flag);
void ItemMenu_PosCategory(void);
void PsynergyMenu_DrawPreparedPsynergyIcons(s32 window, s32 owner);
s32 PsynergyMenu_DrawListPage(s32 window, s32 unused, const struct MenuResult *);
s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused, struct MenuResult *);
void UiMenu_PositionCursor(s32 x, s32 y);
void Audio_PlayCue(s32 cue);
void PsynergyMenu_CallIconRoutineWithValue(void *menu, s32 owner);
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
    Scheduler_RemoveCallback((u32)(Menu_UpdateEntryObjectTransforms));
    Menu_SpawnIconEntries(menu, menu->message_window);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgSwitchCharacterHelp, menu->status_window, SWITCH_HELP_X, -24);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgSwitchCharacterHelp + 2, menu->status_window, 0, -24);

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        ItemMenu_PosCategory();
        RenderOutput_RedrawSavedRectFar(menu->status_window);
        menu->psynergy_count = (s32)PsynergyMenu_CollectActions(
            (struct BattleUnit *)Owner_GetStateFar(menu->owner_ids[0]), menu->psynergies, 0);
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
            if (gKeyState & 1) {
                Audio_PlayCue(112);
                result = 1;
                done = 1;
                break;
            }
            if (gKeyState & 2) {
                Audio_PlayCue(113);
                result = -1;
                done = 1;
                Scheduler_AddOrUpdateCallback((s32)(Menu_UpdateEntryObjectTransforms), 0xc80);
                break;
            }
            if ((gKeysRepeat & 0x100) || (gKeysRepeat & 0x200)) {
                Audio_PlayCue(111);
                tab = menu->tab_index[0];
                menu->selected_index_by_owner[menu->owner_table[tab]] = state.selected_index;
                if (gKeysRepeat & 0x100)
                    tab++;
                else
                    tab--;
                tab = (tab + menu->owner_count) % menu->owner_count;
                menu->selected_owner = menu->owner_table[tab];
                menu->owner_ids[0] = menu->owner_table[tab];
                menu->tab_index[0] = tab;
                PsynergyMenu_CallIconRoutineWithValue(menu, menu->owner_table[tab]);
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

/* The routine after the Psynergy action selector: it only reports
   success. */
s32 PsynergyMenu_ActionReturnTrue(void)
{
    return 1;
}

void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
    struct PsynergyMenuState *menu = gMenuWork;

    Owner_GetStateFar(owner_id);
    ItemMenu_HideAllIcons();
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void PsynergyMenu_PreparedIconsNoOp(void)
{
}

/* A second empty routine; nothing in the ROM refers to either. */
void PsynergyMenu_PreparedIconsSecondNoOp(void)
{
}
