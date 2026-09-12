#include "inventory_menu.h"

void Func_080a17c4(void *icon);

void Menu_HideEmptyEntryIcons(const u16 *items)
{
    struct InventoryMenuState *menu = Data_03001f2c;
    s32 slot;

    for (slot = 0; slot < 32; slot++) {
        if (items[slot] == 0) {
            Func_080a17c4(menu->entry_icons[slot]);
            menu->entry_icons[slot]->state = 13;
        }
    }
}
