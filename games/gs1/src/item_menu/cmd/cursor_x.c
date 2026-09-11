#include "scene.h"
#include "abi/item_menu/cmd/cursor_x.h"
#include "item_menu.h"

extern s32 ItemMenu_CommandCursorXTable[];

s32 ItemMenu_CmdCursorX(s32 column, s32 row)
{
    if (column > 2 || row > 2 || column < 0 || row < 0)
        return 0;
    return ItemMenu_CommandCursorXTable[row * 3 + column];
}
