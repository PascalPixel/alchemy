#include "types.h"
#include "item_menu.h"
#include "global_cells.h"

#define CharacterSelector_Run Func_080a7440

extern s32 Func_080a77a4(s32 mode);

s32 CharacterSelector_Run(void)
{
    struct ItemMenuState *menu;
    s32 result;

    menu = *(struct ItemMenuState **)ADDR_03001F2C;
    menu->selected_slot = 0;
    result = Func_080a77a4(0);
    if (result != -1) {
        result = menu->item_owner;
    }
    return result;
}
