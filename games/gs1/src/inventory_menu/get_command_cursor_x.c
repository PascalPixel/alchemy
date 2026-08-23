#include "inventory_menu.h"

extern s32 Data_080af2e4[];
#define InventoryMenu_CommandCursorXTable Data_080af2e4

s32 InventoryMenu_GetCommandCursorX(s32 column, s32 row)
{
    if (column > 2 || row > 2 || column < 0 || row < 0)
        return 0;
    return InventoryMenu_CommandCursorXTable[row * 3 + column];
}
