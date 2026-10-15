#ifndef ALCHEMY_MENU_RESULT_H
#define ALCHEMY_MENU_RESULT_H

#include "unit/menu/menu.h"
#include "types.h"

struct MenuResult
{
    s32 ownerState;
    s32 unused04;
    s32 page;
    s32 pageCount;
    s32 row;
    s32 entryCount;
    s32 selectedIndex;
};

#endif
