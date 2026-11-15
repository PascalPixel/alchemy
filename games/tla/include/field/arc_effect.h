#ifndef ALCHEMY_ARC_EFFECT_H
#define ALCHEMY_ARC_EFFECT_H

#include "types.h"

/// A field Psynergy's arc work, the block of heap slot HEAP_SLOT_ARC_EFFECT:
/// two runs of arc objects, their ages, and the resource, group and target
/// they were spawned with (BattleEffect_SpawnScaledArcObjects).
struct ArcEffectWork {
  u8 objects[2][80];
  s16 age[2];
  s16 resource;
  s16 group;
  s32 target;
};

struct Actor;

void BattleEffect_SpawnScaledArcObjects(struct Actor *target);

#endif
