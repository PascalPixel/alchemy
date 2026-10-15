#ifndef ALCHEMY_UNIT_SCENE_MOGALL_FOREST_H
#define ALCHEMY_UNIT_SCENE_MOGALL_FOREST_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *MogallForest_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 FieldScene_RunSceneEntryHook(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gMogallForestPlacements2_0[];
extern s32 MogallForest_ActorScript[];
extern s32 gMogallForestPlacements1_1[];
extern struct SceneEntrance gMogallForestEntrances1[];
extern struct SceneEntrance gMogallForestEntrances2[];
extern struct SceneEntrance gMogallForestEntrances3[];
extern struct SceneEntrance gMogallForestEntrancesOther[];
extern u32 MogallForest_SceneTable[];
extern struct ScenePlacement gMogallForestPlacements1[];
extern struct ScenePlacement gMogallForestPlacements2[];
extern struct ScenePlacement gMogallForestPlacements3[];
extern struct ScenePlacement gMogallForestPlacementsOther[];
extern struct SceneEvent gMogallForestEvents1[];
extern struct SceneEvent gMogallForestEvents2[];
extern struct SceneEvent gMogallForestEvents3[];
extern struct SceneEvent gMogallForestEventsOther[];

#endif
