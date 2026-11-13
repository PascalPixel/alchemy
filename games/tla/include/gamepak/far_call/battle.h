#ifndef ALCHEMY_FAR_CALL_BATTLE_H
#define ALCHEMY_FAR_CALL_BATTLE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct TargetPoint;

s32 BattleUnit_GetEffectHeightFar(s32 unit);

void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);
void BattleBackground_SetPaletteLevelFar(s32 background, s32 level);

void *GetMotionRecordFar(void *object, s32 index);
s32 BattleMotion_ProjectConditionalPositionFar(s32 unit,
                                               struct TargetPoint *point);

#endif
