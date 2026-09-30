#include "OWNER_STATE.H"
#include "TYPES.H"
#include "ITEM.H"
#include "IWRAM_CALL.H"

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
