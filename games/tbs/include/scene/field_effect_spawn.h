#ifndef ALCHEMY_FIELD_EFFECT_SPAWN_H
#define ALCHEMY_FIELD_EFFECT_SPAWN_H

/* The small effects twenty-four scenes scatter, each moving at its own
   velocity, growing and spinning (shared/field_effect_spawn.c). */

#include "types.h"

struct ACTOR;

/* Moves, scales and spins a small effect by one tick. */
void scFieldEffectUpdateMotionScaleAndSpin(struct ACTOR *actor);

/* A random spin for a field effect, in 1/65536 turns a tick: up to a
   thirty-second of a turn either way, as the effect's Uint16 spin wraps it. */
#define FIELD_EFFECT_RANDOM_SPIN() (syRandomBelow(4096) + (Uint16)-2048)

#endif
