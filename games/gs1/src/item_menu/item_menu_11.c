#include "scene.h"
#include "item_menu.h"
#include "far_runtime.h"
#include "owner_state.h"
#include "types.h"
#include "item.h"
#include "gs1_edition.h"

/* item_menu/is_special.c */
s32 ItemMenu_IsSpecial(s32 item_id)
{
    s32 result;
    s32 first;

    if (item_id > 0xC4) {
        goto L0;
    }
    first = 0xC1;
    if (item_id < first) {
        goto L0;
    }
    result = 1;
    return result;
L0:
    result = 0;
    return result;
}

/* item_menu/draw/msg.c */
s32 UiText_DrawAt(s32, s32, s32, s32);

void ItemMenu_DrawMsg(s32 unused, s32 message)
{
    struct ItemMenuState *menu;

    menu = gIw;
    UiWindow_Commit(menu->message_window);
    UiText_DrawAt(message, menu->message_window, 0, 0);
}

/* item_menu/icon/hide_empty.c */
void Menu_HideEmptyEntryIcons(const u16 *items)
{
    struct ItemMenuState *menu = gIw;
    s32 slot;

    for (slot = 0; slot < 32; slot++) {
        if (items[slot] == 0) {
            Sys_Do(menu->entry_icons[slot]);
            menu->entry_icons[slot]->state = 13;
        }
    }
}

/* item_menu/count.c */
s32 ItemMenu_Count(s32 owner_id)
{
    s32 item_id;
    s32 remaining;
    s32 count;
    u16 *slots;

    count = 0;
    slots = OwnerState_GetFar(owner_id)->inventory;
    remaining = 0xE;
    do {
        item_id = 0x1FF & *slots;
        slots += 1;
        if (item_id != 0) {
            count += 1;
        }
        remaining -= 1;
    } while (remaining >= 0);
    return count;
}

/* item_menu/collect.c */
s32 ItemMenu_Collect(struct OwnerInventoryState *owner, u16 *items)
{
    s32 count;
    s32 i;

    for (i = 0; i < 32; i++) {
        items[i] = 0;
    }
    count = 0;
    for (i = 0; i < 15; i++) {
        items[i] = 0;
        if (owner->inventory[i] != 0) {
            items[count] = owner->inventory[i];
            count++;
        }
    }
    return count;
}

/* item_menu/draw/icons.c */
void ItemMenu_DrawIcons(u16 *items, s32 style)
{
    s32 remaining;
    u16 *entries;
    struct ItemMenuIcon **icons;
    s32 item_id;

    icons = gIw->entry_icons;
    entries = items;
    remaining = 14;
    do {
        item_id = *entries++;
        if (item_id != 0) {
            if (style == 0) {
                ItemMenu_DrawIcon(
                    2, item_id, (*icons)->render_target, 0);
            } else {
                ItemMenu_DrawIcon(
                    7, item_id, (*icons)->render_target, 0);
            }
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(items);
}

/* item_menu/refresh_owner.c */
void UiWindow_Commit(s32 window);

void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);

void ItemMenu_RefreshOwner(s32 owner_id, s32 mode)
{
    struct ItemMenuState *menu;
    struct OwnerInventoryState *owner;
    u16 *items;

    menu = gIw;
    owner = OwnerState_GetFar(owner_id);
    items = menu->items;
    menu->item_count = ItemMenu_Collect(owner, items, 0);
    UiWindow_Commit(menu->item_window);
    Sys_Do(mode);
    ItemMenu_DrawIcons(items, 0);
    if (ItemMenu_Count(owner_id) == 0)
        UiText_DrawAt(
            (s32)&ItemMenu_EmptyMsg, menu->item_window, 8, 24);
}

void ItemMenu_NoOp(void)
{
}

/* item_menu/cmd/cursor_x.c */
extern s32 ItemMenu_CommandCursorXTable[];

s32 ItemMenu_CmdCursorX(s32 column, s32 row)
{
    if (column > 2 || row > 2 || column < 0 || row < 0)
        return 0;
    return ItemMenu_CommandCursorXTable[row * 3 + column];
}

/* item_menu/cmd/cursor_y.c */
s32 ItemMenu_CmdCursorY(s32 column, s32 row)
{
    s32 y;

    (void)column;
    y = 0x1E;
    if (row != 0) {
        y = 0x26;
    }
    return y;
}

/* item_menu/cmd/states.c */
#define COMMAND_DISABLED (-1)
#define COMMAND_AVAILABLE 1

void ItemMenu_BuildCmd(s8 *command_states)
{
    struct ItemMenuState *menu;
    struct ItemDefinition *item;

    menu = gIw;
    item = Item_Get(0x1ff & menu->selected_item);

    if (item->type == 0) {
        command_states[0] = COMMAND_AVAILABLE;
        command_states[1] = COMMAND_DISABLED;
    } else {
        command_states[0] = COMMAND_DISABLED;
        command_states[1] = COMMAND_AVAILABLE;
    }

    if (Sys_Apply(menu->item_owner, menu->selected_item) != -1)
        command_states[0] = COMMAND_AVAILABLE;
    else
        command_states[0] = COMMAND_DISABLED;

    if (menu->selected_item & 0x400)
        command_states[0] = COMMAND_DISABLED;

    if (Item_CanOwnerEquip(
            menu->item_owner,
            menu->selected_item & 0x1ff) == 0) {
        command_states[1] = COMMAND_DISABLED;
    }

    command_states[3] = COMMAND_AVAILABLE;
    command_states[5] = COMMAND_AVAILABLE;
    command_states[2] = COMMAND_AVAILABLE;

    if (menu->selected_item & 0x200) {
        command_states[4] = COMMAND_AVAILABLE;
        command_states[1] = COMMAND_DISABLED;
    } else {
        command_states[4] = COMMAND_DISABLED;
    }

    if (item->flags & 2) {
        command_states[4] = COMMAND_DISABLED;
        if (menu->selected_item & 0x200) {
            command_states[3] = COMMAND_DISABLED;
            command_states[5] = COMMAND_DISABLED;
        }
    }

    if (Sys_Check(menu->selected_item & 0x1ff) != 0)
        command_states[0] = COMMAND_AVAILABLE;

    if (menu->party_count <= 1)
        command_states[3] = COMMAND_DISABLED;

    if (item->flags & 8)
        command_states[5] = COMMAND_DISABLED;
}

/* item_menu/cmd/labels.c */
#if defined(GS1_EDITION_JA)
#define ITEM_TEXT_X 0x28
#else
#define ITEM_TEXT_X 0x20
#endif

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void UiText_DrawAt(s32, s32, s32, s32);
extern char Value_00000b33;
#define ItemMenu_CommandUseMessage Value_00000b33

void ItemMenu_DrawCmd(void *command_states, s32 window)
{
    s32 disabled;
    s32 value;
    u32 message;

    UiPalette_SetColor(0xf);
    value = FIELD(command_states, s8 *, 0);
    disabled = -1;
    if (value == disabled)
        UiPalette_SetColor(0xe);

    message = (u32)&ItemMenu_CommandUseMessage;
    UiText_DrawAt(message, window, 0, 0x18);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 1) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 1, window, ITEM_TEXT_X, 0x18);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 3) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 2, window, 0, 0x20);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 5) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 3, window, 0x50, 0x20);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 2) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 4, window, 0x50, 0x18);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 4) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 5, window, ITEM_TEXT_X, 0x20);
    UiPalette_SetColor(0xf);
}
