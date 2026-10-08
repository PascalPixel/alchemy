#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "item.h"
#include "edition.h"
#include "types.h"
#include "io_reg.h"
#include "scene.h"
#include "layout_guard.h"
#include "global_cells.h"
#include "fixed_math.h"
#include "system.h"
#include "ui.h"
#include "inventory_menu.h"
#include "tbs_edition.h"
#include "btltype.h"
#include "btlsys.h"
#include "menu_result.h"
#include "text/msg_ids.h"
s32 Item_CanOwnerEquip(s32 owner, s32 item);
s32 GameFlag_TestFar(s32 flag);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *state, s32 flags);

#define RenderOutput_ClearListFar ((void (*)(void *))RenderOutput_ClearListFar)
#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

#define UiWindow_ClearInteriorTilesFar ((void (*)(s32,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)

void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
s32 Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);

void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused0, s32 unused1);
void AudioCommand_PlayFar(s32 cue);
#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32 gFrameCount;
#endif

#define LIST_PAGE_SIZE 5

void Resource_LoadPairedBlocks(void)
{
    struct InventoryMenuState *state = gMenuWork;
    s32 value = Resource_FindFreeEntry();

    state->resource_slots[0] = value;
    VramBlock_LoadCached(value, 128, Menu_ArrowTiles);
    value = Resource_FindFreeEntry();
    state->resource_slots[1] = value;
    VramBlock_LoadCached(value, 128, Menu_SmallDownArrowTiles);
}

s32 ItemMenu_PageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    struct InventoryMenuState *menu = gMenuWork;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    limit = ItemMenu_Count(menu->pane_owner[index]);
    encoded = (s32)UnitGetFar(menu->pane_owner[index]);
    value = menu->selected_index_by_owner[menu->pane_owner[index]];
    if ((value + 1) > limit) {
        value = limit - 1;
    }
    quotient = value / 5;
    remainder = value % 5;
    groups = limit / 5;
    if (limit % 5 != 0) {
        groups++;
    }
    result->owner_state = encoded;
    result->page = quotient;
    result->page_count = groups;
    result->row = remainder;
    result->entry_count = limit;
    result->selected_index = value;
    return 1;
}

