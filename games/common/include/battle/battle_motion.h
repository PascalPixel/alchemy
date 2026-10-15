#ifndef ALCHEMY_BATTLE_MOTION_H
#define ALCHEMY_BATTLE_MOTION_H

#include "edition.h"
#include "types.h"

#if GAME_TLA
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travel_divisor,
                                 s32 initial_velocity_y);
#endif

#endif
