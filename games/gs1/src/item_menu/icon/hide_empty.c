#include "scene.h"
#include "abi/item_menu/icon/hide_empty.h"
#include "item_menu.h"

void Menu_HideEmptyEntryIcons(const u16 *items)
{
    struct ItemMenuState *menu = gIw;
    s32 slot;

    for (slot = 0; slot < 32; slot++) {
        if (items[slot] == 0) {
            Sys_Do(menu->entry_icons[slot]);
            menu->entry_icons[slot]->state = 13;
        }
    }
}
