#include "INVENTORY_MENU.H"

extern s32 ItemMenu_CommandColumnXTable[];
#define InventoryMenu_CommandCursorXTable ItemMenu_CommandColumnXTable

s32 ItemMenu_CmdCursorX(s32 column, s32 row)
{
    if (column > 2 || row > 2 || column < 0 || row < 0)
        return 0;
    return InventoryMenu_CommandCursorXTable[row * 3 + column];
}
