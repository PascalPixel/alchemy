#ifndef ALCHEMY_UNIT_SCENE_SOL_SANCTUM_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_SOL_SANCTUM_ENTRANCE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *SolSanctumEntrance_GetEntrances(void);
const struct SceneRegion *SolSanctumEntrance_GetRegions(void);
const u32 *SolSanctumEntrance_GetExits(void);
const struct ScenePlacement *SolSanctumEntrance_GetPlacements(void);
const struct SceneEvent *SolSanctumEntrance_GetEvents(void);
s32 SolSanctumEntrance_Initialize(void);
extern struct SceneEntrance gSolSanctumEntranceEntrancesOther[];
extern struct SceneEntrance gSolSanctumEntranceEntrances2[];
extern struct SceneEntrance gSolSanctumEntranceEntrances1[];
extern u32 gSolSanctumEntrance_Exits[];
extern struct ScenePlacement gSolSanctumEntrancePlacementsOther[];
extern struct ScenePlacement gSolSanctumEntrancePlacements1[];
extern struct ScenePlacement gSolSanctumEntrancePlacements1Entrances11To13[];
extern struct ScenePlacement gSolSanctumEntrancePlacements1Entrances14To16[];
extern struct ScenePlacement gSolSanctumEntrancePlacements2[];
extern struct SceneEvent gSolSanctumEntranceEventsOther[];
extern struct SceneEvent gSolSanctumEntranceEvents2[];
extern struct SceneEvent gSolSanctumEntranceEvents1[];
extern struct SceneEvent gSolSanctumEntranceEvents1Entrances11To13[];
extern struct SceneEvent gSolSanctumEntranceEvents1Entrances14To16[];
extern s32 gSolSanctumPushSteps[];

#endif
