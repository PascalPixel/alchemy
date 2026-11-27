#ifndef ALCHEMY_FIELD_EFFECT_SPAWN_H
#define ALCHEMY_FIELD_EFFECT_SPAWN_H

/* @file */
/* The small effects twenty-four scenes scatter, each moving at its own */
/* velocity, growing and spinning (shared/field_effect_spawn.c). */

#include "types.h"

struct ACTOR;

/* Moves, scales and spins a small effect by one frame. */
void scFieldEffectUpdateMotionScaleAndSpin(struct ACTOR *actor);

/* A random spin for a field effect, in 1/65536 turns a frame: up to a */
/* thirty-second of a turn either way, as the effect's u16 spin wraps it. */
#define FIELD_EFFECT_RANDOM_SPIN() (syRandomBelow(4096) + (u16)-2048)

#endif
