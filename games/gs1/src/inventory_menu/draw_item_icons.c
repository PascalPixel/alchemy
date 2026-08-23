#include "inventory_menu.h"

void InventoryMenu_DrawItemIcons(u16 *items, s32 style) {
    s32 remaining;
    u16 *entries;
    struct InventoryMenuIcon **icons;
    s32 item_id;

    icons = Data_03001f2c->entry_icons;
    entries = items;
    remaining = 14;
    do {
        item_id = *entries++;
        if (item_id != 0) {
            if (style == 0) {
                InventoryMenu_DrawItemIcon(
                    2, item_id, (*icons)->render_target, 0);
            } else {
                InventoryMenu_DrawItemIcon(
                    7, item_id, (*icons)->render_target, 0);
            }
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(items);
}
