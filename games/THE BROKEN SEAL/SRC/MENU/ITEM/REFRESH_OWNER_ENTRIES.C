#include "M7_INTERFACES.H"
#include "INVENTORY_MENU.H"

void ItemMenu_RefreshEntry(s32 layout)
{
    struct Object080a1c **slot;
    struct Object080a1c **scan;
    struct Object080a1c *object;
    s32 index;
    s32 origin_y;
    struct InventoryMenuState *menu;

    menu = gMenuWork;
    origin_y = 0x38;
    if (layout != 1) {
        origin_y = 0x28;
    }
    slot = (struct Object080a1c **)menu->entry_icons;
    index = 0;
    scan = slot;
    do {
        object = *scan++;
        if (object != NULL) {
            ItemMenu_PosOwner(slot, index, 0x74, origin_y, 5);
        }
        index++;
        slot++;
    } while (index <= 0xE);
}
