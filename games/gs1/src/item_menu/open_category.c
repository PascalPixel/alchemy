#include "far_runtime.h"
#include "item_menu.h"

void Func_080a9cbc(void);

void ItemMenu_OpenCategory(s32 owner_id)
{
    struct ItemMenuState *menu;

    menu = Data_03001f2c;
    Func_080a9cbc();
    UiWindow_Commit(menu->item_window);
    ItemMenu_DrawCategory(menu->item_window, owner_id, 0);
}
