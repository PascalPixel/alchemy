#ifndef ALCHEMY_MODULE_SCENE_MIKASALLA_INTERIORS_H
#define ALCHEMY_MODULE_SCENE_MIKASALLA_INTERIORS_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern struct ActorPlacement gMikasallaInteriors_InnPlacements[];
extern struct ActorPlacement gMikasallaInteriors_SanctumPlacements[];
extern struct Trigger gMikasallaInteriors_InnTriggers[];
extern struct Trigger gMikasallaInteriors_SanctumTriggers[];

void Shop_Run();

void MikasallaInteriors_FadeInAndClearFlag(void);

extern u32 gMikasallaInteriors_ExplorersWidowPaces[];
extern u32 gMikasallaInteriors_ClimbersWifePaces[];

#endif
