#ifndef ALCHEMY_BATTLE_OBJ_H
#define ALCHEMY_BATTLE_OBJ_H

#include "edition.h"
#include "types.h"

void Actor_Place(void *object, s32 unit, s32 x, s32 y);
#if GAME_TLA
void ObjMotRst(s32 slot);
#endif

#endif
