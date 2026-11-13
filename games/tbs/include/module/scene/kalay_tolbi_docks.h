#ifndef ALCHEMY_MODULE_SCENE_KALAY_TOLBI_DOCKS_H
#define ALCHEMY_MODULE_SCENE_KALAY_TOLBI_DOCKS_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern u16 gKalayTolbiDocks_KalayDoorCells[];
extern u16 gKalayTolbiDocks_TolbiDoorCells[];
extern s32 gKalayTolbiDocks_BodyguardSpinScript[];
extern struct SceneEntrance gKalayTolbiDocks_NoEntrances[];
extern struct SceneEntrance gKalayTolbiDocks_KalayEntrances[];
extern struct SceneEntrance gKalayTolbiDocks_TolbiEntrances[];
extern struct SceneEntrance gKalayTolbiDocks_RoomEntrances[];
extern u32 gKalayTolbiDocks_Exits[];
extern struct ActorPlacement gKalayTolbiDocks_NoActorPlacements[];
extern struct ActorPlacement gKalayTolbiDocks_TolbiActorPlacements[];
extern struct ActorPlacement gKalayTolbiDocks_TolbiActorPlacementsStranded[];
extern struct ActorPlacement gKalayTolbiDocks_KalayActorPlacements[];
extern struct ActorPlacement gKalayTolbiDocks_KalayActorPlacementsShipGone[];
extern struct ActorPlacement gKalayTolbiDocks_RoomActorPlacements[];
extern struct ActorPlacement gKalayTolbiDocks_RoomActorPlacementsShipGone[];
extern struct ActorPlacement gKalayTolbiDocks_RoomActorPlacementsStranded[];
extern struct Trigger gKalayTolbiDocks_NoTriggers[];
extern struct Trigger gKalayTolbiDocks_KalayTriggers[];
extern struct Trigger gKalayTolbiDocks_KalayTriggersShipGone[];
extern struct Trigger gKalayTolbiDocks_TolbiTriggers[];
extern struct Trigger gKalayTolbiDocks_TolbiTriggersStranded[];
extern struct Trigger gKalayTolbiDocks_RoomTriggers[];
extern struct Trigger gKalayTolbiDocks_RoomTriggersShipGone[];
extern struct Trigger gKalayTolbiDocks_RoomTriggersStranded[];

#endif
