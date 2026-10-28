#ifndef ALCHEMY_UNIT_MAP_KALAY_TOLBI_DOCKS_H
#define ALCHEMY_UNIT_MAP_KALAY_TOLBI_DOCKS_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *KalayTolbiDocks_GetEntrances(void);
const struct SceneExitRegion *KalayTolbiDocks_GetRegions(void);
const u32 *KalayTolbiDocks_GetExits(void);
const struct ActorPlacement *KalayTolbiDocks_GetActorPlacements(void);
const struct Trigger *KalayTolbiDocks_GetTriggers(void);
s32 KalayTolbiDocks_Initialize(void);
extern u16 gKalayTolbiDocks_KalayDoorCells[];
extern u16 gKalayTolbiDocks_TolbiDoorCells[];
extern s32 gKalayTolbiDocks_BodyguardSpinScript[];
extern struct SceneEntrance gKalayTolbiDocks_NoEntrances[];
extern struct SceneEntrance gKalayTolbiDocks_KalayEntrances[];
extern struct SceneEntrance gKalayTolbiDocks_TolbiEntrances[];
extern struct SceneEntrance gKalayTolbiDocks_InteriorEntrances[];
extern u32 gKalayTolbiDocks_Exits[];
extern struct ActorPlacement gKalayTolbiDocks_NoActorPlacements[];
extern struct ActorPlacement gKalayTolbiDocks_TolbiActorPlacements[];
extern struct ActorPlacement gKalayTolbiDocks_TolbiActorPlacementsStranded[];
extern struct ActorPlacement gKalayTolbiDocks_KalayActorPlacements[];
extern struct ActorPlacement gKalayTolbiDocks_KalayActorPlacementsShipGone[];
extern struct ActorPlacement gKalayTolbiDocks_InteriorActorPlacements[];
extern struct ActorPlacement gKalayTolbiDocks_InteriorActorPlacementsShipGone[];
extern struct ActorPlacement gKalayTolbiDocks_InteriorActorPlacementsStranded[];
extern struct Trigger gKalayTolbiDocks_NoTriggers[];
extern struct Trigger gKalayTolbiDocks_KalayTriggers[];
extern struct Trigger gKalayTolbiDocks_KalayTriggersShipGone[];
extern struct Trigger gKalayTolbiDocks_TolbiTriggers[];
extern struct Trigger gKalayTolbiDocks_TolbiTriggersStranded[];
extern struct Trigger gKalayTolbiDocks_InteriorTriggers[];
extern struct Trigger gKalayTolbiDocks_InteriorTriggersShipGone[];
extern struct Trigger gKalayTolbiDocks_InteriorTriggersStranded[];

#endif
