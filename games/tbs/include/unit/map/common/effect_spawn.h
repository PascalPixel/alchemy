#ifndef ALCHEMY_UNIT_MAP_COMMON_EFFECT_SPAWN_H
#define ALCHEMY_UNIT_MAP_COMMON_EFFECT_SPAWN_H

#include "types.h"

union ObjectEventOrEffect;

void FieldEffect_UpdateMotionScaleAndSpin(
    union ObjectEventOrEffect *fieldObject);

#endif
