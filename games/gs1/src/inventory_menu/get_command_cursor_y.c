#include "inventory_menu.h"

s32 InventoryMenu_GetCommandCursorY(s32 column, s32 row) {
    s32 y;

    (void)column;
    y = 0x1E;
    if (row != 0) {
        y = 0x26;
    }
    return y;
}
