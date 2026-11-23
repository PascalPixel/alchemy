#ifndef ALCHEMY_WORLD_MAP_ATTACHED_EFFECTS_H
#define ALCHEMY_WORLD_MAP_ATTACHED_EFFECTS_H

/// @file
/// The particles that rise round a glowing actor, in Vale and on the World Map
/// (shared/world_map_attached_effects.c).

#include "types.h"

struct Actor;

/// Attaches two rising particles to the actor.
void WorldMap_CreateAttachedEffects(struct Actor *parent);

#endif
