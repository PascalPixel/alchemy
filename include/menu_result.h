#ifndef ALCHEMY_MENU_RESULT_H
#define ALCHEMY_MENU_RESULT_H

#include "types.h"

struct MenuResult {
    s32 value0;
    s32 unused4;
    s32 value8;
    s32 valueC;
    s32 value10;
    s32 value14;
    s32 value18;
};

s32 Func_080a5578(struct MenuResult *, s32);
s32 Func_080a6a00(struct MenuResult *, s32);
s32 Func_080a8b8c(struct MenuResult *, s32);

#endif
