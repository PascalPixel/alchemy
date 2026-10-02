#include "FAR_RUNTIME.H"
#include "INVENTORY_MENU.H"

void ItemMenu_PosCategory(void);

void ItemMenu_OpenCategory(s32 owner_id)
{
    struct InventoryMenuState *menu;

    menu = gMenuWork;
    ItemMenu_PosCategory();
    RenderOutput_RedrawSavedRectFar(menu->item_window);
    ItemMenu_DrawCategory((s32)menu->item_window, owner_id, 0);
}
