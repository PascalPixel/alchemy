#ifndef ALCHEMY_UNIT_SCENE_COMMON_MERCURY_LIGHTHOUSE_LIGHTS_H
#define ALCHEMY_UNIT_SCENE_COMMON_MERCURY_LIGHTHOUSE_LIGHTS_H

#include "types.h"
#include "static_assert.h"

struct FieldActor;

// Published in the scene's light-controller slot while the pillar is active.
struct MercuryLighthouseLights {
  s32 levels;
  s32 terrainCode;
  s32 sparkTimer;
  s32 targetX;
  s32 targetZ;
  struct FieldActor *pillar;
  struct FieldActor *stepEffect;
};

STATIC_ASSERT(sizeof(struct MercuryLighthouseLights) == 0x1c);

void MercuryLighthouse_SpawnLightObjects(s32 region,
                                         struct MercuryLighthouseLights *st);

#endif
