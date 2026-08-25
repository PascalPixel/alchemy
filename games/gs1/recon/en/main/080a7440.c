#include "types.h"
#include "inventory_menu.h"

#define CharacterSelector_Run Func_080a7440

extern s32 Func_080a77a4(s32 mode);

s32 CharacterSelector_Run(void)
{
    struct InventoryMenuState *menu;
    s32 result;

    menu = Data_03001f2c;
    result = 0;
    menu->selected_slot = result;
    result = Func_080a77a4(0);
    if (result != -1) {
        result = menu->item_owner;
    }
    return result;
}