s32 ItemMenu_DrawItemDetailPage(s32 arg0, void *arg1, struct MenuResult *state)
{
    struct InventoryMenuState *menu;
    s32 page;
    s32 combined;
    s32 row;

    page = state->page;
    menu = gMenuWork;
    combined = page * 5;
    combined += state->row;
    state->selected_index = combined;

#if EDITION_INTERNATIONAL
    RenderOutput_RedrawSavedRectFar(menu->info_window);
#else
    RenderOutput_ClearListFar(menu->info_window);
#endif
    WaitFrames(1);

    combined = state->selected_index;
    if (menu->items[combined] != 0) {
        s32 masked = (menu->items[combined] & ITEM_ID_MASK) + (s32)MsgItemPlainName;
#if EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffsetFar(masked, (s32)menu->info_window, 0, 0);
#else
        UiText_DrawResourceFar(masked, (s32)menu->info_window, 0, 0);
#endif
    }

    row = 0;
    do {
        if (row == state->row) {
            Render_SetTilemapFlagRect((s32)menu->item_window, 1, row * 2 + 1, 14, 1, 14);
        } else {
            Render_SetTilemapFlagRect((s32)menu->item_window, 1, row * 2 + 1, 14, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}

#if EDITION_INTERNATIONAL
#define PAGE_X  116
#define ENTRY_X 24
#else
#define PAGE_X  120
#define ENTRY_X 32
#endif

s32 ItemMenu_DrawNamePage(
    s32 window,
    s32 unused,
    const struct MenuResult *state)
{
    struct InventoryMenuState *menu =
        gMenuWork;
    u32 page;
    u32 first_entry;
    u32 visible_count;
    u8 row;
    const u16 *item_id;

    (void)unused;

    RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
    UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

    page = state->page;
    first_entry = page * 5;
    visible_count = (u8)(state->entry_count - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

    Menu_SetPageIcons(5, first_entry, window, PAGE_X, 34);
    Menu_DrawPageIndicator(window, state->entry_count, 5, state->page, 15);

    row = 0;
    if (visible_count > row) {
        item_id = &menu->items[first_entry];
        do {
            UiText_DrawCharacterAtOffsetFar(
                (item_id[0] & ITEM_ID_MASK) + (s32)MsgItemName,
                (s32)menu->item_window,
                ENTRY_X,
                row * 16 + 8
            );
            row++;
            item_id++;
        } while (visible_count > row);
    }

    return 1;
}

s32 InventoryMenu_ItemNamePageReturnTrue(void)
{
    return 1;
}

void InventoryMenu_ItemNamePageNoOp(void)
{
}

s32 ItemMenu_RunList(s32 pane)
{
    struct InventoryMenuState *menu;
    s32 window;
    s32 changed;
    s32 nav;
    s32 tab;
    u8 i;
    s32 result;
    s32 redraw;
    s32 done;
    struct BtlUnit *owner;
    s32 prev;
    struct RenderOutput *icon;
    s32 work[5];
    struct MenuResult state;

    menu = gMenuWork;
    result = 0;
    prev = 0;
    menu->equip_preview = result;
    UiWindow_UpdateOrCreate((s32 *)&menu->list_window, 13, 3, 17, 14, 2);
    window = (s32)menu->list_window;
    done = 0;

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        owner = UnitGetFar(menu->pane_owner[pane]);
        menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        menu->selected_item_icon->active = 13;
        ItemMenu_PageResult(&state, pane);
        UiMenu_PositionCursor(98, state.selected_index * 16 + 36);
        changed = 1;
        redraw = 1;

        while (GameFlag_TestFar(0x150) == 0) {
            UiMenu_PositionCursor(98, state.row * 16 + 36);

            if (changed != 0) {
                changed = 0;
                if (owner->inventory[prev] != 0) {
                    icon = menu->entry_icons[prev];
                    UiIcon_PrepareObject(icon);
                }
                if (redraw != 0) {
                    WaitFrames(1);
                    ItemMenu_DrawNamePage(window, 0, &state);
                    if (pane == 0) {
                        UiText_DrawCharacterAtOffsetFar((s32)MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
                    }
                    redraw = 0;
                }
                ItemMenu_DrawItemDetailPage(window, work, &state);
                menu->selected_items[pane] = menu->items[state.selected_index];
                ItemMenu_DrawEquipPreview(menu->pane_owner[pane], state.selected_index, 0, menu->pane_owner[pane]);
                if (menu->items[state.selected_index] != 0) {
                    icon = menu->entry_icons[state.selected_index];
                    icon->active = 9;
                    icon->anim_step = 0;
                    icon->sentinel = 250;
                }
                for (i = 0; i < menu->party_count; i++) {
                    AnimationObjects_SelectAnimationFar(menu->owner_objects[i], 1);
                }
            }

            if ((gFrameCount & 31) == 0) {
                for (i = 0; i < menu->party_count; i++) {
                    if (Item_CanOwnerEquip(menu->owner_ids[i],
                            ITEM_ID_MASK & menu->items[state.selected_index]) != 0) {
                        AnimationObjects_SelectAnimationFar(menu->owner_objects[i], 3);
                    }
                }
            }

            WaitFrames(1);
            prev = state.selected_index;
            nav = Menu_HandlePageInput(0, state.entry_count, LIST_PAGE_SIZE,
                &state.row, &state.page);
            if (nav == 1) {
                redraw = 1;
                changed = 1;
            }
            if (nav == 0) {
                changed = 1;
            }
            if (nav == -1) {
                changed = 0;
            }

            if ((gKeyState & KEY_A) != 0
                && menu->items[state.selected_index] != 0) {
                AudioCommand_PlayFar(173);
                result = menu->items[state.selected_index];
                done = 1;
                break;
            }
            if ((gKeyState & KEY_B) != 0) {
                AudioCommand_PlayFar(113);
                result = -1;
                done = 1;
                break;
            }
            if ((gKeysRepeat & KEY_R) != 0
                || (gKeysRepeat & KEY_L) != 0) {
                if (pane == 1) {
                    AudioCommand_PlayFar(114);
                    WaitFrames(1);
                } else {
                    AudioCommand_PlayFar(111);
                    menu->selected_index_by_owner[menu->pane_owner[pane]] =
                        state.selected_index;
                    tab = menu->pane_index[pane];
                    do {
                        if ((gKeysRepeat & KEY_R) != 0) {
                            tab++;
                        } else {
                            tab--;
                        }
                        tab = (tab + menu->party_count) % menu->party_count;
                        menu->selected_owner = menu->owner_ids[tab];
                        menu->pane_owner[pane] = menu->owner_ids[tab];
                        menu->pane_index[pane] = tab;
                        menu->item_count = ItemMenu_Collect(
                            UnitGetFar(menu->pane_owner[pane]), menu->items, 0);
                    } while (menu->item_count == 0);
                    for (i = 0; i <= 3; i++) {
                        menu->owner_y[i] = 30;
                    }
                    menu->owner_y[tab] = 26;
                    break;
                }
            }
        }
    }

    UiWindow_ClearInteriorTilesFar(window, 0, 88, 120, 96);
    UiIcon_PrepareObject(menu->grid_cursor);
    menu->selected_slots[pane] = state.selected_index;
    menu->selected_index_by_owner[menu->pane_owner[pane]] = state.selected_index;
    menu->selected_items[pane] = result;
    if (GameFlag_TestFar(0x150) != 0) {
        result = -1;
    }
    WaitFrames(1);
    return result;
}
