#include "far_runtime.h"
#include "inventory_menu.h"

void Func_080a9cbc(void);

void InventoryMenu_OpenCategoryItems(s32 owner_id) {
    struct InventoryMenuState *menu;

    menu = Data_03001f2c;
    Func_080a9cbc();
    UiWindow_Commit(menu->item_window);
    InventoryMenu_DrawCategoryItems(menu->item_window, owner_id, 0);
}
