#ifndef ALCHEMY_ARC_EFFECT_H
#define ALCHEMY_ARC_EFFECT_H

/// @file
/// The swelling arcs a field Psynergy shows over its target.

#include "types.h"
#include "field/effect_sprite.h"

struct Actor;

/// Frames a pair of swelling arcs shows for, rising a pixel a frame.
#define ARC_GROUP_FRAMES 32

/// A pair's age before it is first spawned: past ARC_GROUP_FRAMES, so not
/// drawn.
#define ARC_GROUP_UNSPAWNED 999

/// A field Psynergy's arc state, the block of heap slot HEAP_SLOT_ARC_EFFECT:
/// two groups of two arc sprites, their ages, and the VRAM cache entry, group
/// and target they were spawned with (BattleEffect_SpawnScaledArcObjects).
struct ArcEffectState {
  struct EffectSprite sprites[2][2];
  s16 age[2];   ///< frames since each group was spawned; ARC_GROUP_UNSPAWNED
                ///< before
  s16 resource; ///< VRAM cache entry holding the arcs' tiles
  s16 group;    ///< the group spawned next
  struct Actor *target; ///< the actor the last group was spawned over
};

void BattleEffect_SpawnScaledArcObjects(struct Actor *target);

#endif
