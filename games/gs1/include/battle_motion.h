#ifndef ALCHEMY_BATTLE_MOTION_H
#define ALCHEMY_BATTLE_MOTION_H

#include "types.h"

void Func_080b82c4(
    s32 actor_id,
    s32 target_id,
    s32 travel_divisor,
    s32 initial_velocity_y);

#define BattleMotion_ApproachTarget Func_080b82c4

#endif
