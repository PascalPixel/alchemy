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
s32 ValeCave_RunSceneScript(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gValeCave_WalkTargets[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gValeCaveEntrancesOther[];
extern struct SceneEntrance gValeCaveEntrances1[];
extern struct SceneEntrance gValeCaveEntrances2[];
extern struct SceneEntrance gValeCaveEntrances3[];
extern u32 gValeCave_Exits[];
extern struct ScenePlacement gValeCavePlacements1[];
extern struct ScenePlacement gValeCavePlacements2[];
extern struct ScenePlacement gValeCavePlacements3[];
extern struct SceneEvent gValeCaveEventsOther[];
extern struct SceneEvent gValeCaveEvents1[];
extern struct SceneEvent gValeCaveEvents2[];
extern struct SceneEvent gValeCaveEvents3[];

#endif
