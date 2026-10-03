#include "EDITION.H"
#include "TYPES.H"
#include "SCENE.H"
#include "LAYOUT_GUARD.H"
#include "GLOBAL_CELLS.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "UI.H"
#include "INVENTORY_MENU.H"
#include "TBS_EDITION.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_RUNTIME.H"
#include "MENU_RESULT.H"

extern u8 Data_080aebcc[];
extern u8 Data_080aeb4c[];
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32, s32, const u8 *);

/* menu/item_menu/page_result.c */

extern u8 MsgItemPlainName;
void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
void RenderOutput_ClearListFar(void *window);
void UiText_DrawMessageAt(s32, s32, s32, s32);
s32 Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);
extern u8 MsgItemName;
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 width, s32 height, s32 style);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Menu_DrawPageIndicator(s32 window, s32 count, s32 page_size, s32 page, s32 style);
void Menu_SetPageIcons(s32 page_size, s32 first_entry, s32 window, s32 x, s32 y);
s32 GameFlag_IsSet(s32 message);
void Object_InitializeMode(s32 object, s32 mode);
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;

void AnimationObjects_SelectAnimationFar(s32 object, s32 mode);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
struct BattleAction *BattleAction_Get(s32 action);
s32 GameFlag_TestFar(s32 message);
void UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused0, s32 unused1);
void UiIcon_PrepareObject(struct InventoryMenuIcon *icon);
void PsynergyMenu_CallIconRoutineWithValue(void *work, s32 value);
void UiMenu_PositionCursor(s32 x, s32 y);
void Audio_PlayCue(s32 cue);
#define KEY_A 1
#define KEY_B 2
#define KEY_SELECT 4
#define KEY_R 0x100
#define KEY_L 0x200
extern u32 gFrameCount;
extern u8 MsgChangeCharacterHelp;

/* Takes a fourth argument; this caller passes the owner there as well. */
s32 Item_CanOwnerEquip(s32 owner, s32 item);
#define ITEM_ID_MASK 0x1ff
#define LIST_PAGE_SIZE 5
s32 ItemMenu_DrawItemDetailPage(s32 arg0, void *arg1, struct MenuResult *state);

void Resource_LoadPairedBlocks(void)
{
    struct InventoryMenuState *state = gMenuWork;
    s32 value = Resource_FindFreeEntry();

    state->resource_slots[0] = value;
    VramBlock_LoadCached(value, 128, Data_080aebcc);
    value = Resource_FindFreeEntry();
    state->resource_slots[1] = value;
    VramBlock_LoadCached(value, 128, Data_080aeb4c);
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
    encoded = (s32)Owner_GetStateFar(menu->pane_owner[index]);
    value = menu->selected_index_by_owner[menu->pane_owner[index]];
    if ((s32)(value + 1) > limit) {
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
        s32 masked = (menu->items[combined] & 0x1ff) + (s32)&MsgItemPlainName;
#if EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffsetFar(masked, (s32)menu->info_window, 0, 0);
#else
        UiText_DrawMessageAt(masked, (s32)menu->info_window, 0, 0);
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
                (item_id[0] & 0x1ff) + (s32)&MsgItemName,
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

/* An empty routine after the name page; nothing in the ROM refers to it. */
void InventoryMenu_ItemNamePageNoOp(void)
{
}

/*
 * types.h already supplies WaitFrames, __modsi3, Audio_PlayCue, GameFlag_IsSet,
 * Ability_GetData, UiText_DrawCharacterAtOffsetFar, UiIcon_PrepareObject and
 * Object_InitializeMode; only the names it does not carry are declared here.
 */

/* The item list loop, the counterpart of PsynergyMenu_RunList: browse the
   owner's items, cycle owners with L and R in the first pane, and return
   the chosen item or -1. */
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
    struct BattleUnit *owner;
    s32 prev;
    struct InventoryMenuIcon *icon;
    s32 work[5];
    struct MenuResult state;

    menu = gMenuWork;
    result = 0;
    prev = 0;
    menu->equip_preview = result;
    UiWindow_UpdateOrCreate((s32 *)&menu->list_window, 13, 3, 17, 14, 2);
    window = (s32)menu->list_window;
    done = 0;

    while (done == 0 && GameFlag_IsSet(0x150) == 0) {
        owner = Owner_GetStateFar(menu->pane_owner[pane]);
        menu->item_count = (u8)ItemMenu_Collect(owner, menu->items, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        menu->selected_item_icon->state = 13;
        ItemMenu_PageResult(&state, pane);
        UiMenu_PositionCursor(98, state.selected_index * 16 + 36);
        changed = 1;
        redraw = 1;

        while (GameFlag_IsSet(0x150) == 0) {
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
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
                    }
                    redraw = 0;
                }
                ItemMenu_DrawItemDetailPage(window, work, &state);
                menu->selected_items[pane] = menu->items[state.selected_index];
                ItemMenu_DrawEquipPreview(menu->pane_owner[pane], state.selected_index, 0, menu->pane_owner[pane]);
                if (menu->items[state.selected_index] != 0) {
                    icon = menu->entry_icons[state.selected_index];
                    icon->state = 9;
                    icon->unknown_0c = 0;
                    icon->sentinel = 250;
                }
                for (i = 0; i < menu->party_count; i++) {
                    Object_InitializeMode((s32)menu->owner_objects[i], 1);
                }
            }

            if ((gFrameCount & 31) == 0) {
                for (i = 0; i < menu->party_count; i++) {
                    if (Item_CanOwnerEquip(menu->owner_ids[i],
                            ITEM_ID_MASK & menu->items[state.selected_index]) != 0) {
                        Object_InitializeMode((s32)menu->owner_objects[i], 3);
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
                Audio_PlayCue(173);
                result = menu->items[state.selected_index];
                done = 1;
                break;
            }
            if ((gKeyState & KEY_B) != 0) {
                Audio_PlayCue(113);
                result = -1;
                done = 1;
                break;
            }
            if ((gKeysRepeat & KEY_R) != 0
                || (gKeysRepeat & KEY_L) != 0) {
                if (pane == 1) {
                    Audio_PlayCue(114);
                    WaitFrames(1);
                } else {
                    Audio_PlayCue(111);
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
                        menu->item_count = (u8)ItemMenu_Collect(
                            Owner_GetStateFar(menu->pane_owner[pane]), menu->items, 0);
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
    if (GameFlag_IsSet(0x150) != 0) {
        result = -1;
    }
    WaitFrames(1);
    return result;
}
