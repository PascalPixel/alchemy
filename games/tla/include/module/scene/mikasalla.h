#ifndef ALCHEMY_MODULE_SCENE_MIKASALLA_H
#define ALCHEMY_MODULE_SCENE_MIKASALLA_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern s16 gMikasalla_TunnelRegions[];
extern struct ActorPlacement gMikasalla_VillagePlacements[];
extern struct ActorPlacement gMikasalla_TunnelPlacements[];

extern struct Trigger gMikasalla_TunnelTriggers[];

extern struct Trigger gMikasalla_VillageTriggers[];
extern s16 gMikasalla_DoorOpening[];
extern s16 gMikasalla_ThirdDoorOpening[];
extern s16 gMikasalla_WideDoorOpening[];
extern s32 gMikasalla_ChickenPaces[];
extern s32 gMikasalla_SheepPaces[];

#endif
