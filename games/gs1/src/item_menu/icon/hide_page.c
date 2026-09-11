#include "scene.h"
#include "abi/item_menu/icon/hide_page.h"
#include "item_menu.h"
s32 Modulo(s32, s32);

void ItemMenu_HidePageIcons(void)
{
    struct ItemMenuState *menu = gIw;
    s32 slot = 0;
    s32 hidden_state = 13;
    struct ItemMenuIcon **icon_slot = menu->entry_icons;

    do {
        struct ItemMenuIcon *icon = *icon_slot++;

        if (icon != 0 && Modulo(slot, 5) == 0) {
            icon->state = hidden_state;
        }
        slot++;
    } while (slot <= 31);
}
