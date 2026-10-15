#ifndef ALCHEMY_UNIT_SCENE_VALE_CAVE_H
#define ALCHEMY_UNIT_SCENE_VALE_CAVE_H

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
s32 HaidiaDou_RunSceneScript(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 HaidiaDou_WalkTargets[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gHaidiaDouEntrancesOther[];
extern struct SceneEntrance gHaidiaDouEntrances1[];
extern struct SceneEntrance gHaidiaDouEntrances2[];
extern struct SceneEntrance gHaidiaDouEntrances3[];
extern u32 HaidiaDou_Exits[];
extern struct ScenePlacement gHaidiaDouPlacements1[];
extern struct ScenePlacement gHaidiaDouPlacements2[];
extern struct ScenePlacement gHaidiaDouPlacements3[];
extern struct SceneEvent gHaidiaDouEventsOther[];
extern struct SceneEvent gHaidiaDouEvents1[];
extern struct SceneEvent gHaidiaDouEvents2[];
extern struct SceneEvent gHaidiaDouEvents3[];

#endif
