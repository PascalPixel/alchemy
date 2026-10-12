#ifndef ALCHEMY_OWNERVAL_H
#define ALCHEMY_OWNERVAL_H

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

#if GAME_TLA
s32 Owner_AdjustFirstValue(s32 owner, s32 delta);
#endif
#if GAME_TLA
s32 Owner_AdjustFirstValueFar(s32 owner, s32 delta);
#endif

#endif
