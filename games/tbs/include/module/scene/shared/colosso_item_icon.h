#ifndef ALCHEMY_MODULE_SCENE_SHARED_COLOSSO_ITEM_ICON_H
#define ALCHEMY_MODULE_SCENE_SHARED_COLOSSO_ITEM_ICON_H

#include "types.h"

// UB: the early calls implicitly declare an int result, but the function
// returns void. Keep that legacy call form for the matching build.
#ifdef UBFIX
void Colosso_ShowItemIcon(s32 actorId, s32 itemId);
#endif

#endif
