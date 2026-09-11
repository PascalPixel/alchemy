#include "scene.h"
#include "abi/item_menu/open_category.h"
#include "far_runtime.h"
#include "item_menu.h"

void ItemMenu_OpenCategory(s32 owner_id)
{
    struct ItemMenuState *menu;

    menu = gIw;
    Sys_Run();
    UiWindow_Commit(menu->item_window);
    ItemMenu_DrawCategory(menu->item_window, owner_id, 0);
}
