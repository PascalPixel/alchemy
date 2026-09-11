#include "scene.h"
#include "abi/item_menu/icon/hide_all.h"
#include "item_menu.h"

void ItemMenu_HideAllIcons(void)
{
    s32 hidden_state = 13;
    struct ItemMenuIcon **icons = gIw->entry_icons;
    s32 slot;

    for (slot = 31; slot >= 0; slot--) {
        struct ItemMenuIcon *icon = *icons++;
        if (icon != 0) {
            icon->state = hidden_state;
        }
    }
}
