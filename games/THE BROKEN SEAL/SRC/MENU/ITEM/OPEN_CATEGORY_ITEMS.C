#include "FAR_RUNTIME.H"
#include "INVENTORY_MENU.H"

void ItemMenu_PosCategory(void);

void ItemMenu_OpenCategory(s32 owner_id)
{
    struct InventoryMenuState *menu;

    menu = gMenuWork;
    ItemMenu_PosCategory();
    UiWindow_Commit(menu->item_window);
    InventoryMenu_DrawCategoryItems(menu->item_window, owner_id, 0);
}
