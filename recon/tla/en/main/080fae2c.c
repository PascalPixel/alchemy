#include "owner_state.h"
#include "types.h"
#include "item.h"
#include "iwram_call.h"

void ItemMenu_RefreshOwner(s32 owner_id, s32 mode)
{
    struct InventoryMenuState *menu;
    struct OwnerInventoryState *owner;
    u16 *items;

    menu = gMenuWork;
    owner = UnitGetFar(owner_id);
    items = menu->items;
    menu->item_count = ItemMenu_Collect(owner, items, 0);
    RenderOutput_RedrawSavedRectFar(menu->item_window);
    ItemMenu_RefreshEntry(mode);
    ItemMenu_DrawIcons(items, 0);
    if (ItemMenu_Count(owner_id) == 0)
        UiText_DrawCharacterAtOffsetFar(
            (s32)&MsgItemMenuEmpty, menu->item_window, 8, 24);
}
