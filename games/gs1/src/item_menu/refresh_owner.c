#include "scene.h"
#include "abi/item_menu/refresh_owner.h"
#include "item_menu.h"
#include "owner_state.h"

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
