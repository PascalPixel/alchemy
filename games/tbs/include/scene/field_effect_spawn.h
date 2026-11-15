#ifndef ALCHEMY_FIELD_EFFECT_SPAWN_H
#define ALCHEMY_FIELD_EFFECT_SPAWN_H

#include "types.h"

struct Actor;

void FieldEffect_UpdateMotionScaleAndSpin(struct Actor *actor);

/// A random spin for a field effect, in 1/65536 turns a frame: up to a
/// thirty-second of a turn either way, as the effect's u16 spin wraps it.
#define FIELD_EFFECT_RANDOM_SPIN() ((Random_Below(4096)) + 0xf800)

#endif
