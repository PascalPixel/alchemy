#include "EDITION.H"
#if EDITION_INTERNATIONAL
/* These localization routines have no counterpart in Japanese TBS. */
#include "INVENTORY_MENU.H"

extern s32 ItemMenu_CommandColumnXTable[];

s32 ItemMenu_CmdCursorX(s32 column, s32 row)
{
    if (column > 2 || row > 2 || column < 0 || row < 0)
        return 0;
    return ItemMenu_CommandColumnXTable[row * 3 + column];
}

s32 ItemMenu_CmdCursorY(s32 column, s32 row)
{
    s32 y;

    (void)column;
    y = 0x1E;
    if (row != 0) {
        y = 0x26;
    }
    return y;
}

#endif
