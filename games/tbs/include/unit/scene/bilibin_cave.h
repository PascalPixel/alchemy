#ifndef ALCHEMY_UNIT_SCENE_BILIBIN_CAVE_H
#define ALCHEMY_UNIT_SCENE_BILIBIN_CAVE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *BiribinoDou_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gBiribinoDouPlacements1_Data[];
extern struct SceneEntrance gBiribinoDouEntrancesOther[];
extern struct SceneEntrance gBiribinoDouEntrances3[];
extern struct SceneEntrance gBiribinoDouEntrances2[];
extern struct SceneEntrance gBiribinoDouEntrances1[];
extern u32 gBiribinoDouExits[];
extern struct ScenePlacement gBiribinoDouPlacementsOther[];
extern struct ScenePlacement gBiribinoDouPlacements3[];
extern struct ScenePlacement gBiribinoDouPlacements2[];
extern struct ScenePlacement gBiribinoDouPlacements1[];
extern struct SceneEvent gBiribinoDouEventsOther[];
extern struct SceneEvent gBiribinoDouEvents3[];
extern struct SceneEvent gBiribinoDouEvents2[];
extern struct SceneEvent gBiribinoDouEvents1[];
extern s32 StagedActor_DirectionSteps[];

#endif
