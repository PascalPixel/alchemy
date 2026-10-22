#ifndef ALCHEMY_UNIT_SCENE_BABI_LIGHTHOUSE_H
#define ALCHEMY_UNIT_SCENE_BABI_LIGHTHOUSE_H

#include "types.h"
#include "field_scene.h"
#include "field_runtime.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *BabiLighthouse_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 BabiLighthouse_SetupScene(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gBabiLighthouse_TrembleScript[];
extern struct SceneEntrance gBabiLighthouse_EntryEntrances[];
extern struct SceneEntrance gBabiLighthouse_LuffEntrances[];
extern struct SceneEntrance gBabiLighthouse_StoneDoorEntrances[];
extern struct SceneEntrance gBabiLighthouse_RoadEntrances[];
extern struct SceneRegion gBabiLighthouse_EntryRegions[];
extern u32 gBabiLighthouse_Exits[];
extern struct ScenePlacement gBabiLighthouse_NoPlacements[];
extern struct ScenePlacement gBabiLighthouse_EntryPlacements[];
extern struct ScenePlacement gBabiLighthouse_LuffPlacements[];
extern struct ScenePlacement gBabiLighthouse_StoneDoorPlacements[];
extern struct ScenePlacement gBabiLighthouse_RoadPlacements[];
extern struct SceneEvent gBabiLighthouse_EntryEvents[];
extern struct SceneEvent gBabiLighthouse_LuffEvents[];
extern struct SceneEvent gBabiLighthouse_StoneDoorEvents[];
extern struct SceneEvent gBabiLighthouse_RoadEvents[];

#endif
