#include "scene.h"
#include "abi/item_menu/draw/icons.h"
#include "item_menu.h"

void ItemMenu_DrawIcons(u16 *items, s32 style)
{
    s32 remaining;
    u16 *entries;
    struct ItemMenuIcon **icons;
    s32 item_id;

    icons = gIw->entry_icons;
    entries = items;
    remaining = 14;
    do {
        item_id = *entries++;
        if (item_id != 0) {
            if (style == 0) {
                ItemMenu_DrawIcon(
                    2, item_id, (*icons)->render_target, 0);
            } else {
                ItemMenu_DrawIcon(
                    7, item_id, (*icons)->render_target, 0);
            }
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(items);
}
