#ifndef ALCHEMY_UNIT_SCENE_MOGALL_FOREST_H
#define ALCHEMY_UNIT_SCENE_MOGALL_FOREST_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *MogallForest_GetEntrances(void);
const struct SceneRegion *MogallForest_GetRegions(void);
const u32 *MogallForest_GetExits(void);
const struct ScenePlacement *MogallForest_GetPlacements(void);
const struct SceneEvent *MogallForest_GetEvents(void);
s32 MogallForest_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gMogallForest_RockScript[];
extern s32 gMogallForest_ActorScript[];
extern s32 gMogallForest_HidingPlaceScript[];
extern struct SceneEntrance gMogallForestSign_Entrances[];
extern struct SceneEntrance gMogallForestQuartz_Entrances[];
extern struct SceneEntrance gMogallForestKillerApe_Entrances[];
extern struct SceneEntrance gMogallForestSidePaths_Entrances[];
extern u32 gMogallForest_Exits[];
extern struct ScenePlacement gMogallForestSign_Placements[];
extern struct ScenePlacement gMogallForestQuartz_Placements[];
extern struct ScenePlacement gMogallForestKillerApe_Placements[];
extern struct ScenePlacement gMogallForestSidePaths_Placements[];
extern struct SceneEvent gMogallForestSign_Events[];
extern struct SceneEvent gMogallForestQuartz_Events[];
extern struct SceneEvent gMogallForestKillerApe_Events[];
extern struct SceneEvent gMogallForestSidePaths_Events[];

#endif
