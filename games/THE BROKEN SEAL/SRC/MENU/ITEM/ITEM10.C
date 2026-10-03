#include "EDITION.H"
#include "BATTLE_RUNTIME.H"
#include "TYPES.H"
#include "INVENTORY_MENU.H"
#include "CALLBACK_SCHEDULER.H"
#include "BATTLE_TYPES.H"
#include "GLOBAL_CELLS.H"
#include "MENU_RESULT.H"
#include "SYSTEM.H"
#include "UI.H"
#include "TBS_EDITION.H"

extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;

/* "{L}-{R}:Switch characters", then "{A}:Status". */
extern u8 MsgSwitchCharacterHelp;
#define KEY_A 1
#define KEY_B 2
#define KEY_R 0x100
#define KEY_L 0x200
s32 GameFlag_TestFar(s32 flag);
s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void ItemMenu_PosCategory(void);
void Menu_UpdateEntryObjectTransforms(void);
s32 InventoryMenu_SortByListOrder(u16 *items, s32 mode);
void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
void RenderOutput_ClearListFar(void *window);
s32 Shop_DrawItemPage(s32 window, s32 unused, struct MenuResult *state);
s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state);
void UiMenu_PositionCursor(s32 x, s32 y);
void Audio_PlayCue(s32 cue);
void PsynergyMenu_CallIconRoutineWithValue(void *work, s32 value);

/* The status screen's item page: browse the current character's items, switch
   characters with L and R, and return 1 for A or -1 for B. */
s32 ItemMenu_SelectItem(void)
{
    struct InventoryMenuState *menu;
    struct BattleUnit *owner;
    s32 result;
    s32 done;
    s32 redraw;
    s32 first;
    s32 nav;
    s32 tab;
    s32 i;
    u16 saved[15];
    struct MenuResult state;

    menu = gMenuWork;
    {
        s32 id = menu->pane_owner[0];

        result = 0;
        Owner_GetStateFar(menu->owner_ids[id]);
    }
#if EDITION_INTERNATIONAL
    UiWindow_UpdateOrCreate((s32 *)&menu->equip_window, 0, 10, 15, 10, 2);
#else
    UiWindow_UpdateOrCreate((s32 *)&menu->equip_window, 0, 10, 13, 10, 2);
#endif
    ItemMenu_PosCategory();

    for (i = 0; i < 32; i++) {
        struct InventoryMenuIcon *icon = menu->entry_icons[i];

        if (icon != 0)
            icon->sentinel = 240;
    }

    Scheduler_RemoveCallback((u32)(Menu_UpdateEntryObjectTransforms));
    UiText_DrawCharacterAtOffsetFar((s32)&MsgSwitchCharacterHelp, (s32)menu->status_window, 64, -24);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgSwitchCharacterHelp + 3, (s32)menu->status_window, 0, -24);
    ItemMenu_PosCategory();
    WaitFrames(1);
    Menu_SpawnIconEntries(menu, (s32)menu->message_window);

    done = 0;
    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        ItemMenu_PosCategory();
        RenderOutput_RedrawSavedRectFar(menu->status_window);
        owner = Owner_GetStateFar(menu->pane_owner[0]);
        for (i = 0; i <= 14; i++)
            saved[i] = owner->inventory[i];
        menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
        InventoryMenu_SortByListOrder(menu->items, 0);
        Menu_BuildPageResult(&state, 0);
        RenderOutput_RedrawSavedRectFar(menu->equip_window);
        ItemMenu_DrawCategory((s32)menu->equip_window, menu->pane_owner[0], 1);
        WaitFrames(1);
        ItemMenu_DrawIcons(menu->items, 0);
        redraw = 1;
        first = 1;

        while (GameFlag_TestFar(0x150) == 0) {
            if (redraw != 0) {
                RenderOutput_ClearListFar(menu->info_window);
                redraw = 0;
                if (first != 0) {
                    first = 0;
                    Shop_DrawItemPage((s32)menu->status_window, 0, &state);
                }
                ItemMenu_DrawEquipPage((s32)menu->status_window, 0, &state);
            }
            menu->pane_icons[0]->state = 1;
            UiMenu_PositionCursor(96, state.row * 16 + 52);
            WaitFrames(1);

            nav = Menu_HandlePageInput(0, state.entry_count, PAGE_ROWS, &state.row, &state.page);
            if (nav == 1) {
                first = 1;
                redraw = 1;
            }
            if (nav == 0)
                redraw = 1;
            if (nav == -1)
                redraw = 0;

            if (gKeyState & KEY_A) {
                Audio_PlayCue(0x70);
                result = 1;
                done = 1;
                break;
            }
            if (gKeyState & KEY_B) {
                Audio_PlayCue(0x71);
                result = -1;
                done = 1;
                break;
            }
            if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
                Audio_PlayCue(0x6f);
                tab = menu->pane_index[0];
                menu->selected_index_by_owner[menu->owner_ids[tab]] = state.selected_index;
                if (gKeysRepeat & KEY_R)
                    tab++;
                else
                    tab--;
                for (i = 0; i <= 14; i++)
                    owner->inventory[i] = saved[i];
                tab = (tab + menu->party_count) % menu->party_count;
                menu->selected_owner = menu->owner_ids[tab];
                menu->pane_owner[0] = menu->owner_ids[tab];
                menu->pane_index[0] = tab;
                PsynergyMenu_CallIconRoutineWithValue(menu, menu->owner_ids[tab]);
                break;
            }
        }
    }

    RenderOutput_ClearListFar(menu->info_window);
    RenderOutput_RedrawSavedRectFar(menu->info_window);
    RenderOutput_ClearListFar(menu->message_window);
    ItemMenu_HideAllIcons();
    RenderOutput_RedrawSavedRectFar(menu->status_window);
    {
        s32 delay = 200;

        delay <<= 4;
        Scheduler_AddOrUpdateCallback((s32)(Menu_UpdateEntryObjectTransforms), delay);
    }
    return result;
}

s32 Menu_MoveGridCursor(s32 *arg0, s32 *arg1, s32 arg2)
{
    s32 row;
    s32 col;

    col = *arg0;
    row = *arg1;
    switch (arg2) {
    case 0x40:
        row -= 1;
        if (row < 0) {
            row = 5;
        }
        if (row <= 3) {
            if (row == 3) {
                if (col <= 4) {
                    col = 0;
                } else {
                    goto set_one;
                }
            } else if (col > 1) {
set_one:
                col = 1;
            }
            if ((row == 3) && (col == 1)) {
                row = 2;
            }
        }
        break;
    case 0x80:
        row += 1;
        if (row > 5) {
            row = 0;
        }
        if ((row == 3) && (col == 1)) {
            row = 4;
        }
        if (row == 4) {
            goto set_zero;
        }
        break;
    case 0x20:
        col -= 1;
        if (row == 3) {
            col += 1;
        } else if (row > 3) {
            if (col < 0) {
                col = 7;
            }
        } else if (col < 0) {
            col = 1;
        }
        break;
    case 0x10:
        col += 1;
        if (row == 3) {
            col -= 1;
        } else if (row > 3) {
            if (col > 7) {
                goto set_zero;
            }
        } else if (col > 1) {
            goto set_zero;
        }
        break;
set_zero:
        col = 0;
        break;
    }
    *arg0 = col;
    *arg1 = row;
    return (row * 9) + col;
}
