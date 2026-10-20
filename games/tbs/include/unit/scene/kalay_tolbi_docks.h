#ifndef ALCHEMY_UNIT_SCENE_KALAY_TOLBI_DOCKS_H
#define ALCHEMY_UNIT_SCENE_KALAY_TOLBI_DOCKS_H

#include "types.h"
#include "field_scene.h"

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
extern struct SceneEntrance gKalayTolbiDocks_TicketOfficeEntrances[];
extern u32 gKalayTolbiDocks_Exits[];
extern struct ScenePlacement gKalayTolbiDocks_NoPlacements[];
extern struct ScenePlacement gKalayTolbiDocks_TolbiPlacements[];
extern struct ScenePlacement gKalayTolbiDocks_TolbiPlacementsStranded[];
extern struct ScenePlacement gKalayTolbiDocks_KalayPlacements[];
extern struct ScenePlacement gKalayTolbiDocks_KalayPlacementsShipGone[];
extern struct ScenePlacement gKalayTolbiDocks_TicketOfficePlacements[];
extern struct ScenePlacement gKalayTolbiDocks_TicketOfficePlacementsShipGone[];
extern struct ScenePlacement gKalayTolbiDocks_TicketOfficePlacementsStranded[];
extern struct SceneEvent gKalayTolbiDocks_NoEvents[];
extern struct SceneEvent gKalayTolbiDocks_KalayEvents[];
extern struct SceneEvent gKalayTolbiDocks_KalayEventsShipGone[];
extern struct SceneEvent gKalayTolbiDocks_TolbiEvents[];
extern struct SceneEvent gKalayTolbiDocks_TolbiEventsStranded[];
extern struct SceneEvent gKalayTolbiDocks_TicketOfficeEvents[];
extern struct SceneEvent gKalayTolbiDocks_TicketOfficeEventsShipGone[];
extern struct SceneEvent gKalayTolbiDocks_TicketOfficeEventsStranded[];

#endif
