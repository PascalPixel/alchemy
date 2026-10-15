#ifndef ALCHEMY_ARC_EFFECT_H
#define ALCHEMY_ARC_EFFECT_H

#include "types.h"

struct ArcEffectWork {
  u8 objects[2][80];
  s16 age[2];
  s16 resource;
  s16 group;
  s32 target;
};

void BattleFx_SpawnScaledArcObjects(s32 object);

#endif
