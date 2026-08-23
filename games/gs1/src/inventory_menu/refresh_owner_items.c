#include "inventory_menu.h"
#include "owner_state.h"

void UiWindow_Commit(s32 window);
void Func_080a1cb0(s32 mode);
void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);

void InventoryMenu_RefreshOwnerItems(s32 owner_id, s32 mode)
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
