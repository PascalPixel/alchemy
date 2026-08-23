#include "inventory_menu.h"
s32 Modulo(s32, s32);

void InventoryMenu_HidePageStartIcons(void)
{
    struct InventoryMenuState *menu = Data_03001f2c;
    s32 slot = 0;
    s32 hidden_state = 13;
    struct InventoryMenuIcon **icon_slot = menu->entry_icons;

    do {
        struct InventoryMenuIcon *icon = *icon_slot++;

        if (icon != 0 && Modulo(slot, 5) == 0) {
            icon->state = hidden_state;
        }
        slot++;
    } while (slot <= 31);
}
