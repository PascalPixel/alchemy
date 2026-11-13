#ifndef ALCHEMY_MODULE_SCENE_STAFF_CREDIT_H
#define ALCHEMY_MODULE_SCENE_STAFF_CREDIT_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;

extern u32 gData_02002eac[];
void Func_02001ad0(struct Actor *object);

void Func_0200200c(s32 value);

void Func_02001f7c(u32 *oam, s32 priority);

void WorldMap_UpdatePlacedActors(void);

void WorldMap_StartPlacedActors(void);

void WorldMap_SpawnSparkPair(struct Actor *object);
void WorldMap_SpawnParticle(struct Actor *object);

#endif
