#include "FAR_RUNTIME.H"
#include "INVENTORY_MENU.H"

void Func_080a9cbc(void);

void ItemMenu_OpenCategory(s32 owner_id)
{
    struct InventoryMenuState *menu;

    menu = Data_03001f2c;
    Func_080a9cbc();
    UiWindow_Commit(menu->item_window);
    InventoryMenu_DrawCategoryItems(menu->item_window, owner_id, 0);
}
