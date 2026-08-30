#ifndef ALCHEMY_BATTLE_ACTOR_H
#define ALCHEMY_BATTLE_ACTOR_H

#include "types.h"

void *Actor_GetObject(s32 unit);
void Actor_Place(void *object, s32 unit, s32 x, s32 y);
void Actor_Commit(void);
s32 Actor_ListSlots(s16 *entries);
void Actor_ResetMotionAtAnchor(s32 slot);

#endif
