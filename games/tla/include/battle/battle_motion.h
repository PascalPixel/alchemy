#ifndef ALCHEMY_BATTLE_MOTION_H
#define ALCHEMY_BATTLE_MOTION_H

/// @file
/// A unit's leap toward its target.

#include "edition.h"
#include "types.h"

void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelFrames,
                                 s32 initialVelocityY);

#endif
