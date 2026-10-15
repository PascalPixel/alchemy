#ifndef ALCHEMY_UNIT_SCENE_CROSSBONE_PLATFORM_H
#define ALCHEMY_UNIT_SCENE_CROSSBONE_PLATFORM_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetSceneTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 TakaraAshiba_ActorFifteenMotionScript[];
extern s32 TakaraAshiba_ActorFifteenWalkTargets[];
extern struct SceneEntrance gTakaraAshibaEntrancesOther[];
extern struct SceneEntrance gTakaraAshibaEntrances1[];
extern struct SceneEntrance gTakaraAshibaEntrances2[];
extern struct SceneEntrance gTakaraAshibaEntrances3[];
extern u32 TakaraAshiba_SceneTable[];
extern struct ScenePlacement gTakaraAshibaPlacementsOther[];
extern struct ScenePlacement gTakaraAshibaPlacements1[];
extern struct ScenePlacement gTakaraAshibaPlacements2[];
extern struct ScenePlacement gTakaraAshibaPlacements3[];
extern struct SceneEvent gTakaraAshibaEventsOther[];
extern struct SceneEvent gTakaraAshibaEvents1[];
extern struct SceneEvent gTakaraAshibaEvents2[];
extern struct SceneEvent gTakaraAshibaEvents3[];
extern u32 TakaraAshiba_SlotColumns[];
extern s32 TakaraAshiba_IconTimer;

#endif
