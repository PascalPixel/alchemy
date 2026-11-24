#ifndef ALCHEMY_GOLDEN_RING_H
#define ALCHEMY_GOLDEN_RING_H

/// @file
/// The particles that rise round a glowing actor, in Vale and on the World Map
/// (shared/golden_ring.c).

#include "types.h"

struct Actor;

/// Attaches two rising particles to the actor.
void Scene_SpawnGoldenRing(struct Actor *parent);

#endif
