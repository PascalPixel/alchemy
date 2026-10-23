#ifndef ALCHEMY_UNIT_SCENE_KALAY_TOLBI_DOCKS_H
#define ALCHEMY_UNIT_SCENE_KALAY_TOLBI_DOCKS_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *KalayTolbiDocks_GetEntrances(void);
const struct SceneRegion *KalayTolbiDocks_GetRegions(void);
const u32 *KalayTolbiDocks_GetExits(void);
const struct ScenePlacement *KalayTolbiDocks_GetPlacements(void);
const struct SceneEvent *KalayTolbiDocks_GetEvents(void);
s32 KalayTolbiDocks_Initialize(void);
extern u16 gKalayTolbiDocks_KalayDoorCells[];
extern u16 gKalayTolbiDocks_TolbiDoorCells[];
extern s32 gKalayTolbiDocks_BodyguardSpinScript[];
extern struct SceneEntrance gKalayTolbiDocks_NoEntrances[];
extern struct SceneEntrance gKalayTolbiDocks_KalayEntrances[];
extern struct SceneEntrance gKalayTolbiDocks_TolbiEntrances[];
extern struct SceneEntrance gKalayTolbiDocks_InteriorEntrances[];
extern u32 gKalayTolbiDocks_Exits[];
extern struct ScenePlacement gKalayTolbiDocks_NoPlacements[];
extern struct ScenePlacement gKalayTolbiDocks_TolbiPlacements[];
extern struct ScenePlacement gKalayTolbiDocks_TolbiPlacementsStranded[];
extern struct ScenePlacement gKalayTolbiDocks_KalayPlacements[];
extern struct ScenePlacement gKalayTolbiDocks_KalayPlacementsShipGone[];
extern struct ScenePlacement gKalayTolbiDocks_InteriorPlacements[];
extern struct ScenePlacement gKalayTolbiDocks_InteriorPlacementsShipGone[];
extern struct ScenePlacement gKalayTolbiDocks_InteriorPlacementsStranded[];
extern struct SceneEvent gKalayTolbiDocks_NoEvents[];
extern struct SceneEvent gKalayTolbiDocks_KalayEvents[];
extern struct SceneEvent gKalayTolbiDocks_KalayEventsShipGone[];
extern struct SceneEvent gKalayTolbiDocks_TolbiEvents[];
extern struct SceneEvent gKalayTolbiDocks_TolbiEventsStranded[];
extern struct SceneEvent gKalayTolbiDocks_InteriorEvents[];
extern struct SceneEvent gKalayTolbiDocks_InteriorEventsShipGone[];
extern struct SceneEvent gKalayTolbiDocks_InteriorEventsStranded[];

#endif
