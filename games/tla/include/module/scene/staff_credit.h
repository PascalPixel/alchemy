#ifndef ALCHEMY_MODULE_SCENE_STAFF_CREDIT_H
#define ALCHEMY_MODULE_SCENE_STAFF_CREDIT_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;

extern u32 gStaffCredit_ColorModes[];
void StaffCredit_SpawnGrowingPair(struct Actor *object);

void WorldMap_UpdatePlacedActors(void);

void WorldMap_StartPlacedActors(void);

void WorldMap_SpawnSparkPair(struct Actor *object);
void WorldMap_SpawnParticle(struct Actor *object);

#endif
