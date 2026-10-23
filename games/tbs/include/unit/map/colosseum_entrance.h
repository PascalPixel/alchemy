#ifndef ALCHEMY_UNIT_SCENE_COLOSSEUM_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_COLOSSEUM_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *ColosseumEntrance_GetEntrances(void);
const struct SceneRegion *ColosseumEntrance_GetRegions(void);
const u32 *ColosseumEntrance_GetExits(void);
const struct ScenePlacement *ColosseumEntrance_GetPlacements(void);
const struct SceneEvent *ColosseumEntrance_GetEvents(void);
s32 ColosseumEntrance_Initialize(void);
extern s32 gColosseumEntrance_GatherIntoIsaacScript[];
extern s32 gColosseumEntrance_GatherIntoGaretScript[];
extern s32 gColosseumEntrance_RejoinPartyScript[];
extern s32 gColosseumEntrance_EnterGatesScript[];
extern s32 gColosseumEntrance_ApproachFinalsGroupScript[];
extern s32 gColosseumEntrance_JoinFinalsFromEastScript[];
extern s32 gColosseumEntrance_JoinFinalsFromSouthScript[];
extern s32 gColosseumEntrance_EnterFinalsScript[];
extern s32 gColosseumEntrance_PulseScaleScript[];
extern struct SceneEntrance gColosseumEntranceEntrancesOther[];
extern struct SceneEntrance gColosseumEntrances[];
extern struct SceneEntrance gColosseumEntranceEntrances[];
extern u32 gColosseumEntranceExits[];
extern struct ScenePlacement gColosseumEntrancePlacementsOther[];
extern struct ScenePlacement gColosseumStandsPlacements[];
extern struct ScenePlacement gColosseumTrialGroundsPlacements[];
extern struct ScenePlacement gColosseumPlacements[];
extern struct ScenePlacement gColosseumSoldiersPlacements[];
extern struct ScenePlacement gColosseumPlacementsFromColosseumAzartMatchWest[];
extern struct ScenePlacement gColosseumPlacementsFromColosseumNavampaMatch[];
extern struct ScenePlacement gColosseumEntrancePlacements[];
extern struct ScenePlacement gColosseumEntrancePlacementsFinalsDay[];
extern struct ScenePlacement gColosseumEntrancePlacementsAfterColosso[];
extern struct SceneEvent gColosseumEntranceEventsOther[];
extern struct SceneEvent gColosseumTunnelEvents[];
extern struct SceneEvent gColosseumEvents[];
extern struct SceneEvent gColosseumEntranceEvents[];
extern struct SceneEvent gColosseumEventsFromColosseumAzartMatchWest[];

#endif
