#include "INVENTORY_MENU.H"
#include "OWNER_STATE.H"


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

    icons = gMenuWork->entry_icons;
    entries = items;
    remaining = 14;
    do {
        item_id = *entries++;
        if (item_id != 0) {
            if (style == 0) {
                Resource_LoadByModeIntoSlotFar(
                    2, item_id, (*icons)->render_target, 0);
            } else {
                Resource_LoadByModeIntoSlotFar(
                    7, item_id, (*icons)->render_target, 0);
            }
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(items);
}


void RenderOutput_RedrawSavedRectFar(s32 window);
void ItemMenu_RefreshEntry(s32 mode);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

void ItemMenu_RefreshOwner(s32 owner_id, s32 mode)
{
    struct InventoryMenuState *menu;
    struct OwnerInventoryState *owner;
    u16 *items;

    menu = gMenuWork;
    owner = Owner_GetStateFar(owner_id);
    items = menu->items;
    menu->item_count = ItemMenu_Collect(owner, items, 0);
    RenderOutput_RedrawSavedRectFar(menu->item_window);
    ItemMenu_RefreshEntry(mode);
    ItemMenu_DrawIcons(items, 0);
    if (ItemMenu_Count(owner_id) == 0)
        UiText_DrawCharacterAtOffsetFar(
            (s32)&MsgItemMenuEmpty, menu->item_window, 8, 24);
}

void InventoryMenu_NoOp(void)
{
}
