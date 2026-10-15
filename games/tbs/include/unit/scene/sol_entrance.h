#ifndef ALCHEMY_UNIT_SCENE_SOL_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_SOL_ENTRANCE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gSolEntranceEntrancesOther[];
extern struct SceneEntrance gSolEntranceEntrances2[];
extern struct SceneEntrance gSolEntranceEntrances1[];
extern u32 SolEntrance_Exits[];
extern struct ScenePlacement gSolEntrancePlacementsOther[];
extern struct ScenePlacement gSolEntrancePlacements1[];
extern struct ScenePlacement gSolEntrancePlacements1Entrances11To13[];
extern struct ScenePlacement gSolEntrancePlacements1Entrances14To16[];
extern struct ScenePlacement gSolEntrancePlacements2[];
extern struct SceneEvent gSolEntranceEventsOther[];
extern struct SceneEvent gSolEntranceEvents2[];
extern struct SceneEvent gSolEntranceEvents1[];
extern struct SceneEvent gSolEntranceEvents1Entrances11To13[];
extern struct SceneEvent gSolEntranceEvents1Entrances14To16[];
extern s32 gSolPushSteps[];

#endif
