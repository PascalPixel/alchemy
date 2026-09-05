#include "inventory_menu.h"

s32 InventoryMenu_IsSpecialItemRange(s32 item_id) {
    s32 result;
    s32 first;

    if (item_id > 0xC4) {
        goto L0;
    }
    first = 0xC1;
    if (item_id < first) {
        goto L0;
    }
    result = 1;
    return result;
L0:
    result = 0;
    return result;
}
