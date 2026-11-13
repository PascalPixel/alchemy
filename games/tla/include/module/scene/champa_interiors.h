#ifndef ALCHEMY_MODULE_SCENE_CHAMPA_INTERIORS_H
#define ALCHEMY_MODULE_SCENE_CHAMPA_INTERIORS_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern struct ActorPlacement gChampaInteriors_ActorPlacements[];
extern struct ActorPlacement gChampaInteriors_IsletHouseActorPlacements[];
extern struct Trigger gChampaInteriors_Triggers[];
extern struct Trigger gChampaInteriors_TriggersOnceBriggsSailed[];
extern struct Trigger gChampaInteriors_IsletHouseTriggers[];

void Scene_FadeInAndClearFlag(void);

#endif
