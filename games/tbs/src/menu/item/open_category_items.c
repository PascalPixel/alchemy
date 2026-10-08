#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "far_runtime.h"
#include "inventory_menu.h"


void ItemMenu_OpenCategory(s32 owner_id)
{
    struct InventoryMenuState *menu;

    menu = gMenuWork;
    ItemMenu_PosCategory();
    RenderOutput_RedrawSavedRectFar(menu->item_window);
    ItemMenu_DrawCategory((s32)menu->item_window, owner_id, 0);
}
