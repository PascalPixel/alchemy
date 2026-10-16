#ifndef ALCHEMY_UNIT_SCENE_COLOSSEUM_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_COLOSSEUM_ENTRANCE_H

#include "types.h"
#include "field_scene.h"

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
extern s32 gColosseumEntrance_GatherAtSpectatorLeaderScript[];
extern s32 gColosseumEntrance_GatherAtSpectatorPartyScript[];
extern s32 gColosseumEntrance_RejoinPartyScript[];
extern s32 gColosseumEntrance_LeaveSpectatorAreaScript[];
extern s32 gColosseumEntrance_ApproachFinalsGroupScript[];
extern s32 gColosseumEntrance_JoinFinalsFromEastScript[];
extern s32 gColosseumEntrance_JoinFinalsFromSouthScript[];
extern s32 gColosseumEntrance_EnterFinalsScript[];
extern s32 gColosseumEntrance_PulseScaleScript[];
extern struct SceneEntrance gColosseumEntranceEntrancesOther[];
extern struct SceneEntrance gColosseumEntranceEntrances1[];
extern struct SceneEntrance gColosseumEntranceEntrances3[];
extern u32 gColosseumEntranceExits[];
extern struct ScenePlacement gColosseumEntrancePlacementsOther[];
extern struct ScenePlacement gColosseumEntrancePlacements1Entrance5[];
extern struct ScenePlacement gColosseumEntrancePlacements1Entrance7[];
extern struct ScenePlacement gColosseumEntrancePlacements1[];
extern struct ScenePlacement gColosseumEntrancePlacements1Entrance8[];
extern struct ScenePlacement gColosseumEntrancePlacements1Entrance12[];
extern struct ScenePlacement gColosseumEntrancePlacements1Entrance66[];
extern struct ScenePlacement gColosseumEntrancePlacements3[];
extern struct ScenePlacement gColosseumEntrancePlacements3Flag962[];
extern struct ScenePlacement gColosseumEntrancePlacements3Flag950[];
extern struct SceneEvent gColosseumEntranceEventsOther[];
extern struct SceneEvent gColosseumEntranceEvents2[];
extern struct SceneEvent gColosseumEntranceEvents1[];
extern struct SceneEvent gColosseumEntranceEvents3[];
extern struct SceneEvent gColosseumEntranceEvents1Entrance12[];

#endif
