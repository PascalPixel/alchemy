#include "m7_interfaces.h"
#include "global_cells.h"

void InventoryMenu_RefreshOwnerEntries(s32 layout) {
    struct Object080a1c **slot;
    struct Object080a1c **scan;
    struct Object080a1c *object;
    s32 index;
    s32 origin_y;
    s32 base;

    base = *(s32 *)ADDR_03001F2C;
    origin_y = 0x38;
    if (layout != 1) {
        origin_y = 0x28;
    }
    slot = (struct Object080a1c **)(base + 0x48);
    index = 0;
    scan = slot;
    do {
        object = *scan++;
        if (object != NULL) {
            InventoryMenu_PositionOwnerEntry(slot, index, 0x74, origin_y, 5);
        }
        index++;
        slot++;
    } while (index <= 0xE);
}
