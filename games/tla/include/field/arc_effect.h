#ifndef ALCHEMY_ARC_EFFECT_H
#define ALCHEMY_ARC_EFFECT_H

#include "types.h"
#include "field/effect_sprite.h"

struct Actor;

/// A field Psynergy's arc work, the block of heap slot HEAP_SLOT_ARC_EFFECT:
/// two groups of two arc sprites, their ages, and the VRAM cache entry, group
/// and target they were spawned with (BattleEffect_SpawnScaledArcObjects).
struct ArcEffectWork {
  struct EffectSprite sprites[2][2];
  s16 age[2];           ///< frames since each group was spawned; 999 before
  s16 resource;         ///< VRAM cache entry holding the arcs' tiles
  s16 group;            ///< the group spawned next
  struct Actor *target; ///< the actor the last group was spawned over
};

void BattleEffect_SpawnScaledArcObjects(struct Actor *target);

#endif
