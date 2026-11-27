#ifndef ALCHEMY_GOLDEN_RING_H
#define ALCHEMY_GOLDEN_RING_H

/* @file */
/* The particles that rise round a glowing actor, in Vale and on the World Map
 */
/* (shared/golden_ring.c). */

#include "types.h"

struct ACTOR;

/* Attaches two rising particles to the actor. */
void scSpawnGoldenRing(struct ACTOR *parent);

#endif
