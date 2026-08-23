#include "inventory_menu.h"

void InventoryMenu_HideAllItemIcons(void) {
    s32 hidden_state = 13;
    struct InventoryMenuIcon **icons = Data_03001f2c->entry_icons;
    s32 slot;

    for (slot = 31; slot >= 0; slot--) {
        struct InventoryMenuIcon *icon = *icons++;
        if (icon != 0) {
            icon->state = hidden_state;
        }
    }
}
