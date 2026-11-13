#ifndef ALCHEMY_MODULE_SCENE_SHAMAN_VILLAGE_CAVE_H
#define ALCHEMY_MODULE_SCENE_SHAMAN_VILLAGE_CAVE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

extern u32 gShamanVillageCave_GladiatorLeaves[];

extern s32 gShamanVillageCave_GladiatorFollowsOut[];

void Field_TryJumpForward();

extern s32 gShamanVillageCave_OvergrowthRests[];
extern u32 gShamanVillageCave_Settle[];

void FieldCamera_WaitForArrival();

#endif
