#include "inventory_menu.h"
#include "owner_state.h"

#define ItemMenu_Collect Func_080a3ddc

s32 ItemMenu_Collect(struct OwnerInventoryState *owner, u16 *items, s32 mode)
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


void ItemMenu_DrawIcons(u16 *items, s32 style)
{
    s32 remaining;
    u16 *entries;
    struct InventoryMenuIcon **icons;
    s32 item_id;

    icons = Data_03001f2c->entry_icons;
    entries = items;
    remaining = 14;
    do {
        item_id = *entries++;
        if (item_id != 0) {
            if (style == 0) {
                InventoryMenu_DrawItemIcon(
                    2, item_id, (*icons)->render_target, 0);
            } else {
                InventoryMenu_DrawItemIcon(
                    7, item_id, (*icons)->render_target, 0);
            }
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(items);
}


void UiWindow_Commit(s32 window);
void Func_080a1cb0(s32 mode);
void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);

void ItemMenu_RefreshOwner(s32 owner_id, s32 mode)
{
    struct InventoryMenuState *menu;
    struct OwnerInventoryState *owner;
    u16 *items;

    menu = Data_03001f2c;
    owner = OwnerState_GetFar(owner_id);
    items = menu->items;
    menu->item_count = InventoryMenu_CollectItems(owner, items, 0);
    UiWindow_Commit(menu->item_window);
    Func_080a1cb0(mode);
    InventoryMenu_DrawItemIcons(items, 0);
    if (InventoryMenu_CountItems(owner_id) == 0)
        UiText_DrawAt(
            (s32)&InventoryMenu_EmptyMessage, menu->item_window, 8, 24);
}

void InventoryMenu_NoOp(void)
{
}
