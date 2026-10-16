#ifndef ALCHEMY_OWNER_VALUE_H
#define ALCHEMY_OWNER_VALUE_H

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

#if GAME_TLA
s32 Unit_AddHp(s32 owner, s32 delta);
s32 Unit_AddHpFar(s32 owner, s32 delta);
#endif

#endif
