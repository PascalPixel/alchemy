#ifndef ALCHEMY_UNIT_MENU_SHOP_WARES_H
#define ALCHEMY_UNIT_MENU_SHOP_WARES_H

#include "types.h"

s32 EventTable_GetRowLimit(void);
void EventTable_ApplyRowAbilities(s32 row_no);
s32 EventTable_CopyRowHeader(s32 row_no, s16 *output);
s32 EventTable_GetRowType(s32 index);

#endif
