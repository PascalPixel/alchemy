#include "INVENTORY_MENU.H"

extern s32 ItemMenu_CommandColumnXTable[];

s32 ItemMenu_CmdCursorX(s32 column, s32 row)
{
    if (column > 2 || row > 2 || column < 0 || row < 0)
        return 0;
    return ItemMenu_CommandColumnXTable[row * 3 + column];
}
