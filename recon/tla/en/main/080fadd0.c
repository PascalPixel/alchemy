#include "OWNER_STATE.H"

void ItemMenu_DrawIcons(u16 *items, s32 style)
{
    s32 remaining;
    u16 *entries;
    struct InventoryMenuIcon **icons;
    s32 item_id;

    icons = gMenuWork->entry_icons;
    entries = items;
    remaining = 14;
    do {
        item_id = *entries++;
        if (item_id != 0) {
            if (style == 0) {
                Resource_LoadByModeIntoSlotFar(
                    2, item_id, (*icons)->render_target, 0);
            } else {
                Resource_LoadByModeIntoSlotFar(
                    7, item_id, (*icons)->render_target, 0);
            }
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(items);
}
