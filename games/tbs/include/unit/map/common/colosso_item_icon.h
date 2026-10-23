#ifndef ALCHEMY_UNIT_MAP_COMMON_COLOSSO_ITEM_ICON_H
#define ALCHEMY_UNIT_MAP_COMMON_COLOSSO_ITEM_ICON_H

#include "types.h"

// UB: the early calls implicitly declare an int result, but the function
// returns void. Keep that legacy call form for the matching build.
#ifdef UBFIX
void Colosso_ShowItemIcon(s32 objectEventId, s32 itemId);
#endif

#endif
