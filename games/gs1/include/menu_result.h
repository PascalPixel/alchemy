#ifndef ALCHEMY_MENU_RESULT_H
#define ALCHEMY_MENU_RESULT_H

#include "types.h"

struct MenuResult {
    s32 owner_state;
    s32 unused_04;
    s32 page;
    s32 page_count;
    s32 row;
    s32 entry_count;
    s32 selected_index;
};

s32 Func_080a8b8c(struct MenuResult *, s32);

#endif
