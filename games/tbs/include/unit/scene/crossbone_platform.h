#ifndef ALCHEMY_UNIT_SCENE_CROSSBONE_PLATFORM_H
#define ALCHEMY_UNIT_SCENE_CROSSBONE_PLATFORM_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance  *Scene_GetEntrances(void);
s32                          SceneData_ReturnZero(void);
u8                          *SceneData_GetSceneTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          Scene_Initialize(void);
extern s32                   gStagedActor_DirectionSteps[];
extern u32                   gStagedActor_FootprintKinds[];
extern s32                   gStagedActor_FootprintBounds[];
extern s32                   gCrossbonePlatform_ActorFifteenMotionScript[];
extern s32                   gCrossbonePlatform_ActorFifteenWalkTargets[];
extern struct SceneEntrance  gCrossbonePlatformEntrancesOther[];
extern struct SceneEntrance  gCrossbonePlatformEntrances1[];
extern struct SceneEntrance  gCrossbonePlatformEntrances2[];
extern struct SceneEntrance  gCrossbonePlatformEntrances3[];
extern u32                   gCrossbonePlatform_SceneTable[];
extern struct ScenePlacement gCrossbonePlatformPlacementsOther[];
extern struct ScenePlacement gCrossbonePlatformPlacements1[];
extern struct ScenePlacement gCrossbonePlatformPlacements2[];
extern struct ScenePlacement gCrossbonePlatformPlacements3[];
extern struct SceneEvent     gCrossbonePlatformEventsOther[];
extern struct SceneEvent     gCrossbonePlatformEvents1[];
extern struct SceneEvent     gCrossbonePlatformEvents2[];
extern struct SceneEvent     gCrossbonePlatformEvents3[];
extern u32                   gCrossbonePlatform_SlotColumns[];
extern s32                   gCrossbonePlatform_IconTimer;

#endif
